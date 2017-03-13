/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-02-26  */
/*     #######          ###    ###      [KRNL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>> CrHack MXIC MX25LXX 器件定义头文件 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_MX25LXX_H__
#define __CR_MX25LXX_H__

#include "../defs.h"

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* MX25LXX 器件 ID */
CR_TYPEDEF struct
{
        byte_t  mak_id;     /* 厂家代码 0xC2 */
        byte_t  mem_id;     /* 内存类型 0x20 */
        byte_t  cap_sz;     /* 器件容量 0x11 = MX25L10
                                        0x12 = MX25L20
                                        0x13 = MX25L40
                                        0x14 = MX25L80
                                        0x15 = MX25L16
                            */
} CR_PACKED sMX25LXX3;

/* MX25LXX 制造 ID */
CR_TYPEDEF struct
{
        byte_t  mak_id;     /* 厂家代码 0xC2 */
        byte_t  mem_id;     /* 器件类型 0x10 = MX25L10
                                        0x11 = MX25L20
                                        0x12 = MX25L40
                                        0x13 = MX25L80
                                        0x14 = MX25L16
                            */
} CR_PACKED sMX25LXX2;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* MX25LXX 状态寄存器位值 */
#define MX25LXX_WIP     0x01    /*  是否正在写数据  */
#define MX25LXX_WEL     0x02    /*  是否可以写数据  */
#define MX25LXX_BP0     0x04    /*  芯片数据保护位  */
#define MX25LXX_BP1     0x08    /*  芯片数据保护位  */
#define MX25LXX_BP2     0x10    /*  芯片数据保护位  */
#define MX25LXX_BP3     0x20    /*  芯片数据保护位  */
#define MX25LXX_QE      0x40    /*  是否开启四通道  */
#define MX25LXX_SRWD    0x80    /* 状态寄存器写保护 */

/* MX25LXX 安全寄存器位值 */
#define MX25LXX_SOTP    0x01    /* 安全 OTP 指示位 */
#define MX25LXX_LDSO    0x02    /* 安全 OTP 锁定位 */

/* MX25LXX 参数值 */
#define MX25LXX_PAGE_SIZE       256     /* 数据页的大小 */
#define MX25LXX_SSEC_SIZE   CR_K2B(4)   /* 小扇区的大小 */
#define MX25LXX_BBLK_SIZE   CR_K2B(64)  /* 大区块的大小 */
#define MX25L10_CHIP_SIZE   CR_K2B(128) /* MX25L10 大小 */
#define MX25L20_CHIP_SIZE   CR_K2B(256) /* MX25L20 大小 */
#define MX25L40_CHIP_SIZE   CR_K2B(512) /* MX25L40 大小 */
#define MX25L80_CHIP_SIZE   CR_M2B(1)   /* MX25L80 大小 */
#define MX25L16_CHIP_SIZE   CR_M2B(2)   /* MX25L16 大小 */

#endif  /* !__CR_MX25LXX_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
