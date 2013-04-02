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
 *             ##       CREATE: 2013-04-02
 *              #
 ================================================
        QuestLAB 图像实验室
 ================================================
 */

#ifndef __QL_XIMGLAB_H__
#define __QL_XIMGLAB_H__

#include "gfx2/gdiwin.h"
#include "../OpenCV/xOpenCV.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "xIMGLAB"
    #define WIN_TITLE   "xIMGLAB"
    #define WIN_CLASS   "xIMGLABCLSS"
    #define WIN_ICONF   "xIMGLAB.ini"
    #define WIN_XCONF   "xIMGLAB.xml"
#endif

/* 输出结果结构 */
typedef struct
{
        uint_t      count;  /* 帧数量 */
        sIMAGE**    frame;  /* 帧列表 */

} sILAB_OUTPUT;

/* 输入参数结构 */
typedef struct
{
        sIMAGE*     input;      /* 输入图片 */
        xmlcaller_t filter;     /* 滤镜接口 */

        /* OpenCV 转换工具函数 */
        bool_t   (*ilab_ipl2img_set) (sIMAGE*, const ipls_t*);
        bool_t   (*ilab_img2ipl_set) (ipls_t*, const sIMAGE*);
        sIMAGE*  (*ilab_ipl2img_dup) (const ipls_t*);
        ipls_t*  (*ilab_img2ipl_dup) (const sIMAGE*);

} sILAB_INPUT;

#endif  /* !__QL_XIMGLAB_H__ */
