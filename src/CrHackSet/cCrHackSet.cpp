
#include "../QstView2D/QstView2D.h"

/* 绘制开关 */
bool_t  s_okay = FALSE;

/* 填充类型 */
typedef void_t  (*flldraw_t) (const sIMAGE*, const sFILL*,
                              cpix_t, const sRECT*);
/* 全局绘制参数 */
static sint_t       s_posx;     /* 当前坐标 */
static sint_t       s_posy;     /* 当前坐标 */
static cpix_t       s_color;    /* 绘制颜色 */
static sIMAGE*      s_image;    /* 绘制对象 */
static pixdraw_t    s_pixdraw;  /* 绘制模式 */
static flldraw_t    s_flldraw;  /* 填充模式 */

/*
---------------------------------------
    开始绘制
---------------------------------------
*/
static bool_t
qst_crh_enter (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstView2D* ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    if (s_okay)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    s_posx = s_posy = 0;
    s_pixdraw = pixel_set32n;
    s_flldraw = fill_set32_c;
    s_color.val = 0xFF000000;
    ctx = (sQstView2D*)parm;
    s_image = CR_VCALL(ctx->draw)->lock(ctx->draw);
    s_okay = TRUE;
    return (TRUE);
}

/*
---------------------------------------
    结束绘制
---------------------------------------
*/
static bool_t
qst_crh_leave (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstView2D* ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    if (!s_okay)
        return (FALSE);
    ctx = (sQstView2D*)parm;
    CR_VCALL(ctx->draw)->unlock(ctx->draw);
    CR_VCALL(ctx->draw)->flip(ctx->draw, FALSE);
    s_okay = FALSE;
    _LEAVE_V2D_SINGLE_
    return (TRUE);
}

/*
---------------------------------------
    刷新绘制
---------------------------------------
*/
static bool_t
qst_crh_refresh (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstView2D* ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    if (!s_okay)
        return (FALSE);
    ctx = (sQstView2D*)parm;
    CR_VCALL(ctx->draw)->unlock(ctx->draw);
    CR_VCALL(ctx->draw)->flip(ctx->draw, FALSE);
    s_image = CR_VCALL(ctx->draw)->lock(ctx->draw);
    return (TRUE);
}

/*
---------------------------------------
    设置颜色
---------------------------------------
*/
static bool_t
qst_crh_setcolor (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    if (!s_okay)
        return (FALSE);

    /* 参数解析 <Red> <Green> <Blue> [Alpha] */
    if (argc < 4)
        return (FALSE);
    CR_NOUSE(parm);
    s_color.c32.rrr = (byte_t)str2intxA(argv[1]);
    s_color.c32.ggg = (byte_t)str2intxA(argv[2]);
    s_color.c32.bbb = (byte_t)str2intxA(argv[3]);
    if (argc > 4)
        s_color.c32.lrp = (byte_t)str2intxA(argv[4]);
    else
        s_color.c32.lrp = 255;
    return (TRUE);
}

/*
---------------------------------------
    设置模式
---------------------------------------
*/
static bool_t
qst_crh_setmode (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    if (!s_okay)
        return (FALSE);

    /* 参数解析 <Mode> */
    if (argc < 2)
        return (FALSE);
    CR_NOUSE(parm);
    if (str_cmpIA(argv[1], "set") == 0) {
        s_pixdraw = pixel_set32n;
        s_flldraw = fill_set32_c;
    }
    else
    if (str_cmpIA(argv[1], "lrp") == 0) {
        s_pixdraw = pixel_lrp32n;
        s_flldraw = fill_lrp32_c;
    }
    else
    if (str_cmpIA(argv[1], "add") == 0) {
        s_pixdraw = pixel_add32n;
        s_flldraw = fill_add32_c;
    }
    else
    if (str_cmpIA(argv[1], "sub") == 0) {
        s_pixdraw = pixel_sub32n;
        s_flldraw = fill_sub32_c;
    }
    else
    if (str_cmpIA(argv[1], "xor") == 0) {
        s_pixdraw = pixel_xor32n;
        s_flldraw = fill_xor32_c;
    }
    else
    if (str_cmpIA(argv[1], "and") == 0) {
        s_pixdraw = pixel_and32n;
        s_flldraw = fill_and32_c;
    }
    else
    if (str_cmpIA(argv[1], "orr") == 0) {
        s_pixdraw = pixel_orr32n;
        s_flldraw = fill_orr32_c;
    }
    else
    if (str_cmpIA(argv[1], "not") == 0) {
        s_pixdraw = pixel_not32n;
        s_flldraw = fill_not32_c;
    }
    else {
        return (FALSE);
    }
    return (TRUE);
}

/*
---------------------------------------
    移动坐标
---------------------------------------
*/
static bool_t
qst_crh_moveto (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X> <Y> */
    if (argc < 3)
        return (FALSE);
    CR_NOUSE(parm);
    s_posx = (sint_t)str2intxA(argv[1]);
    s_posy = (sint_t)str2intxA(argv[2]);
    return (TRUE);
}

/*
---------------------------------------
    移动坐标 (绘制)
---------------------------------------
*/
static bool_t
qst_crh_lineto (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sLINE   line;
    sint_t  newx, newy;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X> <Y> */
    if (argc < 3)
        return (FALSE);
    CR_NOUSE(parm);
    newx = (sint_t)str2intxA(argv[1]);
    newy = (sint_t)str2intxA(argv[2]);
    rect_set_xy(&line, s_posx, s_posy, newx, newy);
    draw_line(s_image, &line, s_color, s_pixdraw);
    s_posx = newx;
    s_posy = newy;
    return (TRUE);
}

