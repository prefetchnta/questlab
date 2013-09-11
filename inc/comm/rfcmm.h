/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-08-15  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack RFCMM 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_RFCMM_H__
#define __CR_RFCMM_H__

#include "comm.h"

/*****************************************************************************/
/*                              RFCMM 通讯协议                               */
/*****************************************************************************/

/* RFCMM 协议包结构 (BE) */
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
/*012*/ int32u  magic;  /* 'ABCD' - 加解密从这里开始 */
/*016*/ byte_t  cmds;   /* 操作命令号 */
/*017*/ byte_t  leng;   /* 数据域的长度 */
/*018*/
} CR_PACKED sRFCMM_HDR;

CR_TYPEDEF struct
{
#if 0
        byte_t  data[leng];     /* 数据域 */
#endif
        int16u  half16_be;      /* 校验位 */

} CR_PACKED sRFCMM_DAT;

/* 区域屏遥控显示数值 (BE) */
CR_TYPEDEF struct
{
/*000*/ int16u  num_time;   /* 遥控显示时间 (秒)
                               0xFFFF 表示一直遥控显示 */
/*002*/ byte_t  arr_type;   /* 箭头方向颜色控制 */
/*003*/ byte_t  arr_time;   /* 箭头闪烁时间 (秒)
                               0xFF 表示一直闪烁 */
/*004*/ int32u  number;     /* 显示的数值 */
/*008*/ byte_t  flash[4];   /* 对应四个位的闪烁时间 */
/*012*/
} CR_PACKED sRFCMM_QYP_NUM;

/* 超声波状态结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  retcode;    /* 返回值 =0x00 */
/*001*/ byte_t  line1[32];  /* 第一路超声波状态 */
/*033*/ byte_t  line2[32];  /* 第二路超声波状态 */
/*065*/ byte_t  line3[32];  /* 第三路超声波状态 */
/*097*/
} CR_PACKED sRFCMM_YDP_CSB_STT;

/* 超声波车位状态结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  retcode;    /* 返回值 =0x00 */
/*001*/ byte_t  send_id;    /* 发送标识 =0x01 */
/*002*/ byte_t  line1[32];  /* 第一路超声波状态 */
/*034*/ byte_t  line2[32];  /* 第二路超声波状态 */
/*066*/ byte_t  line3[32];  /* 第三路超声波状态 */
/*098*/
} CR_PACKED sRFCMM_YDP_CSB_POS;

/* 超声波主机查询结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x01 */
/*002*/ byte_t  nouse;  /* 保留不用 */
/*003*/ byte_t  sum8b;  /* 8位累加和 */
/*004*/
} CR_PACKED sRFCMM_CSB_01;

/* 超声波查询序列号结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x02 */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_02;

/* 超声波设置序列号结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x03 */
/*002*/ byte_t  sn[10]; /* 序列号 */
/*012*/ byte_t  sum8b;  /* 8位累加和 */
/*013*/
} CR_PACKED sRFCMM_CSB_03;

/* 超声波查询设备编号结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x04 */
/*002*/ byte_t  sn[10]; /* 序列号 */
/*012*/ byte_t  sum8b;  /* 8位累加和 */
/*013*/
} CR_PACKED sRFCMM_CSB_04;

/* 超声波设置设备编号结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x05 */
/*002*/ byte_t  sn[10]; /* 序列号 */
/*012*/ byte_t  ndevid; /* 新设备编号 */
/*013*/ byte_t  sum8b;  /* 8位累加和 */
/*014*/
} CR_PACKED sRFCMM_CSB_05;

/* 超声波控制车位灯结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x06 */
/*002*/ byte_t  cntrl;  /* 灯状态控制字 */
/*003*/ byte_t  lastt;  /* 持续时间 (单位为30秒
                            0xFF 表示一直保持) */
/*004*/ byte_t  sum8b;  /* 8位累加和 */
/*005*/
} CR_PACKED sRFCMM_CSB_06;

/* 超声波查询去抖次数结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x07 */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_07;

