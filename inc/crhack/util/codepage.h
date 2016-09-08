/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-05-08  */
/*     #######          ###    ###      [UTIL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> 自定义编码转换头文件 <<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_CODEPAGE_H__
#define __CR_CODEPAGE_H__

#include "../defs.h"

/* CP932 - Shift-JIS */
CR_API void_t*  str_cp932_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp932 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp932_to_utf8(str)  (ansi_t*)str_cp932_to_uni(str, NULL, FALSE)
#define utf8_to_cp932(str)  (ansi_t*)str_uni_to_cp932(str, NULL, FALSE)
#define cp932_to_utf16(str) (wide_t*)str_cp932_to_uni(str, NULL,  TRUE)
#define utf16_to_cp932(str) (ansi_t*)str_uni_to_cp932(str, NULL,  TRUE)

/* CP936 - GBK */
CR_API void_t*  str_cp936_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp936 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp936_to_utf8(str)  (ansi_t*)str_cp936_to_uni(str, NULL, FALSE)
#define utf8_to_cp936(str)  (ansi_t*)str_uni_to_cp936(str, NULL, FALSE)
#define cp936_to_utf16(str) (wide_t*)str_cp936_to_uni(str, NULL,  TRUE)
#define utf16_to_cp936(str) (ansi_t*)str_uni_to_cp936(str, NULL,  TRUE)

/* CP949 - EUC-KR */
CR_API void_t*  str_cp949_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp949 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp949_to_utf8(str)  (ansi_t*)str_cp949_to_uni(str, NULL, FALSE)
#define utf8_to_cp949(str)  (ansi_t*)str_uni_to_cp949(str, NULL, FALSE)
#define cp949_to_utf16(str) (wide_t*)str_cp949_to_uni(str, NULL,  TRUE)
#define utf16_to_cp949(str) (ansi_t*)str_uni_to_cp949(str, NULL,  TRUE)

/* CP950 - BIG5 */
CR_API void_t*  str_cp950_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp950 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp950_to_utf8(str)  (ansi_t*)str_cp950_to_uni(str, NULL, FALSE)
#define utf8_to_cp950(str)  (ansi_t*)str_uni_to_cp950(str, NULL, FALSE)
#define cp950_to_utf16(str) (wide_t*)str_cp950_to_uni(str, NULL,  TRUE)
#define utf16_to_cp950(str) (ansi_t*)str_uni_to_cp950(str, NULL,  TRUE)

/* CP437 - U.S. */
CR_API void_t*  str_cp437_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp437 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp437_to_utf8(str)  (ansi_t*)str_cp437_to_uni(str, NULL, FALSE)
#define utf8_to_cp437(str)  (ansi_t*)str_uni_to_cp437(str, NULL, FALSE)
#define cp437_to_utf16(str) (wide_t*)str_cp437_to_uni(str, NULL,  TRUE)
#define utf16_to_cp437(str) (ansi_t*)str_uni_to_cp437(str, NULL,  TRUE)

/* CP720 - Arabic */
CR_API void_t*  str_cp720_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp720 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp720_to_utf8(str)  (ansi_t*)str_cp720_to_uni(str, NULL, FALSE)
#define utf8_to_cp720(str)  (ansi_t*)str_uni_to_cp720(str, NULL, FALSE)
#define cp720_to_utf16(str) (wide_t*)str_cp720_to_uni(str, NULL,  TRUE)
#define utf16_to_cp720(str) (ansi_t*)str_uni_to_cp720(str, NULL,  TRUE)

/* CP737 - Greek */
CR_API void_t*  str_cp737_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp737 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp737_to_utf8(str)  (ansi_t*)str_cp737_to_uni(str, NULL, FALSE)
#define utf8_to_cp737(str)  (ansi_t*)str_uni_to_cp737(str, NULL, FALSE)
#define cp737_to_utf16(str) (wide_t*)str_cp737_to_uni(str, NULL,  TRUE)
#define utf16_to_cp737(str) (ansi_t*)str_uni_to_cp737(str, NULL,  TRUE)

