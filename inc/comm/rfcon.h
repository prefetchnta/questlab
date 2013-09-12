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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack RFCON 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_RFCON_H__
#define __CR_RFCON_H__ 0x55866555UL

#include "comm.h"

/*****************************************************************************/
/*                              RFCON 通讯协议                               */
/*****************************************************************************/

/* RFCON 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  head;   /* 起始符02 */
/*001*/ ansi_t  type;   /* 设备类型 */
/*002*/ byte_t  size;   /* 数据长度 */
/*003*/ byte_t  cmds;   /* 命令字段 */
/*004*/
} CR_PACKED sRFCON_HDR;

CR_TYPEDEF struct
{
#if 0
        byte_t  data[size];     /* 数据域 */
#endif
        int16u  half16_be;      /* 校验位 */
        byte_t  tail;           /* 结束符03 */

} CR_PACKED sRFCON_DAT;

/* RFCON 输入信息结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  inp[4];     /* 输入电平 */
/*004*/ byte_t  ups_sta;    /* 市电 < 80V = 0
                               市电 > 80V = 1 */
/*005*/
} CR_PACKED sRFCON_INP;

/* RFCON 输出单元结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  id;     /* 端口编号 */
/*001*/ byte_t  stat;   /* 输出电平 */
/*002*/ byte_t  time;   /* 维持时间(秒, 0xFF 表示常开) */
/*003*/
} CR_PACKED sRFCON_OUT;

/* RFCON 纸币操作结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  guid;   /* 通讯唯一编号 */
/*001*/ byte_t  cash;   /* 纸币面值/纸币处理 */
/*002*/
} CR_PACKED sRFCON_CASH_OP;

/* RFCON 纸币结果结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  guid;   /* 通讯唯一编号 */
/*001*/ byte_t  ends;   /* 纸币处理结果 */
/*002*/ byte_t  cash;   /* 纸币面值的值 */
/*003*/
} CR_PACKED sRFCON_CASH_RS;

/* RFCON 硬币投入结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  guid;   /* 通讯唯一编号 */
/*001*/ byte_t  coin;   /* 投入的硬币数 */
/*002*/
} CR_PACKED sRFCON_COIN_IN;

/* RFCON 找零信息结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  changed;    /* 已找零数量 */
/*001*/ byte_t  unchanged;  /* 未找零数量 */
/*002*/ byte_t  status;     /* 硬币余量信息 */
/*003*/
} CR_PACKED sRFCON_COIN_OT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* RFCON 串口板返回结构 */
typedef struct
{
        uint_t  type;   /* 帧类型 */
        uint_t  size;   /* 帧长度 */
        byte_t  cmds;   /* 命令值 */

        /* 共用的数据结构 */
        union
        {
            byte_t          error;      /* 错误值 */
            byte_t          id[32];     /* 条码值 */
            int32u          cardid;     /* 卡号值 */
            sRFCON_INP      input;      /* 输入口 */
            sRFCON_CASH_OP  cashop;     /* 纸币机 */
            sRFCON_CASH_RS  cashrs;     /* 纸币机 */
            sRFCON_COIN_IN  coinin;     /* 硬币机 */
            sRFCON_COIN_OT  coinot;     /* 吐币机 */

        } data;

} sRFCON_INFO;

/* 设备类型取值 */
#define RFCON_ID    'A' /* 条码枪 */
#define RFCON_M1    'R' /* 读卡机 */
#define RFCON_INP   'I' /* 输入口 */
#define RFCON_OUT   'O' /* 输出口 */
#define RFCON_UPS   'U' /* UPS 电源 */
#define RFCON_CASH  'B' /* 纸币机 */
#define RFCON_COIN  'C' /* 硬币机 */
#define RFCON_SPIT  'G' /* 吐币机 */
#define RFCON_PRNT  'P' /* 打印机 */
#define RFCON_STAT  'S' /* 状态切换 */
#define RFCON_WDOG  'V' /* 心跳检测 */
#define RFCON_XDIR  'X' /* 直接透传 */

/* 交易阶段代码 */
#define RFCON_R_IDLE    0x00    /* 空闲 */
#define RFCON_R_FEES    0x01    /* 收费 */
#define RFCON_R_CHNG    0x02    /* 找零 */
#define RFCON_R_PRNT    0x03    /* 打印 */

/* 纸币吞吐控制 */
#define RFCON_CASH_IN   0x01    /* 纸币吞入 */
#define RFCON_CASH_OT   0x02    /* 纸币吐出 */

