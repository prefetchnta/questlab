
#include "QstView2D.h"

/* 外部库引用 */
#pragma comment (lib, "CrH_GFX2.lib")
#pragma comment (lib, "CrH_MATH.lib")
#pragma comment (lib, "GFX2_GDI.lib")
#pragma comment (lib, "ResLoader.lib")

/* 缺省的窗口大小 */
#define QV2D_DEF_WIDTH  800
#define QV2D_DEF_HEIGHT 300

/* 鼠标坐标拆分宏 */
#define GET_X_LPARAM(lp)    ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)    ((int)(short)HIWORD(lp))

/* 全局工作上下文 */
static sQstView2D   s_wrk_ctx;

/* 工作线程的声明 */
CR_API uint_t STDCALL qst_v2d_main (void_t *param);

/* 内部函数的声明 */
CR_API void_t   qst_load_cfg (sQV2D_conf *cfgs);
CR_API void_t   qst_load_filter (sQstView2D *parm);
CR_API void_t   qst_repaint_win (sQstView2D *parm);
CR_API bool_t   qst_resize_win (sQstView2D *parm);
CR_API void_t   qst_do_keyboard (sQstView2D *parm);
CR_API void_t   qst_do_mouse (sQstView2D *parm, sint_t m_x,
                              sint_t m_y, bool_t down);
/*
=======================================
    Windows 窗口消息处理
=======================================
*/
LRESULT CALLBACK
WindowProc (
  __CR_IN__ HWND    hwnd,
  __CR_IN__ UINT    message,
  __CR_IN__ WPARAM  wparam,
  __CR_IN__ LPARAM  lparam
    )
{
    MINMAXINFO*     mnmx;
    static uint_t   fw = 0;
    static uint_t   fh = 0;
    uint_t          ww, hh;

    switch (message)
    {
        default:
            break;

        /* 鼠标移动处理 */
        case WM_MOUSEMOVE:
            qst_do_mouse(&s_wrk_ctx, GET_X_LPARAM(lparam),
                GET_Y_LPARAM(lparam), wparam == MK_LBUTTON);
            return (FALSE);

        /* 限制窗口最小尺寸 */
        case WM_GETMINMAXINFO:
            mnmx = (MINMAXINFO*)lparam;
            mnmx->ptMinTrackSize.x = QV2D_DEF_WIDTH;
            mnmx->ptMinTrackSize.y = QV2D_DEF_HEIGHT;
            return (FALSE);

        /* 窗口改变大小后需要重建后台 */
        case WM_SIZE:
            ww = LOWORD(lparam);
            hh = HIWORD(lparam);
            if (ww == 0 || hh == 0)
                return (FALSE);
            if (ww == fw && hh == fh)
                return (FALSE);
            if (!qst_resize_win(&s_wrk_ctx))
                return (FALSE);
            fw = ww;
            fh = hh;
            return (FALSE);

        /* 使用自己的方法清屏 */
        case WM_ERASEBKGND:
            return (FALSE);

        /* 根据需要刷新前台窗口 */
        case WM_PAINT:
            qst_repaint_win(&s_wrk_ctx);
            return (FALSE);

        /* 关闭窗口退出应用程序 */
        case WM_CLOSE:
            PostQuitMessage(0);
            return (FALSE);
    }
    return (DefWindowProcA(hwnd, message, wparam, lparam));
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

    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 生成一个可变大小的窗口 */
    mtlock_init(&s_wrk_ctx.lock);
    qst_load_cfg(&s_wrk_ctx.cfgs);
    misc_desk_init(WIN_ICONF, &x1, &y1, &ww, &hh,
                   QV2D_DEF_WIDTH, QV2D_DEF_HEIGHT);
    if (ww < QV2D_DEF_WIDTH)  ww = QV2D_DEF_WIDTH;
    if (hh < QV2D_DEF_HEIGHT) hh = QV2D_DEF_HEIGHT;
    s_wrk_ctx.hwnd = (HWND)window_open(curt_app, (void_t*)WindowProc,
                        x1, y1, ww, hh, WIN_TITLE, WIN_CLASS, (ansi_t*)101,
                                    CR_WSTYLE_NORMAL);
    if (s_wrk_ctx.hwnd == NULL)
        return (QST_ERROR);
    SetWindowLongPtr(s_wrk_ctx.hwnd, GWL_STYLE,
        GetWindowLongPtr(s_wrk_ctx.hwnd, GWL_STYLE) & (~WS_MAXIMIZEBOX));

    iGFX2*  draw;
    sIMAGE* imgs;

    /* 创建 GDI 绘制对象 (只支持32位色屏幕) */
    draw = (iGFX2*)create_gdi_canvas(s_wrk_ctx.hwnd, 0, 0, FALSE);
    if (draw == NULL) {
        window_kill(s_wrk_ctx.hwnd, curt_app, WIN_CLASS);
        return (QST_ERROR);
    }
    imgs = CR_VCALL(draw)->lock(draw);
    if (imgs == NULL || imgs->fmt != CR_ARGB8888) {
        window_kill(s_wrk_ctx.hwnd, curt_app, WIN_CLASS);
        return (QST_ERROR);
    }
    CR_VCALL(draw)->unlock(draw);
    CR_VCALL(draw)->clear(draw, s_wrk_ctx.cfgs.bkcolor, 0);
    s_wrk_ctx.draw = draw;

    /* 初始化网络 */
    if (!socket_init()) {
        window_kill(s_wrk_ctx.hwnd, curt_app, WIN_CLASS);
        return (QST_ERROR);
    }
    s_wrk_ctx.netw = netw_cli_open(EXE_XNAME);
    if (s_wrk_ctx.netw == NULL) {
        window_kill(s_wrk_ctx.hwnd, curt_app, WIN_CLASS);
        return (QST_ERROR);
    }
    /* 读取需要超时, 不然线程无法退出 */
    socket_set_timeout(s_wrk_ctx.netw, -1, QST_TCP_TOUT);

    thrd_t  thrd;

    /* 生成工作线程 */
    s_wrk_ctx.send = TRUE;
    s_wrk_ctx.quit = FALSE;
    s_wrk_ctx.cur_busy = LoadCursor(NULL, IDC_WAIT);
    s_wrk_ctx.cur_free = LoadCursor(NULL, IDC_ARROW);
    s_wrk_ctx.res_loader = res_loader_get();
    if (s_wrk_ctx.res_loader->init != NULL)
        s_wrk_ctx.res_loader->init(s_wrk_ctx.netw, NULL);
    qst_load_filter(&s_wrk_ctx);
    thrd = thread_new(0, qst_v2d_main, &s_wrk_ctx, FALSE);
    if (thrd == NULL) {
        window_kill(s_wrk_ctx.hwnd, curt_app, WIN_CLASS);
        return (QST_ERROR);
    }

    /* 消息循环 */
    while (!s_wrk_ctx.quit)
    {
        MSG     msg;

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                s_wrk_ctx.quit = TRUE;
                break;
            }
            if (msg.message == WM_QV2D_SET_WINDOW_TITLE) {
                SetWindowTextA(s_wrk_ctx.hwnd, (ansi_t*)msg.lParam);
            }
            else
            if (msg.message == WM_QV2D_WIN_BRING_TO_TOP) {
                misc_bring2top(s_wrk_ctx.hwnd, NULL);
            }
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else {
            qst_do_keyboard(&s_wrk_ctx);
        }
    }
    thread_wait(thrd);
    thread_del(thrd);
    window_kill(s_wrk_ctx.hwnd, curt_app, WIN_CLASS);
    netw_cli_close(s_wrk_ctx.netw);
    return (QST_OKAY);
}
