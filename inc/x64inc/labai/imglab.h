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
 *             ##       CREATE: 2018-03-05
 *              #
 ================================================
        图像 LAB 头文件
 ================================================
 */

#ifndef __AI_IMGLAB_H__
#define __AI_IMGLAB_H__

#include "crhack.h"

/*****************************************************************************/
/*                                 结构转换                                  */
/*****************************************************************************/

/* OpenCV 矩阵结构类型 */
typedef void_t*     ximage_t;   /* cv::Mat* */

/* CrHack 与 OpenCV 矩阵结构释放 */
CR_API void_t   imglab_mat_del (ximage_t mat);
CR_API void_t   imglab_crh_del (const sIMAGE *img);

/* CrHack 与 OpenCV 矩阵结构交换 */
CR_API sIMAGE*  imglab_mat2crh_dup (ximage_t mat);
CR_API bool_t   imglab_mat2crh_set (sIMAGE *img, ximage_t mat);
CR_API ximage_t imglab_crh2mat_dup (const sIMAGE *img);
CR_API ximage_t imglab_crh2mat_set (const sIMAGE *img);

/*****************************************************************************/
/*                                 图片加载                                  */
/*****************************************************************************/

/* OpenCV 图片文件加载 */
CR_API ximage_t imglab_load_file (const ansi_t *name);
CR_API ximage_t imglab_load_data (const void_t *data, leng_t size);

/* OpenCV 图片文件保存 */
CR_API bool_t   imglab_save_file_png (const ansi_t *name, ximage_t mat);

/*****************************************************************************/
/*                                 视频加载                                  */
/*****************************************************************************/

/* OpenCV 视频文件类型 */
typedef void_t*     xvideo_t;   /* cv::VideoCapture* */

CR_API xvideo_t imglab_video_new (const ansi_t *name);
CR_API void_t   imglab_video_del (xvideo_t avi);
CR_API ximage_t imglab_video_get (xvideo_t avi);
CR_API int64u   imglab_video_count (xvideo_t avi);
CR_API int64u   imglab_video_seek (xvideo_t avi, int64u frame);

/*****************************************************************************/
/*                                摄像头操作                                 */
/*****************************************************************************/

/* OpenCV 摄像头接口 */
#define CAP_OCV_ANY             0
#define CAP_OCV_IEEE1394        300
#define CAP_OCV_DSHOW           700
#define CAP_OCV_PVAPI           800
#define CAP_OCV_OPENNI          900
#define CAP_OCV_OPENNI_ASUS     910
#define CAP_OCV_ANDROID         1000
#define CAP_OCV_XIAPI           1100
#define CAP_OCV_AVFOUNDATION    1200
#define CAP_OCV_GIGANETIX       1300
#define CAP_OCV_MSMF            1400
#define CAP_OCV_WINRT           1410
#define CAP_OCV_INTELPERC       1500
#define CAP_OCV_OPENNI2         1600
#define CAP_OCV_OPENNI2_ASUS    1610
#define CAP_OCV_OPENNI2_ASTRA   1620
#define CAP_OCV_GPHOTO2         1700
#define CAP_OCV_GSTREAMER       1800
#define CAP_OCV_FFMPEG          1900
#define CAP_OCV_IMAGES          2000
#define CAP_OCV_ARAVIS          2100
#define CAP_OCV_OPENCV_MJPEG    2200
#define CAP_OCV_INTEL_MFX       2300
#define CAP_OCV_XINE            2400
#define CAP_OCV_UEYE            2500
#define CAP_OCV_OBSENSOR        2600

/* OpenCV 摄像头类型 */
typedef void_t*     camera_t;   /* cv::VideoCapture* */

CR_API camera_t imglab_camera_new (uint_t id, uint_t type);
CR_API camera_t imglab_camera_new2 (const ansi_t *name, uint_t type);
CR_API void_t   imglab_camera_del (camera_t cam);
CR_API ximage_t imglab_camera_get (camera_t cam);

