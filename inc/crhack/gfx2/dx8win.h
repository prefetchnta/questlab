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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack DX8 图形绘制接口定义 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DX8WIN_H__
#define __CR_DX8WIN_H__

#define _CR_USE_D3D8_
#include "../gfx3.h"

#if defined(_CR_HAVE_D3D8_)

/*****************************************************************************/
/*                                 绘制接口                                  */
/*****************************************************************************/

/* 创建传入的句柄结构 */
typedef struct
{
        HWND                hwnd;   /* 窗口句柄 */
        sD3D8_MAIN*         main;   /* 预先生成 */
        const sD3D8_CALL*   call;   /* 调用接口 */

} sDX8_HDLE;

/* DX8 绘制表面实现结构 */
typedef struct
{
        /* 虚函数表 */
        const iGFX2_vtbl*   __vptr__;

        /* 数据成员 */
        sIMAGE  __back__;   /* 只有宽高是有用的 */

        /* 个性部分 */
        iGFX2*          m_fill;     /* 填充用的纹理 */
        sDX8_HDLE       m_hdle;     /* 设备句柄结构 */
        sD3D8_MAIN*     m_main;     /* D3D8 设备对象 */
        LPD3DXSPRITE    m_sprt;     /* 用于绘制纹理 */

} iGFX2_DX8M;

/* DX8 离屏表面实现结构 */
typedef struct
{
        /* 虚函数表 */
        const iGFX2_vtbl*   __vptr__;

        /* 数据成员 */
        sIMAGE  __back__;   /* 锁定时可以操作 */

        /* 个性部分 */
        DWORD       m_flags;    /* 锁定标志 */
        iGFX2_DX8M* m_device;   /* 设备对象 */
        sD3D8_TEXR* m_texture;  /* 纹理对象 */

} iGFX2_DX8S;

/* 生成 DX8 图形绘制接口 */
CR_API iGFX2_DX8M*  create_dx8_canvas (const sDX8_HDLE *hdle,
                            uint_t scn_cw, uint_t scn_ch, bool_t full);
/* 生成 DX8 图形离屏表面 */
CR_API iGFX2_DX8S*  create_dx8_bitmap (iGFX2_DX8M *device,
                                       sD3D8_TEXR *texture,
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
do_dx8_enter (
  __CR_IN__ iGFX2_DX8M* dst
    )
{
    dst->m_main->dev->BeginScene();
    dst->m_sprt->Begin();
}

/*
=======================================
    结束绘制
=======================================
*/
cr_inline void_t
do_dx8_leave (
  __CR_IN__ iGFX2_DX8M* dst
    )
{
    dst->m_sprt->End();
    dst->m_main->dev->EndScene();
}

#endif  /* !_CR_SICK_INLINE_ */

CR_API bool_t   fill_dx8_draw (const iGFX2_DX8M *dst,
                               const sFILL *fill, cl32_t color);

CR_API bool_t   blit_dx8_copy (const iGFX2_DX8M *dst,
                               const iGFX2_DX8S *src,
                               const sBLIT *blit, cl32_t color);

CR_API bool_t   blit_dx8_zoom (const iGFX2_DX8M *dst,
                               const iGFX2_DX8S *src,
                               const sZOOM *zoom, cl32_t color);

CR_API bool_t   blit_dx8_rote (const iGFX2_DX8M *dst,
                               const iGFX2_DX8S *src,
                               const sBLIT *blit, fp32_t cx, fp32_t cy,
                               fp32_t ccw, cl32_t color);

CR_API bool_t   blit_dx8_full (const iGFX2_DX8M *dst,
                               const iGFX2_DX8S *src,
                               const sZOOM *zoom, fp32_t cx, fp32_t cy,
                               fp32_t ccw, cl32_t color);

CR_API bool_t   blit_dx8_matx (const iGFX2_DX8M *dst,
                               const iGFX2_DX8S *src,
                               const RECT *rect, const D3DXMATRIX *matx,
                               cl32_t color);

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

/* DX8 调用接口表 */
typedef struct
{
    /* 离屏表面 */
    iGFX2_DX8S* (*create_bitmap) (iGFX2_DX8M *device,
                                  sD3D8_TEXR *texture,
                                  bool_t dynamic);
    /* 模式设置 */
    void_t  (*do_enter) (iGFX2_DX8M *dst);
    void_t  (*do_leave) (iGFX2_DX8M *dst);

    /* 原生绘图 */
    bool_t  (*fill_draw) (const iGFX2_DX8M *dst, const sFILL *fill,
                          cl32_t color);

    bool_t  (*blit_copy) (const iGFX2_DX8M *dst, const iGFX2_DX8S *src,
                          const sBLIT *blit, cl32_t color);

    bool_t  (*blit_zoom) (const iGFX2_DX8M *dst, const iGFX2_DX8S *src,
                          const sZOOM *zoom, cl32_t color);

    bool_t  (*blit_rote) (const iGFX2_DX8M *dst, const iGFX2_DX8S *src,
                          const sBLIT *blit, fp32_t cx, fp32_t cy,
                          fp32_t ccw, cl32_t color);

    bool_t  (*blit_full) (const iGFX2_DX8M *dst, const iGFX2_DX8S *src,
                          const sZOOM *zoom, fp32_t cx, fp32_t cy,
                          fp32_t ccw, cl32_t color);

    bool_t  (*blit_matx) (const iGFX2_DX8M *dst, const iGFX2_DX8S *src,
                          const RECT *rect, const D3DXMATRIX *matx,
                          cl32_t color);
} sDX8_CALL;

/* 获取 DX8 调用接口表 */
CR_API const sDX8_CALL* dx8call_get (void_t);

#endif  /* _CR_HAVE_D3D8_ */

#endif  /* !__CR_DX8WIN_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
