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
#include "../QstLibs/QstLibs.h"

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

} sILAB_INPUT;

/* 执行源的几个接口类型 */
typedef bool_t  (*ilab_init_t) (uint_t, ansi_t**);
typedef bool_t  (*ilab_main_t) (sILAB_OUTPUT*, const sILAB_INPUT*);
typedef void_t  (*ilab_kill_t) (void_t);

#endif  /* !__QL_XIMGLAB_H__ */
