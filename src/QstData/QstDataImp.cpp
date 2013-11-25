/*
 ================================================
 *      ######
 *    #########                            ##
 *   ####    ###                           ##
 *   ##       ##                           ##
 *  ##         ## ##   ##   ####     ### ######
 *  ##         ## ##   ##  ######  ##### ######
 *  ##         ## ##   ## ###  ## ###      ##
 *  ##    #    ## ##   ## ##  ##  #####    ##
 *   ##   ##   ## ##   ## ## ##    #####   ##
 *   ###   ## ##  ##   ## ###   #     ##   ##
 *    ##########  ####### ####### ######   ##
 *      #### ##    ######   ####   ####    ##
 *            ##
 *             ##       CREATE: 2013-11-25
 *              #
 ================================================
        QuestLAB 数据探测实现
 ================================================
 */

#include "QstData.h"

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    插件 DLL 入口点
=======================================
*/
BOOL WINAPI
DllMain (
  __CR_IN__ HANDLE  hinst,
  __CR_IN__ DWORD   reason,
  __CR_UU__ LPVOID  reserved
    )
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}

#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/*                                 观察实现                                  */
/*****************************************************************************/

/*
---------------------------------------
    INT8S
---------------------------------------
*/
static ansi_t*
int08s_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    ansi_t  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(ansi_t*)data;
    return (str_fmtA(": %d", val));
}

/*
---------------------------------------
    INT8U
---------------------------------------
*/
static ansi_t*
int08u_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    byte_t  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(byte_t*)data;
    return (str_fmtA(": %u", val));
}

/*
---------------------------------------
    INT16S
---------------------------------------
*/
static ansi_t*
int16s_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int16s  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int16s*)data;
    return (str_fmtA(": %d", val));
}

/*
---------------------------------------
    INT16U
---------------------------------------
*/
static ansi_t*
int16u_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int16u  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int16u*)data;
    return (str_fmtA(": %u", val));
}

/*
---------------------------------------
    INT32S
---------------------------------------
*/
static ansi_t*
int32s_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32s  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int32s*)data;
    return (str_fmtA(": %d", val));
}

/*
---------------------------------------
    INT32U
---------------------------------------
*/
static ansi_t*
int32u_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int32u*)data;
    return (str_fmtA(": %u", val));
}

/*
---------------------------------------
    INT64S
---------------------------------------
*/
static ansi_t*
int64s_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int64s  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int64s*)data;
    return (str_fmtA(": %I64d", val));
}

/*
---------------------------------------
    INT64U
---------------------------------------
*/
static ansi_t*
int64u_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int64u  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int64u*)data;
    return (str_fmtA(": %I64u", val));
}

/*
---------------------------------------
    HALF
---------------------------------------
*/
static ansi_t*
fp16_t_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    fp16_t  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(fp16_t*)data;
    return (str_fmtA(": %G", fp16_to_fp32(val)));
}

/*
---------------------------------------
    FLOAT
---------------------------------------
*/
static ansi_t*
fp32_t_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    fp32_t  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(fp32_t*)data;
    return (str_fmtA(": %G", val));
}

/*
---------------------------------------
    DOUBLE
---------------------------------------
*/
static ansi_t*
fp64_t_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    fp64_t  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(fp64_t*)data;
    return (str_fmtA(": %G", val));
}

/*
---------------------------------------
    DOSDATE
---------------------------------------
*/
static ansi_t*
dosdate_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int16u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int16u*)data;
    if (!date_from_dos(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u",
        dtm.year, dtm.month, dtm.day));
}

/*
---------------------------------------
    DOSTIME
---------------------------------------
*/
static ansi_t*
dostime_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int16u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int16u*)data;
    if (!time_from_dos(&dtm, val))
        return (NULL);
    return (str_fmtA(": %02u:%02u:%02u",
        dtm.hour, dtm.minute, dtm.second));
}

/*
---------------------------------------
    FILETIME
---------------------------------------
*/
static ansi_t*
filetime_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int64u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int64u*)data;
    if (!datetime_from_w32(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u %02u:%02u:%02u",
        dtm.year, dtm.month, dtm.day, dtm.hour, dtm.minute, dtm.second));
}

/*
---------------------------------------
    time32_t
---------------------------------------
*/
static ansi_t*
time32_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int32u*)data;
    if (!datetime_from_unx(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u %02u:%02u:%02u",
        dtm.year, dtm.month, dtm.day, dtm.hour, dtm.minute, dtm.second));
}

/*
---------------------------------------
    time64_t
---------------------------------------
*/
static ansi_t*
time64_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int64u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int64u*)data;
    if (!datetime_from_unx(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u %02u:%02u:%02u",
        dtm.year, dtm.month, dtm.day, dtm.hour, dtm.minute, dtm.second));
}

/*
---------------------------------------
    longdt
---------------------------------------
*/
static ansi_t*
longdt_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int64u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int64u*)data;
    if (!datetime_from_mac(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u %02u:%02u:%02u",
        dtm.year, dtm.month, dtm.day, dtm.hour, dtm.minute, dtm.second));
}

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

/* 导出所有数值观察单元 */
CR_API const sQDAT_UNIT viewer[] =
{
    { "INT8S", int08s_show },
    { "INT8U", int08u_show },
    { "INT16S", int16s_show },
    { "INT16U", int16u_show },
    { "INT32S", int32s_show },
    { "INT32U", int32u_show },
    { "INT64S", int64s_show },
    { "INT64U", int64u_show },
    { "HALF", fp16_t_show },
    { "FLOAT", fp32_t_show },
    { "DOUBLE", fp64_t_show },
    { "DOSDATE", dosdate_show },
    { "DOSTIME", dostime_show },
    { "FILETIME", filetime_show },
    { "time32_t", time32_show },
    { "time64_t", time64_show },
    { "longdt", longdt_show },
    { NULL, NULL }
};
