
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
static bool_t   s_have;
static bool_t   s_buffer;
static uint_t   s_state;
static uint_t   s_at_idx;
static bool_t   s_type[6];  /* 高亮 + 下划线 + 上划线 + 中划线 + 反色 + 闪烁 */
static sint_t   s_color[2];     /* 前景色 + 背景色 */
static ansi_t   s_attr[33];
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
    s_have = s_buffer = FALSE;
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
    s_have = s_buffer = FALSE;
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
    渲染 ANSI 字符串
---------------------------------------
*/
static void_t
qst_csi_render (
  __CR_IN__ const sQstComm* parm,
  __CR_IN__ const ansi_t*   tail
    )
{
    ansi_t*         str;
    ansi_t*         send;
    ansi_t*         span;
    const ansi_t*   fg = "color:";
    const ansi_t*   bk = "background-color:";
    const ansi_t*   fc = parm->cfgs.fg;
    const ansi_t*   bc = parm->cfgs.bg;
    const ansi_t*   tx = "text-decoration:none;";

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

    /* 下划线|划线风格 */
    if (s_type[1])
        tx = "text-decoration:underline;";
    else
    if (s_type[2])
        tx = "text-decoration:overline;";
    else
    if (s_type[3])
        tx = "text-decoration:line-through;";

    /* 反色风格 */
    if (s_type[4]) {
        fg = "background-color:";
        bk = "color:";
    }

    /* 闪烁风格 - 未实现 */
    /*********************/
    span = str_fmtA("<span style=\"%s%s%s%s%s\">", tx, fg, fc, bk, bc);
    if (span != NULL) {
        if (tail != NULL) {
            CR_VCALL(s_html)->write(s_html, "</span>", 7);
            CR_VCALL(s_html)->write(s_html, tail, str_sizeA(tail));
        }
        else {
            CR_VCALL(s_html)->write(s_html, "</span>", 8);
        }
        str = (ansi_t*)(CR_VCALL(s_html)->flush(s_html));
        send = str_fmtA("%s%s", span, str);
        if (send != NULL) {
            ((CTextOper*)(parm->oper))->html(send);
            mem_free(send);
        }
        CR_VCALL(s_html)->reput(s_html, 0);
        s_have = FALSE;
        mem_free(span);
    }
}

/*
---------------------------------------
    处理 ANSI 转义属性
---------------------------------------
*/
static void_t
qst_csi_attrib (
  __CR_IN__ const sQstComm* parm
    )
{
    leng_t      idx;
    uint_t      at[8];
    leng_t      count;
    ansi_t**    attrs;

    /* 发送上次结果 */
    if (s_buffer && s_have)
        qst_csi_render(parm, NULL);

    /* 解析转义属性 */
    attrs = str_splitA(s_attr, ';', &count);
    if (attrs == NULL)
        return;
    if (count > cntsof(at))
        count = cntsof(at);
    for (idx = 0; idx < count; idx++) {
        at[idx] = str2intA(attrs[idx]);
        if (at[idx] == 0) {
            s_have = s_buffer = FALSE;
            s_color[0] = s_color[1] = -1;
            mem_zero(s_type, sizeof(s_type));
            mem_free(attrs);
            return;
        }
    }
    mem_free(attrs);
    s_buffer = TRUE;
    for (idx = 0; idx < count; idx++) {
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

            case 53:    /* 上划线 */
                s_type[2] = TRUE;
                break;
            case 55:
                s_type[2] = FALSE;
                break;

            case 9:     /* 中划线 */
                s_type[3] = TRUE;
                break;
            case 29:
                s_type[3] = FALSE;
                break;

            case 7:     /* 反色 */
                s_type[4] = TRUE;
                break;
            case 27:
                s_type[4] = FALSE;
                break;

            case 5:     /* 闪烁 */
                s_type[5] = TRUE;
                break;
            case 25:
                s_type[5] = FALSE;
                break;
        }
    }
}

/*
---------------------------------------
    输出 HTML 转义字符
---------------------------------------
*/
static void_t
qst_csi_output (
  __CR_IN__ const sQstComm* parm,
  __CR_IN__ ansi_t          cha
    )
{
    ansi_t  show[2];

    if (cha == CR_AC('\"')) {
        if (s_buffer) {
            s_have = TRUE;
            CR_VCALL(s_html)->write(s_html, "&quot;", 6);
        }
        else {
            ((CTextOper*)(parm->oper))->html("&quot;");
        }
    }
    else
    if (cha == CR_AC('&')) {
        if (s_buffer) {
            s_have = TRUE;
            CR_VCALL(s_html)->write(s_html, "&amp;", 5);
        }
        else {
            ((CTextOper*)(parm->oper))->html("&amp;");
        }
    }
    else
    if (cha == CR_AC('\'')) {
        if (s_buffer) {
            s_have = TRUE;
            CR_VCALL(s_html)->write(s_html, "&apos;", 6);
        }
        else {
            ((CTextOper*)(parm->oper))->html("&apos;");
        }
    }
    else
    if (cha == CR_AC('<')) {
        if (s_buffer) {
            s_have = TRUE;
            CR_VCALL(s_html)->write(s_html, "&lt;", 4);
        }
        else {
            ((CTextOper*)(parm->oper))->html("&lt;");
        }
    }
    else
    if (cha == CR_AC('>')) {
        if (s_buffer) {
            s_have = TRUE;
            CR_VCALL(s_html)->write(s_html, "&gt;", 4);
        }
        else {
            ((CTextOper*)(parm->oper))->html("&gt;");
        }
    }
    else
    if (cha == CR_AC(' ')) {
        if (s_buffer) {
            s_have = TRUE;
            CR_VCALL(s_html)->write(s_html, "&nbsp;", 6);
        }
        else {
            ((CTextOper*)(parm->oper))->html("&nbsp;");
        }
    }
    else
    if (cha == CR_AC('\r') || cha == CR_AC('\n')) {
        if (s_buffer) {
            qst_csi_render(parm, "<br>");
        }
        else {
            ((CTextOper*)(parm->oper))->html("<br>");
        }
    }
    else {
        if (s_buffer) {
            s_have = TRUE;
            CR_VCALL(s_html)->putb_no(s_html, cha);
        }
        else {
            show[0] = cha;
            show[1] = NIL;
            ((CTextOper*)(parm->oper))->html(show);
        }
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
    ansi_t      show[2];
    sQstComm*   ctx = (sQstComm*)parm;

    switch (s_state)
    {
        case 0:     /* 查找 ESC 阶段 */
            if (cha != 0x1B) {
                if (cha != CR_AC('\n') &&
                    cha != CR_AC('\r') && !is_printA(cha))
                    show[0] = CR_AC(' ');
                else
                    show[0] = cha;
                qst_csi_output(ctx, show[0]);
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
                qst_csi_output(ctx, show[0]);
                qst_csi_output(ctx, show[1]);
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
                qst_csi_attrib(ctx);
                s_state = 0;
            }
            else {
                s_attr[s_at_idx++] = cha;
            }
            break;
    }
}
