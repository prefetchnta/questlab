/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-01-05  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack IAR C/C++ 编译器配置头文件 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_IARCC_H__
#define __CR_IARCC_H__

/************************************/
/* 不同架构的编译器版本标法都不一样 */
/* 具体参见编译器的预定义宏说明文档 */
/************************************/
#if defined(__IAR_SYSTEMS_ICC__)

    /* 编译器类型定义 */
    #define _CR_CC_IARCC_
    /*------------------------------------------------*/

    /* 编译器版本定义 */
    #define _CR_CC_VER_     __VER__
    /*------------------------------------------------*/

    /* 编译器名称定义 */
    #if     defined(__ICCARM__)
        #define _CR_CC_ICCARM_
        #define _CR_CC_STR_ "IAR C/C++ for ARM"

    #elif   defined(__ICC8051__)
        #define _CR_CC_ICC8051_
        #define _CR_CC_STR_ "IAR C/C++ for 8051"

    #else
        #error "iarcc.h: CC TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器版本过滤 */
    #if     0
        #error "iarcc.h: CC TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器平台架构 */
    #if     defined(__ICCARM__)
        #define _CR_AR_ARM_     /* ARM */
        #if (__CPU_MODE__ == 1)
            #define _CR_AR_THUMB_   /* THUMB */
        #endif
    #elif   defined(__ICC8051__)
        #define _CR_AR_MCS51_   /* Intel MCS-51 */
    #else
        #error "iarcc.h: AR TYPE not supported yet!"
    #endif
    #if defined(__ICCARM__)
        #if     (__CORE__ == __ARM4M__)
            #define _CR_ARM_V16_    0
            #define _CR_ARM_V32_    4
        #elif   (__CORE__ == __ARM4TM__)
            #define _CR_ARM_V16_    1
            #define _CR_ARM_V32_    4
        #elif   (__CORE__ == __ARM5__)
            #define _CR_ARM_V16_    2
            #define _CR_ARM_V32_    5
        #elif   (__CORE__ == __ARM5E__)
            #define _CR_ARM_V16_    2
            #define _CR_ARM_V32_    5
        #elif   (__CORE__ == __ARM6__)
            #define _CR_ARM_V16_    3
            #define _CR_ARM_V32_    6
        #elif   (__CORE__ == __ARM6M__)
            #define _CR_ARM_V16_    3
            #define _CR_ARM_V32_    0
        #elif   (__CORE__ == __ARM6SM__)
            #define _CR_ARM_V16_    3
            #define _CR_ARM_V32_    0
        #elif   (__CORE__ == __ARM7M__)
            #define _CR_ARM_V16_    4
            #define _CR_ARM_V32_    0
        #elif   (__CORE__ == __ARM7EM__)
            #define _CR_ARM_V16_    4
            #define _CR_ARM_V32_    0
        #elif   (__CORE__ == __ARM7A__)
            #define _CR_ARM_V16_    4
            #define _CR_ARM_V32_    7
        #elif   (__CORE__ == __ARM7R__)
            #define _CR_ARM_V16_    4
            #define _CR_ARM_V32_    7
        #else
            #error "iarcc.h: never get here!"
        #endif
    #endif
    /*------------------------------------------------*/

    /* 编译器操作系统 */
    #define _CR_OS_NAKED_
    /*------------------------------------------------*/

    /* 编译器全局定义 */
    #include <intrinsics.h>
    /*------------------------------------------------*/

    /* 编译器内存模型 */
    #if defined(_CR_CC_ICC8051_)
        #define _CR_SMALL_  /* 16bit 小模式 */
    #endif
    #undef  _CR_LARGE_  /* 16bit 巨模式 */
    /*------------------------------------------------*/

    /* 编译器特有类型 */
    #if defined(_CR_CC_ICC8051_)
        typedef bool    ubit_t;
    #else
        typedef unsigned int    ubit_t;
    #endif
    /*------------------------------------------------*/

    /* 编译器指针修饰 */
    #if !defined(_CR_CC_ICC8051_)
        #define _far_
        #define _rom_
        #define _slw_
        #define _pge_
        #define _ram_
    #else
        #define _far_   __far
        #define _rom_   __code
        #define _slw_   __idata
        #define _pge_   __pdata
        #define _ram_   __xdata
    #endif
    /*------------------------------------------------*/

    /* 编译器浮点配置 */
    #undef  _CR_NO_FLT32_
    #undef  _CR_NO_FLT64_
    #undef  _CR_DOUBLE32_
    /*------------------------------------------------*/

    /* 编译器64位整数 */
    #undef  _CR_NO_INT64_
    #define _CR_USE_LLONG_
    #undef  _CR_USE_INT64_
    /*------------------------------------------------*/

    /* 编译器64位常数后缀 */
    #define CR_SLL(x)   x##LL
    #define CR_ULL(x)   x##ULL
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
    #undef  _CR_ASM_ATnT_
    /*------------------------------------------------*/

    /* 编译器成员对齐修饰 */
    #define CR_ALIGN(x)
    /*------------------------------------------------*/

    /* 编译器导入库的选项 */
    #define _CR_NO_PRAGMA_LIB_
    /*------------------------------------------------*/

    /* 编译器紧凑结构修饰 */
    #if     defined(_CR_CC_ICC8051_)
        #define CR_PACKED
        #define CR_TYPEDEF  typedef
        #define _CR_NO_PRAGMA_PACK_

    #elif   defined(_CR_CC_ICCARM_)
        #define CR_PACKED
        #define CR_TYPEDEF  typedef
        #undef  _CR_NO_PRAGMA_PACK_
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
    #if     defined(_CR_CC_ICC8051_)
        #define CR_I32  "L"
        #define CR_I64
    #elif   defined(_CR_CC_ICCARM_)
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
    #define _CR_NO_MATHC99_
    /*------------------------------------------------*/

    /* LIBC beginthreadex() 返回值 */
    #undef  CR_BTEX_FAIL
    /*------------------------------------------------*/

    /* LIBC 支持宽字符串函数的设置 */
    #undef  _CR_HAVE_WCS_
    /*------------------------------------------------*/

    /* 处理器架构的空指令宏设置 */
    #define CR_NOP  __no_operation()
    /*------------------------------------------------*/

#endif  /* __IAR_SYSTEMS_ICC__ */

#endif  /* !__CR_IARCC_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
