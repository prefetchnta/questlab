/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-06  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 字符串操作头文件 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_STRLIB_H__
#define __CR_STRLIB_H__

#include "defs.h"

/*****************************************************************************/
/*                               字符类型识别                                */
/*****************************************************************************/

CR_API bool_t   is_alnumA (ansi_t ch);  /* 数字字母 */
CR_API bool_t   is_alphaA (ansi_t ch);  /*     字母 */
CR_API bool_t   is_cntrlA (ansi_t ch);  /* 控制字符 */
CR_API bool_t   is_digitA (ansi_t ch);  /* 数字     */
CR_API bool_t   is_graphA (ansi_t ch);  /* 图形字符 */
CR_API bool_t   is_lowerA (ansi_t ch);  /* 小写字母 */
CR_API bool_t   is_printA (ansi_t ch);  /* 显示字符 */
CR_API bool_t   is_punctA (ansi_t ch);  /* 标点字符 */
CR_API bool_t   is_spaceA (ansi_t ch);  /* 空白字符 */
CR_API bool_t   is_upperA (ansi_t ch);  /* 大写字母 */
CR_API bool_t   is_xnumbA (ansi_t ch);  /* 十六进制 */
CR_API bool_t   is_breakA (ansi_t ch);  /* 截断变量 */
CR_API bool_t   is_wordzA (ansi_t ch);  /* 变量字符 */
CR_API bool_t   is_timesA (ansi_t ch);  /* 时间分割 */
CR_API bool_t   is_datesA (ansi_t ch);  /* 日期分割 */
CR_API bool_t   is_slashA (ansi_t ch);  /* 路径分割 */
CR_API bool_t   is_plateA (ansi_t ch);  /* 车牌符号 */

CR_API bool_t   is_alnumW (wide_t ch);  /* 数字字母 */
CR_API bool_t   is_alphaW (wide_t ch);  /*     字母 */
CR_API bool_t   is_cntrlW (wide_t ch);  /* 控制字符 */
CR_API bool_t   is_digitW (wide_t ch);  /* 数字     */
CR_API bool_t   is_graphW (wide_t ch);  /* 图形字符 */
CR_API bool_t   is_lowerW (wide_t ch);  /* 小写字母 */
CR_API bool_t   is_printW (wide_t ch);  /* 显示字符 */
CR_API bool_t   is_punctW (wide_t ch);  /* 标点字符 */
CR_API bool_t   is_spaceW (wide_t ch);  /* 空白字符 */
CR_API bool_t   is_upperW (wide_t ch);  /* 大写字母 */
CR_API bool_t   is_xnumbW (wide_t ch);  /* 十六进制 */
CR_API bool_t   is_breakW (wide_t ch);  /* 截断变量 */
CR_API bool_t   is_wordzW (wide_t ch);  /* 变量字符 */
CR_API bool_t   is_timesW (wide_t ch);  /* 时间分割 */
CR_API bool_t   is_datesW (wide_t ch);  /* 日期分割 */
CR_API bool_t   is_slashW (wide_t ch);  /* 路径分割 */
CR_API bool_t   is_plateW (wide_t ch);  /* 车牌符号 */

/*****************************************************************************/
/*                              字符串工具函数                               */
/*****************************************************************************/

/***** 基本组 *****/
CR_API ansi_t*  str_uprA (ansi_t *str);
CR_API wide_t*  str_uprW (wide_t *str);
CR_API ansi_t*  str_lwrA (ansi_t *str);
CR_API wide_t*  str_lwrW (wide_t *str);
CR_API ansi_t*  str_flpA (ansi_t *str);
CR_API wide_t*  str_flpW (wide_t *str);

CR_API ansi_t*  str_trimA  (ansi_t *str);
CR_API wide_t*  str_trimW  (wide_t *str);
CR_API ansi_t*  str_trimLA (ansi_t *str);
CR_API wide_t*  str_trimLW (wide_t *str);
CR_API ansi_t*  str_trimRA (ansi_t *str);
CR_API wide_t*  str_trimRW (wide_t *str);

CR_API ansi_t*  str_strA  (const ansi_t *str1, const ansi_t *str2);
CR_API wide_t*  str_strW  (const wide_t *str1, const wide_t *str2);
CR_API ansi_t*  str_strIA (const ansi_t *str1, const ansi_t *str2);
CR_API wide_t*  str_strIW (const wide_t *str1, const wide_t *str2);

