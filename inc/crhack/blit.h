/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-27  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 软件 2D 头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_BLIT_H__
#define __CR_BLIT_H__

#include "gfx2.h"
#include "fileio.h"

/*****************************************************************************/
/*                                 公用部分                                  */
/*****************************************************************************/

/* 字节像素结构 */
typedef struct
{
/*000*/ byte_t  idx;    /* 颜色值 */
/*001*/ byte_t  rv1;    /* 保留值 */
/*002*/ byte_t  rv2;    /* 保留值 */
/*003*/ byte_t  lrp;    /* 透明值 */
/*004*/
} pix1_t;

/* 16位像素结构 */
typedef struct
{
/*000*/ int16u  clr;    /* 颜色值 */
/*002*/ byte_t  rv1;    /* 保留值 */
/*003*/ byte_t  lrp;    /* 透明值 */
/*004*/
} pix2_t;

/* 32位像素结构 */
typedef struct
{
/*000*/ byte_t  bbb;    /* 蓝分量 */
/*001*/ byte_t  ggg;    /* 绿分量 */
/*002*/ byte_t  rrr;    /* 红分量 */
/*003*/ byte_t  lrp;    /* 透明值 */
/*004*/
} pix4_t;

/* 统一像素结构 */
typedef union
{
        int32u  val;    /* 32位数值 */
        pix1_t  c08;    /* 字节像素 */
        pix2_t  c16;    /* 16位像素 */
        pix4_t  c32;    /* 32位像素 */

} cpix_t;

/* 像素渲染模式值 */
#define CR_BLT_MAX  7
#define CR_BLT_SET  0   /* 直接复制 */
#define CR_BLT_AND  1   /* 逻辑  与 */
#define CR_BLT_ORR  2   /* 逻辑  或 */
#define CR_BLT_NOT  3   /* 逻辑  非 */
#define CR_BLT_XOR  4   /* 逻辑异或 */
#define CR_BLT_ADD  5   /* 饱和加法 */
#define CR_BLT_SUB  6   /* 饱和减法 */
#define CR_BLT_ALP  7   /* 透明混合 */

/* 像素转换/绘制函数类型 */
typedef cpix_t  (*pixcnvt_t) (const int32u*, cl32_t);
typedef void_t  (*pixdraw_t) (const sIMAGE*, sint_t, sint_t, cpix_t);
typedef void_t  (*dotdraw_t) (const sIMAGE*, fp32_t, fp32_t, cpix_t);

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    08位色插值参数设置
=======================================
*/
cr_inline cpix_t
CR_COLOR08 (
  __CR_IN__ byte_t  c08,
  __CR_IN__ byte_t  lerp CR_DEFAULT(255)
    )
{
    cpix_t  color;

    color.c08.idx = c08;
    color.c08.rv1 = 0x00;
    color.c08.rv2 = 0x00;
    color.c08.lrp = lerp;
    return (color);
}

/*
=======================================
    12位色插值参数设置
=======================================
*/
cr_inline cpix_t
CR_COLOR12 (
  __CR_IN__ int16u  c12,
  __CR_IN__ byte_t  lerp CR_DEFAULT(255)
    )
{
    cpix_t  color;

    color.c16.clr = c12;
    color.c16.rv1 = 0x00;
    color.c16.lrp = lerp >> 4;
    return (color);
}

/*
=======================================
    15位色插值参数设置
=======================================
*/
cr_inline cpix_t
CR_COLOR15 (
  __CR_IN__ int16u  c15,
  __CR_IN__ byte_t  lerp CR_DEFAULT(255)
    )
{
    cpix_t  color;

    color.c16.clr = c15;
    color.c16.rv1 = 0x00;
    color.c16.lrp = lerp >> 3;
    return (color);
}

/*
=======================================
    16位色插值参数设置
=======================================
*/
cr_inline cpix_t
CR_COLOR16 (
  __CR_IN__ int16u  c16,
  __CR_IN__ byte_t  lerp CR_DEFAULT(255)
    )
{
    cpix_t  color;

    color.c16.clr = c16;
    color.c16.rv1 = 0x00;
    color.c16.lrp = lerp >> 2;
    return (color);
}

/*
=======================================
    24位色插值参数设置
=======================================
*/
cr_inline cpix_t
CR_COLOR24 (
  __CR_IN__ int32u  c24,
  __CR_IN__ byte_t  lerp CR_DEFAULT(255)
    )
{
    cpix_t  color;

    color.val = c24;
    color.c32.lrp = lerp;
    return (color);
}

