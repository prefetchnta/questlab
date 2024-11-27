
#include "xOpenCV.h"
#include "../QstView2D/QstView2D.h"
#include "opencv2/opencv.hpp"
using namespace cv;

/* 全局绘制参数 */
static sint_t   s_posx  = 0;                        /* 当前坐标 */
static sint_t   s_posy  = 0;                        /* 当前坐标 */
static sint_t   s_thick = 1;                        /* 绘制宽度 */
static sint_t   s_tline = 8;                        /* 线条类型 */
static Scalar   s_color = Scalar(0, 0, 0, 255);     /* 绘制颜色 */

/*
---------------------------------------
    初始化参数
---------------------------------------
*/
static bool_t
qst_ocv_init (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(parm);
    CR_NOUSE(argc);
    CR_NOUSE(argv);
    return (TRUE);
}

/*
---------------------------------------
    设置颜色
---------------------------------------
*/
static bool_t
qst_ocv_color (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t  rr, gg, bb, aa;

    CR_NOUSE(parm);

    /* 参数解析 <Red> <Green> <Blue> [Alpha] */
    if (argc < 4)
        return (FALSE);
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
qst_ocv_width (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(parm);

    /* 参数解析 <Width> */
    if (argc < 2)
        return (FALSE);
    s_thick = (sint_t)str2intxA(argv[1]);
    return (TRUE);
}

/*
---------------------------------------
    设置线型
---------------------------------------
*/
static bool_t
qst_ocv_tline (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t  type;

    CR_NOUSE(parm);

    /* 参数解析 <Type> */
    if (argc < 2)
        return (FALSE);
    type = str2intxA(argv[1]);  /* CV_AA = 16 */
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
    CR_NOUSE(parm);

    /* 参数解析 <X> <Y> */
    if (argc < 3)
        return (FALSE);
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

    /* 参数解析 <X> <Y> */
    if (argc < 3)
        return (FALSE);

    sIMAGE*     draw;
    IplImage    image;

    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    ilab_img2ipl_set(&image, draw);

    Mat img = cvarrToMat(&image, false);

    newx = (sint_t)str2intxA(argv[1]);
    newy = (sint_t)str2intxA(argv[2]);
    line(img, Point(s_posx, s_posy), Point(newx, newy),
                s_color, s_thick, s_tline);
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

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);

    sIMAGE*     draw;
    IplImage    image;

    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    ilab_img2ipl_set(&image, draw);

    Mat img = cvarrToMat(&image, false);

    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    line(img, Point(sx, sy), Point(dx, dy),
            s_color, s_thick, s_tline);
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

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);

    sIMAGE*     draw;
    IplImage    image;

    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    ilab_img2ipl_set(&image, draw);

    Mat img = cvarrToMat(&image, false);

    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    rectangle(img, Point(sx, sy), Point(dx, dy),
                s_color, s_thick, s_tline);
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

    /* 参数解析 <X> <Y> <Width> <Height> */
    if (argc < 5)
        return (FALSE);

    sIMAGE*     draw;
    IplImage    image;

    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    ilab_img2ipl_set(&image, draw);

    Mat img = cvarrToMat(&image, false);

    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    ww = (sint_t)str2intxA(argv[3]);
    hh = (sint_t)str2intxA(argv[4]);
    rectangle(img, Point(sx, sy), Point(sx + ww - 1, sy + hh - 1),
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

    /* 参数解析 <X> <Y> <Radius> */
    if (argc < 4)
        return (FALSE);

    sIMAGE*     draw;
    IplImage    image;

    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    ilab_img2ipl_set(&image, draw);

    Mat img = cvarrToMat(&image, false);

    cx = (sint_t)str2intxA(argv[1]);
    cy = (sint_t)str2intxA(argv[2]);
    rr = (sint_t)str2intxA(argv[3]);
    circle(img, Point(cx, cy), rr, s_color, s_thick, s_tline);
    return (TRUE);
}

