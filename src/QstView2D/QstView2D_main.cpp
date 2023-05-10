
#include "QstView2D.h"

/* 多线程锁简化宏 */
#define _ENTER_V2D_SINGLE_  \
    mtlock_acquire(&(((sQstView2D*)parm)->lock));
#define _LEAVE_V2D_SINGLE_  \
    mtlock_release(&(((sQstView2D*)parm)->lock));

/* 内部函数的声明 */
CR_API void_t   qst_draw_image (sQstView2D *parm);
CR_API void_t   qst_make_image (sQstView2D *parm);
CR_API void_t   qst_set_index (sQstView2D *parm, int32u index);
CR_API void_t   qst_render_data (sQstView2D *parm, sLOADER *ldrs);
CR_API void_t   qst_move_xy (sQstView2D *parm, sint_t delta_x, sint_t delta_y);
CR_API bool_t   qst_save_now (const sQstView2D *parm, const ansi_t *name,
                              uint_t argc, ansi_t *argv[]);
CR_API bool_t   qst_save_all (const sQstView2D *parm, const ansi_t *name,
                              uint_t argc, ansi_t *argv[]);
CR_API bool_t   qst_save_min (const sQstView2D *parm, const ansi_t *name,
                              uint_t argc, ansi_t *argv[]);
