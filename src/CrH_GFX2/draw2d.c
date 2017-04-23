/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-02-22  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 几何绘制函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "blit.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
=======================================
    画方框
=======================================
*/
CR_API void_t
draw_rect (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ cpix_t          color,
  __CR_IN__ pixdraw_t       pixel_draw
    )
{
    sRECT   clip;
    sint_t  xx, yy;

    /* 剪裁 */
    if (!clip_rect(&clip, rect, &dst->clip_win))
        return;

    /* 上边 */
    if (rect->y1 == clip.y1) {
        yy = clip.y1;
        for (xx = clip.x1; xx <= clip.x2; xx++)
            pixel_draw(dst, xx, yy, color);
    }

    /* 右边 */
    if (rect->x2 == clip.x2) {
        xx = clip.x2;
        for (yy = clip.y1; yy <= clip.y2; yy++)
            pixel_draw(dst, xx, yy, color);
    }

    /* 下边 */
    if (rect->y2 == clip.y2) {
        yy = clip.y2;
        for (xx = clip.x1; xx <= clip.x2; xx++)
            pixel_draw(dst, xx, yy, color);
    }

    /* 左边 */
    if (rect->x1 == clip.x1) {
        xx = clip.x1;
        for (yy = clip.y1; yy <= clip.y2; yy++)
            pixel_draw(dst, xx, yy, color);
    }
}

/*
=======================================
    画线段
=======================================
*/
CR_API void_t
draw_line (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ const sLINE*    pos,
  __CR_IN__ cpix_t          color,
  __CR_IN__ pixdraw_t       pixel_draw
    )
{
    sLINE   draw;
    sint_t  x1, y1, x2, y2;
    sint_t  dx, dy, dx2, dy2;
    sint_t  dd, dxy, xinc, yinc;

    /* 水平 & 垂直检查 */
    if (pos->y1 == pos->y2) {
        draw_lineh(dst, pos->x1, pos->x2, pos->y1, color, pixel_draw);
        return;
    }
    if (pos->x1 == pos->x2) {
        draw_linev(dst, pos->x1, pos->y1, pos->y2, color, pixel_draw);
        return;
    }

    /* 线段剪裁检查 */
    if (!clip_line(&draw, pos, &dst->clip_win))
        return;
    x1 = draw.x1;
    y1 = draw.y1;
    x2 = draw.x2;
    y2 = draw.y2;
    if (x2 > x1) {
        xinc = 1;
        dx = x2 - x1;
    }
    else {
        xinc = -1;
        dx = x1 - x2;
    }
    dx2 = dx * 2;

    if (y2 > y1) {
        yinc = 1;
        dy = y2 - y1;
    }
    else {
        yinc = -1;
        dy = y1 - y2;
    }
    dy2 = dy * 2;
    pixel_draw(dst, x1, y1, color);
    if (dx >= dy)
    {
        dd  = dy2 - dx;
        dxy = dy2 - dx2;
        for (; dx != 0; dx--)
        {
            if (dd <= 0) {
                dd += dy2;
            }
            else {
                dd += dxy;
                y1 += yinc;
            }
            x1 += xinc;
            pixel_draw(dst, x1, y1, color);
        }
    }
    else
    {
        dd  = dx2 - dy;
        dxy = dx2 - dy2;
        for (; dy != 0; dy--)
        {
            if (dd <= 0) {
                dd += dx2;
            }
            else {
                dd += dxy;
                x1 += xinc;
            }
            y1 += yinc;
            pixel_draw(dst, x1, y1, color);
        }
    }
}

/*
=======================================
    画水平线
=======================================
*/
CR_API void_t
draw_lineh (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x1,
  __CR_IN__ sint_t          x2,
  __CR_IN__ sint_t          dy,
  __CR_IN__ cpix_t          color,
  __CR_IN__ pixdraw_t       pixel_draw
    )
{
    sint_t          temp;
    const sRECT*    clip = &dst->clip_win;

    if (dy < clip->y1 || dy > clip->y2)
        return;
    if (x1 > x2) CR_SWAP(x1, x2, temp);

    x1 = CR_MAX(x1, clip->x1);
    x2 = CR_MIN(x2, clip->x2);
    if (x1 > x2)
        return;

    while (x1 <= x2)
        pixel_draw(dst, x1++, dy, color);
}

