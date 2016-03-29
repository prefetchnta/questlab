/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-19  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 像素操作头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_PIXELS_H__
#define __CR_PIXELS_H__

#include "gfx2.h"
#include "morder.h"

/*****************************************************************************/
/*                              单像素格式转换                               */
/*****************************************************************************/

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    8888to332
=======================================
*/
cr_inline byte_t
pixel32to332 (
  __CR_OT__ byte_t* dst,
  __CR_IN__ int32u  src
    )
{
    (src) = DWORD_LE(src);
    *dst  = (byte_t)((src >>  6) & 0x03);
    *dst |= (byte_t)((src >> 11) & 0x1C);
    *dst |= (byte_t)((src >> 16) & 0xE0);
    return (*dst);
}

/*
=======================================
    8888to565
=======================================
*/
cr_inline int16u
pixel32to565 (
  __CR_OT__ int16u* dst,
  __CR_IN__ int32u  src
    )
{
    (src) = DWORD_LE(src);
    *dst  = (int16u)((src >> 3) & 0x001F);
    *dst |= (int16u)((src >> 5) & 0x07E0);
    *dst |= (int16u)((src >> 8) & 0xF800);
    *dst  = WORD_LE(*dst);
    return (*dst);
}

/*
=======================================
    8888toX555
=======================================
*/
cr_inline int16u
pixel32toX555 (
  __CR_OT__ int16u* dst,
  __CR_IN__ int32u  src
    )
{
    (src) = DWORD_LE(src);
    *dst  = (int16u)((src >> 3) & 0x001F);
    *dst |= (int16u)((src >> 6) & 0x03E0);
    *dst |= (int16u)((src >> 9) & 0x7C00);
    *dst  = WORD_LE(*dst);
    return (*dst);
}

/*
=======================================
    8888to1555
=======================================
*/
cr_inline int16u
pixel32to1555 (
  __CR_OT__ int16u* dst,
  __CR_IN__ int32u  src
    )
{
    (src) = DWORD_LE(src);
    *dst  = (int16u)((src >>  3) & 0x001F);
    *dst |= (int16u)((src >>  6) & 0x03E0);
    *dst |= (int16u)((src >>  9) & 0x7C00);
    *dst |= (int16u)((src >> 16) & 0x8000);
    *dst  = WORD_LE(*dst);
    return (*dst);
}

/*
=======================================
    8888to4444
=======================================
*/
cr_inline int16u
pixel32to4444 (
  __CR_OT__ int16u* dst,
  __CR_IN__ int32u  src
    )
{
    (src) = DWORD_LE(src);
    *dst  = (int16u)((src >>  4) & 0x000F);
    *dst |= (int16u)((src >>  8) & 0x00F0);
    *dst |= (int16u)((src >> 12) & 0x0F00);
    *dst |= (int16u)((src >> 16) & 0xF000);
    *dst  = WORD_LE(*dst);
    return (*dst);
}

#endif  /* !_CR_SICK_INLINE_ */

/* 避免查表数据多次重复 */
#if defined(_CR_USE_PIXELCVT_)

/* (0, 1) - (0, 255) */
static const byte_t _rom_ g_by02to256[2] =
{
    0x00, 0xFF,
};

/* (0, 3) - (0, 255) */
static const byte_t _rom_ g_by04to256[4] =
{
    0x00, 0x55, 0xAA, 0xFF,
};

/* (0, 7) - (0, 255) */
static const byte_t _rom_ g_by08to256[8] =
{
    0x00, 0x24, 0x49, 0x6D, 0x92, 0xB6, 0xDB, 0xFF,
};

/* (0, 15) - (0, 255) */
static const byte_t _rom_ g_by16to256[16] =
{
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
};

/* (0, 31) - (0, 255) */
static const byte_t _rom_ g_by32to256[32] =
{
    0x00, 0x08, 0x10, 0x19, 0x21, 0x29, 0x31, 0x3A,
    0x42, 0x4A, 0x52, 0x5A, 0x63, 0x6B, 0x73, 0x7B,
    0x84, 0x8C, 0x94, 0x9C, 0xA5, 0xAD, 0xB5, 0xBD,
    0xC5, 0xCE, 0xD6, 0xDE, 0xE6, 0xEF, 0xF7, 0xFF,
};