CR_API bool_t   qst_save_show (const sQstView2D *parm, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   qst_save_show2 (const sQstView2D *parm, const ansi_t *name,
                                uint_t argc, ansi_t *argv[], const sRECT *box,
                                fp32_t ccw, fp32_t scale, bool_t lerp);
CR_API bool_t   qst_save_show3 (const sQstView2D *parm, const ansi_t *name,
                                uint_t argc, ansi_t *argv[], uint_t tilew,
                                uint_t tileh, uint_t stepx, uint_t stepy);
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
=======================================
    读入配置文件
=======================================
*/
CR_API void_t
qst_load_cfg (
  __CR_OT__ sQV2D_conf* cfgs
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
    cfgs->bkcolor = ini_key_intx32U("qv2d::bkcolor", 0x007F7F7FUL, ini);
    cfgs->is_blends = ini_key_intxU("qv2d::is_blends", TRUE, ini);
    cfgs->is_center = ini_key_intxU("qv2d::is_center", TRUE, ini);
    cfgs->use_keycolor = ini_key_intxU("qv2d::use_keycolor", TRUE, ini);
    cfgs->def_keycolor = ini_key_intx32U("qv2d::def_keycolor",
                            0x00000000UL, ini);
    ini_closeU(ini);
    return;

_load_defs:
    cfgs->bkcolor = 0x007F7F7FUL;
    cfgs->is_blends = TRUE;
    cfgs->is_center = TRUE;
    cfgs->use_keycolor = TRUE;
    cfgs->def_keycolor = 0x00000000UL;
}

/*
=======================================
    释放当前浏览图片
=======================================
*/
CR_API void_t
qst_clear (
  __CR_IO__ sQstView2D* parm
    )
{
    if (parm->fmtz != NULL) {
        fmtz_free(parm->fmtz);
        parm->fmtz = NULL;
    }
    if (parm->pictz != NULL) {
        fmtz_free((sFMTZ*)parm->pictz);
        parm->pictz = NULL;
        parm->slide = NULL;
    }
    if (parm->paint != NULL) {
        image_del(parm->paint);
        parm->paint = NULL;
    }
}

/*
=======================================
    刷新前台窗口
=======================================
*/
CR_API void_t
qst_repaint_win (
  __CR_IO__ sQstView2D* parm
    )
{
    HDC         hdc;
    HBRUSH      hbr;
    int32u      clr;
    iGFX2_GDI*  draw;
    PAINTSTRUCT paint;

    /* 后台缓冲已经建立, 刷新前台区域 */
    hdc = BeginPaint(parm->hwnd, &paint);
    if (parm->draw != NULL) {
        draw = (iGFX2_GDI*)parm->draw;
        BitBlt(hdc, 0, 0, draw->__back__.position.ww,
               draw->__back__.position.hh, draw->m_back,
               0, 1, SRCCOPY);
    }
    else {
        clr = parm->cfgs.bkcolor;
        hbr = CreateSolidBrush(argb32_to_gdi(&clr));
        if (hbr != NULL) {
            FillRect(hdc, &paint.rcPaint, hbr);
            DeleteObject(hbr);
        }
    }
    EndPaint(parm->hwnd, &paint);
}

/*
=======================================
    窗口改变大小处理
=======================================
*/
CR_API bool_t
qst_resize_win (
  __CR_IO__ sQstView2D* parm
    )
{
    bool_t  rett;

    if (parm->draw == NULL)
        return (TRUE);
    _ENTER_V2D_SINGLE_
    rett = CR_VCALL(parm->draw)->reset(parm->draw);
    if (rett) {
        qst_move_xy(parm, 0, 0);
        qst_draw_image(parm);
    }
    _LEAVE_V2D_SINGLE_
    return (rett);
}

/*
---------------------------------------
    窗口是否激活
---------------------------------------
*/
static bool_t
qst_is_active_win (
  __CR_IN__ const sQstView2D*   parm
    )
{
    if (GetActiveWindow() == parm->hwnd)
        return (TRUE);
    return (FALSE);
}

/*
=======================================
    键盘按键处理
=======================================
*/
CR_API void_t
qst_do_keyboard (
  __CR_IO__ sQstView2D* parm
    )
{
    sRECT*  window;
    sint_t  step_x;
    sint_t  step_y;
    sint_t  delta_x;
    sint_t  delta_y;
    bool_t  pressed;

    /* 窗口激活才响应按键 */
    if (!qst_is_active_win(parm)) {
        thread_sleep(1);
        return;
    }
    pressed = FALSE;
    delta_x = delta_y = 0;
    if (key_input_test('W')) {
        delta_y -= 1;
        pressed = TRUE;
    }
    if (key_input_test('S')) {
        delta_y += 1;
        pressed = TRUE;
    }
    if (key_input_test('A')) {
        delta_x -= 1;
        pressed = TRUE;
    }
    if (key_input_test('D')) {
        delta_x += 1;
        pressed = TRUE;
    }
    if (pressed) {
        window = &parm->draw->__back__.position;
        step_x = (sint_t)(window->ww / 200);
        step_y = (sint_t)(window->hh / 200);
        delta_x *= (step_x == 0) ? 1 : step_x;
        delta_y *= (step_y == 0) ? 1 : step_y;
        _ENTER_V2D_SINGLE_
        qst_move_xy(parm, delta_x, delta_y);
        _LEAVE_V2D_SINGLE_
    }
    else
    if (key_input_click('Z')) {
        _ENTER_V2D_SINGLE_
        parm->trans = FALSE;
        qst_draw_image(parm);
        _LEAVE_V2D_SINGLE_
    }
    else
    if (key_input_click('X')) {
        _ENTER_V2D_SINGLE_
        parm->trans = TRUE;
        qst_draw_image(parm);
        _LEAVE_V2D_SINGLE_
    }
    else
    if (key_input_test('Q')) {
        _ENTER_V2D_SINGLE_
        if (parm->index != 0)
            qst_set_index(parm, parm->index - 1);
        _LEAVE_V2D_SINGLE_
        thread_sleep(50);
    }
    else
    if (key_input_test('E')) {
        _ENTER_V2D_SINGLE_
        qst_set_index(parm, parm->index + 1);
        _LEAVE_V2D_SINGLE_
        thread_sleep(50);
    }
    else {
        thread_sleep(1);
    }
}

/*
=======================================
    鼠标移动处理
=======================================
*/
CR_API void_t
qst_do_mouse (
  __CR_IO__ sQstView2D* parm,
  __CR_IN__ sint_t      m_x,
  __CR_IN__ sint_t      m_y,
  __CR_IN__ bool_t      down
    )
{
    sint_t          delta_x;
    sint_t          delta_y;
    static sint_t   pre_x = 0;
    static sint_t   pre_y = 0;

    if (!down) {
        pre_x = m_x;
        pre_y = m_y;
        return;
    }
    delta_x = pre_x - m_x;
    delta_y = pre_y - m_y;
    pre_x = m_x;
    pre_y = m_y;
    if (delta_x == 0 && delta_y == 0)
        return;
    _ENTER_V2D_SINGLE_
    qst_move_xy(parm, delta_x, delta_y);
    _LEAVE_V2D_SINGLE_
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
    sQstView2D* parm;

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
    parm = (sQstView2D*)param;
    xmlcall_setup(parm->flt_lst, port);
    return (TRUE);
}

/*
=======================================
    搜索并加载所有滤镜插件
=======================================
*/
CR_API void_t
qst_load_filter (
  __CR_IO__ sQstView2D* parm
    )
{
    /* 创建 XMLCALL 对象 */
    parm->flt_lst = xmlcall_load(NULL, 0, parm->netw);
    if (parm->flt_lst == NULL)
        return;

    /* 搜索所有滤镜插件并加载 */
    file_searchA(QST_PATH_PLUGIN, FALSE, TRUE, FALSE,
                 "*.dll", filter_loader, parm);

    /* 启动64位插件代理程序 */
    misc_call_exe("x64bin\\fQUEST64.exe", FALSE, TRUE);
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
qst_v2d_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(argc);
    CR_NOUSE(argv);
    ((sQstView2D*)parm)->quit = TRUE;
    return (FALSE);
}

/*
---------------------------------------
    显示窗口位置
---------------------------------------
*/
static bool_t
qst_v2d_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstView2D* ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstView2D*)parm;
    misc_bring2top(ctx->hwnd, NULL);
    return (TRUE);
}

