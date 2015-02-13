
#include "xOpenCV.h"
#include "../QstLibs/QstLibs.h"
#include "opencv2/opencv.hpp"
using namespace cv;

/*
---------------------------------------
    自适应二值化
---------------------------------------
*/
static bool_t
adaptive_binary (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    uint_t  type, size;
    fp64_t  vmax, delta;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    vmax = xml_attr_fp64U("max", 255.0, param);
    delta = xml_attr_fp64U("delta", 10.0, param);
    type = xml_attr_intxU("type", CV_ADAPTIVE_THRESH_MEAN_C, param);
    size = xml_attr_intxU("size", 65, param);

    Mat outp, inpt(&draw, false);

    if (size < 3)
        size = 3;
    else if (size % 2 == 0)
        size += 1;
    if (type != CV_ADAPTIVE_THRESH_MEAN_C)
        type = ADAPTIVE_THRESH_GAUSSIAN_C;
    cvtColor(inpt, outp, CV_BGR2GRAY);
    adaptiveThreshold(outp, outp, vmax, type, THRESH_BINARY, size, delta);
    cvtColor(outp, inpt, CV_GRAY2BGRA);
    return (TRUE);
}

/*
---------------------------------------
    均值模糊处理
---------------------------------------
*/
static bool_t
box_blur (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    uint_t  ksize_x, ksize_y;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    ksize_x = xml_attr_intxU("ksize_x", 9, param);
    ksize_y = xml_attr_intxU("ksize_y", 9, param);

    Mat inpt(&draw, false);

    /* 卷积核大小必须为奇数 */
    if (ksize_x < 3)
        ksize_x = 3;
    else if (ksize_x % 2 == 0)
        ksize_x += 1;
    if (ksize_y < 3)
        ksize_y = 3;
    else if (ksize_y % 2 == 0)
        ksize_y += 1;
    blur(inpt, inpt, Size(ksize_x, ksize_y));
    return (TRUE);
}

/*
---------------------------------------
    高斯模糊处理
---------------------------------------
*/
static bool_t
gaussian_blur (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    uint_t  ksize_x, ksize_y;
    fp64_t  sigma_x, sigma_y;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    ksize_x = xml_attr_intxU("ksize_x", 9, param);
    ksize_y = xml_attr_intxU("ksize_y", 9, param);
    sigma_x = xml_attr_fp64U("sigma_x", 0.0, param);
    sigma_y = xml_attr_fp64U("sigma_y", 0.0, param);

    Mat inpt(&draw, false);

    /* 卷积核大小必须为奇数 */
    if (ksize_x < 3)
        ksize_x = 3;
    else if (ksize_x % 2 == 0)
        ksize_x += 1;
    if (ksize_y < 3)
        ksize_y = 3;
    else if (ksize_y % 2 == 0)
        ksize_y += 1;
    GaussianBlur(inpt, inpt, Size(ksize_x, ksize_y), sigma_x, sigma_y);
    return (TRUE);
}

/*
---------------------------------------
    中值模糊处理
---------------------------------------
*/
static bool_t
median_blur (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    uint_t      size;
    sIMAGE*     dest;
    IplImage    draw;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    size = xml_attr_intxU("ksize", 5, param);

    Mat inpt(&draw, false);

    /* 卷积核大小必须为奇数 */
    if (size < 3)
        size = 3;
    else if (size % 2 == 0)
        size += 1;
    medianBlur(inpt, inpt, size);
    return (TRUE);
}

