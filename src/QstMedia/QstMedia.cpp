//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "uPlay.h"
#include "QstMedia.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
USEFORM("uPlay.cpp", frmPlay);
//---------------------------------------------------------------------------

/* 全局工作上下文 */
static sQstMedia    s_wrk_ctx;

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
CR_API void_t
qst_load_cfg (
  __CR_OT__ sQXMM_conf* cfgs
    )
{
    sINIu*  ini;
    ansi_t* str;

    /* 加载配置文件 */
    str = file_load_as_strA(QST_PATH_CONFIG WIN_ICONF);
    if (str == NULL)
        goto _load_defs;
    ini = ini_parseU(str);
    mem_free(str);
    if (ini == NULL)
        goto _load_defs;

    /* 读入配置参数 */
    cfgs->enabled = ini_key_intxU("qxmm::enabled", TRUE, ini);
    cfgs->loop_play = ini_key_intxU("qxmm::loop_play", FALSE, ini);
    ini_closeU(ini);
    return;

_load_defs:
    cfgs->enabled = TRUE;
    cfgs->loop_play = FALSE;
}

/*
=======================================
    停止播放
=======================================
*/
CR_API bool_t
qst_stop (void_t)
{
    bool_t  rett;

    _ENTER_XMM_SINGLE_
    if (s_wrk_ctx.play == NULL)
        rett = FALSE;
    else
        rett = CR_VCALL(s_wrk_ctx.play)->stop(s_wrk_ctx.play);
    _LEAVE_XMM_SINGLE_
    return (rett);
}

/*
=======================================
    开始播放
=======================================
*/
CR_API bool_t
qst_play (void_t)
{
    bool_t  rett;

    _ENTER_XMM_SINGLE_
    if (s_wrk_ctx.play == NULL)
        rett = FALSE;
    else
        rett = CR_VCALL(s_wrk_ctx.play)->play(s_wrk_ctx.play);
    _LEAVE_XMM_SINGLE_
    return (rett);
}

/*
=======================================
    暂停播放
=======================================
*/
CR_API bool_t
qst_pause (void_t)
{
    bool_t  rett;

    _ENTER_XMM_SINGLE_
    if (s_wrk_ctx.play == NULL)
        rett = FALSE;
    else
        rett = CR_VCALL(s_wrk_ctx.play)->pause(s_wrk_ctx.play);
    _LEAVE_XMM_SINGLE_
    return (rett);
}

/*
=======================================
    是否结束
=======================================
*/
CR_API bool_t
qst_is_over (
  __CR_OT__ bool_t* replay
    )
{
    bool_t  over;

    _ENTER_XMM_SINGLE_
    if (s_wrk_ctx.play == NULL ||
        !CR_VCALL(s_wrk_ctx.play)->is_over(s_wrk_ctx.play, &over))
        over = TRUE;
    _LEAVE_XMM_SINGLE_
    *replay = s_wrk_ctx.cfgs.loop_play;
    return (over);
}

/*
=======================================
    获取位置
=======================================
*/
CR_API sint_t
qst_get_pos (
  __CR_OT__ int64u* time_ms
    )
{
    int64u  curt, total;

    _ENTER_XMM_SINGLE_
    if (s_wrk_ctx.play == NULL ||
        !CR_VCALL(s_wrk_ctx.play)->get_pos(s_wrk_ctx.play, &curt, &total))
        curt = total = 0;
    _LEAVE_XMM_SINGLE_
    *time_ms = curt;
    if (total == 0)
        return (0);
    curt *= QST_XMM_MAX;
    curt /= total;
    return ((sint_t)curt);
}

/*
=======================================
    计算播放时间
=======================================
*/
CR_API ansi_t*
qst_ms2str (
  __CR_OT__ ansi_t* str,
  __CR_IN__ int64u  time
    )
{
    uint_t  hh, mm, ss;

    time += 500;
    time /= 1000;
    hh = (uint_t)(time / 3600);
    mm = (uint_t)(time % 3600);
    ss = (uint_t)(mm % 60);
    mm = (uint_t)(mm / 60);
    sprintf(str, "%02u:%02u:%02u", hh, mm, ss);
    return (str);
}

