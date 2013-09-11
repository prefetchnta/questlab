/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-10-22  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_COMM_H__
#define __CR_COMM_H__

#include "devlib.h"

/* 有用的函数回调类型 */
typedef void_t  (*comm_delay_t) (void_t);
typedef void_t  (*comm_crypto_t) (void_t*, leng_t);

/*****************************************************************************/
/*                                 通用协议                                  */
/*****************************************************************************/

/*
    基本协议格式 (BE)
    ~~~~~~~~~~~~~~~~~
    55 AA 长度(2B) 数据(0 - 65529) 校验(2B)
    长度只表示数据的长度，不包括头和校验
    数据校验方式和加解密方法可以自选或没有
*/
typedef struct
{
        /* 附加参数 */
        void_t*     ctx_param;

        /* 校验回调 */
        int16u  (*hasher) (const void_t*, leng_t);

        /* 加密回调 */
        void_t  (*encode) (void_t*, void_t*, leng_t);

        /* 解密回调 */
        void_t  (*decode) (void_t*, void_t*, leng_t);

} sCOMM_CTX;

/* 最大数据长度 */
#define COMM_MAX_LENG   65529

/***** 异步串口部分 *****/
CR_API bool_t   comm_sio_send (uint_t port, const sCOMM_CTX *parm,
                               const void_t *data, int16u size);
CR_API void_t*  comm_sio_recv (uint_t port, const sCOMM_CTX *parm,
                               uint_t *size);
CR_API void_t*  comm_sio_iorw (uint_t port, const sCOMM_CTX *parm,
                               uint_t *osize, const void_t *ibuff,
                               int16u isize, comm_delay_t delay);
/***** 网络接口部分 *****/
CR_API bool_t   comm_tcp_send (socket_t netw, const sCOMM_CTX *parm,
                               const void_t *data, int16u size);
CR_API void_t*  comm_tcp_recv (socket_t netw, const sCOMM_CTX *parm,
                               uint_t *size);
CR_API void_t*  comm_tcp_iorw (socket_t netw, const sCOMM_CTX *parm,
                               uint_t *osize, const void_t *ibuff,
                               int16u isize, comm_delay_t delay);
CR_API bool_t   comm_udp_send (socket_t netw, const sCOMM_CTX *parm,
                               const ansi_t *addr, int16u port,
                               const void_t *data, int16u size);
CR_API void_t*  comm_udp_recv (socket_t netw, const sCOMM_CTX *parm,
                               uint_t *size);
CR_API void_t*  comm_udp_iorw (socket_t netw, const sCOMM_CTX *parm,
                               const ansi_t *addr, int16u port,
                               uint_t *osize, const void_t *ibuff,
                               int16u isize, comm_delay_t delay);

#endif  /* !__CR_COMM_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
