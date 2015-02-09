/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 杂项操作头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_MSCLIB_H__
#define __CR_MSCLIB_H__

#include "defs.h"

/*****************************************************************************/
/*                                 杂项函数                                  */
/*****************************************************************************/

/* 普通循环延时 */
CR_API void_t   delay08 (byte_t num);
CR_API void_t   delay16 (int16u num);
CR_API void_t   delay32 (int32u num);

/* 伪随机数生成 */
CR_API void_t   rand_seed (uint_t seed);
CR_API sint_t   rand_get (void_t);
CR_API sint_t   rand_getx (sint_t mod);

/* 快速数值计算 */
CR_API int16u   sqrt_int32 (int32u val);

/* 快速排序算法 */
CR_API void_t   quick_sort (void_t *base, leng_t num, leng_t width,
                            sint_t (*comp)(const void_t*, const void_t*));
/* BCD 二进制互换 */
CR_API byte_t   bcd2bin (byte_t bcd);
CR_API byte_t   bin2bcd (byte_t bin);
CR_API bool_t   bcd2bin_okay (byte_t bcd);
CR_API bool_t   bin2bcd_okay (byte_t bin);

/* 填充1到最高位 */
CR_API byte_t   fill2top08 (byte_t val);
CR_API int16u   fill2top16 (int16u val);
CR_API int32u   fill2top32 (int32u val);
CR_API int64u   fill2top64 (int64u val);

/* 数值置1位数统计 */
CR_API uint_t   count_bits08 (byte_t val);
CR_API uint_t   count_bits16 (int16u val);
CR_API uint_t   count_bits32 (int32u val);
CR_API uint_t   count_bits64 (int64u val);

/* 最大最小公约数计算 */
CR_API uint_t   numb_gcd   (uint_t a, uint_t b);
CR_API uint_t   numb_lcm   (uint_t a, uint_t b);
CR_API int32u   numb_gcd32 (int32u a, int32u b);
CR_API int32u   numb_lcm32 (int32u a, int32u b);
CR_API int64u   numb_gcd64 (int64u a, int64u b);
CR_API int64u   numb_lcm64 (int64u a, int64u b);

/* 字节数组相关操作 */
CR_API void_t   bytes_swap (void_t *data, leng_t size, leng_t block);
CR_API void_t   bytes_fill (void_t *data, leng_t size, leng_t start,
                            byte_t (*fill)(leng_t, leng_t));
/* 位串相关操作 */
CR_API byte_t   bits_swap08 (byte_t val, ufast_t bits);
CR_API int16u   bits_swap16 (int16u val, ufast_t bits);
CR_API int32u   bits_swap32 (int32u val, ufast_t bits);
CR_API int64u   bits_swap64 (int64u val, ufast_t bits);

/* 数码管字符串生成器 */
CR_API byte_t*  seg7dp_gen (byte_t *dst, const ansi_t *src,
                            ubit_t neg, leng_t size);

/*****************************************************************************/
/*                                有限状态机                                 */
/*****************************************************************************/

/* 整数型状态单元结构 */
typedef struct  _sFSM_UNIT_N
{
        ufast_t     prev;   /* 上一个状态 */
        ufast_t     name;   /* 状态的名称 */
        ufast_t     next;   /* 下一个状态 */
        void_t*     parm;   /* 状态上下文 */

        /* 状态表现与状态处理回调 */
        void_t  (*fsm_render) (struct _sFSM_UNIT_N *that, void_t *param);
        bool_t  (*fsm_handle) (struct _sFSM_UNIT_N *that, void_t *param);

} sFSM_UNIT_N;

/* 整数型状态机 */
typedef struct
{
        ufast_t         crrnt;  /* 当前状态 */
        ufast_t         count;  /* 状态总数 */
        sFSM_UNIT_N*    lists;  /* 状态列表 */

} sFSM_N;

CR_API void_t   fsm_n_sstep (sFSM_N *nfsm, void_t *param);
CR_API bool_t   fsm_n_sgoto (sFSM_N *nfsm, ufast_t sname);
CR_API bool_t   fsm_n_start (sFSM_N *nfsm, ufast_t entry,
                             ufast_t count, sFSM_UNIT_N *state);
/* 名称型状态类型 */
typedef void_t*         xfsm_t;
typedef const ansi_t*   xnfsm_t;

/* 名称型状态单元结构 */
typedef struct  _sFSM_UNIT_X
{
        xnfsm_t     prev;   /* 上一个状态 */
        xnfsm_t     name;   /* 状态的名称 */
        xnfsm_t     next;   /* 下一个状态 */
        void_t*     parm;   /* 状态上下文 */

        /* 状态表现与状态处理回调 */
        void_t  (*fsm_render) (struct _sFSM_UNIT_X *that, void_t *param);
        bool_t  (*fsm_handle) (struct _sFSM_UNIT_X *that, void_t *param);

} sFSM_UNIT_X;

CR_API void_t   fsm_x_free (xfsm_t xfsm);
CR_API xfsm_t   fsm_x_start (xnfsm_t entry, leng_t count,
                             sFSM_UNIT_X *state);
CR_API void_t   fsm_x_sstep (xfsm_t xfsm, void_t *param);
CR_API bool_t   fsm_x_sgoto (xfsm_t xfsm, xnfsm_t sname);

#endif  /* !__CR_MSCLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
