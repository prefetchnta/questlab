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
        ImageLAB OpenCV 目标识别函数库
 ================================================
 */

#include "imglab_int.hpp"
#include "opencv2/wechat_qrcode.hpp"

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

    cscd = new(std::nothrow) cv::CascadeClassifier ();
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

    rcts = new(std::nothrow) std::vector<cv::Rect> ();
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
        bar1 = new(std::nothrow) cv::barcode::BarcodeDetector (sr_prototxt, sr_model);
    else
        bar1 = new(std::nothrow) cv::barcode::BarcodeDetector ();
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
                if ((*text)[idx * 2] != NULL)
                {
                    ansi_t* str = (*text)[idx * 2];

                    while (*str != 0) {
                        if (*str == '_')
                            *str = '-';
                        str++;
                    }
                }
            }
            else {
                (*text)[idx * 2 + 0] = NULL;
                (*text)[idx * 2 + 1] = NULL;
            }
        }
    }
    return (cnts);
}

/*****************************************************************************/
/*                                二维码识别                                 */
/*****************************************************************************/

/* 内部结构 */
struct  sQR2D_OCV
{
    uint_t  type;

    union {
        cv::QRCodeDetector*                 cmmt;
        cv::QRCodeDetectorAruco*            aruc;
        cv::wechat_qrcode::WeChatQRCode*    wcht;
    };
};

/*
=======================================
    创建二维码识别器
=======================================
*/
CR_API qr2code_ocv_t
imglab_ocv_qr2code_new (
  __CR_IN__ uint_t                          type,
  __CR_IN__ const sOCV_QRCodeWeChatParam*   wechat
    )
{
    sQR2D_OCV*  qr2d;

    qr2d = struct_new(sQR2D_OCV);
    if (qr2d == NULL)
        return (NULL);

    if (type == QR2D_OCV_TYPE_NORMAL) {
        qr2d->cmmt = new(std::nothrow) cv::QRCodeDetector ();
        if (qr2d->cmmt == NULL)
            goto _failure;
    }
    else
    if (type == QR2D_OCV_TYPE_ARUCOX) {
        qr2d->aruc = new(std::nothrow) cv::QRCodeDetectorAruco ();
        if (qr2d->aruc == NULL)
            goto _failure;
    }
    else
    if (type == QR2D_OCV_TYPE_WECHAT) {
        if (wechat != NULL) {
            qr2d->wcht = new(std::nothrow) cv::wechat_qrcode::WeChatQRCode (
                                wechat->det_prototxt, wechat->det_model,
                                wechat->sr_prototxt, wechat->sr_model);
        }
        else {
            qr2d->wcht = new(std::nothrow) cv::wechat_qrcode::WeChatQRCode ();
        }
        if (qr2d->wcht == NULL)
            goto _failure;
    }
    else {
        goto _failure;
    }
    qr2d->type = type;
    return ((qr2code_ocv_t)qr2d);

_failure:
    mem_free(qr2d);
    return (NULL);
}

/*
=======================================
    释放二维码识别器
=======================================
*/
CR_API void_t
imglab_ocv_qr2code_del (
  __CR_IN__ qr2code_ocv_t   qr2d
    )
{
    sQR2D_OCV*  qrcd;

    qrcd = (sQR2D_OCV*)qr2d;
    if (qrcd->type == QR2D_OCV_TYPE_NORMAL)
        delete qrcd->cmmt;
    else
    if (qrcd->type == QR2D_OCV_TYPE_ARUCOX)
        delete qrcd->aruc;
    else
    if (qrcd->type == QR2D_OCV_TYPE_WECHAT)
        delete qrcd->wcht;
    mem_free(qrcd);
}

