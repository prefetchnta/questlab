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
 *             ##       CREATE: 2015-03-20
 *              #
 ================================================
        文本文件编码转换小工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

#include <conio.h>

/*
---------------------------------------
    显示工具用法
---------------------------------------
*/
static void_t usage (void_t)
{
    printf("codepage <from> <to> <root_dir> <file_list>\n");
    printf("===========================================\n");
    printf("000   : System\n");
    printf("037   : IBM EBCDIC - U.S./Canada\n");
    printf("437   : OEM - United States\n");
    printf("500   : IBM EBCDIC - International\n");
    printf("708   : Arabic - ASMO 708\n");
    printf("709   : Arabic - ASMO 449+, BCON V4\n");
    printf("710   : Arabic - Transparent Arabic\n");
    printf("720   : Arabic - Transparent ASMO\n");
    printf("737   : OEM - Greek (formerly 437G)\n");
    printf("775   : OEM - Baltic\n");
    printf("850   : OEM - Multilingual Latin I\n");
    printf("852   : OEM - Latin II\n");
    printf("855   : OEM - Cyrillic (primarily Russian)\n");
    printf("857   : OEM - Turkish\n");
    printf("858   : OEM - Multilingual Latin I + Euro symbol\n");
    printf("860   : OEM - Portuguese\n");
    printf("861   : OEM - Icelandic\n");
    printf("862   : OEM - Hebrew\n");
    printf("863   : OEM - Canadian-French\n");
    printf("864   : OEM - Arabic\n");
    printf("865   : OEM - Nordic\n");
    printf("866   : OEM - Russian\n");
    getch();
    printf("869   : OEM - Modern Greek\n");
    printf("870   : IBM EBCDIC - Multilingual/ROECE (Latin-2)\n");
    printf("874   : ANSI/OEM - Thai (same as 28605, ISO 8859-15)\n");
    printf("875   : IBM EBCDIC - Modern Greek\n");
    printf("932   : ANSI/OEM - Japanese, Shift-JIS\n");
    printf("936   : ANSI/OEM - Simplified Chinese (PRC, Singapore)\n");
    printf("949   : ANSI/OEM - Korean (Unified Hangul Code)\n");
    printf("950   : ANSI/OEM - Traditional Chinese (Taiwan; Hong Kong SAR, PRC)\n");
    printf("1026  : IBM EBCDIC - Turkish (Latin-5)\n");
    printf("1047  : IBM EBCDIC - Latin 1/Open System\n");
    printf("1140  : IBM EBCDIC - U.S./Canada (037 + Euro symbol)\n");
    printf("1141  : IBM EBCDIC - Germany (20273 + Euro symbol)\n");
    printf("1142  : IBM EBCDIC - Denmark/Norway (20277 + Euro symbol)\n");
    printf("1143  : IBM EBCDIC - Finland/Sweden (20278 + Euro symbol)\n");
    printf("1144  : IBM EBCDIC - Italy (20280 + Euro symbol)\n");
    printf("1145  : IBM EBCDIC - Latin America/Spain (20284 + Euro symbol)\n");
    printf("1146  : IBM EBCDIC - United Kingdom (20285 + Euro symbol)\n");
    printf("1147  : IBM EBCDIC - France (20297 + Euro symbol)\n");
    printf("1148  : IBM EBCDIC - International (500 + Euro symbol)\n");
    printf("1149  : IBM EBCDIC - Icelandic (20871 + Euro symbol)\n");
    printf("1200  : Unicode UCS-2 Little-Endian (BMP of ISO 10646)\n");
    printf("1201  : Unicode UCS-2 Big-Endian\n");
    printf("1250  : ANSI - Central European\n");
    printf("1251  : ANSI - Cyrillic\n");
    getch();
    printf("1252  : ANSI - Latin I\n");
    printf("1253  : ANSI - Greek\n");
    printf("1254  : ANSI - Turkish\n");
    printf("1255  : ANSI - Hebrew\n");
    printf("1256  : ANSI - Arabic\n");
    printf("1257  : ANSI - Baltic\n");
    printf("1258  : ANSI/OEM - Vietnamese\n");
    printf("1361  : Korean (Johab)\n");
    printf("10000 : MAC - Roman\n");
    printf("10001 : MAC - Japanese\n");
    printf("10002 : MAC - Traditional Chinese (Big5)\n");
    printf("10003 : MAC - Korean\n");
    printf("10004 : MAC - Arabic\n");
    printf("10005 : MAC - Hebrew\n");
    printf("10006 : MAC - Greek I\n");
    printf("10007 : MAC - Cyrillic\n");
    printf("10008 : MAC - Simplified Chinese (GB 2312)\n");
    printf("10010 : MAC - Romania\n");
    printf("10017 : MAC - Ukraine\n");
    printf("10021 : MAC - Thai\n");
    printf("10029 : MAC - Latin II\n");
    printf("10079 : MAC - Icelandic\n");
    printf("10081 : MAC - Turkish\n");
    printf("10082 : MAC - Croatia\n");
    getch();
    printf("12000 : Unicode UCS-4 Little-Endian\n");
    printf("12001 : Unicode UCS-4 Big-Endian\n");
    printf("20000 : CNS - Taiwan\n");
    printf("20001 : TCA - Taiwan\n");
    printf("20002 : Eten - Taiwan\n");
    printf("20003 : IBM5550 - Taiwan\n");
    printf("20004 : TeleText - Taiwan\n");
    printf("20005 : Wang - Taiwan\n");
    printf("20105 : IA5 IRV International Alphabet No. 5 (7-bit)\n");
    printf("20106 : IA5 German (7-bit)\n");
    printf("20107 : IA5 Swedish (7-bit)\n");
    printf("20108 : IA5 Norwegian (7-bit)\n");
    printf("20127 : US-ASCII (7-bit)\n");
    printf("20261 : T.61\n");
    printf("20269 : ISO 6937 Non-Spacing Accent\n");
    printf("20273 : IBM EBCDIC - Germany\n");
    printf("20277 : IBM EBCDIC - Denmark/Norway\n");
    printf("20278 : IBM EBCDIC - Finland/Sweden\n");
    printf("20280 : IBM EBCDIC - Italy\n");
    printf("20284 : IBM EBCDIC - Latin America/Spain\n");
    printf("20285 : IBM EBCDIC - United Kingdom\n");
    printf("20290 : IBM EBCDIC - Japanese Katakana Extended\n");
    printf("20297 : IBM EBCDIC - France\n");
    printf("20420 : IBM EBCDIC - Arabic\n");
    getch();
    printf("20423 : IBM EBCDIC - Greek\n");
    printf("20424 : IBM EBCDIC - Hebrew\n");
    printf("20833 : IBM EBCDIC - Korean Extended\n");
    printf("20838 : IBM EBCDIC - Thai\n");
    printf("20866 : Russian - KOI8-R\n");
    printf("20871 : IBM EBCDIC - Icelandic\n");
    printf("20880 : IBM EBCDIC - Cyrillic (Russian)\n");
    printf("20905 : IBM EBCDIC - Turkish\n");
    printf("20924 : IBM EBCDIC - Latin-1/Open System (1047 + Euro symbol)\n");
    printf("20932 : JIS X 0208-1990 & 0121-1990\n");
    printf("20936 : Simplified Chinese (GB2312)\n");
    printf("20949 : Korean Wansung\n");
    printf("21025 : IBM EBCDIC - Cyrillic (Serbian, Bulgarian)\n");
    printf("21027 : (deprecated)\n");
    printf("21866 : Ukrainian (KOI8-U)\n");
    printf("28591 : ISO 8859-1 Latin I\n");
    printf("28592 : ISO 8859-2 Central Europe\n");
    printf("28593 : ISO 8859-3 Latin 3\n");
    printf("28594 : ISO 8859-4 Baltic\n");
    printf("28595 : ISO 8859-5 Cyrillic\n");
    printf("28596 : ISO 8859-6 Arabic\n");
    printf("28597 : ISO 8859-7 Greek\n");
    printf("28598 : ISO 8859-8 Hebrew\n");
    printf("28599 : ISO 8859-9 Latin 5\n");
    getch();
    printf("28603 : ISO 8859-13 Estonian\n");
    printf("28605 : ISO 8859-15 Latin 9\n");
    printf("29001 : Europa 3\n");
    printf("38598 : ISO 8859-8 Hebrew\n");
    printf("50220 : ISO 2022 Japanese with no halfwidth Katakana\n");
    printf("50221 : ISO 2022 Japanese with halfwidth Katakana\n");
    printf("50222 : ISO 2022 Japanese JIS X 0201-1989\n");
    printf("50225 : ISO 2022 Korean\n");
    printf("50227 : ISO 2022 Simplified Chinese\n");
    printf("50229 : ISO 2022 Traditional Chinese\n");
    printf("50930 : Japanese (Katakana) Extended\n");
    printf("50931 : US/Canada and Japanese\n");
    printf("50933 : Korean Extended and Korean\n");
    printf("50935 : Simplified Chinese Extended and Simplified Chinese\n");
    printf("50936 : Simplified Chinese\n");
    printf("50937 : US/Canada and Traditional Chinese\n");
    printf("50939 : Japanese (Latin) Extended and Japanese\n");
    printf("51932 : EUC - Japanese\n");
    printf("51936 : EUC - Simplified Chinese\n");
    printf("51949 : EUC - Korean\n");
    printf("51950 : EUC - Traditional Chinese\n");
    printf("52936 : HZ-GB2312 Simplified Chinese\n");
    printf("54936 : GB18030 Simplified Chinese (4 Byte)\n");
    printf("57002 : ISCII Devanagari\n");
    getch();
    printf("57003 : ISCII Bengali\n");
    printf("57004 : ISCII Tamil\n");
    printf("57005 : ISCII Telugu\n");
    printf("57006 : ISCII Assamese\n");
    printf("57007 : ISCII Oriya\n");
    printf("57008 : ISCII Kannada\n");
    printf("57009 : ISCII Malayalam\n");
    printf("57010 : ISCII Gujarati\n");
    printf("57011 : ISCII Punjabi\n");
    printf("65000 : Unicode UTF-7\n");
    printf("65001 : Unicode UTF-8\n");
}

