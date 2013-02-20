/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-15  */
/*     #######          ###    ###      [PORT]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-02-19  */
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
/*  >>>>>>>>>>>>>>> CrHack Microsoft C/C++ 编译器配置头文件 <<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_MSC_H__
#define __CR_MSC_H__ 0xE0D69CC2UL

/*****************************/
/* VC++  4.2 _MSC_VER = 1000 */
/* VC++  5.0 _MSC_VER = 1100 */
/* VC++  6.0 _MSC_VER = 1200 */
/* VC++  7.0 _MSC_VER = 1300 */
/* VC++  7.1 _MSC_VER = 1310 */
/* VC++  8.0 _MSC_VER = 1400 */
/* VC++  9.0 _MSC_VER = 1500 */
/* VC++ 10.0 _MSC_VER = 1600 */
/* VC++ 11.0 _MSC_VER = 1700 */
/*****************************/
#if defined(_MSC_VER)

    /* 编译器类型定义 */
    #define _CR_CC_MSC_
    /*------------------------------------------------*/

    /* 编译器版本定义 */
    #define _CR_CC_VER_         _MSC_VER
    /*------------------------------------------------*/

    /* 编译器名称定义 */
    #define _CR_CC_STR_     "Microsoft C/C++"
    /*------------------------------------------------*/

    /* 编译器版本过滤 */
    #if (_CR_CC_VER_ < 1300)
        #error "msc.h: CC TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器平台架构 */
    #if     defined(_M_X64)
        #define _CR_AR_X64_     /* AMD64 & EM64T */

    #elif   defined(_M_IX86)
        #define _CR_AR_X86_     /* Intel X86 */

    #elif   defined(_M_ARMT)
        #define _CR_AR_THUMB_   /* THUMB */

    #elif   defined(ARM) || \
            defined(_ARM_) || defined(_M_ARM)
        #define _CR_AR_ARM_     /* ARM */

    #elif   defined(_M_MRX000)
        #define _CR_AR_MIPS_    /* MIPS */

    #elif   defined(_M_PPC) || \
            defined(_M_MPPC)
        #define _CR_AR_PPC_     /* PowerPC (MAC) */

    #elif   defined(_M_IA64)
        #define _CR_AR_IA64_    /* IA64 */

    #elif   defined(_M_ALPHA)
        #define _CR_AR_ALPHA_   /* Alpha AXP */

    #else
        #error "msc.h: AR TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器操作系统 */
    #if defined(_WIN32_WCE)
        #define _CR_OS_MSWIN_
        #define _CR_OS_WINCE_

    #elif   defined(_WIN64)
        #define _CR_OS_MSWIN_
        #define _CR_OS_WIN64_

    #elif   defined(_WIN32)
        #define _CR_OS_MSWIN_
        #define _CR_OS_WIN32_
    #else
        #error "msc.h: OS TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器全局定义 */
    #if defined(_CR_OS_WINCE_)
        #ifndef BUFSIZ
            #define BUFSIZ  1024
        #endif
    #endif
    #if (_CR_CC_VER_ >= 1400)
        #ifndef _CRT_SECURE_NO_WARNINGS
            #define _CRT_SECURE_NO_WARNINGS
        #endif
        #ifndef _CRT_NONSTDC_NO_DEPRECATE
            #define _CRT_NONSTDC_NO_DEPRECATE
        #endif
    #endif
    /*------------------------------------------------*/

    /* 编译器多余警告 */
    #pragma warning (disable: 4054) /* 函数指针2数据指针 */
    #pragma warning (disable: 4055) /* 数据指针2函数指针 */
    #pragma warning (disable: 4201) /*  匿名结构体的警告 */
    #pragma warning (disable: 4214) /* 非 int 结构体位域 */

    /* VC6 和 WINCE SDK 缺少这个 */
    #if (_CR_CC_VER_ == 1200) || \
        (defined(_CR_OS_WINCE_) && \
        (_CR_CC_VER_ >= 1400) && (_CR_CC_VER_ <= 1500))
        struct _RPC_ASYNC_STATE;    /* 缺少结构体前置声明 */
    #endif
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
        #define _slw_
        #define _pge_
        #define _ram_
        #define _rom_
        #define _far_
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
    #if (_CR_CC_VER_ < 1310)
        #define CR_SLL(x)   x##I64
        #define CR_ULL(x)   x##uI64
    #else
        #define CR_SLL(x)   x##LL
        #define CR_ULL(x)   x##ULL
    #endif
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
    #define CR_INLINE   __forceinline
    /*------------------------------------------------*/

    /* 编译器函数导出修饰 */
    #ifndef _CR_OS_MSWIN_
        #define CR_EXPORT
    #else
        #define CR_EXPORT   __declspec(dllexport)
    #endif
    /*------------------------------------------------*/

    /* 编译器内联汇编风格 */
    #if defined(_CR_AR_X86_)
        #define _CR_ASM_INTL_
    #elif   0
        #define _CR_ASM_ATnT_
    #else
        #define _CR_ASM_SPEC_
    #endif
    /*------------------------------------------------*/

    /* 编译器noreturn修饰 */
    #if (_CR_CC_VER_ < 1200)
        #define CR_NORETURN
    #else
        #define CR_NORETURN __declspec(noreturn)
    #endif
    /*------------------------------------------------*/

    /* 编译器noinline修饰 */
    #if (_CR_CC_VER_ < 1300)
        #define CR_NOINLINE
    #else
        #define CR_NOINLINE __declspec(noinline)
    #endif
    /*------------------------------------------------*/

    /* 编译器成员对齐修饰 */
    #if (_CR_CC_VER_ < 1300)
        #define CR_ALIGN(x)
    #else
        #define CR_ALIGN(x) __declspec(align(x))
    #endif
    /*------------------------------------------------*/

    /* 编译器导入库的选项 */
    #if     0
        #define _CR_NO_PRAGMA_LIB_
    #endif
    /*------------------------------------------------*/

    /* 编译器紧凑结构修饰 */
    #define CR_PACKED
    #define CR_TYPEDEF  typedef
    #if     0
        #define _CR_NO_PRAGMA_PACK_
    #endif
    /*------------------------------------------------*/

    /* 编译器分支优化指示 */
    #define surely(x)   (x)
    #define mostly(x)   (x)
    #define rarely(x)   (x)
    /*------------------------------------------------*/

    /* 编译器不支持多线程 */
    #if !defined(_MT)
        #define _CR_NO_MT_
    #endif
    /*------------------------------------------------*/

    /* 编译器不支持宽字符 */
    #if     0
        #define _CR_NO_WIDE_
    #endif
    /*------------------------------------------------*/

    /* 编译器指令函数优化 */
    #if     0
        #define _CR_NO_CSWAP_
        #define _CR_NO_CROT32_
        #define _CR_NO_CROT64_
    #endif

    #if     (_CR_CC_VER_ < 1400)
        #define _CR_NO_IROTSM_
        #define _CR_NO_INTRIN_

    #elif   !defined(_CR_AR_X86_) && \
            !defined(_CR_AR_X64_) && \
            !defined(_CR_AR_IA64_)
        #define _CR_NO_IROTSM_
    #endif
    #define cr_rotl08       _rotl8
    #define cr_rotr08       _rotr8
    #define cr_rotl16       _rotl16
    #define cr_rotr16       _rotr16
    #define cr_rotl32       _rotl
    #define cr_rotr32       _rotr
    #define cr_rotl64       _rotl64
    #define cr_rotr64       _rotr64
    #define cr_byteswap16   _byteswap_ushort
    #define cr_byteswap32   _byteswap_ulong
    #define cr_byteswap64   _byteswap_uint64
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
    #define CR_I64  "I64"
    /*------------------------------------------------*/

    /* LIBC 是否有 errno.h 头文件 */
    #if defined(_CR_OS_WINCE_)
        #define _CR_NO_ERRNO_
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持64位STDIO文件偏移 */
    #if defined(_CR_OS_WINCE_)
        #define _CR_NO_STDIO64_
    #else
        #define _CR_MS_STDIO64_
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持64位POSIX文件偏移 */
    #if defined(_CR_OS_WINCE_)
        #define _CR_NO_POSIX64_
    #else
        #define _CR_MS_POSIX64_
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持 C99 数学函数设置 */
    #if (_CR_CC_VER_ < 1400)
        #ifndef __cplusplus
            #define _CR_NO_MATHC99_
        #endif
    #endif
    /*------------------------------------------------*/

    /* LIBC beginthreadex() 返回值 */
    #if !defined(_CR_NO_STDC_) && \
        !defined(_CR_OS_WINCE_)
        #define CR_BTEX_FAIL    0
    #endif
    /*------------------------------------------------*/

    /* LIBC 支持宽字符串函数的设置 */
    #ifndef _CR_NO_STDC_
        #define _CR_HAVE_WCS_
    #endif
    /*------------------------------------------------*/

    /* 处理器架构的空指令宏设置 */
    #if defined(_CR_AR_X86_)
        #define CR_NOP  __asm nop
    #endif
    /*------------------------------------------------*/

    /* 处理器架构的对齐访问设置 */
    #if !defined(_CR_AR_X86_) && \
        !defined(_CR_AR_X64_)   /* maybe more */
        #define _CR_ALIGN_NEEDED_
    #endif
    /*------------------------------------------------*/

    /* 处理器架构支持原子级操作 */
    #if     0
        #define _CR_FAST_ATOM_
    #endif
    /*------------------------------------------------*/

#endif  /* _MSC_VER */

#endif  /* !__CR_MSC_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
