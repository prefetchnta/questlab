/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-06-23  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack DirectShow 多媒体函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_XMM_DSHOW_CPP__
#define __CR_XMM_DSHOW_CPP__ 0xF4D511C0UL

#include "extz.h"
#include "fileio.h"
#include "memlib.h"
#include "strlib.h"

/* 根据编译器取舍 */
#if     !defined(_CR_CC_MSC_)
    #define _CR_NO_SDK_DSHOW_
#elif   (_CR_CC_VER_ < 1310)
    #define _CR_NO_SDK_DSHOW_
#elif   defined(_CR_OS_WINCE_) && (_WIN32_WCE < 0x0500)
    #define _CR_NO_SDK_DSHOW_
#else
    #undef  _CR_NO_SDK_DSHOW_
#endif

#if !defined(_CR_NO_SDK_DSHOW_)

#include <dshow.h>

/* DirectShow 导入库 */
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "strmiids.lib")
    #if defined(_CR_OS_WINCE_)
        #pragma comment (lib, "strmbase.lib")
    #endif
#endif

/* 内部数据结构 */
typedef struct
{
        /* 窗口句柄 */
        HWND    hwnd;

        /* 显示模式 */
        uint_t  mode;

        /* DirectShow */
        IBasicAudio*    BA;
        IBasicVideo*    BV;
        IVideoWindow*   VW;
        IGraphBuilder*  GB;
        IMediaControl*  MC;
        IMediaPosition* MP;

} sDSHOW;

/*
=======================================
    DSHOW 加载文件A
=======================================
*/
CR_API dshow_t
dshow_loadA (
  __CR_IN__ const ansi_t*   name
    )
{
    wide_t* wname;
    dshow_t dshow;

    wname = local_to_utf16(CR_LOCAL, name);
    if (wname == NULL) {
        err_set(__CR_XMM_DSHOW_CPP__, CR_NULL,
                "dshow_loadA()", "local_to_utf16() failure");
        return (NULL);
    }
    dshow = dshow_loadW(wname);
    mem_free(wname);
    if (dshow == NULL) {
        err_set(__CR_XMM_DSHOW_CPP__, CR_NULL,
                "dshow_loadA()", "dshow_loadW() failure");
        return (NULL);
    }
    return (dshow);
}

/*
=======================================
    DSHOW 加载文件W
=======================================
*/
CR_API dshow_t
dshow_loadW (
  __CR_IN__ const wide_t*   name
    )
{
    sDSHOW* rett;
    HRESULT result;

    /* 初始动作 */
    if (!file_existW(name)) {
        err_set(__CR_XMM_DSHOW_CPP__, FALSE,
                "dshow_loadW()", "file_existW() failure");
        return (NULL);
    }
    rett = struct_new(sDSHOW);
    if (rett == NULL) {
        err_set(__CR_XMM_DSHOW_CPP__, CR_NULL,
                "dshow_loadW()", "struct_new() failure");
        return (NULL);
    }
    struct_zero(rett, sDSHOW);

    /* 获取所有用到的 DSHOW 接口 */
    result = CoCreateInstance(CLSID_FilterGraph, NULL,
                    CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
                            (void_t**)(&rett->GB));
    if (result != S_OK) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_loadW()", "CoCreateInstance() failure");
        goto _failure;
    }
    result = rett->GB->RenderFile(name, NULL);
    if (result != S_OK) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_loadW()", "IGB::RenderFile() failure");
        goto _failure;
    }
    result = rett->GB->QueryInterface(IID_IMediaControl,
                    (void_t**)(&rett->MC));
    if (result != S_OK) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_loadW()", "IGB::QueryInterface() failure");
        goto _failure;
    }
    result = rett->GB->QueryInterface(IID_IMediaPosition,
                    (void_t**)(&rett->MP));
    if (result != S_OK) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_loadW()", "IGB::QueryInterface() failure");
        goto _failure;
    }
    result = rett->GB->QueryInterface(IID_IVideoWindow,
                    (void_t**)(&rett->VW));
    if (result != S_OK) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_loadW()", "IGB::QueryInterface() failure");
        goto _failure;
    }
    result = rett->GB->QueryInterface(IID_IBasicVideo,
                    (void_t**)(&rett->BV));
    if (result != S_OK) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_loadW()", "IGB::QueryInterface() failure");
        goto _failure;
    }

    /* 如果是音频媒体释放视频接口 */
    if (dshow_is_audio((dshow_t)rett)) {
        rett->VW->put_Visible(OAFALSE);
        rett->VW->put_Owner(NULL);
        SAFE_XREL(rett->BV)
        SAFE_XREL(rett->VW)
    }
    result = rett->GB->QueryInterface(IID_IBasicAudio,
                    (void_t**)(&rett->BA));
    if (result != S_OK) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_loadW()", "IGB::QueryInterface() failure");
        goto _failure;
    }
    rett->MP->put_CurrentPosition(0);
    return ((dshow_t)rett);

