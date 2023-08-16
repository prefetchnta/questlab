
#include "QstTrade_Int.h"

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/*
---------------------------------------
    绘制水平网格（内部）
---------------------------------------
*/
static sint_t
qtrd_grid_h_draw (
  __CR_IN__ const sQTRD_candle* canx,
  __CR_IN__ double              value
    )
{
    return (qtrd_grid_h_draw(canx->image, &canx->dinfo, value, FALSE,
                    canx->cfg->line_grid_dash, canx->cfg->line_grid_width,
                             canx->cfg->line_grid_color));
}

/*
---------------------------------------
    绘制垂直网格（内部）
---------------------------------------
*/
static sint_t
qtrd_grid_v_draw (
  __CR_IN__ const sQTRD_candle* canx,
  __CR_IN__ size_t              time
    )
{
    return (qtrd_grid_v_draw(canx->image, canx->hllst,
                canx->enter, canx->leave, time, canx->cfg->line_grid_dash,
                canx->cfg->line_grid_width, canx->cfg->line_grid_color));
}

/*
---------------------------------------
    绘制曲线（内部）
---------------------------------------
*/
static void_t
qtrd_line_draw (
  __CR_IN__ const sQTRD_candle* canx,
  __CR_IN__ const double*       data,
  __CR_IN__ size_t              step,
  __CR_IN__ cpix_t              color
    )
{
    qtrd_line_draw_plus(canx->image, canx->hllst, &canx->dinfo, canx->enter,
                        canx->count, data, step, canx->cfg->line_width, color);
}

/*
---------------------------------------
    设置极值（内部）
---------------------------------------
*/
static void_t
qtrd_find_min_max (
  __CR_IO__ sQTRD_candle*   canx,
  __CR_IN__ const double*   data,
  __CR_IN__ size_t          step
    )
{
    qtrd_setup_min_max(&canx->dinfo, canx->leave, canx->count, data, step);
}

/*
---------------------------------------
    缺口的生成
---------------------------------------
*/
static bool_t
qtrd_candle_gap_make (
  __CR_IO__ sQTRD_candle*   can
    )
{
    asy::array<asy::kday_gap>   gap;

    /* 计算跳空缺口 */
    gap.init();
    if (!can->kdays->make_GAP(&gap, can->leave, can->enter)) {
        gap.free();
        return (FALSE);
    }
    can->gpcnt = (uint_t)gap.size();
    if (can->gpcnt == 0)
        return (TRUE);
    can->gpdat = gap.data();
    can->gplst = mem_talloc(can->gpcnt, sRECT);
    if (can->gplst == NULL) {
        gap.free();
        can->gpdat = NULL;
        return (FALSE);
    }
    return (TRUE);
}

/*****************************************************************************/
/*                                  蜡烛图                                   */
/*****************************************************************************/

