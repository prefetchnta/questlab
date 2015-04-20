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

#include "memlib.h"
#include "strlib.h"
#include "gfx2/dx8win.h"
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "d3d8.lib")
    #pragma comment (lib, "d3dx8.lib")
    #pragma comment (lib, "dxguid8.lib")
#endif

/*****************************************************************************/
/*                                 绘制接口                                  */
/*****************************************************************************/

#if defined(_CR_HAVE_D3D8_)
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
    iGFX2_DX8M* real;

    real = (iGFX2_DX8M*)that;
    real->m_sprt->Release();
    CR_VCALL(real->m_fill)->release(real->m_fill);
    if (real->m_hdle.main == NULL)
        real->m_hdle.call->release_main(real->m_main);
    mem_free(that);
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
    RECT        rect;
    iGFX2_DX8M* real;

    /* 重新获取窗口大小 */
    real = (iGFX2_DX8M*)that;
    if (!GetClientRect(real->m_hdle.hwnd, &rect))
        return (FALSE);

    /* 如果发生了改变则复位设备 */
    if ((uint_t)rect.right  != that->__back__.position.ww ||
        (uint_t)rect.bottom != that->__back__.position.hh)
    {
        real->m_sprt->OnLostDevice();
        if (!real->m_hdle.call->main_reset(real->m_main, FALSE,
                rect.right, rect.bottom, D3DFMT_UNKNOWN, D3DFMT_UNKNOWN,
                                FALSE, D3DMULTISAMPLE_NONE))
            return (FALSE);
        real->m_sprt->OnResetDevice();
        rect_set_wh(&that->__back__.clip_win, 0, 0, rect.right, rect.bottom);
        struct_cpy(&that->__back__.position, &that->__back__.clip_win, sRECT);
    }
    return (TRUE);
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
    /* 不支持 */
    CR_NOUSE(that);
    return (NULL);
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
    CR_NOUSE(that);
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
    HRESULT     retc;
    iGFX2_DX8M* real;

    real = (iGFX2_DX8M*)that;
    retc = real->m_main->dev->Present(NULL, NULL, NULL, NULL);
    if (FAILED(retc))
        return (FALSE);
    CR_NOUSE(sync);
    return (TRUE);
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
    HRESULT     retc;
    iGFX2_DX8M* real;

    real = (iGFX2_DX8M*)that;
    retc = real->m_main->dev->Clear(0, NULL, D3DCLEAR_TARGET,
                                    color, 1.0f, 0);
    if (FAILED(retc))
        return (FALSE);
    CR_NOUSE(param);
    return (TRUE);
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
static const iGFX2_vtbl s_canvas_vtbl =
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
CR_API iGFX2_DX8M*
create_dx8_canvas (
  __CR_IN__ const sDX8_HDLE*    hdle,
  __CR_IN__ uint_t              scn_cw,
  __CR_IN__ uint_t              scn_ch,
  __CR_IN__ bool_t              full
    )
{
    RECT        rect;
    HRESULT     retc;
    sD3D8_TEXR* fill;
    iGFX2_DX8M* rett;

    /* 使用伪全屏 */
    if (full) {
        scn_cw = GetSystemMetrics(SM_CXSCREEN);
        scn_ch = GetSystemMetrics(SM_CYSCREEN);
        if (!SetWindowPos(hdle->hwnd, HWND_TOP, 0, 0,
                          scn_cw, scn_ch, SWP_SHOWWINDOW))
            return (NULL);
    }
    else if (scn_cw == 0 || scn_ch == 0)
    {
        /* 非法宽高, 获取窗口大小 */
        if (!GetClientRect(hdle->hwnd, &rect))
            return (NULL);
        scn_cw = rect.right;
        scn_ch = rect.bottom;
    }

    /* 生成对象 */
    rett = struct_new(iGFX2_DX8M);
    if (rett == NULL)
        return (NULL);
    struct_zero(&rett->__back__, sIMAGE);

    /* 创建 D3D8 设备 */
    if (hdle->main == NULL) {
        rett->m_main = hdle->call->create_main(hdle->hwnd, FALSE,
                            scn_cw, scn_ch, D3DFMT_UNKNOWN, D3DFMT_UNKNOWN,
                                        FALSE, D3DMULTISAMPLE_NONE);
        if (rett->m_main == NULL)
            goto _failure1;
    }
    else {
        rett->m_main = hdle->main;
    }

    /* 创建填充用的纹理 */
    fill = hdle->call->create_tex2(rett->m_main, 1, 1, D3DFMT_A8R8G8B8,
                                   D3DPOOL_MANAGED, 0, 1);
    if (fill == NULL)
        goto _failure2;
    rett->m_fill = (iGFX2*)create_dx8_bitmap(rett, fill, FALSE);
    if (rett->m_fill == NULL) {
        hdle->call->release_texr(fill);
        goto _failure2;
    }
    if (!CR_VCALL(rett->m_fill)->clear(rett->m_fill, 0xFFFFFFFFUL, 0))
        goto _failure3;

    /* 生成精灵绘制对象 */
    retc = D3DXCreateSprite(rett->m_main->dev, &rett->m_sprt);
    if (FAILED(retc))
        goto _failure3;

    /* 返回生成的对象 */
    rett->__vptr__ = &s_canvas_vtbl;
    struct_cpy(&rett->m_hdle, hdle, sDX8_HDLE);
    rect_set_wh(&rett->__back__.clip_win, 0, 0, scn_cw, scn_ch);
    struct_cpy(&rett->__back__.position, &rett->__back__.clip_win, sRECT);
    return (rett);

_failure3:
    CR_VCALL(rett->m_fill)->release(rett->m_fill);
_failure2:
    if (hdle->main == NULL)
        hdle->call->release_main(rett->m_main);
_failure1:
    mem_free(rett);
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
    CR_NOUSE(param);
    CR_NOUSE(count);
    CR_NOUSE(scn_fmt);
    return ((iGFX2*)create_dx8_canvas((sDX8_HDLE*)handle,
                        scn_cw, scn_ch, full));
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
    sDX8_HDLE*  hdle;
    iGFX2_DX8S* real;

    real = (iGFX2_DX8S*)that;
    hdle = &real->m_device->m_hdle;
    hdle->call->release_texr(real->m_texture);
    mem_free(that);
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
    HRESULT         retc;
    iGFX2_DX8S*     real;
    D3DLOCKED_RECT  info;

    real = (iGFX2_DX8S*)that;
    retc = real->m_texture->obj.tex2->LockRect(0, &info, NULL, real->m_flags);
    if (FAILED(retc))
        return (NULL);
    real->__back__.data = (byte_t*)info.pBits;
    real->__back__.bpl  = (leng_t )info.Pitch;
    real->__back__.size = real->__back__.bpl *
                          real->__back__.position.hh;
    return (&real->__back__);
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
    ((iGFX2_DX8S*)that)->m_texture->obj.tex2->UnlockRect(0);
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
    清除回调
---------------------------------------
*/
static void_t
tex_fill (
  __CR_OT__ D3DXVECTOR4*    pOut,
  __CR_IN__ D3DXVECTOR2*    pTexCoord,
  __CR_IN__ D3DXVECTOR2*    pTexelSize,
  __CR_IN__ LPVOID          pData
    )
{
    D3DXVECTOR4*    fill;

    CR_NOUSE(pTexCoord);
    CR_NOUSE(pTexelSize);

    fill = (D3DXVECTOR4*)pData;
    pOut->x = fill->x;
    pOut->y = fill->y;
    pOut->z = fill->z;
    pOut->w = fill->w;
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
    sint_t      vals;
    HRESULT     retc;
    iGFX2_DX8S* real;
    D3DXVECTOR4 fill;

    /* R */
    vals = (sint_t)((color >> 16) & 0xFF);
    fill.x = (fp32_t)(vals + (vals >> 7)) / 256.0f;

    /* G */
    vals = (sint_t)((color >> 8) & 0xFF);
    fill.y = (fp32_t)(vals + (vals >> 7)) / 256.0f;

    /* B */
    vals = (sint_t)(color & 0xFF);
    fill.z = (fp32_t)(vals + (vals >> 7)) / 256.0f;

    /* A */
    vals = (sint_t)(color >> 24);
    fill.w = (fp32_t)(vals + (vals >> 7)) / 256.0f;

    /* 填充 */
    real = (iGFX2_DX8S*)that;
    retc = D3DXFillTexture(real->m_texture->obj.tex2, tex_fill, &fill);
    if (FAILED(retc))
        return (FALSE);
    CR_NOUSE(param);
    return (TRUE);
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
static const iGFX2_vtbl s_bitmap_vtbl =
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
  __CR_IN__ sD3D8_TEXR* texture,
  __CR_IN__ bool_t      dynamic
    )
{
    iGFX2_DX8S* rett;

    /* 必须是 2D 贴图 */
    if (texture->face != 1)
        return (NULL);

    /* 生成对象并设置图片参数 */
    rett = struct_new(iGFX2_DX8S);
    if (rett == NULL)
        return (NULL);
    if (!image_set(&rett->__back__, NULL, (leng_t)(-1L), 0, 0,
                texture->info.Width, texture->info.Height,
                image_d3d_to_crh(texture->info.Format), FALSE, 8)) {
        rect_set_wh(&rett->__back__.clip_win, 0, 0,
                    texture->info.Width, texture->info.Height);
        struct_cpy(&rett->__back__.position, &rett->__back__.clip_win, sRECT);
    }

    /* 返回生成的对象 */
    rett->__vptr__ = &s_bitmap_vtbl;
    rett->m_flags = dynamic ? D3DLOCK_DISCARD : 0;
    rett->m_device = device;
    rett->m_texture = texture;
    return (rett);
}

/*****************************************************************************/
/*                                 原生绘制                                  */
/*****************************************************************************/

/*
=======================================
    直接填充操作
=======================================
*/
CR_API bool_t
fill_dx8_draw (
  __CR_IO__ const iGFX2_DX8M*   dst,
  __CR_IN__ const sFILL*        fill,
  __CR_IN__ cl32_t              color
    )
{
    sZOOM   zoom;

    zoom.dx = fill->dx;
    zoom.dy = fill->dy;
    zoom.dw = fill->dw;
    zoom.dh = fill->dh;
    zoom.sx = zoom.sy = 0;
    zoom.sw = zoom.sh = 1;
    return (blit_dx8_zoom(dst, (iGFX2_DX8S*)dst->m_fill, &zoom, color));
}

/*
=======================================
    直接 BLT 操作
=======================================
*/
CR_API bool_t
blit_dx8_copy (
  __CR_IO__ const iGFX2_DX8M*   dst,
  __CR_IN__ const iGFX2_DX8S*   src,
  __CR_IN__ const sBLIT*        blit,
  __CR_IN__ cl32_t              color
    )
{
    RECT        rect;
    HRESULT     retc;
    D3DXVECTOR2 dpos;

    dpos.x = (fp32_t)blit->dx;
    dpos.y = (fp32_t)blit->dy;
    rect.left   = (LONG)blit->sx;
    rect.top    = (LONG)blit->sy;
    rect.right  = (LONG)(rect.left + blit->sw);
    rect.bottom = (LONG)(rect.top  + blit->sh);
    retc = dst->m_sprt->Draw(src->m_texture->obj.tex2, &rect,
                             NULL, NULL, 0.0f, &dpos, color);
    if (FAILED(retc))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    缩放 BLT 操作
=======================================
*/
CR_API bool_t
blit_dx8_zoom (
  __CR_IO__ const iGFX2_DX8M*   dst,
  __CR_IN__ const iGFX2_DX8S*   src,
  __CR_IN__ const sZOOM*        zoom,
  __CR_IN__ cl32_t              color
    )
{
    RECT        rect;
    HRESULT     retc;
    D3DXVECTOR2 dpos;
    D3DXVECTOR2 scle;

    dpos.x = (fp32_t)zoom->dx;
    dpos.y = (fp32_t)zoom->dy;
    rect.left   = (LONG)zoom->sx;
    rect.top    = (LONG)zoom->sy;
    rect.right  = (LONG)(rect.left + zoom->sw);
    rect.bottom = (LONG)(rect.top  + zoom->sh);
    scle.x = (fp32_t)zoom->dw / (fp32_t)zoom->sw;
    scle.y = (fp32_t)zoom->dh / (fp32_t)zoom->sh;
    retc = dst->m_sprt->Draw(src->m_texture->obj.tex2, &rect,
                             &scle, NULL, 0.0f, &dpos, color);
    if (FAILED(retc))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    旋转 BLT 操作
=======================================
*/
CR_API bool_t
blit_dx8_rote (
  __CR_IO__ const iGFX2_DX8M*   dst,
  __CR_IN__ const iGFX2_DX8S*   src,
  __CR_IN__ const sBLIT*        blit,
  __CR_IN__ fp32_t              cx,
  __CR_IN__ fp32_t              cy,
  __CR_IN__ fp32_t              ccw,
  __CR_IN__ cl32_t              color
    )
{
    RECT        rect;
    HRESULT     retc;
    D3DXVECTOR2 dpos;
    D3DXVECTOR2 cntr;

    dpos.x = (fp32_t)blit->dx;
    dpos.y = (fp32_t)blit->dy;
    rect.left   = (LONG)blit->sx;
    rect.top    = (LONG)blit->sy;
    rect.right  = (LONG)(rect.left + blit->sw);
    rect.bottom = (LONG)(rect.top  + blit->sh);
    cntr.x = (fp32_t)blit->sw * cx;
    cntr.y = (fp32_t)blit->sh * cy;
    retc = dst->m_sprt->Draw(src->m_texture->obj.tex2, &rect,
                             NULL, &cntr, ccw, &dpos, color);
    if (FAILED(retc))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    全功能 BLT 操作
=======================================
*/
CR_API bool_t
blit_dx8_full (
  __CR_IO__ const iGFX2_DX8M*   dst,
  __CR_IN__ const iGFX2_DX8S*   src,
  __CR_IN__ const sZOOM*        zoom,
  __CR_IN__ fp32_t              cx,
  __CR_IN__ fp32_t              cy,
  __CR_IN__ fp32_t              ccw,
  __CR_IN__ cl32_t              color
    )
{
    RECT        rect;
    HRESULT     retc;
    D3DXVECTOR2 dpos;
    D3DXVECTOR2 cntr;
    D3DXVECTOR2 scle;

    dpos.x = (fp32_t)zoom->dx;
    dpos.y = (fp32_t)zoom->dy;
    rect.left   = (LONG)zoom->sx;
    rect.top    = (LONG)zoom->sy;
    rect.right  = (LONG)(rect.left + zoom->sw);
    rect.bottom = (LONG)(rect.top  + zoom->sh);
    scle.x  = (fp32_t)zoom->dw;
    cntr.x  = scle.x * cx;
    scle.x /= (fp32_t)zoom->sw;
    scle.y  = (fp32_t)zoom->dh;
    cntr.y  = scle.y * cy;
    scle.y /= (fp32_t)zoom->sh;
    retc = dst->m_sprt->Draw(src->m_texture->obj.tex2, &rect,
                             &scle, &cntr, ccw, &dpos, color);
    if (FAILED(retc))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    变换 BLT 操作
=======================================
*/
CR_API bool_t
blit_dx8_matx (
  __CR_IO__ const iGFX2_DX8M*   dst,
  __CR_IN__ const iGFX2_DX8S*   src,
  __CR_IN__ const RECT*         rect,
  __CR_IN__ const D3DXMATRIX*   matx,
  __CR_IN__ cl32_t              color
    )
{
    HRESULT retc;

    retc = dst->m_sprt->DrawTransform(src->m_texture->obj.tex2,
                                      rect, matx, color);
    if (FAILED(retc))
        return (FALSE);
    return (TRUE);
}

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

static const sDX8_CALL s_dx8call =
{
    /* 创建 */
    create_dx8_bitmap,

    /* 模式 */
    do_dx8_enter,
    do_dx8_leave,

    /* 绘制 */
    fill_dx8_draw,
    blit_dx8_copy,
    blit_dx8_zoom,
    blit_dx8_rote,
    blit_dx8_full,
    blit_dx8_matx,
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

#endif  /* _CR_HAVE_D3D8_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
