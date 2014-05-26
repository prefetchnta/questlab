/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-02-10  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 像素混合函数库模板 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 根据指定的宏选择操作符 */
#if (CR_BLT_MODE != CR_BLT_ADD) && \
    (CR_BLT_MODE != CR_BLT_SUB) && \
    (CR_BLT_MODE != CR_BLT_ALP)
    #error "blends.inl: invalid CR_BLT_MODE type!"
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
    像素混合 (fmt = 08)
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
    byte_t* addr = pixel_addr1(dst, x, CR_PIXEL_Y);

    /* 饱和加法 */
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT08
    CR_BLEND_ADD_MAIN08(addr[0], color.c08.idx)

    /* 饱和减法 */
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT08
    CR_BLEND_SUB_MAIN08(addr[0], color.c08.idx)

    /* 插值混合 */
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT08
    CR_BLEND_LRP_LOAD08(color.c08.lrp)
    CR_BLEND_LRP_MAIN08(addr[0], color.c08.idx)
#endif
}

/*
=======================================
    像素混合 (fmt = 12)
=======================================
*/
CR_API void_t
#ifndef CR_PIXEL_FLIP
CR_NAME_PIXEL(12z) (
#else
CR_NAME_PIXEL(12n) (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ cpix_t          color
    )
{
    int16u* addr = pixel_addr2(dst, x, CR_PIXEL_Y);

    /* 饱和加法 */
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT12
    CR_BLEND_ADD_MAIN12(addr[0], color.c16.clr)

    /* 饱和减法 */
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT12
    CR_BLEND_SUB_MAIN12(addr[0], color.c16.clr)

    /* 插值混合 */
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT12
    CR_BLEND_LRP_LOAD12(color.c16.lrp)
    CR_BLEND_LRP_MAIN12(addr[0], color.c16.clr)
#endif
}

/*
=======================================
    像素混合 (fmt = 15)
=======================================
*/
CR_API void_t
#ifndef CR_PIXEL_FLIP
CR_NAME_PIXEL(15z) (
#else
CR_NAME_PIXEL(15n) (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ cpix_t          color
    )
{
    int16u* addr = pixel_addr2(dst, x, CR_PIXEL_Y);

    /* 饱和加法 */
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT15
    CR_BLEND_ADD_MAIN15(addr[0], color.c16.clr)

    /* 饱和减法 */
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT15
    CR_BLEND_SUB_MAIN15(addr[0], color.c16.clr)

    /* 插值混合 */
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT15
    CR_BLEND_LRP_LOAD15(color.c16.lrp)
    CR_BLEND_LRP_MAIN15(addr[0], color.c16.clr)
#endif
}

/*
=======================================
    像素混合 (fmt = 16)
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
    int16u* addr = pixel_addr2(dst, x, CR_PIXEL_Y);

    /* 饱和加法 */
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT16
    CR_BLEND_ADD_MAIN16(addr[0], color.c16.clr)

    /* 饱和减法 */
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT16
    CR_BLEND_SUB_MAIN16(addr[0], color.c16.clr)

    /* 插值混合 */
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT16
    CR_BLEND_LRP_LOAD16(color.c16.lrp)
    CR_BLEND_LRP_MAIN16(addr[0], color.c16.clr)
#endif
}

/*
=======================================
    像素混合 (fmt = 24)
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
    byte_t* addr = pixel_addr3(dst, x, CR_PIXEL_Y);

    /* 饱和加法 */
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT24
    CR_BLEND_ADD_MAIN24(addr[0], addr[1], addr[2],
            color.c32.bbb, color.c32.ggg, color.c32.rrr)

    /* 饱和减法 */
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT24
    CR_BLEND_SUB_MAIN24(addr[0], addr[1], addr[2],
            color.c32.bbb, color.c32.ggg, color.c32.rrr)

    /* 插值混合 */
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT24
    CR_BLEND_LRP_LOAD24(color.c32.lrp)
    CR_BLEND_LRP_MAIN24(addr[0], addr[1], addr[2],
            color.c32.bbb, color.c32.ggg, color.c32.rrr)
#endif
}