/*
=======================================
    创建蜡烛图
=======================================
*/
CR_API bool_t
qtrd_candle_init (
  __CR_IN__ sIMAGE*             img,
  __CR_OT__ sQTRD_candle*       out,
  __CR_IN__ asy::kdays*         dat,
  __CR_IN__ const sDATETIME*    beg,
  __CR_IN__ const sQTRD_conf*   cfg
    )
{
    sQTRD_candle    ret;

    /* 必须是32位色的正图 */
    if (img->gdi || img->fmt != CR_ARGB8888)
        return (FALSE);
    ret.gpcnt = 0;
    ret.gpdat = NULL;
    ret.gplst = NULL;

    /* 计算绘制的起始时间索引
       其索引值要比结束时间的索引值大 */
    if (!dat->index_by_date(beg, &ret.enter, false))
        return (FALSE);
    ret.dinfo.width = cfg->bar_width_full - cfg->bar_side_space;
    ret.dinfo.height = img->position.hh;

    /* 计算图片里能放几根蜡烛，最少1根 */
    ret.count = img->position.ww / cfg->bar_width_full;
    if (ret.count == 0)
        return (FALSE);
    ret.prlst = mem_talloc(ret.count * 2, sRECT);
    if (ret.prlst == NULL)
        return (FALSE);
    ret.hllst = &ret.prlst[ret.count];
    ret.clrst = mem_talloc(ret.count, cpix_t);
    if (ret.clrst == NULL) {
        mem_free(ret.prlst);
        return (FALSE);
    }
    ret.atlst = mem_talloc(ret.count, int64u);
    if (ret.atlst == NULL) {
        mem_free(ret.clrst);
        mem_free(ret.prlst);
        return (FALSE);
    }
    ret.image = img;
    ret.kdays = dat;

    /* 计算实际能绘制几根蜡烛 */
    if (ret.count - 1 >= ret.enter)
        ret.leave = 0;
    else
        ret.leave = ret.enter - ret.count + 1;
    ret.index = ret.leave;
    ret.count = ret.enter - ret.leave + 1;
    (dat->get(ret.enter))->date(&ret.enter_date);
    (dat->get(ret.leave))->date(&ret.leave_date);
    ret.cfg = cfg;

    /* 统计这个区间的数据 */
    dat->make_range(&ret.vinfo, ret.leave, ret.enter);
    ret.dinfo.min_val = ret.vinfo.LOW;
    ret.dinfo.max_val = ret.vinfo.HIGH;
    struct_cpy(out, &ret, sQTRD_candle);

    const asy::kday_node*   stt = dat->get(ret.enter);

    /* 开始生成蜡烛 */
    for (uint_t idx = 0; idx < ret.count; idx++, stt--)
    {
        /* 颜色属性选择 */
        ret.atlst[idx] = 0UL;
        if (stt->data.detail.TOPEN < stt->data.detail.TCLOSE) {
            if (cfg->bar_is_rhollow)
                ret.atlst[idx] = QTRD_CANDLE_HOLLOW;
            ret.clrst[idx].val = cfg->bar_color_rise.val;
        }
        else
        if (stt->data.detail.TOPEN > stt->data.detail.TCLOSE) {
            if (cfg->bar_is_dhollow)
                ret.atlst[idx] = QTRD_CANDLE_HOLLOW;
            ret.clrst[idx].val = cfg->bar_color_drop.val;
        }
        else {
            ret.clrst[idx].val = cfg->bar_color_eque.val;
        }
    }
    qtrd_candle_gap_make(out);
    qtrd_candle_rebuild(out);
    return (TRUE);
}

/*
=======================================
    释放蜡烛图
=======================================
*/
CR_API void_t
qtrd_candle_kill (
  __CR_IO__ sQTRD_candle*   can
    )
{
    TRY_FREE(can->gpdat);
    TRY_FREE(can->gplst);
    mem_free(can->prlst);
    mem_free(can->clrst);
    mem_free(can->atlst);
    struct_zero(can, sQTRD_candle);
}

/*
=======================================
    重建蜡烛图
=======================================
*/
CR_API void_t
qtrd_candle_rebuild (
  __CR_IO__ sQTRD_candle*   can
    )
{
    double  up = can->dinfo.max_val - can->vinfo.HIGH;
    double  dn = can->vinfo.LOW - can->dinfo.min_val, hh;
    double  tt = up - dn, aa = can->cfg->can_up_dn_padded;

    /* 尽量使图上下对称 */
    if (tt < 0) tt = -tt;
    if (up > dn)
        can->dinfo.min_val -= tt;
    else
        can->dinfo.max_val += tt;
    aa += can->cfg->win_space_betw;
    hh = (can->dinfo.max_val - can->dinfo.min_val) * aa;
    hh /= can->dinfo.height - 2 * aa;
    can->dinfo.max_val += hh;
    can->dinfo.min_val -= hh;

    sint_t                  lft = 0;
    const asy::kday_node*   stt = can->kdays->get(can->enter);

    /* 重新生成蜡烛 */
    for (uint_t idx = 0; idx < can->count; idx++, stt--)
    {
        /* 价格框 & 高低框计算 */
        stt->data.detail.price_box(&can->prlst[idx], lft, &can->dinfo);
        stt->data.detail.halo_box(&can->hllst[idx], lft, &can->dinfo,
                                        can->cfg->bar_updn_wline);
        lft += can->cfg->bar_width_full;
    }

    sint_t  x1, x2, y1, y2;

    /* 重新生成缺口 */
    x2 = can->image->position.x2;
    for (uint_t idx = 0; idx < can->gpcnt; idx++)
    {
        size_t  bias = can->gpdat[idx].bias;

        x1 = can->prlst[bias].x1;
        y1 = can->dinfo.get_positive_y(can->gpdat[idx].hi) + 1;
        y2 = can->dinfo.get_positive_y(can->gpdat[idx].lo) - 1;
        if (y1 > y2) y1 = y2;
        rect_set_xy(&can->gplst[idx], x1, y1, x2, y2);
    }
}