/*
---------------------------------------
    检查是否是 UTF-8
---------------------------------------
*/
static bool_t
is_utf8_file (
  __CR_IN__ const ansi_t*   str
    )
{
    leng_t  temp;
    int32u  ucs4;

    /* BOM 标志检查 */
    if (chr_cmpA(str, BOM_UTF8, 3) == 0)
        str += 3;

    /* 逐个字符检查 */
    while (*str != NIL) {
        temp = utf8_to_ucs4(&ucs4, str);
        if (temp == 0)
            return (FALSE);
        str += temp;
    }
    return (TRUE);
}

/* 用来传递参数 */
typedef struct
{
        uint_t  tot, cvt;   /* 文件转换统计计数 */
        uint_t  from, to;   /* 源与目标转换编码 */
        bool_t  skip_utf8;  /* 跳过 UTF-8 源文件 */
        bool_t  del_u8_bom; /* 删除 UTF-8 BOM 标志 */

} sCP_PARAM;

/*
---------------------------------------
    文件处理回调
---------------------------------------
*/
static bool_t
do_each_file (
  __CR_IO__ void_t*     param,
  __CR_IN__ sSEARCHa*   finfo
    )
{
    leng_t      len;
    ansi_t*     src;
    ansi_t*     dst;
    wide_t*     tmp;
    sCP_PARAM*  ctx;

    /* 统计文件总数 */
    ctx = (sCP_PARAM*)param;
    ctx->tot += 1;

    /* 跳过空文件和只读文件 */
    if ((finfo->size == 0) ||
        (finfo->attr & CR_FILE_ATTR_RDO))
        return (TRUE);

    /* 过滤掉大文件 */
    if (finfo->size > CR_M2B(8))
        return (TRUE);
    if (ctx->del_u8_bom && finfo->size <= 3)
        return (TRUE);

    /* 加载整个文件 */
    src = file_load_as_strA(finfo->name);
    if (src == NULL)
        return (TRUE);

    /* 跳过二进制文件 */
    len = str_lenA(src);
    if (len != finfo->size)
        goto _func_out;

    /* 删除 UTF-8 文件的 BOM 标志 */
    if (ctx->del_u8_bom) {
        if (chr_cmpA(src, BOM_UTF8, 3) != 0)
            goto _func_out;
        if (!is_utf8_file(src))
            goto _func_out;
        if (!file_saveA(finfo->name, src + 3, len - 3))
            goto _func_out;
        ctx->cvt += 1;
        printf("%s\n", finfo->name);
        goto _func_out;
    }

    /* 检查是否是 UTF-8, 跳过不用转换的文件 */
    if (ctx->skip_utf8 && is_utf8_file(src))
        goto _func_out;

    /* 转换到 UTF-16 编码 */
    len = MultiByteToWideChar(ctx->from, MB_ERR_INVALID_CHARS,
                              src, -1, NULL, 0);
    if (len == 0)
        goto _func_out;
    tmp = str_allocW(len);
    if (tmp == NULL)
        goto _func_out;
    MultiByteToWideChar(ctx->from, 0, src, -1, tmp, len);
    mem_free(src);

    /* 转换到目标编码 */
    len = WideCharToMultiByte(ctx->to, 0, tmp, -1, NULL, 0, NULL, NULL);
    if (len == 0) {
        mem_free(tmp);
        return (TRUE);
    }
    dst = str_allocA(len);
    if (dst == NULL) {
        mem_free(tmp);
        return (TRUE);
    }
    len = WideCharToMultiByte(ctx->to, 0, tmp, -1, dst, len, NULL, NULL);
    mem_free(tmp);

    /* 写入整个文件 (去掉后面的0) */
    if (!file_saveA(finfo->name, dst, len - 1)) {
        mem_free(dst);
        return (TRUE);
    }
    ctx->cvt += 1;
    printf("%s\n", finfo->name);
    mem_free(dst);
    return (TRUE);

_func_out:
    mem_free(src);
    return (TRUE);
}