/*
---------------------------------------
    释放当前播放媒体
---------------------------------------
*/
static void_t
qst_clear (
  __CR_IO__ sQstMedia*  parm
    )
{
    TfrmMain*   frm;

    /* 先确保停止播放再释放 */
    frm = (TfrmMain*)(parm->form);
    frm->btnStopClick(NULL);
    if (frm->timPlay->Enabled) {
        frm->timPlay->Enabled = false;
        frm->timPlayTimer(NULL);
    }
    if (parm->fmtz != NULL) {
        _ENTER_XMM_SINGLE_
        fmtz_free((sFMTZ*)parm->fmtz);
        parm->fmtz = NULL;
        parm->play = NULL;
        _LEAVE_XMM_SINGLE_
    }
    frmPlay->Hide();
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
    cmd_shl_send(netw, "txt:clear 0 3");

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
        info = str_fmtA("info::main=\"3> %s\"", temp);
        mem_free(temp);
        if (info != NULL) {
            cmd_ini_send(netw, info);
            mem_free(info);
        }
    }
}

/*
---------------------------------------
    尝试加载目标数据
---------------------------------------
*/
static bool_t
qst_try_load (
  __CR_IO__ sQstMedia*  parm,
  __CR_IO__ sLOADER*    ldrs
    )
{
    leng_t  idx;
    leng_t  cnts;
    sFMTZ*  fmtz;
    sFMTZ*  tmpz;
    sARRAY  list;
    ansi_t* info;
    sLOADER copy;

    /* 关闭加载功能 */
    if (!parm->cfgs.enabled)
        return (FALSE);

    /* 释放上次加载 */
    qst_clear(parm);

    /* 初始化加载尝试 */
    fmtz = tmpz = NULL;
    array_initT(&list, ansi_t*);
    list.free = finfo_free;
    struct_cpy(&copy, ldrs, sLOADER);
    cnts = array_get_sizeT(&parm->extz, sENGINE*);

    sENGINE**   unit;
    sFMT_DAT*   datz;
    sFMT_PRT*   xmmz;

    /* 尝试到有结果为止 */
_retry:
    /* 逐个插件试验加载 */
    for (unit = NULL, idx = 0; idx < cnts; idx++) {
        unit = array_get_unitT(&parm->extz, sENGINE*, idx);
        tmpz = engine_do(*unit, &copy);
        if (tmpz != NULL)
            break;
    }

    /* 释放前次的数据 */
    if (fmtz != NULL)
        fmtz_free(fmtz);

    /* 无法识别的媒体文件 */
    if (unit == NULL || tmpz == NULL)
        goto _func_out;

    /* 需要经过二次解码 */
    if (tmpz->type == CR_FMTZ_NOP ||
        tmpz->type == CR_FMTZ_DEC)
    {
        /* 换成内存解析 */
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

    /* 得到可以加载的媒体文件 */
    if (tmpz->type == CR_FMTZ_PRT)
    {
        bool_t      canc;
        int64u      curt;
        int64u      total;
        uint_t      ww, hh;
        ansi_t      str[64];
        iXMMEDIA*   xmmplay;

        xmmz = (sFMT_PRT*)tmpz;
        if (str_cmpA(xmmz->more, "iXMMEDIA") != 0)
            goto _func_out;
        xmmplay = (iXMMEDIA*)xmmz->port;
        if (!CR_VCALL(xmmplay)->get_pos(xmmplay, &curt, &total))
            goto _func_out;
        if (total == 0)         /* 过滤掉静态的内容 */
            goto _func_out;
        canc = CR_VCALL(xmmplay)->is_video(xmmplay);
        if (canc) {
            if (!CR_VCALL(xmmplay)->video_size(xmmplay, &ww, &hh))
                goto _func_out;
            frmPlay->ClientWidth  = ww;
            frmPlay->ClientHeight = hh;
            if (!CR_VCALL(xmmplay)->video_setwin(xmmplay, frmPlay->Handle, 0))
                goto _func_out;
            misc_bring2top(frmMain->Handle, Application->Handle);
            frmPlay->Show();
        }
        parm->fmtz = xmmz;
        parm->play = xmmplay;

        /* 压入插件信息 */
        info = str_fmtA("Plug-in: %s", (*unit)->info);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入媒体信息 */
        info = str_fmtA("+-XMMedia: %s", xmmz->infor);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入长度信息 */
        qst_ms2str(str, total);
        if (canc) {
            info = str_fmtA("+---Video total time: %s (%I64u ms)",
                                    str, total);
        } else {
            info = str_fmtA("+---Audio total time: %s (%I64u ms)",
                                    str, total);
        }
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 发送整个信息列表 */
        qst_send_finfo(&list, parm->netw);
        array_freeT(&list, ansi_t*);

        /* 加载后马上播放 */
        frmMain->btnPlayClick(NULL);
        return (TRUE);
    }

    /* 释放临时的数据 */
_func_out:
    if (tmpz != NULL)
        fmtz_free(tmpz);
    array_freeT(&list, ansi_t*);
    return (FALSE);
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
qst_xmm_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstMedia*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMedia*)parm;
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
qst_xmm_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstMedia*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMedia*)parm;
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
qst_xmm_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstMedia*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMedia*)parm;
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
qst_xmm_win_load (
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
    sQstMedia*  ctx;

    ctx = (sQstMedia*)parm;
    frm = (TfrmMain*)(ctx->form);
    misc_bring2top(frm->Handle, Application->Handle);

    /* 设置窗口位置 */
    frm->Left = x1;
    frm->Top  = y1;
    return (TRUE);
}

