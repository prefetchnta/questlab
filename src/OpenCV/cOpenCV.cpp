
#include "xOpenCV.h"
#include "../QstView2D/QstView2D.h"
#undef  getT    /* 有冲突 */
#include "opencv2/opencv.hpp"
using namespace cv;

/* 绘制开关 */
bool_t  s_okay = FALSE;

/* 全局绘制参数 */
static sint_t       s_posx;     /* 当前坐标 */
static sint_t       s_posy;     /* 当前坐标 */
static Scalar       s_color;    /* 绘制颜色 */
static sint_t       s_thick;    /* 绘制宽度 */
static sint_t       s_tline;    /* 线条类型 */
static IplImage     s_image;    /* 绘制对象 */

/*
---------------------------------------
    翻转坐标 (因为目标是 GDI 表面)
---------------------------------------
*/
inline sint_t
flip_vv (
  __CR_IN__ sint_t  y
    )
{
    return (s_image.height - y - 1);
}

/*
---------------------------------------
    开始绘制
---------------------------------------
*/
static bool_t
qst_ocv_enter (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sIMAGE*     img;
    sQstView2D* ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    if (s_okay)
        return (FALSE);
    _ENTER_V2D_SINGLE_
    s_thick = 1;
    s_tline = 8;
    s_posx = s_posy = 0;
    s_color = Scalar(0, 0, 0, 255);
    ctx = (sQstView2D*)parm;
    img = CR_VCALL(ctx->draw)->lock(ctx->draw);
    ilab_img2ipl_set(&s_image, img);
    s_okay = TRUE;
    return (TRUE);
}

/*
---------------------------------------
    结束绘制
---------------------------------------
*/
static bool_t
qst_ocv_leave (
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
qst_ocv_refresh (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sIMAGE*     img;
    sQstView2D* ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    if (!s_okay)
        return (FALSE);
    ctx = (sQstView2D*)parm;
    CR_VCALL(ctx->draw)->unlock(ctx->draw);
    CR_VCALL(ctx->draw)->flip(ctx->draw, FALSE);
    img = CR_VCALL(ctx->draw)->lock(ctx->draw);
    ilab_img2ipl_set(&s_image, img);
    return (TRUE);
}

/*
---------------------------------------
    设置颜色
---------------------------------------
*/
static bool_t
qst_ocv_setcolor (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t  rr, gg, bb, aa;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <Red> <Green> <Blue> [Alpha] */
    if (argc < 4)
        return (FALSE);
    CR_NOUSE(parm);
    rr = str2intxA(argv[1]);
    gg = str2intxA(argv[2]);
    bb = str2intxA(argv[3]);
    if (argc > 4)
        aa = str2intxA(argv[4]);
    else
        aa = 255;
    s_color = Scalar(bb, gg, rr, aa);
    return (TRUE);
}

/*
---------------------------------------
    设置宽度
---------------------------------------
*/
static bool_t
qst_ocv_setwidth (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    if (!s_okay)
        return (FALSE);

    /* 参数解析 <Width> */
    if (argc < 2)
        return (FALSE);
    CR_NOUSE(parm);
    s_thick = (sint_t)str2intxA(argv[1]);
    return (TRUE);
}

/*
---------------------------------------
    设置线型
---------------------------------------
*/
static bool_t
qst_ocv_setline (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t  type;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <Type> */
    if (argc < 2)
        return (FALSE);
    CR_NOUSE(parm);
    type = str2intxA(argv[1]);
    if (type != 8 && type != 4 && type != CV_AA)
        return (FALSE);
    s_tline = (sint_t)type;
    return (TRUE);
}

/*
---------------------------------------
    移动坐标
---------------------------------------
*/
static bool_t
qst_ocv_moveto (
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
qst_ocv_lineto (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  newx, newy;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X> <Y> */
    if (argc < 3)
        return (FALSE);

    Mat img(&s_image, false);

    CR_NOUSE(parm);
    newx = (sint_t)str2intxA(argv[1]);
    newy = (sint_t)str2intxA(argv[2]);
    line(img, Point(s_posx, flip_vv(s_posy)),
        Point(newx, flip_vv(newy)), s_color, s_thick, s_tline);
    s_posx = newx;
    s_posy = newy;
    return (TRUE);
}

/*
---------------------------------------
    绘制线段
---------------------------------------
*/
static bool_t
qst_ocv_line (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  sx, sy;
    sint_t  dx, dy;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);

    Mat img(&s_image, false);

    CR_NOUSE(parm);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    line(img, Point(sx, flip_vv(sy)),
        Point(dx, flip_vv(dy)), s_color, s_thick, s_tline);
    return (TRUE);
}

/*
---------------------------------------
    绘制方框 (坐标)
---------------------------------------
*/
static bool_t
qst_ocv_rect_xy (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  sx, sy;
    sint_t  dx, dy;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);

    Mat img(&s_image, false);

    CR_NOUSE(parm);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    rectangle(img, Point(sx, flip_vv(sy)),
        Point(dx, flip_vv(dy)), s_color, s_thick, s_tline);
    return (TRUE);
}

