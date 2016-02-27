/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-15  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 日期时间头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_RTCLIB_H__
#define __CR_RTCLIB_H__

#include "defs.h"

/*****************************************************************************/
/*                               日期时间计算                                */
/*****************************************************************************/

/* 日期时间结构 */
typedef struct
{
        uint_t  year;       /* 年 */
        ufast_t month;      /* 月 */
        ufast_t day;        /* 日 */
        ufast_t hour;       /* 时 */
        ufast_t minute;     /* 分 */
        ufast_t second;     /* 秒 */
        ufast_t week;       /* 周 */

} sDATETIME;

/* 日期时间属性 */
typedef struct
{
        bool_t  year_leap;  /* 是否闰年 */
        uint_t  year_days;  /* 年总天数 */
        uint_t  days_year;  /* 年内天数 */
        int32u  days_secs;  /* 天内秒数 */
        ufast_t month_day;  /* 月总天数 */

} sDATEATTR;

/* 日期时间读写 */
CR_API bool_t   datetime_get (sDATETIME *datetime);
CR_API bool_t   datetime_set (const sDATETIME *datetime);

/* 日期时间计算 */
CR_API ufast_t  date_set_week (sDATETIME *date);
CR_API ufast_t  date_get_week (const sDATETIME *date);
CR_API void_t   date_inc (sDATETIME *date, int32u days);
CR_API void_t   date_dec (sDATETIME *date, int32u days);
CR_API void_t   time_inc (sDATETIME *time, int32u seconds);
CR_API void_t   time_dec (sDATETIME *time, int32u seconds);
CR_API maxs_t   date_sub (const sDATETIME *date1,
                          const sDATETIME *date2);
CR_API bool_t   datetime_chk (const sDATETIME *datetime);
CR_API maxs_t   datetime_sub (const sDATETIME *datetime1,
                              const sDATETIME *datetime2);
CR_API sint_t   datetime_cmp (const sDATETIME *datetime1,
                              const sDATETIME *datetime2);
CR_API bool_t   datetime_attr (sDATEATTR *dateattr,
                               const sDATETIME *datetime);
CR_API bool_t   str2datetimeA (sDATETIME *datetime, const ansi_t *string,
                               ansi_t split_char CR_DEFAULT(0x20));
CR_API bool_t   str2datetimeW (sDATETIME *datetime, const wide_t *string,
                               wide_t split_char CR_DEFAULT(0x20));

/*****************************************************************************/
/*                               日期时间解析                                */
/*****************************************************************************/

/* DOS (FAT) */
CR_API bool_t   date_to_dos (int16u *value, const sDATETIME *date);
CR_API bool_t   time_to_dos (int16u *value, const sDATETIME *time);
CR_API bool_t   date_from_dos (sDATETIME *date, int16u value);
CR_API bool_t   time_from_dos (sDATETIME *time, int16u value);
CR_API bool_t   datetime_to_dos (int16u value[2], const sDATETIME *datetime);
CR_API bool_t   datetime_from_dos (sDATETIME *datetime, const int16u value[2]);

/* WIN32 (FILETIME) */
CR_API bool_t   datetime_to_w32 (int64u *value, const sDATETIME *datetime);
CR_API bool_t   datetime_from_w32 (sDATETIME *datetime, int64u value);

/* UNIX (time64_t) */
CR_API bool_t   datetime_to_unx (int64u *value, const sDATETIME *datetime);
CR_API bool_t   datetime_from_unx (sDATETIME *datetime, int64u value);

/* MACOS (longdt) */
CR_API bool_t   datetime_to_mac (int64u *value, const sDATETIME *datetime);
CR_API bool_t   datetime_from_mac (sDATETIME *datetime, int64u value);

/* WIN32 (OLETIME) */
CR_API bool_t   datetime_to_ole (fp64_t *value, const sDATETIME *datetime);
CR_API bool_t   datetime_from_ole (sDATETIME *datetime, fp64_t value);

/* DATETIME (BCD) */
typedef struct
{
        byte_t  yr_hi;      /* 纪 */
        byte_t  yr_lo;      /* 年 */
        byte_t  month;      /* 月 */
        byte_t  day;        /* 日 */
        byte_t  hour;       /* 时 */
        byte_t  minute;     /* 分 */
        byte_t  second;     /* 秒 */

} sBCD_DT;

CR_API bool_t   datetime_to_bcd (sBCD_DT *value, const sDATETIME *datetime);
CR_API bool_t   datetime_from_bcd (sDATETIME *datetime, const sBCD_DT *value);

#endif  /* !__CR_RTCLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
