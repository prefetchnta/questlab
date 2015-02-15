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
/*  >>>>>>>>>>>>>>>>>>>> CrHack UCDOS 点阵字库绘制模板 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/*
************************************************
**  CR_UC_PRINT = 为打印用字库
**  CR_UC_HZK_T = 分离汉字符号区
**  FONT_UC_DRAW_TRAN = 函数名 tran
**  FONT_UC_DRAW_TEXT = 函数名 text
**  FONT_UC_CALC_RECT = 函数名 calc
**  CR_UC_ASC_START = 字库 ASCII 开始值
**  CR_UC_HZK_SW, CR_UC_ASC_SW = 中西文存放的宽
**  CR_UC_HZK_TW, CR_UC_HZK_TH = 中文的真实宽高
************************************************
*/
#define CR_UC_ASC_TW    (CR_UC_HZK_TW / 2)
#define CR_UC_ASC_SIZE  (CR_UC_ASC_SW * CR_UC_HZK_TH / 8)
#define CR_UC_HZK_SIZE  (CR_UC_HZK_SW * CR_UC_HZK_TH / 8)

/*
---------------------------------------
    绘制点阵文字 (透明)
---------------------------------------
*/
static bool_t
FONT_UC_DRAW_TRAN (
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
    uint_t      xspace, yspace;
    uint_t      t, x, y, z, dx, dy, sx;
    byte_t      *memo, *ptr, dat[CR_UC_HZK_SIZE];
    iFONT_UC    *real = (iFONT_UC*)that;

#ifndef CR_UC_HZK_T
    iDATIN  *asc, *hzk, *two;
#else
    iDATIN  *asc, *hzk, *chr, *two;
#endif
    /* 以免每次读结构体 */
    pixdraw_t   pixel_draw;

    if (real->m_draw == NULL)
        return (FALSE);

    /* 空字符串直接返回 */
    if (str_lenA((ansi_t*)text) == 0)
        return (TRUE);

    /* 对目标表面的剪裁 */
    if (!clip_rect(&clip, rect, &real->m_rect))
        return (TRUE);

    /* 锁住位图表面 (只对硬件加速有效) */
    dest = CR_VCALL(real->m_draw)->lock(real->m_draw);
    if (dest == NULL)
        return (FALSE);
    dx = rect->x1;
    dy = rect->y1;
    CR_NOUSE(cpage);
    asc = real->m_ascf;
    hzk = real->m_hzkf;
    memo = (byte_t*)text;
    color.val = real->font.__color__;
    pixel_draw = real->pixel_draw;
    sx = dx; /* 保存开始位置以便换行 */

#ifndef CR_UC_HZK_T
    two = hzk;
#else
    chr = real->m_chrf;
#endif
    xspace = real->m_xspc;
    yspace = real->m_yspc;
    while (*memo != 0)
    {
        /* 识别中西文 */
        if (memo[0] >= 0xA1 && memo[1] >= 0xA1)
        {
            /* 中文字符 */
#ifndef CR_UC_HZK_T
            if (hzk == NULL)
                goto _invalid_hzk;
            offs = (memo[0] - 0xA1) * 94 + (memo[1] - 0xA1);
#else
            if (memo[0] >= 0xB0) {
                if (hzk == NULL)
                    goto _invalid_hzk;
                two = hzk;
                offs = (memo[0] - 0xB0) * 94 + (memo[1] - 0xA1);
            }
            else {
                if (chr == NULL)
                    goto _invalid_hzk;
                two = chr;
                offs = (memo[0] - 0xA1) * 94 + (memo[1] - 0xA1);
            }
#endif
            offs *= CR_UC_HZK_SIZE;
            if (!CR_VCALL(two)->seek(two, offs, SEEK_SET))
                goto _invalid_hzk;
            if (CR_VCALL(two)->read(two,dat,CR_UC_HZK_SIZE) != CR_UC_HZK_SIZE)
                goto _invalid_hzk;

            ptr = dat;
            for (x = 0, y = 0; y < CR_UC_HZK_TH; y++) {
                for (t = 0; t < CR_UC_HZK_SW / 8; t++, ptr++) {
                    for (z = 0; z < 8; z++, *ptr <<= 1) {
#ifndef CR_UC_PRINT
                        if ((*ptr & 0x80) &&
                            clip_pixel(dx + x, dy + y, &clip))
                            pixel_draw(dest, dx + x, dy + y, color);
#else
                        if ((*ptr & 0x80) &&
                            clip_pixel(dx + y, dy + x, &clip))
                            pixel_draw(dest, dx + y, dy + x, color);
#endif
                        if (x++ >= CR_UC_HZK_SW - 1)
                            x = 0;
                    }
                }
            }
_invalid_hzk:
            memo += 2;
            dx += CR_UC_HZK_TW + xspace;
        }
        else if (*memo == (byte_t)('\n'))
        {
            /* 换行字符 */
            memo++;
            dx = sx;
            dy += CR_UC_HZK_TH + yspace;
        }
        else if (*memo == (byte_t)('\r'))
        {
            /* 换行字符 */
            memo++;
            if (*memo == (byte_t)('\n'))
                 memo++;
            dx = sx;
            dy += CR_UC_HZK_TH + yspace;
        }
        else
        {
            /* 西文字符 */
            if (asc == NULL)
                goto _invalid_asc;
#if (CR_UC_ASC_START != 0)
            if (*memo < CR_UC_ASC_START)
                goto _invalid_asc;
#endif
            offs = *memo - CR_UC_ASC_START;
            offs *= CR_UC_ASC_SIZE;
            if (!CR_VCALL(asc)->seek(asc, offs, SEEK_SET))
                goto _invalid_asc;
            if (CR_VCALL(asc)->read(asc,dat,CR_UC_ASC_SIZE) != CR_UC_ASC_SIZE)
                goto _invalid_asc;

            ptr = dat;
            for (x = 0, y = 0; y < CR_UC_HZK_TH; y++) {
                for (t = 0; t < CR_UC_ASC_SW / 8; t++, ptr++) {
                    for (z = 0; z < 8; z++, *ptr <<= 1) {
#ifndef CR_UC_PRINT
                        if ((*ptr & 0x80) &&
                            clip_pixel(dx + x, dy + y, &clip))
                            pixel_draw(dest, dx + x, dy + y, color);
#else
                        if ((*ptr & 0x80) &&
                            clip_pixel(dx + y, dy + x, &clip))
                            pixel_draw(dest, dx + y, dy + x, color);
#endif
                        if (x++ >= CR_UC_ASC_SW - 1)
                            x = 0;
                    }
                }
            }
_invalid_asc:
            memo += 1;
            dx += CR_UC_ASC_TW + xspace;
        }
    }
    /* 解锁位图表面 (只对硬件加速有效) */
    CR_VCALL(real->m_draw)->unlock(real->m_draw);
    return (TRUE);
}