_failure:
    dshow_free((dshow_t)rett);
    return (NULL);
}

/*
=======================================
    DSHOW 释放文件
=======================================
*/
CR_API void_t
dshow_free (
  __CR_IN__ dshow_t dshow
    )
{
    sDSHOW* real;

    real = (sDSHOW*)dshow;
    if (real->VW != NULL) {
        real->VW->put_Visible(OAFALSE);
        real->VW->put_Owner(NULL);
    }
    TRY_XREL(real->MC)
    TRY_XREL(real->MP)
    TRY_XREL(real->BV)
    TRY_XREL(real->BA)
    TRY_XREL(real->VW)
    TRY_XREL(real->GB)
    mem_free(dshow);
}

/*
=======================================
    DSHOW 是否是音频文件
=======================================
*/
CR_API bool_t
dshow_is_audio (
  __CR_IN__ dshow_t dshow
    )
{
    return (!dshow_is_video(dshow));
}

/*
=======================================
    DSHOW 是否是视频文件
=======================================
*/
CR_API bool_t
dshow_is_video (
  __CR_IN__ dshow_t dshow
    )
{
    LONG    visb;
    sDSHOW* real;

    real = (sDSHOW*)dshow;
    if (real->VW == NULL)
        return (FALSE);
    if (real->BV == NULL)
        return (FALSE);
    return (SUCCEEDED(real->VW->get_Visible(&visb)));
}

/*
=======================================
    DSHOW 开始播放
=======================================
*/
CR_API bool_t
dshow_play (
  __CR_IN__ dshow_t dshow
    )
{
    sDSHOW* real;
    HRESULT result;

    real = (sDSHOW*)dshow;
    result = real->MC->Run();
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_play()", "IMC::Run() failure");
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    DSHOW 暂停播放
=======================================
*/
CR_API bool_t
dshow_pause (
  __CR_IN__ dshow_t dshow
    )
{
    sDSHOW* real;
    HRESULT result;

    real = (sDSHOW*)dshow;
    result = real->MC->Pause();
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_pause()", "IMC::Pause() failure");
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    DSHOW 停止播放
=======================================
*/
CR_API bool_t
dshow_stop (
  __CR_IN__ dshow_t dshow
    )
{
    sDSHOW* real;
    HRESULT result;

    real = (sDSHOW*)dshow;
    result = real->MC->Stop();
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_stop()", "IMC::Stop() failure");
        return (FALSE);
    }
    real->MP->put_CurrentPosition(0);
    return (TRUE);
}

/*
=======================================
    DSHOW 重头播放
=======================================
*/
CR_API bool_t
dshow_replay (
  __CR_IN__ dshow_t dshow
    )
{
    sDSHOW* real;
    HRESULT result;

    real = (sDSHOW*)dshow;
    result = real->MP->put_CurrentPosition(0);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_replay()", "IMP::put_CurrentPosition() failure");
        return (FALSE);
    }
    result = real->MC->Run();
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_replay()", "IMC::Run() failure");
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    DSHOW 是否已经结束
=======================================
*/
CR_API bool_t
dshow_is_over (
  __CR_IN__ dshow_t dshow,
  __CR_OT__ bool_t* over
    )
{
    sDSHOW* real;
    HRESULT result;
    REFTIME stop_time;
    REFTIME curt_time;

    real = (sDSHOW*)dshow;
    result = real->MP->get_Duration(&stop_time);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_is_over()", "IMP::get_Duration() failure");
        return (FALSE);
    }
    result = real->MP->get_CurrentPosition(&curt_time);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_is_over()", "IMP::get_CurrentPosition() failure");
        return (FALSE);
    }
    *over = (curt_time < stop_time) ? FALSE : TRUE;
    return (TRUE);
}

/*
=======================================
    DSHOW 获取视频宽高
=======================================
*/
CR_API bool_t
dshow_video_size (
  __CR_IN__ dshow_t dshow,
  __CR_OT__ uint_t* width,
  __CR_OT__ uint_t* height
    )
{
    sDSHOW* real;
    LONG    ww, hh;
    HRESULT result;

    real = (sDSHOW*)dshow;
    if (real->BV == NULL) {
        err_set(__CR_XMM_DSHOW_CPP__, CR_NULL,
                "dshow_video_size()", "invalid param: dshow");
        return (FALSE);
    }
    result = real->BV->GetVideoSize(&ww, &hh);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_video_size()", "IBV::GetVideoSize() failure");
        return (FALSE);
    }
    if (width  != NULL) *width  = (uint_t)ww;
    if (height != NULL) *height = (uint_t)hh;
    return (TRUE);
}

