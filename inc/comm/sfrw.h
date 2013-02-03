/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-07-31  */
/*     #######          ###    ###      [COMM]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-01-10  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack SFRW 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_SFRW_H__
#define __CR_SFRW_H__ 0x2CD606B4UL

#include "comm.h"

/*****************************************************************************/
/*                              SFRW 通讯协议                                */
/*****************************************************************************/

/* SFRW 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ int32u  type;   /* 命令类型 */
/*004*/ int32u  size;   /* 数据长度 */
/*008*/
} CR_PACKED sSFRW_HDR;

/* 文件信息结构 */
CR_TYPEDEF struct
{
/*000*/ int32u  size;   /* 文件大小 */
/*004*/ int32u  crc32;  /* 文件校验 */
/*008*/
} CR_PACKED sSFRW_INFO;

/* 日期时间结构 */
CR_TYPEDEF struct
{
/*000*/ int16u  year;       /* 年 */
/*002*/ byte_t  month;      /* 月 */
/*003*/ byte_t  day;        /* 日 */
/*004*/ byte_t  hour;       /* 时 */
/*005*/ byte_t  minute;     /* 分 */
/*006*/ byte_t  second;     /* 秒 */
/*007*/ byte_t  week;       /* 周 */
/*008*/
} CR_PACKED sSFRW_TIME;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* 功能支持模式 */
#define SFRW_DOWN   0x01    /* 下载支持 */
#define SFRW_UPPE   0x02    /* 上传支持 */

/* 内置的命令类型 */
#define SFRW_OKAY   "OKAY"  /* 操作成功 */
#define SFRW_FAIL   "FAIL"  /* 操作失败 */
/* ------------------------------------ */
#define SFRW_FIND   "FIND"  /* 查询文件 */
#define SFRW_PULL   "PULL"  /* 下载文件 */
#define SFRW_PUSH   "PUSH"  /* 上传文件 */
/* ------------------------------------ */
#define SFRW_INFO   "INFO"  /* 文件信息 */
#define SFRW_DATA   "DATA"  /* 文件数据 */
/* ------------------------------------ */
CR_API int32u   sfrw_get_head (const sSFRW_HDR *head);
CR_API void_t   sfrw_set_head (sSFRW_HDR *head, const ansi_t *type,
                               int32u size);
CR_API bool_t   sfrw_get_finfo (sSFRW_INFO *info, const ansi_t *name);

/***** 网络接口部分 *****/
CR_API bool_t   sfrw_tcp_send_data (socket_t netw, const ansi_t *type,
                                    int32u size, const void_t *data);
CR_API bool_t   sfrw_tcp_send_file (socket_t netw, const ansi_t *name);
CR_API bool_t   sfrw_tcp_recv_data (socket_t netw, sSFRW_HDR *head,
                                    void_t **data);
CR_API bool_t   sfrw_tcp_recv_file (socket_t netw, sSFRW_HDR *head,
                                    const ansi_t *name, int32u *crc32);
CR_API bool_t   sfrw_tcp_srv_loop (socket_t netw, uint_t flags,
                                   const ansi_t *down, const ansi_t *uppe);
CR_API bool_t   sfrw_tcp_cli_find (socket_t netw, sSFRW_INFO *info,
                                   const ansi_t *path);
CR_API bool_t   sfrw_tcp_cli_pull (socket_t netw, const ansi_t *path,
                                   const ansi_t *name);
CR_API bool_t   sfrw_tcp_cli_push (socket_t netw, const ansi_t *path,
                                   const ansi_t *name);

#endif  /* !__CR_SFRW_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
