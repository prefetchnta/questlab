/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-15  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 公用定义头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DEFS_H__
#define __CR_DEFS_H__

#include "conf.h"

/*****************************************************************************/
/*                               编译环境定义                                */
/*****************************************************************************/

/* 函数调用规范 */
#if defined(_CR_AR_X86_) && \
    defined(_CR_OS_WIN32_)
    #define CCCCALL     __cdecl
    #define STDCALL     __stdcall
    #define REGCALL     __fastcall
#else
    #define CCCCALL
    #define STDCALL
    #define REGCALL
#endif

/* 纯 C 函数修饰 */
#ifdef  __cplusplus
    #define  CR_C_FUNC  extern "C"
    #define _CR_C_HEAD_ extern "C" {
    #define _CR_C_TAIL_ }
#else
    #define  CR_C_FUNC  extern
    #define _CR_C_HEAD_
    #define _CR_C_TAIL_
#endif

/* 建立和使用 DLL 所需的函数前缀 */
#define CR_DLL  CR_C_FUNC CR_EXPORT
#ifndef _CR_BUILD_DLL_
    #ifndef _CR_USE_DLL_
        #define CR_API  CR_C_FUNC
    #else
        #define CR_API  CR_C_FUNC CR_IMPORT
    #endif
#else
    #define CR_API  CR_C_FUNC CR_EXPORT
#endif

/* 基本类型定义 */
typedef   signed int    sint_t;
typedef unsigned int    uint_t;
typedef unsigned char   byte_t;

/* 16/32位整数定义 */
#if defined(_CR_SYS16_)
typedef   signed int    int16s;
typedef unsigned int    int16u;
typedef   signed long   int32s;
typedef unsigned long   int32u;
#else   /* (_CR_SYS32_) */
        /* (_CR_SYS64_) */
typedef   signed short  int16s;
typedef unsigned short  int16u;
typedef   signed int    int32s;
typedef unsigned int    int32u;
#endif

/* 64位整数定义 */
#if     defined(_CR_NO_INT64_)
    #include "port/int64.h"
#elif   defined(_CR_USE_INT64_)
typedef   signed __int64    int64s;
typedef unsigned __int64    int64u;
#else       /* (_CR_USE_LLONG_) */
typedef   signed long long  int64s;
typedef unsigned long long  int64u;
#endif

/* 平台最快类型 */
#ifndef _CR_ARCH8_
typedef   signed int    sfast_t;
typedef unsigned int    ufast_t;
#else
typedef   signed char   sfast_t;
typedef unsigned char   ufast_t;
#endif

/* 美观用的类型 */
#define void_t  void
typedef byte_t  uchar;
typedef int16u  wchar;
typedef  char   achar;
typedef achar   ansi_t;
typedef wchar   wide_t;
typedef ubit_t  retc_t;
typedef ufast_t bool_t;

/* 浮点数的类型 */
typedef int16s  fp16_t;
#ifndef _CR_NO_FLT32_
    typedef float   fp32_t;
#else
    typedef int32s  fp32_t;
#endif
#ifndef _CR_NO_FLT64_
    #if !defined(_CR_DOUBLE32_)
    typedef double      fp64_t;
    #else
    typedef long double fp64_t;
    #endif
#else
    typedef int64s  fp64_t;
#endif
#include "port/float.h"

/* 很大的浮点数 */
#define CR_LARG64   (1.0E30)
#define CR_LARG32   (1.0E18f)

/* 很小的浮点数 */
#define CR_ABIT64   (1.0E-10)
#define CR_ABIT32   (1.0E-06f)

/* 文件系统大小 */
#if defined(_CR_FILE32_)
    typedef int32s  fdist_t;
    typedef int32u  fsize_t;

    /* 关闭 GCC 64位文件开关 */
    #undef  _LARGE_FILE
    #undef  _LARGEFILE_SOURCE
    #undef  _LARGEFILE64_SOURCE
    #define _FILE_OFFSET_BITS   32

#else   /* (_CR_FILE64_) */
    typedef int64s  fdist_t;
    typedef int64u  fsize_t;

    /* 打开 GCC 64位文件开关 */
    #define _LARGE_FILE         1
    #define _LARGEFILE_SOURCE   1
    #define _LARGEFILE64_SOURCE 1
    #define _FILE_OFFSET_BITS   64
#endif

/* 内存容量大小 */
#if defined(_CR_MSIZE32_)
    typedef int32s  mdist_t;
    typedef int32u  msize_t;