/*
=======================================
    32位色插值参数设置
=======================================
*/
cr_inline cpix_t
CR_COLOR32 (
  __CR_IN__ int32u  c32
    )
{
    cpix_t  color;

    color.val = c32;
    return (color);
}

#endif  /* !_CR_SICK_INLINE_ */

/**************************************************/
/* +---------------------------------+            */
/* | 对于 CRHACK/BLIT 提供的绘图函数 +----------+ */
/* | cpix_t 的颜色值必须用 CR_COLOR*() 函数合成 | */
/* +--------------------------------------------+ */
/**************************************************/
#if     defined(_CR_USE_PIXEL15_)
    #define CR_COLOR(clr, lrp)  CR_COLOR15(clr, lrp)
#elif   defined(_CR_USE_PIXEL16_)
    #define CR_COLOR(clr, lrp)  CR_COLOR16(clr, lrp)
#elif   defined(_CR_USE_PIXEL24_)
    #define CR_COLOR(clr, lrp)  CR_COLOR24(clr, lrp)
#else       /* (_CR_USE_PIXEL32_) */
    #define CR_COLOR(clr, lrp)  CR_COLOR24(clr, lrp)
#endif

/* 相应屏幕格式的绘制 API 映射 */
#if     defined(_CR_USE_PIXEL15_)
    #define pixel_andz      pixel_and16z
    #define pixel_andn      pixel_and16n
    #define pixel_orrz      pixel_orr16z
    #define pixel_orrn      pixel_orr16n
    #define pixel_notz      pixel_not16z
    #define pixel_notn      pixel_not16n
    #define pixel_xorz      pixel_xor16z
    #define pixel_xorn      pixel_xor16n
    #define pixel_getz      pixel_get16z
    #define pixel_getn      pixel_get16n
    #define pixel_setz      pixel_set16z
    #define pixel_setn      pixel_set16n
    #define pixel_addz      pixel_add15z
    #define pixel_addn      pixel_add15n
    #define pixel_subz      pixel_sub15z
    #define pixel_subn      pixel_sub15n
    #define pixel_lrpz      pixel_lrp15z
    #define pixel_lrpn      pixel_lrp15n
    #define dot_wu_get      dot_wu_get15
    #define dot_wu_set      dot_wu_set15
    #define fill_and_c      fill_and16_c
    #define fill_orr_c      fill_orr16_c
    #define fill_not_c      fill_not16_c
    #define fill_xor_c      fill_xor16_c
    #define fill_set_c      fill_set16_c
    #define fill_add_c      fill_add15_c
    #define fill_sub_c      fill_sub15_c
    #define fill_lrp_c      fill_lrp15_c
    #define blit_and_c      blit_and16_c
    #define blit_orr_c      blit_orr16_c
    #define blit_xor_c      blit_xor16_c
    #define blit_set_c      blit_set16_c
    #define blit_str_c      blit_str16_c
    #define blit_dtr_c      blit_dtr16_c
    #define blit_add_c      blit_add15_c
    #define blit_sub_c      blit_sub15_c
    #define blit_lrp_c      blit_lrp15_c
    #define blit_alp_c      blit_alp15_c
    #define blit_msk_c      blit_msk16_c

#elif   defined(_CR_USE_PIXEL16_)
    #define pixel_andz      pixel_and16z
    #define pixel_andn      pixel_and16n
    #define pixel_orrz      pixel_orr16z
    #define pixel_orrn      pixel_orr16n
    #define pixel_notz      pixel_not16z
    #define pixel_notn      pixel_not16n
    #define pixel_xorz      pixel_xor16z
    #define pixel_xorn      pixel_xor16n
    #define pixel_getz      pixel_get16z
    #define pixel_getn      pixel_get16n
    #define pixel_setz      pixel_set16z
    #define pixel_setn      pixel_set16n
    #define pixel_addz      pixel_add16z
    #define pixel_addn      pixel_add16n
    #define pixel_subz      pixel_sub16z
    #define pixel_subn      pixel_sub16n
    #define pixel_lrpz      pixel_lrp16z
    #define pixel_lrpn      pixel_lrp16n
    #define dot_wu_get      dot_wu_get16
    #define dot_wu_set      dot_wu_set16
    #define fill_and_c      fill_and16_c
    #define fill_orr_c      fill_orr16_c
    #define fill_not_c      fill_not16_c
    #define fill_xor_c      fill_xor16_c
    #define fill_set_c      fill_set16_c
    #define fill_add_c      fill_add16_c
    #define fill_sub_c      fill_sub16_c
    #define fill_lrp_c      fill_lrp16_c
    #define blit_and_c      blit_and16_c
    #define blit_orr_c      blit_orr16_c
    #define blit_xor_c      blit_xor16_c
    #define blit_set_c      blit_set16_c
    #define blit_str_c      blit_str16_c
    #define blit_dtr_c      blit_dtr16_c
    #define blit_add_c      blit_add16_c
    #define blit_sub_c      blit_sub16_c
    #define blit_lrp_c      blit_lrp16_c
    #define blit_alp_c      blit_alp16_c
    #define blit_msk_c      blit_msk16_c

