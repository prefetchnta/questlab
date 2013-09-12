/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-10-24  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack TX10I 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_TX10I_H__
#define __CR_TX10I_H__ 0x7C0E9DC0UL

#include "comm.h"

/*****************************************************************************/
/*                              TX10I 通讯协议                               */
/*****************************************************************************/

/* TX10I 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  vers;   /* 协议版本 (当前为0x01) */
/*001*/ int16u  size;   /* 全协议长度 (包含校验) */
/*003*/ int16u  tags;   /* 包识别码 (会话的编号) */
/*005*/ byte_t  ctype;  /* 命令类型 (bit0: 1-请求 0-应答)
                                    (bit1: 1-密文 0-明文) */
/*006*/ byte_t  stype;  /* 源类型 */
/*007*/ int16u  srcid;  /* 源编号 */
/*009*/ byte_t  dtype;  /* 目标类型 */
/*010*/ int16u  dstid;  /* 目标编号 */
/*012*/
} CR_PACKED sTX10I_HDR;

CR_TYPEDEF struct
{
#if 0
        byte_t  data[leng];     /* 数据域 */
#endif
        int16u  half16_be;      /* 校验位 */

} CR_PACKED sTX10I_DAT;

CR_TYPEDEF struct
{
/*000*/ int32u  tags;   /* 'ABCD' 明文
                           'RFHZ' 密文 */
/*004*/ byte_t  cmds;   /* 操作命令号 */
/*005*/ byte_t  leng;   /* 数据域的长度 */
/*006*/
#if 0
        byte_t  data[leng];     /* 数据域 */
        byte_t  padded[xxxx];   /* 密文需要填充到16字节 */
#endif
} CR_PACKED sTX10I_OPS;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

#define TX10I_ACK   0x00    /* 应答位 */
#define TX10I_REQ   0x01    /* 请求位 */
#define TX10I_TXT   0x00    /* 明文位 */
#define TX10I_CRY   0x02    /* 密文位 */

/* TX10I 通讯上下文对象 */
typedef void_t*     tx10i_t;

CR_API int16u   tx10i_get_head (sTX10I_HDR *head);
CR_API int16u   tx10i_set_head (sTX10I_HDR *head, int16u leng);
CR_API void_t   tx10i_set_hash (void_t *data, int16u size);
CR_API bool_t   tx10i_get_hash (const void_t *data, int16u size);

/***** 网络接口部分 *****/
CR_API tx10i_t  tx10i_net_open (socket_t netw,
                                const byte_t key_recv[16],
                                const byte_t key_send[16]);
CR_API void_t   tx10i_net_close (tx10i_t ctx);
CR_API bool_t   tx10i_udp_send (tx10i_t ctx, sTX10I_HDR *head,
                                const void_t *data, int16u size);
CR_API void_t*  tx10i_udp_recv (tx10i_t ctx, sTX10I_HDR *head,
                                uint_t *size);
CR_API void_t*  tx10i_udp_iorw (tx10i_t ctx, sTX10I_HDR *ssend,
                                uint_t *osize, const void_t *ibuff,
                                int16u isize, comm_delay_t delay);

#endif  /* !__CR_TX10I_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
