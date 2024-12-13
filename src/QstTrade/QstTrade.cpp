
#include "QstTrade.h"
#include "gfx2/gdiwin.h"

/* 外部库引用 */
#pragma comment (lib, "CrH_GFX2.lib")
#pragma comment (lib, "CrH_UTIL.lib")
#pragma comment (lib, "GFX2_GDI.lib")

/* 全局变量 */
iFONT*  g_sml_font = NULL;      /* 小字字体对象 */
iFONT*  g_mdl_font = NULL;      /* 中字字体对象 */
iFONT*  g_lrg_font = NULL;      /* 大字字体对象 */
iGFX2*  g_bmp_temp = NULL;      /* 临时测量表面 */

/* 蜡烛图 */
sIMAGE* g_win_candle = NULL;

/* 量能图 */
sIMAGE* g_win_voturn = NULL;
sIMAGE* g_win_vaturn = NULL;

/* 指标图 */
sIMAGE* g_win_indctr[QTRD_INDCTR_VMAX] = { NULL };

/* 系统配置 */
sQTRD_conf  g_sys_cfg;

/* 图表对象 */
sQTRD_candle    g_can_chart = { 0 };
sQTRD_amount    g_mnt_chart = { NULL };
sQTRD_indctr    g_ind_chart[QTRD_INDCTR_VMAX] = { NULL };

/*****************************************************************************/
/*                                 工具函数                                  */
/*****************************************************************************/

/*
=======================================
    设置极值
=======================================
*/
CR_API void_t
qtrd_setup_min_max (
  __CR_IO__ asy::kday_draw* info,
  __CR_IN__ size_t          leave,
  __CR_IN__ size_t          count,
  __CR_IN__ const double*   data,
  __CR_IN__ size_t          step
    )
{
    double  value;

    /* 计算数据起始位置 */
    if (step < sizeof(double))
        step = sizeof(double);
    data = (double*)((byte_t*)data + leave * step);

    /* 开始查找极值 */
    for (uint_t idx = 0; idx < count; idx++) {
        value = *data;
        if (info->min_val > value)
            info->min_val = value;
        if (info->max_val < value)
            info->max_val = value;
        data = (double*)((byte_t*)data + step);
    }
}

/*
=======================================
    画线笔画
=======================================
*/
CR_API void_t
qtrd_pixel_draw (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ cpix_t          color
    )
{
    sFILL   fll;
    uint_t  wdt = dst->pal[0];
    sint_t  dlt = wdt / 2;

    fll.dx = x - dlt;
    fll.dy = y - dlt;
    fll.dw = wdt;
    fll.dh = wdt;
    fill_set32_c(dst, &fll, color, NULL);
}

/*
=======================================
    选择线宽
=======================================
*/
CR_API pixdraw_t
qtrd_line_width_func (
  __CR_IO__ sIMAGE* image,
  __CR_IN__ uint_t  width
    )
{
    if (width % 2 == 0)
        width++;
    if (width != 1) {
        image->pal[0] = width;
        return (qtrd_pixel_draw);
    }
    return (pixel_set32z);
}

/*
=======================================
    绘制曲线 (正值)
=======================================
*/
CR_API void_t
qtrd_line_draw_plus (
  __CR_IO__ sIMAGE*                 image,
  __CR_IN__ const sRECT*            rect,
  __CR_IN__ const asy::kday_draw*   info,
  __CR_IN__ size_t                  enter,
  __CR_IN__ uint_t                  count,
  __CR_IN__ const double*           data,
  __CR_IN__ size_t                  step,
  __CR_IN__ uint_t                  width,
  __CR_IN__ cpix_t                  color
    )
{
    sLINE   line;
    sint_t  x1, x2, y1, y2;

    /* 计算数据起始位置 */
    x1 = (rect->x1 + rect->x2) / 2;
    if (step < sizeof(double))
        step = sizeof(double);
    data = (double*)((byte_t*)data + enter * step);
    y1 = info->get_positive_y(*data, false);

    pixdraw_t   pxdrw = qtrd_line_width_func(image, width);

    /* 开始绘制曲线 */
    for (uint_t idx = 1; idx < count; idx++) {
        x2 = (rect[idx].x1 + rect[idx].x2) / 2;
        data = (double*)((byte_t*)data - step);
        y2 = info->get_positive_y(*data, false);
        line_set_xy(&line, x1, y1, x2, y2);
        draw_line(image, &line, color, pxdrw);
        x1 = x2;
        y1 = y2;
    }
}