/*
=======================================
    重置蜡烛图
=======================================
*/
CR_API void_t
qtrd_candle_reset (
  __CR_IO__ sQTRD_candle*   can
    )
{
    can->dinfo.min_val = can->vinfo.LOW;
    can->dinfo.max_val = can->vinfo.HIGH;
    qtrd_candle_rebuild(can);
}

/*
=======================================
    绘制蜡烛图
=======================================
*/
CR_API void_t
qtrd_candle_draw (
  __CR_IN__ const sQTRD_candle* can
    )
{
    uint_t      hcnt = can->cfg->can_grid_h_count;
    uint_t      vcnt = can->cfg->can_grid_v_count;
    sQTRD_text* ghtx = mem_talloc(hcnt + 1, sQTRD_text);
    sQTRD_text* gvtx = mem_talloc(vcnt + 1, sQTRD_text);

    /* 一般都是成功的 */
    if (ghtx == NULL || gvtx == NULL) {
        TRY_FREE(ghtx);
        TRY_FREE(gvtx);
        return;
    }

    sFILL   fll;
    cpix_t  clr;
    bool_t  hll;
    double  stp, val;
    size_t  tim, hptr = 0, vptr = 0;
    sRECT*  gap = can->gplst, bx1, bx2;
    uint_t  spx = can->cfg->win_space_betw;
    uint_t  spy = can->cfg->text_sml.height;
    uint_t  dlt = can->cfg->bar_hollow_sub;
    uint_t  sub = dlt * 2;

    /* 开始绘制垂直线 */
    gvtx[vptr].x = qtrd_grid_v_draw(can, can->enter) + spx;
    gvtx[vptr].y = can->image->position.y2 - spy;
    gvtx[vptr++].date(&can->enter_date);
    if (vcnt > 1) {
        val = can->enter;
        stp = (val - can->leave) / vcnt;
        if (stp > 1) {
            for (uint_t idx = 0; idx < vcnt - 1; idx++, vptr++) {
                val -= stp;
                tim = (size_t)val;
                gvtx[vptr].x = qtrd_grid_v_draw(can, tim);
                gvtx[vptr].y = gvtx[0].y;
                gvtx[vptr].date((can->kdays->get()) + tim);
            }
        }
    }
    gvtx[vptr].x = qtrd_grid_v_draw(can, can->leave) - spx;
    gvtx[vptr].y = gvtx[0].y;
    gvtx[vptr++].date(&can->leave_date);

    /* 开始绘制水平线 */
    ghtx[hptr].x = gvtx[0].x;
    ghtx[hptr].y = qtrd_grid_h_draw(can, can->vinfo.HIGH);
    ghtx[hptr++].num2(can->vinfo.HIGH);
    if (hcnt > 1) {
        val = can->vinfo.HIGH;
        stp = (val - can->vinfo.LOW) / hcnt;
        for (uint_t idx = 0; idx < hcnt - 1; idx++, hptr++) {
            val -= stp;
            ghtx[hptr].x = ghtx[0].x;
            ghtx[hptr].y = qtrd_grid_h_draw(can, val) - spy / 2;
            ghtx[hptr].num2(val);
        }
    }
    ghtx[hptr].x = ghtx[0].x;
    ghtx[hptr].y = qtrd_grid_h_draw(can, can->vinfo.LOW) - spy;
    ghtx[hptr++].num2(can->vinfo.LOW);

    /* 开始绘制跳空口 */
    if (can->gplst != NULL && can->cfg->gap_is_render) {
        clr.val = can->cfg->gap_blk_color.val;
        for (uint_t idx = 0; idx < can->gpcnt; idx++, gap++) {
            fll.dx = gap->x1;
            fll.dy = gap->y1;
            fll.dw = gap->ww;
            fll.dh = gap->hh;
            fill_set32_c(can->image, &fll, clr, NULL);
        }
    }

    /* 开始绘制蜡烛图 */
    for (uint_t idx = 0; idx < can->count; idx++) {
        clr.val = can->clrst[idx].val;
        struct_cpy(&bx1, &can->prlst[idx], sRECT);
        struct_cpy(&bx2, &can->hllst[idx], sRECT);
        hll = (bool_t)(!!(can->atlst[idx] & QTRD_CANDLE_HOLLOW));
        if (!hll || bx1.ww <= sub || bx1.hh <= sub)
        {
            /* 实心蜡烛 */
            fll.dx = bx1.x1;
            fll.dy = bx1.y1;
            fll.dw = bx1.ww;
            fll.dh = bx1.hh;
            fill_set32_c(can->image, &fll, clr, NULL);
            fll.dx = bx2.x1;
            fll.dy = bx2.y1;
            fll.dw = bx2.ww;
            fll.dh = bx2.hh;
            fill_set32_c(can->image, &fll, clr, NULL);
        }
        else
        {
            /* 空心蜡烛 */
            draw_rect(can->image, &bx1, clr, pixel_set32z);
            for (uint_t kk = 0; kk < dlt; kk++) {
                bx1.x1 += 1;
                bx1.y1 += 1;
                bx1.x2 -= 1;
                bx1.y2 -= 1;
                bx1.ww -= 2;
                bx1.hh -= 2;
                draw_rect(can->image, &bx1, clr, pixel_set32z);
            }
            fll.dx = bx2.x1;
            fll.dy = bx2.y1;
            fll.dw = bx2.ww;
            fll.dh = bx1.y1 - bx2.y1 + 1;
            fill_set32_c(can->image, &fll, clr, NULL);
            fll.dy = bx1.y2;
            fll.dh = bx2.y2 - bx1.y2 + 1;
            fill_set32_c(can->image, &fll, clr, NULL);
        }
    }

    /* 开始绘制网格文字 */
    for (tim = 0; tim < hptr; tim++) {
        rect_set_wh(&bx1, ghtx[tim].x, ghtx[tim].y, 0, 0);
        qtrd_text_out(can->image, ghtx[tim].str, can->cfg->win_text_color,
                      can->cfg->win_tran_color, &bx1, 0, g_sml_font);
    }
    rect_set_wh(&bx1, gvtx[0].x, gvtx[0].y, 0, 0);
    qtrd_text_out(can->image, gvtx[0].str, can->cfg->win_text_color,
                  can->cfg->win_tran_color, &bx1, 0, g_sml_font);
    for (tim = 1; tim < vptr - 1; tim++) {
        qtrd_text_box(&bx2, gvtx[tim].str, g_sml_font);
        rect_set_wh(&bx1, gvtx[tim].x - bx2.ww / 2, gvtx[tim].y, 0, 0);
        qtrd_text_out(can->image, gvtx[tim].str, can->cfg->win_text_color,
                      can->cfg->win_tran_color, &bx1, 0, g_sml_font);
    }
    qtrd_text_box(&bx2, gvtx[tim].str, g_sml_font);
    rect_set_wh(&bx1, gvtx[tim].x - bx2.ww, gvtx[tim].y, 0, 0);
    qtrd_text_out(can->image, gvtx[tim].str, can->cfg->win_text_color,
                  can->cfg->win_tran_color, &bx1, 0, g_sml_font);
    mem_free(ghtx);
    mem_free(gvtx);
}

