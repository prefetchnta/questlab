/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-04-08  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 设备驱动头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DEVLIB_H__
#define __CR_DEVLIB_H__

#include "defs.h"

/*****************************************************************************/
/*                                 串口通讯                                  */
/*****************************************************************************/

/* sio_setup() 停止位 */
#define CR_SIO_STOP10   0   /* 1  位 */
#define CR_SIO_STOP15   1   /* 1.5位 */
#define CR_SIO_STOP20   2   /* 2  位 */

/* sio_setup() 校验位 */
#define CR_SIO_NOP      0   /* 不使用 */
#define CR_SIO_ODD      1   /* 奇校验 */
#define CR_SIO_EVEN     2   /* 偶校验 */
#define CR_SIO_MARK     3   /* 标  记 */
#define CR_SIO_SPCE     4   /* 空  白 */

/* sio_flush() 的标志 */
#define CR_SIO_FLU_RX   0   /* 读取状态 */
#define CR_SIO_FLU_TX   1   /* 写入状态 */
#define CR_SIO_FLU_RT   2   /* 读写状态 */
#define CR_SIO_FLU_WR   3   /* 阻塞写入 */

/* sio_io_get() 的标志 */
#define CR_SIO_I_CTS    1   /* CTS */
#define CR_SIO_I_DSR    2   /* DSR */
#define CR_SIO_I_RING   4   /* RI/RING */
#define CR_SIO_I_RLSD   8   /* CD/DCD/RLSD */

/* sio_io_set() 的标志 */
#define CR_SIO_O_DTR    0   /* DTR */
#define CR_SIO_O_RTS    1   /* RTS */

CR_API void_t   sio_init (void_t);
CR_API void_t   sio_free (void_t);
CR_API bool_t   sio_set_name (uint_t port, const ansi_t *name);
CR_API bool_t   sio_setup (uint_t port, int32u baud, uint_t bits,
                           uint_t parity, uint_t stop);
CR_API bool_t   sio_set_rd_timeout (uint_t port, uint_t interval,
                                    uint_t multiplier, uint_t constant);
CR_API bool_t   sio_set_wr_timeout (uint_t port, uint_t multiplier,
                                    uint_t constant);
CR_API bool_t   sio_open  (uint_t port);
CR_API bool_t   sio_close (uint_t port);
CR_API bool_t   sio_flush (uint_t port, uint_t flags);
CR_API uint_t   sio_read  (uint_t port, void_t *data, uint_t size);
CR_API uint_t   sio_write (uint_t port, const void_t *data, uint_t size);
CR_API bool_t   sio_set_buffer  (uint_t port, uint_t rx_size, uint_t tx_size);
CR_API bool_t   sio_clear_error (uint_t port);
CR_API bool_t   sio_rw_rest (uint_t port, leng_t *rx_len, leng_t *tx_len);
CR_API bool_t   sio_set_bit9 (uint_t port, bool_t onoff);
CR_API bool_t   sio_io_get (uint_t port, uint_t *value);
CR_API bool_t   sio_io_set (uint_t port, uint_t pin, bool_t onoff);

/*****************************************************************************/
/*                                 网络通讯                                  */
/*****************************************************************************/

/* 套接字类型 */
typedef void_t*     socket_t;

/* socket_shutdown() 标志 */
#define CR_NET_SHT_RX   0   /* 关闭读取 */
#define CR_NET_SHT_TX   1   /* 关闭写入 */
#define CR_NET_SHT_RT   2   /* 关闭读写 */

/* 读取数据错误码 */
#define CR_SOCKET_CLOSED    ((uint_t)-2)    /* 连接断开 */
#define CR_SOCKET_TIMEOUT   ((uint_t)-3)    /* 连接超时 */