/*
=======================================
    DSHOW 设置视频模式
=======================================
*/
CR_API bool_t
dshow_video_mode (
  __CR_IN__ dshow_t dshow,
  __CR_IN__ uint_t  mode
    )
{
    sDSHOW* real;
    RECT    rect;
    LONG    x1, y1;
    LONG    dw, dh;
    LONG    sw, sh;
    uint_t  ww, hh;
    HRESULT result;

    /* 过滤参数 */
    if (mode > XMM_VIDEO_NATIVE) {
        err_set(__CR_XMM_DSHOW_CPP__, mode,
                "dshow_video_mode()", "invalid param: mode");
        return (FALSE);
    }
    real = (sDSHOW*)dshow;
    if (real->VW == NULL) {
        err_set(__CR_XMM_DSHOW_CPP__, CR_NULL,
                "dshow_video_mode()", "invalid param: dshow");
        return (FALSE);
    }
    if (real->hwnd == NULL) {
        real->VW->put_Left(0);
        real->VW->put_Top(0);
        real->VW->put_Width(0);
        real->VW->put_Height(0);
        err_set(__CR_XMM_DSHOW_CPP__, CR_ERROR,
                "dshow_video_mode()", "invalid param: dshow");
        return (FALSE);
    }
    if (mode == XMM_VIDEO_FULLSCREEN)
    {
        /* 全屏播放 */
        result = real->VW->put_FullScreenMode(OATRUE);
        if (FAILED(result)) {
            err_set(__CR_XMM_DSHOW_CPP__, result,
                    "dshow_video_mode()", "IVW::put_FullScreenMode() failure");
            return (FALSE);
        }
        real->mode = mode;
        return (TRUE);
    }

    /* 需要计算窗口大小 */
    result = real->VW->put_FullScreenMode(OAFALSE);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_video_mode()", "IVW::put_FullScreenMode() failure");
        return (FALSE);
    }
    if (!GetClientRect(real->hwnd, &rect)) {
        err_set(__CR_XMM_DSHOW_CPP__, GetLastError(),
                "dshow_video_mode()", "GetClientRect() failure");
        return (FALSE);
    }
    if (!dshow_video_size(dshow, &ww, &hh)) {
        err_set(__CR_XMM_DSHOW_CPP__, FALSE,
                "dshow_video_mode()", "dshow_video_size() failure");
        return (FALSE);
    }
    sw = (LONG)ww;
    sh = (LONG)hh;
    if (mode == XMM_VIDEO_FIT || mode == XMM_VIDEO_NATIVE)
    {
        if (mode == XMM_VIDEO_NATIVE &&
            rect.right >= sw && rect.bottom >= sh)
        {
            /* 居中窗口播放 */
            x1 = (rect.right  - sw) / 2;
            y1 = (rect.bottom - sh) / 2;
            dw = sw;
            dh = sh;
        }
        else
        {
            /* 缩放到窗口的一边 */
            if (sw * rect.bottom > rect.right * sh)
            {
                dw = rect.right;
                dh = (LONG)((fp32_t)dw / sw * sh);
                x1 = 0;
                y1 = (rect.bottom - dh) / 2;
            }
            else
            if (sw * rect.bottom < rect.right * sh)
            {
                dh = rect.bottom;
                dw = (LONG)((fp32_t)dh / sh * sw);
                x1 = (rect.right - dw) / 2;
                y1 = 0;
            }
            else
            {
                dw = rect.right;
                dh = rect.bottom;
                x1 = 0;
                y1 = 0;
            }
        }
        real->VW->put_Left(x1);
        real->VW->put_Top(y1);
        real->VW->put_Width(dw);
        real->VW->put_Height(dh);
    }
    else
    {
        /* 缩放到窗口大小 */
        real->VW->put_Left(0);
        real->VW->put_Top(0);
        real->VW->put_Width(rect.right);
        real->VW->put_Height(rect.bottom);
    }
    real->mode = mode;
    return (TRUE);
}

/*
=======================================
    DSHOW 设置视频窗口
=======================================
*/
CR_API bool_t
dshow_video_setwin (
  __CR_IN__ dshow_t dshow,
  __CR_IN__ hwnd_t  hwnd,
  __CR_IN__ uint_t  mode
    )
{
    sDSHOW* real;
    HRESULT result;

    real = (sDSHOW*)dshow;
    if (real->VW == NULL) {
        err_set(__CR_XMM_DSHOW_CPP__, CR_NULL,
                "dshow_video_setwin()", "invalid param: dshow");
        return (FALSE);
    }
    result = real->VW->put_Owner((OAHWND)hwnd);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_video_setwin()", "IVW::put_Owner() failure");
        return (FALSE);
    }
    result = real->VW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS |
                                       WS_CLIPCHILDREN);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_video_setwin()", "IVW::put_WindowStyle() failure");
        goto _failure;
    }
    real->hwnd = (HWND)hwnd;
    if (!dshow_video_mode(dshow, mode)) {
        err_set(__CR_XMM_DSHOW_CPP__, FALSE,
                "dshow_video_setwin()", "dshow_video_mode() failure");
        goto _failure;
    }
    return (TRUE);

