
#include "xIMGLAB.h"

/* 外部库引用 */
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "CrH_BLIT.lib")
#endif

/* 显示画面索引 */
static uint_t   s_img_idx;

/* 执行源的几个接口类型 */
typedef bool_t  (*ilab_init_t) (uint_t, ansi_t**);
typedef bool_t  (*ilab_main_t) (sILAB_OUTPUT*, const sILAB_INPUT*);
typedef void_t  (*ilab_kill_t) (void_t);

/* OpenCV 转换工具函数类型 */
typedef bool_t  (*ipl2img_set_t) (sIMAGE*, const ipls_t*);
typedef bool_t  (*img2ipl_set_t) (ipls_t*, const sIMAGE*);
typedef sIMAGE* (*ipl2img_dup_t) (const ipls_t*);
typedef ipls_t* (*img2ipl_dup_t) (const sIMAGE*);

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
    switch (message)
    {
        default:
            break;

        /* 按键处理 */
        case WM_KEYUP:
            if (wparam == VK_LEFT) {
                if (s_img_idx != 0)
                    s_img_idx -= 1;
            }
            else
            if (wparam == VK_RIGHT) {
                if (s_img_idx != (uint_t)-1)
                    s_img_idx += 1;
            }
            else
            if (wparam == VK_HOME) {
                s_img_idx = 0;
            }
            else
            if (wparam == VK_END) {
                s_img_idx = (uint_t)-1;
            }
            else
            if (wparam == VK_ESCAPE) {
                PostQuitMessage(0);
            }
            return (FALSE);

        /* 使用自己的方法清屏 */
        case WM_ERASEBKGND:
            return (FALSE);

        /* 关闭窗口退出应用程序 */
        case WM_CLOSE:
            PostQuitMessage(0);
            return (FALSE);
    }
    return (DefWindowProcA(hwnd, message, wparam, lparam));
}

/*
---------------------------------------
    加载滤镜插件并注册接口
---------------------------------------
*/
static bool_t
filter_loader (
  __CR_IN__ void_t*     param,
  __CR_IN__ sSEARCHa*   finfo
    )
{
    sbin_t      dll;
    ansi_t*     name;
    sXC_PORT*   port;

    /* 过滤文件名和大小 */
    if (finfo->size <= CR_K2B(2))
        return (TRUE);
    name = finfo->name + str_lenA(QST_PATH_PLUGIN);
    if (name[0] != CR_AC('f') && name[0] != CR_AC('x'))
        return (TRUE);

    /* 加载并获取接口列表 */
    dll = sbin_loadA(finfo->name);
    if (dll == NULL)
        return (TRUE);
    port = sbin_exportT(dll, "qst_v2d_filter", sXC_PORT*);
    if (port == NULL) {
        sbin_unload(dll);
        return (TRUE);
    }
    xmlcall_setup((xmlcaller_t)param, port);
    return (TRUE);
}

/*
---------------------------------------
    释放结果列表
---------------------------------------
*/
static void_t
free_img_list (
  __CR_IO__ sILAB_OUTPUT*   output
    )
{
    uint_t  idx;

    if (output->frame != NULL) {
        for (idx = 0; idx < output->n_img; idx++)
            image_del(output->frame[idx]);
        mem_free(output->frame);
    }
    if (output->texts != NULL) {
        for (idx = 0; idx < output->n_txt; idx++) {
            if (output->texts[idx].kll)
                mem_free(output->texts[idx].txt);
        }
        mem_free(output->texts);
    }
    struct_zero(output, sILAB_OUTPUT);
}

