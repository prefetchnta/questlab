/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-04-17  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack GNU C/C++ 编译器配置头文件 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_GCC_H__
#define __CR_GCC_H__

/****************************************/
/* 考虑到 GCC 可能有多种平台的兼容版本  */
/* 所以其配置头文件放到头文件列表的最后 */
/****************************************/
#if defined(__GNUC__) || defined(__clang__)

    /* 编译器类型定义 */
    #define _CR_CC_GCC_
    /*------------------------------------------------*/

    /* 编译器版本定义 */
    #if defined(__clang__)
        #ifndef __clang_patchlevel__
            #define _CR_CC_VER_ (__clang_major__ * 10000 + \
                                 __clang_minor__ * 100)
        #else
            #define _CR_CC_VER_ (__clang_major__ * 10000 + \
                                 __clang_minor__ * 100 + \
                                 __clang_patchlevel__)
        #endif
    #else
        #ifndef __GNUC_PATCHLEVEL__
            #define _CR_CC_VER_ (__GNUC__ * 10000 + \
                                 __GNUC_MINOR__ * 100)
        #else
            #define _CR_CC_VER_ (__GNUC__ * 10000 + \
                                 __GNUC_MINOR__ * 100 + \
                                 __GNUC_PATCHLEVEL__)
        #endif
    #endif
    /*------------------------------------------------*/

    /* 编译器名称定义 */
    #if     defined(__clang__)
        #define _CR_CC_CLANG_
        #define _CR_CC_STR_     "CLANG"

    #elif   defined(__MINGW64__)
        #define _CR_CC_MINGW_
        #define _CR_CC_MINGW64_
        #define _CR_CC_STR_     "GCC (MinGW64)"

    #elif   defined(__MINGW32__)
        #define _CR_CC_MINGW_
        #define _CR_CC_MINGW32_
        #define _CR_CC_STR_     "GCC (MinGW32)"

    #elif   defined(__GO32__) || \
            defined(__DJGPP__)
        #define _CR_CC_DJGPP_
        #define _CR_CC_STR_     "GCC (DJGPP)"

    #elif   defined(__CYGWIN__)
        #define _CR_CC_CYGWIN_
        #define _CR_CC_STR_     "GCC (Cygwin)"

    #else
        #define _CR_CC_STR_     "GCC (GNU)"
    #endif
    /*------------------------------------------------*/

    /* 编译器版本过滤 */
    #if (_CR_CC_VER_ <= 40100)
        #error "gcc.h: CC TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器平台架构 */
    #if     defined(__amd64) || defined(__amd64__) || \
            defined(__x86_64) || defined(__x86_64__)
        #define _CR_AR_X64_     /* AMD64 & EM64T */

    #elif   defined(i386) || defined(__i386) || \
            defined(__i386__) || defined(_X86_)
        #define _CR_AR_X86_     /* Intel X86 */

    #elif   defined(__arm) || defined(__arm__) || \
            defined(__aarch64) || defined(__aarch64__) || \
            defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7S__) || \
            defined(__ARM_ARCH_ISA_A64)
        #define _CR_AR_ARM_     /* ARM */
        #if     defined(__ARM_ARCH_ISA_A64) || \
                defined(__aarch64) || defined(__aarch64__)
            #define _CR_AR_ARM64_
        #elif   defined(__ARM_ARCH_7S__)
            #define _CR_AR_ARM7S_
        #elif   defined(__thumb) || defined(__thumb__)
            #define _CR_AR_THUMB_   /* THUMB */
        #endif
    #elif   defined(mips) || defined(__mips__)
        #define _CR_AR_MIPS_    /* MIPS */
        #if     defined(__mips64)
            #define _CR_AR_MIPS64_  /* MIPS64 */
        #elif   defined(__mips16)
            #define _CR_AR_MIPS16_  /* MIPS16e */
        #endif
    #elif   defined(__ppc__) || defined(__powerpc) || \
            defined(__powerpc__) || defined(__POWERPC__)
        #define _CR_AR_PPC_     /* PowerPC */

    #elif   defined(__sparc__)
        #define _CR_AR_SPARC_   /* SPARC */

    #elif   defined(__sh__)
        #define _CR_AR_SH_      /* SuperH */

    #elif   defined(__m68k__)
        #define _CR_AR_68K_     /* M68000 */

    #elif   defined(__ia64__) || \
            defined(_IA64) || defined(__IA64__)
        #define _CR_AR_IA64_    /* IA64 */

    #elif   defined(__hppa__) || defined(__hppa)
        #define _CR_AR_HPPA_    /* HP/PA RISC */

    #elif   defined(__alpha__)
        #define _CR_AR_ALPHA_   /* Alpha AXP */

    #elif   defined(__ADSPBLACKFIN__) || \
            defined(__bfin) || defined(__bfin__) || \
            defined(__BFIN) || defined(__BFIN__)
        #define _CR_AR_ADSPBF_  /* Blackfin */
    #else
        #error "gcc.h: AR TYPE not supported yet!"
    #endif

    /* ARM 架构版本 */
    #if defined(_CR_AR_ARM_)
        #if     defined(_CR_AR_ARM64_)
            #define _CR_ARM_V16_    4
            #define _CR_ARM_V32_    8
        #elif   defined(__ARM_ARCH_7A__) || \
                defined(__ARM_ARCH_7S__)
            #define _CR_ARM_V16_    4
            #define _CR_ARM_V32_    7
        #elif   defined(__ARM_ARCH)
            #define _CR_ARM_V32_    __ARM_ARCH
            #if (_CR_ARM_V32_ >= 7)
                #define _CR_ARM_V16_    4
            #else
                #define _CR_ARM_V16_    0
            #endif
        #endif
    #endif
    /*------------------------------------------------*/

    /* 编译器操作系统 */
    #if     defined(_WIN32_WCE)
        #define _CR_OS_MSWIN_
        #define _CR_OS_WINCE_

    #elif   defined(WIN64) || defined(_WIN64) || \
            defined(__WIN64) || defined(__WIN64__)
        #define _CR_OS_MSWIN_
        #define _CR_OS_WIN64_

    #elif   defined(WIN32) || defined(_WIN32) || \
            defined(__WIN32) || defined(__WIN32__)
        #define _CR_OS_MSWIN_
        #define _CR_OS_WIN32_

    #elif   defined(MSDOS) || defined(__MSDOS) || \
            defined(__MSDOS__)
        #define _CR_OS_DOS32_

    #elif   defined(__APPLE__)
        #define _CR_OS_UNIX_
        #define _CR_OS_MACOSX_
        #if     (defined(TARGET_OS_IOS) && TARGET_OS_IOS) || \
                (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE)
            #define _CR_OS_IOS_
        #elif   (defined(TARGET_OS_WATCH) && TARGET_OS_WATCH)
            #define _CR_OS_WATCHOS_
        #endif

    /* *NIX 操作系统必须放在最后 */
    #elif   defined(unix) || defined(__unix) || \
            defined(__unix__) || defined(linux) || \
            defined(__linux) || defined(__linux__)
        #define _CR_OS_UNIX_

        #if     defined(__ANDROID__)
            #define _CR_OS_ANDROID_

        #elif   defined(linux) || defined(__linux) || \
            defined(__linux__)
            #define _CR_OS_LINUX_
        #endif
    #else
        #define _CR_OS_NAKED_   /* 最后是裸机 */
    #endif
    /*------------------------------------------------*/

    /* 编译器全局定义 */
    #if defined(_CR_OS_WIN32_)
        /* 最低要求为 Windows 2000 */
        #if     !defined(WINVER)
            #define WINVER  0x0500
        #elif   (WINVER < 0x0500)
            #undef  WINVER
            #define WINVER  0x0500
        #endif
        #if     !defined(_WIN32_WINNT)
            #define _WIN32_WINNT    0x0500
        #elif   (_WIN32_WINNT < 0x0500)
            #undef  _WIN32_WINNT
            #define _WIN32_WINNT    0x0500
        #endif
    #endif
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
    #define _CR_USE_LLONG_
    #undef  _CR_USE_INT64_
    /*------------------------------------------------*/

    /* 编译器64位常数后缀 */
    #define CR_SLL(x)   x##LL
    #define CR_ULL(x)   x##ULL
    /*------------------------------------------------*/

    /* 编译器内联函数修饰 */
    #undef  _CR_NO_INLINE_
    #define cr_inline   static __inline
    /*------------------------------------------------*/

    /* 编译器汇编内联函数 */
    #define fasm_inline     cr_inline
    /*------------------------------------------------*/

    /* 编译器安全内联函数 */
    #define safe_inline     cr_inline
    /*------------------------------------------------*/

    /* 编译器函数导出修饰 */
    #ifndef _CR_OS_MSWIN_
        #define CR_EXPORT
        #define CR_IMPORT
    #else
        #define CR_EXPORT   __attribute__((dllexport))
        #define CR_IMPORT   __attribute__((dllimport))
    #endif
    /*------------------------------------------------*/

    /* 编译器内联汇编风格 */
    #undef  _CR_ASM_INTL_
    #define _CR_ASM_ATnT_
    /*------------------------------------------------*/

    /* 编译器成员对齐修饰 */
    #define CR_ALIGN(x) __attribute__((aligned(x)))
    /*------------------------------------------------*/

    /* 编译器导入库的选项 */
    #define _CR_NO_PRAGMA_LIB_
    /*------------------------------------------------*/

    /* 编译器紧凑结构修饰 */
    #define CR_TYPEDEF  typedef
    #if defined(_CR_CC_MINGW_)
        #define CR_PACKED
        #undef  _CR_NO_PRAGMA_PACK_
    #else
        #define CR_PACKED   __attribute__((packed))
        #define _CR_NO_PRAGMA_PACK_
    #endif
    /*------------------------------------------------*/

    /* 编译器分支优化指示 */
    #if (_CR_CC_VER_ < 20900)
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
    #if defined(_CR_OS_DOS32_) || \
        defined(_CR_OS_NAKED_)
        #define _CR_NO_MT_
    #endif
    /*------------------------------------------------*/

    /* 编译器不支持宽字符 */
    #undef  _CR_NO_WIDE_
    /*------------------------------------------------*/

    /* 编译器指令函数优化 */
    #if (_CR_CC_VER_ >= 40800)
        #define cr_byteswap16   __builtin_bswap16
    #endif
    #if (_CR_CC_VER_ >= 40300)
        #define cr_byteswap32   __builtin_bswap32
        #define cr_byteswap64   __builtin_bswap64
    #endif
    /*------------------------------------------------*/

    /* LIBC printf() 整数宽度前缀 */
    #define CR_I32
    #ifndef _CR_CC_MINGW_
        #define CR_I64  "ll"
    #else
        #define CR_I64  "I64"
    #endif
    /*------------------------------------------------*/

    /* LIBC 是否有 errno.h 头文件 */
    #if defined(_CR_OS_WINCE_)
        #define _CR_NO_ERRNO_
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持64位STDIO文件偏移 */
    #if defined(_CR_OS_DOS32_) || \
        defined(_CR_OS_MSWIN_) || \
        defined(_CR_OS_ANDROID_)
        #define _CR_NO_STDIO64_
    #endif
    #undef  _CR_MS_STDIO64_
    /*------------------------------------------------*/

    /* LIBC 支持 C99 数学函数设置 */
    #undef  _CR_NO_MATHC99_
    /*------------------------------------------------*/

    /* LIBC beginthreadex() 返回值 */
    #if defined(_CR_OS_MSWIN_)
        #define CR_BTEX_FAIL    0
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持宽字符串函数的设置 */
    #if defined(_CR_OS_MSWIN_)
        #define _CR_HAVE_WCS_
    #endif
    /*------------------------------------------------*/

    /* 处理器架构的空指令宏设置 */
    #define CR_NOP  __asm__("nop")
    /*------------------------------------------------*/

#endif  /* __GNUC__ */

#endif  /* !__CR_GCC_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