/* 打印机状态字 */
#define RFCON_PRNT_BUSY     0x01    /* 忙碌 */
#define RFCON_PRNT_OFFLINE  0x02    /* 离线 */
#define RFCON_PRNT_NOPAPER  0x04    /* 缺纸 */
#define RFCON_PRNT_LEAKPPR  0x08    /* 纸将尽 */
#define RFCON_PRNT_COM_ERR  0x80    /* 通讯错 */

/* 读卡器认证方式 */
#define RFCON_IC_KEYS   0x00    /* 密码认证 */
#define RFCON_IC_KEYA   0x01    /* KEYA认证 */
#define RFCON_IC_KEYB   0x02    /* KEYB认证 */

/* 读卡器错误代码 */
#define TCR_ERR_OVERTIME        0x10    /* 超时 */
#define TCR_ERR_DEVICE          0x12    /* 吞/吐卡机故障, 无响应 */
#define TCR_READ_FM1702_ERR     0x70    /* FM1702 出错 */
#define TCR_READ_SN_ERR         0X71    /* 卡错误, 读不了 */
#define TCR_CARD_KEY_ERR        0x72    /* 密码错误 */
#define TCR_READ_CARD_ERR       0x73    /* 读卡扇区块错误 */
#define TCR_WRITE_CARD_ERR      0x74    /* 写卡错误 */
#define TCR_SECTORS_NUM_ERR     0x75    /* 扇区超范围 */
#define TCR_PARA_ERR            0x76    /* 参数(数据)错误 */
#define TCR_LEN_ERR             0x77    /* 长度不对 */
#define TCR_DATA_ERR            0x78    /* 数据错误 */
#define TCR_MCAH_BLOCK_ERR      0x79    /* 卡机阻塞 */

CR_API uint_t   rfcon_get_money (byte_t cash);
CR_API int16u   rfcon_get_head (const sRFCON_HDR *head,
                                ansi_t *type, byte_t *cmds);
CR_API void_t   rfcon_set_head (sRFCON_HDR *head, ansi_t type,
                                byte_t size, byte_t cmds);
CR_API void_t   rfcon_set_hash (sRFCON_HDR *data);
CR_API bool_t   rfcon_get_hash (const sRFCON_HDR *data);

/***** 异步串口部分 *****/
CR_API bool_t   rfcon_sio_send (uint_t port, ansi_t type, byte_t cmds,
                                const void_t *data, byte_t size);
CR_API bool_t   rfcon_sio_recv (uint_t port, ansi_t *type, byte_t *cmds,
                                void_t *data, uint_t *size);
CR_API bool_t   rfcon_sio_iorw (uint_t port, ansi_t type, byte_t cmds,
                                void_t *obuff, uint_t *osize,
                                const void_t *ibuff, byte_t isize,
                                comm_delay_t delay);
/*===== 应用层接口 =====*/
CR_API bool_t   rfcon_sio_input (uint_t port, sRFCON_INFO *info);
CR_API bool_t   rfcon_sio_output (uint_t port, const sRFCON_OUT *gpio,
                                  uint_t count);
CR_API bool_t   rfcon_sio_alarm (uint_t port, byte_t on_off, byte_t delays);
CR_API bool_t   rfcon_sio_sound (uint_t port, byte_t on_off);
CR_API bool_t   rfcon_sio_switch (uint_t port, byte_t state, byte_t param);
CR_API bool_t   rfcon_sio_ic_read (uint_t port, byte_t secs, byte_t dat[48]);
CR_API bool_t   rfcon_sio_ic_write (uint_t port, byte_t secs,
                                    const byte_t dat[48]);
CR_API bool_t   rfcon_sio_ic_cardio (uint_t port, bool_t getin);
CR_API bool_t   rfcon_sio_ic_cardid (uint_t port, byte_t rcv[8], uint_t *size);
CR_API bool_t   rfcon_sio_ic_setpass (uint_t port, const byte_t pass[8]);
CR_API bool_t   rfcon_sio_ic_access (uint_t port, byte_t type,
                                     const byte_t key[6]);
CR_API uint_t   rfcon_sio_print_stt (uint_t port);
CR_API bool_t   rfcon_sio_print_txt (uint_t port, const byte_t *cmds,
                                     byte_t size);
CR_API bool_t   rfcon_sio_ups (uint_t port, bool_t on_off);
CR_API bool_t   rfcon_sio_xsend (uint_t port, byte_t xcom, const void_t *data,
                                 byte_t size);
CR_API bool_t   rfcon_sio_xiorw (uint_t port, byte_t xcom, void_t *obuff,
                            uint_t *osize, const void_t *ibuff, byte_t isize);

#endif  /* !__CR_RFCON_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