/*
---------------------------------------
    绘制点阵文字 (实体)
---------------------------------------
*/
static bool_t
FONT_UC_DRAW_TEXT (
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
    uint_t      xspace, yspace;
    uint_t      t, x, y, z, dx, dy, sx;
    byte_t      *memo, *ptr, dat[CR_UC_HZK_SIZE];
    iFONT_UC    *real = (iFONT_UC*)that;

#ifndef CR_UC_HZK_T
    iDATIN  *asc, *hzk, *two;
#else
    iDATIN  *asc, *hzk, *chr, *two;
#endif
    /* 以免每次读结构体 */
    pixdraw_t   pixel_draw;

    if (real->m_draw == NULL)
        return (FALSE);

    /* 空字符串直接返回 */
    if (str_lenA((ansi_t*)text) == 0)
        return (TRUE);

    /* 对目标表面的剪裁 */
    if (!clip_rect(&clip, rect, &real->m_rect))
        return (TRUE);

    /* 锁住位图表面 (只对硬件加速有效) */
    dest = CR_VCALL(real->m_draw)->lock(real->m_draw);
    if (dest == NULL)
        return (FALSE);
    dx = rect->x1;
    dy = rect->y1;
    CR_NOUSE(cpage);
    asc = real->m_ascf;
    hzk = real->m_hzkf;
    memo = (byte_t*)text;
    color.val = real->font.__color__;
    bkcolor.val = real->font.__bkcolor__;
    pixel_draw = real->pixel_draw;
    sx = dx; /* 保存开始位置以便换行 */

#ifndef CR_UC_HZK_T
    two = hzk;
#else
    chr = real->m_chrf;
#endif
    xspace = real->m_xspc;
    yspace = real->m_yspc;
    while (*memo != 0)
    {
        /* 识别中西文 */
        if (memo[0] >= 0xA1 && memo[1] >= 0xA1)
        {
            /* 中文字符 */
#ifndef CR_UC_HZK_T
            if (hzk == NULL)
                goto _invalid_hzk;
            offs = (memo[0] - 0xA1) * 94 + (memo[1] - 0xA1);
#else
            if (memo[0] >= 0xB0) {
                if (hzk == NULL)
                    goto _invalid_hzk;
                two = hzk;
                offs = (memo[0] - 0xB0) * 94 + (memo[1] - 0xA1);
            }
            else {
                if (chr == NULL)
                    goto _invalid_hzk;
                two = chr;
                offs = (memo[0] - 0xA1) * 94 + (memo[1] - 0xA1);
            }
#endif
            offs *= CR_UC_HZK_SIZE;
            if (!CR_VCALL(two)->seek(two, offs, SEEK_SET))
                goto _invalid_hzk;
            if (CR_VCALL(two)->read(two,dat,CR_UC_HZK_SIZE) != CR_UC_HZK_SIZE)
                goto _invalid_hzk;

            ptr = dat;
            for (x = 0, y = 0; y < CR_UC_HZK_TH; y++) {
                for (t = 0; t < CR_UC_HZK_SW / 8; t++, ptr++) {
                    for (z = 0; z < 8; z++, *ptr <<= 1) {
#ifndef CR_UC_PRINT
                        if (clip_pixel(dx + x, dy + y, &clip)) {
                            if (*ptr & 0x80)
                                pixel_draw(dest, dx + x, dy + y, color);
                            else
                                pixel_draw(dest, dx + x, dy + y, bkcolor);
                        }
#else
                        if (clip_pixel(dx + y, dy + x, &clip)) {
                            if (*ptr & 0x80)
                                pixel_draw(dest, dx + y, dy + x, color);
                            else
                                pixel_draw(dest, dx + y, dy + x, bkcolor);
                        }
#endif
                        if (x++ >= CR_UC_HZK_SW - 1)
                            x = 0;
                    }
                }
            }
_invalid_hzk:
            memo += 2;
            dx += CR_UC_HZK_TW + xspace;
        }
        else if (*memo == (byte_t)('\n'))
        {
            /* 换行字符 */
            memo++;
            dx = sx;
            dy += CR_UC_HZK_TH + yspace;
        }
        else if (*memo == (byte_t)('\r'))
        {
            /* 换行字符 */
            memo++;
            if (*memo == (byte_t)('\n'))
                 memo++;
            dx = sx;
            dy += CR_UC_HZK_TH + yspace;
        }
        else
        {
            /* 西文字符 */
            if (asc == NULL)
                goto _invalid_asc;
#if (CR_UC_ASC_START != 0)
            if (*memo < CR_UC_ASC_START)
                goto _invalid_asc;
#endif
            /* 西文字符 */
            offs = *memo - CR_UC_ASC_START;
            offs *= CR_UC_ASC_SIZE;
            if (!CR_VCALL(asc)->seek(asc, offs, SEEK_SET))
                goto _invalid_asc;
            if (CR_VCALL(asc)->read(asc,dat,CR_UC_ASC_SIZE) != CR_UC_ASC_SIZE)
                goto _invalid_asc;

            ptr = dat;
            for (x = 0, y = 0; y < CR_UC_HZK_TH; y++) {
                for (t = 0; t < CR_UC_ASC_SW / 8; t++, ptr++) {
                    for (z = 0; z < 8; z++, *ptr <<= 1) {
#ifndef CR_UC_PRINT
                        if (clip_pixel(dx + x, dy + y, &clip)) {
                            if (*ptr & 0x80)
                                pixel_draw(dest, dx + x, dy + y, color);
                            else
                                pixel_draw(dest, dx + x, dy + y, bkcolor);
                        }
#else
                        if (clip_pixel(dx + y, dy + x, &clip)) {
                            if (*ptr & 0x80)
                                pixel_draw(dest, dx + y, dy + x, color);
                            else
                                pixel_draw(dest, dx + y, dy + x, bkcolor);
                        }
#endif
                        if (x++ >= CR_UC_ASC_SW - 1)
                            x = 0;
                    }
                }
            }
_invalid_asc:
            memo += 1;
            dx += CR_UC_ASC_TW + xspace;
        }
    }
    /* 解锁位图表面 (只对硬件加速有效) */
    CR_VCALL(real->m_draw)->unlock(real->m_draw);
    return (TRUE);
}