#elif   defined(_CR_USE_PIXEL24_)
    #define pixel_andz      pixel_and24z
    #define pixel_andn      pixel_and24n
    #define pixel_orrz      pixel_orr24z
    #define pixel_orrn      pixel_orr24n
    #define pixel_notz      pixel_not24z
    #define pixel_notn      pixel_not24n
    #define pixel_xorz      pixel_xor24z
    #define pixel_xorn      pixel_xor24n
    #define pixel_getz      pixel_get24z
    #define pixel_getn      pixel_get24n
    #define pixel_setz      pixel_set24z
    #define pixel_setn      pixel_set24n
    #define pixel_addz      pixel_add24z
    #define pixel_addn      pixel_add24n
    #define pixel_subz      pixel_sub24z
    #define pixel_subn      pixel_sub24n
    #define pixel_lrpz      pixel_lrp24z
    #define pixel_lrpn      pixel_lrp24n
    #define dot_wu_get      dot_wu_get24
    #define dot_wu_set      dot_wu_set24
    #define fill_and_c      fill_and24_c
    #define fill_orr_c      fill_orr24_c
    #define fill_not_c      fill_not24_c
    #define fill_xor_c      fill_xor24_c
    #define fill_set_c      fill_set24_c
    #define fill_add_c      fill_add24_c
    #define fill_sub_c      fill_sub24_c
    #define fill_lrp_c      fill_lrp24_c
    #define blit_and_c      blit_and24_c
    #define blit_orr_c      blit_orr24_c
    #define blit_xor_c      blit_xor24_c
    #define blit_set_c      blit_set24_c
    #define blit_str_c      blit_str24_c
    #define blit_dtr_c      blit_dtr24_c
    #define blit_add_c      blit_add24_c
    #define blit_sub_c      blit_sub24_c
    #define blit_lrp_c      blit_lrp24_c
    #define blit_alp_c      blit_alp24_c
    #define blit_msk_c      blit_msk24_c

#else       /* (_CR_USE_PIXEL32_) */
    #define pixel_andz      pixel_and32z
    #define pixel_andn      pixel_and32n
    #define pixel_orrz      pixel_orr32z
    #define pixel_orrn      pixel_orr32n
    #define pixel_notz      pixel_not32z
    #define pixel_notn      pixel_not32n
    #define pixel_xorz      pixel_xor32z
    #define pixel_xorn      pixel_xor32n
    #define pixel_getz      pixel_get32z
    #define pixel_getn      pixel_get32n
    #define pixel_setz      pixel_set32z
    #define pixel_setn      pixel_set32n
    #define pixel_addz      pixel_add32z
    #define pixel_addn      pixel_add32n
    #define pixel_subz      pixel_sub32z
    #define pixel_subn      pixel_sub32n
    #define pixel_lrpz      pixel_lrp32z
    #define pixel_lrpn      pixel_lrp32n
    #define dot_wu_get      dot_wu_get32
    #define dot_wu_set      dot_wu_set32
    #define fill_and_c      fill_and32_c
    #define fill_orr_c      fill_orr32_c
    #define fill_not_c      fill_not32_c
    #define fill_xor_c      fill_xor32_c
    #define fill_set_c      fill_set32_c
    #define fill_add_c      fill_add32_c
    #define fill_sub_c      fill_sub32_c
    #define fill_lrp_c      fill_lrp32_c
    #define blit_and_c      blit_and32_c
    #define blit_orr_c      blit_orr32_c
    #define blit_xor_c      blit_xor32_c
    #define blit_set_c      blit_set32_c
    #define blit_str_c      blit_str32_c
    #define blit_dtr_c      blit_dtr32_c
    #define blit_add_c      blit_add32_c
    #define blit_sub_c      blit_sub32_c
    #define blit_lrp_c      blit_lrp32_c
    #define blit_alp_c      blit_alp32_c
    #define blit_msk_c      blit_msk32_c

