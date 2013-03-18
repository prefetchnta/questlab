//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "QstText.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
//---------------------------------------------------------------------------

/* 全局工作上下文 */
static sQstText     s_wrk_ctx;

/* 当前工作状态设置宏 */
#define QST_SET_STATE_FREE \
    ((TfrmMain*)(ctx->form))->Caption = WIN_TITLE;
#define QST_SET_STATE_BUSY \
    ((TfrmMain*)(ctx->form))->Caption = WIN_TITLE " - Loading...";

/*
---------------------------------------
    释放信息列表
---------------------------------------
*/
static void_t
finfo_free (
  __CR_IN__ void_t* obj
    )
{
    ansi_t**    unit;

    unit = (ansi_t**)obj;
    mem_free(*unit);
}

/*
---------------------------------------
    FMTZ 插件释放回调
---------------------------------------
*/
static void_t
plugin_free (
  __CR_IN__ void_t* obj
    )
{
    sENGINE**   unit;

    unit = (sENGINE**)obj;
    engine_free(*unit);
}

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/*
---------------------------------------
    读入配置文件
---------------------------------------
*/
static void_t
qst_load_cfg (
  __CR_OT__ sQEDT_conf* cfgs
    )
{
    sINIu*  ini;
    ansi_t* str;
    sint_t  size;

    /* 加载配置文件 */
    TRY_FREE(cfgs->font_face)
    str = file_load_as_strA(QST_PATH_CONFIG WIN_ICONF);
    if (str == NULL)
        goto _load_defs;
    ini = ini_parseU(str);
    mem_free(str);
    if (ini == NULL)
        goto _load_defs;

    /* 读入配置参数 */
    cfgs->font_size = ini_key_intxU("qedt::font_size", 12, ini);
    cfgs->font_face = ini_key_stringU("qedt::font_face", ini);
    if (cfgs->font_face != NULL) {
        str = cfgs->font_face;
        cfgs->font_face = utf8_to_local(CR_LOCAL, str);
        mem_free(str);
    }
    size = ini_key_intxU("qedt::max_size_kb", 20480, ini);
    if (size < 0) size = 0;
    cfgs->max_fsize = size;
    cfgs->max_fsize *= 1024;
    ini_closeU(ini);
    return;

_load_defs:
    cfgs->font_size = 12;
    cfgs->font_face = NULL;
    cfgs->max_fsize = 20480;
    cfgs->max_fsize *= 1024;
}

/*
---------------------------------------
    刷新编辑器设置
---------------------------------------
*/
static void_t
qst_set_editor (
  __CR_IO__ sQstText*   parm
    )
{
    sptr_t      val;
    TfrmMain*   frm;
    sQEDT_conf* cfg;

    /* 在临界区里操作控件 */
    cfg = &parm->cfgs;
    frm = (TfrmMain*)(parm->form);
    crisec_enter(parm->lock);

    /* 设置全局字体 (默认 Fixedsys 字体) */
    sci_call(SCI_STYLESETBACK, STYLE_DEFAULT, 0x00FFFFFF);
    sci_call(SCI_STYLESETFORE, STYLE_DEFAULT, 0x00000000);
    if (cfg->font_face != NULL)
        sci_call(SCI_STYLESETFONT, STYLE_DEFAULT, cfg->font_face);
    else
        sci_call(SCI_STYLESETFONT, STYLE_DEFAULT, "Fixedsys");
    sci_call(SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, SC_CHARSET_DEFAULT);
    sci_call(SCI_STYLESETSIZE, STYLE_DEFAULT, cfg->font_size);
    sci_call(SCI_STYLESETBOLD, STYLE_DEFAULT, FALSE);
    sci_call(SCI_STYLECLEARALL, NULL, NULL);

    /* 设置行号风格 (最大10万行) */
    sci_call(SCI_SETMARGINTYPEN, 0, SC_MARGIN_SYMBOL);
    sci_call(SCI_SETMARGINWIDTHN, 0, 16);
    val = sci_call(SCI_TEXTWIDTH, STYLE_LINENUMBER, "_99999");
    sci_call(SCI_SETMARGINTYPEN, 1, SC_MARGIN_NUMBER);
    sci_call(SCI_SETMARGINWIDTHN, 1, val);
    sci_call(SCI_SETMARGINTYPEN, 2, SC_MARGIN_SYMBOL);
    sci_call(SCI_SETMARGINWIDTHN, 2, 16);

    /* 显示空白字符并只用 UTF-8 编码显示 */
    sci_call(SCI_SETREADONLY, FALSE, NULL);
    sci_call(SCI_SETVIEWWS, SCWS_VISIBLEALWAYS, NULL);
    sci_call(SCI_SETCODEPAGE, SC_CP_UTF8, NULL);

    /* 退出临界区 */
    crisec_leave(parm->lock);
}