/*****************************************************************************/
/*                                 目标区域                                  */
/*****************************************************************************/

/* OpenCV 区域结构类型 */
typedef void_t*     xrect_t;        /* cv::Rect* */
typedef void_t*     xrect_lst_t;    /* std::vector<cv::Rect>* */
typedef void_t*     xroct_t;        /* cv::RotatedRect* */
typedef void_t*     xroct_lst_t;    /* std::vector<cv::RotatedRect>* */
typedef void_t*     xpnt2_t;        /* cv::Point* */
typedef void_t*     xpoly_t;        /* std::vector<cv::Point>* */
typedef void_t*     xpoly_lst_t;    /* std::vector<std::vector<cv::Point>>* */

/* OpenCV 区域结构释放 */
CR_API void_t   imglab_xpoly_del (xpoly_t ply);
CR_API void_t   imglab_rects_del (xrect_lst_t list);
CR_API void_t   imglab_rocts_del (xroct_lst_t list);
CR_API void_t   imglab_polys_del (xpoly_lst_t list);

/* OpenCV 区域结构操作 */
CR_API uint_t   imglab_xpoly_count (xpoly_t ply);
CR_API uint_t   imglab_rects_count (xrect_lst_t list);
CR_API uint_t   imglab_rocts_count (xroct_lst_t list);
CR_API uint_t   imglab_polys_count (xpoly_lst_t list);
CR_API xpnt2_t  imglab_xpoly_idx (xpoly_t ply, uint_t idx);
CR_API xrect_t  imglab_rects_idx (xrect_lst_t list, uint_t idx);
CR_API xroct_t  imglab_rocts_idx (xroct_lst_t list, uint_t idx);
CR_API xpoly_t  imglab_polys_idx (xpoly_lst_t list, uint_t idx);
CR_API xpnt2_t  imglab_xpoly_idx2 (xpoly_t ply, uint_t idx);
CR_API xrect_t  imglab_rects_idx2 (xrect_lst_t list, uint_t idx);
CR_API xroct_t  imglab_rocts_idx2 (xroct_lst_t list, uint_t idx);
CR_API xpoly_t  imglab_polys_idx2 (xpoly_lst_t list, uint_t idx);

/* CrHack 与 OpenCV 区域结构交换 */
CR_API void_t   imglab_xpoly_get (sPNT2 *pnt, xpnt2_t con,
                                  uint_t count CR_DEFAULT(1));

CR_API void_t   imglab_xpoly_set (xpnt2_t con, const sPNT2 *pnt,
                                  uint_t count CR_DEFAULT(1));

CR_API void_t   imglab_rects_get (sRECT *box, xrect_t rct,
                                  uint_t count CR_DEFAULT(1));

CR_API void_t   imglab_rects_set (xrect_t rct, const sRECT *box,
                                  uint_t count CR_DEFAULT(1));

CR_API void_t   imglab_rocts_get (sPNT2 *pnt, xroct_t rct,
                                  uint_t count CR_DEFAULT(1));

CR_API void_t   imglab_rocts_get2 (sRECT *box, fp32_t *angle,
                      xroct_t rct, uint_t count CR_DEFAULT(1));

CR_API void_t   imglab_rocts_set (xroct_t rct, const sRECT *box,
                                  const fp32_t *angle,
                                  uint_t count CR_DEFAULT(1));

CR_API void_t   imglab_rocts_set2 (xroct_t rct, const sPNT2 *pnt,
                                   uint_t count CR_DEFAULT(1));
/* OpenCV 绘制调试框 */
CR_API void_t   imglab_draw_xrect (ximage_t mat, xrect_t rct,
                                   cpix_t color, int xbias, int ybias,
                                   int lbold);

CR_API void_t   imglab_draw_rects (ximage_t mat, xrect_lst_t list,
                                   cpix_t color, int xbias, int ybias,
                                   int lbold, int index);

CR_API void_t   imglab_draw_xroct (ximage_t mat, xroct_t rct,
                                   cpix_t color, int lbold);

