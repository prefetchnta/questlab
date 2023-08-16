
#include "QstTrade_Int.h"

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/*
---------------------------------------
    绘制标题分割线
---------------------------------------
*/
static void_t
qtrd_title_draw (
  __CR_IN__ const sQTRD_indctr* indx
    )
{
    sint_t  dy = indx->candl->cfg->text_sml.height;

    dy += indx->candl->cfg->win_space_betw - 1;
    draw_lineh(indx->image, 0, indx->image->position.x2, dy,
               indx->candl->cfg->line_grid_color, pixel_set32z);
}

/*
---------------------------------------
    绘制水平网格（内部）
---------------------------------------
*/
static sint_t
qtrd_grid_h_draw (
  __CR_IN__ const sQTRD_indctr* indx,
  __CR_IN__ double              value
    )
{
    return (qtrd_grid_h_draw(indx->chart, &indx->dinfo, value,
                        indx->xfull, indx->candl->cfg->line_grid_dash,
                            indx->candl->cfg->line_grid_width,
                            indx->candl->cfg->line_grid_color));
}

/*
---------------------------------------
    绘制曲线（内部）
---------------------------------------
*/
static void_t
qtrd_line_draw (
  __CR_IN__ const sQTRD_indctr* indx,
  __CR_IN__ const double*       data,
  __CR_IN__ size_t              step,
  __CR_IN__ cpix_t              color
    )
{
    if (!indx->xfull) {
        qtrd_line_draw_plus(indx->chart, indx->candl->hllst, &indx->dinfo,
                            indx->candl->enter, indx->candl->count, data,
                            step, indx->candl->cfg->line_width, color);
    }
    else {
        qtrd_line_draw_full(indx->chart, indx->candl->hllst, &indx->dinfo,
                            indx->candl->enter, indx->candl->count, data,
                            step, indx->candl->cfg->line_width, color);
    }
}

/*
---------------------------------------
    设置极值（内部）
---------------------------------------
*/
static void_t
qtrd_find_min_max (
  __CR_IO__ sQTRD_indctr*   indx,
  __CR_IN__ const double*   data,
  __CR_IN__ size_t          step
    )
{
    qtrd_setup_min_max(&indx->dinfo, indx->candl->leave,
                        indx->candl->count, data, step);
}

/*
---------------------------------------
    处理文字边界
---------------------------------------
*/
static void_t
qtrd_height_clip (
  __CR_IN__ const sQTRD_indctr* indx,
  __CR_IO__ sint_t*             list,
  __CR_IN__ uint_t              count
    )
{
    sint_t  hft = indx->candl->cfg->text_sml.height;
    uint_t  spy = hft / 2;

    for (uint_t idx = 0; idx < count; idx++) {
        list[idx] -= spy;
        if (list[idx] < 0)
            list[idx] = 0;
        else
        if (list[idx] + hft >= indx->chart->position.y2)
            list[idx] = indx->chart->position.y2 - hft;
    }
}

/*****************************************************************************/
/*                                  指标图                                   */
/*****************************************************************************/

/*
=======================================
    创建指标图
=======================================
*/
CR_API bool_t
qtrd_indctr_make (
  __CR_IN__ sIMAGE*         img,
  __CR_OT__ sQTRD_indctr*   out,
  __CR_IN__ sQTRD_candle*   can
    )
{
    /* 必须是32位色的正图 */
    if (img->gdi || img->fmt != CR_ARGB8888)
        return (FALSE);
    out->image = img;
    out->chart = &out->clone;
    out->candl = can;
    out->dinfo.width = can->dinfo.width;
    out->dinfo.min_val = 0;
    out->dinfo.max_val = 0;
    out->xfull = FALSE;

    uint_t  spy = can->cfg->text_sml.height;

    /* 图表的图片要去掉上面的文字区 */
    spy += can->cfg->win_space_betw;
    struct_cpy(&out->clone, img, sIMAGE);
    out->clone.data += spy * out->clone.bpl;
    out->clone.clip_win.y2 -= spy;
    out->clone.clip_win.hh -= spy;
    out->clone.position.y2 -= spy;
    out->clone.position.hh -= spy;
    out->dinfo.height = out->clone.position.hh;
    return (TRUE);
}

/*****************************************************************************/
/*                                   MACD                                    */
/*****************************************************************************/