/*
=======================================
    设置二维码识别器
=======================================
*/
CR_API void_t
imglab_ocv_qr2code_param (
  __CR_IN__ qr2code_ocv_t   qr2d,
  __CR_IN__ const void_t*   param_data,
  __CR_IN__ size_t          param_size
    )
{
    sQR2D_OCV*  qrcd;

    qrcd = (sQR2D_OCV*)qr2d;
    if (qrcd->type == QR2D_OCV_TYPE_NORMAL) {
        if (param_size == sizeof(sOCV_QRCodeParam))
        {
            sOCV_QRCodeParam*   parm;

            parm = (sOCV_QRCodeParam*)param_data;
            if (parm->epsX > 0)
                qrcd->cmmt->setEpsX(parm->epsX);
            if (parm->epsY > 0)
                qrcd->cmmt->setEpsY(parm->epsY);
            qrcd->cmmt->setUseAlignmentMarkers(!!parm->useAlignmentMarkers);
        }
    }
    else
    if (qrcd->type == QR2D_OCV_TYPE_ARUCOX) {
        if (param_size == sizeof(sOCV_QRCodeArucoParam))
        {
            sOCV_QRCodeArucoParam*          parm;
            cv::QRCodeDetectorAruco::Params copy;

            parm = (sOCV_QRCodeArucoParam*)param_data;
            copy.minModuleSizeInPyramid   = parm->minModuleSizeInPyramid;
            copy.maxRotation              = parm->maxRotation;
            copy.maxModuleSizeMismatch    = parm->maxModuleSizeMismatch;
            copy.maxTimingPatternMismatch = parm->maxTimingPatternMismatch;
            copy.maxPenalties             = parm->maxPenalties;
            copy.maxColorsMismatch        = parm->maxColorsMismatch;
            copy.scaleTimingPatternScore  = parm->scaleTimingPatternScore;
            qrcd->aruc->setDetectorParameters(copy);
        }
        else
        if (param_size == sizeof(sOCV_QRCodeArucoDetector))
        {
            sOCV_QRCodeArucoDetector*       parm;
            cv::aruco::DetectorParameters   copy;

            parm = (sOCV_QRCodeArucoDetector*)param_data;
            copy.adaptiveThreshWinSizeMin              =   parm->adaptiveThreshWinSizeMin;
            copy.adaptiveThreshWinSizeMax              =   parm->adaptiveThreshWinSizeMax;
            copy.adaptiveThreshWinSizeStep             =   parm->adaptiveThreshWinSizeStep;
            copy.adaptiveThreshConstant                =   parm->adaptiveThreshConstant;
            copy.minMarkerPerimeterRate                =   parm->minMarkerPerimeterRate;
            copy.maxMarkerPerimeterRate                =   parm->maxMarkerPerimeterRate;
            copy.polygonalApproxAccuracyRate           =   parm->polygonalApproxAccuracyRate;
            copy.minCornerDistanceRate                 =   parm->minCornerDistanceRate;
            copy.minDistanceToBorder                   =   parm->minDistanceToBorder;
            copy.minMarkerDistanceRate                 =   parm->minMarkerDistanceRate;
            copy.minGroupDistance                      =   parm->minGroupDistance;
            copy.cornerRefinementMethod                =   parm->cornerRefinementMethod;
            copy.cornerRefinementWinSize               =   parm->cornerRefinementWinSize;
            copy.relativeCornerRefinmentWinSize        =   parm->relativeCornerRefinmentWinSize;
            copy.cornerRefinementMaxIterations         =   parm->cornerRefinementMaxIterations;
            copy.cornerRefinementMinAccuracy           =   parm->cornerRefinementMinAccuracy;
            copy.markerBorderBits                      =   parm->markerBorderBits;
            copy.perspectiveRemovePixelPerCell         =   parm->perspectiveRemovePixelPerCell;
            copy.perspectiveRemoveIgnoredMarginPerCell =   parm->perspectiveRemoveIgnoredMarginPerCell;
            copy.maxErroneousBitsInBorderRate          =   parm->maxErroneousBitsInBorderRate;
            copy.minOtsuStdDev                         =   parm->minOtsuStdDev;
            copy.errorCorrectionRate                   =   parm->errorCorrectionRate;
            copy.aprilTagQuadDecimate                  =   parm->aprilTagQuadDecimate;
            copy.aprilTagQuadSigma                     =   parm->aprilTagQuadSigma;
            copy.aprilTagMinClusterPixels              =   parm->aprilTagMinClusterPixels;
            copy.aprilTagMaxNmaxima                    =   parm->aprilTagMaxNmaxima;
            copy.aprilTagCriticalRad                   =   parm->aprilTagCriticalRad;
            copy.aprilTagMaxLineFitMse                 =   parm->aprilTagMaxLineFitMse;
            copy.aprilTagMinWhiteBlackDiff             =   parm->aprilTagMinWhiteBlackDiff;
            copy.aprilTagDeglitch                      =   parm->aprilTagDeglitch;
            copy.detectInvertedMarker                  = !!parm->detectInvertedMarker;
            copy.useAruco3Detection                    = !!parm->useAruco3Detection;
            copy.minSideLengthCanonicalImg             =   parm->minSideLengthCanonicalImg;
            copy.minMarkerLengthRatioOriginalImg       =   parm->minMarkerLengthRatioOriginalImg;
            qrcd->aruc->setArucoParameters(copy);
        }
    }
    else
    if (qrcd->type == QR2D_OCV_TYPE_WECHAT) {
        if (param_size == sizeof(sOCV_QRCodeWeChatParam))
        {
            sOCV_QRCodeWeChatParam* parm;

            parm = (sOCV_QRCodeWeChatParam*)param_data;
            if (parm->fscale > 0)
                qrcd->wcht->setScaleFactor(parm->fscale);
        }
    }
}