/*
=======================================
    画垂直线
=======================================
*/
CR_API void_t
draw_linev (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          dx,
  __CR_IN__ sint_t          y1,
  __CR_IN__ sint_t          y2,
  __CR_IN__ cpix_t          color,
  __CR_IN__ pixdraw_t       pixel_draw
    )
{
    sint_t          temp;
    const sRECT*    clip = &dst->clip_win;

    if (dx < clip->x1 || dx > clip->x2)
        return;
    if (y1 > y2) CR_SWAP(y1, y2, temp);

    y1 = CR_MAX(y1, clip->y1);
    y2 = CR_MIN(y2, clip->y2);
    if (y1 > y2)
        return;

    while (y1 <= y2)
        pixel_draw(dst, dx, y1++, color);
}

/*
=======================================
    画折线
=======================================
*/
CR_API void_t
draw_lines (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ const sPNT2*    pos,
  __CR_IN__ uint_t          count,
  __CR_IN__ leng_t          skip,
  __CR_IN__ cpix_t          color,
  __CR_IN__ pixdraw_t       pixel_draw
    )
{
    sLINE   line;
    sPNT2*  next;
    uint_t  index;

    if (count-- <= 1)
        return;
    if (skip == 0)
        skip = sizeof(sPNT2);

    for (index = 0; index < count; index++, pos = next) {
        next = (sPNT2*)((byte_t*)pos + skip);
        line_set_xy(&line, pos->x, pos->y, next->x, next->y);
        draw_line(dst, &line, color, pixel_draw);
    }
}

/*
=======================================
    画正圆
=======================================
*/
CR_API void_t
draw_circle (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ sint_t          cx,
  __CR_IN__ sint_t          cy,
  __CR_IN__ sint_t          radius,
  __CR_IN__ cpix_t          color,
  __CR_IN__ pixdraw_t       pixel_draw
    )
{
    sint_t  xx, yy, dd;
    /* ------------- */
    const sRECT*    clip;

    if (radius <= 0)
        return;
    clip = &dst->clip_win;

    /* 安全检查, 加法溢出无碍 */
    if ((cx > clip->x2 + radius) ||
        (cx + radius < clip->x1) ||
        (cy > clip->y2 + radius) ||
        (cy + radius < clip->y1))
        return;

    yy = radius;
    dd = 3 - radius * 2;

    for (xx = 0; xx <= yy; xx++)
    {
        if (clip_pixel(cx + xx, cy + yy, clip))
            pixel_draw(dst, cx + xx, cy + yy, color);

        if (clip_pixel(cx + xx, cy - yy, clip))
            pixel_draw(dst, cx + xx, cy - yy, color);

        if (clip_pixel(cx - xx, cy - yy, clip))
            pixel_draw(dst, cx - xx, cy - yy, color);

        if (clip_pixel(cx - xx, cy + yy, clip))
            pixel_draw(dst, cx - xx, cy + yy, color);

        if (clip_pixel(cx + yy, cy + xx, clip))
            pixel_draw(dst, cx + yy, cy + xx, color);

        if (clip_pixel(cx + yy, cy - xx, clip))
            pixel_draw(dst, cx + yy, cy - xx, color);

        if (clip_pixel(cx - yy, cy - xx, clip))
            pixel_draw(dst, cx - yy, cy - xx, color);

        if (clip_pixel(cx - yy, cy + xx, clip))
            pixel_draw(dst, cx - yy, cy + xx, color);

        if (dd < 0)
            dd += xx * 4 + 6;
        else
            dd += (xx - yy--) * 4 + 10;
    }
}

/*
=======================================
    画多边形
=======================================
*/
CR_API void_t
draw_polygon (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ const sPNT2*    pos,
  __CR_IN__ uint_t          count,
  __CR_IN__ leng_t          skip,
  __CR_IN__ cpix_t          color,
  __CR_IN__ pixdraw_t       pixel_draw
    )
{
    sLINE   line;
    sPNT2*  next;
    sPNT2*  start;
    uint_t  index;

    if (count-- <= 2)
        return;
    if (skip == 0)
        skip = sizeof(sPNT2);
    start = (sPNT2*)pos;

    for (index = 0; index < count; index++, pos = next) {
        next = (sPNT2*)((byte_t*)pos + skip);
        line_set_xy(&line, pos->x, pos->y, next->x, next->y);
        draw_line(dst, &line, color, pixel_draw);
    }
    line_set_xy(&line, start->x, start->y, pos->x, pos->y);
    draw_line(dst, &line, color, pixel_draw);
}