/*
=======================================
    绘制曲线 (正负)
=======================================
*/
CR_API void_t
qtrd_line_draw_full (
  __CR_IO__ sIMAGE*                 image,
  __CR_IN__ const sRECT*            rect,
  __CR_IN__ const asy::kday_draw*   info,
  __CR_IN__ size_t                  enter,
  __CR_IN__ uint_t                  count,
  __CR_IN__ const double*           data,
  __CR_IN__ size_t                  step,
  __CR_IN__ uint_t                  width,
  __CR_IN__ cpix_t                  color
    )
{
    sLINE   line;
    sint_t  x1, x2, y1, y2;

    /* 计算数据起始位置 */
    x1 = (rect->x1 + rect->x2) / 2;
    if (step < sizeof(double))
        step = sizeof(double);
    data = (double*)((byte_t*)data + enter * step);
    y1 = info->get_complete_y(*data, false);

    pixdraw_t   pxdrw = qtrd_line_width_func(image, width);

    /* 开始绘制曲线 */
    for (uint_t idx = 1; idx < count; idx++) {
        x2 = (rect[idx].x1 + rect[idx].x2) / 2;
        data = (double*)((byte_t*)data - step);
        y2 = info->get_complete_y(*data, false);
        line_set_xy(&line, x1, y1, x2, y2);
        draw_line(image, &line, color, pxdrw);
        x1 = x2;
        y1 = y2;
    }
}

/*
=======================================
    绘制水平网格
=======================================
*/
CR_API sint_t
qtrd_grid_h_draw (
  __CR_IO__ sIMAGE*                 image,
  __CR_IN__ const asy::kday_draw*   info,
  __CR_IN__ double                  value,
  __CR_IN__ bool_t                  xfull,
  __CR_IN__ uint_t                  dash,
  __CR_IN__ uint_t                  width,
  __CR_IN__ cpix_t                  color
    )
{
    sint_t      dy;
    pixdraw_t   px;

    /* 绘制线段 */
    px = qtrd_line_width_func(image, width);
    if (xfull) dy = info->get_complete_y(value, false);
    else       dy = info->get_positive_y(value, false);
    draw_lineh_dot(image, 0, image->clip_win.x2, dy, dash, color, px);
    return (dy);
}

/*
=======================================
    绘制垂直网格
=======================================
*/
CR_API sint_t
qtrd_grid_v_draw (
  __CR_IO__ sIMAGE*         image,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ size_t          enter,
  __CR_IN__ size_t          leave,
  __CR_IN__ size_t          time,
  __CR_IN__ uint_t          dash,
  __CR_IN__ uint_t          width,
  __CR_IN__ cpix_t          color
    )
{
    sint_t      dx;
    pixdraw_t   px;

    /* 安全检查 */
    if (time > enter || time < leave)
        return (-32768);
    enter -= time;
    dx = (rect[enter].x1 + rect[enter].x2) / 2;

    /* 绘制线段 */
    px = qtrd_line_width_func(image, width);
    draw_linev_dot(image, dx, 0, image->clip_win.y2, dash, color, px);
    return (dx);
}

/*
=======================================
    填充字体参数
=======================================
*/
CR_API void_t
qtrd_font_param (
  __CR_OT__ sQTRD_font*     param,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ sint_t          height,
  __CR_IN__ uint_t          weight
    )
{
    leng_t  len;

    param->height = height;
    param->weight = weight;
    if (name == param->name)
        return;
    mem_zero(param->name, sizeof(param->name));
    if ((len = str_lenW(name)) >= LF_FACESIZE)
        len = LF_FACESIZE - 1;
    chr_cpyW(param->name, name, len);
}

