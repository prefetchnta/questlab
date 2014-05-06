
#include "QstComm.h"
#pragma warning (disable: 4127)
#pragma warning (disable: 4512)

#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

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

    CR_NOUSE(curt_app);
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
    misc_desk_init(WIN_ICONF, &x1, &y1, &ww, &hh,
                   QCOM_DEF_WIDTH, QCOM_DEF_HEIGHT);
    if (ww < QCOM_DEF_WIDTH)  ww = QCOM_DEF_WIDTH;
    if (hh < QCOM_DEF_HEIGHT) hh = QCOM_DEF_HEIGHT;

    QSize           wsize;
    QMainWindow     qt_win;

    qt_win.setMinimumSize(QCOM_DEF_WIDTH, QCOM_DEF_HEIGHT);
    qt_win.setWindowFlags(qt_win.windowFlags()
        & (~Qt::WindowMaximizeButtonHint));
    qt_win.move(x1, y1);
    qt_win.resize(ww, hh);
    wsize = qt_win.frameSize();
    ww -= (wsize.width()  - ww);
    hh -= (wsize.height() - hh);
    qt_win.resize(ww, hh);

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
/*
    thrd = thread_new(0, qst_com_main, &s_wrk_ctx, FALSE);
    if (thrd == NULL)
        return (QST_ERROR);
*/
    /* 开始 QT 流程 */
    qt_win.show();
    qt_app.exec();

    /* 等待线程结束 */
    thread_wait(thrd);
    thread_del(thrd);
    netw_cli_close(s_wrk_ctx.netw);
    return (QST_OKAY);
}