CR_API ansi_t*  str_cpyA  (ansi_t *dst, const ansi_t *src);
CR_API wide_t*  str_cpyW  (wide_t *dst, const wide_t *src);
CR_API ansi_t*  str_cpyNA (ansi_t *dst, const ansi_t *src, leng_t len);
CR_API wide_t*  str_cpyNW (wide_t *dst, const wide_t *src, leng_t len);

CR_API ansi_t*  str_catA  (ansi_t *dst, const ansi_t *src);
CR_API wide_t*  str_catW  (wide_t *dst, const wide_t *src);
CR_API ansi_t*  str_catNA (ansi_t *dst, const ansi_t *src, leng_t len);
CR_API wide_t*  str_catNW (wide_t *dst, const wide_t *src, leng_t len);

CR_API sint_t   str_cmpA  (const ansi_t *str1, const ansi_t *str2);
CR_API sint_t   str_cmpW  (const wide_t *str1, const wide_t *str2);
CR_API sint_t   str_cmpIA (const ansi_t *str1, const ansi_t *str2);
CR_API sint_t   str_cmpIW (const wide_t *str1, const wide_t *str2);
CR_API sint_t   str_cmpNA (const ansi_t *str1, const ansi_t *str2, leng_t len);
CR_API sint_t   str_cmpNW (const wide_t *str1, const wide_t *str2, leng_t len);

CR_API ansi_t*  str_chrA  (const ansi_t *str, ansi_t ch);
CR_API wide_t*  str_chrW  (const wide_t *str, wide_t ch);
CR_API ansi_t*  str_chr2A (const ansi_t *str, ansi_t ch1, ansi_t ch2);
CR_API wide_t*  str_chr2W (const wide_t *str, wide_t ch1, wide_t ch2);

CR_API ansi_t*  str_esc_chrU  (const ansi_t *str, ansi_t ch);
CR_API wide_t*  str_esc_chrW  (const wide_t *str, wide_t ch);
CR_API ansi_t*  str_esc_chr2U (const ansi_t *str, ansi_t ch1, ansi_t ch2);
CR_API wide_t*  str_esc_chr2W (const wide_t *str, wide_t ch1, wide_t ch2);

/***** 转换组 *****/
CR_API ansi_t*  int2strA   (ansi_t name[32], uint_t value);
CR_API wide_t*  int2strW   (wide_t name[32], uint_t value);
CR_API ansi_t*  int2str32A (ansi_t name[32], int32u value);
CR_API wide_t*  int2str32W (wide_t name[32], int32u value);
CR_API ansi_t*  int2str64A (ansi_t name[32], int64u value);
CR_API wide_t*  int2str64W (wide_t name[32], int64u value);

CR_API ansi_t*  hex2strA (ansi_t *str, const void_t *data, leng_t size);
CR_API wide_t*  hex2strW (wide_t *str, const void_t *data, leng_t size);
CR_API void_t*  str2datA (void_t *data, leng_t *size, const ansi_t *str);
CR_API void_t*  str2datW (void_t *data, leng_t *size, const wide_t *str);

CR_API ansi_t*  str2lstA (uint_t *list, leng_t count, const ansi_t *str,
                          const ansi_t head_tail_block[3]);
CR_API wide_t*  str2lstW (uint_t *list, leng_t count, const wide_t *str,
                          const wide_t head_tail_block[3]);

CR_API ansi_t*  str2vecA (fp32_t *list, leng_t count, const ansi_t *str,
                          const ansi_t head_tail_block[3]);
CR_API wide_t*  str2vecW (fp32_t *list, leng_t count, const wide_t *str,
                          const wide_t head_tail_block[3]);

CR_API uint_t*  str2lst2A (leng_t *count, const ansi_t *str,
                           const ansi_t head_tail_block[3],
                           leng_t *skip CR_DEFAULT(NULL));
CR_API uint_t*  str2lst2W (leng_t *count, const wide_t *str,
                           const wide_t head_tail_block[3],
                           leng_t *skip CR_DEFAULT(NULL));

CR_API fp32_t*  str2vec2A (leng_t *count, const ansi_t *str,
                           const ansi_t head_tail_block[3],
                           leng_t *skip CR_DEFAULT(NULL));
CR_API fp32_t*  str2vec2W (leng_t *count, const wide_t *str,
                           const wide_t head_tail_block[3],
                           leng_t *skip CR_DEFAULT(NULL));