/* (0, 63) - (0, 255) */
static const byte_t _rom_ g_by64to256[64] =
{
    0x00, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C,
    0x20, 0x24, 0x28, 0x2D, 0x31, 0x35, 0x39, 0x3D,
    0x41, 0x45, 0x49, 0x4D, 0x51, 0x55, 0x59, 0x5D,
    0x61, 0x65, 0x69, 0x6D, 0x71, 0x75, 0x79, 0x7D,
    0x82, 0x86, 0x8A, 0x8E, 0x92, 0x96, 0x9A, 0x9E,
    0xA2, 0xA6, 0xAA, 0xAE, 0xB2, 0xB6, 0xBA, 0xBE,
    0xC2, 0xC6, 0xCA, 0xCE, 0xD2, 0xD7, 0xDB, 0xDF,
    0xE3, 0xE7, 0xEB, 0xEF, 0xF3, 0xF7, 0xFB, 0xFF,
};

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    332to8888
=======================================
*/
cr_inline int32u
pixel332to32 (
  __CR_OT__ int32u* dst,
  __CR_IN__ byte_t  src
    )
{
    /* Alpha 通道无改变, 自行决定 */
    ((uchar*)dst)[0] = g_by04to256[(src >> 0) & 0x03];
    ((uchar*)dst)[1] = g_by08to256[(src >> 2) & 0x07];
    ((uchar*)dst)[2] = g_by08to256[(src >> 5) & 0x07];
    return (*dst);
}

/*
=======================================
    565to8888
=======================================
*/
cr_inline int32u
pixel565to32 (
  __CR_OT__ int32u* dst,
  __CR_IN__ int16u  src
    )
{
    (src) = WORD_LE(src);
    ((uchar*)dst)[0] = g_by32to256[(src >>  0) & 0x1F];
    ((uchar*)dst)[1] = g_by64to256[(src >>  5) & 0x3F];
    ((uchar*)dst)[2] = g_by32to256[(src >> 11) & 0x1F];
    ((uchar*)dst)[3] = 0xFF;
    return (*dst);
}

/*
=======================================
    X555to8888
=======================================
*/
cr_inline int32u
pixelX555to32 (
  __CR_OT__ int32u* dst,
  __CR_IN__ int16u  src
    )
{
    (src) = WORD_LE(src);
    ((uchar*)dst)[0] = g_by32to256[(src >>  0) & 0x1F];
    ((uchar*)dst)[1] = g_by32to256[(src >>  5) & 0x1F];
    ((uchar*)dst)[2] = g_by32to256[(src >> 10) & 0x1F];
    ((uchar*)dst)[3] = 0xFF;
    return (*dst);
}

/*
=======================================
    1555to8888
=======================================
*/
cr_inline int32u
pixel1555to32 (
  __CR_OT__ int32u* dst,
  __CR_IN__ int16u  src
    )
{
    (src) = WORD_LE(src);
    ((uchar*)dst)[0] = g_by32to256[(src >>  0) & 0x1F];
    ((uchar*)dst)[1] = g_by32to256[(src >>  5) & 0x1F];
    ((uchar*)dst)[2] = g_by32to256[(src >> 10) & 0x1F];
    ((uchar*)dst)[3] = g_by02to256[(src >> 15) & 0x01];
    return (*dst);
}

/*
=======================================
    4444to8888
=======================================
*/
cr_inline int32u
pixel4444to32 (
  __CR_OT__ int32u* dst,
  __CR_IN__ int16u  src
    )
{
    (src) = WORD_LE(src);
    ((uchar*)dst)[0] = g_by16to256[(src >>  0) & 0x0F];
    ((uchar*)dst)[1] = g_by16to256[(src >>  4) & 0x0F];
    ((uchar*)dst)[2] = g_by16to256[(src >>  8) & 0x0F];
    ((uchar*)dst)[3] = g_by16to256[(src >> 12) & 0x0F];
    return (*dst);
}

#endif  /* !_CR_SICK_INLINE_ */

#endif  /* _CR_USE_PIXELCVT_ */

/*****************************************************************************/
/*                              调色板格式转换                               */
/*****************************************************************************/

CR_API void_t   pal_3b_to_4b (void_t *pal4, const void_t *pal3,
                              uint_t len CR_DEFAULT(256));
