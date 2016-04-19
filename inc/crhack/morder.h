/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-17  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 字节顺序头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_MORDER_H__
#define __CR_MORDER_H__

#include "defs.h"

/* Visual C++ */
#if defined(_CR_CC_MSC_)
    #ifndef _CR_NO_INTRIN_
        #ifndef _CR_OS_WINCE_
            #include <intrin.h>
        #else
            #include <cmnintrin.h>
        #endif
        #include <stdlib.h>
    #endif
#endif

/*****************************************************************************/
/*                                 循环位移                                  */
/*****************************************************************************/

/*
=======================================
    字节循环左移 [port]
=======================================
*/
#if defined(_CR_NO_INLINE_) && defined(cr_rotl08)
    #define rotl_byte_t(val, shift) \
            cr_rotl08(val, (unsigned char)(shift))
#else
cr_inline byte_t
rotl_byte_t (
  __CR_IN__ byte_t  val,
  __CR_IN__ uint_t  shift
    )
{
/* Compiler intrinsic support */
#if     defined(cr_rotl08)
    return (cr_rotl08(val, (unsigned char)shift));

/* AT&T style inline asm (X86) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X86_)
    __asm__("rolb %%cl, %0" : "=r"(val) : "0"(val), "c"(shift));
    return (val);

#else
    return ((byte_t)((val << shift) | (val >> (8 - shift))));

#endif  /* AR & CP TYPE predefines */
}
#endif  /* _CR_NO_INLINE_ && cr_rotl08 */

/*
=======================================
    字节循环右移 [port]
=======================================
*/
#if defined(_CR_NO_INLINE_) && defined(cr_rotr08)
    #define rotr_byte_t(val, shift) \
            cr_rotr08(val, (unsigned char)(shift))
#else
cr_inline byte_t
rotr_byte_t (
  __CR_IN__ byte_t  val,
  __CR_IN__ uint_t  shift
    )
{
/* Compiler intrinsic support */
#if     defined(cr_rotr08)
    return (cr_rotr08(val, (unsigned char)shift));

/* AT&T style inline asm (X86) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X86_)
    __asm__("rorb %%cl, %0" : "=r"(val) : "0"(val), "c"(shift));
    return (val);

#else
    return ((byte_t)((val >> shift) | (val << (8 - shift))));

#endif  /* AR & CP TYPE predefines */
}
#endif  /* _CR_NO_INLINE_ && cr_rotr08 */

/*
=======================================
    单字循环左移 [port]
=======================================
*/
#if defined(_CR_NO_INLINE_) && defined(cr_rotl16)
    #define rotl_int16u(val, shift) \
            cr_rotl16(val, (unsigned char)(shift))
#else
cr_inline int16u
rotl_int16u (
  __CR_IN__ int16u  val,
  __CR_IN__ uint_t  shift
    )
{
/* Compiler intrinsic support */
#if     defined(cr_rotl16)
    return (cr_rotl16(val, (unsigned char)shift));

/* AT&T style inline asm (X86) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X86_)
    __asm__("rolw %%cl, %0" : "=r"(val) : "0"(val), "c"(shift));
    return (val);

#else
    return ((int16u)((val << shift) | (val >> (16 - shift))));

#endif  /* AR & CP TYPE predefines */
}
#endif  /* _CR_NO_INLINE_ && cr_rotl16 */

/*
=======================================
    单字循环右移 [port]
=======================================
*/
#if defined(_CR_NO_INLINE_) && defined(cr_rotr16)
    #define rotr_int16u(val, shift) \
            cr_rotr16(val, (unsigned char)(shift))
#else
cr_inline int16u
rotr_int16u (
  __CR_IN__ int16u  val,
  __CR_IN__ uint_t  shift
    )
{
/* Compiler intrinsic support */
#if     defined(cr_rotr16)
    return (cr_rotr16(val, (unsigned char)shift));

/* AT&T style inline asm (X86) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X86_)
    __asm__("rorw %%cl, %0" : "=r"(val) : "0"(val), "c"(shift));
    return (val);

#else
    return ((int16u)((val >> shift) | (val << (16 - shift))));

#endif  /* AR & CP TYPE predefines */
}
#endif  /* _CR_NO_INLINE_ && cr_rotr16 */

/*
=======================================
    双字循环左移 [port]
=======================================
*/
#if defined(_CR_NO_INLINE_) && defined(cr_rotl32)
    #define rotl_int32u(val, shift) \
            cr_rotl32(val, (unsigned char)(shift))
