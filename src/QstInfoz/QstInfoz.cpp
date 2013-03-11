//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "QstInfoz.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
//---------------------------------------------------------------------------

/* 全局工作上下文 */
static sQstInfoz    s_wrk_ctx;

/*****************************************************************************/
/*                               公用命令单元                                */
/*****************************************************************************/

/*
---------------------------------------
    退出应用程序
---------------------------------------
*/
static bool_t
qst_inf_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstInfoz*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstInfoz*)parm;
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
qst_inf_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstInfoz*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstInfoz*)parm;
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
qst_inf_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstInfoz*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstInfoz*)parm;
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
qst_inf_win_load (
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
    sQstInfoz*  ctx;

    ctx = (sQstInfoz*)parm;
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
    清除文本显示框
---------------------------------------
*/
static bool_t
qst_inf_txt_clear (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t      id;
    uint_t      hd;
    uint_t      num;
    uint_t      app;
    TfrmMain*   frm;
    sQstInfoz*  ctx;

    ctx = (sQstInfoz*)parm;
    frm = (TfrmMain*)(ctx->form);

    /* 参数解析 [0/1] [信息编号] */
    if (argc == 1) {
        frm->txtInfoL->Clear();
        frm->txtInfoR->Clear();
        return (TRUE);
    }

    /* 信息编号用于清除指定的行 */
    id = str2intxA(argv[1]);
    if (argc > 2)
        app = (argv[2])[0];
    else
        app = 0;

    /* 清除左边的文本框 */
    if (id == 0) {
        if (app == 0) {
            frm->txtInfoL->Clear();
            return (TRUE);
        }
        num = frm->txtInfoL->Lines->Count;
        frm->txtInfoL->Lines->BeginUpdate();
        for (uint_t idx = 0; idx < num; idx++) {
            hd = (frm->txtInfoL->Lines->Strings[idx])[1];
            if (hd == app) {
                frm->txtInfoL->Lines->Delete((int)idx);
                idx -= 1;
                num -= 1;
            }
        }
        frm->txtInfoL->Lines->EndUpdate();
        return (TRUE);
    }

    /* 清除右边的文本框 */
    if (id == 1) {
        if (app == 0) {
            frm->txtInfoR->Clear();
            return (TRUE);
        }
        num = frm->txtInfoR->Lines->Count;
        frm->txtInfoR->Lines->BeginUpdate();
        for (uint_t idx = 0; idx < num; idx++) {
            hd = (frm->txtInfoR->Lines->Strings[idx])[1];
            if (hd == app) {
                frm->txtInfoR->Lines->Delete((int)idx);
                idx -= 1;
                num -= 1;
            }
        }
        frm->txtInfoR->Lines->EndUpdate();
        return (TRUE);
    }
    return (FALSE);
}

/*****************************************************************************/
/*                               INI 命令处理                                */
/*****************************************************************************/

/*
---------------------------------------
    INI 命令处理
---------------------------------------
*/
static void_t
qst_inf_ini_main (
  __CR_IN__ sINIu*      ini,
  __CR_IO__ sQstInfoz*  parm
    )
{
    ansi_t*     txt;
    uint_t      idx, num, app, hd;
    TfrmMain*   frm = (TfrmMain*)parm->form;

    /* 设置文本框 (左) */
    txt = ini_key_stringU("info::main", ini);
    if (txt != NULL)
    {
        /* 前两个字符指定信息编号 */
        if (txt[1] == '>') {
            app = txt[0];
            num = frm->txtInfoL->Lines->Count;
            frm->txtInfoL->Lines->BeginUpdate();
            for (idx = 0; idx < num; idx++) {
                hd = (frm->txtInfoL->Lines->Strings[idx])[1];
                if (hd > app) {
                    frm->txtInfoL->Lines->Insert((int)idx, txt);
                    break;
                }
            }
            if (idx >= num)
                frm->txtInfoL->Lines->Append(txt);
            frm->txtInfoL->Lines->EndUpdate();
        }
        mem_free(txt);
    }

    /* 设置文本框 (右) */
    txt = ini_key_stringU("info::subs", ini);
    if (txt != NULL)
    {
        /* 前两个字符指定信息编号 */
        if (txt[1] == '>') {
            app = txt[0];
            num = frm->txtInfoR->Lines->Count;
            frm->txtInfoR->Lines->BeginUpdate();
            for (idx = 0; idx < num; idx++) {
                hd = (frm->txtInfoR->Lines->Strings[idx])[1];
                if (hd > app) {
                    frm->txtInfoR->Lines->Insert((int)idx, txt);
                    break;
                }
            }
            if (idx >= num)
                frm->txtInfoR->Lines->Append(txt);
            frm->txtInfoR->Lines->EndUpdate();
        }
        mem_free(txt);
    }
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit",  qst_inf_app_exit  },
    { "win:load",  qst_inf_win_load  },
    { "win:save",  qst_inf_win_save  },
    { "win:show",  qst_inf_win_show  },
    { "app:reset", qst_inf_txt_clear },

    /***** 清除信息命令 *****/
    { "txt:clear", qst_inf_txt_clear },

    /***** 私有命令映射 *****/
    { "qinf:app:exit", qst_inf_app_exit },
    { "qinf:win:show", qst_inf_win_show },
};

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_inf_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    sQstInfoz*  ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_inf_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    ctx = (sQstInfoz*)param;

    /* 工作循环 */
    while (!ctx->quit)
    {
        sINIu*  iniu;
        ansi_t* text;
        ansi_t* string;

        /* 接收一条命令 */
        /* 即使是出错也要继续运行 */
        string = netw_cmd_recv(ctx->netw);
        if (string == NULL) {
            thread_sleep(1);
            continue;
        }

        /* 尝试解析 INI 指令 */
        text = cmd_ini_get(string);
        if (text != NULL) {
            mem_free(string);
            iniu = ini_parseU(text);
            mem_free(text);
            if (iniu != NULL) {
                qst_inf_ini_main(iniu, ctx);
                ini_closeU(iniu);
            }
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
    thrd = thread_new(0, qst_inf_main, &s_wrk_ctx, TRUE);
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
