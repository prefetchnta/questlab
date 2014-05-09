
#include "QstComm.h"

/*
=======================================
    DOS Shell 命令行
=======================================
*/
CR_API void_t*
qst_dos_tran (
  __CR_IN__ const ansi_t*   string,
  __CR_OT__ uint_t*         ot_size
    )
{
    leng_t  len;
    ansi_t* dat;

    len = str_lenA(string);
    dat = str_allocA(len + 2);
    if (dat == NULL)
        return (NULL);
    mem_cpy(dat, string, len);
    dat[len + 0] = '\r';
    dat[len + 1] = '\n';
    *ot_size = len + 2;
    return (dat);
}

/*
=======================================
    Unix Shell 命令行
=======================================
*/
CR_API void_t*
qst_unx_tran (
  __CR_IN__ const ansi_t*   string,
  __CR_OT__ uint_t*         ot_size
    )
{
    leng_t  len;
    ansi_t* dat;

    len = str_lenA(string);
    dat = str_allocA(len + 1);
    if (dat == NULL)
        return (NULL);
    mem_cpy(dat, string, len);
    dat[len + 0] = '\n';
    *ot_size = len + 1;
    return (dat);
}

/*
=======================================
    Mac Shell 命令行
=======================================
*/
CR_API void_t*
qst_mac_tran (
  __CR_IN__ const ansi_t*   string,
  __CR_OT__ uint_t*         ot_size
    )
{
    leng_t  len;
    ansi_t* dat;

    len = str_lenA(string);
    dat = str_allocA(len + 1);
    if (dat == NULL)
        return (NULL);
    mem_cpy(dat, string, len);
    dat[len + 0] = '\r';
    *ot_size = len + 1;
    return (dat);
}

/*
=======================================
    16进制数据发送
=======================================
*/
CR_API void_t*
qst_hex_tran (
  __CR_IN__ const ansi_t*   string,
  __CR_OT__ uint_t*         ot_size
    )
{
    leng_t  len;
    void_t* dat;

    len = str_lenA(string);
    if (len == 0)
        return (NULL);
    len += 1;
    if (len > 1) len /= 2;
    dat = mem_malloc(len + 1);
    if (dat == NULL)
        return (NULL);
    str2datA(dat, &len, string);
    if (len == 0) {
        mem_free(dat);
        return (NULL);
    }
    *ot_size = len;
    return (dat);
}

/*
=======================================
    转义字符串发送
=======================================
*/
CR_API void_t*
qst_esc_tran (
  __CR_IN__ const ansi_t*   string,
  __CR_OT__ uint_t*         ot_size
    )
{
    leng_t  len;
    ansi_t* dat;
    ansi_t* str;

    str = str_fmtA("\"%s\"", string);
    if (str == NULL)
        return (NULL);
    dat = str_esc_dupU(str, &len);
    mem_free(str);
    *ot_size = len;
    return (dat);
}
