/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-05-13  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack KeilC51 编译器配置头文件 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_CX51_H__
#define __CR_CX51_H__ 0x640F6D03UL

/*****************************/
/* 版本值 701 表示版本 V7.01 */
/*****************************/
#if defined(__C51__) || defined(__CX51__)

    /* 编译器类型定义 */
    #define _CR_CC_CX51_
    /*------------------------------------------------*/

    /* 编译器版本定义 */
    #ifndef __CX51__
        #define _CR_CC_VER_ __C51__
    #else
        #define _CR_CC_VER_ __CX51__
    #endif
    /*------------------------------------------------*/

    /* 编译器名称定义 */
    #define _CR_CC_STR_     "KeilC51"
    /*------------------------------------------------*/

    /* 编译器版本过滤 */
    #if (_CR_CC_VER_ < 701)
        #error "cx51.h: CC TYPE not supported yet!"
    #endif
    /*------------------------------------------------*/

    /* 编译器平台架构 */
    #define _CR_AR_MCS51_   /* Intel MCS-51 */
    /*------------------------------------------------*/

    /* 编译器操作系统 */
    #if     1
        #define _CR_OS_NAKED_
    #endif
    /*------------------------------------------------*/

    /* 编译器全局定义 */
    #if (_CR_CC_VER_ == 817)
        #ifdef  __CX2__
            #undef  __CX2__
            #define __CX2__ 1
        #else
            #define __CX2__ 0
        #endif
    #endif
    #ifndef _CR_USE_51_DATA_
        #define data    _nouse_data_
    #endif
    #define _CR_ORDER_BE_       /* 大端模式 */
    #define _CR_SICK_INLINE_    /* 剔除内联 */
    /*------------------------------------------------*/

    /* 编译器多余警告 */

    /*------------------------------------------------*/

    /* 编译器内存模型 */
    #if     1
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
    typedef bit     ubit_t;
    /*------------------------------------------------*/

    /* 编译器指针修饰 */
    #if     1
        #define _far_   far
        #define _rom_   code
        #define _slw_   idata
        #define _pge_   pdata
        #define _ram_   xdata
    #endif
    /*------------------------------------------------*/

    /* 编译器32位浮点 */
    #if     0
        #define _CR_NO_FLT32_
    #endif
    /*------------------------------------------------*/

    /* 编译器64位浮点 */
    #if     1
        #define _CR_NO_FLT64_
    #endif
    /*------------------------------------------------*/

    /* 编译器64位整数 */
    #if     1
        #define _CR_NO_INT64_
    #elif   0
        #define _CR_USE_LLONG_
    #else
        #define _CR_USE_INT64_
    #endif
    /*------------------------------------------------*/

    /* 编译器64位常数后缀 */
    #define CR_SLL(x)   x
    #define CR_ULL(x)   x
    /*------------------------------------------------*/

    /* 编译器内联函数修饰 */
    #define _CR_NO_INLINE_
    #define inline      static
    /*------------------------------------------------*/

    /* 编译器汇编内联函数 */
    #define fasm_inline static
    /*------------------------------------------------*/

    /* 编译器安全内联函数 */
    #define safe_inline static
    /*------------------------------------------------*/

    /* 编译器强迫内联修饰 */
    #define CR_INLINE   static
    /*------------------------------------------------*/

    /* 编译器函数导出修饰 */
    #if     1
        #define CR_EXPORT
        #define CR_IMPORT
    #endif
    /*------------------------------------------------*/

    /* 编译器内联汇编风格 */
    #if     0
        #define _CR_ASM_INTL_
    #elif   0
        #define _CR_ASM_ATnT_
    #else
        #define _CR_ASM_SPEC_
    #endif
    /*------------------------------------------------*/

    /* 编译器noreturn修饰 */
    #if     1
        #define CR_NORETURN
    #endif
    /*------------------------------------------------*/

    /* 编译器noinline修饰 */
    #if     1
        #define CR_NOINLINE
    #endif
    /*------------------------------------------------*/

    /* 编译器成员对齐修饰 */
    #if     1
        #define CR_ALIGN(x)
    #endif
    /*------------------------------------------------*/

    /* 编译器导入库的选项 */
    #if     1
        #define _CR_NO_PRAGMA_LIB_
    #endif
    /*------------------------------------------------*/

    /* 编译器紧凑结构修饰 */
    #define CR_PACKED
    #define CR_TYPEDEF  typedef
    #if     1
        #define _CR_NO_PRAGMA_PACK_
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
    #if     1
        #define _CR_NO_WIDE_
    #endif
    /*------------------------------------------------*/

    /* 编译器指令函数优化 */
    #define _CR_NO_CSWAP_
    #define _CR_NO_CROT64_
    #if     0
        #define _CR_NO_CROT32_
        #define _CR_NO_IROTSM_
        #define _CR_NO_INTRIN_
    #endif
    #define cr_rotl08   _crol_
    #define cr_rotr08   _cror_
    #define cr_rotl16   _irol_
    #define cr_rotr16   _iror_
    #define cr_rotl32   _lrol_
    #define cr_rotr32   _lror_
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
    #define CR_I08  "B"
    #define CR_I16
    #define CR_I32  "L"
    #if     0
        #define CR_I64
    #endif
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
    #define CR_NOP  _nop_()
    extern void _nop_ (void);
    /*------------------------------------------------*/

    /* 处理器架构的对齐访问设置 */
    #if     0
        #define _CR_ALIGN_NEEDED_
    #endif
    /*------------------------------------------------*/

    /* 处理器架构支持原子级操作 */
    #if     0
        #define _CR_FAST_ATOM_
    #endif
    /*------------------------------------------------*/

#endif  /* __C51__ || __CX51__ */

#endif  /* !__CR_CX51_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