#else   /* (_CR_MSIZE64_) */
    typedef int64s  mdist_t;
    typedef int64u  msize_t;
#endif

/* 平台相关类型 */
#if defined(_CR_SYS64_)
    typedef fp64_t  real_t;
    #define CR_LARG CR_LARG64
    #define CR_ABIT CR_ABIT64
    typedef int64s  dist_t;
    typedef int64u  leng_t;
#else   /* (_CR_SYS16_) */
        /* (_CR_SYS32_) */
    typedef fp32_t  real_t;
    #define CR_LARG CR_LARG32
    #define CR_ABIT CR_ABIT32
    #if defined(_CR_SMALL_)
    typedef int16s  dist_t;
    typedef int16u  leng_t;
    #else   /* (_CR_LARGE_) */
            /* (_CR_SYS32_) */
    typedef int32s  dist_t;
    typedef int32u  leng_t;
    #endif
#endif

/* 快速内存长度映射 */
#if defined(_CR_ARCH8_) && \
    defined(_CR_FAST_LENG_)
    #define dist_t  sfast_t
    #define leng_t  ufast_t
#endif

/* 平台最大类型 */
#if defined(_CR_NO_INT64_)
    typedef int32s  maxs_t;
    typedef int32u  maxu_t;
    #define CR_MAXU64(n)    ((n).lo32)
#else
    typedef int64s  maxs_t;
    typedef int64u  maxu_t;
    #define CR_MAXU64(n)    (n)
#endif

/* for size_t */
#ifndef _CR_NO_STDC_
    #include <stddef.h>
#else
    #if     defined(_CR_SYS16_)
        typedef int16u  size_t;
        typedef int16s  ptrdiff_t;
    #elif   defined(_CR_SYS32_)
        typedef int32u  size_t;
        typedef int32s  ptrdiff_t;
    #else       /* (_CR_SYS64_) */
        typedef int64u  size_t;
        typedef int64s  ptrdiff_t;
    #endif
#endif

/* 特殊应用类型 */
typedef int32u      cl32_t;     /* 标准32位色 (蓝 先) */
typedef void_t*     hwnd_t;     /* GUI 相关的窗口句柄 */

/* 三种函数类型 (返回指针防止可能的值截断) */
typedef void_t* (CCCCALL *nopfunc_t) (void_t);
typedef void_t* (CCCCALL *cccfunc_t) (void_t*, ...);
typedef void_t* (STDCALL *stdfunc_t) (void_t*, void_t*);

/* 编译器库函数差异映射 */
#if defined(_CR_DOUBLE32_) || defined(_CR_NO_MATHC99_)
    #define  FSIN(x)    ((fp32_t) sin(x))
    #define  FCOS(x)    ((fp32_t) cos(x))
    #define  FTAN(x)    ((fp32_t) tan(x))
    #define FASIN(x)    ((fp32_t)asin(x))
    #define FACOS(x)    ((fp32_t)acos(x))
    #define FATAN(x)    ((fp32_t)atan(x))
    #define FSQRT(x)    ((fp32_t)sqrt(x))
    #define FPOW(x,y)   ((fp32_t)pow(x,y))
#else
    #define  FSIN(x)    ((fp32_t) sinf(x))
    #define  FCOS(x)    ((fp32_t) cosf(x))
    #define  FTAN(x)    ((fp32_t) tanf(x))
    #define FASIN(x)    ((fp32_t)asinf(x))
    #define FACOS(x)    ((fp32_t)acosf(x))
    #define FATAN(x)    ((fp32_t)atanf(x))
    #define FSQRT(x)    ((fp32_t)sqrtf(x))
    #define FPOW(x,y)   ((fp32_t)powf(x,y))
#endif

/*****************************************************************************/
/*                               美化代码定义                                */
/*****************************************************************************/

/* 函数参数作用符 */
#define __CR_IN__   /* 输入参数 */
#define __CR_OT__   /* 输出参数 */
#define __CR_IO__   /* 输入输出 */
#define __CR_UU__   /* 无用参数 */

/* 函数参数缺省值宏 */
#ifdef  __cplusplus
    #define CR_DEFAULT(v)   =(v)
#else
    #define CR_DEFAULT(v)
#endif

/* 无用参数和代码消除警告宏 */
#if !defined(_CR_CC_BCC_)
    #define CR_NOUSE(v)     ((v) = (v))
#else
    #define CR_NOUSE(v) fmj_xzz((uchar*)(v))
/*
=======================================
    取消无用参数的警告
=======================================
*/
cr_inline void_t*
fmj_xzz (
  __CR_UU__ void_t* nouse
    )
{
    return ((uchar*)nouse);
}

