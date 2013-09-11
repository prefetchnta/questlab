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
/*  >>>>>>>>>>>>>>>>>>>> CrHack ISO14443 通讯协议头文件 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_ISO14443_H__
#define __CR_ISO14443_H__

#include "comm.h"

/*****************************************************************************/
/*                            ISO14443 通讯协议                              */
/*****************************************************************************/

/* 设置状态字 */
#define ISO14443_BELL   0x01    /* 蜂鸣器鸣叫 */
#define ISO14443_LED    0x02    /* LED 灯亮起 */
#define ISO14443_OKAY   0x10    /* 成功时亮并叫 */
#define ISO14443_AUTO   0x20    /* 自动检测卡 */

/* 密钥类型选择 */
#define ISO14443_KEYA   0x00    /* 使用 KeyA */
#define ISO14443_KEYB   0x80    /* 使用 KeyB */

/***** 异步串口部分 *****/
CR_API ansi_t*  iso14443_sio_get_ver (uint_t port, byte_t addr);
CR_API bool_t   iso14443_sio_setup (uint_t port, byte_t addr, byte_t param);
CR_API bool_t   iso14443_sio_online (uint_t port, byte_t addr);
CR_API bool_t   iso14443_sio_set_addr (uint_t port, byte_t addr, byte_t naddr);
CR_API bool_t   iso14443_sio_rd_blk (uint_t port, byte_t addr, byte_t keyab,
                            byte_t nkey, byte_t nblock, byte_t data[16]);
CR_API bool_t   iso14443_sio_wr_blk (uint_t port, byte_t addr, byte_t keyab,
                            byte_t nkey, byte_t nblock, const byte_t data[16]);
CR_API bool_t   iso14443_sio_stop (uint_t port, byte_t addr);
CR_API bool_t   iso14443_sio_get_uid (uint_t port, byte_t addr, byte_t uid[5]);
CR_API bool_t   iso14443_sio_set_eep (uint_t port, byte_t addr, byte_t nkey,
                                      const byte_t data[6]);
CR_API bool_t   iso14443_sio_set_key (uint_t port, byte_t addr, byte_t keyab,
                            byte_t nkey, byte_t nblock, const byte_t data[16]);
CR_API bool_t   iso14443_sio_vsub (uint_t port, byte_t addr, byte_t keyab,
                            byte_t nkey, byte_t nblock, const byte_t data[4]);
CR_API bool_t   iso14443_sio_vadd (uint_t port, byte_t addr, byte_t keyab,
                            byte_t nkey, byte_t nblock, const byte_t data[4]);
CR_API bool_t   iso14443_sio_rd_blks (uint_t port, byte_t addr, byte_t start,
                                      void_t *data, uint_t size);
CR_API bool_t   iso14443_sio_wr_blks (uint_t port, byte_t addr, byte_t start,
                                      const void_t *data, uint_t size);

#endif  /* !__CR_ISO14443_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
