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
 *             ##       CREATE: 2023-05-08
 *              #
 ================================================
        图像 LAB OpenCV 分类函数库
 ================================================
 */

#include "imglab_int.hpp"

/*****************************************************************************/
/*                                级联分类器                                 */
/*****************************************************************************/

/*
=======================================
    创建级联分类器
=======================================
*/
CR_API cascade_ocv_t
imglab_ocv_cascade_new (
  __CR_IN__ const ansi_t*   file
    )
{
    cv::CascadeClassifier*  cscd;

    cscd = new cv::CascadeClassifier ();
    if (cscd == NULL)
        return (NULL);
    if (file != NULL) {
        if (!cscd->load(file) || cscd->empty()) {
            delete cscd;
            return (NULL);
        }
    }
    return ((cascade_ocv_t)cscd);
}

/*
=======================================
    释放级联分类器
=======================================
*/
CR_API void_t
imglab_ocv_cascade_del (
  __CR_IN__ cascade_ocv_t   clss
    )
{
    cv::CascadeClassifier*  cscd;

    cscd = (cv::CascadeClassifier*)clss;
    delete cscd;
}

/*
=======================================
    加载级联分类器模型
=======================================
*/
CR_API bool_t
imglab_ocv_cascade_load (
  __CR_IN__ cascade_ocv_t   clss,
  __CR_IN__ const ansi_t*   file
    )
{
    cv::CascadeClassifier*  cscd;

    cscd = (cv::CascadeClassifier*)clss;
    if (!cscd->load(file) || cscd->empty())
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    执行级联分类器分类
=======================================
*/
CR_API xrect_lst_t
imglab_ocv_cascade_doit (
  __CR_IN__ cascade_ocv_t   clss,
  __CR_IN__ ximage_t        mat,
  __CR_IN__ fp32_t          fscale,
  __CR_IN__ sint_t          min_nghbrs,
  __CR_IN__ sint_t          flags,
  __CR_IN__ uint_t          min_width,
  __CR_IN__ uint_t          min_height,
  __CR_IN__ uint_t          max_width,
  __CR_IN__ uint_t          max_height
    )
{
    cv::CascadeClassifier*  cscd;

    cscd = (cv::CascadeClassifier*)clss;
    if (cscd->empty())
        return (NULL);

    std::vector<cv::Rect>*  rcts;

    rcts = new std::vector<cv::Rect> ();
    if (rcts == NULL)
        return (NULL);

    cv::Mat*    mm = (cv::Mat*)mat;
    cv::Size    smin(min_width, min_height);
    cv::Size    smax(max_width, max_height);

    cscd->detectMultiScale(*mm, *rcts, fscale, min_nghbrs, flags, smin, smax);
    return ((xrect_lst_t)rcts);
}