CR_API real_t   str2realA (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API real_t   str2realW (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API fp32_t   str2fp32A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API fp32_t   str2fp32W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API fp64_t   str2fp64A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API fp64_t   str2fp64W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));

CR_API sint_t   str2intA   (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API sint_t   str2intW   (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int32s   str2int32A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int32s   str2int32W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int64s   str2int64A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int64s   str2int64W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));

CR_API uint_t   str2binA   (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API uint_t   str2binW   (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int32u   str2bin32A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int32u   str2bin32W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int64u   str2bin64A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int64u   str2bin64W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));

CR_API uint_t   str2octA   (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API uint_t   str2octW   (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int32u   str2oct32A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int32u   str2oct32W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int64u   str2oct64A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int64u   str2oct64W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));

CR_API uint_t   str2hexA   (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API uint_t   str2hexW   (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int32u   str2hex32A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int32u   str2hex32W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int64u   str2hex64A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int64u   str2hex64W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));

CR_API uint_t   str2intxA   (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API uint_t   str2intxW   (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int32u   str2intx32A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int32u   str2intx32W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int64u   str2intx64A (const ansi_t *str, leng_t *skip CR_DEFAULT(NULL));
CR_API int64u   str2intx64W (const wide_t *str, leng_t *skip CR_DEFAULT(NULL));

/***** 文件组 *****/
CR_API ansi_t*  path_uniqueA (ansi_t *str);
CR_API wide_t*  path_uniqueW (wide_t *str);
CR_API ansi_t** path_splitA (ansi_t *str, uint_t *count);
CR_API wide_t** path_splitW (wide_t *str, uint_t *count);
CR_API ansi_t*  path_appendA (const ansi_t *path1, const ansi_t *path2);
CR_API wide_t*  path_appendW (const wide_t *path1, const wide_t *path2);
CR_API ansi_t*  path_extractA (ansi_t *dst, const ansi_t *src);
CR_API wide_t*  path_extractW (wide_t *dst, const wide_t *src);

CR_API ansi_t*  flname_uniqueA (ansi_t *str);
CR_API wide_t*  flname_uniqueW (wide_t *str);
CR_API ansi_t*  flname_extractA (ansi_t *dst, const ansi_t *src);
CR_API wide_t*  flname_extractW (wide_t *dst, const wide_t *src);
CR_API sint_t   flname_compareA (const ansi_t *str1, const ansi_t *str2);
CR_API sint_t   flname_compareW (const wide_t *str1, const wide_t *str2);

CR_API ansi_t*  filext_removeA (ansi_t *str);
CR_API wide_t*  filext_removeW (wide_t *str);
CR_API ansi_t*  filext_changeA (ansi_t *dst, const ansi_t *src,
                                const ansi_t *ext);
CR_API wide_t*  filext_changeW (wide_t *dst, const wide_t *src,
                                const wide_t *ext);
CR_API ansi_t*  filext_extractA (ansi_t *dst, const ansi_t *src);
CR_API wide_t*  filext_extractW (wide_t *dst, const wide_t *src);
CR_API bool_t   filext_checkA (const ansi_t *str, const ansi_t *ext);
CR_API bool_t   filext_checkW (const wide_t *str, const wide_t *ext);

CR_API bool_t   wildcard_matchA  (const ansi_t *str, const ansi_t *pat);
CR_API bool_t   wildcard_matchW  (const wide_t *str, const wide_t *pat);
CR_API bool_t   wildcard_matchIA (const ansi_t *str, const ansi_t *pat);
CR_API bool_t   wildcard_matchIW (const wide_t *str, const wide_t *pat);

/***** 解析组 *****/
CR_API leng_t   str_esc_sizeU (const ansi_t *str);
CR_API leng_t   str_esc_sizeW (const wide_t *str);
CR_API leng_t   str_raw_sizeA (const ansi_t *str);
CR_API leng_t   str_raw_sizeW (const wide_t *str);
CR_API ansi_t*  str_esc_makeU (const ansi_t *str);
CR_API wide_t*  str_esc_makeW (const wide_t *str);

CR_API leng_t   str_esc_cpyU (ansi_t *dst, const ansi_t *src,
                              leng_t *skip CR_DEFAULT(NULL));
CR_API leng_t   str_raw_cpyW (wide_t *dst, const wide_t *src,
                              leng_t *skip CR_DEFAULT(NULL));
CR_API leng_t   str_esc_cpyW (wide_t *dst, const wide_t *src,
                              leng_t *skip CR_DEFAULT(NULL));
CR_API leng_t   str_raw_cpyA (ansi_t *dst, const ansi_t *src,
                              leng_t *skip CR_DEFAULT(NULL));

CR_API ansi_t*  str_esc_dupU (const ansi_t *str, leng_t *size
                    CR_DEFAULT(NULL), leng_t *skip CR_DEFAULT(NULL));
CR_API ansi_t*  str_raw_dupA (const ansi_t *str, leng_t *size
                    CR_DEFAULT(NULL), leng_t *skip CR_DEFAULT(NULL));
CR_API wide_t*  str_esc_dupW (const wide_t *str, leng_t *size
                    CR_DEFAULT(NULL), leng_t *skip CR_DEFAULT(NULL));
CR_API wide_t*  str_raw_dupW (const wide_t *str, leng_t *size
                    CR_DEFAULT(NULL), leng_t *skip CR_DEFAULT(NULL));

CR_API ansi_t*  remove_xml_cmtA (ansi_t *str);
CR_API wide_t*  remove_xml_cmtW (wide_t *str);
CR_API ansi_t*  skip_spaceA (const ansi_t *str);
CR_API wide_t*  skip_spaceW (const wide_t *str);
CR_API ansi_t*  skip_cpp_cmtA (const ansi_t *str);
CR_API wide_t*  skip_cpp_cmtW (const wide_t *str);
CR_API ansi_t*  skip_xml_cmtA (const ansi_t *str);
CR_API wide_t*  skip_xml_cmtW (const wide_t *str);

CR_API ansi_t*  str_esc_emptyU (ansi_t *str);
CR_API wide_t*  str_esc_emptyW (wide_t *str);
CR_API ansi_t** str_splitA (ansi_t *str, ansi_t sep, leng_t *count);
CR_API wide_t** str_splitW (wide_t *str, wide_t sep, leng_t *count);
CR_API ansi_t** str_cmd_splitA (ansi_t *str, uint_t *count);
CR_API wide_t** str_cmd_splitW (wide_t *str, uint_t *count);
CR_API ansi_t*  str_url_splitA (const ansi_t *url, ansi_t **scheme,
                                ansi_t **username, ansi_t **password,
                                ansi_t **host, uint_t *port, ansi_t **path,
                                ansi_t **params, ansi_t **flags);
CR_API wide_t*  str_url_splitW (const wide_t *url, wide_t **scheme,
                                wide_t **username, wide_t **password,
                                wide_t **host, uint_t *port, wide_t **path,
                                wide_t **params, wide_t **flags);
CR_API leng_t   str_get_lineA (const ansi_t *pos, const ansi_t *str,
                               leng_t *cols CR_DEFAULT(NULL));
CR_API leng_t   str_get_lineW (const wide_t *pos, const wide_t *str,
                               leng_t *cols CR_DEFAULT(NULL));

/*****************************************************************************/
/*                              多字节编码工具                               */
/*****************************************************************************/

/* 字符长度计算 */
typedef uint_t  (*offs_cha_t) (const ansi_t*);

CR_API uint_t   offset_gbk (const ansi_t *str);
CR_API uint_t   offset_utf8 (const ansi_t *str);
CR_API uint_t   offset_big5 (const ansi_t *str);
CR_API uint_t   offset_sjis (const ansi_t *str);
CR_API uint_t   offset_eucjp (const ansi_t *str);
CR_API uint_t   offset_ascii (const ansi_t *str);
CR_API uint_t   offset_gb2312 (const ansi_t *str);
CR_API uint_t   offset_gb18030 (const ansi_t *str);

CR_API leng_t   str_length (const ansi_t *str, offs_cha_t get_offs);

/* 文本自动断行 */
CR_API ansi_t*  text_wrap1 (const ansi_t *text, uint_t count,
                            offs_cha_t get_offs);
CR_API ansi_t*  text_wrap2 (const ansi_t *text, uint_t count,
                            offs_cha_t get_offs);
/* URL 编码转换 */
CR_API ansi_t*  url_encode (const ansi_t *str);
CR_API ansi_t*  url_decode (const ansi_t *str);

/*****************************************************************************/
/*                              字符串编码转换                               */
/*****************************************************************************/

/* 直接截断转换 */
CR_API wide_t*  str_char2wide (const ansi_t *str);
CR_API ansi_t*  str_wide2char (const wide_t *str);

/* 字节顺序映射 */
#if defined(_CR_ORDER_LE_)
    #define ucs4_to_utf16   ucs4_to_utf16le
    #define ucs4_to_utf32   ucs4_to_utf32le
    #define utf16_to_ucs4   utf16le_to_ucs4
    #define utf32_to_ucs4   utf32le_to_ucs4
#else   /* (_CR_ORDER_BE_) */
    #define ucs4_to_utf16   ucs4_to_utf16be
    #define ucs4_to_utf32   ucs4_to_utf32be
    #define utf16_to_ucs4   utf16be_to_ucs4
    #define utf32_to_ucs4   utf32be_to_ucs4
#endif

/* 单个字符转换 */
CR_API uint_t   ucs4_to_utf8    (void_t *chr, int32u ucs4);
CR_API uint_t   ucs4_to_utf16le (void_t *chr, int32u ucs4);
CR_API uint_t   ucs4_to_utf16be (void_t *chr, int32u ucs4);
CR_API uint_t   ucs4_to_utf32le (void_t *chr, int32u ucs4);
CR_API uint_t   ucs4_to_utf32be (void_t *chr, int32u ucs4);

CR_API uint_t   utf8_to_ucs4    (int32u *ucs4, const void_t *chr);
CR_API uint_t   utf16le_to_ucs4 (int32u *ucs4, const void_t *chr);
CR_API uint_t   utf16be_to_ucs4 (int32u *ucs4, const void_t *chr);
CR_API uint_t   utf32le_to_ucs4 (int32u *ucs4, const void_t *chr);
CR_API uint_t   utf32be_to_ucs4 (int32u *ucs4, const void_t *chr);

/* 字符串的转换 */
CR_API wide_t*  utf8_to_utf16 (const ansi_t *utf8);
CR_API ansi_t*  utf16_to_utf8 (const wide_t *wide);

/* 字符相互定位 */
CR_API leng_t   utf8_seek_utf16 (const ansi_t *utf8, leng_t pos);
CR_API leng_t   utf16_seek_utf8 (const wide_t *wide, leng_t pos);

/* 多字节编码转换 (需外部库支持) */
CR_API void_t*  str_acp2uni (uint_t codepage, const ansi_t *str,
                             leng_t *size, bool_t use_utf16);
CR_API ansi_t*  str_uni2acp (uint_t codepage, const void_t *str,
                             leng_t *size, bool_t use_utf16);
/* 转换简化参数宏 */
#define local_to_utf8(cp, str)  (ansi_t*)str_acp2uni(cp, str, NULL, FALSE)
#define utf8_to_local(cp, str)  (ansi_t*)str_uni2acp(cp, str, NULL, FALSE)
#define local_to_utf16(cp, str) (wide_t*)str_acp2uni(cp, str, NULL,  TRUE)
#define utf16_to_local(cp, str) (ansi_t*)str_uni2acp(cp, str, NULL,  TRUE)

/* 全角/半角字符转换 (需外部库支持) */
CR_API void_t*  str_half2full (const void_t *half, uint_t codepage);
CR_API void_t*  str_full2half (const void_t *full, uint_t codepage);

/* 文本编码转换 (支持 BOM 标志和编码指示) */
CR_API ansi_t*  text_to_utf8 (const ansi_t *text, uint_t *codepage,
                              bool_t is_xml CR_DEFAULT(FALSE));
CR_API wide_t*  text_to_utf16 (const ansi_t *text, uint_t *codepage,
                               bool_t is_xml CR_DEFAULT(FALSE));

/* 字符串格式化 + 编码转换 (从本地编码转出) */
CR_API ansi_t*  str_fmtA (const ansi_t *format, ...);
CR_API ansi_t*  str_fmtU (const ansi_t *format, ...);
CR_API wide_t*  str_fmtW (const ansi_t *format, ...);
CR_API void_t*  str_fmtX (uint_t codepage, leng_t *size,
                          const ansi_t *format, ...);
/* 兼容老代码的映射 */
#define str_format  str_fmtX

/* HTML/XML 转义字符处理 */
CR_API ansi_t*  html_to_stringU (const ansi_t *str);
CR_API wide_t*  html_to_stringW (const wide_t *str);

#endif  /* !__CR_STRLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
