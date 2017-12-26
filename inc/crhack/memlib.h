/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-16  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 内存操作头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_MEMLIB_H__
#define __CR_MEMLIB_H__

#include "defs.h"

#ifndef _CR_NO_STDC_
    #include <string.h>
#endif

/*****************************************************************************/
/*                               内存分配释放                                */
/*****************************************************************************/

/* 分配释放函数类型 */
typedef void_t* (*cr_malloc_t) (size_t);
typedef void_t* (*cr_calloc_t) (size_t, size_t);
typedef void_t  (*cr_free_t) (void_t*);
typedef void_t* (*cr_realloc_t) (void_t*, size_t);

CR_API void_t   mem_reset (void_t);
CR_API void_t   mem_init (cr_malloc_t alloc, cr_free_t mfree);
CR_API void_t*  mem_size (const void_t *ptr, leng_t *size);
CR_API void_t   mem_info (msize_t *total, msize_t *avail);
CR_API void_t   mem_count (dist_t *size, leng_t *maxs,
                           leng_t *tots, sint_t *count);
#if !defined(ASY_NO_CRHACK)
CR_API void_t   mem_free (const void_t *ptr);
CR_API void_t*  mem_malloc (leng_t size);
CR_API void_t*  mem_calloc (leng_t num, leng_t size);
CR_API void_t*  mem_malloc32 (int32u size);
CR_API void_t*  mem_malloc64 (int64u size);
CR_API void_t*  mem_calloc32 (int32u num, leng_t size);
CR_API void_t*  mem_calloc64 (int64u num, leng_t size);
CR_API void_t*  mem_realloc (void_t *ptr, leng_t new_size);
#endif

/* 字符串的分配宏 */
#define str_allocA(len) (ansi_t*)mem_malloc(len)
#define str_allocW(len)  mem_talloc(len, wide_t)

/* 结构体的分配宏 */
#define struct_new(t)   (t*)mem_malloc(sizeof(t))

/* 特定类型分配宏 */
#define mem_talloc(s,t)     (t*)mem_calloc(s, sizeof(t))
#define mem_talloc32(s,t)   (t*)mem_calloc32(s, sizeof(t))
#define mem_talloc64(s,t)   (t*)mem_calloc64(s, sizeof(t))

/*****************************************************************************/
/*                               内存基本操作                                */
/*****************************************************************************/

/* 使用 LIBC 内存操作 (默认) */
#ifndef _CR_NO_STDC_
    #define mem_set     memset
    #define mem_cpy     memcpy
    #define mem_cmp     memcmp
    #define mem_mov     memmove
#endif

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    内存填充
=======================================
*/
#ifndef mem_set
cr_inline void_t*
mem_set (
  __CR_OT__ void_t* dst,
  __CR_IN__ byte_t  val,
  __CR_IN__ leng_t  len
    )
{
    byte_t* ptr = (byte_t*)dst;

    for (; len != 0; len--)
        *ptr++ = val;
    return (dst);
}
#endif  /* !mem_set() */

/*
=======================================
    内存复制
=======================================
*/
#ifndef mem_cpy
cr_inline void_t*
mem_cpy (
  __CR_OT__ void_t*         dst,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          len
    )
{
    byte_t* ptr = (byte_t*)dst;
    byte_t* dat = (byte_t*)src;

    for (; len != 0; len--)
        *ptr++ = *dat++;
    return (dst);
}
#endif  /* !mem_cpy() */

/*
=======================================
    内存比较
=======================================
*/
#ifndef mem_cmp
cr_inline sint_t
mem_cmp (
  __CR_IN__ const void_t*   dst,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          len
    )
{
    ansi_t  dv, sv;

    for (; len != 0; len--)
    {
        dv = *(ansi_t*)dst;
        sv = *(ansi_t*)src;
        if (dv != sv)
            return ((sint_t)(dv - sv));

        dst = (uchar*)dst + 1;
        src = (uchar*)src + 1;
    }
    return ((sint_t)0x0000);
}
#endif  /* !mem_cmp() */

