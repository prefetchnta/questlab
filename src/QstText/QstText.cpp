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
    cfgs->max_fsize = ini_key_intxU("qedt::max_size_kb", 20480, ini);
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

    /* 退出临界区 */
    crisec_leave(parm->lock);
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

    /* 清过滤列表 */
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
    ldr.page = page;

    /* 尝试加载指定文件 */
    QST_SET_STATE_BUSY
    //qst_try_load(ctx, &ldr);
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
    ldr.page = page;

    /* 尝试加载指定文件 */
    QST_SET_STATE_BUSY
    //qst_try_load(ctx, &ldr);
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
    CR_NOUSE(parm);
    CR_NOUSE(argc);
    CR_NOUSE(argv);
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

    /***** 清除文本命令 *****/
    { "edt:clear", qst_edt_edt_clear },

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