CR_API void_t   imglab_draw_rocts (ximage_t mat, xroct_lst_t list,
                                   cpix_t color, int lbold, int index);

CR_API void_t   imglab_draw_xploy (ximage_t mat, xpoly_t ply,
                                   cpix_t color, int lbold);

CR_API void_t   imglab_draw_ploys (ximage_t mat, xpoly_lst_t list,
                                   cpix_t color, int lbold, int index);

/*****************************************************************************/
/*                                 图形绘制                                  */
/*****************************************************************************/

/* OpenCV 绘制函数 */
#define FONT_OCV_HERSHEY_SIMPLEX        0
#define FONT_OCV_HERSHEY_PLAIN          1
#define FONT_OCV_HERSHEY_DUPLEX         2
#define FONT_OCV_HERSHEY_COMPLEX        3
#define FONT_OCV_HERSHEY_TRIPLEX        4
#define FONT_OCV_HERSHEY_COMPLEX_SMALL  5
#define FONT_OCV_HERSHEY_SCRIPT_SIMPLEX 6
#define FONT_OCV_HERSHEY_SCRIPT_COMPLEX 7
#define FONT_OCV_ITALIC                 16
CR_API void_t   imglab_draw_text (ximage_t mat, const ansi_t *text,
                            sint_t dx, sint_t dy, sint_t font, sint_t height,
                                    cpix_t color, sint_t lbold);

CR_API sint_t   imglab_calc_text (sPNT2 *size, const ansi_t *text,
                            sint_t font, sint_t height, sint_t lbold);
/* CrHack 绘制函数 */
CR_API void_t   imglab_draw_gb2312 (ximage_t mat, const ansi_t *text,
                            sint_t dx, sint_t dy, uint_t height, int32u mode,
                                    cpix_t color, cpix_t bkcolor);

CR_API void_t   imglab_calc_gb2312 (sPNT2 *size, const ansi_t *text,
                                    uint_t height);

/*****************************************************************************/
/*                                 图片滤波                                  */
/*****************************************************************************/

/* OpenCV 均值滤波 */
CR_API void_t   imglab_ocv_blur_box (ximage_t mat, uint_t ksize_x,
                                     uint_t ksize_y);
/* OpenCV 高斯滤波 */
CR_API void_t   imglab_ocv_blur_gauss (ximage_t mat, uint_t ksize_x,
                                       uint_t ksize_y, fp64_t sigma_x,
                                       fp64_t sigma_y);
/* OpenCV 中值滤波 */
CR_API void_t   imglab_ocv_blur_median (ximage_t mat, uint_t ksize);

/*****************************************************************************/
/*                                 目标识别                                  */
/*****************************************************************************/

/*******************/
/* OpenCV 级联分类 */
/*******************/
typedef void_t*     cascade_ocv_t;      /* cv::CascadeClassifier* */

CR_API cascade_ocv_t
imglab_ocv_cascade_new (const ansi_t *file);

CR_API void_t
imglab_ocv_cascade_del (cascade_ocv_t clss);

CR_API bool_t
imglab_ocv_cascade_load (cascade_ocv_t clss, const ansi_t *file);

#define CSCD_OCV_DO_CANNY_PRUNING       1
#define CSCD_OCV_SCALE_IMAGE            2
#define CSCD_OCV_FIND_BIGGEST_OBJECT    4
#define CSCD_OCV_DO_ROUGH_SEARCH        8

CR_API xrect_lst_t
imglab_ocv_cascade_doit (cascade_ocv_t clss, ximage_t mat, fp32_t fscale,
                         sint_t min_nghbrs, sint_t flags, uint_t min_width,
                                uint_t min_height, uint_t max_width,
                                        uint_t max_height);
/*********************/
/* OpenCV 一维码识别 */
/*********************/
typedef void_t*     barcode_ocv_t;      /* cv::barcode::BarcodeDetector* */

CR_API barcode_ocv_t
imglab_ocv_barcode_new (const ansi_t *sr_model CR_DEFAULT(NULL),
                        const ansi_t *sr_prototxt CR_DEFAULT(NULL));