/*
=======================================
    设置 MACD 指标图
=======================================
*/
CR_API void_t
qtrd_indctr_set_macd (
  __CR_IO__ sQTRD_indctr*   ind
    )
{
    ansi_t      name[64];
    /* --------------- */
    const double*   dif;
    const double*   dea;
    const double*   macd;

    sprintf(name, "DIF(%u,%u)", ind->candl->cfg->macd_days_fast,
                                ind->candl->cfg->macd_days_slow);
    dif = ind->candl->kdays->line(name);
    if (dif == NULL)
        return;
    sprintf(name, "DEA(%u,%u,%u)", ind->candl->cfg->macd_days_fast,
                                   ind->candl->cfg->macd_days_slow,
                                   ind->candl->cfg->macd_days_diff);
    dea = ind->candl->kdays->line(name);
    if (dea == NULL)
        return;
    sprintf(name, "MACD(%u,%u,%u)", ind->candl->cfg->macd_days_fast,
                                    ind->candl->cfg->macd_days_slow,
                                    ind->candl->cfg->macd_days_diff);
    macd = ind->candl->kdays->line(name);
    if (macd == NULL)
        return;
    ind->xfull = TRUE;
    ind->dinfo.min_val = *dif;
    ind->dinfo.max_val = *dif;
    qtrd_find_min_max(ind, dif, 0);
    qtrd_find_min_max(ind, dea, 0);
    qtrd_find_min_max(ind, macd, 0);
    ind->dinfo.make_axis_mirror();
}

/*
=======================================
    绘制 MACD 指标图
=======================================
*/
CR_API void_t
qtrd_indctr_draw_macd (
  __CR_IN__ const sQTRD_indctr* ind
    )
{
    ansi_t      name[64];
    /* --------------- */
    const double*   dif;
    const double*   dea;
    const double*   macd;

    sprintf(name, "DIF(%u,%u)", ind->candl->cfg->macd_days_fast,
                                ind->candl->cfg->macd_days_slow);
    dif = ind->candl->kdays->line(name);
    if (dif == NULL)
        return;
    sprintf(name, "DEA(%u,%u,%u)", ind->candl->cfg->macd_days_fast,
                                   ind->candl->cfg->macd_days_slow,
                                   ind->candl->cfg->macd_days_diff);
    dea = ind->candl->kdays->line(name);
    if (dea == NULL)
        return;
    sprintf(name, "MACD(%u,%u,%u)", ind->candl->cfg->macd_days_fast,
                                    ind->candl->cfg->macd_days_slow,
                                    ind->candl->cfg->macd_days_diff);
    macd = ind->candl->kdays->line(name);
    if (macd == NULL)
        return;
    macd += ind->candl->enter;

    /* 绘制 MACD 柱 */
    for (uint_t idx = 0; idx < ind->candl->count; idx++, macd--)
    {
        sFILL   fll;
        cpix_t  clr;
        sRECT*  box = &ind->candl->prlst[idx];

        fll.dx = box->x1;
        fll.dw = box->ww;
        if (*macd > 0) {
            fll.dy = ind->dinfo.get_complete_y(*macd);
            fll.dh = ind->dinfo.height / 2 - fll.dy + 1;
            clr.val = ind->candl->cfg->bar_color_rise.val;
        }
        else
        if (*macd < 0) {
            fll.dy = ind->dinfo.height / 2;
            fll.dh = ind->dinfo.get_complete_y(*macd) - fll.dy + 1;
            clr.val = ind->candl->cfg->bar_color_drop.val;
        }
        else {
            fll.dy = ind->dinfo.height / 2;
            fll.dh = 1;
            clr.val = ind->candl->cfg->bar_color_eque.val;
        }
        fill_set32_c(ind->chart, &fll, clr, NULL);
    }

    /* 绘制两条均线 */
    qtrd_title_draw(ind);
    qtrd_line_draw(ind, dif, 0, ind->candl->cfg->macd_line_dif_color);
    qtrd_line_draw(ind, dea, 0, ind->candl->cfg->macd_line_dea_color);
}

/*****************************************************************************/
/*                                    KDJ                                    */
/*****************************************************************************/

