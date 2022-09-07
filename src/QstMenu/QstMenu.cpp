//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "uAbout.h"
#include "QstMenu.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
USEFORM("uAbout.cpp", frmAbout);
//---------------------------------------------------------------------------

/* 全局工作上下文 */
static sQstMenu     s_wrk_ctx;

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/*
=======================================
    发送单条远程命令
=======================================
*/
CR_API void_t
qst_send_cmdz (
  __CR_IN__ const ansi_t*   cmd
    )
{
    cmd_shl_send(s_wrk_ctx.netw, cmd);
}

/*
---------------------------------------
    加载一组子菜单项
---------------------------------------
*/
static leng_t
qst_load_item (
  __CR_IN__ sXMLw*          xml,
  __CR_IN__ leng_t          idx,
  __CR_IN__ const wide_t*   end,
  __CR_IN__ TMenuItem*      menu,
  __CR_IN__ sCURBEAD*       event
    )
{
    TMenuItem*  item;

    /* 逐个标签加载 */
    for (idx++; idx < xml->count; idx++)
    {
        ansi_t* name_a;
        ansi_t* text_a;
        ansi_t* scut_a;
        wide_t* name_w;
        wide_t* text_w;
        wide_t* scut_w;

        /* 跳过没用的节点 */
        if (str_cmpIW(xml->nodes[idx].name, end) == 0)
            break;
        if (str_cmpIW(xml->nodes[idx].name, CR_WS("MenuItem")) != 0) {
            idx = xml_skip_tagW(xml->nodes, idx, xml->count) - 1;
            continue;
        }

        /* 解析菜单项 */
        name_w = xml_attr_stringW(CR_WS("name"), &xml->nodes[idx]);
        if (name_w == NULL)
            continue;
        text_w = xml_attr_stringW(CR_WS("text"), &xml->nodes[idx]);
        if (text_w == NULL) {
            mem_free(name_w);
            continue;
        }
        scut_w = xml_attr_stringW(CR_WS("scut"), &xml->nodes[idx]);

        /* 转换到本地编码 */
        name_a = utf16_to_local(CR_LOCAL, name_w);
        mem_free(name_w);
        text_a = utf16_to_local(CR_LOCAL, text_w);
        mem_free(text_w);
        if (scut_w != NULL) {
            scut_a = utf16_to_local(CR_LOCAL, scut_w);
            mem_free(scut_w);
        }
        else {
            scut_a = NULL;
        }
        if (name_a == NULL || text_a == NULL) {
            TRY_FREE(name_a);
            TRY_FREE(text_a);
            TRY_FREE(scut_a);
            continue;
        }

        bool    radio;
        bool    chked;
        uint_t  group;

        if (xml_attr_intxW(CR_WS("radio"), FALSE, &xml->nodes[idx]))
            radio = true;
        else
            radio = false;
        if (xml_attr_intxW(CR_WS("checked"), FALSE, &xml->nodes[idx]))
            chked = true;
        else
            chked = false;
        group = xml_attr_intxW(CR_WS("group"), 0, &xml->nodes[idx]);

        sMenuEvent*     find;
        TNotifyEvent    call = NULL;

        /* 根据名称查找事件 */
        find = curbead_findT(event, sMenuEvent, name_a);
        if (find != NULL) call = find->call;

        /* 生成菜单项 */
        if (scut_a == NULL) {
            item = NewItem(text_a, 0, chked, true, call, 0, name_a);
        }
        else {
            item = NewItem(text_a, TextToShortCut(scut_a), chked,
                           true, call, 0, name_a);
            mem_free(scut_a);
        }
        mem_free(name_a);
        mem_free(text_a);
        item->RadioItem = radio;
        item->GroupIndex = (Byte)group;
        menu->Add(item);

        /* 有子菜单项 */
        if (!xml->nodes[idx].closed)
            idx = qst_load_item(xml, idx, CR_WS("/MenuItem"), item, event);
    }
    return (idx);
}