/*
---------------------------------------
    释放当前浏览文本
---------------------------------------
*/
static void_t
qst_clear (
  __CR_IO__ sQstText*   parm
    )
{
    TfrmMain*   frm;

    parm->show = NULL;
    parm->xbom = FALSE;
    parm->page = get_sys_codepage();
    SAFE_FREE(parm->text)
    frm = (TfrmMain*)(parm->form);
    frm->edtCPage->Text = IntToStr(parm->page);
    crisec_enter(parm->lock);
    sci_call(SCI_CLEARALL, NULL, NULL);
    crisec_leave(parm->lock);
}

/*
---------------------------------------
    发送文件信息
---------------------------------------
*/
static void_t
qst_send_finfo (
  __CR_IN__ sARRAY*     list,
  __CR_IN__ socket_t    netw
    )
{
    leng_t  cnts, idx;

    /* 清除前面的信息 */
    cmd_shl_send(netw, "txt:clear 0 4");

    /* 逐条发送文件信息 */
    cnts = array_get_sizeT(list, ansi_t*);
    for (idx = 0; idx < cnts; idx++)
    {
        ansi_t*     temp;
        ansi_t*     info;
        ansi_t**    unit;

        /* 转义转换后发给别人 */
        unit = array_get_unitT(list, ansi_t*, idx);
        temp = str_esc_makeU(*unit);
        if (temp == NULL)
            continue;
        info = str_fmtA("info::main=\"4> %s\"", temp);
        mem_free(temp);
        if (info != NULL) {
            cmd_ini_send(netw, info);
            mem_free(info);
        }
    }
}

/*
---------------------------------------
    检查是否是 UTF-8
---------------------------------------
*/
static bool_t
is_utf8_file (
  __CR_IN__ const ansi_t*   str
    )
{
    leng_t  temp;
    int32u  ucs4;

    /* 逐个字符检查 */
    while (*str != NIL) {
        temp = utf8_to_ucs4(&ucs4, str);
        if (temp == 0)
            return (FALSE);
        str += temp;
    }
    return (TRUE);
}