/*
---------------------------------------
    保存窗口位置
---------------------------------------
*/
static bool_t
qst_v2d_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    RECT        win;
    sQstView2D* ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 获取窗口位置 */
    ctx = (sQstView2D*)parm;
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
qst_v2d_win_load (
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

    sQstView2D* ctx;

    /* 设置窗口位置 */
    ctx = (sQstView2D*)parm;
    misc_bring2top(ctx->hwnd, NULL);
    return (SetWindowPos(ctx->hwnd, HWND_TOP, (int)x1, (int)y1,
                 (int)ww, (int)hh, SWP_SHOWWINDOW));
}

/*
---------------------------------------
    加载配置文件
---------------------------------------
*/
static bool_t
qst_v2d_cfg_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstView2D* ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    _ENTER_V2D_SINGLE_
    ctx = (sQstView2D*)parm;
    qst_load_cfg(&ctx->cfgs);
    qst_make_image(ctx);
    qst_draw_image(ctx);
    _LEAVE_V2D_SINGLE_
    return (TRUE);
}

/*
---------------------------------------
    释放所有 FMTZ 插件
---------------------------------------
*/
static bool_t
qst_v2d_ext_free (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstView2D* ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstView2D*)parm;
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
qst_v2d_ext_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sbin_t      dll;
    sQstView2D* ctx;

    /* 参数解析 <插件路径> */
    if (argc < 2)
        return (FALSE);

    sENGINE*        port;
    engine_init_t   func;

    /* 获取引擎接口 */
    dll = sbin_loadA(argv[1]);
    if (dll == NULL)
        return (FALSE);
    func = sbin_exportT(dll, "engine_get", engine_init_t);
    if (func == NULL)
        goto _failure;
    if ((port = func()) == NULL)
        goto _failure;
    port->sbin = dll;
    port->mask = CR_FMTZ_MASK_DAT | CR_FMTZ_MASK_PIC;

    /* 压入插件列表 */
    ctx = (sQstView2D*)parm;
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
qst_v2d_ldr_file (
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
    sQstView2D* ctx = (sQstView2D*)parm;

    /* 附加参数 aprm 不设为空 */
    if (argc > 6)
        set_ldrA(&ldr, argv[1], argv[6], head, tail);
    else
        set_ldrA(&ldr, argv[1], "", head, tail);
    if (ctx->res_router != NULL)
        ldr.nprm = (void_t*)(ctx->res_router->load);
    ldr.page = page;

    /* 渲染目标数据 */
    _ENTER_V2D_SINGLE_
    qst_render_data(ctx, &ldr);
    _LEAVE_V2D_SINGLE_

    /* 无论成功失败都返回成功 */
    return (TRUE);
}

