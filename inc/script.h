/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-10-21  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 脚本解析头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_SCRIPT_H__
#define __CR_SCRIPT_H__

#include "defs.h"

/*****************************************************************************/
/*                                 通用变量                                  */
/*****************************************************************************/

/* 变量类型 */
#define CR_UV_VOID  0   /*  空的类型  */
#define CR_UV_INTS  1   /*  有符号数  */
#define CR_UV_INTU  2   /*  无符号数  */
#define CR_UV_REAL  3   /*  浮点实数  */
#define CR_UV_ANSI  4   /*  字符串-A  */
#define CR_UV_WIDE  5   /*  字符串-W  */
#define CR_UV_ADDR  6   /*  内存地址  */
#define CR_UV_FNOP  7   /* 无参数调用 */
#define CR_UV_FCCC  8   /* C 风格调用 */
#define CR_UV_FSTD  9   /* STD 的调用 */

/* 通用变量 */
typedef union
{
        dist_t      uv_ints;    /*  有符号数  */
        leng_t      uv_intu;    /*  无符号数  */
        real_t      uv_real;    /*  浮点实数  */
        ansi_t*     uv_ansi;    /*  字符串-A  */
        wide_t*     uv_wide;    /*  字符串-W  */
        void_t*     uv_addr;    /*  内存地址  */
        nopfunc_t   nopfunc;    /* 无参数调用 */
        cccfunc_t   cccfunc;    /* C 风格调用 */
        stdfunc_t   stdfunc;    /* STD 的调用 */

} uvar_t;

/* 通用函数 */
typedef sint_t  (*uv_func_t) (void_t*);

/* 变量模板 */
typedef struct
{
        uint_t  type;   /* 变量类型 */
        uvar_t  vals;   /* 变量取值 */
        size_t  size;   /* 数据大小 */
} sUVAR;

/* 设置各个类型变量 */
#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    设置空型变量
=======================================
*/
inline void_t
set_uv_void (
  __CR_OT__ sUVAR*  that
    )
{
    that->size = 0;
    that->type = CR_UV_VOID;
    that->vals.uv_ints = 0;
}

/*
=======================================
    设置有符号数
=======================================
*/
inline void_t
set_uv_ints (
  __CR_OT__ sUVAR*  that,
  __CR_IN__ dist_t  value
    )
{
    that->size = 0;
    that->type = CR_UV_INTS;
    that->vals.uv_ints = value;
}

/*
=======================================
    设置无符号数
=======================================
*/
inline void_t
set_uv_intu (
  __CR_OT__ sUVAR*  that,
  __CR_IN__ leng_t  value
    )
{
    that->size = 0;
    that->type = CR_UV_INTU;
    that->vals.uv_intu = value;
}

/*
=======================================
    设置浮点实数
=======================================
*/
inline void_t
set_uv_real (
  __CR_OT__ sUVAR*  that,
  __CR_IN__ real_t  value
    )
{
    that->size = 0;
    that->type = CR_UV_REAL;
    that->vals.uv_real = value;
}

/*
=======================================
    设置字符串-A
=======================================
*/
inline void_t
set_uv_ansi (
  __CR_OT__ sUVAR*          that,
  __CR_IN__ const ansi_t*   astr,
  __CR_IN__ size_t          size
    )
{
    that->size = size;
    that->type = CR_UV_ANSI;
    that->vals.uv_ansi = (ansi_t*)astr;
}

/*
=======================================
    设置字符串-W
=======================================
*/
inline void_t
set_uv_wide (
  __CR_OT__ sUVAR*          that,
  __CR_IN__ const wide_t*   wstr,
  __CR_IN__ size_t          size
    )
{
    that->size = size;
    that->type = CR_UV_WIDE;
    that->vals.uv_wide = (wide_t*)wstr;
}

/*
=======================================
    设置内存地址
=======================================
*/
inline void_t
set_uv_addr (
  __CR_OT__ sUVAR*          that,
  __CR_IN__ const void_t*   addr,
  __CR_IN__ size_t          size
    )
{
    that->size = size;
    that->type = CR_UV_ADDR;
    that->vals.uv_addr = (void_t*)addr;
}

/*
=======================================
    设置 C 风格调用
=======================================
*/
inline void_t
set_uv_ccall (
  __CR_OT__ sUVAR*  that,
  __CR_IN__ void_t* func,
  __CR_IN__ uint_t  nparm,
  __CR_IN__ uint_t  rtype
    )
{
    nparm &= 0xFF;
    rtype &= 0xFF;
    rtype <<= 8;    /* 低位放参数个数 */
    rtype |= nparm;
    that->size = (size_t)rtype;
    if (nparm == 0)
        that->type = CR_UV_FNOP;
    else
        that->type = CR_UV_FCCC;
    that->vals.uv_addr = func;
}

/*
=======================================
    设置 STD 风格调用
=======================================
*/
inline void_t
set_uv_scall (
  __CR_OT__ sUVAR*  that,
  __CR_IN__ void_t* func,
  __CR_IN__ uint_t  nparm,
  __CR_IN__ uint_t  rtype
    )
{
    nparm &= 0xFF;
    rtype &= 0xFF;
    rtype <<= 8;    /* 低位放参数个数 */
    rtype |= nparm;
    that->size = (size_t)rtype;
    if (nparm == 0)
        that->type = CR_UV_FNOP;
    else
        that->type = CR_UV_FSTD;
    that->vals.uv_addr = func;
}

#endif  /* !_CR_SICK_INLINE_ */

#endif  /* !__CR_SCRIPT_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
