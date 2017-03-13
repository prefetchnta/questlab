/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-13  */
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
/*  >>>>>>>>>>>>>>>>> CrHack EPSON RX-8025 器件定义头文件 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_RX8025_H__
#define __CR_RX8025_H__

#include "../defs.h"

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* RX-8025 内存结构图 */
CR_TYPEDEF struct
{
/*000*/ byte_t  d_second;   /* Seconds */
/*001*/ byte_t  d_minute;   /* Minutes */
/*002*/ byte_t  d_hour;     /* Hours */
/*003*/ byte_t  d_week;     /* Weekdays */
/*004*/ byte_t  d_day;      /* Days */
/*005*/ byte_t  d_month;    /* Months */
/*006*/ byte_t  d_year;     /* Years */
/*007*/ byte_t  offset;     /* Digital Offset */
/*008*/ byte_t  aw_minute;  /* Alarm_W ; Minute */
/*009*/ byte_t  aw_hour;    /* Alarm_W ; Hour */
/*00A*/ byte_t  aw_week;    /* Alarm_W ; Weekday */
/*00B*/ byte_t  ad_minute;  /* Alarm_D ; Minute */
/*00C*/ byte_t  ad_hour;    /* Alarm_D ; Hour */
/*00D*/ byte_t  reserved;   /* Reserved */
/*00E*/ byte_t  control1;   /* Control1 */
/*00F*/ byte_t  control2;   /* Control2 */
/*010*/
} CR_PACKED sRX8025;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

#endif  /* !__CR_RX8025_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