/*
---------------------------------------
    加载一个内存文件
---------------------------------------
*/
static bool_t
qst_v2d_ldr_smem (
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
    sQstView2D* ctx = (sQstView2D*)parm;

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
    if (ctx->res_router != NULL)
        ldr.nprm = (void_t*)(ctx->res_router->load);
    ldr.page = page;

    /* 渲染目标数据 */
    _ENTER_V2D_SINGLE_
    qst_render_data(ctx, &ldr);
    _LEAVE_V2D_SINGLE_

    /* 用完后需要释放掉 */
    mem_free(data);
    return (TRUE);
}

/*
---------------------------------------
    设置图片帧号
---------------------------------------
*/
static bool_t
qst_v2d_set_now (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    int32u  idx = 0;

    /* 参数解析 [图片帧号] */
    if (argc > 1)
        idx = str2intx32A(argv[1]);
    _ENTER_V2D_SINGLE_
    ((sQstView2D*)parm)->send = FALSE;
    qst_set_index((sQstView2D*)parm, idx);
    ((sQstView2D*)parm)->send = TRUE;
    _LEAVE_V2D_SINGLE_
    return (TRUE);
}

/*
---------------------------------------
    加载命令插件 (不需要释放)
---------------------------------------
*/
static bool_t
qst_v2d_cmd_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sbin_t      dll;
    sQstView2D* ctx;

    /* 参数解析 <插件路径> */
    if (argc < 2)
        return (FALSE);

    sQST_CMD*   list;

    /* 获取命令表接口 */
    dll = sbin_loadA(argv[1]);
    if (dll == NULL)
        return (FALSE);
    list = sbin_exportT(dll, "qst_v2d_cmdz", sQST_CMD*);
    if (list == NULL) {
        sbin_unload(dll);
        return (FALSE);
    }

    /* 命令加入列表 */
    ctx = (sQstView2D*)parm;
    cmd_exec_addn(ctx->objs, list);
    return (TRUE);
}

/*
---------------------------------------
    加载滤镜脚本
---------------------------------------
*/
static bool_t
qst_v2d_flt_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sXMLu*      xml;
    ansi_t*     str;
    sQstView2D* ctx;

    /* 参数解析 [脚本路径] */
    ctx = (sQstView2D*)parm;
    if (argc > 1) {
        str = file_load_as_strA(argv[1]);
        if (str == NULL)
            return (FALSE);
        xml = xml_parseU(str);
        mem_free(str);
        if (xml == NULL)
            return (FALSE);
    }
    else {
        xml = NULL;
    }
    _ENTER_V2D_SINGLE_
    if (ctx->flt_scr != NULL)
        xml_closeU(ctx->flt_scr);
    ctx->flt_scr = xml;
    qst_make_image(ctx);
    qst_draw_image(ctx);
    _LEAVE_V2D_SINGLE_
    return (TRUE);
}

/*
---------------------------------------
    显示 Alpha 通道
---------------------------------------
*/
static bool_t
qst_v2d_g2d_alpha (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(argc);
    CR_NOUSE(argv);

    _ENTER_V2D_SINGLE_
    ((sQstView2D*)parm)->trans = TRUE;
    qst_draw_image((sQstView2D*)parm);
    _LEAVE_V2D_SINGLE_
    return (TRUE);
}

/*
---------------------------------------
    显示 Color 通道
---------------------------------------
*/
static bool_t
qst_v2d_g2d_color (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(argc);
    CR_NOUSE(argv);

    _ENTER_V2D_SINGLE_
    ((sQstView2D*)parm)->trans = FALSE;
    qst_draw_image((sQstView2D*)parm);
    _LEAVE_V2D_SINGLE_
    return (TRUE);
}

