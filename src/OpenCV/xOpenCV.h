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
/*                                 公用部分                                  */
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

#endif  /* !__QL_XOPENCV_H__ */
