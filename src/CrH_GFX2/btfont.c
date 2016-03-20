/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-30  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 通用点阵文字绘制接口实现 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "bltint.h"
#include "memlib.h"
#include "strlib.h"

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iFONT_UC    ucdos;

        /* 个性部分 */
        uint_t  m_hzk_th; /* 文字的真实高 */
        uint_t  m_asc_start; /* 字库 ASCII 开始值 */
        uint_t  m_hzk_sw, m_asc_sw; /* 中西文存放的宽 */
        uint_t  m_hzk_tw, m_asc_tw; /* 中西文真实的宽 */
        leng_t  m_hzk_size, m_asc_size; /* 中西文字节大小 */

        /* 计算偏移值的回调 */
        chr2off_t   char2offs;

} iFONT_BIT;

/*
---------------------------------------
    获取扩展的文字接口
---------------------------------------
*/
static void_t*
iFONT_BIT_getMore (
  __CR_IN__ iFONT*  that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iFONT::BIT") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    绘制点阵文字 (透明)
---------------------------------------
*/
static bool_t
iFONT_BIT_draw_tran (
  __CR_IN__ iFONT*          that,
  __CR_IN__ const void_t*   text,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          cpage
    )
{
    sRECT       clip;
    int32u      offs;
    cpix_t      color;
    sIMAGE      *dest;
    iDATIN      *asc, *hzk;
    uint_t      xspace, yspace;
    uint_t      hzk_sw, asc_sw;
    uint_t      hzk_th, asc_start;
    leng_t      hzk_size, asc_size;
    uint_t      hzk_tw, asc_tw, step;
    uint_t      t, x, y, z, dx, dy, sx;
    byte_t      *memo, *ptr, dat[648];
    iFONT_BIT   *real = (iFONT_BIT*)that;

    /* 以免每次读结构体 */
    chr2off_t   char2offs;
    pixdraw_t   pixel_draw;

    if (real->ucdos.m_draw == NULL)
        return (FALSE);

    /* 空字符串直接返回 */
    if (str_lenA((ansi_t*)text) == 0)
        return (TRUE);

    /* 对目标表面的剪裁 */
    if (!clip_rect(&clip, rect, &real->ucdos.m_rect))
        return (TRUE);

    /* 锁住位图表面 (只对硬件加速有效) */
    dest = CR_VCALL(real->ucdos.m_draw)->lock(real->ucdos.m_draw);
    if (dest == NULL)
        return (FALSE);
    dx = rect->x1;
    dy = rect->y1;
    CR_NOUSE(cpage);
    memo = (byte_t*)text;
    asc = real->ucdos.m_ascf;
    hzk = real->ucdos.m_hzkf;
    color.val = real->ucdos.font.__color__;
    char2offs = real->char2offs;
    pixel_draw = real->ucdos.pixel_draw;
    sx = dx; /* 保存开始位置以便换行 */

    /* 加载点阵文字属性 */
    hzk_sw = real->m_hzk_sw;
    asc_sw = real->m_asc_sw;
    hzk_tw = real->m_hzk_tw;
    asc_tw = real->m_asc_tw;
    hzk_th = real->m_hzk_th;
    xspace = real->ucdos.m_xspc;
    yspace = real->ucdos.m_yspc;
    hzk_size  = real->m_hzk_size;
    asc_size  = real->m_asc_size;
    asc_start = real->m_asc_start;

    while (*memo != 0)
    {
        /* 识别中西文 */
        step = char2offs(memo, &offs);
        if (step > 1)
        {
            /* 中文字符 */
            if (hzk == NULL)
                goto _invalid_hzk;
            offs *= (int32u)hzk_size;
            if (!CR_VCALL(hzk)->seek(hzk, offs, SEEK_SET))
                goto _invalid_hzk;
            if (CR_VCALL(hzk)->read(hzk, dat, hzk_size) != hzk_size)
                goto _invalid_hzk;

            ptr = dat;
            for (x = 0, y = 0; y < hzk_th; y++) {
                for (t = 0; t < hzk_sw / 8; t++, ptr++) {
                    for (z = 0; z < 8; z++, *ptr <<= 1) {
                        if ((*ptr & 0x80) &&
                            clip_pixel(dx + x, dy + y, &clip))
                            pixel_draw(dest, dx + x, dy + y, color);
                        if (x++ >= hzk_sw - 1)
                            x = 0;
                    }
                }
            }
_invalid_hzk:
            memo += step;
            dx += hzk_tw + xspace;
        }
        else if (*memo == (byte_t)('\n'))
        {
            /* 换行字符 */
            memo++;
            dx = sx;
            dy += hzk_th + yspace;
        }
        else if (*memo == (byte_t)('\r'))
        {
            /* 换行字符 */
            memo++;
            if (*memo == (byte_t)('\n'))
                 memo++;
            dx = sx;
            dy += hzk_th + yspace;
        }
        else
        {
            /* 西文字符 */
            if (asc == NULL ||
               (uint_t)(*memo) < asc_start)
                goto _invalid_asc;
            offs = *memo - asc_start;
            offs *= (int32u)asc_size;
            if (!CR_VCALL(asc)->seek(asc, offs, SEEK_SET))
                goto _invalid_asc;
            if (CR_VCALL(asc)->read(asc, dat, asc_size) != asc_size)
                goto _invalid_asc;

            ptr = dat;
            for (x = 0, y = 0; y < hzk_th; y++) {
                for (t = 0; t < asc_sw / 8; t++, ptr++) {
                    for (z = 0; z < 8; z++, *ptr <<= 1) {
                        if ((*ptr & 0x80) &&
                            clip_pixel(dx + x, dy + y, &clip))
                            pixel_draw(dest, dx + x, dy + y, color);
                        if (x++ >= asc_sw - 1)
                            x = 0;
                    }
                }
            }
_invalid_asc:
            memo += 1;
            dx += asc_tw + xspace;
        }
    }
    /* 解锁位图表面 (只对硬件加速有效) */
    CR_VCALL(real->ucdos.m_draw)->unlock(real->ucdos.m_draw);
    return (TRUE);
}

/*
---------------------------------------
    绘制点阵文字 (实体)
---------------------------------------
*/
static bool_t
iFONT_BIT_draw_text (
  __CR_IN__ iFONT*          that,
  __CR_IN__ const void_t*   text,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          cpage
    )
{
    sRECT       clip;
    int32u      offs;
    cpix_t      color;
    cpix_t      bkcolor;
    sIMAGE      *dest;
    iDATIN      *asc, *hzk;
    uint_t      xspace, yspace;
    uint_t      hzk_sw, asc_sw;
    uint_t      hzk_th, asc_start;
    leng_t      hzk_size, asc_size;
    uint_t      hzk_tw, asc_tw, step;
    uint_t      t, x, y, z, dx, dy, sx;
    byte_t      *memo, *ptr, dat[648];
    iFONT_BIT   *real = (iFONT_BIT*)that;

    /* 以免每次读结构体 */
    chr2off_t   char2offs;
    pixdraw_t   pixel_draw;

    if (real->ucdos.m_draw == NULL)
        return (FALSE);

    /* 空字符串直接返回 */
    if (str_lenA((ansi_t*)text) == 0)
        return (TRUE);

    /* 对目标表面的剪裁 */
    if (!clip_rect(&clip, rect, &real->ucdos.m_rect))
        return (TRUE);

    /* 锁住位图表面 (只对硬件加速有效) */
    dest = CR_VCALL(real->ucdos.m_draw)->lock(real->ucdos.m_draw);
    if (dest == NULL)
        return (FALSE);
    dx = rect->x1;
    dy = rect->y1;
    CR_NOUSE(cpage);
    memo = (byte_t*)text;
    asc = real->ucdos.m_ascf;
    hzk = real->ucdos.m_hzkf;
    color.val = real->ucdos.font.__color__;
    bkcolor.val = real->ucdos.font.__bkcolor__;
    char2offs = real->char2offs;
    pixel_draw = real->ucdos.pixel_draw;
    sx = dx; /* 保存开始位置以便换行 */

    /* 加载一堆点阵文字属性 */
    hzk_sw = real->m_hzk_sw;
    asc_sw = real->m_asc_sw;
    hzk_tw = real->m_hzk_tw;
    asc_tw = real->m_asc_tw;
    hzk_th = real->m_hzk_th;
    xspace = real->ucdos.m_xspc;
    yspace = real->ucdos.m_yspc;
    hzk_size  = real->m_hzk_size;
    asc_size  = real->m_asc_size;
    asc_start = real->m_asc_start;

    while (*memo != 0)
    {
        /* 识别中西文 */
        step = char2offs(memo, &offs);
        if (step > 1)
        {
            /* 中文字符 */
            if (hzk == NULL)
                goto _invalid_hzk;
            offs *= (int32u)hzk_size;
            if (!CR_VCALL(hzk)->seek(hzk, offs, SEEK_SET))
                goto _invalid_hzk;
            if (CR_VCALL(hzk)->read(hzk, dat, hzk_size) != hzk_size)
                goto _invalid_hzk;

            ptr = dat;
            for (x = 0, y = 0; y < hzk_th; y++) {
                for (t = 0; t < hzk_sw / 8; t++, ptr++) {
                    for (z = 0; z < 8; z++, *ptr <<= 1) {
                        if (clip_pixel(dx + x, dy + y, &clip)) {
                            if (*ptr & 0x80)
                                pixel_draw(dest, dx + x, dy + y, color);
                            else
                                pixel_draw(dest, dx + x, dy + y, bkcolor);
                        }
                        if (x++ >= hzk_sw - 1)
                            x = 0;
                    }
                }
            }
_invalid_hzk:
            memo += step;
            dx += hzk_tw + xspace;
        }
        else if (*memo == (byte_t)('\n'))
        {
            /* 换行字符 */
            memo++;
            dx = sx;
            dy += hzk_th + yspace;
        }
        else if (*memo == (byte_t)('\r'))
        {
            /* 换行字符 */
            memo++;
            if (*memo == (byte_t)('\n'))
                 memo++;
            dx = sx;
            dy += hzk_th + yspace;
        }
        else
        {
            /* 西文字符 */
            if (asc == NULL ||
               (uint_t)(*memo) < asc_start)
                goto _invalid_asc;
            offs = *memo - asc_start;
            offs *= (int32u)asc_size;
            if (!CR_VCALL(asc)->seek(asc, offs, SEEK_SET))
                goto _invalid_asc;
            if (CR_VCALL(asc)->read(asc, dat, asc_size) != asc_size)
                goto _invalid_asc;

            ptr = dat;
            for (x = 0, y = 0; y < hzk_th; y++) {
                for (t = 0; t < asc_sw / 8; t++, ptr++) {
                    for (z = 0; z < 8; z++, *ptr <<= 1) {
                        if (clip_pixel(dx + x, dy + y, &clip)) {
                            if (*ptr & 0x80)
                                pixel_draw(dest, dx + x, dy + y, color);
                            else
                                pixel_draw(dest, dx + x, dy + y, bkcolor);
                        }
                        if (x++ >= asc_sw - 1)
                            x = 0;
                    }
                }
            }
_invalid_asc:
            memo += 1;
            dx += asc_tw + xspace;
        }
    }
    /* 解锁位图表面 (只对硬件加速有效) */
    CR_VCALL(real->ucdos.m_draw)->unlock(real->ucdos.m_draw);
    return (TRUE);
}

/*
---------------------------------------
    测量文字输出范围
---------------------------------------
*/
static bool_t
iFONT_BIT_calc_rect (
  __CR_IN__ iFONT*          that,
  __CR_IN__ const void_t*   text,
  __CR_IO__ sRECT*          rect,
  __CR_IN__ uint_t          cpage
    )
{
    int32u      offs;
    byte_t*     memo;
    uint_t      xspace, yspace;
    uint_t      dx, dy, sx, step;
    uint_t      hzk_tw, asc_tw, hzk_th;
    iFONT_BIT*  real = (iFONT_BIT*)that;

    /* 以免每次读结构体 */
    chr2off_t   char2offs;

    CR_NOUSE(cpage);
    sx = dx = dy = 0;
    memo = (byte_t*)text;
    char2offs = real->char2offs;

    /* 加载一堆点阵文字属性 */
    hzk_tw = real->m_hzk_tw;
    asc_tw = real->m_asc_tw;
    hzk_th = real->m_hzk_th;
    xspace = real->ucdos.m_xspc;
    yspace = real->ucdos.m_yspc;

    while (*memo != 0)
    {
        /* 识别中西文 */
        step = char2offs(memo, &offs);
        if (step > 1)
        {
            /* 中文字符 */
            memo += step;
            dx += hzk_tw + xspace;
        }
        else
        if (*memo == (byte_t)('\n'))
        {
            /* 换行字符 */
            memo++;
            dx = 0;
            dy += hzk_th + yspace;
        }
        else
        if (*memo == (byte_t)('\r'))
        {
            /* 换行字符 */
            memo++;
            if (*memo == (byte_t)('\n'))
                 memo++;
            dx = 0;
            dy += hzk_th + yspace;
        }
        else
        {
            /* 西文字符 */
            memo += 1;
            dx += asc_tw + xspace;
        }

        /* 取最大边长 */
        if (dx > sx)
            sx = dx;
    }
    rect_set_wh(rect, rect->x1, rect->y1, sx, dy + hzk_th + yspace);
    return (TRUE);
}

/*
---------------------------------------
    GBK 编码的偏移回调
---------------------------------------
*/
static uint_t
gbk_char2offs (
  __CR_IN__ const byte_t*   str,
  __CR_OT__ int32u*         offset
    )
{
    if (!(str[0] >= 0x81 && str[1] >= 0x40))
        return (1);
    *offset = (str[0] - 0x81) * 191 + (str[1] - 0x40);
    return (2);
}

/*
---------------------------------------
    GB2312 编码的偏移回调
---------------------------------------
*/
static uint_t
gb2312_char2offs (
  __CR_IN__ const byte_t*   str,
  __CR_OT__ int32u*         offset
    )
{
    if (!(str[0] >= 0xA1 && str[1] >= 0xA1))
        return (1);
    *offset = (str[0] - 0xA1) * 94 + (str[1] - 0xA1);
    return (2);
}

/* 接口虚函数表 */
static const iFONT_vtbl _rom_ s_font_vtbl =
{
    iFONT_UC_release, iFONT_BIT_getMore,
    iFONT_UC_enter, iFONT_UC_leave, iFONT_UC_bind,
    iFONT_UC_setMode, iFONT_UC_setColor, iFONT_UC_setBkColor,
    iFONT_BIT_draw_tran, iFONT_BIT_draw_text, iFONT_BIT_calc_rect,
};

/*
=======================================
    生成通用点阵文字绘制接口
=======================================
*/
CR_API iFONT*
create_bit_font (
  __CR_IN__ iDATIN*         asc,
  __CR_IN__ iDATIN*         hzk,
  __CR_IN__ uint_t          size,
  __CR_IN__ uint_t          xspc,
  __CR_IN__ uint_t          yspc,
  __CR_IN__ const ansi_t*   type
    )
{
    iFONT_BIT*  font;

    if (size > 72 || size < 8)
        return (NULL);

    font = struct_new(iFONT_BIT);
    if (font == NULL)
        return (NULL);
    struct_zero(font, iFONT_BIT);

    /* 根据编码类型设置相应的回调 */
    if (str_cmpA(type, "GBK") == 0) {
        font->char2offs = gbk_char2offs;
    }
    else
    if (str_cmpA(type, "GB2312") == 0) {
        font->char2offs = gb2312_char2offs;
    }
    else {
        mem_free(font);
        return (NULL);
    }
    font->m_hzk_th = size;
    font->m_hzk_tw = size;
    font->m_asc_tw = size / 2;
    font->m_hzk_sw = font->m_hzk_tw + (uint_t)CR_PADDED(font->m_hzk_tw, 8);
    font->m_asc_sw = font->m_asc_tw + (uint_t)CR_PADDED(font->m_asc_tw, 8);
    font->m_hzk_size = font->m_hzk_sw * size / 8;
    font->m_asc_size = font->m_asc_sw * size / 8;
    if ((fsize_t)asc->__size__ <= font->m_asc_size * 96)
        font->m_asc_start = 0x20;

    font->ucdos.m_ascf = asc;
    font->ucdos.m_hzkf = hzk;
    font->ucdos.m_xspc = xspc;
    font->ucdos.m_yspc = yspc;
    font->ucdos.font.__vptr__ = &s_font_vtbl;
    return ((iFONT*)font);
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