/* CP771 - KBL */
CR_API void_t*  str_cp771_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp771 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp771_to_utf8(str)  (ansi_t*)str_cp771_to_uni(str, NULL, FALSE)
#define utf8_to_cp771(str)  (ansi_t*)str_uni_to_cp771(str, NULL, FALSE)
#define cp771_to_utf16(str) (wide_t*)str_cp771_to_uni(str, NULL,  TRUE)
#define utf16_to_cp771(str) (ansi_t*)str_uni_to_cp771(str, NULL,  TRUE)

/* CP775 - Baltic */
CR_API void_t*  str_cp775_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp775 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp775_to_utf8(str)  (ansi_t*)str_cp775_to_uni(str, NULL, FALSE)
#define utf8_to_cp775(str)  (ansi_t*)str_uni_to_cp775(str, NULL, FALSE)
#define cp775_to_utf16(str) (wide_t*)str_cp775_to_uni(str, NULL,  TRUE)
#define utf16_to_cp775(str) (ansi_t*)str_uni_to_cp775(str, NULL,  TRUE)

/* CP850 - Latin 1 */
CR_API void_t*  str_cp850_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp850 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp850_to_utf8(str)  (ansi_t*)str_cp850_to_uni(str, NULL, FALSE)
#define utf8_to_cp850(str)  (ansi_t*)str_uni_to_cp850(str, NULL, FALSE)
#define cp850_to_utf16(str) (wide_t*)str_cp850_to_uni(str, NULL,  TRUE)
#define utf16_to_cp850(str) (ansi_t*)str_uni_to_cp850(str, NULL,  TRUE)

/* CP852 - Latin 2 */
CR_API void_t*  str_cp852_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp852 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp852_to_utf8(str)  (ansi_t*)str_cp852_to_uni(str, NULL, FALSE)
#define utf8_to_cp852(str)  (ansi_t*)str_uni_to_cp852(str, NULL, FALSE)
#define cp852_to_utf16(str) (wide_t*)str_cp852_to_uni(str, NULL,  TRUE)
#define utf16_to_cp852(str) (ansi_t*)str_uni_to_cp852(str, NULL,  TRUE)

/* CP855 - Cyrillic */
CR_API void_t*  str_cp855_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp855 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp855_to_utf8(str)  (ansi_t*)str_cp855_to_uni(str, NULL, FALSE)
#define utf8_to_cp855(str)  (ansi_t*)str_uni_to_cp855(str, NULL, FALSE)
#define cp855_to_utf16(str) (wide_t*)str_cp855_to_uni(str, NULL,  TRUE)
#define utf16_to_cp855(str) (ansi_t*)str_uni_to_cp855(str, NULL,  TRUE)

/* CP857 - Turkish */
CR_API void_t*  str_cp857_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp857 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp857_to_utf8(str)  (ansi_t*)str_cp857_to_uni(str, NULL, FALSE)
#define utf8_to_cp857(str)  (ansi_t*)str_uni_to_cp857(str, NULL, FALSE)
#define cp857_to_utf16(str) (wide_t*)str_cp857_to_uni(str, NULL,  TRUE)
#define utf16_to_cp857(str) (ansi_t*)str_uni_to_cp857(str, NULL,  TRUE)

/* CP860 - Portuguese */
CR_API void_t*  str_cp860_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp860 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp860_to_utf8(str)  (ansi_t*)str_cp860_to_uni(str, NULL, FALSE)
#define utf8_to_cp860(str)  (ansi_t*)str_uni_to_cp860(str, NULL, FALSE)
#define cp860_to_utf16(str) (wide_t*)str_cp860_to_uni(str, NULL,  TRUE)
#define utf16_to_cp860(str) (ansi_t*)str_uni_to_cp860(str, NULL,  TRUE)

/* CP861 - Icelandic */
CR_API void_t*  str_cp861_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp861 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp861_to_utf8(str)  (ansi_t*)str_cp861_to_uni(str, NULL, FALSE)
#define utf8_to_cp861(str)  (ansi_t*)str_uni_to_cp861(str, NULL, FALSE)
#define cp861_to_utf16(str) (wide_t*)str_cp861_to_uni(str, NULL,  TRUE)
#define utf16_to_cp861(str) (ansi_t*)str_uni_to_cp861(str, NULL,  TRUE)