/*
---------------------------------------
    释放当前浏览图片
---------------------------------------
*/
static bool_t
qst_v2d_g2d_clear (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstView2D* ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    _ENTER_V2D_SINGLE_
    ctx = (sQstView2D*)parm;
    qst_clear(ctx);
    qst_make_image(ctx);
    qst_draw_image(ctx);
    _LEAVE_V2D_SINGLE_
    return (TRUE);
}

/*
---------------------------------------
    创建绘图画布对象
---------------------------------------
*/
static bool_t
qst_v2d_g2d_canvas (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t      ww;
    uint_t      hh;
    sQstView2D* ctx;

    /* 参数解析 <画布宽> <画布高> */
    if (argc < 3)
        return (FALSE);
    ww = str2intxA(argv[1]);
    hh = str2intxA(argv[2]);
    if ((sint_t)ww <= 0 || (sint_t)hh <= 0)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    ctx = (sQstView2D*)parm;
    qst_clear(ctx);
    ctx->paint = image_new(0, 0, ww, hh, CR_ARGB8888, FALSE, 4);
    if (ctx->paint != NULL) {
        mem_set(ctx->paint->data, 0xFF, ctx->paint->size);
        qst_make_image(ctx);
        qst_draw_image(ctx);
    }
    _LEAVE_V2D_SINGLE_
    return (TRUE);
}

/*
---------------------------------------
    刷新当前浏览图片
---------------------------------------
*/
static bool_t
qst_v2d_g2d_refresh (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstView2D* ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    _ENTER_V2D_SINGLE_
    ctx = (sQstView2D*)parm;
    qst_make_image(ctx);
    qst_draw_image(ctx);
    _LEAVE_V2D_SINGLE_
    return (TRUE);
}

/*
---------------------------------------
    保存当前浏览图片
---------------------------------------
*/
static bool_t
qst_v2d_g2d_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    bool_t      ret;
    sQstView2D* ctx;

    /* 参数解析 <文件名> [...] */
    if (argc < 2)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    ctx = (sQstView2D*)parm;
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_busy);
    ret = qst_save_now(ctx, argv[1], argc - 2, &argv[2]);
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_free);
    _LEAVE_V2D_SINGLE_
    return (ret);
}

/*
---------------------------------------
    保存当前所有图片
---------------------------------------
*/
static bool_t
qst_v2d_g2d_saveall (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    bool_t      ret;
    sQstView2D* ctx;

    /* 参数解析 <文件名> [...] */
    if (argc < 2)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    ctx = (sQstView2D*)parm;
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_busy);
    ret = qst_save_all(ctx, argv[1], argc - 2, &argv[2]);
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_free);
    _LEAVE_V2D_SINGLE_
    return (ret);
}

/*
---------------------------------------
    保存当前显示图片
---------------------------------------
*/
static bool_t
qst_v2d_g2d_savenow (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    bool_t      ret;
    sQstView2D* ctx;

    /* 参数解析 <文件名> [...] */
    if (argc < 2)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    ctx = (sQstView2D*)parm;
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_busy);
    ret = qst_save_show(ctx, argv[1], argc - 2, &argv[2]);
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_free);
    _LEAVE_V2D_SINGLE_
    return (ret);
}

/*
---------------------------------------
    保存当前显示图片 (局部)
---------------------------------------
*/
static bool_t
qst_v2d_g2d_grab (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT       box;
    bool_t      ret;
    sQstView2D* ctx;

    /* 参数解析 <X> <Y> <Width> <Height> <文件名> [...] */
    if (argc < 6)
        return (FALSE);
    ctx = (sQstView2D*)parm;
    if (ctx->image == NULL)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_busy);
    box.x1 = str2intxA(argv[1]);
    box.y1 = str2intxA(argv[2]);
    box.ww = str2intxA(argv[3]);
    if (box.ww == 0)
        box.ww = ctx->image->position.ww - box.x1;
    box.hh = str2intxA(argv[4]);
    if (box.hh == 0)
        box.hh = ctx->image->position.hh - box.y1;
    rect_set_wh(&box, box.x1, box.y1, box.ww, box.hh);
    ret = qst_save_show2(ctx, argv[5], argc - 6, &argv[6],
                         &box, -1.0f, 0.0f, FALSE);
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_free);
    _LEAVE_V2D_SINGLE_
    return (ret);
}