/* 超声波设置去抖次数结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x08 */
/*002*/ byte_t  times;  /* 去抖次数 */
/*003*/ byte_t  sum8b;  /* 8位累加和 */
/*004*/
} CR_PACKED sRFCMM_CSB_08;

/* 超声波查询基准距离结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x09 */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_09;

/* 超声波设置基准距离结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x0A */
/*002*/ byte_t  dists;  /* 基准距离 (单位0.1米) */
/*003*/ byte_t  sum8b;  /* 8位累加和 */
/*004*/
} CR_PACKED sRFCMM_CSB_0A;

/* 超声波查询预留车位结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x0B */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_0B;

/* 超声波设置预留车位结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x0C */
/*002*/ byte_t  value;  /* 预留车位值 */
/*003*/ byte_t  sum8b;  /* 8位累加和 */
/*004*/
} CR_PACKED sRFCMM_CSB_0C;

/* 超声波查询探测距离结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x0D */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_0D;

/* 超声波查询固件版本结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x90 */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_90;

/* 区域屏干接点计数值 (BE) */
CR_TYPEDEF struct
{
/*000*/ int32u  gan_cnt1;   /* 1号干接点计数值 */
/*004*/ int32u  gan_cnt2;   /* 2号干接点计数值 */
/*008*/ int32u  gan_cnt3;   /* 3号干接点计数值 */
/*012*/ int32u  gan_cnt4;   /* 4号干接点计数值 */
/*016*/
} CR_PACKED sRFCMM_QYP_GAN;

/* 引导屏管理信息结构 (BE) */
CR_TYPEDEF struct
{
/*000*/ byte_t  mgr_id;     /* 管理的引导屏序号 */
/*001*/ int16u  ydp_id;     /* 引导屏的编号 */
/*003*/ int16u  udp_port;   /* 网络通讯的端口 (6500) */
/*005*/ byte_t  ipv4[4];    /* 引导屏的 IP 地址 */
/*009*/ int32u  csb_mgr1;   /* 第一路超声波管理标志 */
/*013*/ int32u  csb_mgr2;   /* 第二路超声波管理标志 */
/*017*/ int32u  csb_mgr3;   /* 第三路超声波管理标志 */
/*021*/
} CR_PACKED sRFCMM_QYP_MGR;
/*
0xF0000000: 第1路编号1~4的超声波探测器纳入管理
0xF000000F: 第2路编号1~4,29~32的超声波探测器纳入管理
0xFFFFFFFF: 第3路所有超声波探测器纳入管理
*/

/* 载波通讯效率结构 (BE) */
CR_TYPEDEF struct
{
/*000*/ byte_t  retcode;    /* 返回值 =0x00 */
/*001*/ int32u  cnt_succ;   /* 接收成功次数 */
/*005*/ int32u  cnt_fail;   /* 接收错误次数 */
/*009*/ int32u  cnt_nack;   /* 无应答的次数 */
/*013*/ byte_t  rat_succ;   /* 最近120次中接收成功次数 */
/*014*/ byte_t  rat_fail;   /* 最近120次中接收错误次数 */
/*015*/ byte_t  rat_nack;   /* 最近120次中无应答的次数 */
/*016*/
} CR_PACKED sRFCMM_YDP_COUNT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* 设备类型取值 */
#define RFCMM_YDP   'g'     /* 引导屏 */
#define RFCMM_QYP   'b'     /* 区域屏 */
#define RFCMM_ATX   'P'     /* 上位机 */

/* 区域屏可管理引导屏的最大个数 */
#define RFCMM_QYP_MAX_YDP   80

/* 箭头方向颜色 */
#define RFCMM_ARR_RR    0x12    /* 红色向右 */
#define RFCMM_ARR_RL    0x0A    /* 红色向左 */
#define RFCMM_ARR_RU    0x05    /* 红色向上 */
#define RFCMM_ARR_GR    0x92    /* 绿色向右 */
#define RFCMM_ARR_GL    0x8A    /* 绿色向左 */
#define RFCMM_ARR_GU    0x85    /* 绿色向上 */