/*
=======================================
    内存移动
=======================================
*/
#ifndef mem_mov
cr_inline void_t*
mem_mov (
  __CR_OT__ void_t*         dst,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          len
    )
{
    void_t* ret = dst;

    if (dst <= src ||
       (uchar*)dst >= (uchar*)src + len)
        return (mem_cpy(dst, src, len));

    dst = (uchar*)dst + len - 1;
    src = (uchar*)src + len - 1;

    for (; len != 0; len--)
    {
        *(uchar*)dst = *(uchar*)src;
        dst = (uchar*)dst - 1;
        src = (uchar*)src - 1;
    }
    return (ret);
}
#endif  /* !mem_mov() */

/*
=======================================
    单字填充
=======================================
*/
#ifndef mem_setw
cr_inline void_t*
mem_setw (
  __CR_OT__ void_t* dst,
  __CR_IN__ int16u  val,
  __CR_IN__ leng_t  len
    )
{
    int16u* ptr = (int16u*)dst;

    for (; len != 0; len--)
        *ptr++ = val;
    return (dst);
}
#endif  /* !mem_setw() */

/*
=======================================
    双字填充
=======================================
*/
#ifndef mem_setd
cr_inline void_t*
mem_setd (
  __CR_OT__ void_t* dst,
  __CR_IN__ int32u  val,
  __CR_IN__ leng_t  len
    )
{
    int32u* ptr = (int32u*)dst;

    for (; len != 0; len--)
        *ptr++ = val;
    return (dst);
}
#endif  /* !mem_setd() */

/*
=======================================
    四字填充
=======================================
*/
#ifndef mem_setq
cr_inline void_t*
mem_setq (
  __CR_OT__ void_t* dst,
  __CR_IN__ int64u  val,
  __CR_IN__ leng_t  len
    )
{
    int64u* ptr = (int64u*)dst;

    for (; len != 0; len--)
    {
#ifndef _CR_NO_INT64_
        *ptr++ = val;
#else
        ptr->lo32 = val.lo32;
        ptr->hi32 = val.hi32;
        ptr++;
#endif
    }
    return (dst);
}
#endif  /* !mem_setq() */

/*
=======================================
    三字节填充
=======================================
*/
#ifndef mem_sett
cr_inline void_t*
mem_sett (
  __CR_OT__ void_t*         dst,
  __CR_IN__ const byte_t    val[3],
  __CR_IN__ leng_t          len
    )
{
    byte_t* ptr = (byte_t*)dst;

    for (; len != 0; len--)
    {
        *ptr++ = val[0];
        *ptr++ = val[1];
        *ptr++ = val[2];
    }
    return (dst);
}
#endif  /* !mem_sett() */

#endif  /* !_CR_SICK_INLINE_ */

/* 结构体的基本操作宏 */
#define  struct_cpy(d,s,t)      mem_cpy(d, s, sizeof(t))
#define _struct_cpy(d,s,t)  (t*)mem_cpy(d, s, sizeof(t))
#define  struct_zero(d, t)      mem_zero(  d, sizeof(t))
#define _struct_zero(d, t)  (t*)mem_zero(  d, sizeof(t))

/* 字符串的基本操作宏 */
#define  chr_cpyA(d, s, n)          mem_cpy(d, s, (n) * sizeof(ansi_t))
#define _chr_cpyA(d, s, n)  (achar*)mem_cpy(d, s, (n) * sizeof(ansi_t))
#define  chr_cmpA(d, s, n)  (sint_t)mem_cmp(d, s, (n) * sizeof(ansi_t))
#define  chr_cpyW(d, s, n)          mem_cpy(d, s, (n) * sizeof(wide_t))
#define _chr_cpyW(d, s, n)  (wchar*)mem_cpy(d, s, (n) * sizeof(wide_t))
#define  chr_cmpW(d, s, n)  (sint_t)mem_cmp(d, s, (n) * sizeof(wide_t))

