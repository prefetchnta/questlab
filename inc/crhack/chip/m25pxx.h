/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-03-03  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack M25PXX 器件定义头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_M25PXX_H__
#define __CR_M25PXX_H__

#include "../defs.h"

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* M25PXX 器件 ID */
CR_TYPEDEF struct
{
        byte_t  mak_id;     /* 厂家代码 0x20 */
        byte_t  mem_id;     /* 内存类型 0x20 */
        byte_t  cap_sz;     /* 器件容量 0x10 = M25P05
                                        0x11 = M25P10
                                        0x12 = M25P20
                                        0x13 = M25P40
                                        0x14 = M25P80
                                        0x15 = M25P16
                                        0x16 = M25P32
                            */
} CR_PACKED sM25PXX;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* M25PXX 状态寄存器位值 */
#define M25PXX_WIP      0x01    /*  是否正在写数据  */
#define M25PXX_WEL      0x02    /*  是否可以写数据  */
#define M25PXX_BP0      0x04    /*  芯片数据保护位  */
#define M25PXX_BP1      0x08    /*  芯片数据保护位  */
#define M25PXX_BP2      0x10    /*  芯片数据保护位  */
#define M25PXX_SRWD     0x80    /* 状态寄存器写保护 */

/* M25PXX 参数值 */
#define M25PXX_SPGE_SIZE        128     /* 小数据页大小 */
#define M25PXX_BPGE_SIZE        256     /* 大数据页大小 */
#define M25PXX_SSEC_SIZE    CR_K2B(32)  /* 小扇区的大小 */
#define M25PXX_BSEC_SIZE    CR_K2B(64)  /* 大扇区的大小 */
#define M25P05_CHIP_SIZE    CR_K2B(64)  /*  M25P05 大小 */
#define M25P10_CHIP_SIZE    CR_K2B(128) /*  M25P10 大小 */
#define M25P20_CHIP_SIZE    CR_K2B(256) /*  M25P20 大小 */
#define M25P40_CHIP_SIZE    CR_K2B(512) /*  M25P40 大小 */
#define M25P80_CHIP_SIZE    CR_M2B(1)   /*  M25P80 大小 */
#define M25P16_CHIP_SIZE    CR_M2B(2)   /*  M25P16 大小 */
#define M25P32_CHIP_SIZE    CR_M2B(4)   /*  M25P32 大小 */

#endif  /* !__CR_M25PXX_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