/*
---------------------------------------
    保存当前显示图片 (缩放)
---------------------------------------
*/
static bool_t
qst_v2d_g2d_zoom (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT       box;
    bool_t      ret;
    fp32_t      mul;
    sQstView2D* ctx;

    /* 参数解析 <X> <Y> <Width> <Height> <Scale> <Lerp> <文件名> [...] */
    if (argc < 8)
        return (FALSE);
    ctx = (sQstView2D*)parm;
    if (ctx->image == NULL)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_busy);
    box.x1 = str2intxA(argv[1]);
    box.y1 = str2intxA(argv[2]);
    box.ww = str2intxA(argv[3]);
    if (box.ww == 0)
        box.ww = ctx->image->position.ww - box.x1;
    box.hh = str2intxA(argv[4]);
    if (box.hh == 0)
        box.hh = ctx->image->position.hh - box.y1;
    rect_set_wh(&box, box.x1, box.y1, box.ww, box.hh);
    mul = str2fp32A(argv[5]);
    ret = str2intxA(argv[6]) ? TRUE : FALSE;
    ret = qst_save_show2(ctx, argv[7], argc - 8, &argv[8],
                         &box, 0.0f, mul, ret);
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_free);
    _LEAVE_V2D_SINGLE_
    return (ret);
}

/*
---------------------------------------
    保存当前显示图片 (旋转)
---------------------------------------
*/
static bool_t
qst_v2d_g2d_rote (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT       box;
    bool_t      ret;
    fp32_t      ccw;
    sQstView2D* ctx;

    /* 参数解析 <X> <Y> <Width> <Height> <CCW> <Lerp> <文件名> [...] */
    if (argc < 8)
        return (FALSE);
    ctx = (sQstView2D*)parm;
    if (ctx->image == NULL)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_busy);
    box.x1 = str2intxA(argv[1]);
    box.y1 = str2intxA(argv[2]);
    box.ww = str2intxA(argv[3]);
    if (box.ww == 0)
        box.ww = ctx->image->position.ww - box.x1;
    box.hh = str2intxA(argv[4]);
    if (box.hh == 0)
        box.hh = ctx->image->position.hh - box.y1;
    rect_set_wh(&box, box.x1, box.y1, box.ww, box.hh);
    ccw = str2fp32A(argv[5]);
    if (ccw < 0.0f) {
        ccw = -ccw;
        ret = TRUE;
    }
    else {
        ret = FALSE;
    }
    while (ccw >= 360.0f)
        ccw -= 360.0f;
    if (ret) {
        ccw = 360.0f - ccw;
        if (ccw >= 360.0f)
            ccw -= 360.0f;
    }
    ret = str2intxA(argv[6]) ? TRUE : FALSE;
    ret = qst_save_show2(ctx, argv[7], argc - 8, &argv[8],
                         &box, ccw, 1.0f, ret);
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_free);
    _LEAVE_V2D_SINGLE_
    return (ret);
}

/*
---------------------------------------
    保存当前显示图片 (旋转+缩放)
---------------------------------------
*/
static bool_t
qst_v2d_g2d_rotz (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT       box;
    bool_t      ret;
    fp32_t      ccw;
    fp32_t      mul;
    sQstView2D* ctx;

    /* 参数解析 <X> <Y> <Width> <Height> <CCW> <Scale> <Lerp> <文件名> [...] */
    if (argc < 9)
        return (FALSE);
    ctx = (sQstView2D*)parm;
    if (ctx->image == NULL)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_busy);
    box.x1 = str2intxA(argv[1]);
    box.y1 = str2intxA(argv[2]);
    box.ww = str2intxA(argv[3]);
    if (box.ww == 0)
        box.ww = ctx->image->position.ww - box.x1;
    box.hh = str2intxA(argv[4]);
    if (box.hh == 0)
        box.hh = ctx->image->position.hh - box.y1;
    rect_set_wh(&box, box.x1, box.y1, box.ww, box.hh);
    ccw = str2fp32A(argv[5]);
    if (ccw < 0.0f) {
        ccw = -ccw;
        ret = TRUE;
    }
    else {
        ret = FALSE;
    }
    while (ccw >= 360.0f)
        ccw -= 360.0f;
    if (ret) {
        ccw = 360.0f - ccw;
        if (ccw >= 360.0f)
            ccw -= 360.0f;
    }
    mul = str2fp32A(argv[6]);
    ret = str2intxA(argv[7]) ? TRUE : FALSE;
    ret = qst_save_show2(ctx, argv[8], argc - 9, &argv[9],
                         &box, ccw, mul, ret);
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_free);
    _LEAVE_V2D_SINGLE_
    return (ret);
}

