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
#include "capstone.h"

/* 外部库引用 */
#pragma comment (lib, "BeaEngine.lib")
#pragma comment (lib, "capstone.lib")
#pragma comment (lib, "QstLibs.lib")

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
    INT24S
---------------------------------------
*/
static ansi_t*
int24s_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int32u  val;

    if (size < sizeof(byte_t) * 3)
        return (NULL);
    if (is_be) {
        val  = ((byte_t*)data)[0];
        val <<= 8;
        val |= ((byte_t*)data)[1];
        val <<= 8;
        val |= ((byte_t*)data)[2];
    }
    else {
        val  = ((byte_t*)data)[2];
        val <<= 8;
        val |= ((byte_t*)data)[1];
        val <<= 8;
        val |= ((byte_t*)data)[0];
    }
    if (val & 0x800000)
        val |= 0xFF000000;
    return (str_fmtA(": %d", val));
}

/*
---------------------------------------
    INT24U
---------------------------------------
*/
static ansi_t*
int24u_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int32u  val;

    if (size < sizeof(byte_t) * 3)
        return (NULL);
    if (is_be) {
        val  = ((byte_t*)data)[0];
        val <<= 8;
        val |= ((byte_t*)data)[1];
        val <<= 8;
        val |= ((byte_t*)data)[2];
    }
    else {
        val  = ((byte_t*)data)[2];
        val <<= 8;
        val |= ((byte_t*)data)[1];
        val <<= 8;
        val |= ((byte_t*)data)[0];
    }
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
    INT48S
---------------------------------------
*/
static ansi_t*
int48s_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int64u  val;

    if (size < sizeof(int16u) * 3)
        return (NULL);
    if (is_be) {
        val  = ((byte_t*)data)[0];
        val <<= 8;
        val |= ((byte_t*)data)[1];
        val <<= 8;
        val |= ((byte_t*)data)[2];
        val <<= 8;
        val |= ((byte_t*)data)[3];
        val <<= 8;
        val |= ((byte_t*)data)[4];
        val <<= 8;
        val |= ((byte_t*)data)[5];
    }
    else {
        val  = ((byte_t*)data)[5];
        val <<= 8;
        val |= ((byte_t*)data)[4];
        val <<= 8;
        val |= ((byte_t*)data)[3];
        val <<= 8;
        val |= ((byte_t*)data)[2];
        val <<= 8;
        val |= ((byte_t*)data)[1];
        val <<= 8;
        val |= ((byte_t*)data)[0];
    }
    if (val & 0x800000000000ULL)
        val |= 0xFFFF000000000000ULL;
    return (str_fmtA(": %I64d", val));
}

/*
---------------------------------------
    INT48U
---------------------------------------
*/
static ansi_t*
int48u_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    int64u  val;

    if (size < sizeof(int16u) * 3)
        return (NULL);
    if (is_be) {
        val  = ((byte_t*)data)[0];
        val <<= 8;
        val |= ((byte_t*)data)[1];
        val <<= 8;
        val |= ((byte_t*)data)[2];
        val <<= 8;
        val |= ((byte_t*)data)[3];
        val <<= 8;
        val |= ((byte_t*)data)[4];
        val <<= 8;
        val |= ((byte_t*)data)[5];
    }
    else {
        val  = ((byte_t*)data)[5];
        val <<= 8;
        val |= ((byte_t*)data)[4];
        val <<= 8;
        val |= ((byte_t*)data)[3];
        val <<= 8;
        val |= ((byte_t*)data)[2];
        val <<= 8;
        val |= ((byte_t*)data)[1];
        val <<= 8;
        val |= ((byte_t*)data)[0];
    }
    return (str_fmtA(": %I64u", val));
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
    SLEB128
---------------------------------------
*/
static ansi_t*
sleb128_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    byte_t  cur;
    uint_t  mov = 0;
    int64s  val = 0;

    while (size != 0) {
        cur = *(byte_t*)data;
        data = (byte_t*)data + 1;
        val |= (int64s)(cur & 0x7F) << mov;
        mov += 7;
        if (!(cur & 0x80)) {
            mov = 64 - mov;
            val <<= mov;
            val >>= mov;
            break;
        }
        size--;
    }
    if (size == 0)
        return (NULL);
    CR_NOUSE(is_be);
    return (str_fmtA(": %I64d", val));
}

