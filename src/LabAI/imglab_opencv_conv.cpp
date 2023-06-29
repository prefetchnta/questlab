/*
 ================================================
 *      ######
 *    #########                            ##
 *   ####    ###                           ##
 *   ##       ##                           ##
 *  ##         ## ##   ##   ####     ### ######
 *  ##         ## ##   ##  ######  ##### ######
 *  ##         ## ##   ## ###  ## ###      ##
 *  ##    #    ## ##   ## ##  ##  #####    ##
 *   ##   ##   ## ##   ## ## ##    #####   ##
 *   ###   ## ##  ##   ## ###   #     ##   ##
 *    ##########  ####### ####### ######   ##
 *      #### ##    ######   ####   ####    ##
 *            ##
 *             ##       CREATE: 2022-11-29
 *              #
 ================================================
        图像 LAB OpenCV 卷积函数库
 ================================================
 */

#include "imglab_int.hpp"

/*****************************************************************************/
/*                                 图片滤波                                  */
/*****************************************************************************/

/*
=======================================
    均值滤波
=======================================
*/
CR_API void_t
imglab_blur_box (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      ksize_x,
  __CR_IN__ uint_t      ksize_y
    )
{
    cv::Mat*    mm;

    // 卷积核大小必须为奇数
    mm = (cv::Mat*)mat;
    if (ksize_x < 3)
        ksize_x = 3;
    else
    if (ksize_x % 2 == 0)
        ksize_x += 1;
    if (ksize_y < 3)
        ksize_y = 3;
    else
    if (ksize_y % 2 == 0)
        ksize_y += 1;
    cv::blur(*mm, *mm, cv::Size(ksize_x, ksize_y));
}

/*
=======================================
    高斯滤波
=======================================
*/
CR_API void_t
imglab_blur_gauss (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      ksize_x,
  __CR_IN__ uint_t      ksize_y,
  __CR_IN__ fp64_t      sigma_x,
  __CR_IN__ fp64_t      sigma_y
    )
{
    cv::Mat*    mm;

    // 卷积核大小必须为奇数
    mm = (cv::Mat*)mat;
    if (ksize_x < 3)
        ksize_x = 3;
    else
    if (ksize_x % 2 == 0)
        ksize_x += 1;
    if (ksize_y < 3)
        ksize_y = 3;
    else
    if (ksize_y % 2 == 0)
        ksize_y += 1;
    cv::GaussianBlur(*mm, *mm, cv::Size(ksize_x, ksize_y), sigma_x, sigma_y);
}

/*
=======================================
    中值滤波
=======================================
*/
CR_API void_t
imglab_blur_median (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      ksize
    )
{
    cv::Mat*    mm;

    // 卷积核大小必须为奇数
    mm = (cv::Mat*)mat;
    if (ksize < 3)
        ksize = 3;
    else
    if (ksize % 2 == 0)
        ksize += 1;
    cv::medianBlur(*mm, *mm, ksize);
}
