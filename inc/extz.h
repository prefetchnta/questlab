/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-05-08  */
/*     #######          ###    ###      [EXTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
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

#include "script.h"

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

/* Windows MCI 媒体播放 */
CR_API bool_t   mci_cd_play (uint_t track);
CR_API bool_t   mci_cd_stop (void_t);

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

/*****************************************************************************/
/*                               外部脚本接口                                */
/*****************************************************************************/

/* 脚本对象类型 */
typedef void_t*     slua_t;
typedef void_t*     pndr_t;

/***** Lua51 脚本接口 *****/
CR_API void_t   scr_lua51_init (void_t);
CR_API slua_t   scr_lua51_open (const ansi_t *name);
CR_API void_t   scr_lua51_close (slua_t slua);
CR_API bool_t   scr_lua51_do_file (slua_t slua, const ansi_t *name);
CR_API bool_t   scr_lua51_do_text (slua_t slua, const ansi_t *text);
CR_API bool_t   scr_lua51_get_var (slua_t slua, const ansi_t *name,
                                   sUVAR *uvar);
CR_API bool_t   scr_lua51_set_var (slua_t slua, const ansi_t *name,
                                   const sUVAR *uvar);
CR_API void_t   scr_lua51_set_func (slua_t slua, const ansi_t *name,
                                    uv_func_t func);
CR_API uint_t   scr_lua51_param (void_t *state, sUVAR *param, uint_t count);
CR_API uint_t   scr_lua51_return (void_t *state, const sUVAR *result,
                                  uint_t count);
CR_API void_t   scr_lua51_error (void_t *state, const ansi_t *message);

/***** Pandora 脚本接口 *****/
CR_API void_t   scr_pndr_init (void_t);
CR_API pndr_t   scr_pndr_open (const ansi_t *name);
CR_API void_t   scr_pndr_close (pndr_t pndr);
CR_API bool_t   scr_pndr_do_file (pndr_t pndr, const ansi_t *name);
CR_API bool_t   scr_pndr_do_text (pndr_t pndr, const ansi_t *text);
CR_API bool_t   scr_pndr_get_var (pndr_t pndr, const ansi_t *name,
                                  sUVAR *uvar);
CR_API bool_t   scr_pndr_set_var (pndr_t pndr, const ansi_t *name,
                                  const sUVAR *uvar);

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