CR_API bool_t   socket_init (void_t);
CR_API void_t   socket_free (void_t);
CR_API leng_t   socket_get (socket_t netw);
CR_API void_t   socket_close (socket_t netw);
CR_API bool_t   socket_shutdown (socket_t netw, uint_t flags);
CR_API bool_t   socket_input_size (socket_t netw, uint_t *size);
CR_API bool_t   socket_input_size2 (socket_t netw, uint_t *size);
CR_API socket_t server_tcp_accept (socket_t netw);
CR_API socket_t server_tcp_open (const ansi_t *addr, int16u port);
CR_API socket_t client_tcp_open (const ansi_t *addr, int16u port, int32s time);
CR_API socket_t client_tcp_open2 (const ansi_t *addr, int16u port, int32s time,
                                  const ansi_t *laddr, int16u lport);
CR_API socket_t server_udp_open (const ansi_t *addr, int16u port);
CR_API socket_t client_udp_open (const ansi_t *addr, int16u port);
CR_API socket_t client_udp_open2 (const ansi_t *addr, int16u port,
                                  const ansi_t *laddr, int16u lport);
CR_API uint_t   socket_tcp_send (socket_t netw, const void_t *data,
                                 uint_t size);
CR_API uint_t   socket_udp_send (socket_t netw, const ansi_t *addr,
                                 int16u port, const void_t *data, uint_t size);
CR_API uint_t   socket_tcp_recv (socket_t netw, void_t *data, uint_t size);
CR_API uint_t   socket_tcp_peek (socket_t netw, void_t *data, uint_t size);
CR_API uint_t   socket_udp_recv (socket_t netw, void_t *data, uint_t size);
CR_API uint_t   socket_udp_peek (socket_t netw, void_t *data, uint_t size);
CR_API void_t   socket_set_timeout (socket_t netw, int32s wr_time,
                                    int32s rd_time);
CR_API int16u   socket_remote_ip (socket_t netw, int32u *ip);
CR_API int16u   socket_remote_ipA (socket_t netw, ansi_t ip[16]);
CR_API bool_t   socket_tcp_set_alive (socket_t netw, uint_t idle,
                                      uint_t interval, uint_t count);
CR_API bool_t   socket_mcast_enter (socket_t netw, const ansi_t *maddr,
                                const ansi_t *laddr, byte_t ttl, byte_t loop);
CR_API bool_t   socket_mcast_leave (socket_t netw);
CR_API bool_t   netcard_get_info (const ansi_t *name, ansi_t ip[16],
                            ansi_t gw[16], ansi_t msk[16], byte_t mac[6]);

/* SOCKET 抽象类结构 */
typedef struct
{
    void_t      (*socket_close) (socket_t);
    bool_t      (*socket_shutdown) (socket_t, uint_t);
    bool_t      (*socket_input_size) (socket_t, uint_t*);
    bool_t      (*socket_input_size2) (socket_t, uint_t*);
    socket_t    (*server_tcp_accept) (socket_t);
    socket_t    (*server_tcp_open) (const ansi_t*, int16u);
    socket_t    (*client_tcp_open) (const ansi_t*, int16u, int32s);
    socket_t    (*client_tcp_open2) (const ansi_t*, int16u, int32s,
                                     const ansi_t*, int16u);
    socket_t    (*server_udp_open) (const ansi_t*, int16u);
    socket_t    (*client_udp_open) (const ansi_t*, int16u);
    socket_t    (*client_udp_open2) (const ansi_t*, int16u,
                                     const ansi_t*, int16u);
    uint_t      (*socket_tcp_send) (socket_t, const void_t*, uint_t);
    uint_t      (*socket_udp_send) (socket_t, const ansi_t*, int16u,
                                    const void_t*, uint_t);
    uint_t      (*socket_tcp_recv) (socket_t, void_t*, uint_t);
    uint_t      (*socket_tcp_peek) (socket_t, void_t*, uint_t);
    uint_t      (*socket_udp_recv) (socket_t, void_t*, uint_t);
    uint_t      (*socket_udp_peek) (socket_t, void_t*, uint_t);
    void_t      (*socket_set_timeout) (socket_t, int32s, int32s);
    int16u      (*socket_remote_ip) (socket_t, int32u*);
    int16u      (*socket_remote_ipA) (socket_t, ansi_t[]);
    bool_t      (*socket_tcp_set_alive) (socket_t, uint_t, uint_t, uint_t);
    bool_t      (*socket_mcast_enter) (socket_t, const ansi_t*,
                                       const ansi_t*, byte_t, byte_t);
    bool_t      (*socket_mcast_leave) (socket_t);
    bool_t      (*netcard_get_info) (const ansi_t*, ansi_t[], ansi_t[],
                                     ansi_t[], byte_t[]);
} iSOCKET;