/*
=======================================
    创建字体
=======================================
*/
CR_API iFONT*
qtrd_font_get (
  __CR_IN__ const sQTRD_font*   param
    )
{
    LOGFONTW    font;

    struct_zero(&font, LOGFONTW);
    font.lfHeight = param->height;
    font.lfWeight = param->weight;
    font.lfCharSet = DEFAULT_CHARSET;
    font.lfQuality = ANTIALIASED_QUALITY;
    chr_cpyW(font.lfFaceName, param->name, LF_FACESIZE);
    return (create_gdi_fontW(&font));
}

/*
=======================================
    创建字体
=======================================
*/
CR_API iFONT*
qtrd_font_get2 (
  __CR_IN__ const wide_t*   name,
  __CR_IN__ sint_t          height,
  __CR_IN__ uint_t          weight
    )
{
    sQTRD_font  param;

    qtrd_font_param(&param, name, height, weight);
    return (qtrd_font_get(&param));
}

/*
=======================================
    释放字体
=======================================
*/
CR_API void_t
qtrd_font_rel (
  __CR_IN__ iFONT*  font
    )
{
    CR_VCALL(font)->release(font);
}

/*
=======================================
    文字测量
=======================================
*/
CR_API void_t
qtrd_text_box (
  __CR_OT__ sRECT*          rect,
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ iFONT*          font,
  __CR_IN__ uint_t          cpage
    )
{
    /* 计算目标区域 */
    rect->x1 = rect->y1 = 0;
    CR_VCALL(font)->bind(font, g_bmp_temp);
    CR_VCALL(font)->calc_rect(font, text, rect, cpage);
}

/*
=======================================
    文字绘制
=======================================
*/
CR_API void_t
qtrd_text_out (
  __CR_IO__ sIMAGE*         image,
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ cpix_t          color,
  __CR_IN__ cpix_t          bkcolor,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          align,
  __CR_IN__ iFONT*          font,
  __CR_IN__ uint_t          cpage
    )
{
    sRECT   dest;
    sBLIT   blit;
    iGFX2*  gfx2;
    int32u  mode;
    bool_t  rett;
    sIMAGE* surf;

    /* 计算目标区域 */
    dest.x1 = rect->x1;
    dest.y1 = rect->y1;
    CR_VCALL(font)->bind(font, g_bmp_temp);
    CR_VCALL(font)->calc_rect(font, text, &dest, cpage);
    egui_rect_align(&dest, rect, align);

    /* 创建临时的 GDI 表面 */
    gfx2 = (iGFX2*)create_gdi_bitmap(dest.ww, dest.hh, CR_ARGB8888);
    if (gfx2 == NULL)
        return;

    /* 临时复制过来 */
    blit.sx = dest.x1;
    blit.sy = dest.y1;
    blit.sw = dest.ww;
    blit.sh = dest.hh;
    blit.dx = blit.dy = 0;
    surf = CR_VCALL(gfx2)->lock(gfx2);
    blit_set32_c(surf, image, &blit, NULL);
    CR_VCALL(gfx2)->unlock(gfx2);
    mode = (bkcolor.val == 0) ? TRANSPARENT : OPAQUE;

    /* 绑定目标表面并输出文字 */
    if (CR_VCALL(font)->bind(font, gfx2) &&
        CR_VCALL(font)->setMode(font, mode) &&
        CR_VCALL(font)->setColor(font, color.val) &&
        CR_VCALL(font)->setBkColor(font, bkcolor.val))
    {
        /* 输出结果回拷到目标画布 */
        CR_VCALL(font)->enter(font);
        rect_set_wh(&dest, 0, 0, dest.ww, dest.hh);
        rett = CR_VCALL(font)->draw_text(font, text, &dest, cpage);
        CR_VCALL(font)->leave(font);
        if (rett) {
            blit.dx = blit.sx;
            blit.dy = blit.sy;
            blit.sx = blit.sy = 0;
            surf = CR_VCALL(gfx2)->lock(gfx2);
            image_fuck_gdi(surf);
            blit_set32_c(image, surf, &blit, NULL);
            CR_VCALL(gfx2)->unlock(gfx2);
        }
    }
    CR_VCALL(gfx2)->release(gfx2);
}

/*****************************************************************************/
/*                                 系统函数                                  */
/*****************************************************************************/

