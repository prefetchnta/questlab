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

/* 文字输出结果 */
typedef struct
{
        sRECT   pos;    /* 输出位置 */
        uint_t  idx;    /* 画面索引 */
        uint_t  aln;    /* 对齐参数 */
        uint_t  pge;    /* 文本编码 */
        cl32_t  fgc;    /* 前景颜色 */
        cl32_t  bkc;    /* 背景颜色 */
        bool_t  trn;    /* 透明输出 */
        bool_t  kll;    /* 是否释放 */
        void_t* txt;    /* 输出文本 */

} sILAB_TXTOUT;

/* 输出结果结构 */
typedef struct
{
        uint_t          n_img;  /* 图片数量 */
        sIMAGE**        frame;  /* 图片列表 */
        uint_t          n_txt;  /* 文本数量 */
        sILAB_TXTOUT*   texts;  /* 文本列表 */

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