/*****************************************************************************/
/*                                 其他外设                                  */
/*****************************************************************************/

/* RTC */
CR_API void_t   rtc_init (void_t);

/* WDG */
CR_API sint_t   wdg_open (const ansi_t *dev);
CR_API void_t   wdg_close (sint_t wdg);
CR_API void_t   wdg_timeout (sint_t wdg, uint_t time_ms);
CR_API void_t   wdg_feed (sint_t wdg);

/* SPI */
#define CR_SPI_CPHA         0x01
#define CR_SPI_CPOL         0x02
#define CR_SPI_CS_HIGH      0x04
#define CR_SPI_LSB_FIRST    0x08
#define CR_SPI_3WIRE        0x10
#define CR_SPI_LOOP         0x20
#define CR_SPI_NO_CS        0x40
#define CR_SPI_READY        0x80
CR_API sint_t   spi_open (const ansi_t *dev, uint_t mode,
                          uint_t bits, uint_t speed);
CR_API void_t   spi_close (sint_t spi);
CR_API bool_t   spi_read (sint_t spi, void_t *data, uint_t size);
CR_API bool_t   spi_write (sint_t spi, const void_t *data, uint_t size);
CR_API bool_t   spi_iorw (sint_t spi, void_t *recv, const void_t *send,
                          uint_t size, bool_t cs_flip);
/* GPIO */
CR_API sint_t   gpio_open (uint_t port, bool_t dir_out);
CR_API void_t   gpio_close (sint_t gpio);
CR_API bool_t   gpio_set (sint_t gpio, bool_t value);
CR_API bool_t   gpio_get (sint_t gpio, bool_t *value);

/* FLASH */
CR_API void_t   flash_read (int32u addr, void_t *data, uint_t size);
CR_API bool_t   flash_prog (int32u addr, const void_t *data, uint_t size);

/* BOOT */
CR_API void_t   boot_jump (int32u addr);
CR_API void_t   boot_goon (int32u addr);
CR_API void_t   boot_reset (void_t);

/* AT CMD */
CR_API void_t   at_flush (void_t);
CR_API void_t   at_send (const void_t *data, uint_t size);
#define at_send_str(str) at_send(str, str_lenA(str))
CR_API void_t   at_throw (uint_t size, uint_t tout);
CR_API ansi_t*  at_wait (ansi_t *out, uint_t size, uint_t tout);
CR_API ansi_t*  at_iorw (ansi_t *out, uint_t size,
                         const ansi_t *inp, uint_t tout);
CR_API bool_t   at_check (uint_t tout);
CR_API bool_t   at_cgmm (ansi_t *out, uint_t size, uint_t tout);
CR_API bool_t   at_csq (uint_t *rssi, uint_t tout);

/* BRIDGE 通讯命令字 */
#define CR_BRIDGE_CMD_SEND  0x00    /* 数据的转发：端口(1B) + 数据(NB)
                                                0x00 - CAN
                                                0x01 - 485
                                                0x02 - 桥接
                                                0x03 - 主板
                                    */
#define CR_BRIDGE_CMD_BAUD  0x01    /* 设置波特率：端口(1B) + 波特率(4B) */
#define CR_BRIDGE_CMD_RESET 0x02    /* 复位 */
#define CR_BRIDGE_CMD_GPIO  0x03    /* CTRL 管脚控制：电平(1B) */

