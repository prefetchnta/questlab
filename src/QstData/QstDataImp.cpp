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

#define BEA_USE_STDCALL
#include "BeaEngine.h"

/* 外部库引用 */
#pragma comment (lib, "BeaEngine.lib")

/* 解析执行体 */
ansi_t* (*s_unasm_doit) (const void_t *data, leng_t size);

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
            s_unasm_doit = NULL;
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}

#endif  /* _CR_BUILD_DLL_ */

/* 星期字符串 */
static const ansi_t*    s_week[] =
{
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
};

/*
---------------------------------------
    判断浮点数是否正常
---------------------------------------
*/
template<class T>
static bool_t
is_fp_okay (
  __CR_IN__ T   val
    )
{
    ansi_t  str[64], *ptr;

    sprintf(str, "%G", val);
    for (ptr = str; *ptr != 0x00; ptr++) {
        if ((*ptr < '0' || *ptr > '9') &&
            (*ptr != '+' && *ptr != '-') &&
            (*ptr != '.' && *ptr != 'E'))
            return (FALSE);
    }
    return (TRUE);
}

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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    ansi_t  val;

    CR_NOUSE(is_be);
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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    byte_t  val;

    CR_NOUSE(is_be);
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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int16s  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int16s*)data;
    if (is_be) val = xchg_int16u(val);
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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int16u  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int16u*)data;
    if (is_be) val = xchg_int16u(val);
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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int32s  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int32s*)data;
    if (is_be) val = xchg_int32u(val);
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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int32u  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int32u*)data;
    if (is_be) val = xchg_int32u(val);
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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int64s  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int64s*)data;
    if (is_be) val = xchg_int64u(val);
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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int64u  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(int64u*)data;
    if (is_be) val = xchg_int64u(val);
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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    fp16_t  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(fp16_t*)data;
    if (is_be) val = xchg_int16u(val);
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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    fp32_t  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(fp32_t*)data;
    if (is_be) val = cvt_i2f(xchg_int32u(cvt_f2i(val)));
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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    fp64_t  val;

    if (size < sizeof(val))
        return (NULL);
    val = *(fp64_t*)data;
    if (is_be) val = cvt_i2d(xchg_int64u(cvt_d2i(val)));
    return (str_fmtA(": %G", val));
}

/*
---------------------------------------
    DOS DATE
---------------------------------------
*/
static ansi_t*
dosdate_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int16u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int16u*)data;
    if (is_be) val = xchg_int16u(val);
    if (!date_from_dos(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u %s",
        dtm.year, dtm.month, dtm.day, s_week[dtm.week]));
}

/*
---------------------------------------
    DOS TIME
---------------------------------------
*/
static ansi_t*
dostime_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int16u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int16u*)data;
    if (is_be) val = xchg_int16u(val);
    if (!time_from_dos(&dtm, val))
        return (NULL);
    return (str_fmtA(": %02u:%02u:%02u",
        dtm.hour, dtm.minute, dtm.second));
}

/*
---------------------------------------
    DOS DATETIME
---------------------------------------
*/
static ansi_t*
dosdttm_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int16u  val[2];
    /* --------- */
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    mem_cpy(val, data, sizeof(val));
    if (is_be) {
        val[0] = xchg_int16u(val[0]);
        val[1] = xchg_int16u(val[1]);
    }
    if (!datetime_from_dos(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u %s %02u:%02u:%02u",
        dtm.year, dtm.month, dtm.day, s_week[dtm.week],
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
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int64u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int64u*)data;
    if (is_be) val = xchg_int64u(val);
    if (!datetime_from_w32(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u %s %02u:%02u:%02u",
        dtm.year, dtm.month, dtm.day, s_week[dtm.week],
                dtm.hour, dtm.minute, dtm.second));
}

/*
---------------------------------------
    time32_t
---------------------------------------
*/
static ansi_t*
time32_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int32u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int32u*)data;
    if (is_be) val = xchg_int32u(val);
    if (!datetime_from_unx(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u %s %02u:%02u:%02u",
        dtm.year, dtm.month, dtm.day, s_week[dtm.week],
                dtm.hour, dtm.minute, dtm.second));
}

/*
---------------------------------------
    time64_t
---------------------------------------
*/
static ansi_t*
time64_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int64u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int64u*)data;
    if (is_be) val = xchg_int64u(val);
    if (!datetime_from_unx(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u %s %02u:%02u:%02u",
        dtm.year, dtm.month, dtm.day, s_week[dtm.week],
                dtm.hour, dtm.minute, dtm.second));
}