CR_API void_t
imglab_ocv_barcode_del (barcode_ocv_t bar1);

CR_API void_t
imglab_ocv_barcode_param (barcode_ocv_t bar1, fp32_t dnsp_lmt,
                          fp32_t grad_thr, const fp32_t *box_sizes,
                                    uint_t size_count);
CR_API size_t
imglab_ocv_barcode_doit (barcode_ocv_t bar1, ximage_t mat, str_lstA_t *text,
                                    xpoly_lst_t *list);
/*********************/
/* OpenCV 二维码识别 */
/*********************/
typedef void_t*     qr2code_ocv_t;      /* cv::QRCodeDetector
                                           cv::QRCodeDetectorAruco
                                           cv::WeChatQRCode */
typedef struct
{
    fp64_t  epsX;
    fp64_t  epsY;
    bool_t  useAlignmentMarkers;

} sOCV_QRCodeParam;

typedef struct  /* cv::QRCodeDetectorAruco::Params */
{
    fp32_t  minModuleSizeInPyramid;
    fp32_t  maxRotation;
    fp32_t  maxModuleSizeMismatch;
    fp32_t  maxTimingPatternMismatch;
    fp32_t  maxPenalties;
    fp32_t  maxColorsMismatch;
    fp32_t  scaleTimingPatternScore;

} sOCV_QRCodeArucoParam;

/* sOCV_QRCodeArucoDetector::cornerRefinementMethod */
#define QR2D_OCV_CORNER_REFINE_NONE     0
#define QR2D_OCV_CORNER_REFINE_SUBPIX   1
#define QR2D_OCV_CORNER_REFINE_CONTOUR  2
#define QR2D_OCV_CORNER_REFINE_APRILTAG 3

typedef struct  /* cv::aruco::DetectorParameters */
{
    sint_t  adaptiveThreshWinSizeMin;
    sint_t  adaptiveThreshWinSizeMax;
    sint_t  adaptiveThreshWinSizeStep;
    fp64_t  adaptiveThreshConstant;
    fp64_t  minMarkerPerimeterRate;
    fp64_t  maxMarkerPerimeterRate;
    fp64_t  polygonalApproxAccuracyRate;
    fp64_t  minCornerDistanceRate;
    sint_t  minDistanceToBorder;
    fp64_t  minMarkerDistanceRate;
    fp32_t  minGroupDistance;
    sint_t  cornerRefinementMethod;
    sint_t  cornerRefinementWinSize;
    fp32_t  relativeCornerRefinmentWinSize;
    sint_t  cornerRefinementMaxIterations;
    fp64_t  cornerRefinementMinAccuracy;
    sint_t  markerBorderBits;
    sint_t  perspectiveRemovePixelPerCell;
    fp64_t  perspectiveRemoveIgnoredMarginPerCell;
    fp64_t  maxErroneousBitsInBorderRate;
    fp64_t  minOtsuStdDev;
    fp64_t  errorCorrectionRate;
    fp32_t  aprilTagQuadDecimate;
    fp32_t  aprilTagQuadSigma;
    sint_t  aprilTagMinClusterPixels;
    sint_t  aprilTagMaxNmaxima;
    fp32_t  aprilTagCriticalRad;
    fp32_t  aprilTagMaxLineFitMse;
    sint_t  aprilTagMinWhiteBlackDiff;
    sint_t  aprilTagDeglitch;
    bool_t  detectInvertedMarker;
    bool_t  useAruco3Detection;
    sint_t  minSideLengthCanonicalImg;
    fp32_t  minMarkerLengthRatioOriginalImg;

} sOCV_QRCodeArucoDetector;

typedef struct
{
    fp32_t          fscale;
    const ansi_t*   sr_model;
    const ansi_t*   sr_prototxt;
    const ansi_t*   det_model;
    const ansi_t*   det_prototxt;

} sOCV_QRCodeWeChatParam;