/*
=======================================
    加载菜单配置文件
=======================================
*/
CR_API bool_t
qst_load_menu (
  __CR_IN__ TMenuItem*  menu,
  __CR_IN__ sCURBEAD*   event
    )
{
    sXMLw*  xml;
    leng_t  idx;
    ansi_t* str;

    /* 加载文件 */
    str = file_load_as_strA(QST_PATH_CONFIG WIN_XCONF);
    if (str == NULL)
        return (FALSE);

    /* 解析 XML 文件 */
    xml = xml_from_textW(str);
    mem_free(str);
    if (xml == NULL)
        return (FALSE);

    /* 检查根标签 */
    if (xml->nodes[0].name[0] == CR_WC('?'))
        idx = 1;
    else
        idx = 0;
    if (xml->nodes[idx].closed ||
        str_cmpIW(xml->nodes[idx].name, CR_WS("QstMenu")) != 0) {
        xml_closeW(xml);
        return (FALSE);
    }

    /* 加载菜单项 (容错执行) */
    qst_load_item(xml, idx, CR_WS("/QstMenu"), menu, event);
    xml_closeW(xml);
    return (TRUE);
}

/*
---------------------------------------
    窗口枚举回调
---------------------------------------
*/
static BOOL CALLBACK
qst_window_enum (
  __CR_IN__ HWND    hwnd,
  __CR_IN__ LPARAM  param
    )
{
    ansi_t  name[256];

    if (GetClassName(hwnd, name, 256)) {
        if (str_cmpA(name, "SciTEWindow") == 0)
            PostMessage(hwnd, WM_CLOSE, 0, 0);
    }
    CR_NOUSE(param);
    return (TRUE);
}

/*
=======================================
    关闭所有外部工具
=======================================
*/
CR_API void_t
qst_quit_all_3rd (void_t)
{
    EnumWindows((WNDENUMPROC)qst_window_enum, 0);
}

/*
---------------------------------------
    判断是否是文本文件
---------------------------------------
*/
static uint_t
qst_is_text_file (
  __CR_IN__ const ansi_t*   name
    )
{
    uint_t  ret;
    leng_t  idx;
    leng_t  len;
    ansi_t* txt;
    fsize_t fsz;

    /* 只支持编辑 4MB 以下的文本文件 */
    if ((fsz = file_sizeA(name)) > CR_M2B(4))
        return (FALSE);
    txt = file_load_as_strA(name);
    if (txt == NULL)
        return (FALSE);
    ret = FALSE;
    len = (leng_t)fsz;

    ansi_t* show = NULL;
    wide_t* pntr = (wide_t*)txt;

    /* 识别 UTF-16 文本文件 */
    if (len >= 2 && len % 2 == 0 && str_lenW(pntr) * 2 == len)
    {
        wide_t* ucs2;

        /* UTF-16 编码校验过滤 */
        if (mem_cmp(txt, BOM_UTF16LE, 2) == 0) {
            ucs2 = (wide_t*)(&txt[2]);
            show = utf16_to_utf8(ucs2);
            if (show != NULL) {
                mem_free(show);
                ret = CR_UTF16X;
            }
        }
        else
        if (mem_cmp(txt, BOM_UTF16BE, 2) == 0) {
            ucs2 = (wide_t*)(&txt[2]);
            for (pntr = ucs2; *pntr != NIL; pntr++)
                *pntr = xchg_int16u(*pntr);
            show = utf16_to_utf8(ucs2);
            if (show != NULL) {
                mem_free(show);
                ret = CR_UTF16X;
            }
        }
    }

    /* 非 UTF-16 文本文件 */
    if (show == NULL && str_lenA(txt) == len)
    {
        byte_t  cha;

        /* 过滤掉可能的二进制文件 */
        for (idx = 0; idx < len; idx++) {
            cha = txt[idx];
            if ((cha == 0xFF) || (cha < 0x20 &&
                   cha != 0x09 && cha != 0x0A &&
                   cha != 0x0C && cha != 0x0D))
                break;
        }
        if (idx == len)
        {
            leng_t  temp;
            int32u  ucs4;

            /* 识别 UTF-8 文本文件 */
            if (len >= 3 && mem_cmp(txt, BOM_UTF8, 3) == 0)
                show = txt + 3;
            else
                show = txt;
            while (*show != NIL) {
                temp = utf8_to_ucs4(&ucs4, show);
                if (temp == 0)
                    break;
                show += temp;
            }
            ret = (*show == NIL) ? CR_UTF8 : TRUE;
        }
    }

    /* 返回判断结果 */
    mem_free(txt);
    return (ret);
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
qst_mnu_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstMenu*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMenu*)parm;
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
qst_mnu_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstMenu*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMenu*)parm;
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
qst_mnu_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstMenu*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMenu*)parm;
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
=======================================
    保存窗口位置
