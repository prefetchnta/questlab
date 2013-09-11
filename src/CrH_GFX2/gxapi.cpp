/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-04-11  */
/*     #######          ###    ###      [GFX2]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack GameAPI 图形绘制接口实现 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "pixels.h"
#include "strlib.h"
#include "gfx2/gxapi.h"

#include <windows.h>

#include "gapi/gx.h"

/* GameAPI 导入库 */
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "gx.lib")
#endif

#define iGFX2_GX_vtbl   iGFX2_vtbl

/* 接口内部数据结构 */
typedef struct
{
        /* 虚函数表 */
        const iGFX2_GX_vtbl*    __vptr__;

        /* 数据成员 */
        sIMAGE  __back__;   /* 显示屏的缓冲 */

        /* 清除接口 */
        bool_t  (*clear) (iGFX2 *that, cl32_t color, int32u param);

} iGFX2_GX;

/*****************************************************************************/
/*                                 清除接口                                  */
/*****************************************************************************/

/*
---------------------------------------
    清除缓冲
---------------------------------------
*/
#define _image_clear12  iGFX2_GX_clear12
#define _image_clear15  iGFX2_GX_clear15
#define _image_clear16  iGFX2_GX_clear16
#define _image_clear24  iGFX2_GX_clear24

#include "../templ/clear2d.inl"

/*****************************************************************************/
/*                                 绘制接口                                  */
/*****************************************************************************/

/*
---------------------------------------
    释放图形绘制接口
---------------------------------------
*/
static void_t
iGFX2_GX_rel_main (
  __CR_IN__ iGFX2*  that
    )
{
    GXCloseDisplay();
    mem_free(that);
}

/*
---------------------------------------
    释放图形绘制接口
---------------------------------------
*/
static void_t
iGFX2_GX_rel_back (
  __CR_IN__ iGFX2*  that
    )
{
    GXCloseDisplay();
    mem_free(that->__back__.data);
    mem_free(that);
}