#endif  /* SCREEN TYPE predefines */

/*****************************************************************************/
/*                                 离屏表面                                  */
/*****************************************************************************/

/* 生成 MEM 图形离屏表面 */
CR_API iGFX2*   create_mem_bitmap (uint_t width, uint_t height,
                                   uint_t crh_fmt);

/*****************************************************************************/
/*                                 文字接口                                  */
/*****************************************************************************/

/* 生成通用点阵文字绘制接口 */
CR_API iFONT*   create_bit_font (iDATIN *asc, iDATIN *hzk, uint_t size,
                                 uint_t xspc, uint_t yspc, const ansi_t *type);

/* 生成 UCDOS 点阵文字绘制接口 */
CR_API iFONT*   create_ucdos_font (iDATIN *asc, iDATIN *hzk, iDATIN *chr,
                                   uint_t size, uint_t xspc, uint_t yspc);

/*****************************************************************************/
/*                                 像素操作                                  */
/*****************************************************************************/

/* 像素-读 */
CR_API byte_t   pixel_get01z (const sIMAGE *dst, sint_t x, sint_t y);
CR_API byte_t   pixel_get01n (const sIMAGE *dst, sint_t x, sint_t y);
CR_API byte_t   pixel_get02z (const sIMAGE *dst, sint_t x, sint_t y);
CR_API byte_t   pixel_get02n (const sIMAGE *dst, sint_t x, sint_t y);
CR_API byte_t   pixel_get04z (const sIMAGE *dst, sint_t x, sint_t y);
CR_API byte_t   pixel_get04n (const sIMAGE *dst, sint_t x, sint_t y);
CR_API byte_t   pixel_get08z (const sIMAGE *dst, sint_t x, sint_t y);
CR_API byte_t   pixel_get08n (const sIMAGE *dst, sint_t x, sint_t y);
CR_API int16u   pixel_get16z (const sIMAGE *dst, sint_t x, sint_t y);
CR_API int16u   pixel_get16n (const sIMAGE *dst, sint_t x, sint_t y);
CR_API int32u   pixel_get24z (const sIMAGE *dst, sint_t x, sint_t y);
CR_API int32u   pixel_get24n (const sIMAGE *dst, sint_t x, sint_t y);
CR_API int32u   pixel_get32z (const sIMAGE *dst, sint_t x, sint_t y);
CR_API int32u   pixel_get32n (const sIMAGE *dst, sint_t x, sint_t y);

/* 逻辑-与 */
CR_API void_t   pixel_and08z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_and08n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_and16z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_and16n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_and24z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_and24n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_and32z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_and32n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);

/* 逻辑-或 */
CR_API void_t   pixel_orr08z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_orr08n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_orr16z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_orr16n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_orr24z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_orr24n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_orr32z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_orr32n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);

/* 逻辑-非 */
CR_API void_t   pixel_not08z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_not08n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_not16z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_not16n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_not24z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_not24n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_not32z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_not32n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);

/* 逻辑异或 */
CR_API void_t   pixel_xor08z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_xor08n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_xor16z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_xor16n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_xor24z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_xor24n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_xor32z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_xor32n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);

/* 直接绘制 */
CR_API void_t   pixel_set01z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set01n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set02z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set02n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set04z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set04n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set08z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set08n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set16z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set16n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set24z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set24n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set32z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_set32n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);

/* 饱和加法 */
CR_API void_t   pixel_add08z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_add08n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_add12z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_add12n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_add15z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_add15n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_add16z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_add16n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_add24z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_add24n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_add32z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_add32n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);

/* 饱和减法 */
CR_API void_t   pixel_sub08z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_sub08n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_sub12z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_sub12n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_sub15z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_sub15n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_sub16z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_sub16n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_sub24z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_sub24n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_sub32z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_sub32n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);

/* 透明混合 */
CR_API void_t   pixel_lrp08z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_lrp08n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_lrp12z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_lrp12n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_lrp15z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_lrp15n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_lrp16z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_lrp16n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_lrp24z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_lrp24n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_lrp32z (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);
CR_API void_t   pixel_lrp32n (const sIMAGE *dst, sint_t x, sint_t y, cpix_t);