/*
---------------------------------------
    ULEB128
---------------------------------------
*/
static ansi_t*
uleb128_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    byte_t  cur;
    uint_t  mov = 0;
    int64u  val = 0;

    while (size != 0) {
        cur = *(byte_t*)data;
        data = (byte_t*)data + 1;
        val |= (int64s)(cur & 0x7F) << mov;
        mov += 7;
        if (!(cur & 0x80))
            break;
        size--;
    }
    if (size == 0)
        return (NULL);
    CR_NOUSE(is_be);
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

/********************/
/* BeaEngine 模式值 */
/********************/
static const ansi_t*    s_bea_archi = "X86";
#define BEA_DEF_OPT (NoTabulation | SuffixedNumeral | \
                     ShowSegmentRegs | ShowEVEXMasking)
static UInt32   s_BeaArchi = 32;
static UInt64   s_BeaOptions = (BEA_DEF_OPT | MasmSyntax);

/*
---------------------------------------
    BeaEngine
---------------------------------------
*/
static ansi_t*
unasm_bea_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    sint_t  retc;
    DISASM  unasm;

    CR_NOUSE(is_be);
    struct_zero(&unasm, DISASM);
    unasm.EIP = (UIntPtr)data;
    unasm.SecurityBlock = (UIntPtr)size;
    unasm.Archi = s_BeaArchi;
    unasm.Options = s_BeaOptions;
    retc = Disasm(&unasm);
    if (retc <= 0)
        return (NULL);
    return (str_fmtA(" %s: (%u) %s", s_bea_archi, retc,
                        unasm.CompleteInstr));
}

/*******************/
/* Capstone 模式值 */
/*******************/
static sint_t           s_CapOptions = CS_MODE_ARM;
static sint_t           s_CapByteOrder = CS_MODE_LITTLE_ENDIAN;
static sint_t           s_CapMIPS = 0, s_CapPPC = 0;
static sint_t           s_CapRISCV = 0, s_CapSH = 0;
static cs_arch          s_CapArchi = CS_ARCH_ARM;
static cs_opt_value     s_CapAsmSyn = CS_OPT_SYNTAX_DEFAULT;
static const ansi_t*    s_cap_archi = "ARM";

/*
---------------------------------------
    Capstone
---------------------------------------
*/
static ansi_t*
unasm_cap_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    csh         caph;
    cs_err      retc;
    sint_t      mode;
    size_t      cnts;
    ansi_t*     dasm;
    cs_insn*    insn;

    CR_NOUSE(is_be);
    mode = s_CapOptions | s_CapByteOrder;
    if (s_CapArchi == CS_ARCH_MIPS)
        mode |= s_CapMIPS;
    else
    if (s_CapArchi == CS_ARCH_PPC)
        mode |= s_CapPPC;
    else
    if (s_CapArchi == CS_ARCH_RISCV)
        mode |= s_CapRISCV;
    else
    if (s_CapArchi == CS_ARCH_SH)
        mode |= s_CapSH;
    retc = cs_open(s_CapArchi, (cs_mode)mode, &caph);
    if (retc != 0)
        return (NULL);
    dasm = NULL;
    cs_option(caph, CS_OPT_DETAIL, CS_OPT_OFF);
    cs_option(caph, CS_OPT_SYNTAX, s_CapAsmSyn);
    cnts = cs_disasm(caph, (uint8_t*)data, size, 0, 0, &insn);
    if (cnts != 0)
    {
        const ansi_t*   bo;

        bo = (s_CapByteOrder & CS_MODE_BIG_ENDIAN) ? "be" : "le";
        dasm = str_fmtA(" %s[%s]: (%u) %s %s", s_cap_archi, bo,
                    insn->size, insn->mnemonic, insn->op_str);
        cs_free(insn, cnts);
    }
    cs_close(&caph);
    return (dasm);
}

/***************/
/* MAME 模式值 */
/***************/
static ansi_t*  s_mame_archi = NULL;