/*
---------------------------------------
    尝试加载目标数据
---------------------------------------
*/
static bool_t
qst_try_load (
  __CR_IO__ sQstText*   parm,
  __CR_IO__ sLOADER*    ldrs
    )
{
    leng_t  idx;
    leng_t  cnts;
    sFMTZ*  fmtz;
    sFMTZ*  tmpz;
    sARRAY  list;
    bool_t  rett;
    bool_t  used;
    bool_t  xbom;
    uint_t  page;
    ansi_t* show;
    ansi_t* info;
    ansi_t* text;
    wide_t* pntr;
    wide_t* ucs2;
    fsize_t size;
    sLOADER copy;

    /* 释放上次加载 */
    qst_clear(parm);

    /* 初始化加载尝试 */
    page = ldrs->page;
    text = show = NULL;
    fmtz = tmpz = NULL;
    rett = xbom = FALSE;
    array_initT(&list, ansi_t*);
    list.free = finfo_free;
    struct_cpy(&copy, ldrs, sLOADER);
    cnts = array_get_sizeT(&parm->extz, sENGINE*);

    sENGINE**   unit;
    sFMT_DAT*   datz;
    sFMT_TXT*   txtz;

    /* 尝试到有结果为止 */
_retry:
    /* 逐个插件试验加载 */
    for (unit = NULL, idx = 0; idx < cnts; idx++) {
        unit = array_get_unitT(&parm->extz, sENGINE*, idx);
        tmpz = engine_do(*unit, &copy);
        if (tmpz != NULL)
            break;
    }

    /* 无法识别的文本文件 */
    if (unit == NULL || tmpz == NULL)
        goto _func_out;

    /* 需要经过二次解码 */
    if (tmpz->type == CR_FMTZ_NOP ||
        tmpz->type == CR_FMTZ_DEC)
    {
        /* 换成内存解析 */
        if (fmtz != NULL)
            fmtz_free(fmtz);
        fmtz = tmpz;
        datz = (sFMT_DAT*)tmpz;
        set_ldrM(&copy, datz->data, datz->unsz, ldrs->aprm, 0, 0);
        copy.page = ldrs->page;

        /* 压入插件信息 */
        info = str_fmtA("Plug-in: %s", (*unit)->info);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入文件信息 */
        info = str_fmtA("+-Decoder: %s", datz->infor);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入大小信息 */
        info = str_fmtA("+---File size: %I64u Bytes / Real size: %u Bytes",
                                    datz->pksz, datz->unsz);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);
        goto _retry;
    }

    /* 得到可以加载的文本文件 */
    if (tmpz->type == CR_FMTZ_TXT)
    {
        txtz = (sFMT_TXT*)tmpz;
        text = txtz->text;

        /* 压入插件信息 */
        info = str_fmtA("Plug-in: %s", (*unit)->info);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入文本信息 */
        info = str_fmtA("+-FormatZ: %s", txtz->infor);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);
    }

