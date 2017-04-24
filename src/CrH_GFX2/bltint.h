/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-28  */
/*     #######          ###    ###      [GFX2]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 软件 2D 内部头文件 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_BLTINT_H__
#define __CR_BLTINT_H__

#include "blit.h"

/* 标准颜色转换函数 */
cpix_t  pixel_cnvt01 (const int32u *pal, cl32_t color);
cpix_t  pixel_cnvt02 (const int32u *pal, cl32_t color);
cpix_t  pixel_cnvt04 (const int32u *pal, cl32_t color);
cpix_t  pixel_cnvt08 (const int32u *pal, cl32_t color);
cpix_t  pixel_cnvt12 (const int32u *pal, cl32_t color);
cpix_t  pixel_cnvt15 (const int32u *pal, cl32_t color);
cpix_t  pixel_cnvt16 (const int32u *pal, cl32_t color);
cpix_t  pixel_cnvt17 (const int32u *pal, cl32_t color);
cpix_t  pixel_cnvt24 (const int32u *pal, cl32_t color);
cpix_t  pixel_cnvt32 (const int32u *pal, cl32_t color);

/* 根据模式查找相应的函数调用 */
pixcnvt_t   pixel_find_cnvt (uint_t fcrh);
pixdraw_t   pixel_find_draw (uint_t fcrh, bool_t flip, uint_t mode);

/* UCDOS 具体实现结构 */
typedef struct
{
        /* 通用部分 */
        iFONT   font;

        /* 个性部分 */
        iGFX2*  m_draw;     /* 绘制表面接口 */
        bool_t  m_flip;     /* 表面是否颠倒 */
        uint_t  m_fcrh;     /* 表面颜色格式 */
        uint_t  m_xspc;     /* 字符的间距离 */
        uint_t  m_yspc;     /* 字符的行距离 */
        iDATIN* m_ascf;     /* 西文点阵数据 */
        iDATIN* m_chrf;     /* 符号点阵数据 */
        iDATIN* m_hzkf;     /* 中文点阵数据 */

        /* 各种模式的回调 */
        pixcnvt_t   pixel_cnvt;
        pixdraw_t   pixel_draw;

        /* 目标表面的范围 */
        sRECT   m_rect;

        /* 可能用到的调色板 */
        int32u  m_pal[256];

} iFONT_UC;

/* 计算偏移的函数指针类型 */
typedef uint_t  (*chr2off_t) (const byte_t*, int32u*);

/* 点阵字库接口共用函数声明 */
void_t  iFONT_UC_enter (iFONT *that);
void_t  iFONT_UC_leave (iFONT *that);
void_t  iFONT_UC_release (iFONT *that);
bool_t  iFONT_UC_bind (iFONT *that, iGFX2 *gfx2);
bool_t  iFONT_UC_setMode (iFONT *that, int32u mode);
bool_t  iFONT_UC_setColor (iFONT *that, cl32_t color);
bool_t  iFONT_UC_setBkColor (iFONT *that, cl32_t color);

#endif  /* !__CR_BLTINT_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
