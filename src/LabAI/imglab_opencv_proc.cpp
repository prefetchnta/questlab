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
        ImageLAB OpenCV 图像处理函数库
 ================================================
 */

#include "imglab_int.hpp"

/* 用的到的常数表 */
static const int s_from_to[] = { 0, 0, 1, 1, 2, 2, 3, 3 };

/*****************************************************************************/
/*                                 图片滤波                                  */
/*****************************************************************************/

/*
=======================================
    均值滤波
=======================================
*/
CR_API ximage_t
imglab_ocv_blur_box (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      ksize_x,
  __CR_IN__ uint_t      ksize_y
    )
{
    cv::Mat*    ss = (cv::Mat*)mat;
    cv::Mat*    dd = new(std::nothrow) cv::Mat (ss->size(), ss->type());

    // 卷积核大小必须为奇数
    if (dd != NULL) {
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
        cv::blur(*ss, *dd, cv::Size(ksize_x, ksize_y));
    }
    return ((ximage_t)dd);
}

/*
=======================================
    栈式滤波
=======================================
*/
CR_API ximage_t
imglab_ocv_blur_stack (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      ksize_x,
  __CR_IN__ uint_t      ksize_y
    )
{
    cv::Mat*    ss = (cv::Mat*)mat;
    cv::Mat*    dd = new(std::nothrow) cv::Mat (ss->size(), ss->type());

    // 卷积核大小必须为奇数
    if (dd != NULL) {
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
        cv::stackBlur(*ss, *dd, cv::Size(ksize_x, ksize_y));
    }
    return ((ximage_t)dd);
}

/*
=======================================
    高斯滤波
=======================================
*/
CR_API ximage_t
imglab_ocv_blur_gauss (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      ksize_x,
  __CR_IN__ uint_t      ksize_y,
  __CR_IN__ fp64_t      sigma_x,
  __CR_IN__ fp64_t      sigma_y
    )
{
    cv::Mat*    ss = (cv::Mat*)mat;
    cv::Mat*    dd = new(std::nothrow) cv::Mat (ss->size(), ss->type());

    // 卷积核大小必须为奇数
    if (dd != NULL) {
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
        cv::GaussianBlur(*ss, *dd, cv::Size(ksize_x, ksize_y), sigma_x, sigma_y);
    }
    return ((ximage_t)dd);
}

/*
=======================================
    中值滤波
=======================================
*/
CR_API ximage_t
imglab_ocv_blur_median (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      ksize
    )
{
    cv::Mat*    ss = (cv::Mat*)mat;
    cv::Mat*    dd = new(std::nothrow) cv::Mat (ss->size(), ss->type());

    // 卷积核大小必须为奇数
    if (dd != NULL) {
        if (ksize < 3)
            ksize = 3;
        else
        if (ksize % 2 == 0)
            ksize += 1;
        cv::medianBlur(*ss, *dd, ksize);
    }
    return ((ximage_t)dd);
}

/*
=======================================
    双边滤波
=======================================
*/
CR_API ximage_t
imglab_ocv_filter_bilateral (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ sint_t      diameter,
  __CR_IN__ fp64_t      sigma_color,
  __CR_IN__ fp64_t      sigma_space
    )
{
    cv::Mat*    ss = (cv::Mat*)mat;
    cv::Mat*    dd = new(std::nothrow) cv::Mat (ss->size(), ss->type());

    if (dd != NULL)
    {
        int chn = ss->channels();

        // 只能是1或3通道的图像
        if (chn == 1 || chn == 3)
        {
            cv::bilateralFilter(*ss, *dd, diameter, sigma_color, sigma_space);
        }
        else
        if (chn == 4)
        {
            cv::Mat bgr, ret;

            cv::cvtColor(*ss, bgr, cv::COLOR_BGRA2BGR);
            cv::bilateralFilter(bgr, ret, diameter, sigma_color, sigma_space);
            cv::mixChannels(&ret, 1, dd, 1, s_from_to, 3);
            cv::mixChannels(ss, 1, dd, 1, s_from_to + 6, 1);
        }
        else
        {
            delete dd;
            dd = NULL;
        }
    }
    return ((ximage_t)dd);
}