/*
---------------------------------------
    获取扩展的绘制接口
---------------------------------------
*/
static void_t*
iGFX2_GX_getMore (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iGFX2::GX") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    改变屏幕大小
---------------------------------------
*/
static bool_t
iGFX2_GX_reset (
  __CR_IN__ iGFX2*  that
    )
{
    /* 全屏应用 */
    CR_NOUSE(that);
    return (FALSE);
}

/*
---------------------------------------
    锁住前台缓冲
---------------------------------------
*/
static sIMAGE*
iGFX2_GX_lock_main (
  __CR_IN__ iGFX2*  that
    )
{
    void_t* ptr;

    ptr = GXBeginDraw();
    if (ptr == NULL)
        return (NULL);
    that->__back__.data = (byte_t*)ptr;
    return (&that->__back__);
}

/*
---------------------------------------
    解锁前台缓冲
---------------------------------------
*/
static void_t
iGFX2_GX_unlock_main (
  __CR_IN__ iGFX2*  that
    )
{
    GXEndDraw();
    that->__back__.data = NULL;
}

/*
---------------------------------------
    显示前台缓冲
---------------------------------------
*/
static bool_t
iGFX2_GX_flip_main (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ bool_t  sync
    )
{
    /* 无双缓冲 */
    CR_NOUSE(that);
    CR_NOUSE(sync);
    return (FALSE);
}

/*
---------------------------------------
    清除前台缓冲
---------------------------------------
*/
static bool_t
iGFX2_GX_clear_main (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    iGFX2_GX*   real;

    if (iGFX2_GX_lock_main(that) == NULL)
        return (FALSE);
    real = (iGFX2_GX*)that;
    real->clear(that, color, param);
    iGFX2_GX_unlock_main(that);
    return (TRUE);
}

/*
---------------------------------------
    锁住后台缓冲
---------------------------------------
*/
static sIMAGE*
iGFX2_GX_lock_back (
  __CR_IN__ iGFX2*  that
    )
{
    return (&that->__back__);
}

/*
---------------------------------------
    解锁后台缓冲
---------------------------------------
*/
static void_t
iGFX2_GX_unlock_back (
  __CR_IN__ iGFX2*  that
    )
{
    CR_NOUSE(that);
}

/*
---------------------------------------
    显示后台缓冲
---------------------------------------
*/
static bool_t
iGFX2_GX_flip_back (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ bool_t  sync
    )
{
    void_t* ptr;

    CR_NOUSE(sync);
    ptr = GXBeginDraw();
    if (ptr == NULL)
        return (FALSE);
    mem_cpy(ptr, that->__back__.data, that->__back__.size);
    GXEndDraw();
    return (TRUE);
}

/*
---------------------------------------
    清除后台缓冲
---------------------------------------
*/
static bool_t
iGFX2_GX_clear_back (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    return (((iGFX2_GX*)that)->clear(that, color, param));
}

/*
---------------------------------------
    设置后台调色板
---------------------------------------
*/
static bool_t
iGFX2_GX_setPal (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ uint_t  start,
  __CR_IN__ uint_t  count
    )
{
    /* 无法使用 */
    CR_NOUSE(that);
    CR_NOUSE(start);
    CR_NOUSE(count);
    return (FALSE);
}

/* 接口虚函数表 */
static const iGFX2_GX_vtbl _rom_ s_main_vtbl =
{
    iGFX2_GX_rel_main, iGFX2_GX_getMore,
    iGFX2_GX_reset, iGFX2_GX_lock_main, iGFX2_GX_unlock_main,
    iGFX2_GX_flip_main, iGFX2_GX_clear_main, iGFX2_GX_setPal,
};

static const iGFX2_GX_vtbl _rom_ s_back_vtbl =
{
    iGFX2_GX_rel_back, iGFX2_GX_getMore,
    iGFX2_GX_reset, iGFX2_GX_lock_back, iGFX2_GX_unlock_back,
    iGFX2_GX_flip_back, iGFX2_GX_clear_back, iGFX2_GX_setPal,
};

/*
=======================================
    生成 GameAPI 图形绘制接口
=======================================
*/
CR_API iGFX2*
create_gx_canvas (
  __CR_IN__ void_t*         handle,
  __CR_IN__ uint_t          scn_cw,
  __CR_IN__ uint_t          scn_ch,
  __CR_IN__ uint_t          scn_fmt,
  __CR_IN__ bool_t          full,
  __CR_IN__ const int32u*   param,
  __CR_IN__ uint_t          count
    )
{
    HWND                hwnd;
    uint_t              w, h;
    iGFX2_GX*           rett;
    GXDisplayProperties prop;

    /* 只支持全屏 */
    CR_NOUSE(scn_cw);
    CR_NOUSE(scn_ch);
    CR_NOUSE(scn_fmt);
    if (!full)
        return (NULL);
    hwnd = (HWND)handle;
    if (GXOpenDisplay(hwnd, GX_FULLSCREEN) == 0)
        return (NULL);

    /* 只支持高彩色, 不支持翻转 */
    prop = GXGetDisplayProperties();
    if (prop.cBPP <= 8 || prop.cbxPitch < 0 || prop.cbyPitch < 0)
        goto _failure1;

    /* 生成接口对象 */
    rett = struct_new(iGFX2_GX);
    if (rett == NULL)
        goto _failure1;
    struct_zero(rett, iGFX2_GX);
    if (prop.cbxPitch > prop.cbyPitch) {
        w = prop.cyHeight;
        h = prop.cxWidth;
        rett->__back__.bpc = prop.cbyPitch;
        rett->__back__.bpl = prop.cbxPitch;
    }
    else {
        w = prop.cxWidth;
        h = prop.cyHeight;
        rett->__back__.bpc = prop.cbxPitch;
        rett->__back__.bpl = prop.cbyPitch;
    }
    rect_set_wh(&rett->__back__.clip_win, 0, 0, w, h);
    rect_set_wh(&rett->__back__.position, 0, 0, w, h);
    rett->__back__.size  = h;
    rett->__back__.size *= rett->__back__.bpl;
    if (prop.ffFormat & kfDirect555) {
        rett->clear = iGFX2_GX_clear15;
        rett->__back__.fmt = CR_ARGBX555;
    }
    else
    if (prop.ffFormat & kfDirect565) {
        rett->clear = iGFX2_GX_clear16;
        rett->__back__.fmt = CR_ARGB565;
    }
    else
    if (prop.ffFormat & kfDirect888) {
        rett->clear = iGFX2_GX_clear24;
        rett->__back__.fmt = CR_ARGB888;
    }
    else
    if (prop.ffFormat & kfDirect444) {
        rett->clear = iGFX2_GX_clear12;
        rett->__back__.fmt = CR_ARGB4444;
    }
    else {
        goto _failure2;
    }

    /* 是否开启后台缓冲 */
    if (param == NULL || count < 1 || param[0])
    {
        /* 默认开启后台缓冲 */
        rett->__vptr__ = &s_back_vtbl;
        rett->__back__.data = (uchar*)mem_malloc(rett->__back__.size + 16);
        if (rett->__back__.data == NULL)
            goto _failure2;
    }
    else
    {
        rett->__vptr__ = &s_main_vtbl;
    }
    return ((iGFX2*)rett);

_failure2:
    mem_free(rett);
_failure1:
    GXCloseDisplay();
    return (NULL);
}

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    获取绘图插件接口 (同名)
=======================================
*/
CR_API iGFX2*
create_canvas (
  __CR_IN__ void_t*         handle,
  __CR_IN__ uint_t          scn_cw,
  __CR_IN__ uint_t          scn_ch,
  __CR_IN__ uint_t          scn_fmt,
  __CR_IN__ bool_t          full,
  __CR_IN__ const int32u*   param,
  __CR_IN__ uint_t          count
    )
{
    return (create_gx_canvas(handle, scn_cw, scn_ch, scn_fmt,
                             full, param, count));
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