_failure:
    real->VW->put_Visible(OAFALSE);
    real->VW->put_Owner(NULL);
    real->hwnd = NULL;
    return (FALSE);
}

/*
=======================================
    DSHOW 设置音量
=======================================
*/
CR_API bool_t
dshow_set_volume (
  __CR_IN__ dshow_t dshow,
  __CR_IN__ sint_t  volume
    )
{
    sDSHOW* real;
    LONG    value;
    HRESULT result;

    real = (sDSHOW*)dshow;
    if (volume < DSHOW_AV_MIN)
        volume = DSHOW_AV_MIN;
    else
    if (volume > DSHOW_AV_MAX)
        volume = DSHOW_AV_MAX;
    value = (LONG)(volume - DSHOW_AV_MAX);
    result = real->BA->put_Volume(value);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_set_volume()", "IBA::put_Volume() failure");
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    DSHOW 获取音量
=======================================
*/
CR_API bool_t
dshow_get_volume (
  __CR_IN__ dshow_t dshow,
  __CR_OT__ sint_t* volume
    )
{
    sDSHOW* real;
    LONG    value;
    HRESULT result;

    real = (sDSHOW*)dshow;
    result = real->BA->get_Volume(&value);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_get_volume()", "IBA::get_Volume() failure");
        return (FALSE);
    }
    if (volume != NULL)
       *volume = (sint_t)(value + DSHOW_AV_MAX);
    return (TRUE);
}

/*
=======================================
    DSHOW 设置平衡
=======================================
*/
CR_API bool_t
dshow_set_balance (
  __CR_IN__ dshow_t dshow,
  __CR_IN__ sint_t  balance
    )
{
    sDSHOW* real;
    LONG    value;
    HRESULT result;

    real = (sDSHOW*)dshow;
    if (balance < DSHOW_AB_MIN)
        balance = DSHOW_AB_MIN;
    else
    if (balance > DSHOW_AB_MAX)
        balance = DSHOW_AB_MAX;
    value = (LONG)balance;
    result = real->BA->put_Balance(value);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_set_balance()", "IBA::put_Balance() failure");
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    DSHOW 获取平衡
=======================================
*/
CR_API bool_t
dshow_get_balance (
  __CR_IN__ dshow_t dshow,
  __CR_OT__ sint_t* balance
    )
{
    sDSHOW* real;
    LONG    value;
    HRESULT result;

    real = (sDSHOW*)dshow;
    result = real->BA->get_Balance(&value);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_get_balance()", "IBA::get_Balance() failure");
        return (FALSE);
    }
    if (balance != NULL)
       *balance = (sint_t)value;
    return (TRUE);
}

/*
=======================================
    DSHOW 设置位置
=======================================
*/
CR_API bool_t
dshow_set_position (
  __CR_IN__ dshow_t dshow,
  __CR_IN__ int64u  curt
    )
{
    sDSHOW* real;
    HRESULT result;
    REFTIME curt_time;

    real = (sDSHOW*)dshow;
    curt_time = (REFTIME)curt;
    curt_time /= 1000.0;
    result = real->MP->put_CurrentPosition(curt_time);
    if (FAILED(result)) {
        err_set(__CR_XMM_DSHOW_CPP__, result,
                "dshow_set_position()", "IMP::put_CurrentPosition() failure");
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    DSHOW 获取位置
=======================================
*/
CR_API bool_t
dshow_get_position (
  __CR_IN__ dshow_t dshow,
  __CR_OT__ int64u* curt,
  __CR_OT__ int64u* total
    )
{
    sDSHOW* real;
    HRESULT result;
    REFTIME stop_time;
    REFTIME curt_time;

    real = (sDSHOW*)dshow;
    if (total != NULL) {
        result = real->MP->get_Duration(&stop_time);
        if (FAILED(result)) {
            err_set(__CR_XMM_DSHOW_CPP__, result,
                    "dshow_get_position()", "IMP::get_Duration() failure");
            return (FALSE);
        }
        *total = (int64s)(stop_time * 1000.0);
    }
    if (curt != NULL) {
        result = real->MP->get_CurrentPosition(&curt_time);
        if (FAILED(result)) {
            err_set(__CR_XMM_DSHOW_CPP__, result,
                    "dshow_get_position()",
                    "IMP::get_CurrentPosition() failure");
            return (FALSE);
        }
        *curt = (int64s)(curt_time * 1000.0);
    }
    return (TRUE);
}

#endif  /* !_CR_NO_SDK_DSHOW_ */

#endif  /* !__CR_XMM_DSHOW_CPP__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