/*
=======================================
    命令行主函数
=======================================
*/
int main (int argc, char *argv[])
{
    sCP_PARAM   ft;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 <源编码> <目标编码> <搜索根目录> <通配符> */
    if (argc < 5) {
        usage();
        return (QST_ERROR);
    }

    /* 填充结构 */
    ft.tot = ft.cvt = 0;
    ft.from = str2intA(argv[1]);
    if (ft.from == CR_LOCAL)
        ft.from = get_sys_codepage();
    ft.to = str2intA(argv[2]);
    if (ft.to == CR_LOCAL)
        ft.to = get_sys_codepage();

    /* 已经是 UTF-8 的文件不用转换 */
    if (ft.to == CR_UTF8)
        ft.skip_utf8 = TRUE;
    else
        ft.skip_utf8 = FALSE;

    /* 两个编码值相等表示删除 UTF-8 BOM 标志 */
    if (ft.from == ft.to)
        ft.del_u8_bom = TRUE;
    else
        ft.del_u8_bom = FALSE;

    /* 子目录递归 + 跳过隐藏目录和文件 */
    file_searchA(argv[3], TRUE, TRUE, FALSE, argv[4], do_each_file, &ft);
    printf("%u file(s) found. %u file(s) converted.\n", ft.tot, ft.cvt);
    return (QST_OKAY);
}
