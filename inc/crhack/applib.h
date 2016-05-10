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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 应用程序头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_APPLIB_H__
#define __CR_APPLIB_H__

#include "defs.h"

/*****************************************************************************/
/*                               应用程序相关                                */
/*****************************************************************************/

/* 应用程序退出回调 */
typedef void_t  (*quit_t) (void_t);

CR_API void_t   quit_now (void_t);
CR_API void_t   quit_set (quit_t func);

#define CR_APP_GUI  0   /* GUI 程序 */
#define CR_APP_CUI  1   /* CUI 程序 */
CR_API bool_t   set_app_type (uint_t type);
CR_API bool_t   set_app_mode (bool_t full);
CR_API void_t   set_gui_hwnd (hwnd_t hwnd);

/* 获取系统本地编码值 */
CR_API uint_t   get_sys_codepage (void_t);

/* 设置系统本地编码值 (部分支持) */
CR_API void_t   set_sys_codepage (uint_t cpage);

/* 编码转换函数类型 */
typedef void_t* (*cr_acp2uni_t) (uint_t, const ansi_t*, leng_t*, bool_t);
typedef ansi_t* (*cr_uni2acp_t) (uint_t, const void_t*, leng_t*, bool_t);

/* 设置外挂编码转换函数 */
CR_API void_t   set_str_acp2uni (cr_acp2uni_t func);
CR_API void_t   set_str_uni2acp (cr_uni2acp_t func);

/*****************************************************************************/
/*                                系统定时器                                 */
/*****************************************************************************/

/* 定时器类型 */
typedef void_t*     xtime_t;

/* 定时器生成释放 */
CR_API xtime_t  timer_new (void_t);
CR_API void_t   timer_del (xtime_t timer);
CR_API int32u   timer_get32 (void_t);
CR_API int64u   timer_get64 (void_t);

/* 定时器获取时差 */
CR_API void_t   timer_set_base  (xtime_t timer);
CR_API fp32_t   timer_get_delta (xtime_t timer);

/*****************************************************************************/
/*                                消息提示框                                 */
/*****************************************************************************/

/* 自定义提示 */
#define CR_STOP     0   /* 严重错误 */
#define CR_WARN     1   /* 警告错误 */
#define CR_DONE     2   /* 成功提示 */
#define CR_ASKU     3   /* 询问提示 */
typedef bool_t  (*msgboxA_t) (const ansi_t*, const ansi_t*, uint_t);
typedef bool_t  (*msgboxW_t) (const wide_t*, const wide_t*, uint_t);

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

#endif  /* !__CR_APPLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
