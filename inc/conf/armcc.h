/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-07-08  */
/*     #######          ###    ###      [PORT]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-05-16  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack ARMCC 编译器配置头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_ARMCC_H__
#define __CR_ARMCC_H__ 0xE7575BA7UL

/*************************************/
/* 版本值 300503 表示版本 V3.0.0.503 */
/*************************************/
#if defined(__CC_ARM)

    /* 编译器类型定义 */
    #define _CR_CC_ARMCC_
    /*------------------------------------------------*/

    /* 编译器版本定义 */
    #define _CR_CC_VER_     __ARMCC_VERSION
    /*------------------------------------------------*/

    /* 编译器名称定义 */
    #if (_CR_CC_VER_ < 200000)
        #define _CR_CC_ARMCC_OLD_
        #define _CR_CC_STR_ "SDT/ADS ARMCC"
    #else
        #define _CR_CC_ARMCC_NEW_
        #define _CR_CC_STR_ "RealView C/C++"
    #endif
    /*------------------------------------------------*/

    /* 编译器版本过滤 */
    #if (_CR_CC_VER_ < 120000)
        #error "armcc.h: CC TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器平台架构 */
    #if defined(__thumb) || defined(__thumb__)
        #define _CR_AR_THUMB_   /* THUMB */
    #else
        #define _CR_AR_ARM_     /* ARM */
    #endif
    #if defined(_CR_CC_ARMCC_OLD_)
        #if     defined(__TARGET_ARCH_3) || \
                defined(__TARGET_ARCH_3M)
            #define _CR_ARM_V16_    0
            #define _CR_ARM_V32_    3
        #elif   defined(__TARGET_ARCH_4) || \
                defined(__TARGET_ARCH_4xM)
            #define _CR_ARM_V16_    0
            #define _CR_ARM_V32_    4
        #elif   defined(__TARGET_ARCH_4T) || \
                defined(__TARGET_ARCH_4TxM)
            #define _CR_ARM_V16_    1
            #define _CR_ARM_V32_    4
        #elif   defined(__TARGET_ARCH_5T) || \
                defined(__TARGET_ARCH_5TE) || \
                defined(__TARGET_ARCH_5TEJ)
            #define _CR_ARM_V16_    2
            #define _CR_ARM_V32_    5
        #else
            #error "armcc.h: never get here!"
        #endif
    #else
        #define _CR_ARM_V32_    __TARGET_ARCH_ARM
        #define _CR_ARM_V16_    __TARGET_ARCH_THUMB
    #endif
    /*------------------------------------------------*/

    /* 编译器操作系统 */
    #if     1
        #define _CR_OS_NAKED_
    #endif
    /*------------------------------------------------*/

    /* 编译器全局定义 */
    #pragma import (__use_no_semihosting_swi)
    #pragma import (__use_two_region_memory)
    /*------------------------------------------------*/

    /* 编译器多余警告 */

    /*------------------------------------------------*/

    /* 编译器内存模型 */
    #if     0
        #define _CR_SMALL_  /* 16bit 小模式 */
    #elif   0
        #define _CR_LARGE_  /* 16bit 巨模式 */
    #endif
    /*------------------------------------------------*/

    /* 编译器过期风格 */
    #if     0
        #define const
        #define signed
        #define volatile
    #endif
    /*------------------------------------------------*/

    /* 编译器特有类型 */
    typedef unsigned int    ubit_t;
    /*------------------------------------------------*/

    /* 编译器指针修饰 */
    #if     1
        #define _far_
        #define _rom_
        #define _slw_
        #define _pge_
        #define _ram_
    #endif
    /*------------------------------------------------*/

    /* 编译器32位浮点 */
    #if     0
        #define _CR_NO_FLT32_
    #endif
    /*------------------------------------------------*/

    /* 编译器64位浮点 */
    #if     0
        #define _CR_NO_FLT64_
    #endif
    /*------------------------------------------------*/

    /* 编译器64位整数 */
    #if     0
        #define _CR_NO_INT64_
    #elif   0
        #define _CR_USE_LLONG_
    #else
        #define _CR_USE_INT64_
    #endif
    /*------------------------------------------------*/

    /* 编译器64位常数后缀 */
    #define CR_SLL(x)   x##LL
    #define CR_ULL(x)   x##ULL
    /*------------------------------------------------*/

    /* 编译器内联函数修饰 */
    #undef  _CR_NO_INLINE_
    #ifndef __cplusplus
        #define inline  __inline
    #endif
    /*------------------------------------------------*/

    /* 编译器汇编内联函数 */
    #define fasm_inline   inline
    /*------------------------------------------------*/

    /* 编译器安全内联函数 */
    #define safe_inline   inline
    /*------------------------------------------------*/

    /* 编译器强迫内联修饰 */
    #if defined(_CR_CC_ARMCC_OLD_)
        #define CR_INLINE   __inline
    #else
        #define CR_INLINE   __forceinline
    #endif
    /*------------------------------------------------*/

    /* 编译器函数导出修饰 */
    #if defined(_CR_CC_ARMCC_OLD_)
        #define CR_EXPORT
        #define CR_IMPORT
    #else
        #define CR_EXPORT   __declspec(dllexport)
        #define CR_IMPORT   __declspec(dllimport)
    #endif
    /*------------------------------------------------*/

    /* 编译器内联汇编风格 */
    #if     1
        #define _CR_ASM_INTL_
    #elif   0
        #define _CR_ASM_ATnT_
    #else
        #define _CR_ASM_SPEC_
    #endif
    /*------------------------------------------------*/

    /* 编译器noreturn修饰 */
    #if defined(_CR_CC_ARMCC_OLD_)
        #define CR_NORETURN
    #else
        #define CR_NORETURN __declspec(noreturn)
    #endif
    /*------------------------------------------------*/

    /* 编译器noinline修饰 */
    #if defined(_CR_CC_ARMCC_OLD_)
        #define CR_NOINLINE
    #else
        #define CR_NOINLINE __declspec(noinline)
    #endif
    /*------------------------------------------------*/

    /* 编译器成员对齐修饰 */
    #if     1
        #define CR_ALIGN(x) __align(x)
    #endif
    /*------------------------------------------------*/

    /* 编译器导入库的选项 */
    #if     1
        #define _CR_NO_PRAGMA_LIB_
    #endif
    /*------------------------------------------------*/

    /* 编译器紧凑结构修饰 */
    #ifndef _CR_CC_ARMCC_OLD_
        #define CR_PACKED
        #define CR_TYPEDEF  typedef
        #if     0
            #define _CR_NO_PRAGMA_PACK_
        #endif
    #else
        #define CR_PACKED
        #define CR_TYPEDEF  typedef __packed
        #if     1
            #define _CR_NO_PRAGMA_PACK_
        #endif
    #endif
    /*------------------------------------------------*/

    /* 编译器分支优化指示 */
    #define surely(x)   (x)
    #define mostly(x)   (x)
    #define rarely(x)   (x)
    /*------------------------------------------------*/

    /* 编译器不支持多线程 */
    #if     1
        #define _CR_NO_MT_
    #endif
    /*------------------------------------------------*/

    /* 编译器不支持宽字符 */
    #if     0
        #define _CR_NO_WIDE_
    #endif
    /*------------------------------------------------*/

    /* 编译器指令函数优化 */
    #define _CR_NO_CSWAP_
    #define _CR_NO_CROT32_
    #define _CR_NO_CROT64_
    #define _CR_NO_IROTSM_
    #define _CR_NO_INTRIN_
    /*------------------------------------------------*/

    /* LIBC 剔除所有 C 函数的选项 */
    #if     0
        #define _CR_NO_STDC_
    #endif
    /*------------------------------------------------*/

    /* LIBC 是否支持 GLIBC 函数库 */
    #if     0
        #define _CR_USE_GLIBC_
    #endif
    /*------------------------------------------------*/

    /* LIBC printf() 整数宽度前缀 */
    #define CR_I08
    #define CR_I16
    #define CR_I32
    #define CR_I64  "ll"
    /*------------------------------------------------*/

    /* LIBC 是否有 errno.h 头文件 */
    #if     0
        #define _CR_NO_ERRNO_
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持64位STDIO文件偏移 */
    #if     1
        #define _CR_NO_STDIO64_
    #else
        #define _CR_MS_STDIO64_
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持64位POSIX文件偏移 */
    #if     1
        #define _CR_NO_POSIX64_
    #else
        #define _CR_MS_POSIX64_
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持 C99 数学函数设置 */
    #if     1
        #define _CR_NO_MATHC99_
    #endif
    /*------------------------------------------------*/

    /* LIBC beginthreadex() 返回值 */
    #if     1
        #undef  CR_BTEX_FAIL
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持宽字符串函数的设置 */
    #if     0
        #define _CR_HAVE_WCS_
    #endif
    /*------------------------------------------------*/

    /* 处理器架构的空指令宏设置 */
    #ifndef _CR_CC_ARMCC_OLD_
        #define CR_NOP  __nop()
    #else
        #define CR_NOP  __asm { nop }
    #endif
    /*------------------------------------------------*/

    /* 处理器架构的对齐访问设置 */
    #if     defined(_CR_AR_ARM_)
        #if (_CR_ARM_V32_ < 7)
            #define _CR_ALIGN_NEEDED_
        #endif
    #elif   defined(_CR_AR_THUMB_)
        #if (_CR_ARM_V16_ < 4)
            #define _CR_ALIGN_NEEDED_
        #endif
    #endif
    /*------------------------------------------------*/

    /* 处理器架构支持原子级操作 */
    #if     0
        #define _CR_FAST_ATOM_
    #endif
    /*------------------------------------------------*/

#endif  /* __CC_ARM */

#endif  /* !__CR_ARMCC_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
