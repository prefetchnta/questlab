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
 *             ##       CREATE: 2015-03-23
 *              #
 ================================================
        点阵字库生成小工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

#include "gfx2/gdiwin.h"
#pragma comment (lib, "GFX2_GDI.lib")

static uint_t   s_align;    /* 对齐参数 */

/* 字体生成信息结构 */
typedef struct
{
        iFONT*  font_out;   /* 文字接口 */
        int32u  sch, ech;   /* 起止字符 */
        wchar*  font_name;  /* 字体名称 */
        uint_t  font_bold;  /* 字体粗细 */
        uint_t  char_type;  /* 字符类型 */

} sFONT_INFO;

/*
---------------------------------------
    释放字体生成信息结构
---------------------------------------
*/
static void_t font_info_free (void_t *obj)
{
    sFONT_INFO* fnt = (sFONT_INFO*)obj;

    if (fnt->font_out != NULL)
        CR_VCALL(fnt->font_out)->release(fnt->font_out);
    mem_free(fnt->font_name);
}

/*
************************************************
**  attrb(0)    =0: 一般        =1: 抗锯齿
**  attrb(1)    =0: 高位在前    =1: 低位在前
**  attrb(2)    =0: 正常        =1: XY轴翻转
**  attrb(3)    =0: 正常        =1: 水平翻转
**  attrb(4)    =0: 正常        =1: 垂直翻转
**  attrb(5+)   点阵像素位数值 (支持1/2/4/8)
************************************************
*/
#define FNT_PT_ANTI             0x01
#define FNT_LB_FRST             0x02
#define FNT_XY_FLIP             0x04
#define FNT_XH_FLIP             0x08
#define FNT_YV_FLIP             0x10
#define FNT_TYPE(val)   (((val) >> 5) & 0x0F)

