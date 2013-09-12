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
/*  >>>>>>>>>>>>>>>>>>>> CrHack DTM_NEW 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DTM_NEW_H__
#define __CR_DTM_NEW_H__ 0xF5CF57FEUL

#include "comm.h"

/*****************************************************************************/
/*                             DTM_NEW 通讯协议                              */
/*****************************************************************************/

/* DTM_NEW 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  head[3];    /* 标志1A 72 73 */
/*003*/ byte_t  cmd_type1;  /* 命令应用大类 */
/*004*/ byte_t  cmd_type2;  /* 命令应用子类 */
/*005*/ byte_t  frame_num;  /* 帧的序号 (+1 可以回绕) */
/*006*/ int16u  additions;  /* 返回错误代码或附加参数 */
/*008*/ int16u  data_size;  /* 后续数据长度 (包括 CRC) */
/*010*/
} CR_PACKED sDTMN_HDR;

CR_TYPEDEF struct
{
#if 0
        byte_t  data[leng-2];   /* 数据 */
#endif
        int16u  crc16c_sum_be;  /* 校验 */

} CR_PACKED sDTMN_DAT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

CR_API int16u   dtmn_get_head (const sDTMN_HDR *head,
                               byte_t *cmd_type1, byte_t *cmd_type2,
                               byte_t *frame_num, int16u *additions);
CR_API void_t   dtmn_set_head (sDTMN_HDR *head, byte_t cmd_type1,
                               byte_t cmd_type2, byte_t frame_num,
                               int16u additions, int16u data_size);
CR_API void_t   dtmn_set_hash (void_t *data, int16u size);
CR_API bool_t   dtmn_get_hash (const sDTMN_HDR *head,
                               const void_t *data, int16u size);
/***** 异步串口部分 *****/
CR_API bool_t   dtmn_sio_send (uint_t port, byte_t cmd_type1,
                               byte_t cmd_type2, byte_t frame_num,
                               int16u additions, const void_t *data,
                               int16u size);
CR_API bool_t   dtmn_sio_recv (uint_t port, void_t *data, uint_t *size,
                               byte_t *cmd_type1, byte_t *cmd_type2,
                               byte_t *frame_num, int16u *additions);
CR_API bool_t   dtmn_sio_iorw (uint_t port, byte_t cmd_type1,
                               byte_t cmd_type2, void_t *obuff,
                               uint_t *osize, int16u *retcode,
                               byte_t frame_num, int16u additions,
                               const void_t *ibuff, int16u isize,
                               comm_delay_t delay);

#endif  /* !__CR_DTM_NEW_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