/* 启动窗口大小 */
#define DEF_WW  640
#define DEF_HH  480

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

    /* 获取命令行参数, 不包括进程文件名 */
    argv = misc_get_param(cmd_line, &argc);

    /* 参数解析 <执行源> <输入源> */
    if (argc < 2)
        return (QST_ERROR);
    s_img_idx = 0;

    sbin_t      sbin;
    ilab_init_t ilab_init;
    ilab_kill_t ilab_kill;
    ilab_main_t ilab_main;

    /* 加载执行源插件 */
    sbin = sbin_loadA(argv[0]);
    if (sbin == NULL) {
        ilab_init = NULL;
        ilab_kill = NULL;
        ilab_main = NULL;
    }
    else {
        ilab_init = sbin_exportT(sbin, "ilab_init", ilab_init_t);
        ilab_kill = sbin_exportT(sbin, "ilab_kill", ilab_kill_t);
        ilab_main = sbin_exportT(sbin, "ilab_main", ilab_main_t);
        if ((ilab_init != NULL) &&
            !ilab_init(argc - 2, &argv[2]))
            return (QST_ERROR);
    }

    sENGINE*        port;
    sILAB_INPUT     ilab_inpt;
    engine_init_t   fmtz_func;

    /* 加载输入源 (只使用 xOpenCV 插件) */
    sbin = sbin_loadA(QST_PATH_PLUGIN "xOpenCV.dll");
    if (sbin == NULL)
        return (QST_ERROR);
    fmtz_func = sbin_exportT(sbin, "engine_get", engine_init_t);
    if (fmtz_func == NULL)
        return (QST_ERROR);
    if ((port = fmtz_func()) == NULL)
        return (QST_ERROR);
    port->mask = CR_FMTZ_MASK_PIC;

    /* 导入 OpenCV 转换工具函数 */
    ilab_inpt.ilab_ipl2img_set = sbin_exportT(sbin,
                                "ilab_ipl2img_set", ipl2img_set_t);
    ilab_inpt.ilab_img2ipl_set = sbin_exportT(sbin,
                                "ilab_img2ipl_set", img2ipl_set_t);
    ilab_inpt.ilab_ipl2img_dup = sbin_exportT(sbin,
                                "ilab_ipl2img_dup", ipl2img_dup_t);
    ilab_inpt.ilab_img2ipl_dup = sbin_exportT(sbin,
                                "ilab_img2ipl_dup", img2ipl_dup_t);
    if (ilab_inpt.ilab_ipl2img_set == NULL ||
        ilab_inpt.ilab_img2ipl_set == NULL ||
        ilab_inpt.ilab_ipl2img_dup == NULL ||
        ilab_inpt.ilab_img2ipl_dup == NULL)
        return (QST_ERROR);

    create_gfx2_t       gfx2_func;
    const sGDI_CALL*    gdi_calls;

    /* 加载 GDI 绘制插件 (限制到 GDI) */
    sbin = sbin_loadA("GFX2_GDI.dll");
    if (sbin == NULL)
        return (QST_ERROR);
    gfx2_func = sbin_exportT(sbin, "create_canvas", create_gfx2_t);
    if (gfx2_func == NULL)
        return (QST_ERROR);
    gdi_calls = sbin_callgetT(sbin, "gdi_call_get", sGDI_CALL);
    if (gdi_calls == NULL)
        return (QST_ERROR);

    HWND    hwnd;

    /* 生成一个固定大小的窗口 */
    hwnd = (HWND)window_open(curt_app, (void_t*)WindowProc,
                0, 0, DEF_WW, DEF_HH, WIN_TITLE, WIN_CLASS, (ansi_t*)101,
                             CR_WSTYLE_FIXED);
    if (hwnd == NULL)
        return (QST_ERROR);
    SetWindowLongPtr(hwnd, GWL_STYLE,
        GetWindowLongPtr(hwnd, GWL_STYLE) & (~WS_MAXIMIZEBOX));

    sFMTZ*      fmtz;
    sLOADER     ldrs;
    sFMT_PIC*   picz;
    iPICTURE*   slide;

    /* 获取一帧图像数据 */
    set_ldrA(&ldrs, argv[1], "", 0, 0);
    fmtz = engine_do(port, &ldrs);
    if (fmtz == NULL) {
        window_kill(hwnd, curt_app, WIN_CLASS);
        return (QST_ERROR);
    }
    if (fmtz->type != CR_FMTZ_PIC && fmtz->type != CR_FMTZ_PRT) {
        fmtz_free(fmtz);
        window_kill(hwnd, curt_app, WIN_CLASS);
        return (QST_ERROR);
    }
    if (fmtz->type == CR_FMTZ_PIC) {
        slide = NULL;
        picz = (sFMT_PIC*)fmtz;
    }
    else {
        if (str_cmpA(((sFMT_PRT*)fmtz)->more, "iPICTURE") != 0) {
            fmtz_free(fmtz);
            window_kill(hwnd, curt_app, WIN_CLASS);
            return (QST_ERROR);
        }
        slide = (iPICTURE*)(((sFMT_PRT*)fmtz)->port);
        picz = CR_VCALL(slide)->get(slide, 0);
        if (picz == NULL) {
            fmtz_free(fmtz);
            window_kill(hwnd, curt_app, WIN_CLASS);
            return (QST_ERROR);
        }
    }

    RECT    rect;
    sint_t  x1, y1;
    uint_t  ww, hh;
    uint_t  sw, sh;

    /* 根据图片大小调整窗口大小 */
    GetClientRect(hwnd, &rect);
    ww = picz->frame->pic->position.ww - rect.right;
    hh = picz->frame->pic->position.hh - rect.bottom;
    ww += DEF_WW;
    hh += DEF_HH;
    if (SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0)) {
        sw = rect.right - rect.left;
        sh = rect.bottom - rect.top;
    }
    else {
        sw = GetSystemMetrics(SM_CXSCREEN);
        sh = GetSystemMetrics(SM_CYSCREEN);
    }
    if (sw < ww) ww = sw;
    if (sh < hh) hh = sh;
    x1 = (sint_t)((sw - ww) / 2);
    y1 = (sint_t)((sh - hh) / 2);
    SetWindowPos(hwnd, HWND_TOP, x1, y1, ww, hh, SWP_SHOWWINDOW);

    iGFX2*  draw;
    sIMAGE* imgs;

    /* 创建 GDI 绘制对象 (只支持32位色屏幕) */
    draw = gfx2_func(hwnd, 0, 0, CR_UNKNOWN, FALSE, NULL, 0);
    if (draw == NULL) {
        fmtz_free(fmtz);
        window_kill(hwnd, curt_app, WIN_CLASS);
        return (QST_ERROR);
    }
    imgs = CR_VCALL(draw)->lock(draw);
    if (imgs == NULL || imgs->fmt != CR_ARGB8888) {
        fmtz_free(fmtz);
        window_kill(hwnd, curt_app, WIN_CLASS);
        return (QST_ERROR);
    }
    CR_VCALL(draw)->unlock(draw);
    CR_VCALL(draw)->clear(draw, 0, 0);

    iFONT*      font;
    LOGFONTA    fnta;

    /* 创建输出文本用的文字绘制对象 */
    struct_zero(&fnta, LOGFONTA);
    fnta.lfCharSet = DEFAULT_CHARSET;
    fnta.lfQuality = ANTIALIASED_QUALITY;
    fnta.lfHeight = 12;
    str_cpyA(fnta.lfFaceName, "Fixedsys");
    font = gdi_calls->create_fontA(&fnta);
    if (font == NULL || !CR_VCALL(font)->bind(font, draw)) {
        fmtz_free(fmtz);
        window_kill(hwnd, curt_app, WIN_CLASS);
        return (QST_ERROR);
    }

    sILAB_OUTPUT    ilab_outp;

    /* 初始化执行源 */
    ilab_inpt.input = img_auto_to_32(NULL, 0, 0, picz->frame->pic);
    if (ilab_inpt.input == NULL) {
        fmtz_free(fmtz);
        window_kill(hwnd, curt_app, WIN_CLASS);
        return (QST_ERROR);
    }
    ilab_inpt.filter = xmlcall_load(NULL, 0, NULL);
    if (ilab_inpt.filter != NULL)
    {
        /* 加载滤镜接口列表 (可选功能, 使用之可少写一些代码) */
        file_searchA(QST_PATH_PLUGIN, FALSE, TRUE, FALSE,
                     "*.dll", filter_loader, ilab_inpt.filter);
    }
    fmtz_free((sFMTZ*)picz);
    struct_zero(&ilab_outp, sILAB_OUTPUT);

    timer_t log = timer_new();

    /* 消息循环 */
    for (;;)
    {
        MSG     msg;

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            sBLIT   blit;
            sIMAGE* show;
            ansi_t  buf[128];

            /* 执行图片数据处理 */
            if (ilab_main != NULL) {
                free_img_list(&ilab_outp);
                if (log != NULL)
                    timer_set_base(log);
                if (!ilab_main(&ilab_outp, &ilab_inpt))
                    break;
                if (log != NULL) {
                    sprintf(buf, WIN_TITLE " - cost: %.3f ms",
                                timer_get_delta(log));
                    SetWindowTextA(hwnd, buf);
                }
                if (s_img_idx >= ilab_outp.n_img)
                    s_img_idx = ilab_outp.n_img - 1;
                show = img_auto_to_32(NULL, 0, 0, ilab_outp.frame[s_img_idx]);
                if (show == NULL)
                    break;
            }
            else {
                s_img_idx = 0;
                show = ilab_inpt.input;
            }

            /* 输出调试图像 */
            blit.dx = blit.dy = 0;
            blit.sx = blit.sy = 0;
            blit.sw = show->position.ww;
            blit.sh = show->position.hh;
            imgs = CR_VCALL(draw)->lock(draw);
            blit_set_c(imgs, show, &blit, NULL);
            CR_VCALL(draw)->unlock(draw);
            if (show != ilab_inpt.input)
                image_del(show);

            /* 输出文字信息 */
            if (ilab_outp.texts != NULL) {
                for (uint_t idx = 0; idx < ilab_outp.n_txt; idx++) {
                    if (ilab_outp.texts[idx].idx != s_img_idx)
                        continue;
                    if (ilab_outp.texts[idx].trn)
                        CR_VCALL(font)->setMode(font, TRANSPARENT);
                    else
                        CR_VCALL(font)->setMode(font, OPAQUE);
                    CR_VCALL(font)->setColor(font, ilab_outp.texts[idx].fgc);
                    CR_VCALL(font)->setBkColor(font, ilab_outp.texts[idx].bkc);
                    CR_VCALL(font)->enter(font);
                    egui_draw_text(font, ilab_outp.texts[idx].txt,
                                        &ilab_outp.texts[idx].pos,
                                         ilab_outp.texts[idx].aln,
                                         ilab_outp.texts[idx].pge);
                    CR_VCALL(font)->leave(font);
                }
            }
            CR_VCALL(draw)->flip(draw, FALSE);

            /* 获取多帧图片的后续帧 */
            if (slide != NULL) {
                picz = CR_VCALL(slide)->get(slide, 0);
                if (picz == NULL)
                    break;
                image_del(ilab_inpt.input);
                ilab_inpt.input = img_auto_to_32(NULL, 0, 0, picz->frame->pic);
                fmtz_free((sFMTZ*)picz);
                if (ilab_inpt.input == NULL)
                    break;
            }
            thread_sleep(1);
        }
    }

    /* 销毁执行源 */
    if (ilab_kill != NULL)
        ilab_kill();
    if (slide != NULL)
        fmtz_free(fmtz);
    window_kill(hwnd, curt_app, WIN_CLASS);
    return (QST_OKAY);
}