/* 结构数组清除操作宏 */
#define mem_zero(d, n)      mem_set(d, 0, n)
#define mem_tzero(d, n, t)  mem_czero(d, n, sizeof(t))
#define mem_czero(d, n, s)  mem_zero(d, (leng_t)(n) * (leng_t)(s))

/*****************************************************************************/
/*                               内存扩展操作                                */
/*****************************************************************************/

CR_API void_t*  mem_dup (const void_t *src, leng_t len);
CR_API sint_t   mem_cmp2 (const void_t *dst, const void_t *src, leng_t len);

/* 结构体的扩展操作宏 */
#define struct_dup(s, t)    (t*)mem_dup(s, sizeof(t))

/* 字符串的扩展操作宏 */
#define str_dupA(s) (ansi_t*)mem_dup(s, str_sizeA(s))
#define str_dupW(s) (wide_t*)mem_dup(s, str_sizeW(s))

/* 内存数据多段替换 */
CR_API void_t*  mem_replace (const void_t *data, leng_t srclen, leng_t *dstlen,
                 leng_t count, const leng_t offsets[], const leng_t blocks[],
                               const void_t *rplcs[], const leng_t rp_sizes[]);
/* 非对齐数据操作 */
#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    读取非对齐单字
=======================================
*/
cr_inline int16u
load_misali16 (
  __CR_IN__ const void_t*   ptr
    )
{
#ifndef _CR_ALIGN_NEEDED_
    return (*(int16u*)ptr);
#else
    #if defined(_CR_CC_VDSP_) && \
        defined(_CR_AR_ADSPBF_)
    return (misaligned_load16((void_t*)ptr));
    #else
    int16u  val;

    mem_cpy(&val, ptr, sizeof(int16u));
    return ((int16u)val);
    #endif
#endif
}

/*
=======================================
    读取非对齐双字
=======================================
*/
cr_inline int32u
load_misali32 (
  __CR_IN__ const void_t*   ptr
    )
{
#ifndef _CR_ALIGN_NEEDED_
    return (*(int32u*)ptr);
#else
    #if defined(_CR_CC_VDSP_) && \
        defined(_CR_AR_ADSPBF_)
    return (misaligned_load32((void_t*)ptr));
    #else
    int32u  val;

    mem_cpy(&val, ptr, sizeof(int32u));
    return ((int32u)val);
    #endif
#endif
}

/*
=======================================
    读取非对齐四字
=======================================
*/
cr_inline int64u
load_misali64 (
  __CR_IN__ const void_t*   ptr
    )
{
#ifndef _CR_ALIGN_NEEDED_
    return (*(int64u*)ptr);
#else
    #if defined(_CR_CC_VDSP_) && \
        defined(_CR_AR_ADSPBF_)
    return (misaligned_load64((void_t*)ptr));
    #else
    int64u  val;

    mem_cpy(&val, ptr, sizeof(int64u));
    return ((int64u)val);
    #endif
#endif
}

/*
=======================================
    写入非对齐单字
=======================================
*/
cr_inline void_t
save_misali16 (
  __CR_OT__ void_t* ptr,
  __CR_IN__ int16u  val
    )
{
#ifndef _CR_ALIGN_NEEDED_
    *(int16u*)ptr = (int16u)val;
#else
    #if defined(_CR_CC_VDSP_) && \
        defined(_CR_AR_ADSPBF_)
    misaligned_store16(ptr, val);
    #else
    mem_cpy(ptr, &val, sizeof(int16u));
    #endif
#endif
}

/*
=======================================
    写入非对齐双字
=======================================
*/
cr_inline void_t
save_misali32 (
  __CR_OT__ void_t* ptr,
  __CR_IN__ int32u  val
    )
{
#ifndef _CR_ALIGN_NEEDED_
    *(int32u*)ptr = (int32u)val;
#else
    #if defined(_CR_CC_VDSP_) && \
        defined(_CR_AR_ADSPBF_)
    misaligned_store32(ptr, val);
    #else
    mem_cpy(ptr, &val, sizeof(int32u));
    #endif
#endif
}

