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

/*****************************************************************************/
/*                                一维码识别                                 */
/*****************************************************************************/

/*
=======================================
    创建一维码识别器
=======================================
*/
CR_API barcode_ocv_t
imglab_ocv_barcode_new (
  __CR_IN__ const ansi_t*   sr_model,
  __CR_IN__ const ansi_t*   sr_prototxt
    )
{
    cv::barcode::BarcodeDetector*   bar1;

    if (sr_model != NULL && sr_prototxt != NULL)
        bar1 = new cv::barcode::BarcodeDetector (sr_prototxt, sr_model);
    else
        bar1 = new cv::barcode::BarcodeDetector ();
    if (bar1 == NULL)
        return (NULL);
    return ((barcode_ocv_t)bar1);
}

/*
=======================================
    释放一维码识别器
=======================================
*/
CR_API void_t
imglab_ocv_barcode_del (
  __CR_IN__ barcode_ocv_t   bar1
    )
{
    cv::barcode::BarcodeDetector*   tbar;

    tbar = (cv::barcode::BarcodeDetector*)bar1;
    delete tbar;
}

/*
=======================================
    设置一维码识别器
=======================================
*/
CR_API void_t
imglab_ocv_barcode_param (
  __CR_IN__ barcode_ocv_t   bar1,
  __CR_IN__ fp32_t          dnsp_lmt,
  __CR_IN__ fp32_t          grad_thr,
  __CR_IN__ const fp32_t*   box_sizes,
  __CR_IN__ uint_t          size_count
    )
{
    cv::barcode::BarcodeDetector*   tbar;

    tbar = (cv::barcode::BarcodeDetector*)bar1;
    if (dnsp_lmt > 0)
        tbar->setDownsamplingThreshold(dnsp_lmt);
    if (grad_thr > 0)
        tbar->setGradientThreshold(grad_thr);
    if (box_sizes != NULL && size_count != 0)
    {
        std::vector<float>  sizes;

        sizes.reserve(size_count);
        for (uint_t idx = 0; idx < size_count; idx++)
            sizes.push_back(box_sizes[idx]);
        tbar->setDetectorScales(sizes);
    }
}

/*
=======================================
    列表多边形生成
=======================================
*/
extern xpoly_lst_t
imglab_split_to_xpoly (
  __CR_IN__ std::vector<cv::Point>* pnts,
  __CR_IN__ size_t                  usize
    )
{
    size_t  count = pnts->size();

    if (count < usize || count % usize != 0)
        return (NULL);

    std::vector<std::vector<cv::Point> >*   list;

    list = new std::vector<std::vector<cv::Point> > ();
    if (list == NULL)
        return (NULL);
    list->reserve(count / usize);

    for (size_t idx = 0; idx < count; idx += usize)
    {
        std::vector<cv::Point>  pt (pnts->begin() + idx,
                                    pnts->begin() + idx + usize);
        list->push_back(pt);
    }
    return ((xpoly_lst_t)list);
}

/*
=======================================
    执行一维码的识别
=======================================
*/
CR_API size_t
imglab_ocv_barcode_doit (
  __CR_IN__ barcode_ocv_t   bar1,
  __CR_IN__ ximage_t        mat,
  __CR_OT__ str_lstA_t*     text,
  __CR_OT__ xpoly_lst_t*    list
    )
{
    std::vector<cv::Point>          pnts;
    cv::barcode::BarcodeDetector*   tbar;

    tbar = (cv::barcode::BarcodeDetector*)bar1;

    cv::Mat*    mm = (cv::Mat*)mat;

    if (text == NULL) {
        if (!tbar->detectMulti(*mm, pnts))
            return (0);
        if (list != NULL)
            *list = imglab_split_to_xpoly(&pnts, 4);
        return (pnts.size() / 4);
    }

    std::vector<std::string>    dinfo, dtype;

    if (!tbar->detectAndDecodeWithType(*mm, dinfo, dtype, pnts))
        return (0);
    if (list != NULL)
        *list = imglab_split_to_xpoly(&pnts, 4);

    size_t  cnts = pnts.size() / 4;

    *text = mem_talloc(cnts * 2, ansi_t*);
    if (*text != NULL) {
        for (size_t idx = 0; idx < cnts; idx++) {
            if (idx < dinfo.size() &&
                idx < dtype.size() && !dtype[idx].empty()) {
                (*text)[idx * 2 + 0] = str_dupA(dtype[idx].c_str());
                (*text)[idx * 2 + 1] = str_dupA(dinfo[idx].c_str());
            }
            else {
                (*text)[idx * 2 + 0] = NULL;
                (*text)[idx * 2 + 1] = NULL;
            }
        }
    }
    return (cnts);
}
