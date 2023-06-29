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
        图像 LAB 内部头文件
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

#endif  /* !__IMGLAB_INT_HPP__ */
