/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-07-08  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 颜色混合计算头文件 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_BLENDS_H__
#define __CR_BLENDS_H__

#include "morder.h"

/*****************************************************************************/
/*                                08位色混合                                 */
/*****************************************************************************/

/* 饱和加法 */
#define CR_BLEND_ADD_INIT08
#define CR_BLEND_ADD_MAIN08(_dst, _src) \
    { \
        if ((_dst) >= 255 - (_src)) \
            (_dst) = 0xFF; \
        else \
            (_dst) = (_dst) + (_src); \
    }

/* 饱和减法 */
#define CR_BLEND_SUB_INIT08
#define CR_BLEND_SUB_MAIN08(_dst, _src) \
    { \
        if ((_dst) <= (_src)) \
            (_dst) = 0x00; \
        else \
            (_dst) = (_dst) - (_src); \
    }

/* 插值混合 */
#define CR_BLEND_LRP_INIT08 sint_t alp; \
                            sint_t dlt;
#define CR_BLEND_LRP_LOAD08(_lrp) \
    { \
        alp = (sint_t)(_lrp); \
        alp = alp + (alp >> 7); \
    }
#define CR_BLEND_LRP_MAIN08(_dst, _src) \
    { \
        dlt = (sint_t)(_src) - (sint_t)(_dst); \
        dlt = dlt * alp / 256; \
        (_dst) = (byte_t)((sint_t)(_dst) + dlt); \
    }

/*****************************************************************************/
/*                                12位色混合                                 */
/*****************************************************************************/

/* 饱和加法 */
#define CR_BLEND_ADD_INIT12 int16u dst_half1, dst_half2; \
                            int16u src_half1, src_half2;
#define CR_BLEND_ADD_MAIN12(_dst, _src) \
    { \
        dst_half1 = ((_dst) & 0x0F0F); \
        dst_half2 = ((_dst) & 0xF0F0) >> 4; \
        src_half1 = ((_src) & 0x0F0F); \
        src_half2 = ((_src) & 0xF0F0) >> 4; \
        dst_half1 = dst_half1 + src_half1; \
        dst_half2 = dst_half2 + src_half2; \
        if (dst_half1 & 0x0010) dst_half1 |= 0x000F; \
        if (dst_half1 & 0x1000) dst_half1 |= 0x0F00; \
        if (dst_half2 & 0x0010) dst_half2 |= 0x000F; \
        if (dst_half2 & 0x1000) dst_half2 |= 0x0F00; \
        dst_half1 &= 0x0F0F; \
        dst_half2 &= 0x0F0F; \
        (_dst) = (dst_half2 << 4) | dst_half1; \
    }

/* 饱和减法 */
#define CR_BLEND_SUB_INIT12 int16u dst_half1, dst_half2; \
                            int16u src_half1, src_half2;
#define CR_BLEND_SUB_MAIN12(_dst, _src) \
    { \
        dst_half1 = ((_dst) & 0x0F0F); \
        dst_half2 = ((_dst) & 0xF0F0) >> 4; \
        src_half1 = ((_src) & 0x0F0F); \
        src_half2 = ((_src) & 0xF0F0) >> 4; \
        dst_half1 = (dst_half1 | 0x1010) - src_half1; \
        dst_half2 = (dst_half2 | 0x1010) - src_half2; \
        if (!(dst_half1 & 0x0010)) dst_half1 &= 0xFFF0; \
        if (!(dst_half1 & 0x1000)) dst_half1 &= 0xF0FF; \
        if (!(dst_half2 & 0x0010)) dst_half2 &= 0xFFF0; \
        if (!(dst_half2 & 0x1000)) dst_half2 &= 0xF0FF; \
        dst_half1 &= 0x0F0F; \
        dst_half2 &= 0x0F0F; \
        (_dst) = (dst_half2 << 4) | dst_half1; \
    }

/* 插值混合 */
#define CR_BLEND_LRP_INIT12 int16u dst_c, src_c; \
                            sint_t dst_b, dst_g, dst_r; \
                            sint_t src_b, src_g, src_r, alp;
#define CR_BLEND_LRP_LOAD12(_lrp) \
    { \
        alp = (sint_t)(_lrp); \
        alp = alp + (alp >> 3); \
    }
