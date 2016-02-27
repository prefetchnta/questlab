/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-08-25  */
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
/*  >>>>>>>>>>>>>>>>> CrHack DALLAS DS18B20 器件定义头文件 <<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DS18B20_H__
#define __CR_DS18B20_H__

#include "1wire.h"

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* DS18B20 内存结构图 */
CR_TYPEDEF struct
{
/*000*/ int16s  temp;       /* 温 度 (LE) */
/*002*/ byte_t  th, tl;     /* 用户寄存器 */
/*004*/ byte_t  config;     /* 设置寄存器 */
/*005*/ byte_t  nouse1;     /* 保 留 (FF) */
/*006*/ byte_t  nouse2;     /* 保 留 (FF) */
/*007*/ byte_t  nouse3;     /* 保 留 (10) */
/*008*/ byte_t  crc8d;      /* CRC-8/DALLAS */
/*009*/
} CR_PACKED sDS18B20;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* DS18B20 温度位数设置常数 */
#define DS18B20_09BIT   0x1F    /* 9 位温度 */
#define DS18B20_10BIT   0x3F    /* 10位温度 */
#define DS18B20_11BIT   0x5F    /* 11位温度 */
#define DS18B20_12BIT   0x7F    /* 12位温度 */

/* DS18B20 1-WIRE 命令字 */
#define DS18B20_CONVERT     0x44    /* 初始化温度转换 */
#define DS18B20_PWR_TYPE    0xB4    /* 读取器件供电类型 */
#define DS18B20_RAM_READ    0xBE    /* 读取器件的 RAM (9字节) */
#define DS18B20_RAM_WRITE   0x4E    /* 写入器件的 RAM (3字节) */
#define DS18B20_EEP_READ    0xB8    /* 器件 EEPROM 读到 RAM (3字节) */
#define DS18B20_EEP_WRITE   0x48    /* 器件 RAM 写入 EEPROM (3字节) */

#endif  /* !__CR_DS18B20_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
