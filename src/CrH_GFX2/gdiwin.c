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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack GDI 图形绘制接口实现 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "pixels.h"
#include "strlib.h"
#include "gfx2/gdiwin.h"

/* 透明色 & 混合导入库 */
#ifndef _CR_NO_PRAGMA_LIB_
    #if !defined(_CR_OS_WINCE_)
        #pragma comment (lib, "msimg32.lib")
    #endif
#endif

/* Windows CE 下没有这个宏 */
#ifndef HGDI_ERROR
    #define HGDI_ERROR  ((HGDIOBJ)GDI_ERROR)
#endif

/* BCB2006 以下 C 编译时会出一个警告 */
#if defined(_CR_CC_BCC_) && (_CR_CC_VER_ < 0x590)
    WINGDIAPI BOOL  WINAPI GdiFlush(VOID);
#endif

/* 64位下会报 int 到 HANDLE 的警告 */
#if defined(_CR_OS_WIN64_)
    #undef  HGDI_ERROR
    #define HGDI_ERROR  (LongToHandle(0xFFFFFFFFL))
#endif

/*****************************************************************************/
/*                                 绘制接口                                  */
/*****************************************************************************/

/*
---------------------------------------
    释放图形绘制接口
---------------------------------------
*/
static void_t
iGFX2_GDI_release (
  __CR_IN__ iGFX2*  that
    )
{
    iGFX2_GDI*  real;

    real = (iGFX2_GDI*)that;
    DeleteObject(real->m_hbmp);
    DeleteDC(real->m_back);

    if (real->m_hwnd != NULL)
        ReleaseDC(real->m_hwnd, real->m_main);
    mem_free(that);
}