/*
=======================================
    像素混合 (fmt = 32)
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
    byte_t* addr = (byte_t*)pixel_addr4(dst, x, CR_PIXEL_Y);

    /* 饱和加法 */
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT32
    CR_BLEND_ADD_MAIN32(addr[0], addr[1], addr[2], addr[3],
            color.c32.bbb, color.c32.ggg, color.c32.rrr, color.c32.lrp)

    /* 饱和减法 */
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT32
    CR_BLEND_SUB_MAIN32(addr[0], addr[1], addr[2], addr[3],
            color.c32.bbb, color.c32.ggg, color.c32.rrr, color.c32.lrp)

    /* 插值混合 */
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT32
    CR_BLEND_LRP_LOAD32(color.c32.lrp)
    CR_BLEND_LRP_MAIN32(addr[0], addr[1], addr[2],
            color.c32.bbb, color.c32.ggg, color.c32.rrr)
#endif
}

#undef  CR_PIXEL_Y

#endif  /* CR_NAME_PIXEL */

/*****************************************************************************/
/*                                 填充模板                                  */
/*****************************************************************************/

/* 选择包含填充函数模板 */
#if defined(CR_NAME_FILLZ)

/*
=======================================
    填充混合 (fmt = 08)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
fill_add08_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
fill_sub08_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
fill_lrp08_c (
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
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT08
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT08
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT08
#endif
    if (dst->bpc != 1 ||
        !blt_use_fill_clip(&oper, dst, 1, fill, rect))
        return;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD08(color.c08.lrp)
#endif
    len = oper.width;
    while (oper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
            CR_BLEND_ADD_MAIN08(oper.addr[0], color.c08.idx)
#elif   (CR_BLT_MODE == CR_BLT_SUB)
            CR_BLEND_SUB_MAIN08(oper.addr[0], color.c08.idx)
#elif   (CR_BLT_MODE == CR_BLT_ALP)
            CR_BLEND_LRP_MAIN08(oper.addr[0], color.c08.idx)
#endif
            oper.addr += 1;
        }
        oper.addr += oper.rest;
    }
}

/*
=======================================
    填充混合 (fmt = 12)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
fill_add12_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
fill_sub12_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
fill_lrp12_c (
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
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT12
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT12
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT12
#endif
    if (dst->bpc != 2 ||
        !blt_use_fill_clip(&oper, dst, 2, fill, rect))
        return;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD12(color.c16.lrp)
#endif
    len = oper.width / 2;
    while (oper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
            CR_BLEND_ADD_MAIN12(*(int16u*)oper.addr, color.c16.clr)
#elif   (CR_BLT_MODE == CR_BLT_SUB)
            CR_BLEND_SUB_MAIN12(*(int16u*)oper.addr, color.c16.clr)
#elif   (CR_BLT_MODE == CR_BLT_ALP)
            CR_BLEND_LRP_MAIN12(*(int16u*)oper.addr, color.c16.clr)
#endif
            oper.addr += 2;
        }
        oper.addr += oper.rest;
    }
}

/*
=======================================
    填充混合 (fmt = 15)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
fill_add15_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
fill_sub15_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
fill_lrp15_c (
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
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT15
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT15
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT15
#endif
    if (dst->bpc != 2 ||
        !blt_use_fill_clip(&oper, dst, 2, fill, rect))
        return;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD15(color.c16.lrp)
#endif
    len = oper.width / 2;
    while (oper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
            CR_BLEND_ADD_MAIN15(*(int16u*)oper.addr, color.c16.clr)
#elif   (CR_BLT_MODE == CR_BLT_SUB)
            CR_BLEND_SUB_MAIN15(*(int16u*)oper.addr, color.c16.clr)
#elif   (CR_BLT_MODE == CR_BLT_ALP)
            CR_BLEND_LRP_MAIN15(*(int16u*)oper.addr, color.c16.clr)
#endif
            oper.addr += 2;
        }
        oper.addr += oper.rest;
    }
}

/*
=======================================
    填充混合 (fmt = 16)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
fill_add16_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
fill_sub16_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
fill_lrp16_c (
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
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT16
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT16
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT16
#endif
    if (dst->bpc != 2 ||
        !blt_use_fill_clip(&oper, dst, 2, fill, rect))
        return;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD16(color.c16.lrp)
#endif
    len = oper.width / 2;
    while (oper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
            CR_BLEND_ADD_MAIN16(*(int16u*)oper.addr, color.c16.clr)
#elif   (CR_BLT_MODE == CR_BLT_SUB)
            CR_BLEND_SUB_MAIN16(*(int16u*)oper.addr, color.c16.clr)
#elif   (CR_BLT_MODE == CR_BLT_ALP)
            CR_BLEND_LRP_MAIN16(*(int16u*)oper.addr, color.c16.clr)
#endif
            oper.addr += 2;
        }
        oper.addr += oper.rest;
    }
}

/*
=======================================
    填充混合 (fmt = 24)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
fill_add24_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
fill_sub24_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
fill_lrp24_c (
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
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT24
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT24
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT24
#endif
    if (dst->bpc != 3 ||
        !blt_use_fill_clip(&oper, dst, 3, fill, rect))
        return;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD24(color.c32.lrp)
#endif
    len = oper.width / 3;
    while (oper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
            CR_BLEND_ADD_MAIN24(oper.addr[0], oper.addr[1], oper.addr[2],
                    color.c32.bbb, color.c32.ggg, color.c32.rrr)
#elif   (CR_BLT_MODE == CR_BLT_SUB)
            CR_BLEND_SUB_MAIN24(oper.addr[0], oper.addr[1], oper.addr[2],
                    color.c32.bbb, color.c32.ggg, color.c32.rrr)
#elif   (CR_BLT_MODE == CR_BLT_ALP)
            CR_BLEND_LRP_MAIN24(oper.addr[0], oper.addr[1], oper.addr[2],
                    color.c32.bbb, color.c32.ggg, color.c32.rrr)
#endif
            oper.addr += 3;
        }
        oper.addr += oper.rest;
    }
}

/*
=======================================
    填充混合 (fmt = 32)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
fill_add32_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
fill_sub32_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
fill_lrp32_c (
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
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT32
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT32
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT32
#endif
    if (dst->bpc != 4 ||
        !blt_use_fill_clip(&oper, dst, 4, fill, rect))
        return;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD32(color.c32.lrp)
#endif
    len = oper.width / 4;
    while (oper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
            CR_BLEND_ADD_MAIN32(oper.addr[0], oper.addr[1],
                    oper.addr[2], oper.addr[3], color.c32.bbb,
                    color.c32.ggg, color.c32.rrr, color.c32.lrp)
#elif   (CR_BLT_MODE == CR_BLT_SUB)
            CR_BLEND_SUB_MAIN32(oper.addr[0], oper.addr[1],
                    oper.addr[2], oper.addr[3], color.c32.bbb,
                    color.c32.ggg, color.c32.rrr, color.c32.lrp)
#elif   (CR_BLT_MODE == CR_BLT_ALP)
            CR_BLEND_LRP_MAIN32(oper.addr[0], oper.addr[1], oper.addr[2],
                    color.c32.bbb, color.c32.ggg, color.c32.rrr)
#endif
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
    BLIT 混合 (fmt = 08)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
blit_add08_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
blit_sub08_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
blit_lrp08_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
#if     (CR_BLT_MODE == CR_BLT_ALP)
  __CR_IN__ cpix_t          trans,
  __CR_IN__ bool_t          strb,
#endif
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT08
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT08
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT08
#endif
    if (dst->bpc != 1 || src->bpc != 1 ||
        !blt_use_blit_clip(&doper, &soper, 1, 1, dst, src, blit, rect))
        return;
    len = soper.width;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD08(trans.c08.lrp)
    if (!strb) {
#endif
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
                CR_BLEND_ADD_MAIN08(doper.addr[0], soper.addr[0])
#elif   (CR_BLT_MODE == CR_BLT_SUB)
                CR_BLEND_SUB_MAIN08(doper.addr[0], soper.addr[0])
#elif   (CR_BLT_MODE == CR_BLT_ALP)
                CR_BLEND_LRP_MAIN08(doper.addr[0], soper.addr[0])
#endif
                doper.addr += 1;
                soper.addr += 1;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
#if     (CR_BLT_MODE == CR_BLT_ALP)
    }
    else {
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                if (soper.addr[0] != trans.c08.idx)
                    CR_BLEND_LRP_MAIN08(doper.addr[0], soper.addr[0])
                doper.addr += 1;
                soper.addr += 1;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
    }
#endif
}

/*
=======================================
    BLIT 混合 (fmt = 12)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
blit_add12_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
blit_sub12_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
blit_lrp12_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
#if     (CR_BLT_MODE == CR_BLT_ALP)
  __CR_IN__ cpix_t          trans,
  __CR_IN__ bool_t          strb,
#endif
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT12
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT12
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT12
#endif
    if (dst->bpc != 2 || src->bpc != 2 ||
        !blt_use_blit_clip(&doper, &soper, 2, 2, dst, src, blit, rect))
        return;
    len = soper.width / 2;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD12(trans.c16.lrp)
    if (!strb) {
#endif
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
                CR_BLEND_ADD_MAIN12(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
#elif   (CR_BLT_MODE == CR_BLT_SUB)
                CR_BLEND_SUB_MAIN12(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
#elif   (CR_BLT_MODE == CR_BLT_ALP)
                CR_BLEND_LRP_MAIN12(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
#endif
                doper.addr += 2;
                soper.addr += 2;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
#if     (CR_BLT_MODE == CR_BLT_ALP)
    }
    else {
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                if (*(int16u*)soper.addr != trans.c16.clr)
                    CR_BLEND_LRP_MAIN12(*(int16u*)doper.addr,
                                        *(int16u*)soper.addr)
                doper.addr += 2;
                soper.addr += 2;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
    }
#endif
}

/*
=======================================
    BLIT 混合 (fmt = 15)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
blit_add15_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
blit_sub15_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
blit_lrp15_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
#if     (CR_BLT_MODE == CR_BLT_ALP)
  __CR_IN__ cpix_t          trans,
  __CR_IN__ bool_t          strb,
#endif
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT15
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT15
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT15
#endif
    if (dst->bpc != 2 || src->bpc != 2 ||
        !blt_use_blit_clip(&doper, &soper, 2, 2, dst, src, blit, rect))
        return;
    len = soper.width / 2;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD15(trans.c16.lrp)
    if (!strb) {
#endif
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
                CR_BLEND_ADD_MAIN15(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
#elif   (CR_BLT_MODE == CR_BLT_SUB)
                CR_BLEND_SUB_MAIN15(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
#elif   (CR_BLT_MODE == CR_BLT_ALP)
                CR_BLEND_LRP_MAIN15(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
#endif
                doper.addr += 2;
                soper.addr += 2;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
#if     (CR_BLT_MODE == CR_BLT_ALP)
    }
    else {
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                if (*(int16u*)soper.addr != trans.c16.clr)
                    CR_BLEND_LRP_MAIN15(*(int16u*)doper.addr,
                                        *(int16u*)soper.addr)
                doper.addr += 2;
                soper.addr += 2;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
    }
#endif
}

/*
=======================================
    BLIT 混合 (fmt = 16)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
blit_add16_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
blit_sub16_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
blit_lrp16_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
#if     (CR_BLT_MODE == CR_BLT_ALP)
  __CR_IN__ cpix_t          trans,
  __CR_IN__ bool_t          strb,
#endif
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT16
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT16
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT16
#endif
    if (dst->bpc != 2 || src->bpc != 2 ||
        !blt_use_blit_clip(&doper, &soper, 2, 2, dst, src, blit, rect))
        return;
    len = soper.width / 2;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD16(trans.c16.lrp)
    if (!strb) {
#endif
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
                CR_BLEND_ADD_MAIN16(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
#elif   (CR_BLT_MODE == CR_BLT_SUB)
                CR_BLEND_SUB_MAIN16(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
#elif   (CR_BLT_MODE == CR_BLT_ALP)
                CR_BLEND_LRP_MAIN16(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
#endif
                doper.addr += 2;
                soper.addr += 2;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
#if     (CR_BLT_MODE == CR_BLT_ALP)
    }
    else {
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                if (*(int16u*)soper.addr != trans.c16.clr)
                    CR_BLEND_LRP_MAIN16(*(int16u*)doper.addr,
                                        *(int16u*)soper.addr)
                doper.addr += 2;
                soper.addr += 2;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
    }
#endif
}

/*
=======================================
    BLIT 混合 (fmt = 24)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
blit_add24_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
blit_sub24_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
blit_lrp24_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
#if     (CR_BLT_MODE == CR_BLT_ALP)
  __CR_IN__ cpix_t          trans,
  __CR_IN__ bool_t          strb,
#endif
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT24
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT24
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT24
#endif
    if (dst->bpc != 3 || src->bpc != 3 ||
        !blt_use_blit_clip(&doper, &soper, 3, 3, dst, src, blit, rect))
        return;
    len = soper.width / 3;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD24(trans.c32.lrp)
    if (!strb) {
#endif
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
                CR_BLEND_ADD_MAIN24(doper.addr[0], doper.addr[1],
                        doper.addr[2], soper.addr[0], soper.addr[1],
                                    soper.addr[2])
#elif   (CR_BLT_MODE == CR_BLT_SUB)
                CR_BLEND_SUB_MAIN24(doper.addr[0], doper.addr[1],
                        doper.addr[2], soper.addr[0], soper.addr[1],
                                    soper.addr[2])
#elif   (CR_BLT_MODE == CR_BLT_ALP)
                CR_BLEND_LRP_MAIN24(doper.addr[0], doper.addr[1],
                        doper.addr[2], soper.addr[0], soper.addr[1],
                                    soper.addr[2])
#endif
                doper.addr += 3;
                soper.addr += 3;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
#if     (CR_BLT_MODE == CR_BLT_ALP)
    }
    else {
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                if (soper.addr[0] != trans.c32.bbb &&
                    soper.addr[1] != trans.c32.ggg &&
                    soper.addr[2] != trans.c32.rrr)
                    CR_BLEND_LRP_MAIN24(doper.addr[0], doper.addr[1],
                            doper.addr[2], soper.addr[0], soper.addr[1],
                                        soper.addr[2])
                doper.addr += 3;
                soper.addr += 3;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
    }
#endif
}

/*
=======================================
    BLIT 混合 (fmt = 32)
=======================================
*/
CR_API void_t
#if     (CR_BLT_MODE == CR_BLT_ADD)
blit_add32_c (
#elif   (CR_BLT_MODE == CR_BLT_SUB)
blit_sub32_c (
#elif   (CR_BLT_MODE == CR_BLT_ALP)
blit_lrp32_c (
#endif
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
#if     (CR_BLT_MODE == CR_BLT_ALP)
  __CR_IN__ cpix_t          trans,
  __CR_IN__ bool_t          strb,
#endif
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
#if     (CR_BLT_MODE == CR_BLT_ADD)
    CR_BLEND_ADD_INIT32
#elif   (CR_BLT_MODE == CR_BLT_SUB)
    CR_BLEND_SUB_INIT32
#elif   (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_INIT32
#endif
    if (dst->bpc != 4 || src->bpc != 4 ||
        !blt_use_blit_clip(&doper, &soper, 4, 4, dst, src, blit, rect))
        return;
    len = soper.width / 4;
#if     (CR_BLT_MODE == CR_BLT_ALP)
    CR_BLEND_LRP_LOAD32(trans.c32.lrp)
    if (!strb) {
#endif
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
#if     (CR_BLT_MODE == CR_BLT_ADD)
                CR_BLEND_ADD_MAIN32(doper.addr[0], doper.addr[1],
                        doper.addr[2], doper.addr[3], soper.addr[0],
                        soper.addr[1], soper.addr[2], soper.addr[3])
#elif   (CR_BLT_MODE == CR_BLT_SUB)
                CR_BLEND_SUB_MAIN32(doper.addr[0], doper.addr[1],
                        doper.addr[2], doper.addr[3], soper.addr[0],
                        soper.addr[1], soper.addr[2], soper.addr[3])
#elif   (CR_BLT_MODE == CR_BLT_ALP)
                CR_BLEND_LRP_MAIN32(doper.addr[0], doper.addr[1],
                        doper.addr[2], soper.addr[0], soper.addr[1],
                                    soper.addr[2])
#endif
                doper.addr += 4;
                soper.addr += 4;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
#if     (CR_BLT_MODE == CR_BLT_ALP)
    }
    else {
        trans.val &= CDWORD_LE(0x00FFFFFFUL);
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                if ((*(int32u*)soper.addr & CDWORD_LE(0x00FFFFFFUL)) !=
                               trans.val)
                    CR_BLEND_LRP_MAIN32(doper.addr[0], doper.addr[1],
                            doper.addr[2], soper.addr[0], soper.addr[1],
                                        soper.addr[2])
                doper.addr += 4;
                soper.addr += 4;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
    }
#endif
}

#endif  /* CR_NAME_BLITZ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
