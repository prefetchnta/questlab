/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-05-07  */
/*     #######          ###    ###      [PORT]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-02-19  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 类型转换头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_CNVRT_H__
#define __CR_CNVRT_H__ 0x9D2605DDUL

/*
=======================================
    获取内存大小高半位
=======================================
*/
inline uint_t
size_hi (
  __CR_IN__ leng_t  size
    )
{
#if defined(_CR_SYS32_) || \
    defined(_CR_SMALL_)
    CR_NOUSE(size);
    return ((uint_t)0x0000);
#else   /* (_CR_SYS64_) */
        /* (_CR_LARGE_) */
    return ((uint_t)(size >> bitsof(uint_t)));
#endif
}

/*
=======================================
    获取文件大小高32位
=======================================
*/
inline int32u
fsize_hi (
  __CR_IN__ fsize_t size
    )
{
#if defined(_CR_FILE32_)
    CR_NOUSE(size);
    return ((int32u)0x00UL);
#else   /* (_CR_FILE64_) */
    return ((int32u)(size >> bitsof(int32u)));
#endif
}

/*
=======================================
    32位数合成64位数
=======================================
*/
inline int64u
mk_size (
  __CR_IN__ int32u  hi,
  __CR_IN__ int32u  lo
    )
{
#ifdef  _CR_NO_INT64_
    int64u  tmp;

    tmp.lo32 = lo;
    tmp.hi32 = hi;
    return (tmp);
#else
    return (((int64u)hi << bitsof(int32u)) | lo);
#endif
}

/*
=======================================
    短整数合成大整数
=======================================
*/
inline maxu_t
mk_maxu (
  __CR_IN__ uint_t  hi,
  __CR_IN__ uint_t  lo
    )
{
    return (((maxu_t)hi << bitsof(uint_t)) | lo);
}

#ifndef _CR_NO_FLOAT_

/*
=======================================
    32整数转单精度浮点数
=======================================
*/
inline fp32_t
cvt_i2f (
  __CR_IN__ int32s  i
    )
{
    union {
        fp32_t  f;
        int32s  i;
    } tmp;

    tmp.i = i;
    return (tmp.f);
}

/*
=======================================
    单精度浮点数转32整数
=======================================
*/
inline int32s
cvt_f2i (
  __CR_IN__ fp32_t  f
    )
{
    union {
        fp32_t  f;
        int32s  i;
    } tmp;

    tmp.f = f;
    return (tmp.i);
}

/*
=======================================
    64整数转双精度浮点数
=======================================
*/
inline fp64_t
cvt_i2d (
  __CR_IN__ int64s  i
    )
{
    union {
        fp64_t  d;
        int64s  i;
    } tmp;

    tmp.i = i;
    return (tmp.d);
}

/*
=======================================
    双精度浮点数转64整数
=======================================
*/
inline int64s
cvt_d2i (
  __CR_IN__ fp64_t  d
    )
{
    union {
        fp64_t  d;
        int64s  i;
    } tmp;

    tmp.d = d;
    return (tmp.i);
}

#endif  /* !_CR_NO_FLOAT_ */

/*
=======================================
    字符串转标志 (2B)
=======================================
*/
inline int16u
mk_tag2 (
  __CR_IN__ const ansi_t*   str
    )
{
    return (*(int16u*)str);
}

/*
=======================================
    字符串转标志 (4B)
=======================================
*/
inline int32u
mk_tag4 (
  __CR_IN__ const ansi_t*   str
    )
{
    return (*(int32u*)str);
}

/*
=======================================
    字符串转标志 (8B)
=======================================
*/
inline int64u
mk_tag8 (
  __CR_IN__ const ansi_t*   str
    )
{
    return (*(int64u*)str);
}

#endif  /* !__CR_CNVRT_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
