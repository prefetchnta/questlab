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
/*  >>>>>>>>>>>>>>>>>> CrHack NXP PCF8583 器件定义头文件 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_PCF8583_H__
#define __CR_PCF8583_H__

#include "../defs.h"

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* PCF8583 内存结构图 */
CR_TYPEDEF struct
{
/*000*/ byte_t  d_config;   /* control/status */
/*001*/ byte_t  d_sec100;   /* hundredth of a second */
/*002*/ byte_t  d_second;   /* seconds */
/*003*/ byte_t  d_minute;   /* minutes */
/*004*/ byte_t  d_m_hour;   /* hours */
/*005*/ byte_t  d_yr_day;   /* year/date */
/*006*/ byte_t  d_wk_mth;   /* weekday/month */
/*007*/ byte_t  d_dtimer;   /* timer */
/*008*/ byte_t  a_calarm;   /* alarm control */
/*009*/ byte_t  a_sec100;   /* hundredth of a second */
/*00A*/ byte_t  a_second;   /* alarm seconds */
/*00B*/ byte_t  a_minute;   /* alarm minutes */
/*00C*/ byte_t  a_m_hour;   /* alarm hours */
/*00D*/ byte_t  a_yr_day;   /* alarm date */
/*00E*/ byte_t  a_wk_mth;   /* alarm month */
/*00F*/ byte_t  a_dtimer;   /* alarm timer */
/*010*/ byte_t  data[240];  /* free RAM */
/*100*/
} CR_PACKED sPCF8583;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

#endif  /* !__CR_PCF8583_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
