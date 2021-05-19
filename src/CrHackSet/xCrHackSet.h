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
 *             ##       CREATE: 2013-03-22
 *              #
 ================================================
        基于 CrHack 的 QV2D cfz 扩展插件
 ================================================
 */

#ifndef __QL_XCRHACKSET_H__
#define __QL_XCRHACKSET_H__

#include "crhack.h"

/*****************************************************************************/
/*                                公用库部分                                 */
/*****************************************************************************/

/* 远程通讯端口 */
#define CRH_REMOTE_IMG_PORT     12345   /* 图片 */

/* 内存通讯文件 */
#define CRH_LCL_IMG_FILE    "share_mem_img_file"
#define CRH_RMT_IMG_DATA    "share_mem_img_data"

/* 通讯结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* 图片头结构 */
CR_TYPEDEF struct
{
/*000*/ int32u  count;      /* 图片帧数 */
/*004*/ ansi_t  info[256];  /* 信息说明 */
/*260*/
} CR_PACKED sPIC_RMT_HEAD;

/* 图片信息结构 */
CR_TYPEDEF struct
{
/*000*/ int32u  bpp;        /* 真实位数 */
/*004*/ int32u  fmt;        /* 真实格式 */
/*008*/ ansi_t  clr[103];   /* 颜色说明 */
/*111*/ byte_t  wnh[5];     /* 分量位数 */
/*116*/ int32u  img_ww;     /* 图片宽度 */
/*120*/ int32u  img_hh;     /* 图片高度 */
/*124*/ int32u  dat_sz;     /* 数据大小 */
/*128*/
} CR_PACKED sPIC_RMT_INFO;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*****************************************************************************/
/*                                格式库部分                                 */
/*****************************************************************************/

/* 引擎接口 */
CR_API sENGINE* engine_remote (void_t);

/* 图片文件格式 */
CR_API sFMTZ*   load_remote_pic (iDATIN *datin, const sLOADER *param);

#endif  /* !__QL_XCRHACKSET_H__ */