/* BRIDGE 端口的类型 */
#define CR_BRIDGE_PORT_CAN  0x00
#define CR_BRIDGE_PORT_485  0x01
#define CR_BRIDGE_PORT_PSS  0x02
#define CR_BRIDGE_PORT_BRD  0x03

/* BRIDGE 命令返回值 */
#define CR_BRIDGE_ERR_OKAY  0x00

/* BRIDGE */
CR_API bool_t   bridge_baud (byte_t port, int32u baud);
CR_API bool_t   bridge_reset (void_t);
CR_API bool_t   bridge_gpio (byte_t level);
CR_API void_t   bridge_commit (byte_t port, const void_t *data, uint_t size);

/* SIMCOM */
CR_API bool_t   simcom_socket_init (void_t);
CR_API void_t   simcom_socket_free (void_t);
CR_API socket_t simcom_client_tcp_open (const ansi_t *addr, int16u port,
                                        int32s time);
CR_API socket_t simcom_client_tcp_open2 (const ansi_t *addr, int16u port,
                                         int32s time, const ansi_t *laddr,
                                         int16u lport);
CR_API socket_t simcom_client_udp_open (const ansi_t *addr, int16u port);
CR_API socket_t simcom_client_udp_open2 (const ansi_t *addr, int16u port,
                                         const ansi_t *laddr, int16u lport);
CR_API void_t   simcom_socket_close (socket_t netw);
CR_API uint_t   simcom_socket_tcp_send (socket_t netw, const void_t *data,
                                        uint_t size);
CR_API uint_t   simcom_socket_udp_send (socket_t netw, const ansi_t *addr,
                                        int16u port, const void_t *data,
                                        uint_t size);
CR_API uint_t   simcom_socket_tcp_recv (socket_t netw, void_t *data,
                                        uint_t size);
CR_API uint_t   simcom_socket_tcp_peek (socket_t netw, void_t *data,
                                        uint_t size);
CR_API uint_t   simcom_socket_udp_recv (socket_t netw, void_t *data,
                                        uint_t size);
CR_API uint_t   simcom_socket_udp_peek (socket_t netw, void_t *data,
                                        uint_t size);
CR_API void_t   simcom_socket_set_timeout (socket_t netw, int32s wr_time,
                                           int32s rd_time);
CR_API bool_t   simcom_socket_input_size (socket_t netw, uint_t *size);
CR_API bool_t   simcom_socket_input_size2 (socket_t netw, uint_t *size);
CR_API bool_t   simcom_socket_tcp_set_alive (socket_t netw, uint_t idle,
                                             uint_t interval, uint_t count);
CR_API void_t   sim7100_setup (iSOCKET *vtbl);
CR_API void_t   sim6320_setup (iSOCKET *vtbl);
CR_API void_t   sim5360_setup (iSOCKET *vtbl);
CR_API bool_t   sim7100_online (const ansi_t *apn, int32u timeout);
CR_API bool_t   sim6320_online (const ansi_t *user, const ansi_t *password,
                                int32u timeout);
CR_API bool_t   sim5360_online (const ansi_t *apn, int32u timeout);

/*****************************************************************************/
/*                                块设备接口                                 */
/*****************************************************************************/

/* 块设备/管理器类型 */
typedef void_t*     devb_t;
typedef void_t*     devb_mgr_t;

/* 公用的块设备控制码 */
#define DEVB_GET_INFOR      0x8000      /* 获取块设备信息 */

