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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack W25XXX 器件定义头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_W25XXX_H__
#define __CR_W25XXX_H__

#include "../defs.h"

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* W25XXX 器件 ID */
CR_TYPEDEF struct
{
        byte_t  mak_id;     /* 厂家代码 0xEF */
        byte_t  mem_id;     /* 内存类型 0x30 */
        byte_t  cap_sz;     /* 器件容量 0x11 = W25X10
                                        0x12 = W25X20
                                        0x13 = W25X40
                                        0x14 = W25X80
                                        0x15 = W25X16
                                        0x16 = W25X32
                                        0x17 = W25X64
                            */
} CR_PACKED sW25XXX3;

/* W25XXX 制造 ID */
CR_TYPEDEF struct
{
        byte_t  mak_id;     /* 厂家代码 0xEF */
        byte_t  mem_id;     /* 器件类型 0x10 = W25X10
                                        0x11 = W25X20
                                        0x12 = W25X40
                                        0x13 = W25X80
                                        0x14 = W25X16
                                        0x15 = W25X32
                                        0x16 = W25X64
                            */
} CR_PACKED sW25XXX2;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* W25XXX 状态寄存器位值 */
#define W25XXX_WIP      0x01    /*  是否正在写数据  */
#define W25XXX_WEL      0x02    /*  是否可以写数据  */
#define W25XXX_BP0      0x04    /*  芯片数据保护位  */
#define W25XXX_BP1      0x08    /*  芯片数据保护位  */
#define W25XXX_BP2      0x10    /*  芯片数据保护位  */
#define W25XXX_TBP      0x20    /*  顶底数据保护位  */
#define W25XXX_SRWD     0x80    /* 状态寄存器写保护 */

/* W25XXX 参数值 */
#define W25XXX_PAGE_SIZE        256     /* 数据页的大小 */
#define W25XXX_SSEC_SIZE    CR_K2B(4)   /* 小扇区的大小 */
#define W25XXX_BBLK_SIZE    CR_K2B(64)  /* 大区块的大小 */
#define W25X10_CHIP_SIZE    CR_K2B(128) /*  W25X10 大小 */
#define W25X20_CHIP_SIZE    CR_K2B(256) /*  W25X20 大小 */
#define W25X40_CHIP_SIZE    CR_K2B(512) /*  W25X40 大小 */
#define W25X80_CHIP_SIZE    CR_M2B(1)   /*  W25X80 大小 */
#define W25X16_CHIP_SIZE    CR_M2B(2)   /*  W25X16 大小 */
#define W25X32_CHIP_SIZE    CR_M2B(4)   /*  W25X32 大小 */
#define W25X64_CHIP_SIZE    CR_M2B(8)   /*  W25X64 大小 */

#endif  /* !__CR_W25XXX_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