/*
---------------------------------------
    获取扩展的绘制接口
---------------------------------------
*/
static void_t*
iGFX2_GDI_getMore (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iGFX2::GDI") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    改变屏幕大小
---------------------------------------
*/
static bool_t
iGFX2_GDI_reset (
  __CR_IN__ iGFX2*  that
    )
{
    RECT        rect;
    iGFX2_GDI*  next;
    iGFX2_GDI*  real = (iGFX2_GDI*)that;

    /* 重新获取窗口大小 */
    if (!GetClientRect(real->m_hwnd, &rect))
        return (FALSE);

    /* 如果发生了改变则重新生成后台缓冲 */
    if ((uint_t)rect.right  != that->__back__.position.ww ||
        (uint_t)rect.bottom != that->__back__.position.hh)
    {
        next = create_gdi_bitmap(rect.right, rect.bottom, CR_UNKNOWN);
        if (next == NULL)
            return (FALSE);
        DeleteObject(real->m_hbmp);
        DeleteDC(real->m_back);

        /* 复制新的数据后释放新结构 */
        real->m_back = next->m_back;
        real->m_hbmp = next->m_hbmp;
        struct_cpy(&real->__back__, &next->__back__, sIMAGE);
        mem_free(next);
    }
    return (TRUE);
}

/*
---------------------------------------
    锁住后台缓冲
---------------------------------------
*/
static sIMAGE*
iGFX2_GDI_lock (
  __CR_IN__ iGFX2*  that
    )
{
#ifndef _CR_OS_WINCE_
    GdiFlush();
#endif
    return (&that->__back__);
}

/*
---------------------------------------
    解锁后台缓冲
---------------------------------------
*/
static void_t
iGFX2_GDI_unlock (
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
iGFX2_GDI_flip (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ bool_t  sync
    )
{
    iGFX2_GDI*  real = (iGFX2_GDI*)that;

    if (!BitBlt(real->m_main, 0, 0, real->__back__.position.ww,
                real->__back__.position.hh, real->m_back, 0, 1, SRCCOPY))
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
iGFX2_GDI_clear (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    RECT        pos;
    HBRUSH      hbr;
    iGFX2_GDI*  real;

    hbr = CreateSolidBrush(argb32_to_gdi(&color));
    if (hbr == NULL)
        return (FALSE);

    real = (iGFX2_GDI*)that;
    pos.left   = 0;
    pos.top    = 1;     /* 这里的加1很重要 */
    pos.right  = real->__back__.position.ww;
    pos.bottom = real->__back__.position.hh + 1;
    if (!FillRect(real->m_back, &pos, hbr)) {
        DeleteObject(hbr);
        return (FALSE);
    }
    DeleteObject(hbr);
    CR_NOUSE(param);
    return (TRUE);
}

/*
---------------------------------------
    设置后台调色板
---------------------------------------
*/
static bool_t
iGFX2_GDI_setPal (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ uint_t  start,
  __CR_IN__ uint_t  count
    )
{
    uint_t      type;
    iGFX2_GDI*  real;

    type = that->__back__.fmt;
    switch (type)
    {
        case CR_INDEX1: type =   2; break;
        case CR_INDEX4: type =  16; break;
        case CR_INDEX8: type = 256; break;

        default:
            return (FALSE);
    }

    /* 设置调色板 */
    if (start >= type)
        start = 0;
    if (count > type - start)
        count = type - start;

    real = (iGFX2_GDI*)that;
    if (SetDIBColorTable(real->m_back, start, count,
            (RGBQUAD*)(&real->__back__.pal[start])) != count)
        return (FALSE);
    return (TRUE);
}

/*
---------------------------------------
    离屏表面使用的空函数
---------------------------------------
*/
static bool_t
iGFX2_GDI_surf_reset (
  __CR_IN__ iGFX2*  that
    )
{
    CR_NOUSE(that);
    return (FALSE);
}

/*
---------------------------------------
    离屏表面使用的空函数
---------------------------------------
*/
static bool_t
iGFX2_GDI_surf_flip (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ bool_t  sync
    )
{
    CR_NOUSE(that);
    CR_NOUSE(sync);
    return (FALSE);
}

/* 接口虚函数表 */
static const iGFX2_vtbl s_canvas_vtbl =
{
    iGFX2_GDI_release, iGFX2_GDI_getMore,
    iGFX2_GDI_reset, iGFX2_GDI_lock, iGFX2_GDI_unlock,
    iGFX2_GDI_flip, iGFX2_GDI_clear, iGFX2_GDI_setPal,
};

static const iGFX2_vtbl s_bitmap_vtbl =
{
    iGFX2_GDI_release, iGFX2_GDI_getMore,
    iGFX2_GDI_surf_reset, iGFX2_GDI_lock, iGFX2_GDI_unlock,
    iGFX2_GDI_surf_flip, iGFX2_GDI_clear, iGFX2_GDI_setPal,
};

/*
=======================================
    生成 GDI 图形绘制接口
=======================================
*/
CR_API iGFX2_GDI*
create_gdi_canvas (
  __CR_IN__ HWND    hwnd,
  __CR_IN__ uint_t  scn_cw,
  __CR_IN__ uint_t  scn_ch,
  __CR_IN__ bool_t  full
    )
{
    HDC         hdc;
    RECT        rect;
    iGFX2_GDI*  canvas;

    /* 全屏时重设窗口大小 */
    if (full) {
        scn_cw = GetSystemMetrics(SM_CXSCREEN);
        scn_ch = GetSystemMetrics(SM_CYSCREEN);
        if (!SetWindowPos(hwnd, HWND_TOP, 0, 0,
                          scn_cw, scn_ch, SWP_SHOWWINDOW))
            return (NULL);
    }
    else if (scn_cw == 0 || scn_ch == 0)
    {
        /* 非法宽高, 获取窗口大小 */
        if (!GetClientRect(hwnd, &rect))
            return (NULL);
        scn_cw = rect.right;
        scn_ch = rect.bottom;
    }

    /* 获取窗口 hDC */
    hdc = GetDC(hwnd);
    if (hdc == NULL)
        return (NULL);

    /* 生成后台缓冲表面 */
    canvas = create_gdi_bitmap(scn_cw, scn_ch, CR_UNKNOWN);
    if (canvas == NULL) {
        ReleaseDC(hwnd, hdc);
        return (NULL);
    }
    canvas->m_main = hdc;
    canvas->m_hwnd = hwnd;
    canvas->__vptr__ = &s_canvas_vtbl;
    return (canvas);
}

/*
=======================================
    生成 GDI 图形离屏表面
=======================================
*/
CR_API iGFX2_GDI*
create_gdi_bitmap (
  __CR_IN__ uint_t  width,
  __CR_IN__ uint_t  height,
  __CR_IN__ uint_t  crh_fmt
    )
{
    HDC         desktop;
    uint_t      bmp_bpp;
    uint_t      pal_num;
    void_t*     bmpdata;
    iGFX2_GDI*  surface;
    BITMAPINFO* bmpinfo;

    if (crh_fmt != CR_UNKNOWN &&
        crh_fmt != CR_INDEX1 && crh_fmt != CR_INDEX4 &&
        crh_fmt != CR_INDEX8 && crh_fmt != CR_ARGBX555 &&
        crh_fmt != CR_ARGB1555 && crh_fmt != CR_ARGB565 &&
        crh_fmt != CR_ARGB888 && crh_fmt != CR_ARGB8888)
        return (NULL);

    surface = struct_new(iGFX2_GDI);
    if (surface == NULL)
        return (NULL);

    desktop = GetDC(GetDesktopWindow());
    if (desktop == NULL)
        goto _failure1;

    if (crh_fmt == CR_INDEX1)
    {
        bmp_bpp = 1;
    }
    else
    if (crh_fmt == CR_INDEX4)
    {
        bmp_bpp = 4;
    }
    else
    if (crh_fmt == CR_ARGBX555 ||
        crh_fmt == CR_ARGB1555)
    {
        bmp_bpp = 16;
    }
    else
    if (crh_fmt == CR_UNKNOWN)
    {
        /* 格式与系统一致 */
        bmp_bpp = GetDeviceCaps(desktop, PLANES);
        if (bmp_bpp != 1)
            goto _failure2;

        bmp_bpp = GetDeviceCaps(desktop, BITSPIXEL);
        crh_fmt = bmp_bpp;

#if 0   /* 此部分暂未实现, 因为大多数屏都是 565 的 */
        if (bmp_bpp == 16)
        {
            /* 检测是否为 555 格式 */
            if (0) {
                crh_fmt = CR_ARGBX555;
            }
        }
#endif
    }
    else
    {
        bmp_bpp = crh_fmt;  /* 这些格式是一一对应的 */
    }

    /* 填充位图头结构 */
    bmpinfo = (BITMAPINFO*)mem_malloc(sizeof(BITMAPINFO) +
                                      sizeof(RGBQUAD) * 255);
    if (bmpinfo == NULL)
        goto _failure2;
    mem_zero(bmpinfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD) * 255);
    bmpinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpinfo->bmiHeader.biWidth  = width;
    bmpinfo->bmiHeader.biHeight = height + 1;   /* 多分配一行, 由于是
                                                   颠倒的关系, Y 从1开始 */
    bmpinfo->bmiHeader.biPlanes = 1;
    bmpinfo->bmiHeader.biBitCount = (WORD)bmp_bpp;

    pal_num = 256;
    if (bmp_bpp == 8) {
        bmpinfo->bmiHeader.biClrUsed = 256;
    }
    else
    if (bmp_bpp == 4) {
        pal_num = 16;
        crh_fmt = CR_INDEX4;
        bmpinfo->bmiHeader.biClrUsed = 16;
    }
    else
    if (bmp_bpp == 1) {
        pal_num = 2;
        crh_fmt = CR_INDEX1;
        bmpinfo->bmiHeader.biClrUsed = 2;
    }

#if defined(_CR_OS_WINCE_)
    if (bmp_bpp == 16 || bmp_bpp == 32)
#else
    if (crh_fmt == CR_ARGB565)  /* 9x/NT 只有 565 需要用 BI_BITFIELDS 格式 */
#endif
    {
#if defined(_CR_OS_WINCE_)
        if (crh_fmt == CR_ARGB8888)
        {
            bmpinfo->bmiColors[0].rgbBlue     = 0x00;
            bmpinfo->bmiColors[0].rgbGreen    = 0x00;
            bmpinfo->bmiColors[0].rgbRed      = 0xFF;
            bmpinfo->bmiColors[0].rgbReserved = 0x00;
            bmpinfo->bmiColors[1].rgbBlue     = 0x00;
            bmpinfo->bmiColors[1].rgbGreen    = 0xFF;
            bmpinfo->bmiColors[1].rgbRed      = 0x00;
            bmpinfo->bmiColors[1].rgbReserved = 0x00;
            bmpinfo->bmiColors[2].rgbBlue     = 0xFF;
            bmpinfo->bmiColors[2].rgbGreen    = 0x00;
            bmpinfo->bmiColors[2].rgbRed      = 0x00;
            bmpinfo->bmiColors[2].rgbReserved = 0x00;
        }
        else
        if (crh_fmt == CR_ARGB565)
        {
#endif
            bmpinfo->bmiColors[0].rgbBlue     = 0x00;
            bmpinfo->bmiColors[0].rgbGreen    = 0xF8;
            bmpinfo->bmiColors[0].rgbRed      = 0x00;
            bmpinfo->bmiColors[0].rgbReserved = 0x00;
            bmpinfo->bmiColors[1].rgbBlue     = 0xE0;
            bmpinfo->bmiColors[1].rgbGreen    = 0x07;
            bmpinfo->bmiColors[1].rgbRed      = 0x00;
            bmpinfo->bmiColors[1].rgbReserved = 0x00;
            bmpinfo->bmiColors[2].rgbBlue     = 0x1F;
            bmpinfo->bmiColors[2].rgbGreen    = 0x00;
            bmpinfo->bmiColors[2].rgbRed      = 0x00;
            bmpinfo->bmiColors[2].rgbReserved = 0x00;

#if defined(_CR_OS_WINCE_)
        }
        else /*
        if (crh_fmt == CR_ARGBX555 ||
            crh_fmt == CR_ARGB1555) */
        {
            bmpinfo->bmiColors[0].rgbBlue     = 0x00;
            bmpinfo->bmiColors[0].rgbGreen    = 0x7C;
            bmpinfo->bmiColors[0].rgbRed      = 0x00;
            bmpinfo->bmiColors[0].rgbReserved = 0x00;
            bmpinfo->bmiColors[1].rgbBlue     = 0xE0;
            bmpinfo->bmiColors[1].rgbGreen    = 0x03;
            bmpinfo->bmiColors[1].rgbRed      = 0x00;
            bmpinfo->bmiColors[1].rgbReserved = 0x00;
            bmpinfo->bmiColors[2].rgbBlue     = 0x1F;
            bmpinfo->bmiColors[2].rgbGreen    = 0x00;
            bmpinfo->bmiColors[2].rgbRed      = 0x00;
            bmpinfo->bmiColors[2].rgbReserved = 0x00;
        }
#endif
        bmpinfo->bmiHeader.biCompression = BI_BITFIELDS;
    }
    else
    {
        bmpinfo->bmiHeader.biCompression = BI_RGB;  /* 普通 RGB 模式 */
    }

    /* 生成 DIB 位图 */
    surface->m_back = CreateCompatibleDC(desktop);
    if (surface->m_back == NULL)
        goto _failure3;

    surface->m_hbmp = CreateDIBSection(surface->m_back, bmpinfo,
                                       DIB_RGB_COLORS, &bmpdata, NULL, 0);
    if (surface->m_hbmp == NULL)
        goto _failure4;

    /* hDC 选入位图 */
    if (SelectObject(surface->m_back, surface->m_hbmp) == HGDI_ERROR)
        goto _failure5;

    /* 设置 sIMAGE 结构 */
    if (!image_set(&surface->__back__, bmpdata, (leng_t)(-1L),
                   0, 0, width, height, crh_fmt, TRUE, 4))
        goto _failure5;

    /* 获取调色板数据 (如果需要的话) */
    if (crh_fmt <= CR_INDEX8) {
        if (GetDIBColorTable(surface->m_back, 0, pal_num, (RGBQUAD*)
                                (&surface->__back__.pal)) != pal_num)
            goto _failure5;
    }
    surface->m_main = NULL;
    surface->m_hwnd = NULL;
    surface->__vptr__ = &s_bitmap_vtbl;
    mem_free(bmpinfo);
    ReleaseDC(GetDesktopWindow(), desktop);
    return (surface);

_failure5:
    DeleteObject(surface->m_hbmp);
_failure4:
    DeleteDC(surface->m_back);
_failure3:
    mem_free(bmpinfo);
_failure2:
    ReleaseDC(GetDesktopWindow(), desktop);
_failure1:
    mem_free(surface);
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
    return ((iGFX2*)create_gdi_canvas((HWND)handle,
                        scn_cw, scn_ch, full));
}
#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/*                                 原生绘制                                  */
/*****************************************************************************/

/*
=======================================
    直接填充操作
=======================================
*/
CR_API bool_t
fill_gdi_draw (
  __CR_IO__ const iGFX2_GDI*    dst,
  __CR_IN__ const sFILL*        fill,
  __CR_IN__ cl32_t              color
    )
{
    RECT    pos;
    HBRUSH  hbr;

    hbr = CreateSolidBrush(argb32_to_gdi(&color));
    if (hbr == NULL)
        return (FALSE);

    pos.left = fill->dx;
    pos.top  = fill->dy + 1;        /* 这里的加1很重要 */
    pos.right  = pos.left + fill->dw;
    pos.bottom = pos.top  + fill->dh;
    if (!FillRect(dst->m_back, &pos, hbr)) {
        DeleteObject(hbr);
        return (FALSE);
    }
    DeleteObject(hbr);
    return (TRUE);
}

/*
=======================================
    直接 BLT 操作
=======================================
*/
CR_API bool_t
blit_gdi_copy (
  __CR_IO__ const iGFX2_GDI*    dst,
  __CR_IN__ const iGFX2_GDI*    src,
  __CR_IN__ const sBLIT*        blit
    )
{
                                    /* 这里的加1很重要 */
    if (!BitBlt(dst->m_back, blit->dx, blit->dy + 1,
                             blit->sw, blit->sh,
                src->m_back, blit->sx, blit->sy + 1, SRCCOPY))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    缩放 BLT 操作
=======================================
*/
CR_API bool_t
blit_gdi_zoom (
  __CR_IO__ const iGFX2_GDI*    dst,
  __CR_IN__ const iGFX2_GDI*    src,
  __CR_IN__ const sZOOM*        zoom
    )
{
                                    /* 这里的加1很重要 */
    if (!StretchBlt(dst->m_back, zoom->dx, zoom->dy + 1,
                                 zoom->dw, zoom->dh,
                    src->m_back, zoom->sx, zoom->sy + 1,
                                 zoom->sw, zoom->sh, SRCCOPY))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    透明 BLT 操作
=======================================
*/
CR_API bool_t
blit_gdi_tran (
  __CR_IO__ const iGFX2_GDI*    dst,
  __CR_IN__ const iGFX2_GDI*    src,
  __CR_IN__ const sBLIT*        blit,
  __CR_IN__ cl32_t              trans
    )
{
    /* Windows CE 4.0+ 支持 */
#if !defined(_CR_OS_WINCE_) || (_WIN32_WCE >= 0x0400)

    if (trans == 0x00000000UL)
        return (blit_gdi_copy(dst, src, blit));

                                    /* 这里的加1很重要 */
    if (!TransparentBlt(dst->m_back, blit->dx, blit->dy + 1,
                                     blit->sw, blit->sh,
                        src->m_back, blit->sx, blit->sy + 1,
                                     blit->sw, blit->sh,
                        argb32_to_gdi(&trans)))
        return (FALSE);
    return (TRUE);
#else

    CR_NOUSE(dst);
    CR_NOUSE(src);
    CR_NOUSE(blit);
    CR_NOUSE(trans);
    return (FALSE);

#endif  /* OS TYPE predefines */
}

/*
=======================================
    Alpha 混合 BLT 操作
=======================================
*/
CR_API bool_t
blit_gdi_blend (
  __CR_IO__ const iGFX2_GDI*    dst,
  __CR_IN__ const iGFX2_GDI*    src,
  __CR_IN__ const sBLIT*        blit
    )
{
    /* Windows CE 5.0+ 支持 */
#if !defined(_CR_OS_WINCE_) || (_WIN32_WCE >= 0x0500)

    BLENDFUNCTION   blend;

    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = 0xFF;
    blend.AlphaFormat = AC_SRC_ALPHA;
                                    /* 这里的加1很重要 */
    if (!AlphaBlend(dst->m_back, blit->dx, blit->dy + 1,
                                 blit->sw, blit->sh,
                    src->m_back, blit->sx, blit->sy + 1,
                                 blit->sw, blit->sh, blend))
        return (FALSE);
    return (TRUE);
#else

    CR_NOUSE(dst);
    CR_NOUSE(src);
    CR_NOUSE(blit);
    return (FALSE);

#endif  /* OS TYPE predefines */
}

/*
=======================================
    Alpha 混合 BLT 操作
=======================================
*/
CR_API bool_t
blit_gdi_alpha (
  __CR_IO__ const iGFX2_GDI*    dst,
  __CR_IN__ const iGFX2_GDI*    src,
  __CR_IN__ const sBLIT*        blit,
  __CR_IN__ uint_t              alpha
    )
{
    /* Windows CE 5.0+ 支持 */
#if !defined(_CR_OS_WINCE_) || (_WIN32_WCE >= 0x0500)

    BLENDFUNCTION   blend;

    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = (BYTE)alpha;
    blend.AlphaFormat = 0;
                                    /* 这里的加1很重要 */
    if (!AlphaBlend(dst->m_back, blit->dx, blit->dy + 1,
                                 blit->sw, blit->sh,
                    src->m_back, blit->sx, blit->sy + 1,
                                 blit->sw, blit->sh, blend))
        return (FALSE);
    return (TRUE);
#else

    CR_NOUSE(dst);
    CR_NOUSE(src);
    CR_NOUSE(blit);
    CR_NOUSE(alpha);
    return (FALSE);

#endif  /* OS TYPE predefines */
}

/*****************************************************************************/
/*                                 文字接口                                  */
/*****************************************************************************/

/*
---------------------------------------
    释放文字绘制接口
---------------------------------------
*/
static void_t
iFONT_GDI_release (
  __CR_IN__ iFONT*  that
    )
{
    iFONT_GDI*  real;

    real = (iFONT_GDI*)that;
    DeleteObject(real->m_font);
    mem_free(that);
}

/*
---------------------------------------
    获取扩展的文字接口
---------------------------------------
*/
static void_t*
iFONT_GDI_getMore (
  __CR_IN__ iFONT*  that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iFONT::GDI") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    开始批绘制
---------------------------------------
*/
static void_t
iFONT_GDI_enter (
  __CR_IN__ iFONT*  that
    )
{
    CR_NOUSE(that);
}

/*
---------------------------------------
    结束批绘制
---------------------------------------
*/
static void_t
iFONT_GDI_leave (
  __CR_IN__ iFONT*  that
    )
{
#ifndef _CR_OS_WINCE_
    GdiFlush();
#endif
    CR_NOUSE(that);
}

/*
---------------------------------------
    绑定绘制接口
---------------------------------------
*/
static bool_t
iFONT_GDI_bind (
  __CR_IO__ iFONT*  that,
  __CR_IN__ iGFX2*  gfx2
    )
{
    iFONT_GDI*  real;
    iGFX2_GDI*  gfx2_gdi;

    gfx2_gdi = (iGFX2_GDI*)(CR_VCALL(gfx2)->getMore(gfx2, "iGFX2::GDI"));
    if (gfx2_gdi == NULL)
        return (FALSE);
    real = (iFONT_GDI*)that;

    /* 字体选入位图 */
    if (SelectObject(gfx2_gdi->m_back, real->m_font) == HGDI_ERROR)
        return (FALSE);
    real->m_draw = gfx2_gdi->m_back;
    return (TRUE);
}

/*
---------------------------------------
    设置绘制模式 (平台相关)
---------------------------------------
*/
static bool_t
iFONT_GDI_setMode (
  __CR_IO__ iFONT*  that,
  __CR_IN__ int32u  mode
    )
{
    iFONT_GDI*  real = (iFONT_GDI*)that;

    if (real->m_draw == NULL)
        return (FALSE);
    if (!SetBkMode(real->m_draw, (int)mode))
        return (FALSE);
    real->__draw_mode__ = mode;
    return (TRUE);
}

/*
---------------------------------------
    设置前景色 (标准32位色)
---------------------------------------
*/
static bool_t
iFONT_GDI_setColor (
  __CR_IO__ iFONT*  that,
  __CR_IN__ cl32_t  color
    )
{
    /* Windows CE 4.0+ 支持 */
#if !defined(_CR_OS_WINCE_) || (_WIN32_WCE >= 0x0400)

    COLORREF    temp;
    iFONT_GDI*  real = (iFONT_GDI*)that;

    if (real->m_draw == NULL)
        return (FALSE);
    temp = SetTextColor(real->m_draw, argb32_to_gdi(&color));
    if (temp == CLR_INVALID)
        return (FALSE);
    real->__color__ = color;    /* 这里已经转换过了 */
    return (TRUE);
#else

    CR_NOUSE(that);
    CR_NOUSE(color);
    return (FALSE);

#endif  /* OS TYPE predefines */
}

/*
---------------------------------------
    设置背景色 (标准32位色)
---------------------------------------
*/
static bool_t
iFONT_GDI_setBkColor (
  __CR_IO__ iFONT*  that,
  __CR_IN__ cl32_t  color
    )
{
    COLORREF    temp;
    iFONT_GDI*  real = (iFONT_GDI*)that;

    if (real->m_draw == NULL)
        return (FALSE);
    temp = SetBkColor(real->m_draw, argb32_to_gdi(&color));
    if (temp == CLR_INVALID)
        return (FALSE);
    real->__bkcolor__ = color;  /* 这里已经转换过了 */
    return (TRUE);
}

/*
---------------------------------------
    绘制文字 (透明)
---------------------------------------
*/
static bool_t
iFONT_GDI_draw_tran (
  __CR_IN__ iFONT*          that,
  __CR_IN__ const void_t*   text,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          cpage
    )
{
    RECT        temp;
    sint_t      mode;
    leng_t      leng;
    bool_t      rett;
    wide_t*     utf16;
    iFONT_GDI*  real = (iFONT_GDI*)that;

    if (real->m_draw == NULL)
        return (FALSE);
    mode = SetBkMode(real->m_draw, TRANSPARENT);
    if (mode == 0)
        return (FALSE);
    temp.left   = rect->x1;
    temp.top    = rect->y1 + 1;     /* 这里的加1很重要 */
    temp.right  = rect->x2 + 1;
    temp.bottom = rect->y2 + 2;

    /* 统一转换到 UTF-16 */
    if (cpage == CR_UTF16X) {
        utf16 = (wide_t*)text;
        leng = str_lenW(utf16);
    }
    else {
        utf16 = (wide_t*)str_acp2uni(cpage, (ansi_t*)text, &leng, TRUE);
        if (utf16 == NULL) {
            rett = FALSE;
            goto _func_out;
        }
        leng = leng / sizeof(wide_t) - 1;   /* 去掉后面的 NIL 字符 */
    }

    /* 支持输出多行文本 */
    if (!DrawTextW(real->m_draw, utf16, (int)leng, &temp, DT_LEFT))
        rett = FALSE;
    else
        rett = TRUE;

    if (utf16 != (wide_t*)text)
        mem_free(utf16);
_func_out:
    if (mode != TRANSPARENT)
        SetBkMode(real->m_draw, mode);
    return (rett);
}

/*
---------------------------------------
    绘制文字 (实体)
---------------------------------------
*/
static bool_t
iFONT_GDI_draw_text (
  __CR_IN__ iFONT*          that,
  __CR_IN__ const void_t*   text,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          cpage
    )
{
    RECT        temp;
    leng_t      leng;
    bool_t      rett;
    wide_t*     utf16;
    iFONT_GDI*  real = (iFONT_GDI*)that;

    if (real->m_draw == NULL)
        return (FALSE);
    temp.left   = rect->x1;
    temp.top    = rect->y1 + 1;     /* 这里的加1很重要 */
    temp.right  = rect->x2 + 1;
    temp.bottom = rect->y2 + 2;

    /* 统一转换到 UTF-16 */
    if (cpage == CR_UTF16X) {
        utf16 = (wide_t*)text;
        leng = str_lenW(utf16);
    }
    else {
        utf16 = (wide_t*)str_acp2uni(cpage, (ansi_t*)text, &leng, TRUE);
        if (utf16 == NULL)
            return (FALSE);
        leng = leng / sizeof(wide_t) - 1;   /* 去掉后面的 NIL 字符 */
    }

    /* 支持输出多行文本 */
    if (!DrawTextW(real->m_draw, utf16, (int)leng, &temp, DT_LEFT))
        rett = FALSE;
    else
        rett = TRUE;

    if (utf16 != (wide_t*)text)
        mem_free(utf16);
    return (rett);
}

/*
---------------------------------------
    测量文字输出范围
---------------------------------------
*/
static bool_t
iFONT_GDI_calc_rect (
  __CR_IN__ iFONT*          that,
  __CR_IN__ const void_t*   text,
  __CR_IO__ sRECT*          rect,
  __CR_IN__ uint_t          cpage
    )
{
    RECT        temp;
    leng_t      leng;
    bool_t      rett;
    wide_t*     utf16;
    iFONT_GDI*  real = (iFONT_GDI*)that;

    if (real->m_draw == NULL)
        return (FALSE);
    temp.left   = 0;
    temp.top    = 0;
    temp.right  = rect->ww;
    temp.bottom = rect->hh;

    /* 统一转换到 UTF-16 */
    if (cpage == CR_UTF16X) {
        leng  = (leng_t)-1;
        utf16 = (wide_t*)text;
    }
    else {
        utf16 = (wide_t*)str_acp2uni(cpage, (ansi_t*)text, &leng, TRUE);
        if (utf16 == NULL)
            return (FALSE);
        leng = leng / sizeof(wide_t) - 1;   /* 去掉后面的 NIL 字符 */
    }

    /* 测量文字输出范围 */
    if (!DrawTextW(real->m_draw, utf16, (int)leng, &temp,
                   DT_LEFT | DT_CALCRECT)) {
        rett = FALSE;
    }
    else {
        rett = TRUE;
        rect_set_wh(rect, rect->x1, rect->y1, temp.right, temp.bottom);
    }

    if (utf16 != (wide_t*)text)
        mem_free(utf16);
    return (rett);
}

/* 接口虚函数表 */
static const iFONT_vtbl s_font_vtbl =
{
    iFONT_GDI_release, iFONT_GDI_getMore,
    iFONT_GDI_enter, iFONT_GDI_leave, iFONT_GDI_bind,
    iFONT_GDI_setMode, iFONT_GDI_setColor, iFONT_GDI_setBkColor,
    iFONT_GDI_draw_tran, iFONT_GDI_draw_text, iFONT_GDI_calc_rect,
};

/*
=======================================
    生成 GDI 文字绘制接口A
=======================================
*/
CR_API iFONT*
create_gdi_fontA (
  __CR_IN__ const LOGFONTA* lplf
    )
{
    iFONT_GDI*  font;

    font = struct_new(iFONT_GDI);
    if (font == NULL)
        return (NULL);
    struct_zero(font, iFONT_GDI);

    /* 生成字体对象 */
    font->m_font = CreateFontIndirectA(lplf);
    if (font->m_font == NULL) {
        mem_free(font);
        return (NULL);
    }
    font->__vptr__ = &s_font_vtbl;
    return ((iFONT*)font);
}

/*
=======================================
    生成 GDI 文字绘制接口W
=======================================
*/
CR_API iFONT*
create_gdi_fontW (
  __CR_IN__ const LOGFONTW* lplf
    )
{
    iFONT_GDI*  font;

    font = struct_new(iFONT_GDI);
    if (font == NULL)
        return (NULL);
    struct_zero(font, iFONT_GDI);

    /* 生成字体对象 */
    font->m_font = CreateFontIndirectW(lplf);
    if (font->m_font == NULL) {
        mem_free(font);
        return (NULL);
    }
    font->__vptr__ = &s_font_vtbl;
    return ((iFONT*)font);
}

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

static const sGDI_CALL s_gdi_call =
{
    /* 创建 */
    create_gdi_bitmap,
    create_gdi_fontA,
    create_gdi_fontW,

    /* 绘制 */
    fill_gdi_draw,
    blit_gdi_copy,
    blit_gdi_zoom,
    blit_gdi_tran,
    blit_gdi_blend,
    blit_gdi_alpha,
};

/*
=======================================
    获取 GDI 调用接口表
=======================================
*/
CR_API const sGDI_CALL*
gdi_call_get (void_t)
{
    return (&s_gdi_call);
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
