/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-05-08  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 扩展功能头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_EXTZ_H__
#define __CR_EXTZ_H__

#include "defs.h"

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

/*****************************************************************************/
/*                                多媒体接口                                 */
/*****************************************************************************/

/* 多媒体播放标志 */
#define XMM_PLAY_LOOP   0x00000001UL    /* 循环播放 */
#define XMM_PLAY_SYNC   0x00000002UL    /* 同步播放 */

/* 视频窗口的模式 */
#define XMM_VIDEO_FIT           0   /* 匹配到播放窗口 */
#define XMM_VIDEO_STRETCH       1   /* 拉伸到播放窗口 */
#define XMM_VIDEO_FULLSCREEN    2   /* 全屏播放此视频 */
#define XMM_VIDEO_NATIVE        3   /* 使用此视频大小 */

/* Windows COM 模块 */
CR_API bool_t   com_init (void_t);
CR_API void_t   com_kill (void_t);

/* Windows API WAV 播放 */
CR_API bool_t   win_wav_stop  (int32u flags);
CR_API bool_t   win_wav_play  (const void_t *data, int32u flags);
CR_API bool_t   win_wav_playA (const ansi_t *name, int32u flags);
CR_API bool_t   win_wav_playW (const wide_t *name, int32u flags);

/* DirectShow 媒体播放 */
typedef void_t*     dshow_t;

/* DirectShow 音量范围 */
#define DSHOW_AV_MIN      0     /* 最小音量 */
#define DSHOW_AV_MAX    10000   /* 最大音量 */

/* DirectShow 平衡参数 */
#define DSHOW_AB_MIN   -10000   /* 右声道最大衰减 */
#define DSHOW_AB_MAX    10000   /* 左声道最大衰减 */

CR_API dshow_t  dshow_loadA (const ansi_t *name);
CR_API dshow_t  dshow_loadW (const wide_t *name);
CR_API void_t   dshow_free (dshow_t dshow);
CR_API bool_t   dshow_is_audio (dshow_t dshow);
CR_API bool_t   dshow_is_video (dshow_t dshow);
CR_API bool_t   dshow_play (dshow_t dshow);
CR_API bool_t   dshow_pause (dshow_t dshow);
CR_API bool_t   dshow_stop (dshow_t dshow);
CR_API bool_t   dshow_replay (dshow_t dshow);
CR_API bool_t   dshow_is_over (dshow_t dshow, bool_t *over);
CR_API bool_t   dshow_video_size (dshow_t dshow, uint_t *width,
                                  uint_t *height);
CR_API bool_t   dshow_video_mode (dshow_t dshow, uint_t mode);
CR_API bool_t   dshow_video_setwin (dshow_t dshow, hwnd_t hwnd,
                                    uint_t mode);
CR_API bool_t   dshow_set_volume (dshow_t dshow, sint_t volume);
CR_API bool_t   dshow_get_volume (dshow_t dshow, sint_t *volume);
CR_API bool_t   dshow_set_balance (dshow_t dshow, sint_t balance);
CR_API bool_t   dshow_get_balance (dshow_t dshow, sint_t *balance);
CR_API bool_t   dshow_set_position (dshow_t dshow, int64u curt);
CR_API bool_t   dshow_get_position (dshow_t dshow, int64u *curt,
                                    int64u *total);
/* 摄像头相关 */
CR_API uint_t   dshow_cam_list (const ansi_t **name, uint_t max_num);

/*****************************************************************************/
/*                               网络扩展接口                                */
/*****************************************************************************/

typedef void_t*     wftp_t;

/***** FTP 操作接口 *****/
CR_API void_t   win_ftp_close (wftp_t ftps);
CR_API wftp_t   win_ftp_openA (const ansi_t *remote, const ansi_t *user,
                               const ansi_t *password);
CR_API wftp_t   win_ftp_openW (const wide_t *remote, const wide_t *user,
                               const wide_t *password);
CR_API bool_t   win_ftp_getfileA (wftp_t ftps, const ansi_t *root,
                        const ansi_t *ftp_file, const ansi_t *loc_file);
CR_API bool_t   win_ftp_getfileW (wftp_t ftps, const wide_t *root,
                        const wide_t *ftp_file, const wide_t *loc_file);
CR_API bool_t   win_ftp_putfileA (wftp_t ftps, const ansi_t *root,
                        const ansi_t *ftp_file, const ansi_t *loc_file);
CR_API bool_t   win_ftp_putfileW (wftp_t ftps, const wide_t *root,
                        const wide_t *ftp_file, const wide_t *loc_file);

/***** HTTP 操作接口 *****/
CR_API bool_t   win_http_getfileA (const ansi_t *url, const ansi_t *name);
CR_API bool_t   win_http_getfileW (const wide_t *url, const wide_t *name);

/*****************************************************************************/
/*                               系统相关杂项                                */
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
/* 应用程序输入相关 */
CR_API bool_t   key_input_test (uint_t vkey);
CR_API bool_t   key_input_click (uint_t vkey);
CR_API void_t   mouse_toggle (bool_t show);
CR_API bool_t   mouse_set_pos (sint_t x, sint_t y);
CR_API bool_t   mouse_get_pos (sint_t *x, sint_t *y);
CR_API int32u   input_tick32 (void_t);

/* 显示分辨率的更改 */
typedef void_t*     dispmode_t;

CR_API dispmode_t   disp_mode_get (void_t);
CR_API bool_t       disp_mode_set (dispmode_t mode, uint_t width,
                                   uint_t height, uint_t bpp);
CR_API void_t       disp_mode_del (dispmode_t mode);

/* 调用 Shell 命令 */
CR_API bool_t   shell_cmd (const ansi_t *cmd, sint_t *retc);

#endif  /* !__CR_EXTZ_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