/*
---------------------------------------
    绘制像素
---------------------------------------
*/
static bool_t
qst_crh_pixel (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  posx, posy;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X> <Y> */
    if (argc < 3)
        return (FALSE);
    CR_NOUSE(parm);
    posx = (sint_t)str2intxA(argv[1]);
    posy = (sint_t)str2intxA(argv[2]);
    if (!clip_pixel(posx, posy, &s_image->clip_win))
        return (TRUE);
    s_pixdraw(s_image, posx, posy, s_color);
    return (TRUE);
}

/*
---------------------------------------
    绘制线段
---------------------------------------
*/
static bool_t
qst_crh_line (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sLINE   line;
    sint_t  sx, sy;
    sint_t  dx, dy;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);
    CR_NOUSE(parm);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    rect_set_xy(&line, sx, sy, dx, dy);
    draw_line(s_image, &line, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制方框 (坐标)
---------------------------------------
*/
static bool_t
qst_crh_rect_xy (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT   rect;
    sint_t  sx, sy;
    sint_t  dx, dy;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);
    CR_NOUSE(parm);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    rect_set_xy(&rect, sx, sy, dx, dy);
    draw_rect(s_image, &rect, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制方框 (宽高)
---------------------------------------
*/
static bool_t
qst_crh_rect_wh (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT   rect;
    sint_t  sx, sy;
    uint_t  ww, hh;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X> <Y> <Width> <Height> */
    if (argc < 5)
        return (FALSE);
    CR_NOUSE(parm);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    ww = (uint_t)str2intxA(argv[3]);
    hh = (uint_t)str2intxA(argv[4]);
    rect_set_wh(&rect, sx, sy, ww, hh);
    draw_rect(s_image, &rect, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制正圆
---------------------------------------
*/
static bool_t
qst_crh_circle (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  cx, cy, rr;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X> <Y> <Radius> */
    if (argc < 4)
        return (FALSE);
    CR_NOUSE(parm);
    cx = (sint_t)str2intxA(argv[1]);
    cy = (sint_t)str2intxA(argv[2]);
    rr = (sint_t)str2intxA(argv[3]);
    draw_circle(s_image, cx, cy, rr, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制椭圆 (坐标)
---------------------------------------
*/
static bool_t
qst_crh_ellipse_xy (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT   rect;
    sint_t  sx, sy;
    sint_t  dx, dy;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);
    CR_NOUSE(parm);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    rect_set_xy(&rect, sx, sy, dx, dy);
    draw_ellipse(s_image, &rect, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制椭圆 (宽高)
---------------------------------------
*/
static bool_t
qst_crh_ellipse_wh (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT   rect;
    sint_t  sx, sy;
    uint_t  ww, hh;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X> <Y> <Width> <Height> */
    if (argc < 5)
        return (FALSE);
    CR_NOUSE(parm);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    ww = (uint_t)str2intxA(argv[3]);
    hh = (uint_t)str2intxA(argv[4]);
    rect_set_wh(&rect, sx, sy, ww, hh);
    draw_ellipse(s_image, &rect, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制填充 (坐标)
---------------------------------------
*/
static bool_t
qst_crh_fill_xy (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sFILL   fill;
    sRECT   rect;
    sint_t  sx, sy;
    sint_t  dx, dy;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);
    CR_NOUSE(parm);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    rect_set_xy(&rect, sx, sy, dx, dy);
    fill.dx = rect.x1;
    fill.dy = rect.y1;
    fill.dw = rect.ww;
    fill.dh = rect.hh;
    s_flldraw(s_image, &fill, s_color, NULL);
    return (TRUE);
}

/*
---------------------------------------
    绘制填充 (宽高)
---------------------------------------
*/
static bool_t
qst_crh_fill_wh (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sFILL   fill;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X> <Y> <Width> <Height> */
    if (argc < 5)
        return (FALSE);
    CR_NOUSE(parm);
    fill.dx = (sint_t)str2intxA(argv[1]);
    fill.dy = (sint_t)str2intxA(argv[2]);
    fill.dw = (uint_t)str2intxA(argv[3]);
    fill.dh = (uint_t)str2intxA(argv[4]);
    s_flldraw(s_image, &fill, s_color, NULL);
    return (TRUE);
}

/*
=======================================
    命令单元导出表
=======================================
*/
CR_API const sQST_CMD   qst_v2d_cmdz[] =
{
    { "crh:enter", qst_crh_enter },
    { "crh:leave", qst_crh_leave },
    { "crh:refresh", qst_crh_refresh },
    { "crh:setcolor", qst_crh_setcolor },
    { "crh:setmode", qst_crh_setmode },
    { "crh:moveto", qst_crh_moveto },
    { "crh:lineto", qst_crh_lineto },
    { "crh:pixel", qst_crh_pixel },
    { "crh:line", qst_crh_line },
    { "crh:rect_xy", qst_crh_rect_xy },
    { "crh:rect_wh", qst_crh_rect_wh },
    { "crh:circle", qst_crh_circle },
    { "crh:ellipse_xy", qst_crh_ellipse_xy },
    { "crh:ellipse_wh", qst_crh_ellipse_wh },
    { "crh:fill_xy", qst_crh_fill_xy },
    { "crh:fill_wh", qst_crh_fill_wh },
    { NULL, NULL },
};