/*
---------------------------------------
    绘制椭圆 (圆弧)
---------------------------------------
*/
static bool_t
qst_ocv_ellps_arc (
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

    /* 参数解析 <X> <Y> <Width> <Height> <Angle> <StartAngle> <EndAngle> */
    if (argc < 8)
        return (FALSE);

    sIMAGE*     draw;
    IplImage    image;

    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    ilab_img2ipl_set(&image, draw);

    Mat img = cvarrToMat(&image, false);

    cx = (sint_t)str2intxA(argv[1]);
    cy = (sint_t)str2intxA(argv[2]);
    ww = (sint_t)str2intxA(argv[3]);
    hh = (sint_t)str2intxA(argv[4]);
    angle = str2fp64A(argv[5]);
    start = str2fp64A(argv[6]);
    agend = str2fp64A(argv[7]);
    ellipse(img, Point(cx, cy), Size(ww, hh), angle, start, agend,
                    s_color, s_thick, s_tline);
    return (TRUE);
}

/*
---------------------------------------
    绘制椭圆 (角度)
---------------------------------------
*/
static bool_t
qst_ocv_ellps_agl (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    fp32_t  angle;
    sint_t  cx, cy;
    sint_t  ww, hh;

    /* 参数解析 <X> <Y> <Width> <Height> <Angle> */
    if (argc < 6)
        return (FALSE);

    sIMAGE*     draw;
    IplImage    image;

    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    ilab_img2ipl_set(&image, draw);

    Mat img = cvarrToMat(&image, false);

    cx = (sint_t)str2intxA(argv[1]);
    cy = (sint_t)str2intxA(argv[2]);
    ww = (sint_t)str2intxA(argv[3]);
    hh = (sint_t)str2intxA(argv[4]);
    angle = str2fp32A(argv[5]);
    ellipse(img, RotatedRect(Point(cx, cy), Size(ww, hh), angle),
                    s_color, s_thick, s_tline);
    return (TRUE);
}

/*
---------------------------------------
    绘制文字 (只支持字母)
---------------------------------------
*/
static bool_t
qst_ocv_text (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    ansi_t* str;
    ansi_t* txt;
    sint_t  fface;
    fp64_t  scale;
    sint_t  sx, sy;

    /* 参数解析 <X> <Y> <FontFace> <FontScale> <EscText> */
    if (argc < 6)
        return (FALSE);

    sIMAGE*     draw;
    IplImage    image;

    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);

    /* 必须使用转义字符串 */
    str = str_fmtA("\"%s\"", argv[5]);
    if (str == NULL)
        return (FALSE);
    txt = str_esc_dupU(str);
    mem_free(str);
    if (txt == NULL)
        return (FALSE);
    ilab_img2ipl_set(&image, draw);

    Mat img = cvarrToMat(&image, false);

    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    fface = (sint_t)str2intxA(argv[3]);
    scale = str2fp64A(argv[4]);
    putText(img, txt, Point(sx, sy), fface, scale,
                s_color, s_thick, s_tline);
    mem_free(txt);
    return (TRUE);
}

/*
=======================================
    命令单元导出表
=======================================
*/
CR_API const sQST_CMD   qst_v2d_cmdz[] =
{
    { "ocv:init", qst_ocv_init },
    { "ocv:color", qst_ocv_color },
    { "ocv:width", qst_ocv_width },
    { "ocv:tline", qst_ocv_tline },
    { "ocv:moveto", qst_ocv_moveto },
    { "ocv:lineto", qst_ocv_lineto },
    { "ocv:line", qst_ocv_line },
    { "ocv:rect_xy", qst_ocv_rect_xy },
    { "ocv:rect_wh", qst_ocv_rect_wh },
    { "ocv:circle", qst_ocv_circle },
    { "ocv:ellps_arc", qst_ocv_ellps_arc },
    { "ocv:ellps_agl", qst_ocv_ellps_agl },
    { "ocv:text", qst_ocv_text },
    { NULL, NULL },
};