#define QR2D_OCV_TYPE_NORMAL    0   /* cv::QRCodeDetector */
#define QR2D_OCV_TYPE_ARUCOX    1   /* cv::QRCodeDetectorAruco */
#define QR2D_OCV_TYPE_WECHAT    2   /* cv::wechat_qrcode::WeChatQRCode */

CR_API qr2code_ocv_t
imglab_ocv_qr2code_new (uint_t type, const sOCV_QRCodeWeChatParam *wechat);

CR_API void_t
imglab_ocv_qr2code_del (qr2code_ocv_t qr2d);

CR_API void_t
imglab_ocv_qr2code_param (qr2code_ocv_t qr2d, const void_t *param_data,
                          size_t param_size);
CR_API size_t
imglab_ocv_qr2code_doit (qr2code_ocv_t qr2d, ximage_t mat, str_lstA_t *text,
                                    xpoly_lst_t *list);
/********************/
/* ZXing 图形码识别 */
/********************/

/* sZXI_ReaderOptions::formats */
#define ZXI_TYPE_NONE           0
#define ZXI_TYPE_AZTEC      (1 << 0)
#define ZXI_TYPE_CODABAR    (1 << 1)
#define ZXI_TYPE_CODE39     (1 << 2)
#define ZXI_TYPE_CODE93     (1 << 3)
#define ZXI_TYPE_CODE128    (1 << 4)
#define ZXI_TYPE_DATABAR    (1 << 5)
#define ZXI_TYPE_DATABAREX  (1 << 6)
#define ZXI_TYPE_DATAMATRIX (1 << 7)
#define ZXI_TYPE_EAN8       (1 << 8)
#define ZXI_TYPE_EAN13      (1 << 9)
#define ZXI_TYPE_ITF        (1 << 10)
#define ZXI_TYPE_MAXICODE   (1 << 11)
#define ZXI_TYPE_PDF417     (1 << 12)
#define ZXI_TYPE_QRCODE     (1 << 13)
#define ZXI_TYPE_UPCA       (1 << 14)
#define ZXI_TYPE_UPCE       (1 << 15)
#define ZXI_TYPE_MC_QRCODE  (1 << 16)
#define ZXI_TYPE_RM_QRCODE  (1 << 17)
#define ZXI_TYPE_DX_FILMEG  (1 << 18)
#define ZXI_TYPE_DATABARLM  (1 << 19)
#define ZXI_TYPE_LINEAR_CODES \
    (ZXI_TYPE_CODABAR | ZXI_TYPE_CODE39 | ZXI_TYPE_CODE93 | \
     ZXI_TYPE_CODE128 | ZXI_TYPE_EAN8 | ZXI_TYPE_EAN13 | ZXI_TYPE_ITF | \
     ZXI_TYPE_DATABAR | ZXI_TYPE_DATABAREX | ZXI_TYPE_DATABARLM | \
     ZXI_TYPE_DX_FILMEG | ZXI_TYPE_UPCA | ZXI_TYPE_UPCE)
#define ZXI_TYPE_MATRIX_CODES \
    (ZXI_TYPE_AZTEC | ZXI_TYPE_DATAMATRIX | ZXI_TYPE_MAXICODE | \
     ZXI_TYPE_PDF417 | ZXI_TYPE_QRCODE | ZXI_TYPE_MC_QRCODE | \
     ZXI_TYPE_RM_QRCODE)
#define ZXI_TYPE_ANY    (ZXI_TYPE_LINEAR_CODES | ZXI_TYPE_MATRIX_CODES)

typedef struct  /* ZXing::ReaderOptions */
{
    bool_t  tryHarder;
    bool_t  tryRotate;
    bool_t  tryInvert;
    bool_t  tryDownscale;
    bool_t  isPure;
    bool_t  tryCode39ExtendedMode;
    bool_t  returnErrors;
    sint_t  downscaleFactor;
    sint_t  eanAddOnSymbol;
    sint_t  binarizer;
    sint_t  textMode;
    cstr_t  characterSet;
    sint_t  minLineCount;
    sint_t  maxNumberOfSymbols;
    sint_t  downscaleThreshold;
    sint_t  formats;

} sZXI_ReaderOptions;