CR_API void_t   pal_4b_to_3b (void_t *pal3, const void_t *pal4,
                              uint_t len CR_DEFAULT(256));

CR_API void_t   pal_3b_to_4b_sw (void_t *pal4, const void_t *pal3,
                                 uint_t len CR_DEFAULT(256));
CR_API void_t   pal_4b_to_3b_sw (void_t *pal3, const void_t *pal4,
                                 uint_t len CR_DEFAULT(256));

CR_API void_t   pal_4b_to_2b_16 (void_t *pal2, const void_t *pal4,
                                 uint_t len CR_DEFAULT(256));
CR_API void_t   pal_4b_to_2b_15 (void_t *pal2, const void_t *pal4,
                                 uint_t len CR_DEFAULT(256));
CR_API void_t   pal_4b_to_2b_17 (void_t *pal2, const void_t *pal4,
                                 uint_t len CR_DEFAULT(256));
CR_API void_t   pal_4b_to_2b_12 (void_t *pal2, const void_t *pal4,
                                 uint_t len CR_DEFAULT(256));

CR_API void_t   pal_4b_alp_sw (void_t *pal4, bool_t swap,
                               uint_t alp CR_DEFAULT(0xFF),
                               uint_t len CR_DEFAULT(256));

CR_API void_t   pal_vga_to_3b (void_t *vga3, uint_t len CR_DEFAULT(256));
CR_API void_t   pal_3b_to_vga (void_t *pal3, uint_t len CR_DEFAULT(256));
CR_API void_t   pal_set_gray8 (void_t *pal4, uint_t len CR_DEFAULT(256));

/*****************************************************************************/
/*                              单像素计算操作                               */
/*****************************************************************************/

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    交换 RB 颜色通道
=======================================
*/
cr_inline int32u
swap_rb32 (
  __CR_IO__ void_t* color
    )
{
    byte_t  tmp;

    CR_SWAP(((byte_t*)color)[0],
            ((byte_t*)color)[2], tmp);
    return (*(int32u*)color);
}

/*
=======================================
    32位色转 COLORREF 格式
=======================================
*/
cr_inline int32u
argb32_to_gdi (
  __CR_IO__ void_t* color
    )
{
    *(int32u*)color &= CDWORD_LE(0x00FFFFFFUL);
    return ((int32u)swap_rb32(color));
}

#endif  /* !_CR_SICK_INLINE_ */

/* 复杂的像素计算 */
CR_API uint_t   rgb2light (uint_t r, uint_t g, uint_t b);

/* 24/32 位色
   无所谓常数不常数 */
#define CR_CRGBA24  CR_VRGBA24
#define CR_CRGBA32  CR_VRGBA32

/* 颜色格式映射 */
#if     defined(_CR_USE_PIXEL15_)
    #define CR_VRGBA(r, g, b, a)    CR_VRGBA15(r, g, b)
    #define CR_VBGRA(b, g, r, a)    CR_VRGBA15(r, g, b)
    #define CR_CRGBA(r, g, b, a)    CR_CRGBA15(r, g, b)
    #define CR_CBGRA(b, g, r, a)    CR_CRGBA15(r, g, b)
#elif   defined(_CR_USE_PIXEL16_)
    #define CR_VRGBA(r, g, b, a)    CR_VRGBA16(r, g, b)
    #define CR_VBGRA(b, g, r, a)    CR_VRGBA16(r, g, b)
    #define CR_CRGBA(r, g, b, a)    CR_CRGBA16(r, g, b)
    #define CR_CBGRA(b, g, r, a)    CR_CRGBA16(r, g, b)
#elif   defined(_CR_USE_PIXEL24_)
    #define CR_VRGBA(r, g, b, a)    CR_VRGBA24(r, g, b)
    #define CR_VBGRA(b, g, r, a)    CR_VRGBA24(r, g, b)
    #define CR_CRGBA(r, g, b, a)    CR_CRGBA24(r, g, b)
    #define CR_CBGRA(b, g, r, a)    CR_CRGBA24(r, g, b)
#else       /* (_CR_USE_PIXEL32_) */
    #define CR_VRGBA(r, g, b, a)    CR_VRGBA32(r, g, b, a)
    #define CR_VBGRA(b, g, r, a)    CR_VRGBA32(r, g, b, a)
    #define CR_CRGBA(r, g, b, a)    CR_CRGBA32(r, g, b, a)
    #define CR_CBGRA(b, g, r, a)    CR_CRGBA32(r, g, b, a)
