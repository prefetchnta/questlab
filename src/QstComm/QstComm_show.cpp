
#include "QstCommInt.h"

/*****************************************************************************/
/*                                 普通显示                                  */
/*****************************************************************************/

/*
---------------------------------------
    文本数据显示 (内部)
---------------------------------------
*/
static void_t
qst_txt_show_int (
  __CR_IN__ void_t*         parm,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size,
  __CR_IN__ bool_t          html
    )
{
    ansi_t      *txt, *str;
    sQstComm    *ctx = (sQstComm*)parm;
    CTextOper   *opr = (CTextOper*)ctx->oper;

    /* 复制成字符串 */
    txt = str_allocA(size + 1);
    if (txt == NULL)
        return;
    mem_cpy(txt, data, size);
    txt[size] = NIL;

    /* 转换编码 */
    if (ctx->page != CR_UTF8) {
        str = local_to_utf8(ctx->page, txt);
        mem_free(txt);
        if (str == NULL)
            return;
        txt = str;
    }

    /* 显示结果 */
    if (html)
        opr->html(txt);
    else
        opr->text(txt);
    mem_free(txt);
}

/*
=======================================
    文本数据显示 (TEXT)
=======================================
*/
CR_API void_t
qst_txt_show (
  __CR_IN__ void_t*         parm,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    qst_txt_show_int(parm, data, size, FALSE);
}

/*
=======================================
    文本数据显示 (HTML)
=======================================
*/
CR_API void_t
qst_htm_show (
  __CR_IN__ void_t*         parm,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    qst_txt_show_int(parm, data, size, TRUE);
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
    ansi_t      *txt;
    sQstComm    *ctx = (sQstComm*)parm;
    CTextOper   *opr = (CTextOper*)ctx->oper;

    /* 转换成16进制数显示 */
    txt = str_allocA(size * 3 + 1);
    if (txt == NULL)
        return;
    for (uint_t idx = 0; idx < size; idx++)
        sprintf(&txt[idx * 3], "%02X ", ((byte_t*)data)[idx]);

    /* 显示结果 */
    opr->text(txt);
    mem_free(txt);
}

/*****************************************************************************/
/*                                 色彩显示                                  */
/*****************************************************************************/

/* ANSI 转义处理上下文 */
static iDATOT*  s_html;     /* HTML 输出缓存对象 */
static bool_t   s_prev;     /* 之前的解析是否为控制符 */
static bool_t   s_have;     /* 之前的 <SPAN> 标签是否封闭 */
static uint_t   s_state;    /* 查找转义的状态机 */
static bool_t   s_type[6];  /* 高亮 + 下划线 + 上划线 + 中划线 + 反色 + 闪烁 */
static sint_t   s_color[2];     /* 前景色 + 背景色 */
static ansi_t   s_attr[33];     /* 用于保存属性字符串 */
static uint_t   s_at_idx;       /* 保存属性字符串的索引 */

/*
=======================================
    清除 ANSI 上下文
=======================================
*/
CR_API void_t
qst_csi_clear (void_t)
{
    s_state = s_at_idx = 0;
    s_have = s_prev = FALSE;
    s_color[0] = s_color[1] = -1;
    mem_zero(s_type, sizeof(s_type));
}

/*
=======================================
    创建 ANSI 上下文
=======================================
*/
CR_API bool_t
qst_csi_init (void_t)
{
    qst_csi_clear();
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
  __CR_IN__ const sQstComm* ctx
    )
{
    ansi_t*         span;
    const ansi_t*   fg = "color:";
    const ansi_t*   bk = "background-color:";
    const ansi_t*   fc = ctx->cfgs.fg;
    const ansi_t*   bc = ctx->cfgs.bg;
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

    /* 之前的标签未封闭, 封闭之 */
    if (s_have) {
        s_have = FALSE;
        CR_VCALL(s_html)->write(s_html, "</span>", 7);
    }

    /* 输出新的属性标签 */
    span = str_fmtA("<span style=\"%s%s%s%s%s\">", tx, fg, fc, bk, bc);
    if (span != NULL) {
        s_have = TRUE;
        CR_VCALL(s_html)->putsA(s_html, span, 0);
        mem_free(span);
    }
}

/*
---------------------------------------
    处理 ANSI 转义属性
---------------------------------------
*/
static bool_t
qst_csi_attrib (void_t)
{
    leng_t      idx;
    uint_t      at[8];
    leng_t      count;
    ansi_t**    attrs;

    /* 解析转义属性 */
    attrs = str_splitA(s_attr, ';', &count);
    if (attrs == NULL)
        return (FALSE);
    if (count > cntsof(at))
        count = cntsof(at);
    for (idx = 0; idx < count; idx++)
        at[idx] = str2intA(attrs[idx]);
    mem_free(attrs);
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

            case 0:     /* 复位 */
                qst_csi_clear();
                CR_VCALL(s_html)->write(s_html, "</span>", 7);
                return (TRUE);

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
    return (TRUE);
}

