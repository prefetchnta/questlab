/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-08-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 1WIRE 总线定义头文件 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_1WIRE_H__
#define __CR_1WIRE_H__

#include "../defs.h"

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* 1-WIRE ROM 码结构 */
CR_TYPEDEF struct
{
        byte_t  family;     /* 家族编号 */
        byte_t  serial[6];  /* 序列编号 */
        byte_t  crc8d;      /* CRC-8/DALLAS */

} CR_PACKED sWIRE1_ROM;

/* 通用的 1-WIRE 命令字 */
#define WIRE1_ROM_READ      0x33    /* 读64位 ROM  (单节点) */
#define WIRE1_ROM_SKIP      0xCC    /* 跳过 ROM 码 (单节点) */
#define WIRE1_ROM_FIND      0xF0    /* 搜索所有设备的 ROM 码 */
#define WIRE1_ROM_MATCH     0x55    /* 匹配指定设备的 ROM 码 */
#define WIRE1_ROM_ALARM     0xEC    /* 搜索报警设备的 ROM 码 */

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

#endif  /* !__CR_1WIRE_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