#endif

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    12位色像素的合成
=======================================
*/
cr_inline int16u
CR_VRGBA12 (
  __CR_IN__ byte_t  r,
  __CR_IN__ byte_t  g,
  __CR_IN__ byte_t  b,
  __CR_IN__ byte_t  a CR_DEFAULT(255)
    )
{
    return (WORD_LE((int16u)((((int16u)a << 8) & 0xF000) |
                             (((int16u)r << 4) & 0x0F00) |
                             (((int16u)g << 0) & 0x00F0) | (b >> 4))));
}

/*
=======================================
    15位色像素的合成
=======================================
*/
cr_inline int16u
CR_VRGBA15 (
  __CR_IN__ byte_t  r,
  __CR_IN__ byte_t  g,
  __CR_IN__ byte_t  b
    )
{
    return (WORD_LE((int16u)((((int16u)r << 7) & 0x7C00) |
                             (((int16u)g << 2) & 0x03E0) | (b >> 3))));
}

/*
=======================================
    16位色像素的合成
=======================================
*/
cr_inline int16u
CR_VRGBA16 (
  __CR_IN__ byte_t  r,
  __CR_IN__ byte_t  g,
  __CR_IN__ byte_t  b
    )
{
    return (WORD_LE((int16u)((((int16u)r << 8) & 0xF800) |
                             (((int16u)g << 3) & 0x07E0) | (b >> 3))));
}

/*
=======================================
    15位色像素的合成
=======================================
*/
cr_inline int16u
CR_VRGBA17 (
  __CR_IN__ byte_t  r,
  __CR_IN__ byte_t  g,
  __CR_IN__ byte_t  b,
  __CR_IN__ byte_t  a CR_DEFAULT(255)
    )
{
    return (WORD_LE((int16u)((((int16u)a << 8) & 0x8000) |
                             (((int16u)r << 7) & 0x7C00) |
                             (((int16u)g << 2) & 0x03E0) | (b >> 3))));
}

/*
=======================================
    24位色像素的合成
=======================================
*/
cr_inline int32u
CR_VRGBA24 (
  __CR_IN__ byte_t  r,
  __CR_IN__ byte_t  g,
  __CR_IN__ byte_t  b
    )
{
    int32u  color;
    byte_t* clr = (byte_t*)(&color);

    clr[0] = b;
    clr[1] = g;
    clr[2] = r;
    clr[3] = 0xFF;
    return (color);
}

/*
=======================================
    32位色像素的合成
=======================================
*/
cr_inline int32u
CR_VRGBA32 (
  __CR_IN__ byte_t  r,
  __CR_IN__ byte_t  g,
  __CR_IN__ byte_t  b,
  __CR_IN__ byte_t  a CR_DEFAULT(255)
    )
{
    int32u  color;
    byte_t* clr = (byte_t*)(&color);

    clr[0] = b;
    clr[1] = g;
    clr[2] = r;
    clr[3] = a;
    return (color);
}

/*
=======================================
    12位色像素的合成 (常数用)
=======================================
*/
cr_inline int16u
CR_CRGBA12 (
  __CR_IN__ byte_t  r,
  __CR_IN__ byte_t  g,
  __CR_IN__ byte_t  b,
  __CR_IN__ byte_t  a CR_DEFAULT(255)
    )
{
    return (CWORD_LE((int16u)((((int16u)a << 8) & 0xF000) |
                              (((int16u)r << 4) & 0x0F00) |
                              (((int16u)g << 0) & 0x00F0) | (b >> 4))));
}

/*
=======================================
    15位色像素的合成 (常数用)
=======================================
*/
cr_inline int16u
CR_CRGBA15 (
  __CR_IN__ byte_t  r,
  __CR_IN__ byte_t  g,
  __CR_IN__ byte_t  b
    )
{
    return (CWORD_LE((int16u)((((int16u)r << 7) & 0x7C00) |
                              (((int16u)g << 2) & 0x03E0) | (b >> 3))));
}