/* CP862 - Hebrew */
CR_API void_t*  str_cp862_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp862 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp862_to_utf8(str)  (ansi_t*)str_cp862_to_uni(str, NULL, FALSE)
#define utf8_to_cp862(str)  (ansi_t*)str_uni_to_cp862(str, NULL, FALSE)
#define cp862_to_utf16(str) (wide_t*)str_cp862_to_uni(str, NULL,  TRUE)
#define utf16_to_cp862(str) (ansi_t*)str_uni_to_cp862(str, NULL,  TRUE)

/* CP863 - Canadian French */
CR_API void_t*  str_cp863_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp863 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp863_to_utf8(str)  (ansi_t*)str_cp863_to_uni(str, NULL, FALSE)
#define utf8_to_cp863(str)  (ansi_t*)str_uni_to_cp863(str, NULL, FALSE)
#define cp863_to_utf16(str) (wide_t*)str_cp863_to_uni(str, NULL,  TRUE)
#define utf16_to_cp863(str) (ansi_t*)str_uni_to_cp863(str, NULL,  TRUE)

/* CP864 - Arabic */
CR_API void_t*  str_cp864_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp864 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp864_to_utf8(str)  (ansi_t*)str_cp864_to_uni(str, NULL, FALSE)
#define utf8_to_cp864(str)  (ansi_t*)str_uni_to_cp864(str, NULL, FALSE)
#define cp864_to_utf16(str) (wide_t*)str_cp864_to_uni(str, NULL,  TRUE)
#define utf16_to_cp864(str) (ansi_t*)str_uni_to_cp864(str, NULL,  TRUE)

/* CP865 - Nordic */
CR_API void_t*  str_cp865_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp865 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp865_to_utf8(str)  (ansi_t*)str_cp865_to_uni(str, NULL, FALSE)
#define utf8_to_cp865(str)  (ansi_t*)str_uni_to_cp865(str, NULL, FALSE)
#define cp865_to_utf16(str) (wide_t*)str_cp865_to_uni(str, NULL,  TRUE)
#define utf16_to_cp865(str) (ansi_t*)str_uni_to_cp865(str, NULL,  TRUE)

/* CP866 - Russian */
CR_API void_t*  str_cp866_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp866 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp866_to_utf8(str)  (ansi_t*)str_cp866_to_uni(str, NULL, FALSE)
#define utf8_to_cp866(str)  (ansi_t*)str_uni_to_cp866(str, NULL, FALSE)
#define cp866_to_utf16(str) (wide_t*)str_cp866_to_uni(str, NULL,  TRUE)
#define utf16_to_cp866(str) (ansi_t*)str_uni_to_cp866(str, NULL,  TRUE)

/* CP869 - Greek 2 */
CR_API void_t*  str_cp869_to_uni (const ansi_t *str, leng_t *size,
                                  bool_t use_utf16);
CR_API ansi_t*  str_uni_to_cp869 (const void_t *str, leng_t *size,
                                  bool_t use_utf16);
/* 转换简化参数宏 */
#define cp869_to_utf8(str)  (ansi_t*)str_cp869_to_uni(str, NULL, FALSE)
#define utf8_to_cp869(str)  (ansi_t*)str_uni_to_cp869(str, NULL, FALSE)
#define cp869_to_utf16(str) (wide_t*)str_cp869_to_uni(str, NULL,  TRUE)
#define utf16_to_cp869(str) (ansi_t*)str_uni_to_cp869(str, NULL,  TRUE)

/* 外挂函数声明 */
#if !defined(_USE_LFN)
CR_API void_t   local_cpage (uint_t cpage);
CR_API void_t*  str_ccp2uni (uint_t codepage, const ansi_t *str,
                             leng_t *size, bool_t use_utf16);
CR_API ansi_t*  str_uni2ccp (uint_t codepage, const void_t *str,
                             leng_t *size, bool_t use_utf16);
#endif  /* !_USE_LFN */

#endif  /* !__CR_CODEPAGE_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
