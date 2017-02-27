/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-10-27  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack M25PEXX 器件定义头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_M25PEXX_H__
#define __CR_M25PEXX_H__

#include "../defs.h"

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* M25PEXX 器件 ID */
CR_TYPEDEF struct
{
        byte_t  mak_id;     /* 厂家代码 0x20 */
        byte_t  mem_id;     /* 内存类型 0x80 */
        byte_t  cap_sz;     /* 器件容量 0x11 = M25PE10
                                        0x12 = M25PE20
                                        0x13 = M25PE40
                                        0x14 = M25PE80
                                        0x15 = M25PE16
                            */
} CR_PACKED sM25PEXX3;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* M25PEXX 状态寄存器位值 */
#define M25PEXX_WIP     0x01    /*  是否正在写数据  */
#define M25PEXX_WEL     0x02    /*  是否可以写数据  */
#define M25PEXX_BP0     0x04    /*  芯片数据保护位  */
#define M25PEXX_BP1     0x08    /*  芯片数据保护位  */
#define M25PEXX_BP2     0x10    /*  芯片数据保护位  */
#define M25PEXX_SRWD    0x80    /* 状态寄存器写保护 */

/* M25PEXX 锁定寄存器位置 (软件写保护) */
#define M25PEXX_SWL     0x01    /* 大扇区写保护位 */
#define M25PEXX_SLD     0x02    /* 锁定位写保护位 */

/* M25PEXX 参数值 */
#define M25PEXX_PAGE_SIZE       256     /* 数据页的大小 */
#define M25PEXX_SSEC_SIZE   CR_K2B(4)   /* 小扇区的大小 */
#define M25PEXX_BBLK_SIZE   CR_K2B(64)  /* 大区块的大小 */
#define M25PE10_CHIP_SIZE   CR_K2B(128) /* M25PE10 大小 */
#define M25PE20_CHIP_SIZE   CR_K2B(256) /* M25PE20 大小 */
#define M25PE40_CHIP_SIZE   CR_K2B(512) /* M25PE40 大小 */
#define M25PE80_CHIP_SIZE   CR_M2B(1)   /* M25PE80 大小 */
#define M25PE16_CHIP_SIZE   CR_M2B(2)   /* M25PE16 大小 */

#endif  /* !__CR_M25PEXX_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
