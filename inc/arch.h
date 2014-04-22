/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-12-26  */
/*     #######          ###    ###      [PORT]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 架构优化头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_ARCH_H__
#define __CR_ARCH_H__

/*****************************************************************************/
/*                               封闭循环优化                                */
/*****************************************************************************/

/* ADI 的 DSP 优化 */
#if defined(_CR_CC_VDSP_)

    /* 循环处理封闭索引 (单向) */
    #define circle_idx_u(index, incr, nitems) \
                __builtin_circindex(index, incr, nitems)

    /* 循环处理封闭索引 (双向) */
    #define circle_idx_s(index, incr, nitems) \
                __builtin_circindex(index, incr, nitems)

    /* 循环处理封闭指针 (单向) */
    #define circle_ptr_u(ptr, incr, base, buflen) \
                __builtin_circptr(ptr, incr, base, buflen)

    /* 循环处理封闭指针 (双向) */
    #define circle_ptr_s(ptr, incr, base, buflen) \
                __builtin_circptr(ptr, incr, base, buflen)
#else
#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    循环处理封闭索引 (单向)
=======================================
*/
cr_inline leng_t
circle_idx_u (
  __CR_IN__ leng_t  index,
  __CR_IN__ leng_t  incr,
  __CR_IN__ leng_t  nitems
    )
{
    index += incr;
    if (index >= nitems)
        index -= nitems;
    return (index);
}

/*
=======================================
    循环处理封闭索引 (双向)
=======================================
*/
cr_inline dist_t
circle_idx_s (
  __CR_IN__ dist_t  index,
  __CR_IN__ dist_t  incr,
  __CR_IN__ leng_t  nitems
    )
{
    index += incr;
    if (index < 0)
        index += nitems;
    else
    if ((leng_t)index >= nitems)
        index -= nitems;
    return (index);
}

/*
=======================================
    循环处理封闭指针 (单向)
=======================================
*/
cr_inline void_t*
circle_ptr_u (
  __CR_IN__ const void_t*   ptr,
  __CR_IN__ leng_t          incr,
  __CR_IN__ const void_t*   base,
  __CR_IN__ leng_t          buflen
    )
{
    ptr = (void_t*)((uchar*)ptr + incr);
    if (ptr >= (void_t*)((uchar*)base + buflen))
        ptr = (void_t*)((uchar*)ptr - buflen);
    return ((void_t*)ptr);
}

/*
=======================================
    循环处理封闭指针 (双向)
=======================================
*/
cr_inline void_t*
circle_ptr_s (
  __CR_IN__ const void_t*   ptr,
  __CR_IN__ dist_t          incr,
  __CR_IN__ const void_t*   base,
  __CR_IN__ leng_t          buflen
    )
{
    ptr = (void_t*)((uchar*)ptr + incr);
    if (ptr < base)
        ptr = (void_t*)((uchar*)ptr + buflen);
    else
    if (ptr >= (void_t*)((uchar*)base + buflen))
        ptr = (void_t*)((uchar*)ptr - buflen);
    return ((void_t*)ptr);
}

#endif  /* !_CR_SICK_INLINE_ */

#endif  /* CC TYPE predefines */

#endif  /* !__CR_ARCH_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
