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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack SDT22 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_SDT22_H__
#define __CR_SDT22_H__

#include "comm.h"

/*****************************************************************************/
/*                              SDT22 通讯协议                               */
/*****************************************************************************/

/* SDT22 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  head;   /* 标志0x55 */
/*001*/ int16u  addr;   /* 设备地址 */
/*003*/ int16u  leng;   /* 数据长度 */
/*005*/ byte_t  sum8;   /* 参数校验 */
/*006*/ byte_t  tail;   /* 标志0xAA */
/*007*/
} CR_PACKED sSDT22_HDR;

CR_TYPEDEF struct
{
#if 0
        byte_t  data[leng-2];   /* 数据 */
#endif
        int16u  crc16c_sum_be;  /* 校验 */

} CR_PACKED sSDT22_DAT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

CR_API int16u   sdt22_get_head (const sSDT22_HDR *head, int16u addr);
CR_API void_t   sdt22_set_head (sSDT22_HDR *head, int16u addr, int16u leng);
CR_API void_t   sdt22_set_hash (void_t *data, int16u size);
CR_API bool_t   sdt22_get_hash (const void_t *data, int16u size);

/***** 异步串口部分 *****/
CR_API bool_t   sdt22_sio_send (uint_t port, int16u addr,
                                const void_t *data, int16u size);
CR_API bool_t   sdt22_sio_recv (uint_t port, int16u addr,
                                void_t *data, uint_t *size);
CR_API bool_t   sdt22_sio_iorw (uint_t port, int16u daddr,
                                int16u saddr, void_t *obuff,
                                uint_t *osize, const void_t *ibuff,
                                int16u isize, comm_delay_t delay);

#endif  /* !__CR_SDT22_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