_func_out:
    if (text != NULL)
    {
        /* 直接使用插件解码的文本 */
        txtz = (sFMT_TXT*)tmpz;
        text = str_allocA(txtz->size + 8);
        if (text == NULL)
            goto _func_exit;
        mem_cpy(text, txtz->text, txtz->size);
        mem_zero(&text[txtz->size], 8);
        size = txtz->size;
        used = TRUE;
    }
    else
    {
        /* 无法由文本插件加载的通过特定规则匹配 */
        if (copy.type == CR_LDR_ANSI)
            size = file_sizeA(copy.name.ansi);
        else
        if (copy.type == CR_LDR_BUFF)
            size = copy.buff.size;
        else
            goto _func_exit;

        /* 文件大小过滤 */
        if (size == 0 || size > parm->cfgs.max_fsize)
            goto _func_exit;

        /* 文件名匹配过滤 */
        if (parm->count != 0 && parm->flists != NULL)
        {
            /* 去掉文件路径 */
            info = (ansi_t*)ldrs->name.ansi;
            cnts = str_lenA(info);
            while (cnts-- != 0) {
                if (is_slashA(info[cnts])) {
                    info += cnts + 1;
                    break;
                }
            }
            for (idx = 0; idx < parm->count; idx++) {
                if ((parm->flists[idx])[0] == NIL)
                    continue;
                if (wildcard_matchIA(info, parm->flists[idx]))
                    break;
            }
            if (idx >= parm->count)
                goto _func_exit;
        }

        /* 加载文件到内存 */
        if (copy.type == CR_LDR_ANSI) {
            text = file_load_as_strA(copy.name.ansi);
            if (text == NULL)
                goto _func_exit;
        }
        else {
            text = str_allocA(copy.buff.size + 8);
            if (text == NULL)
                goto _func_exit;
            mem_cpy(text, copy.buff.data, copy.buff.size);
            mem_zero(&text[copy.buff.size], 8);
        }
        used = FALSE;
    }

    /* 识别 UTF-16 文本文件 */
    if (size >= 2 && size % 2 == 0)
    {
        /* UTF-16 编码校验过滤 */
        if (mem_cmp(text, BOM_UTF16LE, 2) == 0) {
            ucs2 = (wide_t*)(&text[2]);
            show = utf16_to_utf8(ucs2);
            mem_free(text);
            if (show == NULL)
                goto _func_exit;
            xbom = TRUE;
            text = show;
            page = CR_UTF16LE;
        }
        else
        if (mem_cmp(text, BOM_UTF16BE, 2) == 0) {
            ucs2 = (wide_t*)(&text[2]);
            for (pntr = ucs2; *pntr != NIL; pntr++)
                *pntr = xchg_int16u(*pntr);
            show = utf16_to_utf8(ucs2);
            mem_free(text);
            if (show == NULL)
                goto _func_exit;
            xbom = TRUE;
            text = show;
            page = CR_UTF16BE;
        }
    }

    /* 非 UTF-16 文本文件 */
    if (show == NULL)
    {
        if (!used)
        {
            /* 过滤掉可能的二进制文件 */
            cnts = str_lenA(text);
            if (cnts != size) {
                mem_free(text);
                goto _func_exit;
            }

            fp32_t  ctrl, tots;
            byte_t* comp = (byte_t*)text;

            ctrl = 0.0f;
            tots = (fp32_t)(cnts * 2);
            for (idx = 0; idx < cnts; idx++) {
                if ((*comp == 0xFF) || (*comp < 0x20 &&
                       *comp != 0x09 && *comp != 0x0A &&
                       *comp != 0x0C && *comp != 0x0D))
                    ctrl += 2.0f;
                comp += 1;
            }

            /* 文本字符必须大于一个比例 */
            if ((tots - ctrl) / tots < 0.618f) {
                mem_free(text);
                goto _func_exit;
            }
        }

        /* 识别 UTF-8 文本文件 */
        if (size >= 3 &&
            mem_cmp(text, BOM_UTF8, 3) == 0) {
            xbom = TRUE;
            page = CR_UTF8;
            show = text + 3;
        }
        else {
            show = text;
        }
    }

    /* UTF-8 编码校验过滤 */
    if (page == CR_UTF8) {
        if (!is_utf8_file(show)) {
            mem_free(text);
            goto _func_exit;
        }
    }
    else
    if (!is_cr_utf16(page)) {
        if (is_utf8_file(show))
            page = CR_UTF8;
    }

    ansi_t* temp;

    /* 统一转换到 UTF-8 编码 */
    if (page == CR_UTF8 || is_cr_utf16(page)) {
        temp = show;
    }
    else {
        temp = local_to_utf8(page, show);
        if (temp == NULL) {
            mem_free(text);
            goto _func_exit;
        }
    }

    TfrmMain*   frm;

    /* 文件内容送入编辑器控件 */
    frm = (TfrmMain*)(parm->form);
    crisec_enter(parm->lock);
    sci_call(SCI_SETTEXT, NULL, temp);
    crisec_leave(parm->lock);
    if (temp != show)
        mem_free(temp);

    /* 保存加载结果 */
    parm->xbom = xbom;
    parm->page = page;
    parm->show = show;
    parm->text = text;

    /* 压入文本信息 */
    info = str_dupA("QstText: Recognized text file");
    if (info != NULL)
        array_push_growT(&list, ansi_t*, &info);
    if (page == CR_UTF8) {
        if (!xbom)
            info = str_dupA("+-CodePage: UTF-8");
        else
            info = str_dupA("+-CodePage: UTF-8 with BOM");
    }
    else
    if (page == CR_UTF16LE) {
        info = str_dupA("+-CodePage: UTF-16LE");
    }
    else
    if (page == CR_UTF16BE) {
        info = str_dupA("+-CodePage: UTF-16BE");
    }
    else {
        info = str_fmtA("+-CodePage: %u", page);
    }
    if (info != NULL)
        array_push_growT(&list, ansi_t*, &info);
    frm->edtCPage->Text = IntToStr(page);

    /* 发送整个信息列表 */
    qst_send_finfo(&list, parm->netw);

    /* 窗口拉到最前面 */
    misc_bring2top(frm->Handle, Application->Handle);
    rett = TRUE;

    /* 释放临时的数据 */
_func_exit:
    if (fmtz != NULL)
        fmtz_free(fmtz);
    if (tmpz != NULL)
        fmtz_free(tmpz);
    array_freeT(&list, ansi_t*);
    return (rett);
}