#define CR_BLEND_LRP_MAIN12(_dst, _src) \
    { \
        dst_c = WORD_LE(_dst); \
        dst_b = (dst_c >> 0) & 0x000F; \
        dst_g = (dst_c >> 4) & 0x000F; \
        dst_r = (dst_c >> 8) & 0x000F; \
        src_c = WORD_LE(_src); \
        src_b = (src_c >> 0) & 0x000F; \
        src_g = (src_c >> 4) & 0x000F; \
        src_r = (src_c >> 8) & 0x000F; \
        dst_b += (src_b - dst_b) * alp / 16; \
        dst_g += (src_g - dst_g) * alp / 16; \
        dst_r += (src_r - dst_r) * alp / 16; \
        (_dst) = WORD_LE((int16u)((dst_g << 4) | \
            (dst_r << 8) | (dst_b) | (dst_c & 0xF000))); \
    }

/*****************************************************************************/
/*                                15位色混合                                 */
/*****************************************************************************/

/* 饱和加法 */
#define CR_BLEND_ADD_INIT15 int16u dst_color, src_color; \
                            int16u dst_half1, dst_half2; \
                            int16u src_half1, src_half2;
#define CR_BLEND_ADD_MAIN15(_dst, _src) \
    { \
        dst_color = WORD_LE(_dst); \
        dst_half1 = (dst_color & 0x7C1F); \
        dst_half2 = (dst_color & 0x83E0) >> 5; \
        src_color = WORD_LE(_src); \
        src_half1 = (src_color & 0x7C1F); \
        src_half2 = (src_color & 0x83E0) >> 5; \
        dst_half1 = dst_half1 + src_half1; \
        dst_half2 = dst_half2 + src_half2; \
        if (dst_half1 & 0x0020) dst_half1 |= 0x001F; \
        if (dst_half1 & 0x8000) dst_half1 |= 0x7C00; \
        if (dst_half2 & 0x0020) dst_half2 |= 0x001F; \
        if (dst_half2 & 0x0800) dst_half2 |= 0x0400; \
        dst_half1 &= 0x7C1F; \
        dst_half2 &= 0x041F; \
        (_dst) = WORD_LE((dst_half2 << 5) | dst_half1); \
    }

/* 饱和减法 */
#define CR_BLEND_SUB_INIT15 int16u dst_color, src_color; \
                            int16u dst_half1, dst_half2; \
                            int16u src_half1, src_half2;
#define CR_BLEND_SUB_MAIN15(_dst, _src) \
    { \
        dst_color = WORD_LE(_dst); \
        dst_half1 = (dst_color & 0x7C1F); \
        dst_half2 = (dst_color & 0x83E0) >> 5; \
        src_color = WORD_LE(_src); \
        src_half1 = (src_color & 0x7C1F); \
        src_half2 = (src_color & 0x83E0) >> 5; \
        dst_half1 = (dst_half1 | 0x8020) - src_half1; \
        dst_half2 = (dst_half2 | 0x0820) - src_half2; \
        if (!(dst_half1 & 0x0020)) dst_half1 &= 0xFFE0; \
        if (!(dst_half1 & 0x8000)) dst_half1 &= 0x83FF; \
        if (!(dst_half2 & 0x0020)) dst_half2 &= 0xFFE0; \
        if (!(dst_half2 & 0x0800)) dst_half2 &= 0xFBFF; \
        dst_half1 &= 0x7C1F; \
        dst_half2 &= 0x041F; \
        (_dst) = WORD_LE((dst_half2 << 5) | dst_half1); \
    }

/* 插值混合 */
#define CR_BLEND_LRP_INIT15 int16u dst_c, src_c; \
                            sint_t dst_b, dst_g, dst_r; \
                            sint_t src_b, src_g, src_r, alp;
#define CR_BLEND_LRP_LOAD15(_lrp) \
    { \
        alp = (sint_t)(_lrp); \
        alp = alp + (alp >> 4); \
    }
#define CR_BLEND_LRP_MAIN15(_dst, _src) \
    { \
        dst_c = WORD_LE(_dst); \
        dst_b = (dst_c >>  0) & 0x001F; \
        dst_g = (dst_c >>  5) & 0x001F; \
        dst_r = (dst_c >> 10) & 0x001F; \
        src_c = WORD_LE(_src); \
        src_b = (src_c >>  0) & 0x001F; \
        src_g = (src_c >>  5) & 0x001F; \
        src_r = (src_c >> 10) & 0x001F; \
        dst_b += (src_b - dst_b) * alp / 32; \
        dst_g += (src_g - dst_g) * alp / 32; \
        dst_r += (src_r - dst_r) * alp / 32; \
        (_dst) = WORD_LE((int16u)((dst_g << 5) | \
            (dst_r << 10) | (dst_b) | (dst_c & 0x8000))); \
    }

/*****************************************************************************/
/*                                16位色混合                                 */
/*****************************************************************************/