/*
---------------------------------------
    MAME unidasm
---------------------------------------
*/
static ansi_t*
unasm_mame_show (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          is_be
    )
{
    FILE*   fp;
    leng_t  len;
    ansi_t* str;
    ansi_t* txt;

    /* 输出执行命令 */
    CR_NOUSE(is_be);
    if (s_mame_archi == NULL || !misc_is_win64())
        return (NULL);
    if (!file_saveA(QST_PATH_OUTPUT "unidasm.bin", data, size))
        return (NULL);
    fp = fopen(QST_PATH_OUTPUT "unidasm.bat", "wb");
    if (fp == NULL)
        return (NULL);
    fprintf(fp, "x64bin\\unidasm.exe "
                QST_PATH_OUTPUT "unidasm.bin -arch %s -count 1 > "
                QST_PATH_OUTPUT "unidasm.txt\r\n", s_mame_archi);
    fclose(fp);

    /* 读取执行结果 */
    misc_call_exe(QST_PATH_OUTPUT "unidasm.bat", TRUE, TRUE);
    str = file_load_as_strA(QST_PATH_OUTPUT "unidasm.txt");
    file_deleteA(QST_PATH_OUTPUT "unidasm.bat");
    file_deleteA(QST_PATH_OUTPUT "unidasm.bin");
    if (str == NULL)
        return (NULL);
    len = str_lenA(str);
    txt = str_strA(str, ": ");
    if (len <= 2 || chr_cmpA(str, "0:", 2) != 0 || txt == NULL) {
        mem_free(str);
        return (NULL);
    }
    str_trimA(str);
    txt = str_fmtA(" %s: %s", s_mame_archi, txt + 2);
    file_deleteA(QST_PATH_OUTPUT "unidasm.txt");
    mem_free(str);
    return (txt);
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
    { "INT24S", int24s_show },
    { "INT24U", int24u_show },
    { "INT32S", int32s_show },
    { "INT32U", int32u_show },
    { "INT48S", int48s_show },
    { "INT48U", int48u_show },
    { "INT64S", int64s_show },
    { "INT64U", int64u_show },
    { "SLEB128", sleb128_show },
    { "ULEB128", uleb128_show },
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
    { "BEA", unasm_bea_show },
    { "CAP", unasm_cap_show },
    { "MAME", unasm_mame_show },
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
        if (strcmp(type, "X86") == 0) {
            s_BeaArchi = 32;
            s_bea_archi = "X86";
        }
        else
        if (strcmp(type, "X64") == 0) {
            s_BeaArchi = 64;
            s_bea_archi = "X64";
        }
        else
        if (strcmp(type, "8086") == 0) {
            s_BeaArchi = 16;
            s_bea_archi = "8086";
        }
        return;
    }

    /* Capstone */
    if (chr_cmpA(type, "Cap:", 4) == 0) {
        type += 4;
        if (strcmp(type, "ARM") == 0) {
            s_CapArchi = CS_ARCH_ARM;
            s_CapOptions = CS_MODE_ARM;
            s_cap_archi = "ARM";
        }
        else
        if (strcmp(type, "Thumb2") == 0) {
            s_CapArchi = CS_ARCH_ARM;
            s_CapOptions = CS_MODE_THUMB;
            s_cap_archi = "ARM(THUMB2)";
        }
        else
        if (strcmp(type, "Thumb8") == 0) {
            s_CapArchi = CS_ARCH_ARM;
            s_CapOptions = CS_MODE_THUMB | CS_MODE_V8;
            s_cap_archi = "ARM(THUMBv8)";
        }
        else
        if (strcmp(type, "MClass") == 0) {
            s_CapArchi = CS_ARCH_ARM;
            s_CapOptions = CS_MODE_THUMB | CS_MODE_MCLASS;
            s_cap_archi = "ARM(Cortex-M)";
        }
        else
        if (strcmp(type, "AArch32") == 0) {
            s_CapArchi = CS_ARCH_ARM;
            s_CapOptions = CS_MODE_ARM | CS_MODE_V8;
            s_cap_archi = "ARM(AArch32)";
        }
        else
        if (strcmp(type, "AArch64") == 0) {
            s_CapArchi = CS_ARCH_ARM64;
            s_CapOptions = CS_MODE_ARM;
            s_cap_archi = "ARM(AArch64)";
        }
        else
        if (strcmp(type, "X86") == 0) {
            s_CapArchi = CS_ARCH_X86;
            s_CapOptions = CS_MODE_32;
            s_cap_archi = "X86";
        }
        else
        if (strcmp(type, "X64") == 0) {
            s_CapArchi = CS_ARCH_X86;
            s_CapOptions = CS_MODE_64;
            s_cap_archi = "X64";
        }
        else
        if (strcmp(type, "8086") == 0) {
            s_CapArchi = CS_ARCH_X86;
            s_CapOptions = CS_MODE_16;
            s_cap_archi = "8086";
        }
        else
        if (strcmp(type, "MIPS32") == 0) {
            s_CapArchi = CS_ARCH_MIPS;
            s_CapOptions = CS_MODE_MIPS32;
            s_cap_archi = "MIPS32";
        }
        else
        if (strcmp(type, "MIPS64") == 0) {
            s_CapArchi = CS_ARCH_MIPS;
            s_CapOptions = CS_MODE_MIPS64;
            s_cap_archi = "MIPS64";
        }
        else
        if (strcmp(type, "PPC32") == 0) {
            s_CapArchi = CS_ARCH_PPC;
            s_CapOptions = CS_MODE_32;
            s_cap_archi = "PPC32";
        }
        else
        if (strcmp(type, "PPC64") == 0) {
            s_CapArchi = CS_ARCH_PPC;
            s_CapOptions = CS_MODE_64;
            s_cap_archi = "PPC64";
        }
        else
        if (strcmp(type, "RISCV32") == 0) {
            s_CapArchi = CS_ARCH_RISCV;
            s_CapOptions = CS_MODE_RISCV32;
            s_cap_archi = "RISCV32";
        }
        else
        if (strcmp(type, "RISCV64") == 0) {
            s_CapArchi = CS_ARCH_RISCV;
            s_CapOptions = CS_MODE_RISCV64;
            s_cap_archi = "RISCV64";
        }
        else
        if (strcmp(type, "M68000") == 0) {
            s_CapArchi = CS_ARCH_M68K;
            s_CapOptions = CS_MODE_M68K_000;
            s_cap_archi = "M68000";
        }
        else
        if (strcmp(type, "M68010") == 0) {
            s_CapArchi = CS_ARCH_M68K;
            s_CapOptions = CS_MODE_M68K_010;
            s_cap_archi = "M68010";
        }
        else
        if (strcmp(type, "M68020") == 0) {
            s_CapArchi = CS_ARCH_M68K;
            s_CapOptions = CS_MODE_M68K_020;
            s_cap_archi = "M68020";
        }
        else
        if (strcmp(type, "M68030") == 0) {
            s_CapArchi = CS_ARCH_M68K;
            s_CapOptions = CS_MODE_M68K_030;
            s_cap_archi = "M68030";
        }
        else
        if (strcmp(type, "M68040") == 0) {
            s_CapArchi = CS_ARCH_M68K;
            s_CapOptions = CS_MODE_M68K_040;
            s_cap_archi = "M68040";
        }
        else
        if (strcmp(type, "M68060") == 0) {
            s_CapArchi = CS_ARCH_M68K;
            s_CapOptions = CS_MODE_M68K_060;
            s_cap_archi = "M68060";
        }
        else
        if (strcmp(type, "MC6301") == 0) {
            s_CapArchi = CS_ARCH_M680X;
            s_CapOptions = CS_MODE_M680X_6301;
            s_cap_archi = "MC6301";
        }
        else
        if (strcmp(type, "MC6309") == 0) {
            s_CapArchi = CS_ARCH_M680X;
            s_CapOptions = CS_MODE_M680X_6309;
            s_cap_archi = "MC6309";
        }
        else
        if (strcmp(type, "MC6800") == 0) {
            s_CapArchi = CS_ARCH_M680X;
            s_CapOptions = CS_MODE_M680X_6800;
            s_cap_archi = "MC6800";
        }
        else
        if (strcmp(type, "MC6801") == 0) {
            s_CapArchi = CS_ARCH_M680X;
            s_CapOptions = CS_MODE_M680X_6801;
            s_cap_archi = "MC6801";
        }
        else
        if (strcmp(type, "MC6805") == 0) {
            s_CapArchi = CS_ARCH_M680X;
            s_CapOptions = CS_MODE_M680X_6805;
            s_cap_archi = "MC6805";
        }
        else
        if (strcmp(type, "MC6808") == 0) {
            s_CapArchi = CS_ARCH_M680X;
            s_CapOptions = CS_MODE_M680X_6808;
            s_cap_archi = "MC6808";
        }
        else
        if (strcmp(type, "MC6809") == 0) {
            s_CapArchi = CS_ARCH_M680X;
            s_CapOptions = CS_MODE_M680X_6809;
            s_cap_archi = "MC6809";
        }
        else
        if (strcmp(type, "MC6811") == 0) {
            s_CapArchi = CS_ARCH_M680X;
            s_CapOptions = CS_MODE_M680X_6811;
            s_cap_archi = "MC6811";
        }
        else
        if (strcmp(type, "HCS12") == 0) {
            s_CapArchi = CS_ARCH_M680X;
            s_CapOptions = CS_MODE_M680X_CPU12;
            s_cap_archi = "HCS12";
        }
        else
        if (strcmp(type, "HCS08") == 0) {
            s_CapArchi = CS_ARCH_M680X;
            s_CapOptions = CS_MODE_M680X_HCS08;
            s_cap_archi = "HCS08";
        }
        else
        if (strcmp(type, "M6502") == 0) {
            s_CapArchi = CS_ARCH_MOS65XX;
            s_CapOptions = CS_MODE_MOS65XX_6502;
            s_cap_archi = "M6502";
        }
        else
        if (strcmp(type, "M65C02") == 0) {
            s_CapArchi = CS_ARCH_MOS65XX;
            s_CapOptions = CS_MODE_MOS65XX_65C02;
            s_cap_archi = "M65C02";
        }
        else
        if (strcmp(type, "W65C02") == 0) {
            s_CapArchi = CS_ARCH_MOS65XX;
            s_CapOptions = CS_MODE_MOS65XX_W65C02;
            s_cap_archi = "W65C02";
        }
        else
        if (strcmp(type, "M65816ss") == 0) {
            s_CapArchi = CS_ARCH_MOS65XX;
            s_CapOptions = CS_MODE_MOS65XX_65816;
            s_cap_archi = "M65816ss";
        }
        else
        if (strcmp(type, "M65816ls") == 0) {
            s_CapArchi = CS_ARCH_MOS65XX;
            s_CapOptions = CS_MODE_MOS65XX_65816_LONG_M;
            s_cap_archi = "M65816ls";
        }
        else
        if (strcmp(type, "M65816sl") == 0) {
            s_CapArchi = CS_ARCH_MOS65XX;
            s_CapOptions = CS_MODE_MOS65XX_65816_LONG_X;
            s_cap_archi = "M65816sl";
        }
        else
        if (strcmp(type, "M65816ll") == 0) {
            s_CapArchi = CS_ARCH_MOS65XX;
            s_CapOptions = CS_MODE_MOS65XX_65816_LONG_MX;
            s_cap_archi = "M65816ll";
        }
        else
        if (strcmp(type, "BPFc") == 0) {
            s_CapArchi = CS_ARCH_BPF;
            s_CapOptions = CS_MODE_BPF_CLASSIC;
            s_cap_archi = "BPFc";
        }
        else
        if (strcmp(type, "BPFe") == 0) {
            s_CapArchi = CS_ARCH_BPF;
            s_CapOptions = CS_MODE_BPF_EXTENDED;
            s_cap_archi = "BPFe";
        }
        else
        if (strcmp(type, "SPARC") == 0) {
            s_CapArchi = CS_ARCH_SPARC;
            s_CapOptions = CS_MODE_BIG_ENDIAN;
            s_cap_archi = "SPARC";
        }
        else
        if (strcmp(type, "SPARCv9") == 0) {
            s_CapArchi = CS_ARCH_SPARC;
            s_CapOptions = CS_MODE_BIG_ENDIAN | CS_MODE_V9;
            s_cap_archi = "SPARCv9";
        }
        else
        if (strcmp(type, "SH2") == 0) {
            s_CapArchi = CS_ARCH_SH;
            s_CapOptions = CS_MODE_SH2;
            s_cap_archi = "SH2";
        }
        else
        if (strcmp(type, "SH2A") == 0) {
            s_CapArchi = CS_ARCH_SH;
            s_CapOptions = CS_MODE_SH2A;
            s_cap_archi = "SH2A";
        }
        else
        if (strcmp(type, "SH3") == 0) {
            s_CapArchi = CS_ARCH_SH;
            s_CapOptions = CS_MODE_SH3;
            s_cap_archi = "SH3";
        }
        else
        if (strcmp(type, "SH4") == 0) {
            s_CapArchi = CS_ARCH_SH;
            s_CapOptions = CS_MODE_SH4;
            s_cap_archi = "SH4";
        }
        else
        if (strcmp(type, "SH4A") == 0) {
            s_CapArchi = CS_ARCH_SH;
            s_CapOptions = CS_MODE_SH4A;
            s_cap_archi = "SH4A";
        }
        else
        if (strcmp(type, "TCORE110") == 0) {
            s_CapArchi = CS_ARCH_TRICORE;
            s_CapOptions = CS_MODE_TRICORE_110;
            s_cap_archi = "TCORE110";
        }
        else
        if (strcmp(type, "TCORE120") == 0) {
            s_CapArchi = CS_ARCH_TRICORE;
            s_CapOptions = CS_MODE_TRICORE_120;
            s_cap_archi = "TCORE120";
        }
        else
        if (strcmp(type, "TCORE130") == 0) {
            s_CapArchi = CS_ARCH_TRICORE;
            s_CapOptions = CS_MODE_TRICORE_130;
            s_cap_archi = "TCORE130";
        }
        else
        if (strcmp(type, "TCORE131") == 0) {
            s_CapArchi = CS_ARCH_TRICORE;
            s_CapOptions = CS_MODE_TRICORE_131;
            s_cap_archi = "TCORE131";
        }
        else
        if (strcmp(type, "TCORE160") == 0) {
            s_CapArchi = CS_ARCH_TRICORE;
            s_CapOptions = CS_MODE_TRICORE_160;
            s_cap_archi = "TCORE160";
        }
        else
        if (strcmp(type, "TCORE161") == 0) {
            s_CapArchi = CS_ARCH_TRICORE;
            s_CapOptions = CS_MODE_TRICORE_161;
            s_cap_archi = "TCORE161";
        }
        else
        if (strcmp(type, "TCORE162") == 0) {
            s_CapArchi = CS_ARCH_TRICORE;
            s_CapOptions = CS_MODE_TRICORE_162;
            s_cap_archi = "TCORE162";
        }
        else
        if (strcmp(type, "EVM") == 0) {
            s_CapArchi = CS_ARCH_EVM;
            s_CapOptions = 0;
            s_cap_archi = "EVM";
        }
        else
        if (strcmp(type, "SystemZ") == 0) {
            s_CapArchi = CS_ARCH_SYSZ;
            s_CapOptions = CS_MODE_BIG_ENDIAN;
            s_cap_archi = "SystemZ";
        }
        else
        if (strcmp(type, "TMS320C64X") == 0) {
            s_CapArchi = CS_ARCH_TMS320C64X;
            s_CapOptions = 0;
            s_cap_archi = "TMS320C64X";
        }
        else
        if (strcmp(type, "XCore") == 0) {
            s_CapArchi = CS_ARCH_XCORE;
            s_CapOptions = 0;
            s_cap_archi = "XCore";
        }
        else
        if (strcmp(type, "WASM") == 0) {
            s_CapArchi = CS_ARCH_WASM;
            s_CapOptions = 0;
            s_cap_archi = "WASM";
        }
        return;
    }

    /* MAME unidasm */
    if (chr_cmpA(type, "MAME:", 5) == 0) {
        type += 5;
        if (s_mame_archi != NULL)
            mem_free(s_mame_archi);
        s_mame_archi = str_dupA(type);
        str_trimA(s_mame_archi);
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
        else
        if (strcmp(mode, "IntrinMem") == 0)
            s_BeaOptions |= IntrinsicMemSyntax;
        return;
    }

    /* Capstone */
    if (chr_cmpA(mode, "Cap:", 4) == 0) {
        mode += 4;
        if (chr_cmpA(mode, "AsmSyn:", 7) == 0) {
            mode += 7;
            if (strcmp(mode, "Default") == 0)
                s_CapAsmSyn = CS_OPT_SYNTAX_DEFAULT;
            else
            if (strcmp(mode, "Intel") == 0)
                s_CapAsmSyn = CS_OPT_SYNTAX_INTEL;
            else
            if (strcmp(mode, "AT&T") == 0)
                s_CapAsmSyn = CS_OPT_SYNTAX_ATT;
            else
            if (strcmp(mode, "Masm") == 0)
                s_CapAsmSyn = CS_OPT_SYNTAX_MASM;
            else
            if (strcmp(mode, "NoReg") == 0)
                s_CapAsmSyn = CS_OPT_SYNTAX_NOREGNAME;
            else
            if (strcmp(mode, "Moto") == 0)
                s_CapAsmSyn = CS_OPT_SYNTAX_MOTOROLA;
        }
        else
        if (chr_cmpA(mode, "ByteOrder:", 10) == 0) {
            mode += 10;
            if (strcmp(mode, "LE") == 0)
                s_CapByteOrder = CS_MODE_LITTLE_ENDIAN;
            else
            if (strcmp(mode, "BE") == 0)
                s_CapByteOrder = CS_MODE_BIG_ENDIAN;
        }
        else
        if (chr_cmpA(mode, "MIPS:", 5) == 0) {
            mode += 5;
            if (strcmp(mode, "II 0") == 0)
                s_CapMIPS &= ~CS_MODE_MIPS2;
            else
            if (strcmp(mode, "II 1") == 0)
                s_CapMIPS |=  CS_MODE_MIPS2;
            else
            if (strcmp(mode, "III 0") == 0)
                s_CapMIPS &= ~CS_MODE_MIPS3;
            else
            if (strcmp(mode, "III 1") == 0)
                s_CapMIPS |=  CS_MODE_MIPS3;
            else
            if (strcmp(mode, "32R6 0") == 0)
                s_CapMIPS &= ~CS_MODE_MIPS32R6;
            else
            if (strcmp(mode, "32R6 1") == 0)
                s_CapMIPS |=  CS_MODE_MIPS32R6;
            else
            if (strcmp(mode, "Micro 0") == 0)
                s_CapMIPS &= ~CS_MODE_MICRO;
            else
            if (strcmp(mode, "Micro 1") == 0)
                s_CapMIPS |=  CS_MODE_MICRO;
        }
        else
        if (chr_cmpA(mode, "PPC:", 4) == 0) {
            mode += 4;
            if (strcmp(mode, "QPX 0") == 0)
                s_CapPPC &= ~CS_MODE_QPX;
            else
            if (strcmp(mode, "QPX 1") == 0)
                s_CapPPC |=  CS_MODE_QPX;
            else
            if (strcmp(mode, "SPE 0") == 0)
                s_CapPPC &= ~CS_MODE_SPE;
            else
            if (strcmp(mode, "SPE 1") == 0)
                s_CapPPC |=  CS_MODE_SPE;
            else
            if (strcmp(mode, "BOOKE 0") == 0)
                s_CapPPC &= ~CS_MODE_BOOKE;
            else
            if (strcmp(mode, "BOOKE 1") == 0)
                s_CapPPC |=  CS_MODE_BOOKE;
            else
            if (strcmp(mode, "PS 0") == 0)
                s_CapPPC &= ~CS_MODE_PS;
            else
            if (strcmp(mode, "PS 1") == 0)
                s_CapPPC |=  CS_MODE_PS;
        }
        else
        if (chr_cmpA(mode, "RISCV:", 6) == 0) {
            mode += 6;
            if (strcmp(mode, "C 0") == 0)
                s_CapRISCV &= ~CS_MODE_RISCVC;
            else
            if (strcmp(mode, "C 1") == 0)
                s_CapRISCV |=  CS_MODE_RISCVC;
        }
        else
        if (chr_cmpA(mode, "SH:", 3) == 0) {
            mode += 3;
            if (strcmp(mode, "FPU 0") == 0)
                s_CapSH &= ~CS_MODE_SHFPU;
            else
            if (strcmp(mode, "FPU 1") == 0)
                s_CapSH |=  CS_MODE_SHFPU;
            else
            if (strcmp(mode, "DSP 0") == 0)
                s_CapSH &= ~CS_MODE_SHDSP;
            else
            if (strcmp(mode, "DSP 1") == 0)
                s_CapSH |=  CS_MODE_SHDSP;
        }
        return;
    }
}
