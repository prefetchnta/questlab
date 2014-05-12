
#include "QstCommInt.h"

/*
=======================================
    文本数据显示
=======================================
*/
CR_API void_t
qst_txt_show (
  __CR_IN__ void_t*         parm,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    ansi_t  cha;
    ansi_t* show;

    show = str_allocA(size + 1);
    if (show == NULL)
        return;

    sQstComm*   ctx = (sQstComm*)parm;
    CTextOper*  opr = (CTextOper*)ctx->oper;

    /* 过滤无法显示的字符 */
    for (uint_t idx = 0; idx < size; idx++) {
        cha = *(ansi_t*)data;
        data = (ansi_t*)data + 1;
        if (cha != CR_AC('\n') &&
            cha != CR_AC('\r') && !is_printA(cha))
            cha = CR_AC(' ');
        show[idx] = cha;
    }
    show[size] = NIL;
    opr->text(show);
    mem_free(show);
}

/*
=======================================
    16进制数据显示
=======================================
*/
CR_API void_t
qst_hex_show (
  __CR_IN__ void_t*         parm,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    ansi_t* ptr;
    ansi_t* show;

    show = str_allocA(size * 3 + 1);
    if (show == NULL)
        return;
    ptr = show;

    sQstComm*   ctx = (sQstComm*)parm;
    CTextOper*  opr = (CTextOper*)ctx->oper;

    /* 转换成16进制数显示 */
    for (uint_t idx = 0; idx < size; idx++) {
        sprintf(ptr, "%02X ", *(byte_t*)data);
        data = (byte_t*)data + 1;
        ptr += 3;
    }
    *ptr = NIL;
    opr->text(show);
    mem_free(show);
}

/* ANSI 转义处理上下文 */
static ansi_t   s_attr[64];
static uint_t   s_at_idx = 0;
static iDATOT*  s_html = NULL;

/*
=======================================
    创建 ANSI 上下文
=======================================
*/
CR_API bool_t
qst_csi_init (void_t)
{
    s_at_idx = 0;
    s_html = create_buff_out(512);
    if (s_html == NULL)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    释放 ANSI 上下文
=======================================
*/
CR_API void_t
qst_csi_free (void_t)
{
    CR_VCALL(s_html)->release(s_html);
}

/*
=======================================
    清除 ANSI 上下文
=======================================
*/
CR_API void_t
qst_csi_clear (void_t)
{
    s_at_idx = 0;
    CR_VCALL(s_html)->reput(s_html, 0);
}

/*
=======================================
    ANSI 转义文本显示
=======================================
*/
CR_API void_t
qst_csi_show (
  __CR_IN__ void_t*         parm,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
}