/*
=======================================
    画椭圆
=======================================
*/
CR_API void_t
draw_ellipse (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ cpix_t          color,
  __CR_IN__ pixdraw_t       pixel_draw
    )
{
    const sRECT*    clip;
    /* --------------- */
    sint_t  cx, cy, r1, r2, rr;
    sint_t  xx, yy, tn, r12, r22, xmax;

    r1 = (rect->ww / 2);
    r2 = (rect->hh / 2);
    cx = (rect->x1 + rect->x2) / 2;
    cy = (rect->y1 + rect->y2) / 2;
    clip = &dst->clip_win;

    /* 安全检查, 加法溢出无碍 */
    if ((cx > clip->x2 + r1) || (cx + r1 < clip->x1) ||
        (cy > clip->y2 + r2) || (cy + r2 < clip->y1))
        return;

    xx = 0;
    yy = r2;
    r12 = r1 * r1;
    r22 = r2 * r2;
    xmax = (sint_t)(r12 / sqrt(r12 + r22));
    tn = r12 - 2 * r2 * r12;
    while (xx <= xmax)
    {
        if (tn < 0 || yy == 0) {
            tn += (4 * xx + 2) * r22;
        }
        else {
            tn += (4 * xx + 2) * r22 + (1 - yy) * 4 * r12;
            yy -= 1;
        }

        if (clip_pixel(cx + xx, cy + yy, clip))
            pixel_draw(dst, cx + xx, cy + yy, color);

        if (clip_pixel(cx - xx, cy + yy, clip))
            pixel_draw(dst, cx - xx, cy + yy, color);

        if (clip_pixel(cx + xx, cy - yy, clip))
            pixel_draw(dst, cx + xx, cy - yy, color);

        if (clip_pixel(cx - xx, cy - yy, clip))
            pixel_draw(dst, cx - xx, cy - yy, color);
        xx += 1;
    }

    if (clip_pixel(cx + xx, cy + yy, clip))
        pixel_draw(dst, cx + xx, cy + yy, color);

    if (clip_pixel(cx - xx, cy + yy, clip))
        pixel_draw(dst, cx - xx, cy + yy, color);

    if (clip_pixel(cx + xx, cy - yy, clip))
        pixel_draw(dst, cx + xx, cy - yy, color);

    if (clip_pixel(cx - xx, cy - yy, clip))
        pixel_draw(dst, cx - xx, cy - yy, color);

    CR_SWAP(r1, r2, rr);

    xx = 0;
    yy = r2;
    r12 = r1 * r1;
    r22 = r2 * r2;
    xmax = (sint_t)(r12 / sqrt(r12 + r22));
    tn = r12 - 2 * r2 * r12;
    while (xx <= xmax)
    {
        if (tn < 0 || yy == 0) {
            tn += (4 * xx + 2) * r22;
        }
        else {
            tn += (4 * xx + 2) * r22 + (1 - yy) * 4 * r12;
            yy -= 1;
        }

        if (clip_pixel(cx + yy, cy + xx, clip))
            pixel_draw(dst, cx + yy, cy + xx, color);

        if (clip_pixel(cx + yy, cy - xx, clip))
            pixel_draw(dst, cx + yy, cy - xx, color);

        if (clip_pixel(cx - yy, cy + xx, clip))
            pixel_draw(dst, cx - yy, cy + xx, color);

        if (clip_pixel(cx - yy, cy - xx, clip))
            pixel_draw(dst, cx - yy, cy - xx, color);
        xx += 1;
    }

    if (clip_pixel(cx + yy, cy + xx, clip))
        pixel_draw(dst, cx + yy, cy + xx, color);

    if (clip_pixel(cx + yy, cy - xx, clip))
        pixel_draw(dst, cx + yy, cy - xx, color);

    if (clip_pixel(cx - yy, cy + xx, clip))
        pixel_draw(dst, cx - yy, cy + xx, color);

    if (clip_pixel(cx - yy, cy - xx, clip))
        pixel_draw(dst, cx - yy, cy - xx, color);
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
