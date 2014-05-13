
#include "QstCommInt.h"

/*
=======================================
    文本数据显示
=======================================
*/
CR_API void_t
qst_txt_show (
  __CR_IN__ void_t* parm,
  __CR_IN__ ansi_t  cha
    )
{
    ansi_t      show[2];
    sQstComm*   ctx = (sQstComm*)parm;
    CTextOper*  opr = (CTextOper*)ctx->oper;

    /* 过滤无法显示的字符 */
    if (cha != CR_AC('\n') &&
        cha != CR_AC('\r') && !is_printA(cha))
        show[0] = CR_AC(' ');
    else
        show[0] = cha;
    show[1] = NIL;
    opr->text(show);
}

/*
=======================================
    16进制数据显示
=======================================
*/
CR_API void_t
qst_hex_show (
  __CR_IN__ void_t* parm,
  __CR_IN__ ansi_t  cha
    )
{
    ansi_t      show[4];
    sQstComm*   ctx = (sQstComm*)parm;
    CTextOper*  opr = (CTextOper*)ctx->oper;

    /* 转换成16进制数显示 */
    sprintf(show, "%02X ", cha);
    opr->text(show);
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
  __CR_IN__ void_t* parm,
  __CR_IN__ ansi_t  cha
    )
{
}
