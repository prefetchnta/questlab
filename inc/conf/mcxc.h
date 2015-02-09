/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-10-28  */
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
/*  >>>>>>>>>>>>>>>>> CrHack MicroChip XC 编译器配置头文件 <<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_MCXC_H__
#define __CR_MCXC_H__

/******************************/
/* 版本值 1030 表示版本 V1.03 */
/******************************/
#if defined(__XC8) || defined(__XC16) || defined(__XC32)

    /* 编译器类型定义 */
    #define _CR_CC_MCXC_
    /*------------------------------------------------*/

    /* 编译器版本定义 */
    #if     defined(__XC8)
        #define _CR_CC_XC8_
        #define _CR_CC_VER_     __XC8_VERSION

    #elif   defined(__XC16)
        #define _CR_CC_XC16_
        #define _CR_CC_VER_     __XC16_VERSION

    #else
        #define _CR_CC_XC32_
        #define _CR_CC_VER_     __XC32_VERSION
    #endif
    /*------------------------------------------------*/

    /* 编译器名称定义 */
    #if     defined(_CR_CC_XC8_)
        #define _CR_CC_STR_     "MicroChip XC8"

    #elif   defined(_CR_CC_XC16_)
        #define _CR_CC_STR_     "MicoChip XC16"

    #else
        #define _CR_CC_STR_     "MicoChip XC32"
    #endif
    /*------------------------------------------------*/

    /* 编译器版本过滤 */
    #if     0
        #error "mcxc.h: CC TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器平台架构 */
    #if     defined(_CR_CC_XC8_)
        #define _CR_AR_PIC8_    /* MicroChip PIC-8bit */

    #elif   defined(_CR_CC_XC16_)
        #define _CR_AR_PIC16_   /* MicroChip PIC-16bit */

    #else
        #define _CR_AR_MIPS_    /* MIPS R3000 */
        #if defined(__mips16)
            #define _CR_AR_MIPS16_  /* MIPS16e */
        #endif
    #endif
    /*------------------------------------------------*/

    /* 编译器操作系统 */
    #define _CR_OS_NAKED_
    /*------------------------------------------------*/

    /* 编译器全局定义 */
    #define _CR_ORDER_LE_   /* 全部小端模式 */
    #if defined(_CR_CC_XC8_)
        #define _CR_SICK_INLINE_    /* 剔除内联 */
    #endif
    /*------------------------------------------------*/

    /* 编译器内存模型 */
    #if !defined(_CR_CC_XC32_)
        #define _CR_SMALL_  /* 16bit 小模式 */
    #endif
    #undef  _CR_LARGE_  /* 16bit 巨模式 */
    /*------------------------------------------------*/

    /* 编译器特有类型 */
    #if defined(_CR_CC_XC8_)
        typedef bit     ubit_t;
    #else
        typedef unsigned int    ubit_t;
    #endif
    /*------------------------------------------------*/

    /* 编译器指针修饰 */
    #define _far_
    #define _rom_
    #define _slw_
    #define _pge_
    #define _ram_
    /*------------------------------------------------*/

    /* 编译器浮点配置 */
    #undef  _CR_NO_FLT32_
    #if defined(_CR_CC_XC8_)
        #define _CR_NO_FLT64_
    #else
        #define _CR_DOUBLE32_
    #endif
    /*------------------------------------------------*/

    /* 编译器64位整数 */
    #if defined(_CR_CC_XC8_)
        #define _CR_NO_INT64_
    #else
        #define _CR_USE_LLONG_
    #endif
    #undef  _CR_USE_INT64_
    /*------------------------------------------------*/

    /* 编译器64位常数后缀 */
    #if defined(_CR_CC_XC8_)
        #define CR_SLL(x)   x
        #define CR_ULL(x)   x
    #else
        #define CR_SLL(x)   x##LL
        #define CR_ULL(x)   x##ULL
    #endif
    /*------------------------------------------------*/

    /* 编译器内联函数修饰 */
    #undef  _CR_NO_INLINE_
    #define cr_inline   static inline
    /*------------------------------------------------*/

    /* 编译器汇编内联函数 */
    #define fasm_inline     cr_inline
    /*------------------------------------------------*/

    /* 编译器安全内联函数 */
    #define safe_inline     cr_inline
    /*------------------------------------------------*/

    /* 编译器函数导出修饰 */
    #define CR_EXPORT
    #define CR_IMPORT
    /*------------------------------------------------*/

    /* 编译器内联汇编风格 */
    #undef  _CR_ASM_INTL_
    #if !defined(_CR_CC_XC8_)
        #define _CR_ASM_ATnT_
    #endif
    /*------------------------------------------------*/

    /* 编译器成员对齐修饰 */
    #if defined(_CR_CC_XC8_)
        #define CR_ALIGN(x)
    #else
        #define CR_ALIGN(x) __attribute__((aligned(x)))
    #endif
    /*------------------------------------------------*/

    /* 编译器导入库的选项 */
    #define _CR_NO_PRAGMA_LIB_
    /*------------------------------------------------*/

    /* 编译器紧凑结构修饰 */
    #if defined(_CR_CC_XC8_)
        #define CR_PACKED
        #define CR_TYPEDEF  typedef
        #define _CR_NO_PRAGMA_PACK_
    #else
        #define CR_TYPEDEF  typedef
        #define CR_PACKED   __attribute__((packed))
        #define _CR_NO_PRAGMA_PACK_
    #endif
    /*------------------------------------------------*/

    /* 编译器分支优化指示 */
    #if defined(_CR_CC_XC8_)
        #define surely(x)   (x)
        #define mostly(x)   (x)
        #define rarely(x)   (x)
    #else
        #define surely(x)   (x)
        #define mostly(x)   __builtin_expect(!!(x), 1)
        #define rarely(x)   __builtin_expect(!!(x), 0)
    #endif
    /*------------------------------------------------*/

    /* 编译器不支持多线程 */
    #define _CR_NO_MT_
    /*------------------------------------------------*/

    /* 编译器不支持宽字符 */
    #if defined(_CR_CC_XC8_)
        #define _CR_NO_WIDE_
    #endif
    /*------------------------------------------------*/

    /* 编译器指令函数优化 */
    #define _CR_NO_INTRIN_
    /*------------------------------------------------*/

    /* LIBC printf() 整数宽度前缀 */
    #if !defined(_CR_CC_XC32_)
        #define CR_I32  "l"
        #define CR_I64
    #else
        #define CR_I32
        #define CR_I64  "ll"
    #endif
    /*------------------------------------------------*/

    /* LIBC 是否有 errno.h 头文件 */
    #undef  _CR_NO_ERRNO_
    /*------------------------------------------------*/

    /* LIBC 支持64位STDIO文件偏移 */
    #define _CR_NO_STDIO64_
    #undef  _CR_MS_STDIO64_
    /*------------------------------------------------*/

    /* LIBC 支持 C99 数学函数设置 */
    #if defined(_CR_CC_XC8_)
        #define _CR_NO_MATHC99_
    #endif
    /*------------------------------------------------*/

    /* LIBC beginthreadex() 返回值 */
    #undef  CR_BTEX_FAIL
    /*------------------------------------------------*/

    /* LIBC 支持宽字符串函数的设置 */
    #undef  _CR_HAVE_WCS_
    /*------------------------------------------------*/

    /* 处理器架构的空指令宏设置 */
    #if defined(_CR_CC_XC8_)
        #define CR_NOP  _nop()
        #pragma intrinsic(_nop)
        extern void _nop (void);
    #else
        #define CR_NOP  __builtin_nop()
    #endif
    /*------------------------------------------------*/

#endif  /* __XC8 || __XC16 || __XC32 */

#endif  /* !__CR_MCXC_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