/* 超声波状态值 */
#define RFCMM_YDP_CSB_STT_PWRON     0x00    /* 上电状态 */
#define RFCMM_YDP_CSB_STT_XMISS     0x01    /* 不存在状态 */
#define RFCMM_YDP_CSB_STT_ONLINE    0x02    /* 在线状态 */
#define RFCMM_YDP_CSB_STT_OFFLINE   0x03    /* 断线状态 */

/* 超声波车位状态值 */
#define RFCMM_YDP_CSB_POS_PWRON     0xE0    /* 上电状态 */
#define RFCMM_YDP_CSB_POS_NOCAR     0xEA    /* 无车状态 */
#define RFCMM_YDP_CSB_POS_HVCAR     0xEF    /* 有车状态 */
#define RFCMM_YDP_CSB_POS_OFFLINE   0xE2    /* 车位断线 */
#define RFCMM_YDP_CSB_POS_XMISS     0xE7    /* 车位不存在 */

/* 超声波灯控制值 */
#define RFCMM_CSB_G_OFF     (0 << 0)    /* 绿灯灭 */
#define RFCMM_CSB_G_FLASH   (1 << 0)    /* 绿灯闪 */
#define RFCMM_CSB_G_XON     (3 << 0)    /* 绿灯亮 */
#define RFCMM_CSB_R_OFF     (0 << 2)    /* 红灯灭 */
#define RFCMM_CSB_R_FLASH   (1 << 2)    /* 红灯闪 */
#define RFCMM_CSB_R_XON     (3 << 2)    /* 红灯亮 */
#define RFCMM_CSB_L_AUTO    (0 << 6)    /* 自  控 */
#define RFCMM_CSB_L_HALF    (1 << 6)    /* 半自动 */
#define RFCMM_CSB_L_MANS    (3 << 6)    /* 遥  控 */

/* 超声波车位状态值 */
#define RFCMM_CSB_POS_HVCAR 0x0F    /* 有车 */
#define RFCMM_CSB_POS_NOCAR 0x0A    /* 无车 */
#define RFCMM_CSB_POS_XMISS 0x00    /* 未知 */
#define RFCMM_CSB_POS_RESVE 0x05    /* 预留 */

CR_API int16u   rfcmm_get_head (sRFCMM_HDR *head);
CR_API int16u   rfcmm_set_head (sRFCMM_HDR *head, byte_t cmds,
                                byte_t leng);
CR_API void_t   rfcmm_set_hash (void_t *data, int16u size);
CR_API bool_t   rfcmm_get_hash (const void_t *data, int16u size);

/***** 网络接口部分 *****/
CR_API bool_t   rfcmm_udp_send (socket_t netw,
                                const sRFCMM_HDR *head, byte_t cmds,
                                const void_t *data, byte_t size);
CR_API bool_t   rfcmm_udp_recv (socket_t netw, sRFCMM_HDR *head,
                                void_t *data, uint_t *size);
CR_API bool_t   rfcmm_udp_iorw (socket_t netw, void_t *obuff,
                                uint_t *osize, byte_t stype,
                                byte_t dtype, byte_t cmds,
                                const void_t *ibuff, byte_t isize,
                                comm_delay_t delay);
/*===== 应用层接口 =====*/
CR_API bool_t   rfcmm_udp_all_online (socket_t netw, byte_t stype,
                                      byte_t dtype);
CR_API bool_t   rfcmm_udp_all_reset (socket_t netw, byte_t stype,
                                     byte_t dtype);
CR_API bool_t   rfcmm_udp_all_devid (socket_t netw, byte_t stype,
                                     byte_t dtype, uint_t *devid);
CR_API ansi_t*  rfcmm_udp_all_serial (socket_t netw, byte_t stype,
                                      byte_t dtype);
CR_API bool_t   rfcmm_udp_ydp_send (socket_t netw, byte_t stype,
                                    byte_t line, const void_t *data,
                                    byte_t size);
CR_API bool_t   rfcmm_udp_ydp_iorw (socket_t netw, byte_t stype,
                                    void_t *obuff, uint_t *osize,
                                    byte_t line, const void_t *ibuff,
                                    byte_t isize);