#endif  /* !_CR_CC_BCC_ */

/* 数据单位转换宏 */
#define CR_PI       3.1415926535898f
#define CR_DTOR     (CR_PI / 180.0f)
#define CR_RTOD     (180.0f / CR_PI)
#define CR_B2K(n)   ((n) / (1024UL))
#define CR_K2B(n)   ((n) * (1024UL))
#define CR_B2M(n)   ((n) / (1024UL * 1024UL))
#define CR_M2B(n)   ((n) * (1024UL * 1024UL))
#define CR_M2S(x)   ((x) * (60UL))
#define CR_S2M(x)   ((x) / (60UL))
#define CR_H2S(x)   ((x) * (60UL * 60UL))
#define CR_S2H(x)   ((x) / (60UL * 60UL))
#define CR_D2S(x)   ((x) * (24UL * 60UL * 60UL))
#define CR_S2D(x)   ((x) / (24UL * 60UL * 60UL))

/* 类型的位数宏 */
#define bitsof(t)   (sizeof(t) * 8)

/* 数组元素的个数宏 */
#define cntsof(a)   (sizeof(a) / sizeof(a[0]))

/* 结构体成员偏移宏 */
#define offsof(s,m) ((size_t)(&(((s _rom_*)0)->m)))

/* 内部版本的名称映射 */
#if     defined(_CR_BUILD_ANSI_)
    #define CR_XC   CR_AC
    #define CR_XN   CR_AC
    #define CR_XS   CR_AS
    #define XCHAR   ansi_t
    #define CR_FAW  CR_FUNC_A
    #define CR_FUW  CR_FUNC_U
    #define CR_TAW  CR_TYPE_A
    #define CR_TUW  CR_TYPE_U
#elif   defined(_CR_BUILD_WIDE_)
    #define CR_XC   CR_WC
    #define CR_XN   CR_NC
    #define CR_XS   CR_WS
    #define XCHAR   wide_t
    #define CR_FAW  CR_FUNC_W
    #define CR_FUW  CR_FUNC_W
    #define CR_TAW  CR_TYPE_W
    #define CR_TUW  CR_TYPE_W
#endif

/* 函数/变量类型的名称后缀宏 */
#define CR_FUNC_A(name)     name##A
#define CR_FUNC_U(name)     name##U
#define CR_FUNC_W(name)     name##W
#define CR_TYPE_A(name)     name##A_t
#define CR_TYPE_U(name)     name##U_t
#define CR_TYPE_W(name)     name##W_t

/* 字符串常数包装宏 (去除可能的警告) */
#if defined(_CR_NO_WIDE_)
    #define CR_AC(c)    ((ansi_t)(c))
    #define CR_NC(c)    ((wide_t)(c))
    #define CR_WC(c)    ((wide_t)(c))
    #define CR_AS(s)    ((const ansi_t*)(s))
#else
    #define CR_AC(c)    ((ansi_t)(   c))
    #define CR_NC(c)    ((wide_t)(   c))
    #define CR_WC(c)    ((wide_t)(L##c))
    #define CR_AS(s)    ((const ansi_t*)(   s))
    #define CR_WS(s)    ((const wide_t*)(L##s))
#endif

/*****************************************************************************/
/*                               有用的宏定义                                */
/*****************************************************************************/

/* 常用常数 */
#undef  NIL
#define NIL       '\0'
#define CR_NIL    '\0'

#undef  NULL
#define NULL        0
#define CR_NULL     0

#undef  TRUE
#define TRUE        1
#define CR_TRUE     1

#undef  FALSE
#define FALSE       0
#define CR_FALSE    0

/* printf() 文件大小修饰符 */
#if defined(_CR_FILE32_)
    #define CR_FSZ  CR_I32
#else   /* (_CR_FILE64_) */
    #define CR_FSZ  CR_I64
#endif

/* printf() 内存大小修饰符 */
#if defined(_CR_MSIZE32_)
    #define CR_MSZ  CR_I32
#else   /* (_CR_MSIZE64_) */
    #define CR_MSZ  CR_I64
#endif

/* printf() 最大整数修饰符 */
#if defined(_CR_NO_INT64_)
    #define CR_MX   CR_I32
#else
    #define CR_MX   CR_I64
#endif

/* printf() 平台大小修饰符 */
#if defined(_CR_SYS64_)
    #define CR_SZ   CR_I64