/* 抗锯齿读 */
CR_API byte_t   dot_wu_get08 (const sIMAGE *dst, fp32_t x, fp32_t y);
CR_API int16u   dot_wu_get12 (const sIMAGE *dst, fp32_t x, fp32_t y);
CR_API int16u   dot_wu_get15 (const sIMAGE *dst, fp32_t x, fp32_t y);
CR_API int16u   dot_wu_get16 (const sIMAGE *dst, fp32_t x, fp32_t y);
CR_API int32u   dot_wu_get24 (const sIMAGE *dst, fp32_t x, fp32_t y);
CR_API int32u   dot_wu_get32 (const sIMAGE *dst, fp32_t x, fp32_t y);

/* 抗锯齿写 */
CR_API void_t   dot_wu_set08 (const sIMAGE *dst, fp32_t x, fp32_t y, cpix_t);
CR_API void_t   dot_wu_set12 (const sIMAGE *dst, fp32_t x, fp32_t y, cpix_t);
CR_API void_t   dot_wu_set15 (const sIMAGE *dst, fp32_t x, fp32_t y, cpix_t);
CR_API void_t   dot_wu_set16 (const sIMAGE *dst, fp32_t x, fp32_t y, cpix_t);
CR_API void_t   dot_wu_set24 (const sIMAGE *dst, fp32_t x, fp32_t y, cpix_t);
CR_API void_t   dot_wu_set32 (const sIMAGE *dst, fp32_t x, fp32_t y, cpix_t);

/*****************************************************************************/
/*                                 几何绘制                                  */
/*****************************************************************************/

CR_API void_t   draw_rect (const sIMAGE *dst, const sRECT *rect,
                           cpix_t color, pixdraw_t pixel_draw);

CR_API void_t   draw_line (const sIMAGE *dst, const sLINE *pos,
                           cpix_t color, pixdraw_t pixel_draw);

CR_API void_t   draw_lineh (const sIMAGE *dst, sint_t x1, sint_t x2,
                            sint_t dy, cpix_t color, pixdraw_t pixel_draw);

CR_API void_t   draw_linev (const sIMAGE *dst, sint_t dx, sint_t y1,
                            sint_t y2, cpix_t color, pixdraw_t pixel_draw);

CR_API void_t   draw_lines (const sIMAGE *dst, const sPNT2 *pos,
                            uint_t count, leng_t skip, cpix_t color,
                            pixdraw_t pixel_draw);

CR_API void_t   draw_circle (const sIMAGE *dst, sint_t cx,
                             sint_t cy, sint_t radius, cpix_t color,
                             pixdraw_t pixel_draw);

CR_API void_t   draw_ellipse (const sIMAGE *dst, const sRECT *rect,
                              cpix_t color, pixdraw_t pixel_draw);

CR_API void_t   draw_polygon (const sIMAGE *dst, const sPNT2 *pos,
                              uint_t count, leng_t skip, cpix_t color,
                              pixdraw_t pixel_draw);

/*****************************************************************************/
/*                                 填充操作                                  */
/*****************************************************************************/

/* 逻辑-与 */
CR_API void_t   fill_and08_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_and16_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_and24_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_and32_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
/* 逻辑-或 */
CR_API void_t   fill_orr08_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_orr16_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_orr24_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_orr32_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
/* 逻辑-非 */
CR_API void_t   fill_not08_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_not16_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_not24_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_not32_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
/* 逻辑异或 */
CR_API void_t   fill_xor08_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_xor16_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_xor24_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_xor32_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
/* 直接绘制 */
CR_API void_t   fill_set08_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_set16_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_set24_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_set32_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
/* 饱和加法 */
CR_API void_t   fill_add08_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_add12_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_add15_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_add16_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_add24_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_add32_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
/* 饱和减法 */
CR_API void_t   fill_sub08_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_sub12_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_sub15_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_sub16_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_sub24_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_sub32_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
/* 透明混合 */
CR_API void_t   fill_lrp08_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_lrp12_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_lrp15_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_lrp16_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_lrp24_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);
CR_API void_t   fill_lrp32_c (const sIMAGE *dst, const sFILL *fill,
                              cpix_t color, const sRECT *rect);

/*****************************************************************************/
/*                                 BLT 操作                                  */
/*****************************************************************************/

/* 逻辑-与 */
CR_API void_t   blit_and08_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_and16_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_and24_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_and32_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
/* 逻辑-或 */
CR_API void_t   blit_orr08_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_orr16_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_orr24_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_orr32_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
/* 逻辑异或 */
CR_API void_t   blit_xor08_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_xor16_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_xor24_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_xor32_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
/* 直接绘制 */
CR_API void_t   blit_set08_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_set16_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_set24_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_set32_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
/* 源透明色 */
CR_API void_t   blit_str08_c (const sIMAGE *dst, const sIMAGE *src,
                        const sBLIT *blit, cpix_t trans, const sRECT *rect);
