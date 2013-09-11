/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-07-31  */
/*     #######          ###    ###      [COMM]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack LLB3 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_LLB3X_H__
#define __CR_LLB3X_H__

#include "comm.h"

/*****************************************************************************/
/*                              LLB3 通讯协议                                */
/*****************************************************************************/

/***** 异步串口部分 *****/
CR_API fp32_t   llb3_sio_get_tc (uint_t port, byte_t addr); /* 获取温度 */
CR_API fp32_t   llb3_sio_get_ta (uint_t port, byte_t addr); /* 获取常温 */
CR_API fp32_t   llb3_sio_get_pc (uint_t port, byte_t addr); /* 获取压力 */
CR_API fp32_t   llb3_sio_get_dc (uint_t port, byte_t addr); /* 获取差压 */
CR_API fp32_t   llb3_sio_get_fc (uint_t port, byte_t addr); /* 获取流量 */
CR_API fp32_t   llb3_sio_get_hc (uint_t port, byte_t addr); /* 获取热量 */
CR_API fp32_t   llb3_sio_get_sq (uint_t port, byte_t addr); /* 累计流量 */
CR_API fp32_t   llb3_sio_get_sh (uint_t port, byte_t addr); /* 累计热量 */
CR_API int16u   llb3_sio_get_ad (uint_t port, byte_t addr,  /* 直接取值 */
                                 byte_t chnn, int16u *maxv);

#endif  /* !__CR_LLB3X_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
