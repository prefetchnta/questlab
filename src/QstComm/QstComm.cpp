
#include "QstCommInt.h"

/* 外部库引用 */
#pragma comment (lib, "Qt5Core.lib")
#pragma comment (lib, "Qt5Gui.lib")
#pragma comment (lib, "Qt5Widgets.lib")

/* 缺省的窗口大小 */
#define QCOM_DEF_WIDTH  800
#define QCOM_DEF_HEIGHT 300

/* 全局工作上下文 */
static sQstComm     s_wrk_ctx;

/* 工作线程的声明 */
CR_API uint_t STDCALL qst_com_main (void_t *param);

/* 内部函数的声明 */
CR_API void_t   qst_load_cfg (sQCOM_conf *cfgs);
CR_API void_t   qst_set_viewer (sQstComm *parm);

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
    uint_t      argc;
    ansi_t**    argv;

    CR_NOUSE(prev_app);
    CR_NOUSE(cmd_show);

    /* 只允许一个例程 */
    if (misc_is_running(EXE_XNAME))
        return (QST_ERROR);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_GUI))
        return (QST_ERROR);
    mem_zero(&s_wrk_ctx, sizeof(s_wrk_ctx));

    int qt_argc;

    /* 获取命令行参数, 不包括进程文件名 */
    argv = misc_get_param(cmd_line, &argc);
    qt_argc = (int)argc;

    QApplication    qt_app(qt_argc, argv);

    qt_app.setApplicationName("QstComm");
    qt_app.setOrganizationName("QuestLAB");

    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 生成一个可变大小的窗口 */
    mtlock_init(&s_wrk_ctx.lock);
    qst_load_cfg(&s_wrk_ctx.cfgs);
    misc_desk_init(WIN_ICONF, &x1, &y1, &ww, &hh,
                   QCOM_DEF_WIDTH, QCOM_DEF_HEIGHT);
    if (ww < QCOM_DEF_WIDTH)  ww = QCOM_DEF_WIDTH;
    if (hh < QCOM_DEF_HEIGHT) hh = QCOM_DEF_HEIGHT;

    RECT            w_rect;
    QMainWindow     qt_win;

    /* Qt 里的宽高都不包括边框
       需要自己用 Win32 API 获取 */
    qt_win.setWindowFlags(qt_win.windowFlags()
        & (~Qt::WindowMaximizeButtonHint));
    qt_win.move(x1, y1);
    qt_win.resize(ww, hh);
    s_wrk_ctx.hwnd = (HWND)qt_win.winId();
    if (!GetWindowRect(s_wrk_ctx.hwnd, &w_rect))
        return (QST_ERROR);
    s_wrk_ctx.fw = w_rect.right - w_rect.left - ww;
    s_wrk_ctx.fh = w_rect.bottom - w_rect.top - hh;
    qt_win.setMinimumSize(QCOM_DEF_WIDTH  - s_wrk_ctx.fw,
                          QCOM_DEF_HEIGHT - s_wrk_ctx.fh);
    qt_win.resize(ww - s_wrk_ctx.fw, hh - s_wrk_ctx.fh);

    QWidget*        cent = new QWidget (&qt_win);
    QTextEdit*      edit = new QTextEdit (cent);
    QHBoxLayout*    hori = new QHBoxLayout (cent);

    /* 创建窗体里的控件 */
    hori->setSpacing(6);
    hori->setContentsMargins(8, 8, 8, 8);
    hori->addWidget(edit);
    qt_win.setCentralWidget(cent);
    s_wrk_ctx.view = (void_t*)(edit);
    s_wrk_ctx.form = (void_t*)(&qt_win);
    qst_set_viewer(&s_wrk_ctx);
    SetClassLongPtr(s_wrk_ctx.hwnd, GCLP_HICON, (LONG_PTR)
                    LoadIconA(curt_app, (ansi_t*)101));
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
    thrd = thread_new(0, qst_com_main, &s_wrk_ctx, FALSE);
    if (thrd == NULL)
        return (QST_ERROR);

    /* 开始 Qt 流程 */
    qt_win.show();
    qt_app.exec();

    /* 关闭线程直接退出 */
    if (!s_wrk_ctx.quit)
        s_wrk_ctx.quit = TRUE;
    thread_wait(thrd);
    thread_del(thrd);
    netw_cli_close(s_wrk_ctx.netw);
    return (QST_OKAY);
}
