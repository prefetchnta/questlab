
#include "QstComm.h"
#pragma warning (disable: 4127)
#pragma warning (disable: 4512)

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/



/*****************************************************************************/
/*                               公用命令单元                                */
/*****************************************************************************/

/*
---------------------------------------
    退出应用程序
---------------------------------------
*/
static bool_t
qst_com_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstComm*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstComm*)parm;
    ctx->quit = TRUE;
    ((QMainWindow*)(ctx->form))->close();
    return (FALSE);
}

/*
---------------------------------------
    显示窗口位置
---------------------------------------
*/
static bool_t
qst_com_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstComm*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstComm*)parm;
    misc_bring2top(ctx->hwnd, NULL);
    return (TRUE);
}

/*
---------------------------------------
    保存窗口位置
---------------------------------------
*/
static bool_t
qst_com_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    RECT        win;
    sQstComm*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 获取窗口位置 */
    ctx = (sQstComm*)parm;
    if (!GetWindowRect(ctx->hwnd, &win))
        return (FALSE);

    sint_t  x1 = win.left;
    sint_t  y1 = win.top;
    uint_t  ww = win.right - win.left;
    uint_t  hh = win.bottom - win.top;

    /* 保存窗口位置 */
    return (misc_desk_save(WIN_ICONF, x1, y1, ww, hh));
}

/*
---------------------------------------
    加载窗口位置
---------------------------------------
*/
static bool_t
qst_com_win_load (
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

    sQstComm*   ctx;

    /* 设置窗口位置 */
    ctx = (sQstComm*)parm;
    misc_bring2top(ctx->hwnd, NULL);
    return (SetWindowPos(ctx->hwnd, HWND_TOP, (int)x1, (int)y1,
                 (int)ww, (int)hh, SWP_SHOWWINDOW));
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit",  qst_com_app_exit },
    { "win:load",  qst_com_win_load },
    { "win:save",  qst_com_win_save },
    { "win:show",  qst_com_win_show },

    /***** 私有命令映射 *****/
    { "qcom:app:exit", qst_com_app_exit },
    { "qcom:win:show", qst_com_win_show },
};

/*
=======================================
    工作线程
=======================================
*/
CR_API uint_t STDCALL
qst_com_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    sQstComm*   ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_com_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    ctx = (sQstComm*)param;

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
