/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-01-27  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack DX8 图形绘制接口实现 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DX8WIN_CPP__
#define __CR_DX8WIN_CPP__ 0x0DF450E7UL

#include "memlib.h"
#include "strlib.h"
#include "gfx2/dx8win.h"

/*****************************************************************************/
/*                                 绘制接口                                  */
/*****************************************************************************/

/*
---------------------------------------
    释放图形绘制接口
---------------------------------------
*/
static void_t
iGFX2_DX8M_release (
  __CR_IN__ iGFX2*  that
    )
{
}

/*
---------------------------------------
    获取扩展的绘制接口
---------------------------------------
*/
static void_t*
iGFX2_DX8M_getMore (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iGFX2::DX8M") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    改变屏幕大小
---------------------------------------
*/
static bool_t
iGFX2_DX8M_reset (
  __CR_IN__ iGFX2*  that
    )
{
}

/*
---------------------------------------
    锁住后台缓冲
---------------------------------------
*/
static sIMAGE*
iGFX2_DX8M_lock (
  __CR_IN__ iGFX2*  that
    )
{
}

/*
---------------------------------------
    解锁后台缓冲
---------------------------------------
*/
static void_t
iGFX2_DX8M_unlock (
  __CR_IN__ iGFX2*  that
    )
{
}

/*
---------------------------------------
    显示后台缓冲
---------------------------------------
*/
static bool_t
iGFX2_DX8M_flip (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ bool_t  sync
    )
{
}

/*
---------------------------------------
    清除后台缓冲
---------------------------------------
*/
static bool_t
iGFX2_DX8M_clear (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
}

/*
---------------------------------------
    设置后台调色板
---------------------------------------
*/
static bool_t
iGFX2_DX8M_setPal (
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
static const iGFX2_vtbl _rom_ s_canvas_vtbl =
{
    iGFX2_DX8M_release, iGFX2_DX8M_getMore,
    iGFX2_DX8M_reset, iGFX2_DX8M_lock, iGFX2_DX8M_unlock,
    iGFX2_DX8M_flip, iGFX2_DX8M_clear, iGFX2_DX8M_setPal,
};

/*
=======================================
    生成 DX8 图形绘制接口
=======================================
*/
CR_API iGFX2*
create_dx8_canvas (
  __CR_IN__ void_t*         handle,
  __CR_IN__ uint_t          scn_cw,
  __CR_IN__ uint_t          scn_ch,
  __CR_IN__ uint_t          scn_fmt,
  __CR_IN__ bool_t          full,
  __CR_IN__ const int32u*   param,
  __CR_IN__ uint_t          count
    )
{
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
    return (create_dx8_canvas(handle, scn_cw, scn_ch, scn_fmt,
                              full, param, count));
}
#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/*                                 离屏表面                                  */
/*****************************************************************************/

/*
---------------------------------------
    释放离屏表面接口
---------------------------------------
*/
static void_t
iGFX2_DX8S_release (
  __CR_IN__ iGFX2*  that
    )
{
}

/*
---------------------------------------
    获取扩展的离屏表面
---------------------------------------
*/
static void_t*
iGFX2_DX8S_getMore (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iGFX2::DX8S") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    离屏表面使用的空函数
---------------------------------------
*/
static bool_t
iGFX2_DX8S_reset (
  __CR_IN__ iGFX2*  that
    )
{
    CR_NOUSE(that);
    return (FALSE);
}

/*
---------------------------------------
    锁住离屏表面
---------------------------------------
*/
static sIMAGE*
iGFX2_DX8S_lock (
  __CR_IN__ iGFX2*  that
    )
{
}

/*
---------------------------------------
    解锁离屏表面
---------------------------------------
*/
static void_t
iGFX2_DX8S_unlock (
  __CR_IN__ iGFX2*  that
    )
{
}

/*
---------------------------------------
    离屏表面使用的空函数
---------------------------------------
*/
static bool_t
iGFX2_DX8S_flip (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ bool_t  sync
    )
{
    CR_NOUSE(that);
    CR_NOUSE(sync);
    return (FALSE);
}

/*
---------------------------------------
    清除离屏表面
---------------------------------------
*/
static bool_t
iGFX2_DX8S_clear (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
}

/*
---------------------------------------
    设置离屏表面调色板
---------------------------------------
*/
static bool_t
iGFX2_DX8S_setPal (
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
static const iGFX2_vtbl _rom_ s_bitmap_vtbl =
{
    iGFX2_DX8S_release, iGFX2_DX8S_getMore,
    iGFX2_DX8S_reset, iGFX2_DX8S_lock, iGFX2_DX8S_unlock,
    iGFX2_DX8S_flip, iGFX2_DX8S_clear, iGFX2_DX8S_setPal,
};

/*
=======================================
    生成 DX8 图形离屏表面
=======================================
*/
CR_API iGFX2_DX8S*
create_dx8_bitmap (
  __CR_IN__ iGFX2_DX8M* device,
  __CR_IN__ sD3D8_TEXR* texture
    )
{
}

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

static const sDX8_CALL _rom_ s_dx8call =
{
    /* 创建 */
    create_dx8_bitmap,
};

/*
=======================================
    获取 DX8 调用接口表
=======================================
*/
CR_API const sDX8_CALL*
dx8call_get (void_t)
{
    return (&s_dx8call);
}

#endif  /* !__CR_DX8WIN_CPP__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