/* 公用的块设备出错代码 */
#define CR_DEVB_ERR_OK              CR_ERRS(0x0000UL)   /* 操作成功 */
#define CR_DEVB_ERR_NO_CLOSE        CR_ERRS(0x0001UL)   /* 无效接口 */
#define CR_DEVB_ERR_NO_READ         CR_ERRS(0x0002UL)   /* 无效接口 */
#define CR_DEVB_ERR_NO_WRITE        CR_ERRS(0x0003UL)   /* 无效接口 */
#define CR_DEVB_ERR_NO_IOCTL        CR_ERRS(0x0004UL)   /* 无效接口 */
#define CR_DEVB_ERR_CONTROL_CODE    CR_ERRS(0x0005UL)   /* 无效控制码 */
#define CR_DEVB_ERR_CONTROL_ISIZE   CR_ERRS(0x0006UL)   /* 无效的输入大小 */
#define CR_DEVB_ERR_CONTROL_OSIZE   CR_ERRS(0x0007UL)   /* 无效的输出大小 */
#define CR_DEVB_ERR_CONTROL_RETTS   CR_ERRS(0x0008UL)   /* 无效的返回大小 */
#define CR_DEVB_ERR_ADDR_OVERFLOW   CR_ERRS(0x0009UL)   /* 指定的地址溢出 */
#define CR_DEVB_ERR_DATA_CLOSED     CR_ERRS(0x000AUL)   /* 数据链路已断开 */
#define CR_DEVB_ERR_READ_TIMEOUT    CR_ERRS(0x000BUL)   /* 数据读取超时 */
#define CR_DEVB_ERR_WRITE_TIMEOUT   CR_ERRS(0x000CUL)   /* 数据写入超时 */

/* 块设备管理结构 */
typedef struct
{
        /* 块设备名称 (哈希查找用) */
        const ansi_t*   device_name;

        /* 打开块设备对象的操作回调 */
        devb_t  (*open) (const ansi_t*);

} sDEV_BNODE;

/* 块设备接口结构 */
typedef struct
{
        /* 块设备接口回调 */
        void_t  (*close) (devb_t);
        leng_t  (*read ) (devb_t, void_t*, leng_t, leng_t);
        leng_t  (*write) (devb_t, const void_t*, leng_t, leng_t);
        bool_t  (*ioctl) (devb_t, uint_t, void_t*, leng_t,
                          const void_t*, leng_t, leng_t*);

        int32u  error;      /* 最后一次操作出错代码 */

        /*==========================================*/
        /* 下面是块设备自己的数据, 隐藏在内部不可见 */
        /*==========================================*/

} sDEV_BLOCK;

/* 块设备信息结构 */
typedef struct
{
        leng_t          page_cnts;  /* 页的个数 */
        leng_t          page_size;  /* 页的大小 */
        leng_t          devb_size;  /* 设备大小 */
        const ansi_t*   devb_name;  /* 设备标识 */

} sDEV_BINFO;

/***** 块设备管理组 *****/
CR_API devb_mgr_t   devb_mgr_init (uint_t count);
CR_API void_t       devb_mgr_free (devb_mgr_t devb_mgr);
CR_API bool_t       devb_mgr_load (devb_mgr_t devb_mgr,
                                   const sDEV_BNODE *devb_node);
CR_API bool_t       devb_mgr_unload (devb_mgr_t devb_mgr,
                                     const ansi_t *devb_name);
/***** 块设备操作组 *****/
CR_API devb_t       devb_open (devb_mgr_t devb_mgr,
                               const ansi_t *devb_name,
                               const ansi_t *sub_type);
CR_API bool_t       devb_close (devb_t devb);
CR_API int32u       devb_get_error (devb_t devb);
CR_API void_t       devb_set_error (devb_t devb, int32u error);
CR_API leng_t       devb_read  (devb_t devb, void_t *data,
                                leng_t addr, leng_t size);
CR_API leng_t       devb_write (devb_t devb, const void_t *data,
                                leng_t addr, leng_t size);
CR_API bool_t       devb_ioctl (devb_t devb, uint_t cmd_code,
                                void_t *obuff, leng_t osize,
                                const void_t *ibuff, leng_t isize,
                                leng_t *ret_size);

#endif  /* !__CR_DEVLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