/*
=======================================
    绘制蜡烛图价格线
=======================================
*/
CR_API void_t
qtrd_candle_draw_price (
  __CR_IN__ const sQTRD_candle* can,
  __CR_IN__ uint_t              type
    )
{
    cpix_t          clrs;
    const double*   data;

    switch (type)
    {
        default:
        case QTRD_PRICE_TCLOSE:
            clrs.val = can->cfg->line_price_color[0].val;
            data = &(can->kdays->get())->data.detail.TCLOSE;
            break;

        case QTRD_PRICE_TOPEN:
            clrs.val = can->cfg->line_price_color[1].val;
            data = &(can->kdays->get())->data.detail.TOPEN;
            break;

        case QTRD_PRICE_HIGH:
            clrs.val = can->cfg->line_price_color[2].val;
            data = &(can->kdays->get())->data.detail.HIGH;
            break;

        case QTRD_PRICE_LOW:
            clrs.val = can->cfg->line_price_color[3].val;
            data = &(can->kdays->get())->data.detail.LOW;
            break;
    }
    qtrd_line_draw(can, data, sizeof(asy::kday_node), clrs);
}

/*****************************************************************************/
/*                                    MA                                     */
/*****************************************************************************/

/*
=======================================
    设置蜡烛图 MA 线
=======================================
*/
CR_API void_t
qtrd_candle_set_ma (
  __CR_IO__ sQTRD_candle*   can
    )
{
    ansi_t  name[64];

    can->dinfo.min_val = can->vinfo.LOW;
    can->dinfo.max_val = can->vinfo.HIGH;
    for (uint_t idx = 0; idx < can->cfg->line_avg_count; idx++)
    {
        const double*   data;

        if (!can->cfg->line_avg_ashow[idx])
            continue;
        sprintf(name, "MA%u", can->cfg->line_avg_atime[idx]);
        data = can->kdays->line(name);
        if (data == NULL)
            continue;
        qtrd_find_min_max(can, data, 0);
    }
    qtrd_candle_rebuild(can);
}