#else   /* (_CR_SYS16_) */
        /* (_CR_SYS32_) */
    #if defined(_CR_SMALL_)
        #define CR_SZ
    #else   /* (_CR_LARGE_) */
            /* (_CR_SYS32_) */
        #define CR_SZ   CR_I32
    #endif
#endif

/* 寄存器大小端偏移宏 */
#ifndef _CR_ORDER_BE_
    #define oREG08  0
    #define oREG16  0
#else
    #define oREG08  3
    #define oREG16  2
#endif

/* 常用编码值 CodePage */
#define CR_GBK      936
#define CR_KOR      949
#define CR_SJIS     932
#define CR_BIG5     950
#define CR_LOCAL    0x00
#define CR_UTF16LE  1200
#define CR_UTF16BE  1201
#define CR_UTF32LE  12000
#define CR_UTF32BE  12001
#define CR_UTF7     65000
#define CR_UTF8     65001
#ifndef _CR_ORDER_BE_
    #define CR_UTF16X   CR_UTF16LE
    #define CR_UTF32X   CR_UTF32LE
#else
    #define CR_UTF16X   CR_UTF16BE
    #define CR_UTF32X   CR_UTF32BE
#endif

/* CodePage 码值判定宏 */
#define is_cr_utf16(cp) \
    ((cp) == CR_UTF16LE || (cp) == CR_UTF16BE)
#define is_cr_utf32(cp) \
    ((cp) == CR_UTF32LE || (cp) == CR_UTF32BE)
#define is_cr_unicode(cp) \
    ((cp) == CR_UTF7 || (cp) == CR_UTF8 || \
     (cp) == CR_UTF16LE || (cp) == CR_UTF16BE || \
     (cp) == CR_UTF32LE || (cp) == CR_UTF32BE)
#define is_cr_widechar(cp) \
    ((cp) == CR_UTF16LE || (cp) == CR_UTF16BE || \
     (cp) == CR_UTF32LE || (cp) == CR_UTF32BE)
#define is_cr_multibyte(cp) \
    ((cp) != CR_UTF16LE && (cp) != CR_UTF16BE && \
     (cp) != CR_UTF32LE && (cp) != CR_UTF32BE)

/* 内存分片常数 */
#if     defined(_CR_SYS64_)
    #define CR_SPLITE   CR_M2B(64UL)
#elif   defined(_CR_SYS32_)
    #define CR_SPLITE   (0x00000000)
#else       /* (_CR_SYS16_) */
    #if defined(_CR_SMALL_)
        #define CR_SPLITE   (0x0000)
    #else   /* (_CR_LARGE_) */
        #define CR_SPLITE   CR_K2B(64UL)
    #endif
#endif

/* BOM 标志字符串 */
#define BOM_UTF8    "\xEF\xBB\xBF"
#define BOM_UTF16LE "\xFF\xFE"
#define BOM_UTF16BE "\xFE\xFF"
#define BOM_UTF32LE "\xFF\xFE\x00\x00"
#define BOM_UTF32BE "\x00\x00\xFE\xFF"

/* 二进制格式转换 */
#ifndef _CR_ARCH8_
    #include "port/cnvrt.h"
#endif

/* 虚函数调用前缀 */
#define CR_VFUNC(ptr)   ((void_t*)(ptr))
#define CR_VCALL(obj)   ((obj)->__vptr__)

/* 外设 I/O 地址 */
#define ADDR_SLW_U08    ((byte_t volatile _slw_*)0)
#define ADDR_PGE_U08    ((byte_t volatile _pge_*)0)
#define ADDR_RAM_U08    ((byte_t volatile _ram_*)0)
#define ADDR_ROM_U08    ((byte_t volatile _rom_*)0)
#define ADDR_FAR_U08    ((byte_t volatile _far_*)0)
#define ADDR_SLW_U16    ((int16u volatile _slw_*)0)
#define ADDR_PGE_U16    ((int16u volatile _pge_*)0)
#define ADDR_RAM_U16    ((int16u volatile _ram_*)0)
#define ADDR_ROM_U16    ((int16u volatile _rom_*)0)
#define ADDR_FAR_U16    ((int16u volatile _far_*)0)
#define ADDR_SLW_U32    ((int32u volatile _slw_*)0)
#define ADDR_PGE_U32    ((int32u volatile _pge_*)0)
#define ADDR_RAM_U32    ((int32u volatile _ram_*)0)
#define ADDR_ROM_U32    ((int32u volatile _rom_*)0)
#define ADDR_FAR_U32    ((int32u volatile _far_*)0)
#define ADDR_SLW_U64    ((int64u volatile _slw_*)0)
#define ADDR_PGE_U64    ((int64u volatile _pge_*)0)
#define ADDR_RAM_U64    ((int64u volatile _ram_*)0)
#define ADDR_ROM_U64    ((int64u volatile _rom_*)0)
#define ADDR_FAR_U64    ((int64u volatile _far_*)0)