#else
cr_inline int32u
rotl_int32u (
  __CR_IN__ int32u  val,
  __CR_IN__ uint_t  shift
    )
{
/* Compiler intrinsic support */
#if     defined(cr_rotl32)
    return (cr_rotl32(val, (unsigned char)shift));

/* AT&T style inline asm (X86) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X86_)
    __asm__("roll %%cl, %0" : "=r"(val) : "0"(val), "c"(shift));
    return (val);

#else
    return ((int32u)((val << shift) | (val >> (32 - shift))));

#endif  /* AR & CP TYPE predefines */
}
#endif  /* _CR_NO_INLINE_ && cr_rotl32 */

/*
=======================================
    双字循环右移 [port]
=======================================
*/
#if defined(_CR_NO_INLINE_) && defined(cr_rotr32)
    #define rotr_int32u(val, shift) \
            cr_rotr32(val, (unsigned char)(shift))
#else
cr_inline int32u
rotr_int32u (
  __CR_IN__ int32u  val,
  __CR_IN__ uint_t  shift
    )
{
/* Compiler intrinsic support */
#if     defined(cr_rotr32)
    return (cr_rotr32(val, (unsigned char)shift));

/* AT&T style inline asm (X86) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X86_)
    __asm__("rorl %%cl, %0" : "=r"(val) : "0"(val), "c"(shift));
    return (val);

#else
    return ((int32u)((val >> shift) | (val << (32 - shift))));

#endif  /* AR & CP TYPE predefines */
}
#endif  /* _CR_NO_INLINE_ && cr_rotr32 */

/*
=======================================
    四字循环左移 [port]
=======================================
*/
#if defined(_CR_NO_INLINE_) && defined(cr_rotl64)
    #define rotl_int64u(val, shift) \
            cr_rotl64(val, (unsigned char)(shift))
#else
cr_inline int64u
rotl_int64u (
  __CR_IN__ int64u  val,
  __CR_IN__ uint_t  shift
    )
{
/* Compiler intrinsic support */
#if     defined(cr_rotl64)
    return (cr_rotl64(val, (unsigned char)shift));

#else
    return ((int64u)((val << shift) | (val >> (64 - shift))));

#endif  /* AR & CP TYPE predefines */
}
#endif  /* _CR_NO_INLINE_ && cr_rotl64 */

/*
=======================================
    四字循环右移 [port]
=======================================
*/
#if defined(_CR_NO_INLINE_) && defined(cr_rotr64)
    #define rotr_int64u(val, shift) \
            cr_rotr64(val, (unsigned char)(shift))
#else
cr_inline int64u
rotr_int64u (
  __CR_IN__ int64u  val,
  __CR_IN__ uint_t  shift
    )
{
/* Compiler intrinsic support */
#if     defined(cr_rotr64)
    return (cr_rotr64(val, (unsigned char)shift));

#else
    return ((int64u)((val >> shift) | (val << (64 - shift))));

#endif  /* AR & CP TYPE predefines */
}
#endif  /* _CR_NO_INLINE_ && cr_rotr64 */

/*****************************************************************************/
/*                                 字节顺序                                  */
/*****************************************************************************/

/* 字节顺序操作映射宏 */
#if defined(_CR_ORDER_LE_)
    #define  WORD_LE(val)   (val)
    #define DWORD_LE(val)   (val)
    #define QWORD_LE(val)   (val)
    #define  WORD_BE(val)   xchg_int16u(val)
    #define DWORD_BE(val)   xchg_int32u(val)
    #define QWORD_BE(val)   xchg_int64u(val)

    #define  CWORD_LE(val)  (val)
    #define CDWORD_LE(val)  (val)
    #define CQWORD_LE(val)  (val)
    #define  CWORD_BE(val)  xchg_cint16u(val)
    #define CDWORD_BE(val)  xchg_cint32u(val)
    #define CQWORD_BE(val)  xchg_cint64u(val)

#else   /* (_CR_ORDER_BE_) */
    #define  WORD_BE(val)   (val)
    #define DWORD_BE(val)   (val)
    #define QWORD_BE(val)   (val)
    #define  WORD_LE(val)   xchg_int16u(val)
    #define DWORD_LE(val)   xchg_int32u(val)
    #define QWORD_LE(val)   xchg_int64u(val)

    #define  CWORD_BE(val)  (val)
    #define CDWORD_BE(val)  (val)
    #define CQWORD_BE(val)  (val)
    #define  CWORD_LE(val)  xchg_cint16u(val)
    #define CDWORD_LE(val)  xchg_cint32u(val)
    #define CQWORD_LE(val)  xchg_cint64u(val)
#endif

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    单字交换顺序 (常数用)
=======================================
*/
cr_inline int16u
xchg_cint16u (
  __CR_IN__ int16u  val
    )
{
    return ((int16u)((((int16u)(((byte_t*)(&val))[0])) << 8) |
                     (((int16u)(((byte_t*)(&val))[1])))));
}