CR_API void_t   blit_str16_c (const sIMAGE *dst, const sIMAGE *src,
                        const sBLIT *blit, cpix_t trans, const sRECT *rect);
CR_API void_t   blit_str24_c (const sIMAGE *dst, const sIMAGE *src,
                        const sBLIT *blit, cpix_t trans, const sRECT *rect);
CR_API void_t   blit_str32_c (const sIMAGE *dst, const sIMAGE *src,
                        const sBLIT *blit, cpix_t trans, const sRECT *rect);
/* 目标关键 */
CR_API void_t   blit_dtr08_c (const sIMAGE *dst, const sIMAGE *src,
                        const sBLIT *blit, cpix_t trans, const sRECT *rect);
CR_API void_t   blit_dtr16_c (const sIMAGE *dst, const sIMAGE *src,
                        const sBLIT *blit, cpix_t trans, const sRECT *rect);
CR_API void_t   blit_dtr24_c (const sIMAGE *dst, const sIMAGE *src,
                        const sBLIT *blit, cpix_t trans, const sRECT *rect);
CR_API void_t   blit_dtr32_c (const sIMAGE *dst, const sIMAGE *src,
                        const sBLIT *blit, cpix_t trans, const sRECT *rect);
/* 饱和加法 */
CR_API void_t   blit_add08_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_add12_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_add15_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_add16_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_add24_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_add32_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
/* 饱和减法 */
CR_API void_t   blit_sub08_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_sub12_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_sub15_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_sub16_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_sub24_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
CR_API void_t   blit_sub32_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, const sRECT *rect);
/* 透明混合 */
CR_API void_t   blit_lrp08_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, cpix_t trans, bool_t strb,
                              const sRECT *rect);
CR_API void_t   blit_lrp12_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, cpix_t trans, bool_t strb,
                              const sRECT *rect);
CR_API void_t   blit_lrp15_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, cpix_t trans, bool_t strb,
                              const sRECT *rect);
CR_API void_t   blit_lrp16_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, cpix_t trans, bool_t strb,
                              const sRECT *rect);
CR_API void_t   blit_lrp24_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, cpix_t trans, bool_t strb,
                              const sRECT *rect);
CR_API void_t   blit_lrp32_c (const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit, cpix_t trans, bool_t strb,
                              const sRECT *rect);
/* 通道插值 */
CR_API void_t   blit_alp08_c (const sIMAGE *dst, const sIMAGE *src,
                              const sIMAGE *chn, const sBLIT *blit,
                              const sRECT *rect);
CR_API void_t   blit_alp12_c (const sIMAGE *dst, const sIMAGE *src,
                              const sIMAGE *chn, const sBLIT *blit,
                              const sRECT *rect);
CR_API void_t   blit_alp15_c (const sIMAGE *dst, const sIMAGE *src,
                              const sIMAGE *chn, const sBLIT *blit,
                              const sRECT *rect);
CR_API void_t   blit_alp16_c (const sIMAGE *dst, const sIMAGE *src,
                              const sIMAGE *chn, const sBLIT *blit,
                              const sRECT *rect);
CR_API void_t   blit_alp24_c (const sIMAGE *dst, const sIMAGE *src,
                              const sIMAGE *chn, const sBLIT *blit,
                              const sRECT *rect);
CR_API void_t   blit_alp32_c (const sIMAGE *dst, const sIMAGE *src,
                              const sIMAGE *chn, const sBLIT *blit,
                              const sRECT *rect);
/* 掩码绘制 */
CR_API void_t   blit_msk08_c (const sIMAGE *dst, const sIMAGE *src,
                              const sIMAGE *msk, const sBLIT *blit,
                              const sRECT *rect, byte_t index);
CR_API void_t   blit_msk16_c (const sIMAGE *dst, const sIMAGE *src,
                              const sIMAGE *msk, const sBLIT *blit,
                              const sRECT *rect, byte_t index);
CR_API void_t   blit_msk24_c (const sIMAGE *dst, const sIMAGE *src,
                              const sIMAGE *msk, const sBLIT *blit,
                              const sRECT *rect, byte_t index);
CR_API void_t   blit_msk32_c (const sIMAGE *dst, const sIMAGE *src,
                              const sIMAGE *msk, const sBLIT *blit,
                              const sRECT *rect, byte_t index);

#endif  /* !__CR_BLIT_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