/*
---------------------------------------
    测量文字输出范围
---------------------------------------
*/
static bool_t
FONT_UC_CALC_RECT (
  __CR_IN__ iFONT*          that,
  __CR_IN__ const void_t*   text,
  __CR_IO__ sRECT*          rect,
  __CR_IN__ uint_t          cpage
    )
{
    uchar*      memo;
    uint_t      dx, dy, sx;
    uint_t      xspace, yspace;
    iFONT_UC*   real = (iFONT_UC*)that;

    CR_NOUSE(that);
    CR_NOUSE(cpage);
    sx = dx = dy = 0;
    memo = (byte_t*)text;
    xspace = real->m_xspc;
    yspace = real->m_yspc;

    while (*memo != 0)
    {
        /* 识别中西文 */
        if (memo[0] >= 0xA1 && memo[1] >= 0xA1)
        {
            /* 中文字符 */
            memo += 2;
            dx += CR_UC_HZK_TW + xspace;
        }
        else
        if (*memo == (byte_t)('\n'))
        {
            /* 换行字符 */
            memo++;
            dx = 0;
            dy += CR_UC_HZK_TH + yspace;
        }
        else
        if (*memo == (byte_t)('\r'))
        {
            /* 换行字符 */
            memo++;
            if (*memo == (byte_t)('\n'))
                 memo++;
            dx = 0;
            dy += CR_UC_HZK_TH + yspace;
        }
        else
        {
            /* 西文字符 */
            memo += 1;
            dx += CR_UC_ASC_TW + xspace;
        }

        /* 取最大边长 */
        if (dx > sx)
            sx = dx;
    }
    rect_set_wh(rect, rect->x1, rect->y1, sx, dy + CR_UC_HZK_TH + yspace);
    return (TRUE);
}

#undef  CR_UC_ASC_TW
#undef  CR_UC_ASC_SIZE
#undef  CR_UC_HZK_SIZE

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
