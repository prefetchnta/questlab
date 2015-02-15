/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-12-20  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>> CrHack DirectShow FMTZ 插件接口实现 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "extz.h"
#include "fmtint.h"
#include "strlib.h"
#include "fmtz/dshow.h"

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iXMMEDIA    xmms;

        /* 个性部分 */
        dshow_t     m_dshw;

} iXMM_DSHW;

/* COM 是否已经初始化 */
static bool_t   s_init = FALSE;
static bool_t   s_xcom = FALSE;

/*
=======================================
    初始化 COM 系统
=======================================
*/
CR_API bool_t
dshow_init (void_t)
{
    if (!s_init) {
        s_xcom = com_init();
        s_init = TRUE;
    }
    return (TRUE);
}

/*
=======================================
    释放 COM 系统
=======================================
*/
CR_API void_t
dshow_kill (void_t)
{
    if (s_init) {
        if (s_xcom)
            com_kill();
        s_init = FALSE;
    }
}

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iXMM_DSHW_release (
  __CR_IN__ iXMMEDIA*   that
    )
{
    iXMM_DSHW*  real;

    real = (iXMM_DSHW*)that;
    dshow_free(real->m_dshw);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iXMM_DSHW_getMore (
  __CR_IN__ iXMMEDIA*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iXMMEDIA::DSHOW") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    播放媒体
---------------------------------------
*/
static bool_t
iXMM_DSHW_play (
  __CR_IN__ iXMMEDIA*   that
    )
{
    iXMM_DSHW*  real;

    real = (iXMM_DSHW*)that;
    return (dshow_play(real->m_dshw));
}

/*
---------------------------------------
    暂停播放
---------------------------------------
*/
static bool_t
iXMM_DSHW_pause (
  __CR_IN__ iXMMEDIA*   that
    )
{
    iXMM_DSHW*  real;

    real = (iXMM_DSHW*)that;
    return (dshow_pause(real->m_dshw));
}

/*
---------------------------------------
    停止播放
---------------------------------------
*/
static bool_t
iXMM_DSHW_stop (
  __CR_IN__ iXMMEDIA*   that
    )
{
    iXMM_DSHW*  real;

    real = (iXMM_DSHW*)that;
    return (dshow_stop(real->m_dshw));
}

/*
---------------------------------------
    设置当前位置
---------------------------------------
*/
static bool_t
iXMM_DSHW_set_pos (
  __CR_IN__ iXMMEDIA*   that,
  __CR_IN__ int64u      curt
    )
{
    iXMM_DSHW*  real;

    real = (iXMM_DSHW*)that;
    return (dshow_set_position(real->m_dshw, curt));
}

/*
---------------------------------------
    获取当前位置
---------------------------------------
*/
static bool_t
iXMM_DSHW_get_pos (
  __CR_IN__ iXMMEDIA*   that,
  __CR_OT__ int64u*     curt,
  __CR_OT__ int64u*     total
    )
{
    iXMM_DSHW*  real;

    real = (iXMM_DSHW*)that;
    return (dshow_get_position(real->m_dshw, curt, total));
}

/*
---------------------------------------
    播放是否结束
---------------------------------------
*/
static bool_t
iXMM_DSHW_is_over (
  __CR_IN__ iXMMEDIA*   that,
  __CR_OT__ bool_t*     over
    )
{
    iXMM_DSHW*  real;

    real = (iXMM_DSHW*)that;
    return (dshow_is_over(real->m_dshw, over));
}

/*
---------------------------------------
    设置音量
---------------------------------------
*/
static bool_t
iXMM_DSHW_set_volume (
  __CR_IO__ iXMMEDIA*   that,
  __CR_IN__ sint_t      percent
    )
{
    sint_t      vols;
    iXMM_DSHW*  real;

    real = (iXMM_DSHW*)that;
    if (percent < 0)
        percent = 0;
    else
    if (percent > 100)
        percent = 100;
    vols = (DSHOW_AV_MAX * percent) / 100;
    if (!dshow_set_volume(real->m_dshw, vols))
        return (FALSE);
    that->__volume__ = percent;
    return (TRUE);
}

/*
---------------------------------------
    获取音量
---------------------------------------
*/
static bool_t
iXMM_DSHW_get_volume (
  __CR_IO__ iXMMEDIA*   that,
  __CR_OT__ sint_t*     percent
    )
{
    sint_t      vols;
    iXMM_DSHW*  real;

    real = (iXMM_DSHW*)that;
    if (!dshow_get_volume(real->m_dshw, &vols))
        return (FALSE);
    that->__volume__ = (vols * 100) / DSHOW_AV_MAX;
    if (percent != NULL)
        *percent = that->__volume__;
    return (TRUE);
}

/*
---------------------------------------
    是否是视频
---------------------------------------
*/
static bool_t
iXMM_DSHW_is_video (
  __CR_IN__ iXMMEDIA*   that
    )
{
    iXMM_DSHW*  real = (iXMM_DSHW*)that;

    return (dshow_is_video(real->m_dshw));
}

/*
---------------------------------------
    获取视频大小
---------------------------------------
*/
static bool_t
iXMM_DSHW_video_size (
  __CR_IN__ iXMMEDIA*   that,
  __CR_OT__ uint_t*     width,
  __CR_OT__ uint_t*     height
    )
{
    iXMM_DSHW*  real;

    real = (iXMM_DSHW*)that;
    return (dshow_video_size(real->m_dshw, width, height));
}

/*
---------------------------------------
    设置视频窗口
---------------------------------------
*/
static bool_t
iXMM_DSHW_video_setwin (
  __CR_IO__ iXMMEDIA*   that,
  __CR_IN__ hwnd_t      hwnd,
  __CR_IN__ bool_t      full
    )
{
    uint_t      mode;
    iXMM_DSHW*  real;

    real = (iXMM_DSHW*)that;
    mode = full ? XMM_VIDEO_FULLSCREEN : XMM_VIDEO_NATIVE;
    if (!dshow_video_setwin(real->m_dshw, hwnd, mode))
        return (FALSE);
    that->__hwnd__ = hwnd;
    that->__full__ = full;
    return (TRUE);
}

/* 接口虚函数表 */
static const iXMMEDIA_vtbl _rom_ s_xmms_vtbl =
{
    iXMM_DSHW_release, iXMM_DSHW_getMore,
    iXMM_DSHW_play, iXMM_DSHW_pause, iXMM_DSHW_stop,
    iXMM_DSHW_set_pos, iXMM_DSHW_get_pos, iXMM_DSHW_is_over,
    iXMM_DSHW_set_volume, iXMM_DSHW_get_volume, iXMM_DSHW_is_video,
    iXMM_DSHW_video_size, iXMM_DSHW_video_setwin,
};

/*
=======================================
    DirectShow 文件读取
=======================================
*/
CR_API sFMT_PRT*
load_dshow (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    dshow_t     dshw;
    sFMT_PRT*   rett;
    iXMM_DSHW*  port;

    /* 必须先初始化 */
    if (!s_init)
        return (NULL);

    /* 只支持磁盘文件 */
    if (param->type != CR_LDR_ANSI &&
        param->type != CR_LDR_WIDE)
        return (NULL);

    /* 加载文件尝试 */
    if (param->type == CR_LDR_ANSI)
        dshw = dshow_loadA(param->name.ansi);
    else
        dshw = dshow_loadW(param->name.wide);
    if (dshw == NULL)
        return (NULL);

    /* 生成媒体播放接口对象 */
    port = struct_new(iXMM_DSHW);
    if (port == NULL) {
        dshow_free(dshw);
        return (NULL);
    }
    struct_zero(port, iXMM_DSHW);
    port->m_dshw = dshw;
    port->xmms.__volume__ = 100;
    port->xmms.__vptr__ = &s_xmms_vtbl;

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iXMM_DSHW_release((iXMMEDIA*)port);
        return (NULL);
    }
    CR_NOUSE(datin);
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iXMMEDIA";
    rett->infor = "Media file supported by DirectShow";
    return (rett);
}

/*
---------------------------------------
    文件格式加载接口
---------------------------------------
*/
static sFMTZ*
engine_dshow_load (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    if (!(engine->mask & CR_FMTZ_MASK_XMM))
        return (NULL);
    return ((sFMTZ*)load_dshow(NULL, loader));
}

/*
---------------------------------------
    引擎插件释放回调
---------------------------------------
*/
static void_t
engine_dshow_free (
  __CR_IO__ sENGINE*    engine
    )
{
    dshow_kill();
    CR_NOUSE(engine);
}

/*
=======================================
    获取引擎插件接口
=======================================
*/
CR_API sENGINE*
engine_dshow (void_t)
{
    sENGINE*    engine;

    engine = engine_init(NULL, NULL, NULL, NULL);
    if (engine == NULL)
        return (NULL);
    if (!dshow_init()) {
        engine_free(engine);
        return (NULL);
    }
    engine->fmtz_load = engine_dshow_load;
    engine->engine_free = engine_dshow_free;
    engine->info = "DirectShow FMTz Engine (Done by CrHackOS)";
    return (engine);
}

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    获取引擎插件接口 (同名)
=======================================
*/
CR_API sENGINE*
engine_get (void_t)
{
    return (engine_dshow());
}
#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