/*
=======================================
    设置 KDJ 指标图
=======================================
*/
CR_API void_t
qtrd_indctr_set_kdj (
  __CR_IO__ sQTRD_indctr*   ind
    )
{
    ansi_t      name[64];
    /* --------------- */
    const double*   data;

    ind->xfull = FALSE;
    ind->dinfo.min_val = 0;
    ind->dinfo.max_val = 100;
    if (ind->candl->cfg->kdj_line_j_is_render) {
        sprintf(name, "J(%u,%u,%u)", ind->candl->cfg->kdj_days_r,
                                     ind->candl->cfg->kdj_days_k,
                                     ind->candl->cfg->kdj_days_d);
        data = ind->candl->kdays->line(name);
        if (data == NULL)
            return;
        qtrd_find_min_max(ind, data, 0);
    }
}

/*
=======================================
    绘制 KDJ 指标图
=======================================
*/
CR_API void_t
qtrd_indctr_draw_kdj (
  __CR_IN__ const sQTRD_indctr* ind
    )
{
    sint_t      grid[5];
    ansi_t      name[64];
    /* --------------- */
    const double*   lkkk;
    const double*   lddd;
    const double*   ljjj;

    /* 绘制标尺 */
    qtrd_title_draw(ind);
    grid[0] = qtrd_grid_h_draw(ind, 20);
    grid[1] = qtrd_grid_h_draw(ind, 50);
    grid[2] = qtrd_grid_h_draw(ind, 80);
    if (ind->candl->cfg->kdj_line_j_is_render) {
        grid[3] = qtrd_grid_h_draw(ind, 0);
        grid[4] = qtrd_grid_h_draw(ind, 100);
    }

    /* 绘制曲线 */
    sprintf(name, "K(%u,%u)", ind->candl->cfg->kdj_days_r,
                              ind->candl->cfg->kdj_days_k);
    lkkk = ind->candl->kdays->line(name);
    if (lkkk == NULL)
        return;
    sprintf(name, "D(%u,%u,%u)", ind->candl->cfg->kdj_days_r,
                                 ind->candl->cfg->kdj_days_k,
                                 ind->candl->cfg->kdj_days_d);
    lddd = ind->candl->kdays->line(name);
    if (lddd == NULL)
        return;
    qtrd_line_draw(ind, lkkk, 0, ind->candl->cfg->kdj_line_k_color);
    qtrd_line_draw(ind, lddd, 0, ind->candl->cfg->kdj_line_d_color);

    if (ind->candl->cfg->kdj_line_j_is_render) {
        sprintf(name, "J(%u,%u,%u)", ind->candl->cfg->kdj_days_r,
                                     ind->candl->cfg->kdj_days_k,
                                     ind->candl->cfg->kdj_days_d);
        ljjj = ind->candl->kdays->line(name);
        if (ljjj == NULL)
            return;
        qtrd_line_draw(ind, ljjj, 0, ind->candl->cfg->kdj_line_j_color);
    }

    sRECT   box;

    /* 绘制文字 */
    qtrd_height_clip(ind, grid, cntsof(grid));
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[0], 0, 0);
    qtrd_text_out(ind->chart, "20", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[1], 0, 0);
    qtrd_text_out(ind->chart, "50", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[2], 0, 0);
    qtrd_text_out(ind->chart, "80", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
    if (!ind->candl->cfg->kdj_line_j_is_render)
        return;
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[3], 0, 0);
    qtrd_text_out(ind->chart, "0", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[4], 0, 0);
    qtrd_text_out(ind->chart, "100", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
}

/*****************************************************************************/
/*                                    RSI                                    */
/*****************************************************************************/

/*
=======================================
    设置 RSI 指标图
=======================================
*/
CR_API void_t
qtrd_indctr_set_rsi (
  __CR_IO__ sQTRD_indctr*   ind
    )
{
    ind->xfull = FALSE;
    ind->dinfo.min_val = 0;
    ind->dinfo.max_val = 100;
}