=======================================
*/
CR_API void_t
qst_mnu_win_save2 (void_t)
{
    /* GUI 使用的包装 */
    qst_mnu_win_save(&s_wrk_ctx, 0, NULL);
}

/*
---------------------------------------
    加载窗口位置
---------------------------------------
*/
static bool_t
qst_mnu_win_load (
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
    sQstMenu*   ctx;

    ctx = (sQstMenu*)parm;
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
=======================================
    加载窗口位置
=======================================
*/
CR_API void_t
qst_mnu_win_load2 (void_t)
{
    /* GUI 使用的包装 */
    qst_mnu_win_load(&s_wrk_ctx, 0, NULL);
}

/*
---------------------------------------
    加载一个磁盘文件
---------------------------------------
*/
static bool_t
qst_mnu_ldr_file (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    /* 参数解析 <文件路径> [头偏移] [尾偏移] [编码] [节点地址] [备注] */
    if (argc < 2)
        return (FALSE);

    TfrmMain*   frm;
    sQstMenu*   ctx;

    ctx = (sQstMenu*)parm;
    frm = (TfrmMain*)(ctx->form);

    uint_t  is_text = FALSE;
    bool_t  checked = FALSE;

    /* 根据设置调用外部程序 */
    if (frm->is_use_scite()) {
        checked = TRUE;
        is_text = qst_is_text_file(argv[1]);
        if (is_text)
        {
            AnsiString  line;

            line = QST_PATH_EXT3RD;
            line += "wscite\\SciTE.exe -close:";
            if (is_text == CR_UTF8) {
                line += " -code.page=65001";
            }
            else
            if (is_text != CR_UTF16X) {
                line += " -code.page=";
                line += IntToStr(frm->m_code_page);
            }
            line += " \"";
            line += AnsiString(argv[1]);
            line += "\"";
            misc_call_exe(line.c_str(), FALSE, FALSE);
        }
    }

    CR_NOUSE(is_text);
    CR_NOUSE(checked);

    /* 无论成功失败都返回成功 */
    return (TRUE);
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit", qst_mnu_app_exit },
    { "win:load", qst_mnu_win_load },
    { "win:save", qst_mnu_win_save },
    { "win:show", qst_mnu_win_show },

    /***** 公用加载命令 *****/
    { "ldr:file", qst_mnu_ldr_file },

    /***** 私有命令映射 *****/
    { "qmnu:app:exit", qst_mnu_app_exit },
    { "qmnu:win:show", qst_mnu_win_show },
};

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_mnu_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    sQstMenu*   ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_mnu_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    ctx = (sQstMenu*)param;

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

    thrd_t  thrd;

    /* 生成工作线程 */
    s_wrk_ctx.quit = FALSE;
    thrd = thread_new(0, qst_mnu_main, &s_wrk_ctx, TRUE);
    if (thrd == NULL)
        return (QST_ERROR);

    /* 标准 VCL 过程开始 */
    try
    {
        Application->Initialize();
        Application->Title = WIN_TITLE;
        Application->CreateForm(__classid(TfrmMain), &frmMain);
        Application->CreateForm(__classid(TfrmAbout), &frmAbout);
        s_wrk_ctx.form = (void_t*)frmMain;
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
