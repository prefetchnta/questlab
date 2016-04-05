/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-02-20  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack DX9 图形绘制接口定义 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DX9WIN_H__
#define __CR_DX9WIN_H__

#define _CR_USE_D3D9_
#include "../gfx3.h"

#if defined(_CR_HAVE_D3D9_)

/*****************************************************************************/
/*                                 绘制接口                                  */
/*****************************************************************************/

/* 创建传入的句柄结构 */
typedef struct
{
        HWND                hwnd;   /* 窗口句柄 */
        sD3D9_MAIN*         main;   /* 预先生成 */
        const sD3D9_CALL*   call;   /* 调用接口 */

} sDX9_HDLE;

/* DX9 绘制表面实现结构 */
typedef struct
{
        /* 虚函数表 */
        const iGFX2_vtbl*   __vptr__;

        /* 数据成员 */
        sIMAGE  __back__;   /* 只有宽高是有用的 */

        /* 个性部分 */
        iGFX2*          m_fill;     /* 填充用的纹理 */
        sDX9_HDLE       m_hdle;     /* 设备句柄结构 */
        sD3D9_MAIN*     m_main;     /* D3D9 设备对象 */
        LPD3DXSPRITE    m_sprt;     /* 用于绘制纹理 */

} iGFX2_DX9M;

/* DX9 离屏表面实现结构 */
typedef struct
{
        /* 虚函数表 */
        const iGFX2_vtbl*   __vptr__;

        /* 数据成员 */
        sIMAGE  __back__;   /* 锁定时可以操作 */

        /* 个性部分 */
        DWORD       m_flags;    /* 锁定标志 */
        iGFX2_DX9M* m_device;   /* 设备对象 */
        sD3D9_TEXR* m_texture;  /* 纹理对象 */

} iGFX2_DX9S;

/* 生成 DX9 图形绘制接口 */
CR_API iGFX2_DX9M*  create_dx9_canvas (const sDX9_HDLE *hdle,
                            uint_t scn_cw, uint_t scn_ch, bool_t full);
/* 生成 DX9 图形离屏表面 */
CR_API iGFX2_DX9S*  create_dx9_bitmap (iGFX2_DX9M *device,
                                       sD3D9_TEXR *texture,
                                       bool_t dynamic);

/*****************************************************************************/
/*                                 原生绘制                                  */
/*****************************************************************************/

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    开始绘制
=======================================
*/
cr_inline void_t
do_dx9_enter (
  __CR_IN__ iGFX2_DX9M* dst
    )
{
    dst->m_main->dev->BeginScene();
}

/*
=======================================
    结束绘制
=======================================
*/
cr_inline void_t
do_dx9_leave (
  __CR_IN__ iGFX2_DX9M* dst
    )
{
    dst->m_main->dev->EndScene();
}

/*
=======================================
    进入直接复制模式
=======================================
*/
cr_inline void_t
mode_dx9_set (
  __CR_IN__ iGFX2_DX9M* dst
    )
{
    dst->m_sprt->Begin(0);
}

/*
=======================================
    进入透明混合模式
=======================================
*/
cr_inline void_t
mode_dx9_alp (
  __CR_IN__ iGFX2_DX9M* dst
    )
{
    dst->m_sprt->Begin(D3DXSPRITE_ALPHABLEND);
}