/*
---------------------------------------
    绘制方框 (宽高)
---------------------------------------
*/
static bool_t
qst_ocv_rect_wh (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  sx, sy;
    sint_t  ww, hh;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X> <Y> <Width> <Height> */
    if (argc < 5)
        return (FALSE);

    Mat img(&s_image, false);

    CR_NOUSE(parm);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    ww = (sint_t)str2intxA(argv[3]);
    hh = (sint_t)str2intxA(argv[4]);
    rectangle(img, Point(sx, flip_vv(sy)),
        Point(sx + ww - 1, flip_vv(sy + hh - 1)),
            s_color, s_thick, s_tline);
    return (TRUE);
}

/*
---------------------------------------
    绘制正圆
---------------------------------------
*/
static bool_t
qst_ocv_circle (
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

    Mat img(&s_image, false);

    CR_NOUSE(parm);
    cx = (sint_t)str2intxA(argv[1]);
    cy = (sint_t)str2intxA(argv[2]);
    rr = (sint_t)str2intxA(argv[3]);
    circle(img, Point(cx, flip_vv(cy)), rr,
        s_color, s_thick, s_tline);
    return (TRUE);
}

/*
---------------------------------------
    绘制椭圆 (圆弧)
---------------------------------------
*/
static bool_t
qst_ocv_ellipse_arc (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    fp64_t  angle;
    fp64_t  start;
    fp64_t  agend;
    sint_t  cx, cy;
    sint_t  ww, hh;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X> <Y> <Width> <Height> <Angle> <StartAngle> <EndAngle> */
    if (argc < 8)
        return (FALSE);

    Mat img(&s_image, false);

    CR_NOUSE(parm);
    cx = (sint_t)str2intxA(argv[1]);
    cy = (sint_t)str2intxA(argv[2]);
    ww = (sint_t)str2intxA(argv[3]);
    hh = (sint_t)str2intxA(argv[4]);
    angle = str2fp64A(argv[5]);
    start = str2fp64A(argv[6]);
    agend = str2fp64A(argv[7]);
    ellipse(img, Point(cx, flip_vv(cy)), Size(ww, hh),
        angle, start, agend, s_color, s_thick, s_tline);
    return (TRUE);
}

/*
---------------------------------------
    绘制椭圆 (角度)
---------------------------------------
*/
static bool_t
qst_ocv_ellipse_agl (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    fp32_t  angle;
    sint_t  cx, cy;
    sint_t  ww, hh;

    if (!s_okay)
        return (FALSE);

    /* 参数解析 <X> <Y> <Width> <Height> <Angle> */
    if (argc < 6)
        return (FALSE);

    Mat img(&s_image, false);

    CR_NOUSE(parm);
    cx = (sint_t)str2intxA(argv[1]);
    cy = (sint_t)str2intxA(argv[2]);
    ww = (sint_t)str2intxA(argv[3]);
    hh = (sint_t)str2intxA(argv[4]);
    angle = str2fp32A(argv[5]);
    ellipse(img, RotatedRect(Point(cx, flip_vv(cy)),
        Size(ww, hh), angle), s_color, s_thick, s_tline);
    return (TRUE);
}

/*
=======================================
    命令单元导出表
=======================================
*/
CR_API const sQST_CMD   qst_v2d_cmdz[] =
{
    { "ocv:enter", qst_ocv_enter },
    { "ocv:leave", qst_ocv_leave },
    { "ocv:refresh", qst_ocv_refresh },
    { "ocv:setcolor", qst_ocv_setcolor },
    { "ocv:setwidth", qst_ocv_setwidth },
    { "ocv:setline", qst_ocv_setline },
    { "ocv:moveto", qst_ocv_moveto },
    { "ocv:lineto", qst_ocv_lineto },
    { "ocv:line", qst_ocv_line },
    { "ocv:rect_xy", qst_ocv_rect_xy },
    { "ocv:rect_wh", qst_ocv_rect_wh },
    { "ocv:circle", qst_ocv_circle },
    { "ocv:ellipse_arc", qst_ocv_ellipse_arc },
    { "ocv:ellipse_agl", qst_ocv_ellipse_agl },
    { NULL, NULL },
};