/*
---------------------------------------
    输出 HTML 转义字符
---------------------------------------
*/
static void_t
qst_csi_output (
  __CR_IN__ ansi_t  cha
    )
{
    switch (cha)
    {
        /* Qt 里 &nbsp; 在有些字体下为两倍字符宽度 */
        default: CR_VCALL(s_html)->putb_no(s_html, cha); break;
        case '<': CR_VCALL(s_html)->write(s_html, "&lt;", 4); break;
        case '>': CR_VCALL(s_html)->write(s_html, "&gt;", 4); break;
        case '&': CR_VCALL(s_html)->write(s_html, "&amp;", 5); break;
        case ' ': CR_VCALL(s_html)->write(s_html, "&nbsp;", 6); break;
        case '\"': CR_VCALL(s_html)->write(s_html, "&quot;", 6); break;
        case '\'': CR_VCALL(s_html)->write(s_html, "&apos;", 6); break;
        case '\r': /* <SPAN></SPAN> 里可以嵌 <BR> 换行标签 */
        case '\n': CR_VCALL(s_html)->write(s_html, "<br>", 4); break;
    }
}

/*
---------------------------------------
    ANSI 转义文本显示
---------------------------------------
*/
static bool_t
qst_csi_parse (
  __CR_IN__ const sQstComm* ctx,
  __CR_IN__ ansi_t          cha
    )
{
    switch (s_state)
    {
        case 0:     /* 查找 ESC 阶段 */
            if (cha != 0x1B) {
                if (s_prev) {
                    s_prev = FALSE;
                    qst_csi_render(ctx);
                }
                qst_csi_output(cha);
            }
            else {
                s_state += 1;
            }
            break;

        case 1:     /* 查找 '[' 阶段 */
            if (cha != '[') {
                s_state = 0;
                return (FALSE);
            }
            s_state += 1;
            s_at_idx = 0;
            break;

        default:    /* 查找 'm' 阶段 */
            if (s_at_idx >= sizeof(s_attr)) {
                s_state = 0;
                return (FALSE);
            }
            if (cha == 'm') {   /* 格式 */
                s_state = 0;
                s_attr[s_at_idx] = NIL;
                if (!qst_csi_attrib())
                    return (FALSE);
                s_prev = TRUE;
            }
            else
            if (cha == 'J') {   /* 清屏 */
                qst_csi_clear();
                CR_VCALL(s_html)->reput(s_html, 0);
                ((CTextOper*)(ctx->oper))->clear();
            }
            else
            if (cha == 'G') {   /* 响铃 */
                s_state = 0;
                MessageBeep(MB_OK);
            }
            else {
                switch (cha)
                {
                    default:    /* 复制参数 */
                        if (!is_digitA(cha) &&
                            cha != ';' && cha != '?') {
                            s_state = 0;
                            return (FALSE);
                        }
                        s_attr[s_at_idx++] = cha;
                        break;

                    case 'A':   /* 光标上移n个位置 */
                    case 'B':   /* 光标下移n个位置 */
                    case 'C':   /* 光标左移n个位置 */
                    case 'D':   /* 光标右移n个位置 */
                    case 's':   /* 保存光标位置 */
                    case 'u':   /* 恢复光标位置 */
                    case 'H':   /* 光标移动到绝对坐标(m,n)处 */
                    case 'K':   /* 删除从光标处开始到行末的所以字符 */
                    case 'M':   /* 延迟n毫秒 */
                    case 'l':   /* 隐藏光标 */
                    case 'h':   /* 显示光标 */
                        s_state = 0;
                        break;
                }
            }
            break;
    }
    return (TRUE);
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
    ansi_t      *txt, *str;
    sQstComm    *ctx = (sQstComm*)parm;
    CTextOper   *opr = (CTextOper*)ctx->oper;

    /* 复制成字符串 */
    txt = str_allocA(size + 1);
    if (txt == NULL)
        return;
    mem_cpy(txt, data, size);
    txt[size] = NIL;

    /* 转换编码 */
    if (ctx->page != CR_UTF8) {
        str = local_to_utf8(ctx->page, txt);
        mem_free(txt);
        if (str == NULL)
            return;
        txt = str;
        size = str_lenA(txt);
    }

    /* Qt 每次输出 HTML 效果是不连续的 */
    CR_VCALL(s_html)->reput(s_html, 0);
    if (s_have) {
        s_have = FALSE;
        qst_csi_render(ctx);
    }

    /* 逐字符解析 */
    for (uint_t idx = 0; idx < size; idx++) {
        if (!qst_csi_parse(ctx, txt[idx])) {
            qst_csi_clear();
            mem_free(txt);
            return;
        }
    }
    mem_free(txt);

    /* 显示结果 */
    CR_VCALL(s_html)->putb_no(s_html, NIL);
    txt = (ansi_t*)(CR_VCALL(s_html)->flush(s_html));
    opr->html(txt);
}
