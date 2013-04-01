
#include "xOpenCV.h"
#undef  getT    /* 有冲突 */
#include "opencv2/opencv.hpp"
using namespace cv;

/*
---------------------------------------
    高斯模糊处理
---------------------------------------
*/
static bool_t
gaussian_blur (
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    uint_t  ksize_x, ksize_y;
    fp64_t  sigma_x, sigma_y;

    CR_NOUSE(nouse);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    ksize_x = xml_attr_intxU("ksize_x", 9, param);
    ksize_y = xml_attr_intxU("ksize_y", 9, param);
    sigma_x = xml_attr_fp64U("sigma_x", 0, param);
    sigma_y = xml_attr_fp64U("sigma_y", 0, param);

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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    uint_t      size;
    sIMAGE*     dest;
    IplImage    draw;

    CR_NOUSE(nouse);
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
  __CR_UU__ void_t*     nouse,
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

    CR_NOUSE(nouse);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    ww = dest->position.ww;
    hh = dest->position.hh;
    if (ww > hh) ww = hh;
    dp = xml_attr_fp64U("dp", 2, param);
    min_dist = xml_attr_fp64U("min_dist", ww / 4, param);
    param1 = xml_attr_fp64U("param1", 200, param);
    param2 = xml_attr_fp64U("param2", 100, param);
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
=======================================
    滤镜接口导出表
=======================================
*/
CR_API const sXC_PORT   qst_v2d_filter[] =
{
    { "opencv_gauss_blur", gaussian_blur },
    { "opencv_median_blur", median_blur },
    { "opencv_hough_circles", hough_circles },
    { NULL, NULL },
};