/*
---------------------------------------
    Hough 变换圆搜索
---------------------------------------
*/
static bool_t
hough_circles (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    fp64_t  dp, min_dist;
    fp64_t  param1, param2;
    uint_t  ww, min_radius;
    uint_t  hh, max_radius;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    ww = dest->position.ww;
    hh = dest->position.hh;
    if (ww > hh) ww = hh;
    dp = xml_attr_fp64U("dp", 2.0, param);
    min_dist = xml_attr_fp64U("min_dist", ww / 4, param);
    param1 = xml_attr_fp64U("param1", 200.0, param);
    param2 = xml_attr_fp64U("param2", 100.0, param);
    min_radius = xml_attr_intxU("min_radius", 0, param);
    max_radius = xml_attr_intxU("max_radius", ww / 2, param);

    size_t  idx, count;
    vector<Vec3f>   result;
    Mat outp, inpt(&draw, false);

    /* 变换需要8位单通道的图片 */
    cvtColor(inpt, outp, CV_BGR2GRAY);
    HoughCircles(outp, result, CV_HOUGH_GRADIENT, dp, min_dist,
                 param1, param2, min_radius, max_radius);
    count = result.size();
    for (idx = 0; idx < count; idx++)
    {
        Point   center(cvRound(result[idx][0]),
                       cvRound(result[idx][1]));
        sint_t  radius=cvRound(result[idx][2]);

        circle(inpt, center,      3, Scalar(0, 255, 0, 255), -1, 8, 0);
        circle(inpt, center, radius, Scalar(0, 0, 255, 255),  3, 8, 0);
    }
    return (TRUE);
}

/*
---------------------------------------
    Hough 变换线搜索
---------------------------------------
*/
static bool_t
hough_lines (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    fp64_t  rho, the;
    uint_t  ksize, thres;
    fp64_t  param1, param2;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    ksize = xml_attr_intxU("ksize", 3, param);
    param1 = xml_attr_fp64U("param1", 50.0, param);
    param2 = xml_attr_fp64U("param2", 200.0, param);
    rho = xml_attr_fp64U("rho", 1.0, param);
    the = xml_attr_fp64U("theta", CV_PI / 180, param);

    size_t  idx, count;
    Mat outp, inpt(&draw, false);

    /* 两种搜索直线的方法 */
    Canny(inpt, outp, param1, param2, ksize);
    if (str_cmpA(param->name, "opencv_hough_lines") == 0)
    {
        fp64_t  srn, stn;
        vector<Vec2f>   lines;

        srn = xml_attr_fp64U("srn", 0.0, param);
        stn = xml_attr_fp64U("stn", 0.0, param);
        thres = xml_attr_intxU("thres", 100, param);
        HoughLines(outp, lines, rho, the, thres, srn, stn);
        count = lines.size();
        for (idx = 0; idx < count; idx++)
        {
            fp64_t  aa, bb;
            fp64_t  x0, y0;

            rho = lines[idx][0];
            the = lines[idx][1];
            aa = cos(the);
            bb = sin(the);
            x0 = aa * rho;
            y0 = bb * rho;

            Point   pt1(cvRound(x0 + 1000 * (-bb)),
                        cvRound(y0 + 1000 * ( aa)));
            Point   pt2(cvRound(x0 - 1000 * (-bb)),
                        cvRound(y0 - 1000 * ( aa)));

            line(inpt, pt1, pt2, Scalar(0, 0, 255, 255), 3, 8);
        }
    }
    else
    {
        vector<Vec4i>   lines;
        fp64_t  min_len, max_gap;

        thres = xml_attr_intxU("thres", 80, param);
        min_len = xml_attr_fp64U("min_len", 30.0, param);
        max_gap = xml_attr_fp64U("max_gap", 10.0, param);
        HoughLinesP(outp, lines, rho, the, thres, min_len, max_gap);
        count = lines.size();
        for (idx = 0; idx < count; idx++) {
            line(inpt, Point(lines[idx][0], lines[idx][1]),
                       Point(lines[idx][2], lines[idx][3]),
                       Scalar(0, 0, 255, 255), 3, 8);
        }
    }
    return (TRUE);
}

/*
=======================================
    滤镜接口导出表
=======================================
*/
CR_API const sXC_PORT   qst_v2d_filter[] =
{
    { "opencv_auto_binary", adaptive_binary },
    { "opencv_box_blur", box_blur },
    { "opencv_gauss_blur", gaussian_blur },
    { "opencv_median_blur", median_blur },
    { "opencv_hough_circles", hough_circles },
    { "opencv_hough_lines", hough_lines },
    { "opencv_hough_linesp", hough_lines },
    { NULL, NULL },
};