/* 饱和加法 */
#define CR_BLEND_ADD_INIT16 int32u dst_temp1; \
                            int16u dst_color, src_color; \
                            int16u dst_half1, dst_half2; \
                            int16u src_half1, src_half2;
#define CR_BLEND_ADD_MAIN16(_dst, _src) \
    { \
        dst_color = WORD_LE(_dst); \
        dst_half1 = dst_color & 0xF81F; \
        dst_half2 = dst_color & 0x07E0; \
        src_color = WORD_LE(_src); \
        src_half1 = src_color & 0xF81F; \
        src_half2 = src_color & 0x07E0; \
        dst_temp1 = (int32u)dst_half1; \
        dst_temp1 = dst_temp1 + src_half1; \
        dst_half2 = dst_half2 + src_half2; \
        dst_half1 = (int16u)dst_temp1; \
        if (dst_half1 & 0x00020) dst_half1 |= 0x001F; \
        if (dst_temp1 & 0x10000) dst_half1 |= 0xF800; \
        if (dst_half2 & 0x00800) dst_half2 |= 0x07E0; \
        dst_half1 &= 0xF81F; \
        dst_half2 &= 0x07E0; \
        (_dst) = WORD_LE(dst_half2 | dst_half1); \
    }

/* 饱和减法 */
#define CR_BLEND_SUB_INIT16 int32u dst_temp1; \
                            int16u dst_color, src_color; \
                            int16u dst_half1, dst_half2; \
                            int16u src_half1, src_half2;
#define CR_BLEND_SUB_MAIN16(_dst, _src) \
    { \
        dst_color = WORD_LE(_dst); \
        dst_half1 = dst_color & 0xF81F; \
        dst_half2 = dst_color & 0x07E0; \
        src_color = WORD_LE(_src); \
        src_half1 = src_color & 0xF81F; \
        src_half2 = src_color & 0x07E0; \
        dst_temp1 = (int32u)dst_half1; \
        dst_temp1 = (dst_temp1 | 0x10020) - src_half1; \
        dst_half2 = (dst_half2 | 0x00800) - src_half2; \
        dst_half1 = (int16u)dst_temp1; \
        if (!(dst_half1 & 0x00020)) dst_half1 &= 0xFFE0; \
        if (!(dst_temp1 & 0x10000)) dst_half1 &= 0x07FF; \
        if (!(dst_half2 & 0x00800)) dst_half2 &= 0xF81F; \
        dst_half1 &= 0xF81F; \
        dst_half2 &= 0x07E0; \
        (_dst) = WORD_LE(dst_half2 | dst_half1); \
    }

/* 插值混合 */
#define CR_BLEND_LRP_INIT16 int16u dst_c, src_c; \
                            sint_t dst_b, dst_g, dst_r; \
                            sint_t src_b, src_g, src_r, alp;
#define CR_BLEND_LRP_LOAD16(_lrp) \
    { \
        alp = (sint_t)(_lrp); \
        alp = alp + (alp >> 5); \
    }
#define CR_BLEND_LRP_MAIN16(_dst, _src) \
    { \
        dst_c = WORD_LE(_dst); \
        dst_b = (dst_c >>  0) & 0x001F; \
        dst_g = (dst_c >>  5) & 0x003F; \
        dst_r = (dst_c >> 11) & 0x001F; \
        src_c = WORD_LE(_src); \
        src_b = (src_c >>  0) & 0x001F; \
        src_g = (src_c >>  5) & 0x003F; \
        src_r = (src_c >> 11) & 0x001F; \
        dst_b += (src_b - dst_b) * alp / 64; \
        dst_g += (src_g - dst_g) * alp / 64; \
        dst_r += (src_r - dst_r) * alp / 64; \
        (_dst) = WORD_LE((int16u)((dst_g << 5) | \
                    (dst_r << 11) | (dst_b))); \
    }

/*****************************************************************************/
/*                                24位色混合                                 */
/*****************************************************************************/

/* 饱和加法 */
#define CR_BLEND_ADD_INIT24
#define CR_BLEND_ADD_MAIN24(_db, _dg, _dr, _sb, _sg, _sr) \
    { \
        if ((_db) >= 255 - (_sb)) \
            (_db) = 0xFF; \
        else \
            (_db) = (_db) + (_sb); \
        if ((_dg) >= 255 - (_sg)) \
            (_dg) = 0xFF; \
        else \
            (_dg) = (_dg) + (_sg); \
        if ((_dr) >= 255 - (_sr)) \
            (_dr) = 0xFF; \
        else \
            (_dr) = (_dr) + (_sr); \
    }