/*
---------------------------------------
    longdt
---------------------------------------
*/
static ansi_t*
longdt_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int64u      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(int64u*)data;
    if (is_be) val = xchg_int64u(val);
    if (!datetime_from_mac(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u %s %02u:%02u:%02u",
        dtm.year, dtm.month, dtm.day, s_week[dtm.week],
                dtm.hour, dtm.minute, dtm.second));
}

/*
---------------------------------------
    OLETIME
---------------------------------------
*/
static ansi_t*
oletime_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    fp64_t      val;
    sDATETIME   dtm;

    if (size < sizeof(val))
        return (NULL);
    val = *(fp64_t*)data;
    if (is_be) val = cvt_i2d(xchg_int64u(cvt_d2i(val)));
    if (!is_fp_okay<fp64_t>(val))
        return (NULL);
    if (!datetime_from_ole(&dtm, val))
        return (NULL);
    return (str_fmtA(": %04u-%02u-%02u %s %02u:%02u:%02u",
        dtm.year, dtm.month, dtm.day, s_week[dtm.week],
                dtm.hour, dtm.minute, dtm.second));
}

/*
---------------------------------------
    STRING
---------------------------------------
*/
static ansi_t*
string_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    leng_t  idx;
    ansi_t* ptr;
    ansi_t* ret;

    CR_NOUSE(is_be);
    if (size == 0)
        return (NULL);
    ptr = (ansi_t*)data;
    for (idx = 0; idx < size; idx++) {
        if (!is_graphA(*ptr) && *ptr != ' ')
            break;
        ptr++;
    }
    if (idx == 0)
        return (NULL);
    ptr = str_allocA(idx + 1);
    if (ptr == NULL)
        return (NULL);
    mem_cpy(ptr, data, idx);
    ptr[idx] = 0x00;
    ret = str_fmtA(": \"%s\"", ptr);
    mem_free(ptr);
    return (ret);
}

/* BeaEngine 模式值 */
#define BEA_DEF_OPT (NoTabulation | SuffixedNumeral | \
                            ShowSegmentRegs)
static UInt32   s_BeaArchi;
static UInt64   s_BeaOptions = (BEA_DEF_OPT | MasmSyntax);

/*
---------------------------------------
    UNASM BeaEngine
---------------------------------------
*/
static ansi_t*
unasm_bea_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int     retc;
    DISASM  unasm;

    struct_zero(&unasm, DISASM);
    unasm.EIP = (UIntPtr)data;
    unasm.SecurityBlock = (UIntPtr)size;
    unasm.Archi = s_BeaArchi;
    unasm.Options = s_BeaOptions;
    retc = Disasm(&unasm);
    if (retc <= 0)
        return (NULL);
    return (str_fmtA(": (%u) %s", retc, unasm.CompleteInstr));
}

/*
---------------------------------------
    UNASM
---------------------------------------
*/
static ansi_t*
unasm_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    CR_NOUSE(is_be);
    if (s_unasm_doit == NULL)
        return (NULL);
    return (s_unasm_doit(data, size));
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
    { "DOS DATE", dosdate_show },
    { "DOS TIME", dostime_show },
    { "DOS DTTM", dosdttm_show },
    { "FILETIME", filetime_show },
    { "time32_t", time32_show },
    { "time64_t", time64_show },
    { "longdt", longdt_show },
    { "OLETIME", oletime_show },
    { "STRING", string_show },
    { "UNASM", unasm_show },
    { NULL, NULL }
};

/*
=======================================
    设置数据类型
=======================================
*/
CR_API void_t
data_type (
  __CR_IN__ const ansi_t*   type
    )
{
    /* BeaEngine */
    if (chr_cmpA(type, "Bea:", 4) == 0) {
        type += 4;
        s_unasm_doit = unasm_bea_show;
        if (strcmp(type, "X86") == 0)
            s_BeaArchi = 0;
        else
        if (strcmp(type, "X64") == 0)
            s_BeaArchi = 64;
        else
        if (strcmp(type, "8086") == 0)
            s_BeaArchi = 16;
        return;
    }
}

/*
=======================================
    设置数据模式
=======================================
*/
CR_API void_t
data_mode (
  __CR_IN__ const ansi_t*   mode
    )
{
    /* BeaEngine */
    if (chr_cmpA(mode, "Bea:", 4) == 0) {
        mode += 4;
        s_BeaOptions = BEA_DEF_OPT;
        if (strcmp(mode, "Masm") == 0)
            s_BeaOptions |= MasmSyntax;
        else
        if (strcmp(mode, "GoAsm") == 0)
            s_BeaOptions |= GoAsmSyntax;
        else
        if (strcmp(mode, "Nasm") == 0)
            s_BeaOptions |= NasmSyntax;
        else
        if (strcmp(mode, "AT&T") == 0)
            s_BeaOptions |= ATSyntax;
        return;
    }
}