/*
---------------------------------------
    获取输出字模数据
---------------------------------------
*/
static bool_t get_char_mask (file_t file, iGFX2 *main, iFONT *font,
                const void_t *text, uint_t attrb, uint_t cpage, uint_t fnt_w,
                            uint_t fnt_h, uint_t *max_w, uint_t *max_h)
{
    leng_t size;
    byte_t data;
    sIMAGE *image;
    sRECT rect, draw;
    sint_t xinc, yinc;
    int32u *dest, *dptr, *sptr;
    uint_t sx, sy, dx, dy, ww, hh;

    /* 先测量字符的宽高 */
    rect_set_wh(&rect, 0, 0, fnt_w, fnt_h);
    CR_VCALL(font)->calc_rect(font, text, &rect, cpage);
    if (rect.ww > fnt_w || rect.hh > fnt_h) {
        printf("invalid font api_wh (%u x %u) > (%u x %u)\n",
                rect.ww, rect.hh, fnt_w, fnt_h);
        return (FALSE);
    }

    /* 获取最大真实宽高 */
    if (*max_w < rect.ww) *max_w = rect.ww;
    if (*max_h < rect.hh) *max_h = rect.hh;

    /* 用黑色清空目标缓冲并绘制字符 */
    CR_VCALL(main)->clear(main, 0, 0);
    sx = (fnt_w - rect.ww) / 2;
    sy = (fnt_h - rect.hh) / 2;
    rect_set_wh(&draw, sx, sy, rect.ww, rect.hh);
    CR_VCALL(font)->draw_tran(font, text, &draw, cpage);

    /* 宽对齐到整字节 (因为 ASCII 要除以2的) */
    if (!(attrb & FNT_XY_FLIP))
        fnt_w += (uint_t)CR_PADDED(fnt_w, s_align);

    /* 分配矫正过的位图内存 */
    size  = fnt_w;
    size *= fnt_h;
    size *= sizeof(int32u);
    dest = dptr = (int32u*)mem_malloc(size);
    if (dest == NULL) {
        printf("out of memory\n");
        return (FALSE);
    }

    /* XY轴翻转 */
    if (attrb & FNT_XY_FLIP) {
        ww = fnt_h;
        hh = fnt_w;
    }
    else {
        ww = fnt_w;
        hh = fnt_h;
    }

    /* 水平翻转 */
    if (attrb & FNT_XH_FLIP) {
        dx = 0;
        sx = ww - 1;
        xinc = -1;
    }
    else {
        sx = 0;
        dx = ww - 1;
        xinc = 1;
    }

    /* 垂直翻转 */
    if (attrb & FNT_YV_FLIP) {
        dy = 0;
        sy = hh - 1;
        yinc = -1;
    }
    else {
        sy = 0;
        dy = hh - 1;
        yinc = 1;
    }

    /* 读取位图数据 */
    image = CR_VCALL(main)->lock(main);
    if (attrb & FNT_XY_FLIP) {
        for (uint_t y = sy; y != dy + yinc; y += yinc)
        for (uint_t x = sx; x != dx + xinc; x += xinc)
        {
            if (!image->gdi)
                sptr = pixel_addr4(image, y, x);
            else
                sptr = pixel_addr4(image, y, fnt_h - x - 1);
            *dptr++ = *sptr & 0x00FFFFFF;
        }
    }
    else {
        for (uint_t y = sy; y != dy + yinc; y += yinc)
        for (uint_t x = sx; x != dx + xinc; x += xinc)
        {
            if (!image->gdi)
                sptr = pixel_addr4(image, x, y);
            else
                sptr = pixel_addr4(image, x, fnt_h - y - 1);
            *dptr++ = *sptr & 0x00FFFFFF;
        }
    }
    dptr = dest;

    /* 根据类型输出点阵数据 */
    switch (FNT_TYPE(attrb))
    {
        case 0:
        case 1:
            /* 低位在前 */
            if (attrb & FNT_LB_FRST) {
                for (; size != 0; size -= 4 * 8) {
                    data = 0x00;
                    for (uint_t k = 8; k != 0; k--) {
                        data >>= 1;
                        if (*dptr++ != 0)
                            data |= 0x80;
                    }
                    /* 写入文件 */
                    if (!file_putb(data, file)) {
                        printf("can't write dest file\n");
                        goto _failure;
                    }
                }
            }
            else {
                for (; size != 0; size -= 4 * 8) {
                    data = 0x00;
                    for (uint_t k = 8; k != 0; k--) {
                        data <<= 1;
                        if (*dptr++ != 0)
                            data |= 0x01;
                    }
                    /* 写入文件 */
                    if (!file_putb(data, file)) {
                        printf("can't write dest file\n");
                        goto _failure;
                    }
                }
            }
            break;

        default:
            printf("font attrb <%u> not supported yet\n", FNT_TYPE(attrb));
            goto _failure;
    }
    CR_VCALL(main)->unlock(main);
    mem_free(dest);
    return (TRUE);

_failure:
    CR_VCALL(main)->unlock(main);
    mem_free(dest);
    return (FALSE);
}

/*
---------------------------------------
    设置并返回指定的字体对象
---------------------------------------
*/
static iFONT* get_char_font (iGFX2 *main, const sARRAY *list, leng_t index)
{
    iFONT *font;
    sFONT_INFO *unit;

    /* 绑定字体对象 */
    unit = array_get_unitT(list, sFONT_INFO, index);
    font = unit->font_out;
    CR_VCALL(font)->bind(font, main);

    /* 设置字体颜色 (黑底白字便于获取亮度) */
    CR_VCALL(font)->setColor(font, 0xFFFFFFFF);
    return (font);
}