/*
=======================================
    非锐化掩模
=======================================
*/
CR_API ximage_t
imglab_ocv_unsharp_masking (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      ksize_x,
  __CR_IN__ uint_t      ksize_y,
  __CR_IN__ fp64_t      sigma_x,
  __CR_IN__ fp64_t      sigma_y,
  __CR_IN__ fp32_t      kpower
    )
{
    cv::Mat*    ss = (cv::Mat*)mat;
    cv::Mat*    dd = new(std::nothrow) cv::Mat (ss->size(), ss->type());

    // 对原图 I 进行高斯模糊 → 得到 I_blur
    // 计算掩模：mask = I - I_blur
    // 锐化结果：I_sharp = I + k * mask（k 为强度系数）
    if (dd != NULL)
    {
        cv::Mat I, I_blur, mask;

        ss->convertTo(I, CV_32F);
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
        cv::GaussianBlur(I, I_blur, cv::Size(ksize_x, ksize_y), sigma_x, sigma_y);
        mask = I - I_blur;
        mask *= kpower;
        I += mask;
        I.convertTo(*dd, ss->type());
    }
    return ((ximage_t)dd);
}

/*
=======================================
    颜色类聚 (RGB/LAB)
=======================================
*/
CR_API ximage_t
imglab_ocv_color_clustering (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      kind,
  __CR_IN__ uint_t      count,
  __CR_IN__ fp64_t      eps,
  __CR_IN__ uint_t      ntrys,
  __CR_IN__ bool_t      is_lab,
  __CR_IN__ bool_t      kmedoids
    )
{
    int chn = ((cv::Mat*)mat)->channels();

    if (chn != 3 && chn != 4)
        return (NULL);

    cv::Mat*    ss = (cv::Mat*)mat;
    cv::Mat*    dd = new(std::nothrow) cv::Mat (ss->size(), ss->type());

    if (dd != NULL)
    {
        cv::Mat src, samples;

        if (is_lab)
        {
            // LAB 空间
            if (chn == 3) {
                cv::cvtColor(*ss, src, cv::COLOR_BGR2Lab);
            }
            else {
                cv::cvtColor(*ss, src, cv::COLOR_BGRA2BGR);
                cv::cvtColor(src, src, cv::COLOR_BGR2Lab);
            }
        }
        else
        {
            // RGB 空间
            if (chn == 3)
                src = ss->clone();
            else
                cv::cvtColor(*ss, src, cv::COLOR_BGRA2BGR);
        }

        // 构造样本矩阵 (像素数×3)
        samples = src.reshape(1, src.rows * src.cols);
        samples.convertTo(samples, CV_32F);

        cv::Mat labels, centers;

        // K-means 聚类
        cv::kmeans(samples, kind, labels,
                   cv::TermCriteria(cv::TermCriteria::EPS +
                                    cv::TermCriteria::COUNT,
                                    count, eps),
                   ntrys, cv::KMEANS_PP_CENTERS, centers);

        if (kmedoids)
        {
            // 在 kmeans 得到 centers 后
            for (int idx = 0; idx < centers.rows; idx++)
            {
                cv::Point best (-1, -1);
                float min_dist = FLT_MAX;
                cv::Vec3f* ptr = centers.row(idx).ptr<cv::Vec3f>(0);

                for (int yy = 0; yy < src.rows; yy++)
                for (int xx = 0; xx < src.cols; xx++)
                {
                    cv::Vec3f cc = src.at<cv::Vec3b>(yy, xx);
                    float dist = (float)cv::norm(cc, *ptr);

                    if (min_dist > dist) {
                        min_dist = dist;
                        best = cv::Point(xx, yy);
                    }
                }
                *ptr = src.at<cv::Vec3b>(best.y, best.x);
            }
        }

        // 用聚类中心重建图像
        for (int yy = 0; yy < src.rows; yy++)
        for (int xx = 0; xx < src.cols; xx++)
        {
            int idx = labels.at<int>(yy * src.cols + xx, 0);
            cv::Vec3f center = centers.at<cv::Vec3f>(idx, 0);

            src.at<cv::Vec3b>(yy, xx) = cv::Vec3b((uchar)center[0],
                                                  (uchar)center[1],
                                                  (uchar)center[2]);
        }

        // 返回结果
        if (is_lab)
            cv::cvtColor(src, src, cv::COLOR_Lab2BGR);
        cv::mixChannels(&src, 1, dd, 1, s_from_to, 3);
        if (chn == 4)
            cv::mixChannels(ss, 1, dd, 1, s_from_to + 6, 1);
    }
    return ((ximage_t)dd);
}

/*****************************************************************************/
/*                                 图片勾边                                  */
/*****************************************************************************/

