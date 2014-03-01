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
 *             ##       CREATE: 2013-03-26
 *              #
 ================================================
        基于 OpenCV 的 QV2D cfz 扩展插件
 ================================================
 */

#ifndef __QL_XOPENCV_H__
#define __QL_XOPENCV_H__

#include "../QstLibs/QstLibs.h"

/*****************************************************************************/
/*                                公用库部分                                 */
/*****************************************************************************/

/* 避免暴露 OpenCV 头文件 */
#define ipls_t  void_t  /* 替代 IplImage 结构 */

/* CrHack 与 OpenCV 图片结构释放 */
CR_API void_t   ilab_ipl_del (const ipls_t *ipl);
CR_API void_t   ilab_img_del (const sIMAGE *img);

/* CrHack 与 OpenCV 图片结构交换 */
CR_API bool_t   ilab_ipl2img_set (sIMAGE *img, const ipls_t *ipl);
CR_API bool_t   ilab_img2ipl_set (ipls_t *ipl, const sIMAGE *img);
CR_API sIMAGE*  ilab_ipl2img_dup (const ipls_t *ipl);
CR_API ipls_t*  ilab_img2ipl_dup (const sIMAGE *img);

/* OpenCV 摄像头类型 */
typedef void_t*     camera_t;

/* OpenCV 摄像头读取 */
CR_API camera_t ilab_camera_new (uint_t id);
CR_API void_t   ilab_camera_del (camera_t cam);
CR_API ipls_t*  ilab_camera_get (camera_t cam);

/* OpenCV 图片文件加载 */
CR_API ipls_t*  ilab_load_fileA (const ansi_t *name);
CR_API ipls_t*  ilab_load_fileW (const wide_t *name);
CR_API ipls_t*  ilab_load_mem (const void_t *data, leng_t size);

/* OpenCV 视频文件类型 */
typedef void_t*     xvideo_t;

CR_API xvideo_t ilab_video_newA (const ansi_t *name);
CR_API xvideo_t ilab_video_newW (const wide_t *name);
CR_API void_t   ilab_video_del (xvideo_t avi);
CR_API ipls_t*  ilab_video_get (xvideo_t avi);
CR_API int64u   ilab_video_count (xvideo_t avi);
CR_API void_t   ilab_video_seek (xvideo_t avi, int64u frame);

/*****************************************************************************/
/*                                格式库部分                                 */
/*****************************************************************************/

CR_API sENGINE*     engine_opencv (void_t);
CR_API sFMT_PRT*    load_ocv_avi (iDATIN *datin, const sLOADER *param);
CR_API sFMT_PRT*    load_ocv_cam (iDATIN *datin, const sLOADER *param);
CR_API sFMT_PIC*    load_ocv_pic (iDATIN *datin, const sLOADER *param);

#endif  /* !__QL_XOPENCV_H__ */