/*
=======================================
    切换文字编码
=======================================
*/
CR_API void_t
qst_change_cpage (
  __CR_IN__ uint_t  cpage
    )
{
    ansi_t*     str;
    TfrmMain*   frm;

    if (s_wrk_ctx.show == NULL)
        return;
    if (s_wrk_ctx.page == CR_UTF8 ||
        is_cr_utf16(s_wrk_ctx.page))
        return;
    if (cpage == CR_UTF8 || is_cr_utf16(cpage)) {
        s_wrk_ctx.xbom = TRUE;
        return;
    }
    str = local_to_utf8(cpage, s_wrk_ctx.show);
    if (str == NULL)
        return;
    frm = (TfrmMain*)(s_wrk_ctx.form);
    crisec_enter(s_wrk_ctx.lock);
    sci_call(SCI_SETTEXT, NULL, str);
    crisec_leave(s_wrk_ctx.lock);
    mem_free(str);
}

/*
=======================================
    另存文本文件
=======================================
*/
CR_API bool_t
qst_save_file (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          page
    )
{
    FILE*       fp;
    leng_t      wrt;
    leng_t      len;
    ansi_t*     str;
    TfrmMain*   frm;

    /* 从编辑器取当前文本 UTF-8 格式 */
    frm = (TfrmMain*)(s_wrk_ctx.form);
    crisec_enter(s_wrk_ctx.lock);
    len = (leng_t)sci_call(SCI_GETLENGTH, NULL, NULL);
    str = str_allocA(len + 1);
    if (str == NULL) {
        crisec_leave(s_wrk_ctx.lock);
        return (FALSE);
    }
    sci_call(SCI_GETTEXT, len + 1, str);
    crisec_leave(s_wrk_ctx.lock);

    /* 根据指定的格式保存文件 */
    fp = fopen(name, "wb");
    if (fp == NULL)
        goto _failure1;
    if (page == CR_UTF8)
    {
        if (s_wrk_ctx.xbom) {
            wrt = fwrite(BOM_UTF8, 1, 3, fp);
            if (wrt != 3)
                goto _failure2;
        }
        wrt = fwrite(str, 1, len, fp);
        if (wrt != len)
            goto _failure2;
    }
    else
    if (is_cr_utf16(page))
    {
        wide_t* ucs2;

        if (page == CR_UTF16BE) {
            wrt = fwrite(BOM_UTF16BE, 1, 2, fp);
            if (wrt != 2)
                goto _failure2;
        }
        else {
            wrt = fwrite(BOM_UTF16LE, 1, 2, fp);
            if (wrt != 2)
                goto _failure2;
        }
        ucs2 = utf8_to_utf16(str);
        if (ucs2 == NULL)
            goto _failure2;
        len = str_lenW(ucs2);
        if (page == CR_UTF16BE) {
            for (wrt = 0; wrt < len; wrt++)
                ucs2[wrt] = xchg_int16u(ucs2[wrt]);
        }
        len *= 2;
        wrt = fwrite(ucs2, 1, len, fp);
        mem_free(ucs2);
        if (wrt != len)
            goto _failure2;
    }
    else
    {
        ansi_t* text;

        text = utf8_to_local(page, str);
        if (text == NULL)
            goto _failure2;
        len = str_lenA(text);
        wrt = fwrite(text, 1, len, fp);
        mem_free(text);
        if (wrt != len)
            goto _failure2;
    }
    mem_free(str);
    fclose(fp);
    return (TRUE);

_failure2:
    fclose(fp);
    file_deleteA(name);
_failure1:
    mem_free(str);
    return (FALSE);
}

/*
=======================================
    加载文本文件
=======================================
*/
CR_API bool_t
qst_load_file (
  __CR_IN__ const ansi_t*   name
    )
{
    sLOADER     ldr;

    set_ldrA(&ldr, name, "", 0, 0);
    ldr.page = get_sys_codepage();
    return (qst_try_load(&s_wrk_ctx, &ldr));
}

/* 文本执行类型列表 */
#define QST_ACT_QSTBAT      0x00    /* QstBat */

/*
=======================================
    执行文本内容
=======================================
*/
CR_API void_t
qst_file_action (
  __CR_IN__ uint_t  item_idx
    )
{
    /* 先保存到临时文件再执行 */
    if (!qst_save_file(QST_TMP_SCRIPT, CR_UTF8))
        return;
    switch (item_idx)
    {
        default: return;
        case QST_ACT_QSTBAT:    /* QstBat */
            misc_call_exe("QstCmdz.exe " QST_TMP_SCRIPT, FALSE, TRUE);
            break;
    }
}

