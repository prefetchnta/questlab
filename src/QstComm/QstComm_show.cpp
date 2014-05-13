
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
static uint_t   s_state;
static uint_t   s_at_idx;
static bool_t   s_buffer;
static ansi_t   s_attr[33];
static iDATOT*  s_html;

/*
=======================================
    创建 ANSI 上下文
=======================================
*/
CR_API bool_t
qst_csi_init (void_t)
{
    s_state = 0;
    s_buffer = FALSE;
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
    s_state = 0;
    s_buffer = FALSE;
    CR_VCALL(s_html)->reput(s_html, 0);
}

/*
---------------------------------------
    处理 ANSI 转义属性
---------------------------------------
*/
static void_t
qst_csi_render (
  __CR_IN__ CTextOper*  oper
    )
{
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
    ansi_t      show[4];
    sQstComm*   ctx = (sQstComm*)parm;
    CTextOper*  opr = (CTextOper*)ctx->oper;

    switch (s_state)
    {
        case 0:     /* 查找 ESC 阶段 */
            if (cha != 0x1B) {
                if (cha != CR_AC('\n') &&
                    cha != CR_AC('\r') && !is_printA(cha))
                    show[0] = CR_AC(' ');
                else
                    show[0] = cha;
                show[1] = NIL;
                if (s_buffer)
                    CR_VCALL(s_html)->putb_no(s_html, show[0]);
                else
                    opr->text(show);
            }
            else {
                s_state += 1;
            }
            break;

        case 1:     /* 查找 '[' 阶段 */
            if (cha != CR_AC('[')) {
                show[0] = CR_AC(' ');
                if (cha != CR_AC('\n') &&
                    cha != CR_AC('\r') && !is_printA(cha))
                    show[1] = CR_AC(' ');
                else
                    show[1] = cha;
                show[2] = NIL;
                if (s_buffer)
                    CR_VCALL(s_html)->write(s_html, show, 2);
                else
                    opr->text(show);
                s_state = 0;
            }
            else {
                s_state += 1;
                s_at_idx = 0;
            }
            break;

        default:
     /* case 2: */  /* 查找 'm' 阶段 */
            if (cha == CR_AC('m') ||
                s_at_idx >= sizeof(s_attr) - 1) {
                s_attr[s_at_idx] = NIL;
                qst_csi_render(opr);
                s_state = 0;
            }
            else {
                s_attr[s_at_idx++] = cha;
            }
            break;
    }
}