/* sZXI_ReaderOptions::eanAddOnSymbol */
#define ZXI_EAN_IGNORE  0
#define ZXI_EAN_READ    1
#define ZXI_EAN_REQUIRE 2

/* sZXI_ReaderOptions::textMode */
#define ZXI_TEXT_PLAIN      0
#define ZXI_TEXT_ECI        1
#define ZXI_TEXT_HRI        2
#define ZXI_TEXT_HEX        3
#define ZXI_TEXT_ESCAPED    4

/* sZXI_ReaderOptions::binarizer */
#define ZXI_BIN_LOCAL_AVERAGE       0
#define ZXI_BIN_GLOBAL_HISTOGRAM    1
#define ZXI_BIN_FIXED_THRESHOLD     2
#define ZXI_BIN_BOOL_CAST           3

CR_API size_t
imglab_zxi_grpcode_doit (ximage_t mat, str_lstA_t *text, xpoly_lst_t *list,
                         const sZXI_ReaderOptions *options CR_DEFAULT(NULL));
/*********************/
/* NCNN NanoDet 识别 */
/*********************/
typedef void_t*     nanodet_ncnn_t;

CR_API nanodet_ncnn_t
imglab_ncnn_nanodet_new (sint_t vk_gpu);

CR_API void_t
imglab_ncnn_nanodet_del (nanodet_ncnn_t nnet);

CR_API bool_t
imglab_ncnn_nanodet_load (nanodet_ncnn_t nnet, const ansi_t *name,
                          bool_t bin_param, bool_t use_vulkan,
                          bool_t use_bf16 CR_DEFAULT(FALSE));
typedef struct
{
        sint_t  thread_num;
        bool_t  light_mode;
        sint_t  target_size;
        fp32_t  prob_threshold;
        fp32_t  nms_threshold;
        fp32_t  mean_vals[3];
        fp32_t  norm_vals[3];
        cstr_t  input_layer;
        cstr_t  cls_pred8, cls_pred16, cls_pred32;
        cstr_t  dis_pred8, dis_pred16, dis_pred32;

} sNCNN_NanoDetParam;

typedef struct
{
        sint_t  thread_num;
        bool_t  light_mode;
        sint_t  target_size;
        sint_t  num_class;
        bool_t  have_sigmoid;
        fp32_t  prob_threshold;
        fp32_t  nms_threshold;
        fp32_t  mean_vals[3];
        fp32_t  norm_vals[3];
        cstr_t  input_layer;
        cstr_t  pred8, pred16;
        cstr_t  pred32, pred64;

} sNCNN_NanoDetPlusParam;

CR_API sRECT_OBJECT*
imglab_ncnn_nanodet_doit (nanodet_ncnn_t nnet, ximage_t mat,
                          const sNCNN_NanoDetParam *param,
                          size_t *count);
CR_API sRECT_OBJECT*
imglab_ncnn_nanodet_plus_doit (nanodet_ncnn_t nnet, ximage_t mat,
                               const sNCNN_NanoDetPlusParam *param,
                               size_t *count);
/**************************/
/* NCNN MobileNetSSD 识别 */
/**************************/
typedef void_t*     mbntssd_ncnn_t;

CR_API mbntssd_ncnn_t
imglab_ncnn_mbntssd_new (sint_t vk_gpu);

CR_API void_t
imglab_ncnn_mbntssd_del (mbntssd_ncnn_t nnet);

CR_API bool_t
imglab_ncnn_mbntssd_load (mbntssd_ncnn_t nnet, const ansi_t *name,
                    const ansi_t *silence, bool_t bin_param, bool_t use_vulkan,
                                bool_t use_bf16 CR_DEFAULT(FALSE));
