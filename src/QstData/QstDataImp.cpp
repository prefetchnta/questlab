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
    INT08S
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
    INT08U
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

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

/* 导出所有数值观察单元 */
CR_API const sQDAT_UNIT viewer[] =
{
    { "INT08S", int08s_show },
    { "INT08U", int08u_show },
    { "INT16S", int16s_show },
    { "INT16U", int16u_show },
    { "INT32S", int32s_show },
    { "INT32U", int32u_show },
    { "INT64S", int64s_show },
    { "INT64U", int64u_show },
    { NULL, NULL }
};