/*****************************************************************************/
/*                               公用命令单元                                */
/*****************************************************************************/

/*
---------------------------------------
    退出应用程序
---------------------------------------
*/
static bool_t
qst_edt_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstText*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstText*)parm;
    frm = (TfrmMain*)(ctx->form);
    SendMessage(frm->Handle, WM_CLOSE, 0, 0);
    ctx->quit = TRUE;
    return (FALSE);
}

/*
---------------------------------------
    显示窗口位置
---------------------------------------
*/
static bool_t
qst_edt_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstText*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstText*)parm;
    frm = (TfrmMain*)(ctx->form);
    misc_bring2top(frm->Handle, Application->Handle);
    return (TRUE);
}

/*
---------------------------------------
    保存窗口位置
---------------------------------------
*/
static bool_t
qst_edt_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstText*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstText*)parm;
    frm = (TfrmMain*)(ctx->form);

    /* 获取窗口位置 */
    sint_t  x1 = frm->Left;
    sint_t  y1 = frm->Top;
    uint_t  ww = frm->Width;
    uint_t  hh = frm->Height;

    /* 保存窗口位置 */
    return (misc_desk_save(WIN_ICONF, x1, y1, ww, hh));
}

/*
---------------------------------------
    加载窗口位置
---------------------------------------
*/
static bool_t
qst_edt_win_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  x1, y1;
    uint_t  ww, hh;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 保存窗口位置 */
    if (!misc_desk_load(WIN_ICONF, &x1, &y1, &ww, &hh))
        return (FALSE);

    TfrmMain*   frm;
    sQstText*   ctx;

    ctx = (sQstText*)parm;
    frm = (TfrmMain*)(ctx->form);
    misc_bring2top(frm->Handle, Application->Handle);

    /* 设置窗口位置 */
    frm->Left   = x1;
    frm->Top    = y1;
    frm->Width  = ww;
    frm->Height = hh;
    return (TRUE);
}

/*
---------------------------------------
    加载配置文件
---------------------------------------
*/
static bool_t
qst_edt_cfg_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstText*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstText*)parm;
    qst_load_cfg(&ctx->cfgs);
    qst_set_editor(ctx);
    return (TRUE);
}

/*
---------------------------------------
    释放所有 FMTZ 插件
---------------------------------------
*/
static bool_t
qst_edt_ext_free (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstText*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstText*)parm;
    array_freeT(&ctx->extz, sENGINE*);
    ctx->extz.free = plugin_free;

    /* 清匹配列表 */
    ctx->count = 0;
    SAFE_FREE(ctx->filter)
    SAFE_FREE(ctx->flists)
    return (TRUE);
}

/*
---------------------------------------
    加载一个 FMTZ 插件
---------------------------------------
*/
static bool_t
qst_edt_ext_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sbin_t      dll;
    sQstText*   ctx;

    /* 参数解析 <插件路径> */
    if (argc < 2)
        return (FALSE);

    /* 过滤已经加载的插件 */
    if (sbin_testA(argv[1]) != NULL)
        return (TRUE);
    dll = sbin_loadA(argv[1]);
    if (dll == NULL)
        return (FALSE);

    sENGINE*        port;
    engine_init_t   func;

    /* 获取引擎接口 */
    func = sbin_exportT(dll, "engine_get", engine_init_t);
    if (func == NULL)
        goto _failure;
    if ((port = func()) == NULL)
        goto _failure;
    port->sbin = dll;
    port->mask = CR_FMTZ_MASK_DAT | CR_FMTZ_MASK_TXT;

    /* 压入插件列表 */
    ctx = (sQstText*)parm;
    if (array_push_growT(&ctx->extz, sENGINE*, &port) == NULL) {
        engine_free(port);
        return (FALSE);
    }
    ctx->extz.free = plugin_free;
    return (TRUE);

_failure:
    sbin_unload(dll);
    return (FALSE);
}

