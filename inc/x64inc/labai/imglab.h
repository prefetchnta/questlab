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

/* OpenCV 摄像头类型 */
typedef void_t*     camera_t;   /* cv::VideoCapture* */

CR_API camera_t imglab_camera_new (uint_t id);
CR_API camera_t imglab_camera_new2 (const ansi_t *name);
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
/*                                 图片滤波                                  */
/*****************************************************************************/

/* OpenCV 均值滤波 */
CR_API void_t   imglab_blur_box (ximage_t mat, uint_t ksize_x,
                                 uint_t ksize_y);
/* OpenCV 高斯滤波 */
CR_API void_t   imglab_blur_gauss (ximage_t mat, uint_t ksize_x,
                                   uint_t ksize_y, fp64_t sigma_x,
                                   fp64_t sigma_y);
/* OpenCV 中值滤波 */
CR_API void_t   imglab_blur_median (ximage_t mat, uint_t ksize);

/*****************************************************************************/
/*                                 图片分类                                  */
/*****************************************************************************/

/* OpenCV 分类结构类型 */
typedef void_t*     cascade_ocv_t;      /* cv::CascadeClassifier* */

/* OpenCV 级联分类器 */
CR_API cascade_ocv_t    imglab_ocv_cascade_new (const ansi_t *file);
CR_API void_t           imglab_ocv_cascade_del (cascade_ocv_t clss);
CR_API bool_t           imglab_ocv_cascade_load (cascade_ocv_t clss,
                                                 const ansi_t *file);
#define CSCD_OCV_DO_CANNY_PRUNING       1
#define CSCD_OCV_SCALE_IMAGE            2
#define CSCD_OCV_FIND_BIGGEST_OBJECT    4
#define CSCD_OCV_DO_ROUGH_SEARCH        8
CR_API xrect_lst_t      imglab_ocv_cascade_doit (cascade_ocv_t clss,
                            ximage_t mat, fp32_t fscale, sint_t min_nghbrs,
                            sint_t flags, uint_t min_width, uint_t min_height,
                            uint_t max_width, uint_t max_height);

#endif  /* !__AI_IMGLAB_H__ */