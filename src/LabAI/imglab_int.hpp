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
 *             ##       CREATE: 2018-03-06
 *              #
 ================================================
        ImageLAB 内部头文件
 ================================================
 */

#ifndef __IMGLAB_INT_HPP__
#define __IMGLAB_INT_HPP__

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include "opencv2/opencv.hpp"

#include "imglab.h"

/* CrHack 与 OpenCV 颜色转换 */
#define CPIX2SCALAR(cpix) \
    cv::Scalar(cpix.c32.bbb, cpix.c32.ggg, cpix.c32.rrr, cpix.c32.lrp)

/* OpenCV 矩阵转 iGFX2 接口 */
iGFX2* imglab_mat_to_igfx2 (ximage_t mat);

/* ImgLAB 内部工具函数 */
xpoly_lst_t imglab_split_to_xpoly (std::vector<cv::Point> *pnts, size_t usize);
void_t imglab_mats_to_points (std::vector<cv::Point> *pnts, const std::vector<cv::Mat> &mats);

#endif  /* !__IMGLAB_INT_HPP__ */