/*
=======================================
    执行二维码的识别
=======================================
*/
CR_API size_t
imglab_ocv_qr2code_doit (
  __CR_IN__ qr2code_ocv_t   qr2d,
  __CR_IN__ ximage_t        mat,
  __CR_OT__ str_lstA_t*     text,
  __CR_OT__ xpoly_lst_t*    list
    )
{
    std::vector<cv::Point>  pnts;
    /* ----------------------- */
    cv::Mat*    mm = (cv::Mat*)mat;
    sQR2D_OCV*  qrcd = (sQR2D_OCV*)qr2d;

    if (text == NULL) {
        if (qrcd->type == QR2D_OCV_TYPE_NORMAL ||
            qrcd->type == QR2D_OCV_TYPE_ARUCOX)
        {
            cv::GraphicalCodeDetector*  gc_det;

            gc_det = (cv::GraphicalCodeDetector*)qrcd->cmmt;
            if (!gc_det->detectMulti(*mm, pnts))
                return (0);
        }
        else
        if (qrcd->type == QR2D_OCV_TYPE_WECHAT)
        {
            std::vector<cv::Mat>    mats;

            qrcd->wcht->detectAndDecode(*mm, mats);
            if (mats.size() < 1)
                return (0);
            imglab_mats_to_points(&pnts, mats);
        }
        else {
            return (0);
        }

        if (list != NULL)
            *list = imglab_split_to_xpoly(&pnts, 4);
        return (pnts.size() / 4);
    }

    std::vector<std::string>    dinfo;

    if (qrcd->type == QR2D_OCV_TYPE_NORMAL ||
        qrcd->type == QR2D_OCV_TYPE_ARUCOX)
    {
        cv::GraphicalCodeDetector*  gc_det;

        gc_det = (cv::GraphicalCodeDetector*)qrcd->cmmt;
        if (!gc_det->detectAndDecodeMulti(*mm, dinfo, pnts))
            return (0);
    }
    else
    if (qrcd->type == QR2D_OCV_TYPE_WECHAT)
    {
        std::vector<cv::Mat>    mats;

        dinfo = qrcd->wcht->detectAndDecode(*mm, mats);
        if (mats.size() < 1)
            return (0);
        imglab_mats_to_points(&pnts, mats);
    }
    else {
        return (0);
    }
    if (list != NULL)
        *list = imglab_split_to_xpoly(&pnts, 4);

    size_t  cnts = pnts.size() / 4;

    *text = mem_talloc(cnts * 2, ansi_t*);
    if (*text != NULL) {
        for (size_t idx = 0; idx < cnts; idx++) {
            if (idx < dinfo.size() && !dinfo[idx].empty()) {
                (*text)[idx * 2 + 0] = str_dupA("QRCode");
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