/*
---------------------------------------
    保存当前显示图片 (切片)
---------------------------------------
*/
static bool_t
qst_v2d_g2d_tile (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t      tww;
    uint_t      thh;
    uint_t      adx;
    uint_t      ady;
    bool_t      ret;
    sQstView2D* ctx;

    /* 参数解析 <TileW> <TileH> <StepX> <StepY> <文件名> [...] */
    if (argc < 6)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    ctx = (sQstView2D*)parm;
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_busy);
    tww = str2intxA(argv[1]);
    thh = str2intxA(argv[2]);
    if (thh == 0) thh = tww;
    adx = str2intxA(argv[3]);
    if (adx == 0) adx = tww;
    ady = str2intxA(argv[4]);
    if (ady == 0) ady = thh;
    ret = qst_save_show3(ctx, argv[5], argc - 6, &argv[6],
                         tww, thh, adx, ady);
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_free);
    _LEAVE_V2D_SINGLE_
    return (ret);
}

/*
---------------------------------------
    保存当前显示图片 (抠出最小方框)
---------------------------------------
*/
static bool_t
qst_v2d_g2d_grub (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    bool_t      ret;
    sQstView2D* ctx;

    /* 参数解析 <文件名> [...] */
    if (argc < 2)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    ctx = (sQstView2D*)parm;
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_busy);
    ret = qst_save_min(ctx, argv[1], argc - 2, &argv[2]);
    QST_SET_CURSOR(ctx->hwnd, ctx->cur_free);
    _LEAVE_V2D_SINGLE_
    return (ret);
}

/*
---------------------------------------
    设置资源根目录路径
---------------------------------------
*/
static bool_t
qst_v2d_res_root (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstView2D* ctx;

    /* 参数解析 <根目录路径> */
    ctx = (sQstView2D*)parm;
    if (argc < 2)
        return (FALSE);
    if (ctx->res_loader->init != NULL)
        ctx->res_loader->init(ctx->netw, argv[1]);
    return (TRUE);
}

