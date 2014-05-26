/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-29  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 像素绘制函数库模板 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 根据指定的宏选择操作符 */
#if     (CR_BLT_MODE == CR_BLT_SET)
    #define CR_PIXEL_OP(addr, color)    (addr)  = (color)
#elif   (CR_BLT_MODE == CR_BLT_AND)
    #define CR_PIXEL_OP(addr, color)    (addr) &= (color)
#elif   (CR_BLT_MODE == CR_BLT_ORR)
    #define CR_PIXEL_OP(addr, color)    (addr) |= (color)
#elif   (CR_BLT_MODE == CR_BLT_NOT)
    #define CR_PIXEL_OP(addr, color)    (addr)  = ~(addr)
#elif   (CR_BLT_MODE == CR_BLT_XOR)
    #define CR_PIXEL_OP(addr, color)    (addr) ^= (color)
#else
    #error "pixels.inl: invalid CR_BLT_MODE type!"
#endif

/*****************************************************************************/
/*                                 像素模板                                  */
/*****************************************************************************/

/* 选择包含像素函数模板 */
#if defined(CR_NAME_PIXEL)

/* 根据指定的宏选择起始坐标 */
#ifndef CR_PIXEL_FLIP
    #define CR_PIXEL_Y  (y)
#else
    #define CR_PIXEL_Y  (dst->clip_win.y2 - y)
#endif