/*
---------------------------------------
    生成点阵字库
---------------------------------------
*/
static sint_t make_font (const sARRAY *list, uint_t attrb, uint_t chset,
        uint_t cpage, uint_t api_w, uint_t api_h, uint_t fnt_w, uint_t fnt_h)
{
    iGFX2 *main;
    leng_t tag_count;
    file_t fhzk, fasc;
    uint_t max_w, max_h;

    /* 没有 RANGE 标签 */
    tag_count = array_get_sizeT(list, sFONT_INFO);
    if (tag_count == 0) {
        printf("no font to create\n");
        return (-1);
    }

    /* 先生成文件 */
    fhzk = file_openA("HZK", CR_FO_WO);
    if (fhzk == NULL) {
        printf("can't create file HZK\n");
        return (-1);
    }
    fasc = file_openA("ASC", CR_FO_WO);
    if (fasc == NULL) {
        printf("can't create file ASC\n");
        file_close(fhzk);
        file_deleteA("HZK");
        return (-1);
    }

    /* 生成一组文字绘制引擎 */
    for (leng_t idx = 0; idx < tag_count; idx++)
    {
        LOGFONTW info;
        sFONT_INFO *unit;

        unit = array_get_unitT(list, sFONT_INFO, idx);
        info.lfHeight = api_h;
        info.lfWidth  = api_w;
        info.lfEscapement  = 0;
        info.lfOrientation = 0;
        info.lfWeight = unit->font_bold;
        info.lfItalic = FALSE;
        info.lfUnderline = FALSE;
        info.lfStrikeOut = FALSE;
        info.lfCharSet = (BYTE)chset;
        info.lfOutPrecision  = OUT_DEFAULT_PRECIS;
        info.lfClipPrecision = CLIP_DEFAULT_PRECIS;
        if (attrb & FNT_PT_ANTI)
            info.lfQuality = ANTIALIASED_QUALITY;
        else
            info.lfQuality = DEFAULT_QUALITY;
        info.lfPitchAndFamily = DEFAULT_PITCH;
        str_cpyNW((wide_t*)info.lfFaceName, unit->font_name, LF_FACESIZE);

        /* 生成字体对象 */
        unit->font_out = create_gdi_fontW(&info);
        if (unit->font_out == NULL) {
            printf("can't create gdi font\n");
            goto _failure2;
        }
    }

    /* 生成 GDI 位图表面 */
    main = (iGFX2*)create_gdi_bitmap(fnt_w, fnt_h, CR_ARGB8888);
    if (main == NULL) {
        printf("can't create gdi bitmap\n");
        goto _failure2;
    }

    iFONT *font;
    ansi_t chr[5];

    /* 正式开始生成点阵字库 */
    mem_zero(chr, sizeof(chr));
    printf("please wait...\n");

    /* 绑定第一个字体 */
    font = get_char_font(main, list, 0);
    if (font == NULL)
        goto _failure1;

    /* 生成 ASCII 字库 (0x20 - 0x7E) */
    printf("ASC - size (%u x %u)\n", fnt_w / 2, fnt_h);
    max_w = max_h = 0;
    for (uint_t ch = 0x20; ch <= 0x7E; ch++) {
        chr[0] = (ansi_t)ch;
        if (!get_char_mask(fasc, main, font, chr, attrb, cpage,
                           fnt_w / 2, fnt_h, &max_w, &max_h))
            goto _failure1;
    }
    printf("ASC - true (%u x %u)\n", max_w, max_h);

    /* 只有一个 RANGE 标签 */
    if (tag_count == 1) {
        file_close(fhzk);
        file_deleteA("HZK");
        goto _success;
    }

    /* 生成多字节编码字库 (根据指定范围) */
    printf("HZK - size (%u x %u)\n", fnt_w, fnt_h);
    max_w = max_h = 0;
    for (leng_t idx = 1; idx < tag_count; idx++)
    {
        sFONT_INFO* unit;
        uint_t char_type;

        /* 绑定目标字体 */
        font = get_char_font(main, list, idx);
        if (font == NULL)
            goto _failure1;
        unit = array_get_unitT(list, sFONT_INFO, idx);

        /* 根据编码范围类型枚举字符 */
        char_type = unit->char_type;
        if (char_type == 0)
        {
            uint_t sch[4], ech[4];

            /* 获取每个区段的范围值 */
            sch[0] = (unit->sch >> 24) & 0xFF;
            sch[1] = (unit->sch >> 16) & 0xFF;
            sch[2] = (unit->sch >>  8) & 0xFF;
            sch[3] = (unit->sch >>  0) & 0xFF;
            ech[0] = (unit->ech >> 24) & 0xFF;
            ech[1] = (unit->ech >> 16) & 0xFF;
            ech[2] = (unit->ech >>  8) & 0xFF;
            ech[3] = (unit->ech >>  0) & 0xFF;

            /* 逐区段方式生成点阵字库 */
            if (unit->sch > 0x00FFFFFFUL) {
                for (uint_t b0 = sch[0]; b0 <= ech[0]; b0++) {
                    chr[0] = (ansi_t)b0;
                    for (uint_t b1 = sch[1]; b1 <= ech[1]; b1++) {
                        chr[1] = (ansi_t)b1;
                        for (uint_t b2 = sch[2]; b2 <= ech[2]; b2++) {
                            chr[2] = (ansi_t)b2;
                            for (uint_t b3 = sch[3]; b3 <= ech[3]; b3++) {
                                chr[3] = (ansi_t)b3;
                                if (!get_char_mask(fhzk, main, font, chr,
                                                   attrb, cpage, fnt_w, fnt_h,
                                                   &max_w, &max_h))
                                    goto _failure1;
                            }
                        }
                    }
                }
            }
            else
            if (unit->sch > 0x0000FFFFUL) {
                chr[3] = 0x00;
                for (uint_t b1 = sch[1]; b1 <= ech[1]; b1++) {
                    chr[0] = (ansi_t)b1;
                    for (uint_t b2 = sch[2]; b2 <= ech[2]; b2++) {
                        chr[1] = (ansi_t)b2;
                        for (uint_t b3 = sch[3]; b3 <= ech[3]; b3++) {
                            chr[2] = (ansi_t)b3;
                            if (!get_char_mask(fhzk, main, font, chr,
                                               attrb, cpage, fnt_w, fnt_h,
                                               &max_w, &max_h))
                                goto _failure1;
                        }
                    }
                }
            }
            else
            if (unit->sch > 0x000000FFUL) {
                chr[2] = 0x00;
                for (uint_t b2 = sch[2]; b2 <= ech[2]; b2++) {
                    chr[0] = (ansi_t)b2;
                    for (uint_t b3 = sch[3]; b3 <= ech[3]; b3++) {
                        chr[1] = (ansi_t)b3;
                        if (!get_char_mask(fhzk, main, font, chr,
                                           attrb, cpage, fnt_w, fnt_h,
                                           &max_w, &max_h))
                            goto _failure1;
                    }
                }
            }
            else {
                chr[1] = 0x00;
                for (uint_t b3 = sch[3]; b3 <= ech[3]; b3++) {
                    chr[0] = (ansi_t)b3;
                    if (!get_char_mask(fhzk, main, font, chr,
                                       attrb, cpage, fnt_w, fnt_h,
                                       &max_w, &max_h))
                        goto _failure1;
                }
            }
        }
        else
        {
            int32u sch = unit->sch;
            int32u ech = unit->ech;

            /* 编码递增方式生成点阵字库 */
            for (int32u ch = sch; ch <= ech; ch++) {
                chr[0] = (ansi_t)(ch >>  0);
                chr[1] = (ansi_t)(ch >>  8);
                chr[2] = (ansi_t)(ch >> 16);
                chr[3] = (ansi_t)(ch >> 24);
                if (!get_char_mask(fhzk, main, font, chr,
                                   attrb, cpage, fnt_w, fnt_h,
                                   &max_w, &max_h))
                    goto _failure1;
            }
        }
    }
    printf("HZK - true (%u x %u)\n", max_w, max_h);
    file_close(fhzk);
_success:
    file_close(fasc);
    CR_VCALL(main)->release(main);
    printf("job's done\n");
    return (TRUE);

_failure1:
    CR_VCALL(main)->release(main);
_failure2:
    file_close(fasc);
    file_close(fhzk);
    file_deleteA("ASC");
    file_deleteA("HZK");
    return (-1);
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 <配置文件> */
    if (argc < 2) {
        printf("makefont <configure.xml>\n");
        return (QST_ERROR);
    }

    ansi_t *text;

    /* 读入整个 XML 文件 */
    text = file_load_as_strA(argv[1]);
    if (text == NULL) {
        printf("missing config file <%s>\n", argv[1]);
        return (QST_ERROR);
    }

    sXMLw *xml;

    /* 开始解析 XML 文件 */
    xml = xml_from_textW(text);
    mem_free(text);
    if (xml == NULL) {
        printf("invalid config file <%s>\n", argv[1]);
        return (QST_ERROR);
    }

    /* 第一个标签指定字体大小及属性 */
    if (xml->nodes[0].closed ||
        str_cmpIW(xml->nodes[0].name, CR_WS("FONT")) != 0) {
        printf("invalid config file <%s>\n", argv[1]);
        return (QST_ERROR);
    }

    sARRAY list;
    uint_t attrb, chset, cpage;
    uint_t api_w, api_h, fnt_w, fnt_h;

    /* FW_DONTCARE(0) + DEFAULT_CHARSET(1) */
    attrb = xml_attr_intxW(CR_WS("attrb"),  0, &xml->nodes[0]);
    chset = xml_attr_intxW(CR_WS("chset"),  1, &xml->nodes[0]);
    cpage = xml_attr_intxW(CR_WS("cpage"),  0, &xml->nodes[0]);
    api_w = xml_attr_intxW(CR_WS("api_w"),  0, &xml->nodes[0]);
    api_h = xml_attr_intxW(CR_WS("api_h"),  0, &xml->nodes[0]);
    fnt_w = xml_attr_intxW(CR_WS("fnt_w"), 16, &xml->nodes[0]);
    fnt_h = xml_attr_intxW(CR_WS("fnt_h"), 16, &xml->nodes[0]);
    if (fnt_w < 8 || api_w > fnt_w ||
        fnt_h < 8 || api_h > fnt_h) {
        printf("invalid font size attribute\n");
        return (QST_ERROR);
    }

    /* 点阵宽必须对齐到整字节 */
    switch (FNT_TYPE(attrb))
    {
        case 0:
        case 1: s_align = 8; break;
        case 2: s_align = 4; break;
        case 4:
        case 8: s_align = 2; break; /* ASCII 要除以二取整 */

        default:
            printf("font attrb <%u> not supported yet\n", FNT_TYPE(attrb));
            return (QST_ERROR);
    }
    if (attrb & FNT_XY_FLIP)
        fnt_h += (uint_t)CR_PADDED(fnt_h, s_align);
    else
        fnt_w += (uint_t)CR_PADDED(fnt_w, s_align);

    /* 初始化属性列表 */
    array_initT(&list, sFONT_INFO);
    list.free = font_info_free;

    /* 逐标签加入列表 */
    for (leng_t idx = 1; idx < xml->count; idx++)
    {
        sFONT_INFO temp;

        /* 注意: 第一个标签必定是 ASCII 编码范围 */
        if (str_cmpIW(xml->nodes[idx].name, CR_WS("/FONT")) == 0)
            break;
        if (!xml->nodes[idx].closed ||
            str_cmpIW(xml->nodes[idx].name, CR_WS("RANGE")) != 0)
            continue;
        temp.font_name = xml_attr_stringW(CR_WS("name"), &xml->nodes[idx]);
        if (temp.font_name == NULL ||
            str_lenW(temp.font_name) >= LF_FACESIZE) {
            printf("invalid range name attribute\n");
            return (QST_ERROR);
        }
        temp.sch = xml_attr_intx32W(CR_WS("schar"), 0, &xml->nodes[idx]);
        temp.ech = xml_attr_intx32W(CR_WS("echar"), 0, &xml->nodes[idx]);
        temp.char_type = xml_attr_intxW(CR_WS("type"), 0, &xml->nodes[idx]);
        if (temp.char_type == 0) {
            if (((temp.sch >>  0) & 255) > ((temp.ech >>  0) & 255) ||
                ((temp.sch >>  8) & 255) > ((temp.ech >>  8) & 255) ||
                ((temp.sch >> 16) & 255) > ((temp.ech >> 16) & 255) ||
                ((temp.sch >> 24) & 255) > ((temp.ech >> 24) & 255)) {
                printf("invalid range schar or echar attribute\n");
                return (QST_ERROR);
            }
        }
        else {
            if (temp.sch > temp.ech) {
                printf("invalid range schar or echar attribute\n");
                return (QST_ERROR);
            }
        }
        temp.font_out = NULL;
        temp.font_bold = xml_attr_intxW(CR_WS("bold"), 0, &xml->nodes[idx]);
        if (array_push_growT(&list, sFONT_INFO, &temp) == NULL) {
            printf("out of memory\n");
            return (QST_ERROR);
        }
    }

    /* 根据列表生成字库 */
    make_font(&list, attrb, chset, cpage, api_w, api_h, fnt_w, fnt_h);
    array_freeT(&list, sFONT_INFO);
    xml_closeW(xml);
    return (QST_OKAY);
}