typedef struct
{
        sint_t  thread_num;
        bool_t  light_mode;
        sint_t  target_size;
        sint_t  net_version;    /* 1, 2, 3 */
        fp32_t  prob_threshold;
        fp32_t  mean_vals[3];
        fp32_t  norm_vals[3];
        cstr_t  input_layer;
        cstr_t  output_layer;

} sNCNN_MobileNetSSD_Param;

CR_API sRECT_OBJECT*
imglab_ncnn_mbntssd_doit (mbntssd_ncnn_t nnet, ximage_t mat,
                          const sNCNN_MobileNetSSD_Param *param,
                          size_t *count);
/******************/
/* NCNN YOLO 识别 */
/******************/
typedef void_t*     yolo_ncnn_t;

CR_API yolo_ncnn_t
imglab_ncnn_yolo_new (sint_t vk_gpu);

CR_API void_t
imglab_ncnn_yolo_del (yolo_ncnn_t nnet);

CR_API bool_t
imglab_ncnn_yolo_load (yolo_ncnn_t nnet, const ansi_t *name,
                    const ansi_t *v5focus, bool_t bin_param, bool_t use_vulkan,
                                bool_t use_bf16 CR_DEFAULT(FALSE));
typedef struct
{
        sint_t  thread_num;
        bool_t  light_mode;
        sint_t  target_size;
        sint_t  yolo_version;   /* 2, 3, 4, 5, 500, 560, 562, 7, 700 */
        fp32_t  prob_threshold;
        fp32_t  nms_threshold;
        fp32_t  mean_vals[3];
        fp32_t  norm_vals[3];
        cstr_t  input_layer;
        cstr_t  output_layer8;
        cstr_t  output_layer16;
        cstr_t  output_layer32;

} sNCNN_YOLO_Param;

CR_API sRECT_OBJECT*
imglab_ncnn_yolo_doit (yolo_ncnn_t nnet, ximage_t mat,
                       const sNCNN_YOLO_Param *param,
                       size_t *count);
/*********************/
/* HyperLPR 车牌识别 */
/*********************/
typedef void_t*     hyperlpr1_t;    /* pr1::PipelinePR* */
typedef void_t*     hyperlpr2_t;    /* pr2::PipelinePR* */
typedef void_t*     hyperlpr3_t;    /* P_HLPR_Context */

#define HLPR1_SEGMENTATION_FREE_METHOD      0
#define HLPR1_SEGMENTATION_BASED_METHOD     1

typedef struct  /* HLPR_ContextConfiguration */
{
        sint_t  max_num;
        sint_t  threads;
        bool_t  use_half;
        fp32_t  box_conf_threshold;
        fp32_t  nms_threshold;
        fp32_t  rec_confidence_threshold;
        sint_t  det_level;

} sHLPR3_Param;

/* V1 */
CR_API hyperlpr1_t
imglab_hyperlpr1_new (const ansi_t *path);

CR_API void_t
imglab_hyperlpr1_del (hyperlpr1_t hlpr);

CR_API sRECT_OBJECT_DESC*
imglab_hyperlpr1_doit (hyperlpr1_t hlpr, ximage_t mat, uint_t method,
                       fp32_t gate, size_t *count);
/* V2 */
CR_API hyperlpr2_t
imglab_hyperlpr2_new (const ansi_t *path);

CR_API void_t
imglab_hyperlpr2_del (hyperlpr2_t hlpr);

CR_API sRECT_OBJECT_DESC*
imglab_hyperlpr2_doit (hyperlpr2_t hlpr, ximage_t mat, bool_t is_db,
                       fp32_t gate, size_t *count);
/* V3 */
CR_API hyperlpr3_t
imglab_hyperlpr3_new (const ansi_t *path, const sHLPR3_Param *param);

CR_API void_t
imglab_hyperlpr3_del (hyperlpr3_t hlpr);

CR_API sRECT_OBJECT_DESC*
imglab_hyperlpr3_doit (hyperlpr3_t hlpr, ximage_t mat, size_t *count);

#endif  /* !__AI_IMGLAB_H__ */