/* 外设 I/O 操作 */
#define IO_INT08(addr)  (*((volatile byte_t*)(addr)))
#define IO_INT16(addr)  (*((volatile int16u*)(addr)))
#define IO_INT32(addr)  (*((volatile int32u*)(addr)))
#define IO_INT64(addr)  (*((volatile int64u*)(addr)))

/* 常用数值操作 */
#define CR_ABS(x)       (((x) <  0 ) ? -(x) : (x))
#define CR_MIN(x,y)     (((x) > (y)) ?  (y) : (x))
#define CR_MAX(x,y)     (((x) > (y)) ?  (x) : (y))
#define CR_TEST2N(val)  (((val) & ((val) - 1)) == 0)
#define CR_SWAP(x,y,t)  (((t) = (x)),  ((x) = (y)), ((y) = (t)))
#define CR_EXTEND(v,n)  ( (n)    - ( (size_t)(v) & ((n)-1)))
#define CR_PADDED(v,n)  (((n)-1) - (((size_t)(v) + ((n)-1)) & ((n)-1)))

/* 安全释放操作 */
#define TRY_FREE(x)     { if ((x) != NULL) mem_free   (x); }
#define TRY_PDEL(x)     { if ((x) != NULL) delete     (x); }
#define TRY_ADEL(x)     { if ((x) != NULL) delete []  (x); }
#define TRY_XREL(x)     { if ((x) != NULL) (x)->Release(); }

#define SAFE_FREE(x)    { if ((x) != NULL) (mem_free   (x), (x) = NULL); }
#define SAFE_PDEL(x)    { if ((x) != NULL) (delete     (x), (x) = NULL); }
#define SAFE_ADEL(x)    { if ((x) != NULL) (delete []  (x), (x) = NULL); }
#define SAFE_XREL(x)    { if ((x) != NULL) ((x)->Release(), (x) = NULL); }

/*****************************************************************************/
/*                               出错处理定义                                */
/*****************************************************************************/

/* 错误信息结构 */
typedef struct
{
        int32u          file;   /* 出错的文件编号 */
        uint_t          line;   /* 出错的代码行数 */
        int32u          l3rd;   /* 第三方返回代码 */
        const ansi_t*   func;   /* 出错的函数名称 */
        const ansi_t*   text;   /* 出错的文本提示 */

} sERROR;

#define CR_ERROR            (-1L)
#define CR_U_ERROR      (( uint_t)-1)
#define CR_F_ERROR      ((fsize_t)-1)
#define CR_P_ERROR      (( leng_t)-1)
#define CR_ERRS(id) (((id) << 16) | __LINE__)
#define CR_FAIL(id) (((id) >> 16) & 0xFFFFUL)
#define CR_SUCC(id)     (!CR_FAIL(id))

/* 错误处理回调 */
typedef void_t (*errlog_t) (const sERROR*);

CR_API void_t   error_hook (errlog_t func);
CR_API void_t   error_set (int32u file, uint_t line, int32u l3rd,
                           const ansi_t *func, const ansi_t *text);

/*****************************************************************************/
/*                               杂项公用定义                                */
/*****************************************************************************/

/* 架构优化定义 */
#ifndef _CR_ARCH8_
    #include "arch.h"
#endif

/* 对象标识类型 */
typedef const ansi_t*   port_t;

/* 公用的接口表 */
typedef struct
{
        /* 所有接口开始必须是这两个 */
        void_t  (*release) (void_t *that);
        void_t* (*getMore) (void_t *that, port_t iid);

} iPORT_vtbl;

/* 公用对象结构 */
typedef struct  _iPORT
{
        /* 虚函数表 */
        const iPORT_vtbl*   __vptr__;
} iPORT;

/* 对齐平台需要的头文件 */
#if defined(_CR_ALIGN_NEEDED_)
    #include "memlib.h"
#endif

/* 三种数据变换接口类型 */
typedef leng_t  (*enc_self_t) (void_t*, leng_t);
typedef leng_t  (*enc_copy_t) (void_t*, leng_t, const void_t*, leng_t);
typedef leng_t  (*enc_parm_t) (void_t*, leng_t, const void_t*, leng_t,
                               void_t*);

#endif  /* !__CR_DEFS_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