CR_API bool_t   rfcmm_udp_qyp_show (socket_t netw, byte_t stype,
                                    const sRFCMM_QYP_NUM *param);
CR_API bool_t   rfcmm_udp_ydp_csb_stt (socket_t netw, byte_t stype,
                                       sRFCMM_YDP_CSB_STT *state);
CR_API bool_t   rfcmm_udp_ydp_csb_pos (socket_t netw, byte_t stype,
                                       sRFCMM_YDP_CSB_POS *state);
CR_API bool_t   rfcmm_udp_all_set_arrow (socket_t netw, byte_t stype,
                                         byte_t dtype, byte_t *adirs,
                                         byte_t isize);
CR_API bool_t   rfcmm_udp_all_get_arrow (socket_t netw, byte_t stype,
                                         byte_t dtype, byte_t *adirs,
                                         uint_t *osize);
CR_API bool_t   rfcmm_udp_qyp_free (socket_t netw, byte_t stype,
                                    uint_t *free);
CR_API bool_t   rfcmm_udp_qyp_set_gan (socket_t netw, byte_t stype,
                                       const sRFCMM_QYP_GAN *param);
CR_API bool_t   rfcmm_udp_qyp_get_gan (socket_t netw, byte_t stype,
                                       sRFCMM_QYP_GAN *param);
CR_API bool_t   rfcmm_udp_qyp_set_ydp (socket_t netw, byte_t stype,
                                       const sRFCMM_QYP_MGR *param);
CR_API bool_t   rfcmm_udp_qyp_get_ydp (socket_t netw, byte_t stype,
                                       sRFCMM_QYP_MGR *param,
                                       byte_t mgr_id);
CR_API bool_t   rfcmm_udp_qyp_del_ydp (socket_t netw, byte_t stype,
                                       byte_t mgr_id);
CR_API bool_t   rfcmm_udp_qyp_del_all (socket_t netw, byte_t stype);
CR_API bool_t   rfcmm_udp_ydp_csb_cnt (socket_t netw, byte_t stype,
                                       sRFCMM_YDP_COUNT *count);
/*===== 超声波接口 =====*/
CR_API bool_t   rfcmm_udp_csb_query (socket_t netw, byte_t stype, byte_t line,
                                   byte_t *state, const sRFCMM_CSB_01 *param);
CR_API bool_t   rfcmm_udp_csb_getsn (socket_t netw, byte_t stype, byte_t line,
                                   byte_t sn[10], const sRFCMM_CSB_02 *param);
CR_API bool_t   rfcmm_udp_csb_setsn (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_03 *param);
CR_API bool_t   rfcmm_udp_csb_getid (socket_t netw, byte_t stype, byte_t line,
                                   byte_t *devid, const sRFCMM_CSB_04 *param);
CR_API bool_t   rfcmm_udp_csb_setid (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_05 *param);
CR_API bool_t   rfcmm_udp_csb_light (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_06 *param);
CR_API bool_t   rfcmm_udp_csb_getft (socket_t netw, byte_t stype, byte_t line,
                                   byte_t *times, const sRFCMM_CSB_07 *param);
CR_API bool_t   rfcmm_udp_csb_setft (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_08 *param);
CR_API bool_t   rfcmm_udp_csb_getds (socket_t netw, byte_t stype, byte_t line,
                                   byte_t *dists, const sRFCMM_CSB_09 *param);
CR_API bool_t   rfcmm_udp_csb_setds (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_0A *param);
CR_API bool_t   rfcmm_udp_csb_getcp (socket_t netw, byte_t stype, byte_t line,
                                   byte_t val[2], const sRFCMM_CSB_0B *param);
CR_API bool_t   rfcmm_udp_csb_setcp (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_0C *param);
CR_API bool_t   rfcmm_udp_csb_lastd (socket_t netw, byte_t stype, byte_t line,
                                   int32u *dists, const sRFCMM_CSB_0D *param);
CR_API ansi_t*  rfcmm_udp_csb_fwver (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_90 *param);

#endif  /* !__CR_RFCMM_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