/*
---------------------------------------
    加载一个磁盘文件
---------------------------------------
*/
static bool_t
qst_edt_ldr_file (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t  page;
    fdist_t head;
    fdist_t tail;

    /* 参数解析 <文件路径> [头偏移] [尾偏移] [编码] [节点地址] [备注] */
    if (argc < 2)
        return (FALSE);
    page = CR_LOCAL;
    head = tail = 0;
    if (argc > 2) {
        head = str2intx64A(argv[2]);
        if (argc > 3) {
            tail = str2intx64A(argv[3]);
            if (argc > 4)
                page = str2intxA(argv[4]);
        }
    }

    sLOADER     ldr;
    sQstText*   ctx = (sQstText*)parm;

    /* 附加参数 aprm 不设为空 */
    if (argc > 6)
        set_ldrA(&ldr, argv[1], argv[6], head, tail);
    else
        set_ldrA(&ldr, argv[1], "", head, tail);
    if (page == CR_LOCAL)
        page = get_sys_codepage();
    ldr.page = page;

    /* 尝试加载文件 */
    QST_SET_STATE_BUSY
    qst_try_load(ctx, &ldr);
    QST_SET_STATE_FREE

    /* 无论成功失败都返回成功 */
    return (TRUE);
}

/*
---------------------------------------
    加载一个内存文件
---------------------------------------
*/
static bool_t
qst_edt_ldr_smem (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t  page;
    leng_t  size;
    void_t* data;
    fdist_t head;
    fdist_t tail;

    /* 参数解析 <共享名> <文件大小> <文件名> [头偏移] [尾偏移] [编码] [备注] */
    if (argc < 4)
        return (FALSE);
    page = CR_LOCAL;
    head = tail = 0;
    size = str2intx32A(argv[2]);
    if (argc > 4) {
        head = str2intx64A(argv[4]);
        if (argc > 5) {
            tail = str2intx64A(argv[5]);
            if (argc > 6)
                page = str2intxA(argv[6]);
        }
    }

    sLOADER     ldr;
    sQstText*   ctx = (sQstText*)parm;

    /* 获取整个共享文件 */
    data = share_file_get(argv[1], size);
    if (data == NULL)
        return (FALSE);

    /* 附加参数 aprm 不设为空 */
    set_ldrA(&ldr, argv[3], "", head, tail);
    if (argc > 7)
        set_ldrM(&ldr, data, size, argv[7], head, tail);
    else
        set_ldrM(&ldr, data, size, "", head, tail);
    if (page == CR_LOCAL)
        page = get_sys_codepage();
    ldr.page = page;

    /* 尝试加载文件 */
    QST_SET_STATE_BUSY
    qst_try_load(ctx, &ldr);
    QST_SET_STATE_FREE

    /* 用完后需要释放掉 */
    mem_free(data);
    return (TRUE);
}

/*
---------------------------------------
    释放当前浏览文本
---------------------------------------
*/
static bool_t
qst_edt_edt_clear (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(argc);
    CR_NOUSE(argv);
    qst_clear((sQstText*)parm);
    return (TRUE);
}

