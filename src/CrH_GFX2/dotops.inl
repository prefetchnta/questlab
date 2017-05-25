/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-05-24  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 像素处理函数库模板 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 根据指定的宏选择起始坐标 */
#ifndef CR_PIXEL_FLIP
    #define CR_PIXEL_Y  (y)
#else
    #define CR_PIXEL_Y  (dst->clip_win.y2 - y)
#endif

/*****************************************************************************/
/*                                 像素读取                                  */
/*****************************************************************************/

/*
=======================================
    像素读取 (idx = 1)
=======================================
*/
CR_API byte_t
#ifndef CR_PIXEL_FLIP
pixel_get01z (
#else
pixel_get01n (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y
    )
{
    byte_t* addr;

    addr = pixel_addr1(dst, ((uint_t)x) / 8, CR_PIXEL_Y);
    return ((addr[0] >> (7 - (((uint_t)x) % 8))) & 1);
}

/*
=======================================
    像素读取 (idx = 2)
=======================================
*/
CR_API byte_t
#ifndef CR_PIXEL_FLIP
pixel_get02z (
#else
pixel_get02n (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y
    )
{
    byte_t* addr;

    addr = pixel_addr1(dst, ((uint_t)x) / 4, CR_PIXEL_Y);
    return ((addr[0] >> (6 - (((uint_t)x) % 4) * 2)) & 3);
}

/*
=======================================
    像素读取 (idx = 4)
=======================================
*/
CR_API byte_t
#ifndef CR_PIXEL_FLIP
pixel_get04z (
#else
pixel_get04n (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y
    )
{
    byte_t* addr;

    addr = pixel_addr1(dst, ((uint_t)x) / 2, CR_PIXEL_Y);
    return ((addr[0] >> (4 - (((uint_t)x) % 2) * 4)) & 15);
}

/*
=======================================
    像素读取 (bpc = 1)
=======================================
*/
CR_API byte_t
#ifndef CR_PIXEL_FLIP
pixel_get08z (
#else
pixel_get08n (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y
    )
{
    byte_t* addr;

    addr = pixel_addr1(dst, x, CR_PIXEL_Y);
    return (addr[0]);
}

/*
=======================================
    像素读取 (bpc = 2)
=======================================
*/
CR_API int16u
#ifndef CR_PIXEL_FLIP
pixel_get16z (
#else
pixel_get16n (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y
    )
{
    int16u* addr;

    addr = pixel_addr2(dst, x, CR_PIXEL_Y);
    return (addr[0]);
}

/*
=======================================
    像素读取 (bpc = 3)
=======================================
*/
CR_API int32u
#ifndef CR_PIXEL_FLIP
pixel_get24z (
#else
pixel_get24n (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y
    )
{
    cpix_t  dots;
    byte_t* addr;

    addr = pixel_addr3(dst, x, CR_PIXEL_Y);
    dots.c32.bbb = addr[0];
    dots.c32.ggg = addr[1];
    dots.c32.rrr = addr[2];
    dots.c32.lrp = 0xFF;
    return (dots.val);
}

/*
=======================================
    像素读取 (bpc = 4)
=======================================
*/
CR_API int32u
#ifndef CR_PIXEL_FLIP
pixel_get32z (
#else
pixel_get32n (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y
    )
{
    int32u* addr;

    addr = pixel_addr4(dst, x, CR_PIXEL_Y);
    return (addr[0]);
}

/*****************************************************************************/
/*                                 像素绘制                                  */
/*****************************************************************************/

/*
=======================================
    像素绘制 (idx = 1)
=======================================
*/
CR_API void_t
#ifndef CR_PIXEL_FLIP
pixel_set01z (
#else
pixel_set01n (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ cpix_t          color
    )
{
    uint_t  offs;
    byte_t* addr;

    addr = pixel_addr1(dst, ((uint_t)x) / 8, CR_PIXEL_Y);
    offs = (uint_t)(7 - (((uint_t)x) % 8));
    addr[0] &= ~(1 << offs);
    addr[0] |= ((color.c08.idx & 1) << offs);
}

/*
=======================================
    像素绘制 (idx = 2)
=======================================
*/
CR_API void_t
#ifndef CR_PIXEL_FLIP
pixel_set02z (
#else
pixel_set02n (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ cpix_t          color
    )
{
    uint_t  offs;
    byte_t* addr;

    addr = pixel_addr1(dst, ((uint_t)x) / 4, CR_PIXEL_Y);
    offs = (uint_t)(6 - (((uint_t)x) % 4) * 2);
    addr[0] &= ~(3 << offs);
    addr[0] |= ((color.c08.idx & 3) << offs);
}

/*
=======================================
    像素绘制 (idx = 4)
=======================================
*/
CR_API void_t
#ifndef CR_PIXEL_FLIP
pixel_set04z (
#else
pixel_set04n (
#endif
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ cpix_t          color
    )
{
    uint_t  offs;
    byte_t* addr;

    addr = pixel_addr1(dst, ((uint_t)x) / 2, CR_PIXEL_Y);
    offs = (uint_t)(4 - (((uint_t)x) % 2) * 4);
    addr[0] &= ~(15 << offs);
    addr[0] |= ((color.c08.idx & 15) << offs);
}

#undef  CR_PIXEL_Y

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