/*
=======================================
    绘制 RSI 指标图
=======================================
*/
CR_API void_t
qtrd_indctr_draw_rsi (
  __CR_IN__ const sQTRD_indctr* ind
    )
{
    sint_t  grid[3];
    ansi_t  name[64];

    /* 绘制标尺 */
    qtrd_title_draw(ind);
    grid[0] = qtrd_grid_h_draw(ind, 20);
    grid[1] = qtrd_grid_h_draw(ind, 50);
    grid[2] = qtrd_grid_h_draw(ind, 80);

    /* 绘制曲线 */
    for (uint_t idx = 0; idx < 3; idx++)
    {
        cpix_t          clrs;
        const double*   data;

        sprintf(name, "RSI%u", ind->candl->cfg->rsi_line_level[idx]);
        data = ind->candl->kdays->line(name);
        if (data == NULL)
            continue;
        clrs.val = ind->candl->cfg->rsi_line_color[idx].val;
        qtrd_line_draw(ind, data, 0, clrs);
    }

    sRECT   box;

    /* 绘制文字 */
    qtrd_height_clip(ind, grid, cntsof(grid));
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[0], 0, 0);
    qtrd_text_out(ind->chart, "20", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[1], 0, 0);
    qtrd_text_out(ind->chart, "50", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[2], 0, 0);
    qtrd_text_out(ind->chart, "80", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
}

/*****************************************************************************/
/*                                    WR                                     */
/*****************************************************************************/

/*
=======================================
    设置 WR 指标图
=======================================
*/
CR_API void_t
qtrd_indctr_set_wr (
  __CR_IO__ sQTRD_indctr*   ind
    )
{
    ind->xfull = FALSE;
    ind->dinfo.min_val = 0;
    ind->dinfo.max_val = 100;
}

/*
=======================================
    绘制 WR 指标图
=======================================
*/
CR_API void_t
qtrd_indctr_draw_wr (
  __CR_IN__ const sQTRD_indctr* ind
    )
{
    sint_t      grid[3];
    ansi_t      name[64];
    /* --------------- */
    const double*   data;

    /* 绘制标尺 */
    qtrd_title_draw(ind);
    grid[0] = qtrd_grid_h_draw(ind, 20);
    grid[1] = qtrd_grid_h_draw(ind, 50);
    grid[2] = qtrd_grid_h_draw(ind, 80);

    /* 绘制曲线 */
    sprintf(name, "WR%u", ind->candl->cfg->wr_line_level);
    data = ind->candl->kdays->line(name);
    if (data == NULL)
        return;
    qtrd_line_draw(ind, data, 0, ind->candl->cfg->wr_line_color);

    sRECT   box;

    /* 绘制文字 */
    qtrd_height_clip(ind, grid, cntsof(grid));
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[0], 0, 0);
    qtrd_text_out(ind->chart, "20", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[1], 0, 0);
    qtrd_text_out(ind->chart, "50", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[2], 0, 0);
    qtrd_text_out(ind->chart, "80", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
}

/*****************************************************************************/
/*                                    CCI                                    */
/*****************************************************************************/

/*
=======================================
    设置 CCI 指标图
=======================================
*/
CR_API void_t
qtrd_indctr_set_cci (
  __CR_IO__ sQTRD_indctr*   ind
    )
{
    ansi_t      name[64];
    /* --------------- */
    const double*   data;

    ind->xfull = TRUE;
    ind->dinfo.min_val = -120;
    ind->dinfo.max_val =  120;
    sprintf(name, "CCI%u", ind->candl->cfg->cci_line_level);
    data = ind->candl->kdays->line(name);
    if (data == NULL)
        return;
    qtrd_find_min_max(ind, data, 0);
    ind->dinfo.make_axis_mirror();
}

/*
=======================================
    绘制 CCI 指标图
=======================================
*/
CR_API void_t
qtrd_indctr_draw_cci (
  __CR_IN__ const sQTRD_indctr* ind
    )
{
    sint_t      grid[2];
    ansi_t      name[64];
    /* --------------- */
    const double*   data;

    /* 绘制标尺 */
    qtrd_title_draw(ind);
    grid[0] = qtrd_grid_h_draw(ind, -100);
    grid[1] = qtrd_grid_h_draw(ind,  100);

    /* 绘制曲线 */
    sprintf(name, "CCI%u", ind->candl->cfg->cci_line_level);
    data = ind->candl->kdays->line(name);
    if (data == NULL)
        return;
    qtrd_line_draw(ind, data, 0, ind->candl->cfg->cci_line_color);

    sRECT   box;

    /* 绘制文字 */
    qtrd_height_clip(ind, grid, cntsof(grid));
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[0], 0, 0);
    qtrd_text_out(ind->chart, "+100", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
    rect_set_wh(&box, ind->candl->cfg->win_space_betw, grid[1], 0, 0);
    qtrd_text_out(ind->chart, "-100", ind->candl->cfg->win_text_color,
                  ind->candl->cfg->win_tran_color, &box, 0, g_sml_font);
}
