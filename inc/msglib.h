/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-16  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 消息提示头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_MSGLIB_H__
#define __CR_MSGLIB_H__ 0xE600F092UL

#include "defs.h"

/*****************************************************************************/
/*                                消息提示框                                 */
/*****************************************************************************/

/* 自定义提示 */
#define CR_STOP     0   /* 严重错误 */
#define CR_WARN     1   /* 警告错误 */
#define CR_DONE     2   /* 成功提示 */
typedef void_t  (*msgboxA_t) (const ansi_t*, const ansi_t*, uint_t);
typedef void_t  (*msgboxW_t) (const wide_t*, const wide_t*, uint_t);

CR_API void_t   set_msg_callA (msgboxA_t func);
CR_API void_t   set_msg_callW (msgboxW_t func);

/* 消息提示函数 */
CR_API void_t   msg_stopA (const ansi_t *text, const ansi_t *title);
CR_API void_t   msg_warnA (const ansi_t *text, const ansi_t *title);
CR_API void_t   msg_doneA (const ansi_t *text, const ansi_t *title);
CR_API bool_t   msg_askuA (const ansi_t *text, const ansi_t *title);

CR_API void_t   msg_stopW (const wide_t *text, const wide_t *title);
CR_API void_t   msg_warnW (const wide_t *text, const wide_t *title);
CR_API void_t   msg_doneW (const wide_t *text, const wide_t *title);
CR_API bool_t   msg_askuW (const wide_t *text, const wide_t *title);

/* 消息提示映射 */
#define msg_stopT(text, title)  msg_stopA(CR_AS(text), CR_AS(title))
#define msg_warnT(text, title)  msg_warnA(CR_AS(text), CR_AS(title))
#define msg_doneT(text, title)  msg_doneA(CR_AS(text), CR_AS(title))

/*****************************************************************************/
/*                               CUI 输出控制                                */
/*****************************************************************************/

/* 字符属性位 (前8位 VGA 兼容) */
#define CR_CUI_TEXT_BLUE    0x0001  /* 前景蓝色 */
#define CR_CUI_TEXT_GREEN   0x0002  /* 前景绿色 */
#define CR_CUI_TEXT_RED     0x0004  /* 前景红色 */
#define CR_CUI_TEXT_LIGHT   0x0008  /* 前景高亮 */
#define CR_CUI_BACK_BLUE    0x0010  /* 背景蓝色 */
#define CR_CUI_BACK_GREEN   0x0020  /* 背景绿色 */
#define CR_CUI_BACK_RED     0x0040  /* 背景红色 */
#define CR_CUI_BACK_LIGHT   0x0080  /* 背景高亮 */
#define CR_CUI_REVERSE      0x0100  /* 反转显示 */
#define CR_CUI_UNDERSCORE   0x0200  /* 带下滑线 */

/* 字符颜色控制 */
CR_API bool_t   cui_set_color (int16u  color);
CR_API bool_t   cui_get_color (int16u *color);
CR_API int16u   cui_make_attr (int16u  color, uint_t attrib);

/* 输出位置控制 */
CR_API bool_t   cui_set_xy (int16u  x, int16u  y);
CR_API bool_t   cui_get_xy (int16u *x, int16u *y);

#endif  /* !__CR_MSGLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