/*
=======================================
    像素绘制 (bpc = 1)
=======================================
*/
CR_API void_t
#ifndef CR_PIXEL_FLIP
CR_NAME_PIXEL(08z) (
#else
CR_NAME_PIXEL(08n) (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ cpix_t          color
    )
{
    byte_t* addr;

#if (CR_BLT_MODE == CR_BLT_NOT)
    CR_NOUSE(color.c08.idx);
#endif
    addr = pixel_addr1(dst, x, CR_PIXEL_Y);
    CR_PIXEL_OP(addr[0], color.c08.idx);
}

/*
=======================================
    像素绘制 (bpc = 2)
=======================================
*/
CR_API void_t
#ifndef CR_PIXEL_FLIP
CR_NAME_PIXEL(16z) (
#else
CR_NAME_PIXEL(16n) (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ cpix_t          color
    )
{
    int16u* addr;

#if (CR_BLT_MODE == CR_BLT_NOT)
    CR_NOUSE(color.c16.clr);
#endif
    addr = pixel_addr2(dst, x, CR_PIXEL_Y);
    CR_PIXEL_OP(addr[0], color.c16.clr);
}

/*
=======================================
    像素绘制 (bpc = 3)
=======================================
*/
CR_API void_t
#ifndef CR_PIXEL_FLIP
CR_NAME_PIXEL(24z) (
#else
CR_NAME_PIXEL(24n) (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ cpix_t          color
    )
{
    byte_t* addr;

#if (CR_BLT_MODE == CR_BLT_NOT)
    CR_NOUSE(color.c32.bbb);
#endif
    addr = pixel_addr3(dst, x, CR_PIXEL_Y);
    CR_PIXEL_OP(addr[0], color.c32.bbb);
    CR_PIXEL_OP(addr[1], color.c32.ggg);
    CR_PIXEL_OP(addr[2], color.c32.rrr);
}

/*
=======================================
    像素绘制 (bpc = 4)
=======================================
*/
CR_API void_t
#ifndef CR_PIXEL_FLIP
CR_NAME_PIXEL(32z) (
#else
CR_NAME_PIXEL(32n) (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ cpix_t          color
    )
{
    int32u* addr;

#if (CR_BLT_MODE == CR_BLT_NOT)
    CR_NOUSE(color.val);
#endif
    addr = pixel_addr4(dst, x, CR_PIXEL_Y);
    CR_PIXEL_OP(addr[0], color.val);
}

#undef  CR_PIXEL_Y

#endif  /* CR_NAME_PIXEL */

/*****************************************************************************/
/*                                 填充模板                                  */
/*****************************************************************************/

/* 选择包含填充函数模板 */
#if defined(CR_NAME_FILLZ)

/* 08位色复制有特别的优化 */
#if (CR_BLT_MODE != CR_BLT_SET)
/*
=======================================
    填充绘制 (bpc = 1)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_SET)
fill_set08_c (
#elif   (CR_BLT_MODE == CR_BLT_AND)
fill_and08_c (
#elif   (CR_BLT_MODE == CR_BLT_ORR)
fill_orr08_c (
#elif   (CR_BLT_MODE == CR_BLT_NOT)
fill_not08_c (
#elif   (CR_BLT_MODE == CR_BLT_XOR)
fill_xor08_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sFILL*    fill,
  __CR_IN__ cpix_t          color,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  oper;

#if (CR_BLT_MODE == CR_BLT_NOT)
    CR_NOUSE(color.c08.idx);
#endif
    if (dst->bpc != 1 ||
        !blt_use_fill_clip(&oper, dst, 1, fill, rect))
        return;
    len = oper.width;
    while (oper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            CR_PIXEL_OP(oper.addr[0], color.c08.idx);
            oper.addr += 1;
        }
        oper.addr += oper.rest;
    }
}
#endif  /* (CR_BLT_MODE != CR_BLT_SET) */

/*
=======================================
    填充绘制 (bpc = 2)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_SET)
fill_set16_c (
#elif   (CR_BLT_MODE == CR_BLT_AND)
fill_and16_c (
#elif   (CR_BLT_MODE == CR_BLT_ORR)
fill_orr16_c (
#elif   (CR_BLT_MODE == CR_BLT_NOT)
fill_not16_c (
#elif   (CR_BLT_MODE == CR_BLT_XOR)
fill_xor16_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sFILL*    fill,
  __CR_IN__ cpix_t          color,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  oper;

#if (CR_BLT_MODE == CR_BLT_NOT)
    CR_NOUSE(color.c16.clr);
#endif
    if (dst->bpc != 2 ||
        !blt_use_fill_clip(&oper, dst, 2, fill, rect))
        return;
    len = oper.width / 2;
    while (oper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            CR_PIXEL_OP(*(int16u*)oper.addr, color.c16.clr);
            oper.addr += 2;
        }
        oper.addr += oper.rest;
    }
}

/*
=======================================
    填充绘制 (bpc = 3)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_SET)
fill_set24_c (
#elif   (CR_BLT_MODE == CR_BLT_AND)
fill_and24_c (
#elif   (CR_BLT_MODE == CR_BLT_ORR)
fill_orr24_c (
#elif   (CR_BLT_MODE == CR_BLT_NOT)
fill_not24_c (
#elif   (CR_BLT_MODE == CR_BLT_XOR)
fill_xor24_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sFILL*    fill,
  __CR_IN__ cpix_t          color,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  oper;

#if (CR_BLT_MODE == CR_BLT_NOT)
    CR_NOUSE(color.c32.bbb);
#endif
    if (dst->bpc != 3 ||
        !blt_use_fill_clip(&oper, dst, 3, fill, rect))
        return;
    len = oper.width / 3;
    while (oper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            CR_PIXEL_OP(oper.addr[0], color.c32.bbb);
            CR_PIXEL_OP(oper.addr[1], color.c32.ggg);
            CR_PIXEL_OP(oper.addr[2], color.c32.rrr);
            oper.addr += 3;
        }
        oper.addr += oper.rest;
    }
}

/*
=======================================
    填充绘制 (bpc = 4)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_SET)
fill_set32_c (
#elif   (CR_BLT_MODE == CR_BLT_AND)
fill_and32_c (
#elif   (CR_BLT_MODE == CR_BLT_ORR)
fill_orr32_c (
#elif   (CR_BLT_MODE == CR_BLT_NOT)
fill_not32_c (
#elif   (CR_BLT_MODE == CR_BLT_XOR)
fill_xor32_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sFILL*    fill,
  __CR_IN__ cpix_t          color,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  oper;

#if (CR_BLT_MODE == CR_BLT_NOT)
    CR_NOUSE(color.val);
#endif
    if (dst->bpc != 4 ||
        !blt_use_fill_clip(&oper, dst, 4, fill, rect))
        return;
    len = oper.width / 4;
    while (oper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            CR_PIXEL_OP(*(int32u*)oper.addr, color.val);
            oper.addr += 4;
        }
        oper.addr += oper.rest;
    }
}

#endif  /* CR_NAME_FILLZ */

/*****************************************************************************/
/*                                BLIT 模板                                  */
/*****************************************************************************/

/* 选择包含 BLIT 函数模板 */
#if defined(CR_NAME_BLITZ)

/*
=======================================
    BLIT 绘制 (bpc = 1)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_AND)
blit_and08_c (
#elif   (CR_BLT_MODE == CR_BLT_ORR)
blit_orr08_c (
#elif   (CR_BLT_MODE == CR_BLT_XOR)
blit_xor08_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 1 || src->bpc != 1 ||
        !blt_use_blit_clip(&doper, &soper, 1, 1, dst, src, blit, rect))
        return;
    len = soper.width;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            CR_PIXEL_OP(doper.addr[0], soper.addr[0]);
            doper.addr += 1;
            soper.addr += 1;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

/*
=======================================
    BLIT 绘制 (bpc = 2)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_AND)
blit_and16_c (
#elif   (CR_BLT_MODE == CR_BLT_ORR)
blit_orr16_c (
#elif   (CR_BLT_MODE == CR_BLT_XOR)
blit_xor16_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 2 || src->bpc != 2 ||
        !blt_use_blit_clip(&doper, &soper, 2, 2, dst, src, blit, rect))
        return;
    len = soper.width / 2;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            CR_PIXEL_OP(*(int16u*)doper.addr, *(int16u*)soper.addr);
            doper.addr += 2;
            soper.addr += 2;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

/*
=======================================
    BLIT 绘制 (bpc = 3)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_AND)
blit_and24_c (
#elif   (CR_BLT_MODE == CR_BLT_ORR)
blit_orr24_c (
#elif   (CR_BLT_MODE == CR_BLT_XOR)
blit_xor24_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 3 || src->bpc != 3 ||
        !blt_use_blit_clip(&doper, &soper, 3, 3, dst, src, blit, rect))
        return;
    len = soper.width / 3;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            CR_PIXEL_OP(doper.addr[0], soper.addr[0]);
            CR_PIXEL_OP(doper.addr[1], soper.addr[1]);
            CR_PIXEL_OP(doper.addr[2], soper.addr[2]);
            doper.addr += 3;
            soper.addr += 3;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

/*
=======================================
    BLIT 绘制 (bpc = 4)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_AND)
blit_and32_c (
#elif   (CR_BLT_MODE == CR_BLT_ORR)
blit_orr32_c (
#elif   (CR_BLT_MODE == CR_BLT_XOR)
blit_xor32_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 4 || src->bpc != 4 ||
        !blt_use_blit_clip(&doper, &soper, 4, 4, dst, src, blit, rect))
        return;
    len = soper.width / 4;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            CR_PIXEL_OP(*(int32u*)doper.addr, *(int32u*)soper.addr);
            doper.addr += 4;
            soper.addr += 4;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

#endif  /* CR_NAME_BLITZ */

#undef  CR_PIXEL_OP

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