/*
=======================================
    Sobel 勾边
=======================================
*/
CR_API ximage_t
imglab_ocv_edge_sobel (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      dx,
  __CR_IN__ uint_t      dy,
  __CR_IN__ uint_t      ksize,
  __CR_IN__ fp64_t      scale,
  __CR_IN__ fp64_t      delta
    )
{
    cv::Mat*    ss = (cv::Mat*)mat;
    cv::Mat*    dd = new(std::nothrow) cv::Mat (ss->size(), ss->type());

    if (dd != NULL)
    {
        cv::Mat grad, grad_x, grad_y;

        // 参数过滤
        if (dx < 1)
            dx = 1;
        else
        if (dx > 2)
            dx = 2;
        if (dy < 1)
            dy = 1;
        else
        if (dy > 2)
            dy = 2;
        if (ksize < 1)
            ksize = 1;
        else
        if (ksize > 7)
            ksize = 7;
        else
        if (ksize % 2 == 0)
            ksize += 1;

        // 计算 X 方向梯度
        cv::Sobel(*ss, grad_x, CV_16S, dx, 0, ksize, scale, delta);

        // 计算 Y 方向梯度
        cv::Sobel(*ss, grad_y, CV_16S, 0, dy, ksize, scale, delta);

        // 合并梯度幅值
        cv::convertScaleAbs(grad_x, grad_x);
        cv::convertScaleAbs(grad_y, grad_y);
        cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, *dd);
    }
    return ((ximage_t)dd);
}

/*
=======================================
    Scharr 勾边
=======================================
*/
CR_API ximage_t
imglab_ocv_edge_scharr (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ fp64_t      scale,
  __CR_IN__ fp64_t      delta
    )
{
    cv::Mat*    ss = (cv::Mat*)mat;
    cv::Mat*    dd = new(std::nothrow) cv::Mat (ss->size(), ss->type());

    if (dd != NULL)
    {
        cv::Mat grad, grad_x, grad_y;

        // 计算 X 方向梯度
        cv::Scharr(*ss, grad_x, CV_16S, 1, 0, scale, delta);

        // 计算 Y 方向梯度
        cv::Scharr(*ss, grad_y, CV_16S, 0, 1, scale, delta);

        // 合并梯度幅值
        cv::convertScaleAbs(grad_x, grad_x);
        cv::convertScaleAbs(grad_y, grad_y);
        cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, *dd);
    }
    return ((ximage_t)dd);
}

/*
=======================================
    Laplacian 勾边
=======================================
*/
CR_API ximage_t
imglab_ocv_edge_laplacian (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      ksize,
  __CR_IN__ fp64_t      scale,
  __CR_IN__ fp64_t      delta
    )
{
    cv::Mat*    ss = (cv::Mat*)mat;
    cv::Mat*    dd = new(std::nothrow) cv::Mat (ss->size(), ss->type());

    if (dd != NULL)
    {
        cv::Mat laps;

        // 参数过滤
        if (ksize < 1)
            ksize = 1;
        else
        if (ksize > 7)
            ksize = 7;
        else
        if (ksize % 2 == 0)
            ksize += 1;
        cv::Laplacian(*ss, laps, CV_16S, ksize, scale, delta);
        cv::convertScaleAbs(laps, *dd);
    }
    return ((ximage_t)dd);
}

/*
=======================================
    Canny 勾边
=======================================
*/
CR_API ximage_t
imglab_ocv_edge_canny (
  __CR_IN__ ximage_t    mat,
  __CR_IN__ fp64_t      lgate,
  __CR_IN__ fp64_t      hgate,
  __CR_IN__ uint_t      ksize,
  __CR_IN__ bool_t      l2grad
    )
{
    cv::Mat*    ss = (cv::Mat*)mat;
    cv::Mat*    dd = new(std::nothrow) cv::Mat (ss->size(), CV_8UC1);

    if (dd != NULL)
    {
        cv::Mat gray;
        int chn = ss->channels();

        // 输入必须是8位单通道图
        if (chn == 3)
            cv::cvtColor(*ss, gray, cv::COLOR_BGR2GRAY);
        else
        if (chn == 4)
            cv::cvtColor(*ss, gray, cv::COLOR_BGRA2GRAY);
        else
            gray = ss->clone();
        if (gray.depth() != CV_8U) {
            cv::normalize(gray, gray, 0, 255, cv::NORM_MINMAX);
            gray.convertTo(gray, CV_8U);
        }
        cv::Canny(gray, *dd, lgate, hgate, ksize, !!l2grad);
    }
    return ((ximage_t)dd);
}
