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
#define __CR_APPLIB_H__ 0xB3CF14EBUL

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

/*****************************************************************************/
/*                              系统 GUI 相关                                */
/*****************************************************************************/

/* 应用程序窗口风格 */
#define CR_WSTYLE_MAXVV         2
#define CR_WSTYLE_BLANK         0       /* 固定大小无标题窗口 */
#define CR_WSTYLE_FIXED         1       /* 固定大小有标题窗口 */
#define CR_WSTYLE_NORMAL        2       /* 可变大小有标题窗口 */
#define CR_WSTYLE_TOPMOST   0x8000      /* 是否为最上层的窗口 */

/* 应用程序窗口创建 */
CR_API hwnd_t   window_open (void_t *instance, void_t *msg_proc,
                             sint_t x, sint_t y, uint_t w, uint_t h,
                             const ansi_t *title, const ansi_t *name,
                             const ansi_t *icon, uint_t style);
/* 应用程序窗口销毁 */
CR_API bool_t   window_kill (hwnd_t hwnd, void_t *instance,
                             const ansi_t *name);
/* 应用程序按键输入 */
CR_API bool_t   key_input_test (uint_t vkey);
CR_API bool_t   key_input_click (uint_t vkey);
CR_API uint_t   key_input_read (bool_t async);

/* 应用程序鼠标相关 */
CR_API void_t   mouse_toggle (bool_t show);
CR_API bool_t   mouse_set_pos (sint_t x, sint_t y);
CR_API bool_t   mouse_get_pos (sint_t *x, sint_t *y);

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
