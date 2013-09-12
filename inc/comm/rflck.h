/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-10-16  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack RFLCK 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_RFLCK_H__
#define __CR_RFLCK_H__ 0x1CEC5AB7UL

#include "comm.h"

/*****************************************************************************/
/*                              RFLCK 通讯协议                               */
/*****************************************************************************/

/* RFLCK 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  tag[2];     /* 协议头 'PE' */
/*002*/ int16u  length;     /* 整个协议包长 */
/*004*/ byte_t  command;    /* 操作的命令字 */
/*005*/
} CR_PACKED sRFLCK_HDR;

CR_TYPEDEF struct
{
#if 0
        byte_t  data[leng];     /* 数据域 */
#endif
        int16u  half16_be;      /* 校验位 */

} CR_PACKED sRFLCK_DAT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* RFLCK 电机控制参数 */
#define RFLCK_GO_UP     'R'     /* 上升 */
#define RFLCK_GO_DOWN   'F'     /* 下降 */
#define RFLCK_GO_STOP   'S'     /* 停止 */
/*
*********************************************
b15         NS - 车位锁的网络状态
                    1: 车位锁在网
                    0: 车位锁不在网
*********************************************
b14:b12     LS[2:0] - 锁的当前状态
                    110: 延续运行
                    101: 下降运动
                    100: 上升运动
                    011: 停在中间
                    010: 停在下限位
                    001: 停在上限位
                    000: 其他
*********************************************
b11:b09     OPSRC[2:0] - 开关锁的信号源
                    101: 遇阻停止
                    100: 遇阻反转
                    011: 协议控制
                    010: 口线控制
                    001: 遥控
                    000: 其他
*********************************************
b08         SCAU - 电机停止原因
                    1: 超时
                    0: 限位
*********************************************
b07         LED - LED 是否闪烁标志
                    1: 报警
                    0: 无
*********************************************
b06:b05     BEEP[1:0] - 蜂鸣器鸣叫标志
                    11: 外力推开
                    10: 停止工作
                    01: 电量不足
                    00: 其他
*********************************************
b04:b01     WS[3:0] - 当前供电源状态
                    1000: 过充
                    0111: 没有电池
                    0110: 掉电(外电不足)
                    0101: 充电
                    0100: 休眠
                    0011: 电量不足
                    0010: 电池
                    0001: 外电
                    0000: 开始
*********************************************
b00         SEN - 电感检测器状态
                    1: 有信号
                    0: 无信号
*********************************************
*/
CR_API int16u   rflck_get_head (const sRFLCK_HDR *head);
CR_API int16u   rflck_set_head (sRFLCK_HDR *head, byte_t cmds,
                                int16u leng);
CR_API void_t   rflck_set_hash (void_t *data, int16u size);
CR_API bool_t   rflck_get_hash (const void_t *data, int16u size);
CR_API uint_t   rflck_moto_ctrl (ansi_t *str, byte_t line, byte_t id,
                                 ansi_t stt);
/***** 网络接口部分 *****/
CR_API bool_t   rflck_udp_send (socket_t netw, byte_t cmds,
                                const void_t *data, int16u size);
CR_API bool_t   rflck_udp_recv (socket_t netw, byte_t *cmds,
                                void_t *data, uint_t *size);
CR_API bool_t   rflck_udp_iorw (socket_t netw, void_t *obuff,
                                uint_t *osize, byte_t cmds,
                                const void_t *ibuff, int16u isize,
                                comm_delay_t delay);
/*===== 应用层接口 =====*/
CR_API bool_t   rflck_udp_lck_iorw (socket_t netw, void_t *obuff,
                                    uint_t *osize, const ansi_t *ibuff);
CR_API bool_t   rflck_udp_lck_read (socket_t netw, int16u state[96]);
CR_API bool_t   rflck_udp_lck_feed (socket_t netw, int16u state[96]);

#endif  /* !__CR_RFLCK_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