/*
=======================================
    系统初始化
=======================================
*/
CR_API bool_t
QstTrade_Init (
  __CR_IN__ uint_t          can_width,
  __CR_IN__ uint_t          can_height,
  __CR_IN__ uint_t          ind_height,
  __CR_IN__ const ansi_t*   cfg_file
    )
{
    /* 创建绘图表面 */
    if (g_win_candle != NULL)
        image_del(g_win_candle);
    g_win_candle = image_new(0, 0, can_width, can_height, 32);
    if (g_win_candle == NULL)
        return (FALSE);
    if (g_win_voturn != NULL)
        image_del(g_win_voturn);
    g_win_voturn = image_new(0, 0, can_width, ind_height, 32);
    if (g_win_voturn == NULL)
        return (FALSE);
    if (g_win_vaturn != NULL)
        image_del(g_win_vaturn);
    g_win_vaturn = image_new(0, 0, can_width, ind_height, 32);
    if (g_win_vaturn == NULL)
        return (FALSE);
    for (uint_t idx = 0; idx < QTRD_INDCTR_VMAX; idx++) {
        if (g_win_indctr[idx] != NULL)
            image_del(g_win_indctr[idx]);
        g_win_indctr[idx] = image_new(0, 0, can_width, ind_height, 32);
        if (g_win_indctr[idx] == NULL)
            return (FALSE);
    }

    /* 加载系统配置 */
    qtrd_configure_reset(&g_sys_cfg);
    if (!qtrd_configure_load(&g_sys_cfg, cfg_file))
        return (FALSE);
    if (!qtrd_configure_apply(&g_sys_cfg))
        return (FALSE);

    return (TRUE);
}

/*
=======================================
    系统释放
=======================================
*/
CR_API void_t
QstTrade_Free (void_t)
{
    /* 释放系统配置 */
    if (g_bmp_temp != NULL) {
        CR_VCALL(g_bmp_temp)->release(g_bmp_temp);
        g_bmp_temp = NULL;
    }
    if (g_sml_font != NULL) {
        CR_VCALL(g_sml_font)->release(g_sml_font);
        g_sml_font = NULL;
    }
    if (g_mdl_font != NULL) {
        CR_VCALL(g_mdl_font)->release(g_mdl_font);
        g_mdl_font = NULL;
    }
    if (g_lrg_font != NULL) {
        CR_VCALL(g_lrg_font)->release(g_lrg_font);
        g_lrg_font = NULL;
    }

    /* 释放绘图表面 */
    if (g_win_candle != NULL) {
        image_del(g_win_candle);
        g_win_candle = NULL;
    }
    if (g_win_voturn != NULL) {
        image_del(g_win_voturn);
        g_win_voturn = NULL;
    }
    if (g_win_vaturn != NULL) {
        image_del(g_win_vaturn);
        g_win_vaturn = NULL;
    }
    for (uint_t idx = 0; idx < QTRD_INDCTR_VMAX; idx++) {
        if (g_win_indctr[idx] != NULL) {
            image_del(g_win_indctr[idx]);
            g_win_indctr[idx] = NULL;
        }
    }

    /* 释放图表对象 */
    if (g_can_chart.prlst != NULL)
        qtrd_candle_kill(&g_can_chart);
    if (g_mnt_chart.volst != NULL)
        qtrd_amount_kill(&g_mnt_chart);
    mem_zero(g_ind_chart, sizeof(g_ind_chart));
}

/*
=======================================
    全局变量清空
=======================================
*/
CR_API void_t
QstTrade_Clean (void_t)
{
    g_bmp_temp = NULL;
    g_sml_font = g_mdl_font = g_lrg_font = NULL;
    g_win_candle = g_win_voturn = g_win_vaturn = NULL;
    mem_zero(g_ind_chart, sizeof(g_ind_chart));
    mem_zero(g_win_indctr, sizeof(g_win_indctr));
    mem_zero(&g_sys_cfg, sizeof(&g_sys_cfg));
    mem_zero(&g_can_chart, sizeof(g_can_chart));
    mem_zero(&g_mnt_chart, sizeof(g_mnt_chart));
}
