//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "QstIndex.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
//---------------------------------------------------------------------------

/* 全局工作上下文 */
static sQstIndex    s_wrk_ctx;

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/*
=======================================
    刷新序号框显示
=======================================
*/
CR_API void_t
qst_refresh_text (void_t)
{
    ansi_t  buf[128];

    sprintf(buf, "%u/%u",
        frmMain->tbrCount->Position,
        frmMain->tbrCount->Max);
    frmMain->edtCount->Text = AnsiString(buf);
}

/*
=======================================
    发送当前序号值
=======================================
*/
CR_API void_t
qst_send_index (
  __CR_IN__ sint_t  idx
    )
{
    ansi_t          buf[64];
    static sint_t   pre = 0;

    /* 保证只发一次 */
    if (pre == idx)
        return;
    pre = idx;

    /* 过滤发送 */
    if (!s_wrk_ctx.send)
        return;

    /* 设置图片帧号 */
    sprintf(buf, "idx:set_now %u", idx);
    cmd_shl_send(s_wrk_ctx.netw, buf);
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
qst_idx_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstIndex*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstIndex*)parm;
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
qst_idx_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstIndex*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstIndex*)parm;
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
qst_idx_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstIndex*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstIndex*)parm;
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
qst_idx_win_load (
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
    sQstIndex*  ctx;

    ctx = (sQstIndex*)parm;
    frm = (TfrmMain*)(ctx->form);
    misc_bring2top(frm->Handle, Application->Handle);

    /* 设置窗口位置 */
    frm->Left = x1;
    frm->Top  = y1;
    return (TRUE);
}

/*
---------------------------------------
    复位序号设置
---------------------------------------
*/
static bool_t
qst_idx_app_reset (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstIndex*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstIndex*)parm;
    frm = (TfrmMain*)(ctx->form);

    /* 如果正在播放, 停止之 */
    if (frm->timPlay->Enabled)
        frm->btnPlayClick(NULL);
    ctx->send = FALSE;
    frm->tbrCount->Min = 0;
    frm->tbrCount->Max = 0;
    ctx->send = TRUE;
    qst_refresh_text();
    return (TRUE);
}

/*
---------------------------------------
    设置最小序号
---------------------------------------
*/
static bool_t
qst_idx_get_min (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  min = 0;

    /* 参数解析 [最小序号] */
    if (argc > 1)
        min = str2intxA(argv[1]);

    TfrmMain*   frm;
    sQstIndex*  ctx;

    if (min < 0)
        min = 0;
    ctx = (sQstIndex*)parm;
    frm = (TfrmMain*)(ctx->form);

    /* 如果正在播放, 停止之 */
    if (frm->timPlay->Enabled)
        frm->btnPlayClick(NULL);
    frm->tbrCount->Min = min;
    return (TRUE);
}

/*
---------------------------------------
    设置最大序号
---------------------------------------
*/
static bool_t
qst_idx_get_max (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  max = 0;

    /* 参数解析 [最大序号] */
    if (argc > 1)
        max = str2intxA(argv[1]);

    TfrmMain*   frm;
    sQstIndex*  ctx;

    if (max < 0)
        max = 0;
    ctx = (sQstIndex*)parm;
    frm = (TfrmMain*)(ctx->form);

    /* 如果正在播放, 停止之 */
    if (frm->timPlay->Enabled)
        frm->btnPlayClick(NULL);
    frm->tbrCount->Max = max;
    qst_refresh_text();
    return (TRUE);
}

/*
---------------------------------------
    设置当前序号
---------------------------------------
*/
static bool_t
qst_idx_get_now (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  now = 0;

    /* 参数解析 [当前序号] */
    if (argc > 1)
        now = str2intxA(argv[1]);

    TfrmMain*   frm;
    sQstIndex*  ctx;

    ctx = (sQstIndex*)parm;
    frm = (TfrmMain*)(ctx->form);

    /* 如果正在播放, 停止之 */
    if (frm->timPlay->Enabled)
        frm->btnPlayClick(NULL);

    /* 需要过滤一下范围 */
    if (now < frm->tbrCount->Min)
        now = frm->tbrCount->Min;
    else
    if (now > frm->tbrCount->Max)
        now = frm->tbrCount->Max;
    if (frm->tbrCount->Position != now) {
        ctx->send = FALSE;
        frm->tbrCount->Position = now;
        ctx->send = TRUE;
    }
    return (TRUE);
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit",  qst_idx_app_exit  },
    { "win:load",  qst_idx_win_load  },
    { "win:save",  qst_idx_win_save  },
    { "win:show",  qst_idx_win_show  },
    { "app:reset", qst_idx_app_reset },

    /***** 索引控制命令 *****/
    { "idx:get_min", qst_idx_get_min },
    { "idx:get_max", qst_idx_get_max },
    { "idx:get_now", qst_idx_get_now },

    /***** 私有命令映射 *****/
    { "qidx:app:exit", qst_idx_app_exit },
    { "qidx:win:show", qst_idx_win_show },
};

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_idx_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    sQstIndex*  ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_idx_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    ctx = (sQstIndex*)param;

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
    s_wrk_ctx.send = TRUE;
    s_wrk_ctx.quit = FALSE;
    thrd = thread_new(0, qst_idx_main, &s_wrk_ctx, TRUE);
    if (thrd == NULL)
        return (QST_ERROR);

    /* 标准 VCL 过程开始 */
    try
    {
        Application->Initialize();
        Application->Title = WIN_TITLE;
        Application->CreateForm(__classid(TfrmMain), &frmMain);
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
