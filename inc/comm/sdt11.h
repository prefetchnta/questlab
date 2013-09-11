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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack SDT11 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_SDT11_H__
#define __CR_SDT11_H__ 0xE80BCF84UL

#include "comm.h"

/*****************************************************************************/
/*                              SDT11 通讯协议                               */
/*****************************************************************************/

/* SDT11 协议包结构 */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  head;   /* 标志0x55 */
/*001*/ byte_t  addr;   /* 设备地址 */
/*002*/ byte_t  leng;   /* 数据长度 */
/*003*/ byte_t  addc;   /* 反码校验 */
/*004*/ byte_t  lenc;   /* 反码校验 */
/*005*/ byte_t  tail;   /* 标志0xAA */
/*006*/
} CR_PACKED sSDT11_HDR;

CR_TYPEDEF struct
{
#if 0
        byte_t  data[leng-2];   /* 数据 */
#endif
        int16u  crc16c_sum_be;  /* 校验 */

} CR_PACKED sSDT11_DAT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

CR_API byte_t   sdt11_get_head (const sSDT11_HDR *head, byte_t addr);
CR_API void_t   sdt11_set_head (sSDT11_HDR *head, byte_t addr, byte_t leng);
CR_API void_t   sdt11_set_hash (void_t *data, byte_t size);
CR_API bool_t   sdt11_get_hash (const void_t *data, byte_t size);

/***** 异步串口部分 *****/
CR_API bool_t   sdt11_sio_send (uint_t port, byte_t addr,
                                const void_t *data, byte_t size);
CR_API bool_t   sdt11_sio_recv (uint_t port, byte_t addr,
                                void_t *data, uint_t *size);
CR_API bool_t   sdt11_sio_iorw (uint_t port, byte_t daddr,
                                byte_t saddr, void_t *obuff,
                                uint_t *osize, const void_t *ibuff,
                                byte_t isize, comm_delay_t delay);

#endif  /* !__CR_SDT11_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
