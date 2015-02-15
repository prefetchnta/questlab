/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-12-18  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack FMODEx FMTZ 插件接口实现 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "fmtint.h"
#include "strlib.h"
#include "fmod/fmod.h"
#include "fmtz/fmodex.h"

/* FMODEx 导入库 */
#ifndef _CR_NO_PRAGMA_LIB_
    #if     defined(_CR_CC_MSC_)
        #if     defined(_CR_OS_WIN32_)
            #pragma comment (lib, "fmodex_vc.lib")
        #elif   defined(_CR_OS_WIN64_)
            #pragma comment (lib, "fmodex64_vc.lib")
        #else
            #error "e_fmodex.c: OS TYPE not supported yet!"
        #endif
    #elif   defined(_CR_CC_BCC_)
        #pragma comment (lib, "fmodex_bc.lib")
    #else
        #error "e_fmodex.c: CC TYPE not supported yet!"
    #endif
#endif

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iXMMEDIA    xmms;

        /* 个性部分 */
        void_t*         m_dat;
        FMOD_SOUND*     m_snd;
        FMOD_CHANNEL*   m_chn;
        ansi_t          m_inf[256];

} iXMM_FMOD;

/* FMODEx 全局对象 */
static FMOD_SYSTEM* s_fmodex = NULL;

/*
=======================================
    生成 FMODEx 系统对象
=======================================
*/
CR_API bool_t
fmodex_init (void_t)
{
    uint_t              vers;
    sint_t              ndrv;
    FMOD_CAPS           caps;
    FMOD_RESULT         result;
    FMOD_SYSTEM*        system;
    FMOD_SPEAKERMODE    spkmode;
    ansi_t              name[256];

    /* 已经生成过了 */
    if (s_fmodex != NULL)
        return (TRUE);

    /* FMODEx 推荐的初始化过程 */
    result = FMOD_System_Create(&system);
    if (result != FMOD_OK)
        return (FALSE);
    result = FMOD_System_GetVersion(system, &vers);
    if (result != FMOD_OK)
        goto _failure;
    if (vers < FMOD_VERSION)
        goto _failure;
    result = FMOD_System_GetNumDrivers(system, &ndrv);
    if (result != FMOD_OK)
        goto _failure;
    if (ndrv == 0) {
        result = FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_NOSOUND);
        if (result != FMOD_OK)
            goto _failure;
    }
    else {
        result = FMOD_System_GetDriverCaps(system, 0, &caps, 0, &spkmode);
        if (result != FMOD_OK)
            goto _failure;
        result = FMOD_System_SetSpeakerMode(system, spkmode);
        if (result != FMOD_OK)
            goto _failure;
        if (caps & FMOD_CAPS_HARDWARE_EMULATED) {
            result = FMOD_System_SetDSPBufferSize(system, 1024, 10);
            if (result != FMOD_OK)
                goto _failure;
        }
        result = FMOD_System_GetDriverInfo(system, 0, name, sizeof(name), 0);
        if (result != FMOD_OK)
            goto _failure;
        if (str_strA(name, "SigmaTel")) {
            result = FMOD_System_SetSoftwareFormat(system,
                            48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0,
                                FMOD_DSP_RESAMPLER_LINEAR);
            if (result != FMOD_OK)
                goto _failure;
        }
    }
    result = FMOD_System_Init(system, 100, FMOD_INIT_NORMAL, 0);
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER) {
        result = FMOD_System_SetSpeakerMode(system, FMOD_SPEAKERMODE_STEREO);
        if (result != FMOD_OK)
            goto _failure;
        result = FMOD_System_Init(system, 100, FMOD_INIT_NORMAL, 0);
    }
    if (result != FMOD_OK)
        goto _failure;

    /* 设置全局对象 */
    s_fmodex = system;
    return (TRUE);

_failure:
    FMOD_System_Release(system);
    return (FALSE);
}