/*
=======================================
    双字交换顺序 (常数用)
=======================================
*/
cr_inline int32u
xchg_cint32u (
  __CR_IN__ int32u  val
    )
{
    return ((int32u)((((int32u)(((byte_t*)(&val))[0])) << 24) |
                     (((int32u)(((byte_t*)(&val))[1])) << 16) |
                     (((int32u)(((byte_t*)(&val))[2])) <<  8) |
                     (((int32u)(((byte_t*)(&val))[3])))));
}

/*
=======================================
    四字交换顺序 (常数用)
=======================================
*/
cr_inline int64u
xchg_cint64u (
  __CR_IN__ int64u  val
    )
{
    return ((int64u)((((int64u)(((byte_t*)(&val))[0])) << 56) |
                     (((int64u)(((byte_t*)(&val))[1])) << 48) |
                     (((int64u)(((byte_t*)(&val))[2])) << 40) |
                     (((int64u)(((byte_t*)(&val))[3])) << 32) |
                     (((int64u)(((byte_t*)(&val))[4])) << 24) |
                     (((int64u)(((byte_t*)(&val))[5])) << 16) |
                     (((int64u)(((byte_t*)(&val))[6])) <<  8) |
                     (((int64u)(((byte_t*)(&val))[7])))));
}

/*
=======================================
    单字交换顺序 [port]
=======================================
*/
cr_inline int16u
xchg_int16u (
  __CR_IN__ int16u  val
    )
{
/* Compiler intrinsic support */
#if     defined(cr_byteswap16)
    return (cr_byteswap16(val));

/* Intel style inline asm (ARM) */
#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_ARM_)
    int32u  r0, tmp = val;

    __asm {
        orr     r0, tmp, tmp, lsl #16
        mov     tmp, r0, ror #8
    }
    return ((int16u)tmp);

/* AT&T style inline asm (X86) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X86_)
    __asm__("xchgb %b0, %h0" : "=q"(val) : "0"(val));
    return (val);

/* AT&T style inline asm (X64) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X64_)
    __asm__("xchgb %b0, %h0" : "=Q"(val) : "0"(val));
    return (val);

#else
    #if !defined(_CR_CC_CX51_)
        return (rotr_int16u(val, 8));
    #else
        return ((int16u)((val >> 8) | (val << 8)));
    #endif
#endif  /* AR & CP TYPE predefines */
}

/*
=======================================
    双字交换顺序 [port]
=======================================
*/
cr_inline int32u
xchg_int32u (
  __CR_IN__ int32u  val
    )
{
/* Compiler intrinsic support */
#if     defined(cr_byteswap32)
    return (cr_byteswap32(val));

/* Intel style inline asm (ARM) */
#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_ARM_)
    int32u  r0, r1;

    __asm {
        mvn     r0, #0x0000FF00
        eor     r1, val, val, ror #16
        and     r1, r0, r1, lsr #8
        eor     val, r1, val, ror #8
    }
    return (val);

/* AT&T style inline asm (X86) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X86_)
    __asm__("bswap  %0" : "=r"(val) : "0"(val));
    return (val);

/* AT&T style inline asm (X64) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X64_)
    __asm__("bswapl %0" : "=r"(val) : "0"(val));
    return (val);

#else
    int16u  hi, lo;

    lo = (int16u)(val);
    hi = (int16u)(val >> 16);
    val  = xchg_int16u(lo);
    val <<= 16;
    val |= xchg_int16u(hi);
    return (val);

#endif  /* AR & CP TYPE predefines */
}

/*
=======================================
    四字交换顺序 [port]
=======================================
*/
cr_inline int64u
xchg_int64u (
  __CR_IN__ int64u  val
    )
{
/* Compiler intrinsic support */
#if     defined(cr_byteswap64)
    return (cr_byteswap64(val));

/* AT&T style inline asm (X86) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X86_)
    union {
        int64u  u;
        struct {
            int32u  a, b;
        } s;
    } tmp;

    tmp.u = val;
    __asm__("bswapl %0; bswapl %1; xchgl %0, %1"
                : "=r"(tmp.s.a), "=r"(tmp.s.b)
                :  "0"(tmp.s.a),  "1"(tmp.s.b));
    return (tmp.u);

/* AT&T style inline asm (X64) */
#elif   defined(_CR_ASM_ATnT_) && defined(_CR_AR_X64_)
    __asm__("bswapq %0" : "=r"(val) : "0"(val));
    return (val);

#else
    int32u  hi, lo;

    lo = (int32u)(val);
    hi = (int32u)(val >> 32);
    val  = xchg_int32u(lo);
    val <<= 32;
    val |= xchg_int32u(hi);
    return (val);

#endif  /* AR & CP TYPE predefines */
}

#endif  /* !_CR_SICK_INLINE_ */

#endif  /* !__CR_MORDER_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