/*
=======================================
    写入非对齐四字
=======================================
*/
cr_inline void_t
save_misali64 (
  __CR_OT__ void_t* ptr,
  __CR_IN__ int64u  val
    )
{
#ifndef _CR_ALIGN_NEEDED_
    *(int64u*)ptr = (int64u)val;
#else
    #if defined(_CR_CC_VDSP_) && \
        defined(_CR_AR_ADSPBF_)
    misaligned_store64(ptr, val);
    #else
    mem_cpy(ptr, &val, sizeof(int64u));
    #endif
#endif
}

#endif  /* !_CR_SICK_INLINE_ */

/* 数据包结构 */
typedef struct
{
        /* 小于0需要释放 */
        dist_t  size;   /* 数据大小 */
        void_t* data;   /* 数据缓冲 */

} sBUFFER;

/* 数据包操作 */
CR_API bool_t   buffer_init (sBUFFER *buff, const void_t *data,
                             leng_t size, bool_t is_free);
CR_API void_t   buffer_free (const sBUFFER *buff);
CR_API leng_t   buffer_size (const sBUFFER *buff);
CR_API ansi_t*  buffer_load_as_str (const sBUFFER *buff);
CR_API void_t*  buffer_load_as_bin (const sBUFFER *buff, leng_t *size);

/*****************************************************************************/
/*                               字符基本操作                                */
/*****************************************************************************/

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    返回字符串长度A
=======================================
*/
cr_inline leng_t
str_lenA (
  __CR_IN__ const ansi_t*   str
    )
{
#if !defined(_CR_LARGE_) && \
    !defined(_CR_NO_STDC_)
    return ((leng_t)strlen((char*)str));
#else
    const ansi_t*   eos = str;

    while (*eos != CR_AC(NIL))
        eos++;
    return ((leng_t)(eos - str));
#endif
}

/*
=======================================
    返回字符串长度W
=======================================
*/
cr_inline leng_t
str_lenW (
  __CR_IN__ const wide_t*   str
    )
{
#if !defined(_CR_LARGE_) && \
     defined(_CR_HAVE_WCS_)
    return ((leng_t)wcslen((wchar_t*)str));
#else
    const wide_t*   eos = str;

    while (*eos != CR_NC(NIL))
        eos++;
    return ((leng_t)(eos - str));
#endif
}

/*
=======================================
    返回字符串大小A
=======================================
*/
cr_inline leng_t
str_sizeA (
  __CR_IN__ const ansi_t*   str
    )
{
    return ((str_lenA(str) + 1) * sizeof(ansi_t));
}

/*
=======================================
    返回字符串大小W
=======================================
*/
cr_inline leng_t
str_sizeW (
  __CR_IN__ const wide_t*   str
    )
{
    return ((str_lenW(str) + 1) * sizeof(wide_t));
}

/*
=======================================
    返回字符串限长长度A
=======================================
*/
cr_inline leng_t
str_lenNA (
  __CR_IN__ const ansi_t*   str,
  __CR_IN__ leng_t          len
    )
{
    const ansi_t*   eos = str;

    while (len != 0 && *eos != CR_AC(NIL))
        len--, eos++;

    return ((leng_t)(eos - str));
}

/*
=======================================
    返回字符串限长长度W
=======================================
*/
cr_inline leng_t
str_lenNW (
  __CR_IN__ const wide_t*   str,
  __CR_IN__ leng_t          len
    )
{
    const wide_t*   eos = str;

    while (len != 0 && *eos != CR_NC(NIL))
        len--, eos++;

    return ((leng_t)(eos - str));
}

#endif  /* !_CR_SICK_INLINE_ */

#endif  /* !__CR_MEMLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
