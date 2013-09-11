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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack FDU 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_FDU_H__
#define __CR_FDU_H__ 0x13A29839UL

#include "comm.h"

/*****************************************************************************/
/*                               FDU 通讯协议                                */
/*****************************************************************************/

/* FDU 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  head[3];    /* 标志01 FF FF */
/*003*/ int16u  frame_len;  /* 整个帧的长度 */
/*005*/ byte_t  char_type;  /* 封包字符类型 */
/*006*/ int16u  frame_num;  /* 多帧的帧序号 */
/*008*/ byte_t  check_sum;  /* 8 位异或反码 */
/*009*/
} CR_PACKED sFDU_HDR;

CR_TYPEDEF struct
{
        byte_t  data_head;      /* 数据标志02 */
#if 0
        byte_t  data[length-12];    /* 帧数据 */
#endif
        byte_t  check_sum;      /* 8 位异或反码 */
        byte_t  is_finish;      /* 是否有后续帧 */

} CR_PACKED sFDU_DAT;

/* FDU 文件信息项结构 */
CR_TYPEDEF struct
{
/*000*/ ansi_t  name[8];    /* 文件文件名 */
/*008*/ ansi_t  exts[3];    /* 文件扩展名 */
/*011*/ int32u  size;       /* 文件大小LE */
/*015*/
} CR_PACKED sFDU_ITM;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* sFDU_DAT::is_finish 取值 */
#define FDU_FN_OVER     0x03    /* 数据帧结束 */
#define FDU_FN_NEXT     0x17    /* 数据帧还有 */

/* sFDU_HDR::char_type 取值 */
#define FDU_CC_INFO     0x21    /* '!' 提示封包 */
#define FDU_CC_DATA     0x41    /* 'A' 数据封包 */
#define FDU_CC_OPER     0x4F    /* 'O' 操作封包 */

CR_API int16u   fdu_get_head (const sFDU_HDR *head,
                              byte_t *char_type, int16u *frame_num);
CR_API void_t   fdu_set_head (sFDU_HDR *head, int16u frame_len,
                              byte_t char_type, int16u frame_num);
CR_API void_t   fdu_set_hash (void_t *data, int16u size, bool_t over);
CR_API bool_t   fdu_get_hash (const void_t *data, int16u size, bool_t *over);

/***** 异步串口部分 *****/
CR_API bool_t   fdu_sio_send (uint_t port, byte_t char_type,
                              int16u frame_num, const void_t *data,
                              int16u size, bool_t over);
CR_API bool_t   fdu_sio_iorw (uint_t port, void_t *obuff,
                              uint_t *osize, byte_t *char_type,
                              int16u *frame_num, bool_t *over,
                              uint_t try_times);
CR_API void_t*  fdu_sio_rfile6 (uint_t port, const ansi_t *name,
                                leng_t *size, uint_t try_times,
                                comm_delay_t delay1, comm_delay_t delay2);
CR_API void_t*  fdu_sio_rfile8 (uint_t port, const ansi_t *name,
                                leng_t *size, uint_t try_times,
                                comm_delay_t delay1, comm_delay_t delay2);
CR_API bool_t   fdu_sio_wfile6 (uint_t port, const ansi_t *name,
                                const void_t *data, leng_t size,
                                uint_t try_times, comm_delay_t delay1,
                                comm_delay_t delay2);
CR_API bool_t   fdu_sio_wfile8 (uint_t port, const ansi_t *name,
                                const void_t *data, leng_t size,
                                uint_t try_times, comm_delay_t delay1,
                                comm_delay_t delay2);
CR_API sFDU_ITM*    fdu_sio_list (uint_t port, leng_t *count,
                                  uint_t try_times, comm_delay_t delay1,
                                  comm_delay_t delay2);

#endif  /* !__CR_FDU_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
