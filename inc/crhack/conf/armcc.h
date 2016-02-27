/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-07-08  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack ARMCC 编译器配置头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_ARMCC_H__
#define __CR_ARMCC_H__

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
    #define _CR_AR_ARM_     /* ARM */
    #if defined(__thumb) || defined(__thumb__)
        #define _CR_AR_THUMB_   /* THUMB */
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
    #define _CR_OS_NAKED_
    /*------------------------------------------------*/

    /* 编译器全局定义 */
    #pragma import (__use_no_semihosting_swi)
    #pragma import (__use_two_region_memory)
    /*------------------------------------------------*/

    /* 编译器内存模型 */
    #undef  _CR_SMALL_  /* 16bit 小模式 */
    #undef  _CR_LARGE_  /* 16bit 巨模式 */
    /*------------------------------------------------*/

    /* 编译器特有类型 */
    typedef unsigned int    ubit_t;
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
    #undef  _CR_NO_FLT64_
    #undef  _CR_DOUBLE32_
    /*------------------------------------------------*/

    /* 编译器64位整数 */
    #undef  _CR_NO_INT64_
    #undef  _CR_USE_LLONG_
    #define _CR_USE_INT64_
    /*------------------------------------------------*/

    /* 编译器64位常数后缀 */
    #define CR_SLL(x)   x##LL
    #define CR_ULL(x)   x##ULL
    /*------------------------------------------------*/

    /* 编译器内联函数修饰 */
    #undef  _CR_NO_INLINE_
    #define cr_inline       __inline
    /*------------------------------------------------*/

    /* 编译器汇编内联函数 */
    #define fasm_inline     cr_inline
    /*------------------------------------------------*/

    /* 编译器安全内联函数 */
    #define safe_inline     cr_inline
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
    #define _CR_ASM_INTL_
    #undef  _CR_ASM_ATnT_
    /*------------------------------------------------*/

    /* 编译器成员对齐修饰 */
    #define CR_ALIGN(x) __align(x)
    /*------------------------------------------------*/

    /* 编译器导入库的选项 */
    #define _CR_NO_PRAGMA_LIB_
    /*------------------------------------------------*/

    /* 编译器紧凑结构修饰 */
    #ifndef _CR_CC_ARMCC_OLD_
        #define CR_PACKED
        #define CR_TYPEDEF  typedef
        #undef  _CR_NO_PRAGMA_PACK_
    #else
        #define CR_PACKED
        #define CR_TYPEDEF  typedef __packed
        #define _CR_NO_PRAGMA_PACK_
    #endif
    /*------------------------------------------------*/

    /* 编译器分支优化指示 */
    #define surely(x)   (x)
    #define mostly(x)   (x)
    #define rarely(x)   (x)
    /*------------------------------------------------*/

    /* 编译器不支持多线程 */
    #define _CR_NO_MT_
    /*------------------------------------------------*/

    /* 编译器不支持宽字符 */
    #undef  _CR_NO_WIDE_
    /*------------------------------------------------*/

    /* 编译器指令函数优化 */
    #define _CR_NO_INTRIN_
    /*------------------------------------------------*/

    /* LIBC printf() 整数宽度前缀 */
    #define CR_I32
    #define CR_I64  "ll"
    /*------------------------------------------------*/

    /* LIBC 是否有 errno.h 头文件 */
    #undef  _CR_NO_ERRNO_
    /*------------------------------------------------*/

    /* LIBC 支持64位STDIO文件偏移 */
    #define _CR_NO_STDIO64_
    #undef  _CR_MS_STDIO64_
    /*------------------------------------------------*/

    /* LIBC 支持 C99 数学函数设置 */
    #define _CR_NO_MATHC99_
    /*------------------------------------------------*/

    /* LIBC beginthreadex() 返回值 */
    #undef  CR_BTEX_FAIL
    /*------------------------------------------------*/

    /* LIBC 支持宽字符串函数的设置 */
    #undef  _CR_HAVE_WCS_
    /*------------------------------------------------*/

    /* 处理器架构的空指令宏设置 */
    #ifndef _CR_CC_ARMCC_OLD_
        #define CR_NOP  __nop()
    #else
        #define CR_NOP  __asm { nop }
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