/*
---------------------------------------
    加载资源路由插件
---------------------------------------
*/
static bool_t
qst_v2d_res_router (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstView2D*     ctx;
    router_get_t    func;

    /* 参数解析 [插件路径] */
    ctx = (sQstView2D*)parm;
    if (ctx->dll_router != NULL) {
        sbin_unload(ctx->dll_router);
        ctx->dll_router = NULL;
        ctx->res_router = NULL;
    }

    /* 不指定参数表示释放当前插件 */
    if (argc < 2)
        return (TRUE);

    /* 加载路由插件获取接口表并设置加载接口 */
    ctx->dll_router = sbin_loadA(argv[1]);
    if (ctx->dll_router == NULL)
        return (FALSE);
    func = sbin_exportT(ctx->dll_router,
                    "res_router_get", router_get_t);
    if (func == NULL) {
        sbin_unload(ctx->dll_router);
        ctx->dll_router = NULL;
        return (FALSE);
    }
    ctx->res_router = func();
    ctx->res_router->setup(ctx->res_loader->load);
    return (TRUE);
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit",  qst_v2d_app_exit  },
    { "cfg:load",  qst_v2d_cfg_load  },
    { "win:load",  qst_v2d_win_load  },
    { "win:save",  qst_v2d_win_save  },
    { "win:show",  qst_v2d_win_show  },
    { "app:reset", qst_v2d_g2d_clear },

    /***** 公用插件命令 *****/
    { "ext:free", qst_v2d_ext_free },
    { "ext:load", qst_v2d_ext_load },

    /***** 公用加载命令 *****/
    { "ldr:file", qst_v2d_ldr_file },
    { "ldr:smem", qst_v2d_ldr_smem },

    /***** 索引控制命令 *****/
    { "idx:set_now", qst_v2d_set_now },

    /***** 公用资源命令 *****/
    { "res:root",   qst_v2d_res_root   },
    { "res:router", qst_v2d_res_router },

    /***** 图片控制命令 *****/
    { "g2d:alpha",   qst_v2d_g2d_alpha   },
    { "g2d:color",   qst_v2d_g2d_color   },
    { "g2d:clear",   qst_v2d_g2d_clear   },
    { "g2d:canvas",  qst_v2d_g2d_canvas  },
    { "g2d:refresh", qst_v2d_g2d_refresh },
    { "g2d:grab",    qst_v2d_g2d_grab    },
    { "g2d:grub",    qst_v2d_g2d_grub    },
    { "g2d:zoom",    qst_v2d_g2d_zoom    },
    { "g2d:rote",    qst_v2d_g2d_rote    },
    { "g2d:rotz",    qst_v2d_g2d_rotz    },
    { "g2d:tile",    qst_v2d_g2d_tile    },
    { "g2d:save",    qst_v2d_g2d_save    },
    { "g2d:saveall", qst_v2d_g2d_saveall },
    { "g2d:savenow", qst_v2d_g2d_savenow },

    /***** 二维插件命令 *****/
    { "g2d:ext:free", qst_v2d_ext_free },
    { "g2d:ext:load", qst_v2d_ext_load },

    /***** 私有命令映射 *****/
    { "qv2d:app:exit", qst_v2d_app_exit },
    { "qv2d:win:show", qst_v2d_win_show },
    { "qv2d:ldr:file", qst_v2d_ldr_file },
    { "qv2d:ldr:smem", qst_v2d_ldr_smem },
    { "qv2d:cmd:load", qst_v2d_cmd_load },
    { "qv2d:flt:load", qst_v2d_flt_load },
};

/*
=======================================
    工作线程
=======================================
*/
CR_API uint_t STDCALL
qst_v2d_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    uint_t      tim;
    fp32_t      tot;
    xtime_t     log;
    sQstView2D* ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_v2d_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    tim = 0;
    tot = 0.0f;
    log = timer_new();
    ctx = (sQstView2D*)param;
    ctx->objs = obj;

    /* 工作循环 */
    while (!ctx->quit)
    {
        bool_t  rett;
        fp32_t  delta;
        ansi_t* string;

        /* 接收一条命令 */
        /* 即使是出错也要继续运行 */
        string = netw_cmd_recv(ctx->netw);
        if (string == NULL) {
            tim = 0;
            tot = 0.0f;
            thread_sleep(1);
            continue;
        }

        /* 执行这条命令 */
        if (log != NULL)
            timer_set_base(log);
        rett = cmd_exec_main(obj, ctx, string);
        mem_free(string);
        if (log != NULL && rett && !ctx->quit)
        {
            ansi_t  buf[128];

            /* 有效的操作显示计数 */
            delta = timer_get_delta(log);
            tim += 1;
            tot += delta;
            if (ctx->trans)
                string = "[Alpha]";
            else if (ctx->cfgs.is_blends)
                string = "[Blend]";
            else
                string = "[Color]";
            sprintf(buf, WIN_TITLE " - last: %.3f ms / "
                        "total: %.3f ms / count: %u %s",
                            delta, tot, tim, string);

            /* 发送消息的方式防止线程死锁 */
            PostMessageA(ctx->hwnd, WM_QV2D_SET_WINDOW_TITLE, 0, (LPARAM)buf);
        }
    }
    if (log != NULL)
        timer_del(log);
    qst_clear(ctx);
    cmd_exec_free(obj);
    return (QST_OKAY);
}