/*
=======================================
    绘制蜡烛图 MA 线
=======================================
*/
CR_API void_t
qtrd_candle_draw_ma (
  __CR_IN__ const sQTRD_candle* can
    )
{
    ansi_t  name[64];

    for (uint_t idx = 0; idx < can->cfg->line_avg_count; idx++)
    {
        const double*   data;

        if (!can->cfg->line_avg_ashow[idx])
            continue;
        sprintf(name, "MA%u", can->cfg->line_avg_atime[idx]);
        data = can->kdays->line(name);
        if (data == NULL)
            continue;
        qtrd_line_draw(can, data, 0, can->cfg->line_avg_color[idx]);
    }
}

/*****************************************************************************/
/*                                    EMA                                    */
/*****************************************************************************/

/*
=======================================
    设置蜡烛图 EMA 线
=======================================
*/
CR_API void_t
qtrd_candle_set_ema (
  __CR_IO__ sQTRD_candle*   can
    )
{
    ansi_t  name[64];

    can->dinfo.min_val = can->vinfo.LOW;
    can->dinfo.max_val = can->vinfo.HIGH;
    for (uint_t idx = 0; idx < can->cfg->line_avg_count; idx++)
    {
        const double*   data;

        if (!can->cfg->line_avg_ashow[idx])
            continue;
        sprintf(name, "EMA%u", can->cfg->line_avg_atime[idx]);
        data = can->kdays->line(name);
        if (data == NULL)
            continue;
        qtrd_find_min_max(can, data, 0);
    }
    qtrd_candle_rebuild(can);
}

/*
=======================================
    绘制蜡烛图 EMA 线
=======================================
*/
CR_API void_t
qtrd_candle_draw_ema (
  __CR_IN__ const sQTRD_candle* can
    )
{
    ansi_t  name[64];

    for (uint_t idx = 0; idx < can->cfg->line_avg_count; idx++)
    {
        const double*   data;

        if (!can->cfg->line_avg_ashow[idx])
            continue;
        sprintf(name, "EMA%u", can->cfg->line_avg_atime[idx]);
        data = can->kdays->line(name);
        if (data == NULL)
            continue;
        qtrd_line_draw(can, data, 0, can->cfg->line_avg_color[idx]);
    }
}

