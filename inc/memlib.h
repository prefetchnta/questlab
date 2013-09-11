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
#define __CR_MEMLIB_H__ 0xA81AD7A1UL

#include "defs.h"

#ifndef _CR_NO_STDC_
    #include <string.h>
#endif

/*****************************************************************************/
/*                               内存分配释放                                */
/*****************************************************************************/

CR_API void_t   mem_reset (void_t);
CR_API void_t   mem_count (dist_t *size, leng_t *maxs,
                           leng_t *tots, sint_t *count);
CR_API void_t   mem_free (const void_t *ptr);
CR_API void_t*  mem_malloc (leng_t size);
CR_API void_t*  mem_calloc (leng_t num, leng_t size);
CR_API void_t*  mem_malloc32 (int32u size);
CR_API void_t*  mem_malloc64 (int64u size);
CR_API void_t*  mem_calloc32 (int32u num, leng_t size);
CR_API void_t*  mem_calloc64 (int64u num, leng_t size);
CR_API void_t*  mem_realloc (void_t *ptr, leng_t new_size);
CR_API void_t   mem_info (msize_t *total, msize_t *avail);

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

/* 16bit 巨模式基本操作映射 */
#if     defined(_CR_LARGE_)
    #define mem_set     mem_far_set
    #define mem_cpy     mem_far_cpy
    #define mem_cmp     mem_far_cmp
    #define mem_mov     mem_far_mov
    #define mem_zero    mem_far_zero
    #define mem_setw    mem_far_setw
    #define mem_setd    mem_far_setd
    #define mem_setq    mem_far_setq
    #define mem_sett    mem_far_sett

CR_API void_t*  mem_far_set (void_t *dst, byte_t val, leng_t len);
CR_API void_t*  mem_far_cpy (void_t *dst, const void_t *src, leng_t len);
CR_API sint_t   mem_far_cmp (const void_t *dst, const void_t *src, leng_t len);
CR_API void_t*  mem_far_mov (void_t *dst, const void_t *src, leng_t len);
CR_API void_t*  mem_far_zero (void_t *dst, leng_t len);
CR_API void_t*  mem_far_setw (void_t *dst, int16u val, leng_t len);
CR_API void_t*  mem_far_setd (void_t *dst, int32u val, leng_t len);
CR_API void_t*  mem_far_setq (void_t *dst, int64u val, leng_t len);
CR_API void_t*  mem_far_sett (void_t *dst, const byte_t val[3], leng_t len);

#endif  /* 函数 mem_****() 重映射 */

/* 使用 LIBC 提供的内存操作 (默认) */
#if !defined(_CR_NO_STDC_) && \
    !defined(mem_set) && !defined(mem_cpy) && \
    !defined(mem_cmp) && !defined(mem_mov)
    #define mem_set     mem_set_stdc
    #define mem_cpy     mem_cpy_stdc
    #define mem_cmp     mem_cmp_stdc
    #define mem_mov     mem_mov_stdc
/*
=======================================
    STDC 内存填充
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define mem_set_stdc    memset
#else
inline void_t*
mem_set_stdc (
  __CR_OT__ void_t* dst,
  __CR_IN__ byte_t  val,
  __CR_IN__ leng_t  len
    )
{
    return (memset(dst, val, len));
}
#endif  /* _CR_NO_INLINE_ */

/*
=======================================
    STDC 内存复制
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define mem_cpy_stdc    memcpy
#else
inline void_t*
mem_cpy_stdc (
  __CR_OT__ void_t*         dst,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          len
    )
{
    return (memcpy(dst, src, len));
}
#endif  /* _CR_NO_INLINE_ */

/*
=======================================
    STDC 内存比较
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define mem_cmp_stdc    memcmp
#else
inline sint_t
mem_cmp_stdc (
  __CR_IN__ const void_t*   dst,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          len
    )
{
    return (memcmp(dst, src, len));
}
#endif  /* _CR_NO_INLINE_ */

/*
=======================================
    STDC 内存移动
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define mem_mov_stdc    memmove
#else
inline void_t*
mem_mov_stdc (
  __CR_OT__ void_t*         dst,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          len
    )
{
    return (memmove(dst, src, len));
}
#endif  /* _CR_NO_INLINE_ */

#endif  /* !_CR_NO_STDC_ && !mem_set() && !mem_cpy() &&
                            !mem_cpy() && !mem_mov() */
/*
=======================================
    内存填充
=======================================
*/
#ifndef mem_set
inline void_t*
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
inline void_t*
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
inline sint_t
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
inline void_t*
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
    内存清零
=======================================
*/
#ifndef mem_zero
#if defined(_CR_NO_INLINE_)
    #define mem_zero(d, n)  mem_set(d, 0, n)
#else
inline void_t*
mem_zero (
  __CR_OT__ void_t* dst,
  __CR_IN__ leng_t  len
    )
{
    return (mem_set(dst, 0, len));
}
#endif  /* _CR_NO_INLINE_ */
#endif  /* !mem_zero() */

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    单字填充
=======================================
*/
#ifndef mem_setw
inline void_t*
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
inline void_t*
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
inline void_t*
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
inline void_t*
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
inline int16u
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
inline int32u
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
inline int64u
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
inline void_t
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
inline void_t
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
inline void_t
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

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    设置数据缓冲
=======================================
*/
inline bool_t
buffer_init (
  __CR_OT__ sBUFFER*        buff,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_free
    )
{
    if (data == NULL) {
        data = mem_malloc(size);
        if (data == NULL) {
            err_set(__CR_MEMLIB_H__, CR_NULL,
                    "buffer_init()", "mem_malloc() failure");
            return (FALSE);
        }
        is_free = TRUE;
    }
    if (!is_free)
        buff->size = (dist_t)(size - 0);
    else
        buff->size = (dist_t)(0 - size);
    buff->data = (void_t*)data;
    return (TRUE);
}

/*
=======================================
    释放数据缓冲
=======================================
*/
inline void_t
buffer_free (
  __CR_IN__ const sBUFFER*  buff
    )
{
    if (buff->size < 0)
        mem_free(buff->data);
}

/*
=======================================
    获取数据大小
=======================================
*/
inline leng_t
buffer_size (
  __CR_IN__ const sBUFFER*  buff
    )
{
    if (buff->size < 0)
        return ((leng_t)(-buff->size));
    return ((leng_t)buff->size);
}

#endif  /* !_CR_SICK_INLINE_ */

/*****************************************************************************/
/*                               字符基本操作                                */
/*****************************************************************************/

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    返回字符串长度A
=======================================
*/
inline leng_t
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
inline leng_t
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
inline leng_t
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
inline leng_t
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
inline leng_t
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
inline leng_t
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