/*
---------------------------------------
    加载配置文件
---------------------------------------
*/
static bool_t
qst_xmm_cfg_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstMedia*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMedia*)parm;
    qst_load_cfg(&ctx->cfgs);
    if (!ctx->cfgs.enabled)
        qst_clear(ctx);
    return (TRUE);
}

/*
---------------------------------------
    释放所有 FMTZ 插件
---------------------------------------
*/
static bool_t
qst_xmm_ext_free (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstMedia*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMedia*)parm;
    array_freeT(&ctx->extz, sENGINE*);
    ctx->extz.free = plugin_free;
    return (TRUE);
}

/*
---------------------------------------
    加载一个 FMTZ 插件
---------------------------------------
*/
static bool_t
qst_xmm_ext_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sbin_t      dll;
    sQstMedia*  ctx;

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
    port->mask = CR_FMTZ_MASK_DAT | CR_FMTZ_MASK_XMM;

    /* 压入插件列表 */
    ctx = (sQstMedia*)parm;
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
qst_xmm_ldr_file (
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
    sQstMedia*  ctx = (sQstMedia*)parm;

    /* 附加参数 aprm 不设为空 */
    if (argc > 6)
        set_ldrA(&ldr, argv[1], argv[6], head, tail);
    else
        set_ldrA(&ldr, argv[1], "", head, tail);
    ldr.page = page;

    /* 尝试加载指定文件 */
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
qst_xmm_ldr_smem (
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
    sQstMedia*  ctx = (sQstMedia*)parm;

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
    ldr.page = page;

    /* 尝试加载指定文件 */
    QST_SET_STATE_BUSY
    qst_try_load(ctx, &ldr);
    QST_SET_STATE_FREE

    /* 用完后需要释放掉 */
    mem_free(data);
    return (TRUE);
}

/*
---------------------------------------
    释放当前播放媒体
---------------------------------------
*/
static bool_t
qst_xmm_xmm_clear (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(argc);
    CR_NOUSE(argv);
    qst_clear((sQstMedia*)parm);
    return (TRUE);
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit",  qst_xmm_app_exit  },
    { "cfg:load",  qst_xmm_cfg_load  },
    { "win:load",  qst_xmm_win_load  },
    { "win:save",  qst_xmm_win_save  },
    { "win:show",  qst_xmm_win_show  },
    { "app:reset", qst_xmm_xmm_clear },

    /***** 公用插件命令 *****/
    { "ext:free", qst_xmm_ext_free },
    { "ext:load", qst_xmm_ext_load },

    /***** 公用加载命令 *****/
    { "ldr:file", qst_xmm_ldr_file },
    { "ldr:smem", qst_xmm_ldr_smem },

    /***** 清除媒体命令 *****/
    { "xmm:clear", qst_xmm_xmm_clear },

    /***** 封包插件命令 *****/
    { "xmm:ext:free", qst_xmm_ext_free },
    { "xmm:ext:load", qst_xmm_ext_load },

    /***** 私有命令映射 *****/
    { "qxmm:app:exit", qst_xmm_app_exit },
    { "qxmm:win:show", qst_xmm_win_show },
};

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_xmm_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    sQstMedia*  ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_xmm_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    ctx = (sQstMedia*)param;

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
    mtlock_init(&s_wrk_ctx.lock);
    qst_load_cfg(&s_wrk_ctx.cfgs);
    thrd = thread_new(0, qst_xmm_main, &s_wrk_ctx, TRUE);
    if (thrd == NULL)
        return (QST_ERROR);

    /* 标准 VCL 过程开始 */
    try
    {
        Application->Initialize();
        Application->Title = WIN_TITLE;
        Application->CreateForm(__classid(TfrmMain), &frmMain);
        Application->CreateForm(__classid(TfrmPlay), &frmPlay);
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
