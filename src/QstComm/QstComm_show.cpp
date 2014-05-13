
#include "QstCommInt.h"

/*****************************************************************************/
/*                                 普通显示                                  */
/*****************************************************************************/

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

/*****************************************************************************/
/*                                 色彩显示                                  */
/*****************************************************************************/

/* ANSI 转义处理上下文 */
static uint_t   s_state;
static uint_t   s_at_idx;
static bool_t   s_buffer;
static bool_t   s_type[4];      /* 高亮 + 下划线 + 闪烁 + 反色 */
static sint_t   s_color[2];     /* 前景色 + 背景色 */
static ansi_t   s_attr[33];
static ansi_t*  s_span = NULL;
static iDATOT*  s_html = NULL;

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
    s_span = NULL;
    s_color[0] = s_color[1] = -1;
    mem_zero(s_type, sizeof(s_type));
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
    TRY_FREE(s_span);
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
    SAFE_FREE(s_span);
    s_color[0] = s_color[1] = -1;
    mem_zero(s_type, sizeof(s_type));
    CR_VCALL(s_html)->reput(s_html, 0);
}

/* 颜色常数表 */
static const ansi_t*    s_lo_color[] =
{
    "#000000;", "#800000;", "#008000;", "#808000;",
    "#000080;", "#800080;", "#008080;", "#C0C0C0;",
};
static const ansi_t*    s_hi_color[] =
{
    "#808080;", "#FF0000;", "#00FF00;", "#FFFF00;",
    "#0000FF;", "#FF00FF;", "#00FFFF;", "#FFFFFF;",
};

/*
---------------------------------------
    处理 ANSI 转义属性
---------------------------------------
*/
static void_t
qst_csi_render (
  __CR_IN__ CTextOper*  opr
    )
{
    uint_t      at[3];
    leng_t      count;
    ansi_t**    attrs;

    if (s_buffer)
    {
        ansi_t* str;
        ansi_t* send;

        /* 发送之前的 HTML 串 */
        CR_VCALL(s_html)->write(s_html, "</span>", 8);
        str = (ansi_t*)(CR_VCALL(s_html)->flush(s_html));
        send = str_fmtA("%s%s", s_span, str);
        if (send != NULL) {
            opr->html(send);
            mem_free(send);
        }
        CR_VCALL(s_html)->reput(s_html, 0);
        SAFE_FREE(s_span);
    }

    /* 解析转义属性 */
    attrs = str_splitA(s_attr, ';', &count);
    if (attrs == NULL)
        return;
    if (count > cntsof(at))
        count = cntsof(at);
    for (leng_t idx = 0; idx < count; idx++) {
        at[idx] = str2intA(attrs[idx]);
        if (at[idx] == 0) {
            s_color[0] = s_color[1] = -1;
            mem_zero(s_type, sizeof(s_type));
            mem_free(attrs);
            s_buffer = FALSE;
            return;
        }
    }
    mem_free(attrs);
    s_buffer = TRUE;

    const ansi_t*   tx = "";
    const ansi_t*   fg = "";
    const ansi_t*   bk = "";
    const ansi_t*   fc = "";
    const ansi_t*   bc = "";

    /* 加入相应的 HTML 标签 */
    for (leng_t idx = 0; idx < count; idx++) {
        switch (at[idx])
        {
            default:    /* 颜色 */
                if (at[idx] >= 30 && at[idx] <= 37)
                    s_color[0] = at[idx] - 30;
                else
                if (at[idx] >= 40 && at[idx] <= 47)
                    s_color[1] = at[idx] - 40;
                break;

            case 1:     /* 高亮 */
                s_type[0] = TRUE;
                break;

            case 4:     /* 下划线 */
                s_type[1] = TRUE;
                break;
            case 24:
                s_type[1] = FALSE;
                break;

            case 5:     /* 闪烁 */
                s_type[2] = TRUE;
                break;
            case 25:
                s_type[2] = FALSE;
                break;

            case 7:     /* 反色 */
                s_type[3] = TRUE;
                break;
            case 27:
                s_type[3] = FALSE;
                break;
        }
    }

    /* 颜色风格 */
    if (s_color[0] > 0) {
        if (s_type[0])
            fc = s_hi_color[s_color[0]];
        else
            fc = s_lo_color[s_color[0]];
    }
    if (s_color[1] > 0) {
        if (s_type[0])
            bc = s_hi_color[s_color[1]];
        else
            bc = s_lo_color[s_color[1]];
    }

    /* 下划线风格 */
    if (s_type[1])
        tx = "text-decoration:underline;";

    /* 反色风格 */
    if (s_type[3]) {
        if (s_color[0] > 0)
            fg = "background-color:";
        if (s_color[1] > 0)
            bk = "color:";
    }
    else {
        if (s_color[0] > 0)
            fg = "color:";
        if (s_color[1] > 0)
            bk = "background-color:";
    }
    s_span = str_fmtA("<span style=\"%s%s%s%s%s\">", tx, fg, fc, bk, bc);
}

/*
---------------------------------------
    输出 HTML 转义字符
---------------------------------------
*/
static void_t
qst_csi_output (
  __CR_IN__ CTextOper*  opr,
  __CR_IN__ ansi_t      cha
    )
{
    ansi_t* str;
    ansi_t* send;

    if (cha == CR_AC('\"')) {
        CR_VCALL(s_html)->write(s_html, "&quot;", 6);
    }
    else
    if (cha == CR_AC('&')) {
        CR_VCALL(s_html)->write(s_html, "&amp;", 5);
    }
    else
    if (cha == CR_AC('\'')) {
        CR_VCALL(s_html)->write(s_html, "&apos;", 6);
    }
    else
    if (cha == CR_AC('<')) {
        CR_VCALL(s_html)->write(s_html, "&lt;", 4);
    }
    else
    if (cha == CR_AC('>')) {
        CR_VCALL(s_html)->write(s_html, "&gt;", 4);
    }
    else
    if (cha == CR_AC('\r') || cha == CR_AC('\n')) {
        CR_VCALL(s_html)->write(s_html, "</span><br>", 12);
        str = (ansi_t*)(CR_VCALL(s_html)->flush(s_html));
        send = str_fmtA("%s%s", s_span, str);
        if (send != NULL) {
            opr->html(send);
            mem_free(send);
        }
        CR_VCALL(s_html)->reput(s_html, 0);
    }
    else {
        CR_VCALL(s_html)->putb_no(s_html, cha);
    }
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
                    qst_csi_output(opr, show[0]);
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
                if (s_buffer) {
                    qst_csi_output(opr, show[0]);
                    qst_csi_output(opr, show[1]);
                }
                else {
                    opr->text(show);
                }
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