/*****************************************************************************/
/*                                    BBI                                    */
/*****************************************************************************/

/*
=======================================
    设置蜡烛图 BBI 线
=======================================
*/
CR_API void_t
qtrd_candle_set_bbi (
  __CR_IO__ sQTRD_candle*   can
    )
{
    ansi_t      name[64];
    /* --------------- */
    const double*   data;

    can->dinfo.min_val = can->vinfo.LOW;
    can->dinfo.max_val = can->vinfo.HIGH;
    sprintf(name, "BBI(%u,%u,%u,%u)", can->cfg->line_bbi_level[0],
                                      can->cfg->line_bbi_level[1],
                                      can->cfg->line_bbi_level[2],
                                      can->cfg->line_bbi_level[3]);
    data = can->kdays->line(name);
    if (data == NULL)
        return;
    qtrd_find_min_max(can, data, 0);
    qtrd_candle_rebuild(can);
}

/*
=======================================
    绘制蜡烛图 BBI 线
=======================================
*/
CR_API void_t
qtrd_candle_draw_bbi (
  __CR_IN__ const sQTRD_candle* can
    )
{
    ansi_t      name[64];
    /* --------------- */
    const double*   data;

    sprintf(name, "BBI(%u,%u,%u,%u)", can->cfg->line_bbi_level[0],
                                      can->cfg->line_bbi_level[1],
                                      can->cfg->line_bbi_level[2],
                                      can->cfg->line_bbi_level[3]);
    data = can->kdays->line(name);
    if (data == NULL)
        return;
    qtrd_line_draw(can, data, 0, can->cfg->line_bbi_color);
}

/*****************************************************************************/
/*                                   BOLL                                    */
/*****************************************************************************/

/*
=======================================
    设置蜡烛图 BOLL 线
=======================================
*/
CR_API void_t
qtrd_candle_set_boll (
  __CR_IO__ sQTRD_candle*   can
    )
{
    ansi_t      name[64];
    /* --------------- */
    const double*   upper;
    const double*   lower;

    can->dinfo.min_val = can->vinfo.LOW;
    can->dinfo.max_val = can->vinfo.HIGH;
    sprintf(name, "BOLL(UP,%u,%u,%d)", can->cfg->line_boll_days,
                                       can->cfg->line_boll_mult, -1);
    upper = can->kdays->line(name);
    if (upper == NULL)
        return;
    sprintf(name, "BOLL(DN,%u,%u,%d)", can->cfg->line_boll_days,
                                       can->cfg->line_boll_mult, -1);
    lower = can->kdays->line(name);
    if (lower == NULL)
        return;
    qtrd_find_min_max(can, upper, 0);
    qtrd_find_min_max(can, lower, 0);
    qtrd_candle_rebuild(can);
}

/*
=======================================
    绘制蜡烛图 BOLL 线
=======================================
*/
CR_API void_t
qtrd_candle_draw_boll (
  __CR_IN__ const sQTRD_candle* can
    )
{
    ansi_t      name[64];
    /* --------------- */
    const double*   upper;
    const double*   middl;
    const double*   lower;

    sprintf(name, "BOLL(UP,%u,%u,%d)", can->cfg->line_boll_days,
                                       can->cfg->line_boll_mult, -1);
    upper = can->kdays->line(name);
    if (upper == NULL)
        return;
    sprintf(name, "MA%u", can->cfg->line_boll_days);
    middl = can->kdays->line(name);
    if (middl == NULL)
        return;
    sprintf(name, "BOLL(DN,%u,%u,%d)", can->cfg->line_boll_days,
                                       can->cfg->line_boll_mult, -1);
    lower = can->kdays->line(name);
    if (lower == NULL)
        return;
    qtrd_line_draw(can, upper, 0, can->cfg->line_boll_color[0]);
    qtrd_line_draw(can, middl, 0, can->cfg->line_boll_color[1]);
    qtrd_line_draw(can, lower, 0, can->cfg->line_boll_color[2]);
}