/*
---------------------------------------
    设置文本文件名匹配列表
---------------------------------------
*/
static bool_t
qst_edt_edt_filter (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    leng_t      count;
    ansi_t*     filter;
    ansi_t**    flists;
    sQstText*   ctx = (sQstText*)parm;

    /* 清匹配列表 */
    ctx->count = 0;
    SAFE_FREE(ctx->filter)
    SAFE_FREE(ctx->flists)

    /* 参数解析 [文件名匹配列表] */
    if (argc > 1) {
        filter = str_dupA(argv[1]);
        if (filter == NULL)
            return (FALSE);
        flists = str_splitA(filter, CR_AC('|'), &count);
        if (flists == NULL) {
            mem_free(filter);
            return (FALSE);
        }

        /* 跳过开头的空白 */
        for (leng_t idx = 0; idx < count; idx++)
            flists[idx] = skip_spaceA(flists[idx]);

        /* 保存结果 */
        ctx->count = count;
        ctx->filter = filter;
        ctx->flists = flists;
    }
    return (TRUE);
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit",  qst_edt_app_exit  },
    { "cfg:load",  qst_edt_cfg_load  },
    { "win:load",  qst_edt_win_load  },
    { "win:save",  qst_edt_win_save  },
    { "win:show",  qst_edt_win_show  },
    { "app:reset", qst_edt_edt_clear },

    /***** 公用插件命令 *****/
    { "ext:free", qst_edt_ext_free },
    { "ext:load", qst_edt_ext_load },

    /***** 公用加载命令 *****/
    { "ldr:file", qst_edt_ldr_file },
    { "ldr:smem", qst_edt_ldr_smem },

    /***** 编辑控制命令 *****/
    { "edt:clear",  qst_edt_edt_clear  },
    { "edt:filter", qst_edt_edt_filter },

    /***** 文本插件命令 *****/
    { "edt:ext:free", qst_edt_ext_free },
    { "edt:ext:load", qst_edt_ext_load },

    /***** 私有命令映射 *****/
    { "qedt:app:exit", qst_edt_app_exit },
    { "qedt:win:show", qst_edt_win_show },
};

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_edt_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    sQstText*   ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_edt_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    ctx = (sQstText*)param;

    /* 工作循环 */
    while (!ctx->quit)
    {
        ansi_t* string;

        /* 接收一条命令 */
        /* 即使是出错也要继续运行 */
        string = netw_cmd_recv(ctx->netw);
        if (string == NULL) {
            thread_sleep(1);
            continue;
        }

        /* 执行这条命令 */
        cmd_exec_main(obj, ctx, string);
        mem_free(string);
    }
    cmd_exec_free(obj);
    return (QST_OKAY);
}

/*
=======================================
    WinMain 程序入口
=======================================
*/
int WINAPI
WinMain (
  __CR_IN__ HINSTANCE   curt_app,
  __CR_IN__ HINSTANCE   prev_app,
  __CR_IN__ LPSTR       cmd_line,
  __CR_IN__ int         cmd_show
    )
{
    CR_NOUSE(curt_app);
    CR_NOUSE(prev_app);
    CR_NOUSE(cmd_line);
    CR_NOUSE(cmd_show);

    /* 只允许一个例程 */
    if (misc_is_running(EXE_XNAME))
        return (QST_ERROR);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_GUI))
        return (QST_ERROR);
    mem_zero(&s_wrk_ctx, sizeof(s_wrk_ctx));

    /* 初始化网络 */
    if (!socket_init())
        return (QST_ERROR);
    s_wrk_ctx.netw = netw_cli_open(EXE_XNAME);
    if (s_wrk_ctx.netw == NULL)
        return (QST_ERROR);

    /* 读取需要超时, 不然线程无法退出 */
    socket_set_timeout(s_wrk_ctx.netw, -1, QST_TCP_TOUT);

    /* 加载 SCI 控件 (无需释放) */
    if (sbin_loadA("SciLexer.dll") == NULL)
        return (QST_ERROR);

    thrd_t  thrd;

    /* 生成工作线程 */
    s_wrk_ctx.quit = FALSE;
    s_wrk_ctx.lock = crisec_new();
    if (s_wrk_ctx.lock == NULL)
        return (QST_ERROR);
    qst_load_cfg(&s_wrk_ctx.cfgs);
    s_wrk_ctx.page = get_sys_codepage();
    thrd = thread_new(0, qst_edt_main, &s_wrk_ctx, TRUE);
    if (thrd == NULL)
        return (QST_ERROR);

    /* 标准 VCL 过程开始 */
    try
    {
        Application->Initialize();
        Application->Title = WIN_TITLE;
        Application->CreateForm(__classid(TfrmMain), &frmMain);
        s_wrk_ctx.form = (void_t*)frmMain;
        qst_set_editor(&s_wrk_ctx);
        if (!thread_goon(thrd))
            return (QST_ERROR);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }

    /* 关闭线程直接退出 */
    if (!s_wrk_ctx.quit)
        s_wrk_ctx.quit = TRUE;
    thread_wait(thrd);
    thread_del(thrd);
    netw_cli_close(s_wrk_ctx.netw);
    return (QST_OKAY);
}
//---------------------------------------------------------------------------
