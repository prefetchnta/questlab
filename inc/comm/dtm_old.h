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
/*  >>>>>>>>>>>>>>>>>>>> CrHack DTM_OLD 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DTM_OLD_H__
#define __CR_DTM_OLD_H__

#include "comm.h"

/*****************************************************************************/
/*                             DTM_OLD 通讯协议                              */
/*****************************************************************************/

/* DTM_OLD 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  head[3];    /* 标志1A 72 73 */
/*003*/ int16u  data_size;  /* 后续数据长度 */
/*005*/
} CR_PACKED sDTMO_HDR;

CR_TYPEDEF struct
{
#if 0
        byte_t  data[leng-1];   /* 数据 */
#endif
        byte_t  check_sum8b;    /* 校验 */

} CR_PACKED sDTMO_DAT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

CR_API int16u   dtmo_get_head (const sDTMO_HDR *head);
CR_API void_t   dtmo_set_head (sDTMO_HDR *head, int16u size);
CR_API void_t   dtmo_set_hash (void_t *data, int16u size);
CR_API bool_t   dtmo_get_hash (const sDTMO_HDR *head,
                               const void_t *data, int16u size);
/***** 异步串口部分 *****/
CR_API bool_t   dtmo_sio_send (uint_t port, const void_t *data,
                               int16u size);
CR_API bool_t   dtmo_sio_recv (uint_t port, void_t *data, uint_t *size);
CR_API bool_t   dtmo_sio_iorw (uint_t port, void_t *obuff, uint_t *osize,
                               const void_t *ibuff, int16u isize,
                               comm_delay_t delay);

#endif  /* !__CR_DTM_OLD_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
