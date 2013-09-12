/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-01-22  */
/*     #######          ###    ###      [VSYS]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 虚拟系统头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_VSYS_H__
#define __CR_VSYS_H__ 0xCE97EDBAUL

#include "defs.h"

/*****************************************************************************/
/*                                虚拟处理器                                 */
/*****************************************************************************/

/* 地址和长度类型 */
typedef struct
{
        uint_t  size;   /* 数值的大小 */

        union {
                /* 地址值 */
                int16u  adr16;  /* 16位 */
                int32u  adr32;  /* 32位 */
                int64u  adr64;  /* 64位 */

                /* 长度值 */
                int16u  len16;  /* 16位 */
                int32u  len32;  /* 32位 */
                int64u  len64;  /* 64位 */
        } value;

} vcpu_addr_t, vcpu_leng_t;

/* 地址段属性类型 */
typedef struct
{
        uint_t  size;   /* 类型的大小 */

        /* 如果使用描述表, 大小必须比四大 */
        union {
                int32u  attr;   /* 直接的属性 */
                void_t* desc;   /* 描述表属性 */
        } value;

} vcpu_attr_t;

/* 地址段和槽类型 */
typedef struct
{
        bool_t          used;   /* 是否已片选 */
        vcpu_addr_t     addr;   /* 地址段起始 */
        vcpu_leng_t     leng;   /* 地址段长度 */
        vcpu_attr_t     attr;   /* 地址段属性 */
        const ansi_t*   name;   /* 地址段名称 */

} vcpu_addr_slot_t;

/* 虚拟处理器类型 */
typedef void_t*     vcpu_t;

/* 虚拟处理器参数 */
typedef struct
{
        /* 片内空间大小 */
        leng_t  chip_size;

        /* 复位 + 中断 + 执行 + 反汇编 */
        void_t  (*cold_reset) (vcpu_t);
        void_t  (*vcpu_intvx) (vcpu_t, uint_t);
        uint_t  (*exec_bytes) (vcpu_t, const vcpu_addr_t*);
        uint_t  (*byte_unasm) (vcpu_t, const vcpu_addr_t*);

        /* 信息输出相关参数 */
        void_t* outparm;

        /* 格式化输出的回调 */
        uint_t  (*print) (void_t*, const ansi_t*, ...);

} vcpu_param_t;

#endif  /* !__CR_VSYS_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