/*
=======================================
    16位色像素的合成 (常数用)
=======================================
*/
cr_inline int16u
CR_CRGBA16 (
  __CR_IN__ byte_t  r,
  __CR_IN__ byte_t  g,
  __CR_IN__ byte_t  b
    )
{
    return (CWORD_LE((int16u)((((int16u)r << 8) & 0xF800) |
                              (((int16u)g << 3) & 0x07E0) | (b >> 3))));
}

/*
=======================================
    15位色像素的合成 (常数用)
=======================================
*/
cr_inline int16u
CR_CRGBA17 (
  __CR_IN__ byte_t  r,
  __CR_IN__ byte_t  g,
  __CR_IN__ byte_t  b,
  __CR_IN__ byte_t  a CR_DEFAULT(255)
    )
{
    return (CWORD_LE((int16u)((((int16u)a << 8) & 0x8000) |
                              (((int16u)r << 7) & 0x7C00) |
                              (((int16u)g << 2) & 0x03E0) | (b >> 3))));
}

#endif  /* !_CR_SICK_INLINE_ */

/*****************************************************************************/
/*                              像素块格式转换                               */
/*****************************************************************************/

CR_API sIMAGE*  img_32_to_565  (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_32_to_888  (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_32_to_X555 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_32_to_1555 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_32_to_4444 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);

CR_API sIMAGE*  img_565_to_32  (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_888_to_32  (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_idx1_to_32 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_idx4_to_32 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_idx8_to_32 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_X555_to_32 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_1555_to_32 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_4444_to_32 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_dxtc_to_32 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
CR_API sIMAGE*  img_auto_to_32 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const sIMAGE *src);
/* 从 D3DFMT 转出 */
CR_API sIMAGE*  img_d3dx_to_32 (sIMAGE *dst, uint_t dx, uint_t dy,
                                const void_t *pixel, leng_t size,
                                uint_t width, uint_t height,
                                const void_t *palette, int32u d3dfmt,
                                uint_t align CR_DEFAULT(4));
/* 转换到任意格式 */
CR_API sIMAGE*  img_auto_to_xx (sIMAGE *dst, const sIMAGE *src);

/*****************************************************************************/
/*                              常用的颜色常数                               */
/*****************************************************************************/

/* 标准颜色常数 */
#define clrAqua         CR_CRGBA(0x00, 0xFF, 0xFF, 255) /* 水绿色 */
#define clrBlack        CR_CRGBA(0x00, 0x00, 0x00, 255) /* 黑  色 */
#define clrBlue         CR_CRGBA(0x00, 0x00, 0xFF, 255) /* 蓝  色 */
#define clrFuchsia      CR_CRGBA(0xFF, 0x00, 0xFF, 255) /* 紫红色 */
#define clrGray         CR_CRGBA(0x80, 0x80, 0x80, 255) /* 灰  色 */
#define clrGreen        CR_CRGBA(0x00, 0x80, 0x00, 255) /* 绿  色 */
#define clrLime         CR_CRGBA(0x00, 0xFF, 0x00, 255) /* 酸橙绿 */
#define clrMaroon       CR_CRGBA(0x80, 0x00, 0x00, 255) /* 褐红色 */
#define clrNavy         CR_CRGBA(0x00, 0x00, 0x80, 255) /* 海军蓝 */
#define clrOlive        CR_CRGBA(0x00, 0x80, 0x00, 255) /* 橄榄绿 */
#define clrOrange       CR_CRGBA(0xFF, 0xA5, 0x00, 255) /* 橙  色 */
#define clrPurple       CR_CRGBA(0x80, 0x00, 0x80, 255) /* 紫  色 */
#define clrRed          CR_CRGBA(0xFF, 0x00, 0x00, 255) /* 红  色 */
#define clrSilver       CR_CRGBA(0xC0, 0xC0, 0xC0, 255) /* 银  色 */
#define clrTeal         CR_CRGBA(0x00, 0x80, 0x80, 255) /* 蓝绿色 */
#define clrWhite        CR_CRGBA(0xFF, 0xFF, 0xFF, 255) /* 白  色 */
#define clrYellow       CR_CRGBA(0xFF, 0xFF, 0x00, 255) /* 黄  色 */

/* WEB 颜色常数 */
#define webAqua         CR_CRGBA(0x7F, 0xDB, 0xFF, 255) /* 水绿色 */
#define webBlack        CR_CRGBA(0x11, 0x11, 0x11, 255) /* 黑  色 */
#define webBlue         CR_CRGBA(0x00, 0x74, 0xD9, 255) /* 蓝  色 */
#define webFuchsia      CR_CRGBA(0xF0, 0x12, 0xBE, 255) /* 紫红色 */
#define webGray         CR_CRGBA(0xAA, 0xAA, 0xAA, 255) /* 灰  色 */
#define webGreen        CR_CRGBA(0x2E, 0xCC, 0x40, 255) /* 绿  色 */
#define webLime         CR_CRGBA(0x01, 0xFF, 0x70, 255) /* 酸橙绿 */
#define webMaroon       CR_CRGBA(0x85, 0x14, 0x4B, 255) /* 褐红色 */
#define webNavy         CR_CRGBA(0x00, 0x1F, 0x3F, 255) /* 海军蓝 */
#define webOlive        CR_CRGBA(0x3D, 0x99, 0x70, 255) /* 橄榄绿 */
#define webOrange       CR_CRGBA(0xFF, 0x85, 0x1B, 255) /* 橙  色 */
#define webPurple       CR_CRGBA(0xB1, 0x0D, 0xC9, 255) /* 紫  色 */
#define webRed          CR_CRGBA(0xFF, 0x41, 0x36, 255) /* 红  色 */
#define webSilver       CR_CRGBA(0xDD, 0xDD, 0xDD, 255) /* 银  色 */
#define webTeal         CR_CRGBA(0x39, 0xCC, 0xCC, 255) /* 蓝绿色 */
#define webWhite        CR_CRGBA(0xFF, 0xFF, 0xFF, 255) /* 白  色 */
#define webYellow       CR_CRGBA(0xFF, 0xDC, 0x00, 255) /* 黄  色 */

/* VCL 颜色常数 */
#define vclAqua         CR_CRGBA(0x00, 0xFF, 0xFF, 255) /* 水绿色 */
#define vclBlack        CR_CRGBA(0x00, 0x00, 0x00, 255) /* 黑  色 */
#define vclBlue         CR_CRGBA(0x00, 0x00, 0xFF, 255) /* 蓝  色 */
#define vclCream        CR_CRGBA(0xFF, 0xFB, 0xF0, 255) /* 奶油色 */
#define vclDkGray       CR_CRGBA(0x80, 0x80, 0x80, 255) /* 深灰色 */
#define vclFuchsia      CR_CRGBA(0xFF, 0x00, 0xFF, 255) /* 紫红色 */
#define vclGray         CR_CRGBA(0x80, 0x80, 0x80, 255) /* 灰  色 */
#define vclGreen        CR_CRGBA(0x00, 0x80, 0x00, 255) /* 绿  色 */
#define vclLime         CR_CRGBA(0x00, 0xFF, 0x00, 255) /* 酸橙绿 */
#define vclLtGray       CR_CRGBA(0xC0, 0xC0, 0xC0, 255) /* 浅灰色 */
#define vclMaroon       CR_CRGBA(0x80, 0x00, 0x00, 255) /* 褐红色 */
#define vclMedGray      CR_CRGBA(0xA0, 0xA0, 0xA4, 255) /* 中灰色 */
#define vclMoneyGreen   CR_CRGBA(0xC0, 0xDC, 0xC0, 255) /* 薄荷绿 */
#define vclNavy         CR_CRGBA(0x00, 0x00, 0x80, 255) /* 海军蓝 */
#define vclOlive        CR_CRGBA(0x80, 0x80, 0x00, 255) /* 橄榄绿 */
#define vclPurple       CR_CRGBA(0x80, 0x00, 0x80, 255) /* 紫  色 */
#define vclRed          CR_CRGBA(0xFF, 0x00, 0x00, 255) /* 红  色 */
#define vclSilver       CR_CRGBA(0xC0, 0xC0, 0xC0, 255) /* 银  色 */
#define vclSkyBlue      CR_CRGBA(0xA6, 0xCA, 0xF0, 255) /* 天蓝色 */
#define vclTeal         CR_CRGBA(0x00, 0x80, 0x80, 255) /* 蓝绿色 */
#define vclWhite        CR_CRGBA(0xFF, 0xFF, 0xFF, 255) /* 白  色 */
#define vclYellow       CR_CRGBA(0xFF, 0xFF, 0x00, 255) /* 黄  色 */

#endif  /* !__CR_PIXELS_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