/*
=======================================
    进入饱和加法模式
=======================================
*/
cr_inline void_t
mode_dx9_add (
  __CR_IN__ iGFX2_DX9M* dst
    )
{
    dst->m_sprt->Begin(0);
    dst->m_main->dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    dst->m_main->dev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    dst->m_main->dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    dst->m_main->dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

/*
=======================================
    进入饱和减法模式
=======================================
*/
cr_inline void_t
mode_dx9_sub (
  __CR_IN__ iGFX2_DX9M* dst
    )
{
    dst->m_sprt->Begin(0);
    dst->m_main->dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    dst->m_main->dev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
    dst->m_main->dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    dst->m_main->dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

/*
=======================================
    退出绘制模式
=======================================
*/
cr_inline void_t
mode_dx9_end (
  __CR_IN__ iGFX2_DX9M* dst
    )
{
    dst->m_sprt->End();
}

#endif  /* !_CR_SICK_INLINE_ */

CR_API bool_t   fill_dx9_draw (const iGFX2_DX9M *dst,
                               const sFILL *fill, cl32_t color);

CR_API bool_t   blit_dx9_copy (const iGFX2_DX9M *dst,
                               const iGFX2_DX9S *src,
                               const sBLIT *blit, cl32_t color);

CR_API bool_t   blit_dx9_zoom (const iGFX2_DX9M *dst,
                               const iGFX2_DX9S *src,
                               const sZOOM *zoom, cl32_t color);

CR_API bool_t   blit_dx9_rote (const iGFX2_DX9M *dst,
                               const iGFX2_DX9S *src,
                               const sBLIT *blit, fp32_t cx, fp32_t cy,
                               fp32_t ccw, cl32_t color);

CR_API bool_t   blit_dx9_full (const iGFX2_DX9M *dst,
                               const iGFX2_DX9S *src,
                               const sZOOM *zoom, fp32_t cx, fp32_t cy,
                               fp32_t ccw, cl32_t color);

CR_API bool_t   blit_dx9_matx (const iGFX2_DX9M *dst,
                               const iGFX2_DX9S *src,
                               const RECT *rect, const D3DXMATRIX *matx,
                               cl32_t color);

/*****************************************************************************/
/*                                 文字接口                                  */
/*****************************************************************************/

/* DX9 具体实现结构 */
typedef struct
{
        /* 虚函数表 */
        const iFONT_vtbl*   __vptr__;

        /* 数据成员 */
        int32u  __draw_mode__;              /* 模式 (平台相关) */
        int32u  __color__, __bkcolor__;     /* 颜色 (平台相关) */

        /* 个性部分 */
        LPD3DXFONT      m_font;     /* 字体对象 */
        LPD3DXSPRITE    m_sprt;     /* 绘制对象 */

} iFONT_DX9;

/* 生成 DX9 文字绘制接口 */
CR_API iFONT*   create_dx9_fontA (iGFX2_DX9M *devs,
                            const D3DXFONT_DESCA *desc);

CR_API iFONT*   create_dx9_fontW (iGFX2_DX9M *devs,
                            const D3DXFONT_DESCW *desc);

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    设备丢失时调用
=======================================
*/
cr_inline void_t
font_dx9_losts (
  __CR_IN__ iFONT_DX9*  dst
    )
{
    dst->m_font->OnLostDevice();
    dst->m_sprt->OnLostDevice();
}

/*
=======================================
    设备复位时调用
=======================================
*/
cr_inline void_t
font_dx9_reset (
  __CR_IN__ iFONT_DX9*  dst
    )
{
    dst->m_font->OnResetDevice();
    dst->m_sprt->OnResetDevice();
}

#endif  /* !_CR_SICK_INLINE_ */

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

/* DX9 调用接口表 */
typedef struct
{
    /* 离屏表面 */
    iGFX2_DX9S* (*create_bitmap) (iGFX2_DX9M *device,
                                  sD3D9_TEXR *texture,
                                  bool_t dynamic);
    /* 文字生成 */
    iFONT*  (*create_fontA) (iGFX2_DX9M *devs, const D3DXFONT_DESCA *desc);
    iFONT*  (*create_fontW) (iGFX2_DX9M *devs, const D3DXFONT_DESCW *desc);

    /* 设备管理 */
    void_t  (*font_losts) (iFONT_DX9 *dst);
    void_t  (*font_reset) (iFONT_DX9 *dst);

    /* 模式设置 */
    void_t  (*do_enter) (iGFX2_DX9M *dst);
    void_t  (*do_leave) (iGFX2_DX9M *dst);
    void_t  (*mode_set) (iGFX2_DX9M *dst);
    void_t  (*mode_alp) (iGFX2_DX9M *dst);
    void_t  (*mode_add) (iGFX2_DX9M *dst);
    void_t  (*mode_sub) (iGFX2_DX9M *dst);
    void_t  (*mode_end) (iGFX2_DX9M *dst);

    /* 原生绘图 */
    bool_t  (*fill_draw) (const iGFX2_DX9M *dst, const sFILL *fill,
                          cl32_t color);

    bool_t  (*blit_copy) (const iGFX2_DX9M *dst, const iGFX2_DX9S *src,
                          const sBLIT *blit, cl32_t color);

    bool_t  (*blit_zoom) (const iGFX2_DX9M *dst, const iGFX2_DX9S *src,
                          const sZOOM *zoom, cl32_t color);

    bool_t  (*blit_rote) (const iGFX2_DX9M *dst, const iGFX2_DX9S *src,
                          const sBLIT *blit, fp32_t cx, fp32_t cy,
                          fp32_t ccw, cl32_t color);

    bool_t  (*blit_full) (const iGFX2_DX9M *dst, const iGFX2_DX9S *src,
                          const sZOOM *zoom, fp32_t cx, fp32_t cy,
                          fp32_t ccw, cl32_t color);

    bool_t  (*blit_matx) (const iGFX2_DX9M *dst, const iGFX2_DX9S *src,
                          const RECT *rect, const D3DXMATRIX *matx,
                          cl32_t color);
} sDX9_CALL;

/* 获取 DX9 调用接口表 */
CR_API const sDX9_CALL* dx9call_get (void_t);

#endif  /* _CR_HAVE_D3D9_ */

#endif  /* !__CR_DX9WIN_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
