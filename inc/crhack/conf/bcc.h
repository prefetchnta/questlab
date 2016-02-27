/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-02-17  */
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
/*  >>>>>>>>>>>>>>>> CrHack Borland C/C++ 编译器配置头文件 <<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_BCC_H__
#define __CR_BCC_H__

/*********************************/
/* BCB 1.0 __BORLANDC__ = 0x052X */
/* BCB 3.0 __BORLANDC__ = 0x053X */
/* BCB 4.0 __BORLANDC__ = 0x054X */
/* BCB 5.0 __BORLANDC__ = 0x055X */
/* BCB 6.0 __BORLANDC__ = 0x056X */
/* BDS2006 __BORLANDC__ = 0x057X */
/* BDS2006 __BORLANDC__ = 0x058X */
/* BDS2007 __BORLANDC__ = 0x059X */
/* BDS2009 __BORLANDC__ = 0x061X */
/* BDS2010 __BORLANDC__ = 0x062X */
/*********************************/
#if defined(__BORLANDC__) || defined(__TURBOC__)

    /* 编译器类型定义 */
    #define _CR_CC_BCC_
    /*------------------------------------------------*/

    /* 编译器版本定义 */
    #ifndef __BORLANDC__
        #define _CR_CC_VER_     __TURBOC__
    #else
        #define _CR_CC_VER_     __BORLANDC__
    #endif
    /*------------------------------------------------*/

    /* 编译器名称定义 */
    #if     (_CR_CC_VER_ >= 0x0620)
        #define _CR_CC_STR_     "Embarcadero C/C++"

    #elif   (_CR_CC_VER_ >= 0x0590)
        #define _CR_CC_STR_     "CodeGear C/C++"

    #else
        #define _CR_CC_STR_     "Borland C/C++"
    #endif
    /*------------------------------------------------*/

    /* 编译器版本过滤 */
    #if (_CR_CC_VER_ < 0x0560)
        #error "bcc.h: CC TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器平台架构 */
    #if     defined(_M_IX86)
        #define _CR_AR_X86_     /* Intel X86 */

    #else
        #error "bcc.h: AR TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器操作系统 */
    #if     defined(__WIN32__)
        #define _CR_OS_MSWIN_
        #define _CR_OS_WIN32_
    #else
        #error "bcc.h: OS TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器全局定义 */
    #define _FM_USE_FASTMATH

    #pragma warn -8027      /* 函数无法内联 */
    #pragma warn -8058      /* 无法预编译头 */
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
    #define fasm_inline     static
    /*------------------------------------------------*/

    /* 编译器安全内联函数 */
    #define safe_inline     static
    /*------------------------------------------------*/

    /* 编译器函数导出修饰 */
    #ifndef _CR_OS_MSWIN_
        #define CR_EXPORT
        #define CR_IMPORT
    #else
        #define CR_EXPORT   __declspec(dllexport)
        #define CR_IMPORT   __declspec(dllimport)
    #endif
    /*------------------------------------------------*/

    /* 编译器内联汇编风格 */
    #if defined(_CR_AR_X86_)
        #define _CR_ASM_INTL_
    #endif
    #undef  _CR_ASM_ATnT_
    /*------------------------------------------------*/

    /* 编译器成员对齐修饰 */
    #define CR_ALIGN(x)
    /*------------------------------------------------*/

    /* 编译器导入库的选项 */
    #undef  _CR_NO_PRAGMA_LIB_
    /*------------------------------------------------*/

    /* 编译器紧凑结构修饰 */
    #define CR_PACKED
    #define CR_TYPEDEF  typedef
    #undef  _CR_NO_PRAGMA_PACK_
    /*------------------------------------------------*/

    /* 编译器分支优化指示 */
    #define surely(x)   (x)
    #define mostly(x)   (x)
    #define rarely(x)   (x)
    /*------------------------------------------------*/

    /* 编译器不支持多线程 */
    #if !defined(__MT__)
        #define _CR_NO_MT_
    #endif
    /*------------------------------------------------*/

    /* 编译器不支持宽字符 */
    #undef  _CR_NO_WIDE_
    /*------------------------------------------------*/

    /* 编译器指令函数优化 */
    #define _CR_NO_INTRIN_
    /*------------------------------------------------*/

    /* LIBC printf() 整数宽度前缀 */
    #define CR_I32
    #define CR_I64  "I64"
    /*------------------------------------------------*/

    /* LIBC 是否有 errno.h 头文件 */
    #undef  _CR_NO_ERRNO_
    /*------------------------------------------------*/

    /* LIBC 支持64位STDIO文件偏移 */
    #if (_CR_CC_VER_ < 0x0610)
        #define _CR_NO_STDIO64_
    #else
        #define _CR_MS_STDIO64_
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持 C99 数学函数设置 */
    #define _CR_NO_MATHC99_
    /*------------------------------------------------*/

    /* LIBC beginthreadex() 返回值 */
    #if (_CR_CC_VER_ < 0x0570)
        #define CR_BTEX_FAIL   -1
    #else
        #define CR_BTEX_FAIL    0
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持宽字符串函数的设置 */
    #define _CR_HAVE_WCS_
    /*------------------------------------------------*/

    /* 处理器架构的空指令宏设置 */
    #define CR_NOP  __asm nop
    /*------------------------------------------------*/

#endif  /* __BORLANDC__ || __TURBOC__ */

#endif  /* !__CR_BCC_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