/*
=======================================
    释放 FMODEx 系统对象
=======================================
*/
CR_API void_t
fmodex_kill (void_t)
{
    if (s_fmodex != NULL) {
        FMOD_System_Release(s_fmodex);
        s_fmodex = NULL;
    }
}

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iXMM_FMOD_release (
  __CR_IN__ iXMMEDIA*   that
    )
{
    iXMM_FMOD*  real;

    real = (iXMM_FMOD*)that;
    FMOD_Channel_Stop(real->m_chn);
    FMOD_Sound_Release(real->m_snd);
    TRY_FREE(real->m_dat);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iXMM_FMOD_getMore (
  __CR_IN__ iXMMEDIA*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iXMMEDIA::FMOD") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    播放媒体
---------------------------------------
*/
static bool_t
iXMM_FMOD_play (
  __CR_IN__ iXMMEDIA*   that
    )
{
    iXMM_FMOD*  real;
    FMOD_RESULT result;

    real = (iXMM_FMOD*)that;
    result = FMOD_Channel_SetPaused(real->m_chn, FALSE);
    if (result != FMOD_OK)
        return (FALSE);
    return (TRUE);
}

/*
---------------------------------------
    暂停播放
---------------------------------------
*/
static bool_t
iXMM_FMOD_pause (
  __CR_IN__ iXMMEDIA*   that
    )
{
    iXMM_FMOD*  real;
    FMOD_RESULT result;

    real = (iXMM_FMOD*)that;
    result = FMOD_Channel_SetPaused(real->m_chn, TRUE);
    if (result != FMOD_OK)
        return (FALSE);
    return (TRUE);
}

/*
---------------------------------------
    停止播放
---------------------------------------
*/
static bool_t
iXMM_FMOD_stop (
  __CR_IN__ iXMMEDIA*   that
    )
{
    iXMM_FMOD*      real;
    FMOD_RESULT     result;
    FMOD_CHANNEL*   channel;

    /* 因为流只能播放一次, 结束后要重新生成 */
    real = (iXMM_FMOD*)that;
    FMOD_Channel_Stop(real->m_chn);
    result = FMOD_System_PlaySound(s_fmodex, FMOD_CHANNEL_FREE,
                                   real->m_snd, TRUE, &channel);
    if (result != FMOD_OK)
        return (FALSE);
    real->m_chn = channel;
    xmms_adj_volume(that, 0);
    return (TRUE);
}

/*
---------------------------------------
    设置当前位置
---------------------------------------
*/
static bool_t
iXMM_FMOD_set_pos (
  __CR_IN__ iXMMEDIA*   that,
  __CR_IN__ int64u      curt
    )
{
    uint_t      pos;
    uint_t      len;
    iXMM_FMOD*  real;
    FMOD_RESULT result;

    real = (iXMM_FMOD*)that;
    result = FMOD_Sound_GetLength(real->m_snd, &len, FMOD_TIMEUNIT_MS);
    if (result != FMOD_OK)
        return (FALSE);
    pos = (curt >= len) ? len : (uint_t)curt;
    result = FMOD_Channel_SetPosition(real->m_chn, pos, FMOD_TIMEUNIT_MS);
    if (result != FMOD_OK)
        return (FALSE);
    return (TRUE);
}

/*
---------------------------------------
    获取当前位置
---------------------------------------
*/
static bool_t
iXMM_FMOD_get_pos (
  __CR_IN__ iXMMEDIA*   that,
  __CR_OT__ int64u*     curt,
  __CR_OT__ int64u*     total
    )
{
    uint_t      pos;
    uint_t      len;
    iXMM_FMOD*  real;
    FMOD_RESULT result;

    real = (iXMM_FMOD*)that;
    if (total != NULL) {
        result = FMOD_Sound_GetLength(real->m_snd, &len,
                                FMOD_TIMEUNIT_MS);
        if (result != FMOD_OK)
            return (FALSE);
        *total = len;
    }
    if (curt != NULL) {
        result = FMOD_Channel_GetPosition(real->m_chn, &pos,
                                FMOD_TIMEUNIT_MS);
        if (result != FMOD_OK)
            return (FALSE);
        *curt = pos;
    }
    return (TRUE);
}

/*
---------------------------------------
    播放是否结束
---------------------------------------
*/
static bool_t
iXMM_FMOD_is_over (
  __CR_IN__ iXMMEDIA*   that,
  __CR_OT__ bool_t*     over
    )
{
    FMOD_BOOL   retc;
    iXMM_FMOD*  real;
    FMOD_RESULT result;

    real = (iXMM_FMOD*)that;
    result = FMOD_Channel_IsPlaying(real->m_chn, &retc);
    if (result != FMOD_OK)
        return (FALSE);
    *over = retc ? FALSE : TRUE;
    return (TRUE);
}

/*
---------------------------------------
    设置音量
---------------------------------------
*/
static bool_t
iXMM_FMOD_set_volume (
  __CR_IO__ iXMMEDIA*   that,
  __CR_IN__ sint_t      percent
    )
{
    float       vols;
    iXMM_FMOD*  real;
    FMOD_RESULT result;

    real = (iXMM_FMOD*)that;
    if (percent < 0)
        percent = 0;
    else
    if (percent > 100)
        percent = 100;
    vols = ((float)percent) / 100.0f;
    result = FMOD_Channel_SetVolume(real->m_chn, vols);
    if (result != FMOD_OK)
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
iXMM_FMOD_get_volume (
  __CR_IO__ iXMMEDIA*   that,
  __CR_OT__ sint_t*     percent
    )
{
    float       vols;
    iXMM_FMOD*  real;
    FMOD_RESULT result;

    real = (iXMM_FMOD*)that;
    result = FMOD_Channel_GetVolume(real->m_chn, &vols);
    if (result != FMOD_OK)
        return (FALSE);
    that->__volume__ = (sint_t)(vols * 100.0f);
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
iXMM_FMOD_is_video (
  __CR_IN__ iXMMEDIA*   that
    )
{
    CR_NOUSE(that);
    return (FALSE);
}

/*
---------------------------------------
    获取视频大小
---------------------------------------
*/
static bool_t
iXMM_FMOD_video_size (
  __CR_IN__ iXMMEDIA*   that,
  __CR_OT__ uint_t*     width,
  __CR_OT__ uint_t*     height
    )
{
    CR_NOUSE(that);
    CR_NOUSE(width);
    CR_NOUSE(height);
    return (FALSE);
}

/*
---------------------------------------
    设置视频窗口
---------------------------------------
*/
static bool_t
iXMM_FMOD_video_setwin (
  __CR_IO__ iXMMEDIA*   that,
  __CR_IN__ hwnd_t      hwnd,
  __CR_IN__ bool_t      full
    )
{
    CR_NOUSE(that);
    CR_NOUSE(hwnd);
    CR_NOUSE(full);
    return (FALSE);
}

/* 接口虚函数表 */
static const iXMMEDIA_vtbl _rom_ s_xmms_vtbl =
{
    iXMM_FMOD_release, iXMM_FMOD_getMore,
    iXMM_FMOD_play, iXMM_FMOD_pause, iXMM_FMOD_stop,
    iXMM_FMOD_set_pos, iXMM_FMOD_get_pos, iXMM_FMOD_is_over,
    iXMM_FMOD_set_volume, iXMM_FMOD_get_volume, iXMM_FMOD_is_video,
    iXMM_FMOD_video_size, iXMM_FMOD_video_setwin,
};

/*
---------------------------------------
    FMODEx 类型信息
---------------------------------------
*/
static bool_t
fmodex_info (
  __CR_IO__ iXMM_FMOD*  port
    )
{
    sint_t              bits;
    sint_t              chns;
    ansi_t*             allx;
    const ansi_t*       type;
    const ansi_t*       fmts;
    FMOD_RESULT         result;
    FMOD_SOUND_TYPE     snd_type;
    FMOD_SOUND_FORMAT   snd_fmts;

    /* 获取所有信息 */
    result = FMOD_Sound_GetFormat(port->m_snd, &snd_type,
                        &snd_fmts, &chns, &bits);
    if (result != FMOD_OK)
        return (FALSE);

    /* 音频格式类型 */
    switch (snd_type)
    {
        default:
            type = "3rd party / unknown plugin format";
            break;

        case FMOD_SOUND_TYPE_AIFF:
            type = "AIFF";
            break;

        case FMOD_SOUND_TYPE_ASF:
            type = "Microsoft Advanced Systems Format (ie WMA/ASF/WMV)";
            break;

        case FMOD_SOUND_TYPE_AT3:
            type = "Sony ATRAC 3 format";
            break;

        case FMOD_SOUND_TYPE_CDDA:
            type = "Digital CD audio";
            break;

        case FMOD_SOUND_TYPE_DLS:
            type = "Sound font / downloadable sound bank";
            break;

        case FMOD_SOUND_TYPE_FLAC:
            type = "FLAC lossless codec";
            break;

        case FMOD_SOUND_TYPE_FSB:
            type = "FMOD Sample Bank";
            break;

        case FMOD_SOUND_TYPE_GCADPCM:
            type = "Nintendo GameCube/Wii ADPCM";
            break;

        case FMOD_SOUND_TYPE_IT:
            type = "Impulse Tracker";
            break;

        case FMOD_SOUND_TYPE_MIDI:
            type = "MIDI";
            break;

        case FMOD_SOUND_TYPE_MOD:
            type = "Protracker / Fasttracker MOD";
            break;

        case FMOD_SOUND_TYPE_MPEG:
            type = "MP2/MP3 MPEG";
            break;

        case FMOD_SOUND_TYPE_OGGVORBIS:
            type = "Ogg vorbis";
            break;

        case FMOD_SOUND_TYPE_PLAYLIST:
            type = "Information only from ASX/PLS/M3U/WAX playlists";
            break;

        case FMOD_SOUND_TYPE_RAW:
            type = "Raw PCM data";
            break;

        case FMOD_SOUND_TYPE_S3M:
            type = "ScreamTracker 3";
            break;

        case FMOD_SOUND_TYPE_SF2:
            type = "Sound font 2 format";
            break;

        case FMOD_SOUND_TYPE_USER:
            type = "User created sound";
            break;

        case FMOD_SOUND_TYPE_WAV:
            type = "Microsoft WAV";
            break;

        case FMOD_SOUND_TYPE_XM:
            type = "FastTracker 2 XM";
            break;

        case FMOD_SOUND_TYPE_XMA:
            type = "Xbox360 XMA";
            break;

        case FMOD_SOUND_TYPE_VAG:
            type = "PlayStation Portable ADPCM VAG format";
            break;

        case FMOD_SOUND_TYPE_AUDIOQUEUE:
            type = "iPhone hardware decoder, supports AAC, ALAC and MP3";
            break;

        case FMOD_SOUND_TYPE_XWMA:
            type = "Xbox360 XWMA";
            break;

        case FMOD_SOUND_TYPE_BCWAV:
            type = "3DS BCWAV container format for DSP ADPCM and PCM";
            break;

        case FMOD_SOUND_TYPE_AT9:
            type = "NGP ATRAC 9 format";
            break;

        case FMOD_SOUND_TYPE_VORBIS:
            type = "Raw vorbis";
            break;

        case FMOD_SOUND_TYPE_MEDIA_FOUNDATION:
            type = "Microsoft Media Foundation wrappers, supports ASF/WMA";
            break;
    }

    /* 音频数据类型 */
    switch (snd_fmts)
    {
        default:
            fmts = "Unitialized / unknown";
            break;

        case FMOD_SOUND_FORMAT_PCM8:
            fmts = "8bit integer PCM data";
            break;

        case FMOD_SOUND_FORMAT_PCM16:
            fmts = "16bit integer PCM data";
            break;

        case FMOD_SOUND_FORMAT_PCM24:
            fmts = "24bit integer PCM data";
            break;

        case FMOD_SOUND_FORMAT_PCM32:
            fmts = "32bit integer PCM data";
            break;

        case FMOD_SOUND_FORMAT_PCMFLOAT:
            fmts = "32bit floating point PCM data";
            break;

        case FMOD_SOUND_FORMAT_GCADPCM:
            fmts = "Compressed Nintendo 3DS/Wii DSP data";
            break;

        case FMOD_SOUND_FORMAT_IMAADPCM:
            fmts = "Compressed IMA ADPCM data";
            break;

        case FMOD_SOUND_FORMAT_VAG:
            fmts = "Compressed PlayStation Portable ADPCM data";
            break;

        case FMOD_SOUND_FORMAT_HEVAG:
            fmts = "Compressed PSVita ADPCM data";
            break;

        case FMOD_SOUND_FORMAT_XMA:
            fmts = "Compressed Xbox360 XMA data";
            break;

        case FMOD_SOUND_FORMAT_MPEG:
            fmts = "Compressed MPEG layer 2 or 3 data";
            break;

        case FMOD_SOUND_FORMAT_CELT:
            fmts = "Compressed CELT data";
            break;

        case FMOD_SOUND_FORMAT_AT9:
            fmts = "Compressed PSVita ATRAC9 data";
            break;

        case FMOD_SOUND_FORMAT_XWMA:
            fmts = "Compressed Xbox360 xWMA data";
            break;

        case FMOD_SOUND_FORMAT_VORBIS:
            fmts = "Compressed Vorbis data";
            break;
    }

    /* 合成说明字符串 */
    allx = str_fmtA("%s (%s) - %uch / %ubits", type, fmts, chns, bits);
    if (allx == NULL)
        return (FALSE);
    str_cpyA(port->m_inf, allx);
    mem_free(allx);
    return (TRUE);
}

/*
=======================================
    FMODEx 文件读取
=======================================
*/
CR_API sFMT_PRT*
load_fmodex (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    uint_t                  size;
    void_t*                 data;
    ansi_t*                 path;
    sFMT_PRT*               rett;
    FMOD_MODE               mode;
    iXMM_FMOD*              port;
    FMOD_SOUND*             sound;
    FMOD_RESULT             result;
    FMOD_CHANNEL*           channel;
    FMOD_CREATESOUNDEXINFO  ex_info;

    /* 必须先初始化 */
    if (s_fmodex == NULL)
        return (NULL);
    mode = FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE | FMOD_ACCURATETIME;

    /* 不支持文件区段功能 */
    switch (param->type)
    {
        case CR_LDR_ANSI:
            data = NULL;
            result = FMOD_System_CreateStream(s_fmodex, param->name.ansi,
                                              mode, NULL, &sound);
            break;

        case CR_LDR_WIDE:
            data = NULL;
            path = utf16_to_local(CR_LOCAL, param->name.wide);
            if (path == NULL)
                return (NULL);
            result = FMOD_System_CreateStream(s_fmodex, path, mode,
                                              NULL, &sound);
            mem_free(path);
            break;

        case CR_LDR_BUFF:
            if (cut_size(&size, param->buff.size))
                return (NULL);
            data = mem_dup(param->buff.data, param->buff.size);
            if (data == NULL)
                return (NULL);
            path = (ansi_t*)data;
            mode |= FMOD_OPENMEMORY;
            mem_zero(&ex_info, sizeof(ex_info));
            ex_info.cbsize = sizeof(ex_info);
            ex_info.length = size;
            result = FMOD_System_CreateStream(s_fmodex, path, mode,
                                              &ex_info, &sound);
            break;

        default:
            return (NULL);
    }

    /* 无法支持的格式 */
    if (result != FMOD_OK)
        goto _failure1;

    /* 生成播放通道对象 */
    result = FMOD_System_PlaySound(s_fmodex, FMOD_CHANNEL_FREE,
                                   sound, TRUE, &channel);
    if (result != FMOD_OK)
        goto _failure2;

    /* 生成媒体播放接口对象 */
    port = struct_new(iXMM_FMOD);
    if (port == NULL)
        goto _failure3;
    struct_zero(port, iXMM_FMOD);
    port->m_dat = data;
    port->m_snd = sound;
    port->m_chn = channel;
    if (!fmodex_info(port)) {
        iXMM_FMOD_release((iXMMEDIA*)port);
        return (NULL);
    }
    port->xmms.__volume__ = 100;
    port->xmms.__vptr__ = &s_xmms_vtbl;

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iXMM_FMOD_release((iXMMEDIA*)port);
        return (NULL);
    }
    CR_NOUSE(datin);
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iXMMEDIA";
    rett->infor = port->m_inf;
    return (rett);

_failure3:
    FMOD_Channel_Stop(channel);
_failure2:
    FMOD_Sound_Release(sound);
_failure1:
    TRY_FREE(data);
    return (NULL);
}

/*
---------------------------------------
    文件格式加载接口
---------------------------------------
*/
static sFMTZ*
engine_fmodex_load (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    if (!(engine->mask & CR_FMTZ_MASK_XMM))
        return (NULL);
    return ((sFMTZ*)load_fmodex(NULL, loader));
}

/*
---------------------------------------
    引擎插件释放回调
---------------------------------------
*/
static void_t
engine_fmodex_free (
  __CR_IO__ sENGINE*    engine
    )
{
    fmodex_kill();
    CR_NOUSE(engine);
}

/*
=======================================
    获取引擎插件接口
=======================================
*/
CR_API sENGINE*
engine_fmodex (void_t)
{
    sENGINE*    engine;

    engine = engine_init(NULL, NULL, NULL, NULL);
    if (engine == NULL)
        return (NULL);
    if (!fmodex_init()) {
        engine_free(engine);
        return (NULL);
    }
    engine->fmtz_load = engine_fmodex_load;
    engine->engine_free = engine_fmodex_free;
    engine->info = "FMODEx FMTz Engine (Done by CrHackOS)";
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
    return (engine_fmodex());
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
