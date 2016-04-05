/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-07  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack GDI 图形绘制接口定义 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_GDIWIN_H__
#define __CR_GDIWIN_H__

#include "../gfx2.h"

#include <windows.h>

/*****************************************************************************/
/*                                 绘制接口                                  */
/*****************************************************************************/

/* GDI 具体实现结构 */
typedef struct
{
        /* 虚函数表 */
        const iGFX2_vtbl*   __vptr__;

        /* 数据成员 */
        sIMAGE  __back__;   /* 显示屏的后台缓冲 */

        /* 个性部分 */
        HDC         m_main;     /* 主表面的 hDC */
        HDC         m_back;     /* 后台表面 hDC */
        HWND        m_hwnd;     /* 主窗口的句柄 */
        HBITMAP     m_hbmp;     /* 后台位图句柄 */

} iGFX2_GDI;

/* 生成 GDI 图形绘制接口 */
CR_API iGFX2_GDI*   create_gdi_canvas (HWND hwnd, uint_t scn_cw,
                                       uint_t scn_ch, bool_t full);
/* 生成 GDI 图形离屏表面 */
CR_API iGFX2_GDI*   create_gdi_bitmap (uint_t width, uint_t height,
                                       uint_t crh_fmt);

/*****************************************************************************/
/*                                 原生绘制                                  */
/*****************************************************************************/

CR_API bool_t   fill_gdi_draw (const iGFX2_GDI *dst, const sFILL *fill,
                               cl32_t color);
CR_API bool_t   blit_gdi_copy (const iGFX2_GDI *dst, const iGFX2_GDI *src,
                               const sBLIT *blit);
CR_API bool_t   blit_gdi_zoom (const iGFX2_GDI *dst, const iGFX2_GDI *src,
                               const sZOOM *zoom);
CR_API bool_t   blit_gdi_tran (const iGFX2_GDI *dst, const iGFX2_GDI *src,
                               const sBLIT *blit, cl32_t trans);
CR_API bool_t   blit_gdi_blend (const iGFX2_GDI *dst, const iGFX2_GDI *src,
                                const sBLIT *blit);
CR_API bool_t   blit_gdi_alpha (const iGFX2_GDI *dst, const iGFX2_GDI *src,
                                const sBLIT *blit, uint_t alpha);

/*****************************************************************************/
/*                                 文字接口                                  */
/*****************************************************************************/

/* GDI 具体实现结构 */
typedef struct
{
        /* 虚函数表 */
        const iFONT_vtbl*   __vptr__;

        /* 数据成员 */
        int32u  __draw_mode__;              /* 模式 (平台相关) */
        int32u  __color__, __bkcolor__;     /* 颜色 (平台相关) */

        /* 个性部分 */
        HDC     m_draw;     /* 绘制表面 hDC */
        HFONT   m_font;     /* 绘制字体句柄 */

} iFONT_GDI;

/* 生成 GDI 文字绘制接口 */
CR_API iFONT*   create_gdi_fontA (const LOGFONTA *lplf);
CR_API iFONT*   create_gdi_fontW (const LOGFONTW *lplf);

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

/* GDI 调用接口表 */
typedef struct
{
    /* 离屏表面 */
    iGFX2_GDI*  (*create_bitmap) (uint_t width, uint_t height,
                                  uint_t crh_fmt);
    /* 文字生成 */
    iFONT*  (*create_fontA) (const LOGFONTA *lplf);
    iFONT*  (*create_fontW) (const LOGFONTW *lplf);

    /* 原生绘图 */
    bool_t  (*fill_draw) (const iGFX2_GDI *dst, const sFILL *fill,
                          cl32_t color);

    bool_t  (*blit_copy) (const iGFX2_GDI *dst, const iGFX2_GDI *src,
                          const sBLIT *blit);

    bool_t  (*blit_zoom) (const iGFX2_GDI *dst, const iGFX2_GDI *src,
                          const sZOOM *zoom);

    bool_t  (*blit_tran) (const iGFX2_GDI *dst, const iGFX2_GDI *src,
                          const sBLIT *blit, cl32_t trans);

    bool_t  (*blit_blend) (const iGFX2_GDI *dst, const iGFX2_GDI *src,
                           const sBLIT *blit);

    bool_t  (*blit_alpha) (const iGFX2_GDI *dst, const iGFX2_GDI *src,
                           const sBLIT *blit, uint_t alpha);
} sGDI_CALL;

/* 获取 GDI 调用接口表 */
CR_API const sGDI_CALL* gdi_call_get (void_t);

#endif  /* !__CR_GDIWIN_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