/* 饱和减法 */
#define CR_BLEND_SUB_INIT24
#define CR_BLEND_SUB_MAIN24(_db, _dg, _dr, _sb, _sg, _sr) \
    { \
        if ((_db) <= (_sb)) \
            (_db) = 0x00; \
        else \
            (_db) = (_db) - (_sb); \
        if ((_dg) <= (_sg)) \
            (_dg) = 0x00; \
        else \
            (_dg) = (_dg) - (_sg); \
        if ((_dr) <= (_sr)) \
            (_dr) = 0x00; \
        else \
            (_dr) = (_dr) - (_sr); \
    }

/* 插值混合 */
#define CR_BLEND_LRP_INIT24 sint_t alp, dlt1; \
                            sint_t dlt2, dlt3;
#define CR_BLEND_LRP_LOAD24(_lrp) \
    { \
        alp = (sint_t)(_lrp); \
        alp = alp + (alp >> 7); \
    }
#define CR_BLEND_LRP_MAIN24(_db, _dg, _dr, _sb, _sg, _sr) \
    { \
        dlt1 = (sint_t)(_sb) - (sint_t)(_db); \
        dlt2 = (sint_t)(_sg) - (sint_t)(_dg); \
        dlt3 = (sint_t)(_sr) - (sint_t)(_dr); \
        dlt1 = dlt1 * alp / 256; \
        dlt2 = dlt2 * alp / 256; \
        dlt3 = dlt3 * alp / 256; \
        (_db) = (byte_t)((sint_t)(_db) + dlt1); \
        (_dg) = (byte_t)((sint_t)(_dg) + dlt2); \
        (_dr) = (byte_t)((sint_t)(_dr) + dlt3); \
    }

/*****************************************************************************/
/*                                32位色混合                                 */
/*****************************************************************************/

/* 饱和加法 */
#define CR_BLEND_ADD_INIT32
#define CR_BLEND_ADD_MAIN32(_db, _dg, _dr, _da, _sb, _sg, _sr, _sa) \
    { \
        if ((_db) >= 255 - (_sb)) \
            (_db) = 0xFF; \
        else \
            (_db) = (_db) + (_sb); \
        if ((_dg) >= 255 - (_sg)) \
            (_dg) = 0xFF; \
        else \
            (_dg) = (_dg) + (_sg); \
        if ((_dr) >= 255 - (_sr)) \
            (_dr) = 0xFF; \
        else \
            (_dr) = (_dr) + (_sr); \
        if ((_da) >= 255 - (_sa)) \
            (_da) = 0xFF; \
        else \
            (_da) = (_da) + (_sa); \
    }

/* 饱和减法 */
#define CR_BLEND_SUB_INIT32
#define CR_BLEND_SUB_MAIN32(_db, _dg, _dr, _da, _sb, _sg, _sr, _sa) \
    { \
        if ((_db) <= (_sb)) \
            (_db) = 0x00; \
        else \
            (_db) = (_db) - (_sb); \
        if ((_dg) <= (_sg)) \
            (_dg) = 0x00; \
        else \
            (_dg) = (_dg) - (_sg); \
        if ((_dr) <= (_sr)) \
            (_dr) = 0x00; \
        else \
            (_dr) = (_dr) - (_sr); \
        if ((_da) <= (_sa)) \
            (_da) = 0x00; \
        else \
            (_da) = (_da) - (_sa); \
    }

/* 插值混合 */
#define CR_BLEND_LRP_INIT32 sint_t alp, dlt1; \
                            sint_t dlt2, dlt3;
#define CR_BLEND_LRP_LOAD32(_lrp) \
    { \
        alp = (sint_t)(_lrp); \
        alp = alp + (alp >> 7); \
    }
#define CR_BLEND_LRP_MAIN32(_db, _dg, _dr, _sb, _sg, _sr) \
    { \
        dlt1 = (sint_t)(_sb) - (sint_t)(_db); \
        dlt2 = (sint_t)(_sg) - (sint_t)(_dg); \
        dlt3 = (sint_t)(_sr) - (sint_t)(_dr); \
        dlt1 = dlt1 * alp / 256; \
        dlt2 = dlt2 * alp / 256; \
        dlt3 = dlt3 * alp / 256; \
        (_db) = (byte_t)((sint_t)(_db) + dlt1); \
        (_dg) = (byte_t)((sint_t)(_dg) + dlt2); \
        (_dr) = (byte_t)((sint_t)(_dr) + dlt3); \
    }

#endif  /* !__CR_BLENDS_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
