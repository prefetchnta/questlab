
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
  __CR_IN__ const sQTRD_amount* mntx
    )
{
    sint_t  dy = mntx->candl->cfg->text_sml.height;

    dy += mntx->candl->cfg->win_space_betw - 1;
    draw_lineh(mntx->imgvo, 0, mntx->imgvo->position.x2, dy,
               mntx->candl->cfg->line_grid_color, pixel_set32z);
    draw_lineh(mntx->imgva, 0, mntx->imgva->position.x2, dy,
               mntx->candl->cfg->line_grid_color, pixel_set32z);
}

/*
---------------------------------------
    绘制曲线（内部）
---------------------------------------
*/
static void_t
qtrd_line_draw (
  __CR_IN__ const sQTRD_amount* mntx,
  __CR_IN__ const double*       data,
  __CR_IN__ size_t              step,
  __CR_IN__ cpix_t              color,
  __CR_IN__ bool_t              is_vo
    )
{
    if (is_vo) {
        qtrd_line_draw_plus(mntx->chtvo, mntx->volst, &mntx->infvo,
                            mntx->candl->enter, mntx->candl->count, data,
                            step, mntx->candl->cfg->line_width, color);
    }
    else {
        qtrd_line_draw_plus(mntx->chtva, mntx->valst, &mntx->infva,
                            mntx->candl->enter, mntx->candl->count, data,
                            step, mntx->candl->cfg->line_width, color);
    }
}

/*
---------------------------------------
    设置极值（内部）
---------------------------------------
*/
static void_t
qtrd_find_min_max (
  __CR_IO__ sQTRD_amount*   mntx,
  __CR_IN__ const double*   data,
  __CR_IN__ size_t          step,
  __CR_IN__ bool_t          is_vo
    )
{
    if (is_vo) {
        qtrd_setup_min_max(&mntx->infvo, mntx->candl->leave,
                            mntx->candl->count, data, step);
    }
    else {
        qtrd_setup_min_max(&mntx->infva, mntx->candl->leave,
                            mntx->candl->count, data, step);
    }
}

/*****************************************************************************/
/*                                  量能图                                   */
/*****************************************************************************/

/*
=======================================
    创建量能图
=======================================
*/
CR_API bool_t
qtrd_amount_init (
  __CR_IN__ sIMAGE*         ivo,
  __CR_IN__ sIMAGE*         iva,
  __CR_OT__ sQTRD_amount*   out,
  __CR_IN__ sQTRD_candle*   can
    )
{
    sQTRD_amount    ret;

    /* 必须是32位色的正图 */
    if (ivo->gdi || ivo->fmt != CR_ARGB8888 ||
        iva->gdi || iva->fmt != CR_ARGB8888)
        return (FALSE);
    ret.imgvo = ivo;
    ret.imgva = iva;
    ret.chtvo = &out->clnvo;
    ret.chtva = &out->clnva;
    ret.candl = can;
    ret.infvo.width = can->dinfo.width;
    ret.infvo.min_val = 0;
    ret.infva.width = can->dinfo.width;
    ret.infva.min_val = 0;

    const asy::kday_node*   lst = can->kdays->get();

    /* 查找最大值 */
    ret.vomax = ret.vamax = -1;
    for (size_t ii = can->leave; ii <= can->enter; ii++) {
        if (ret.vomax < lst[ii].data.detail.VOTURNOVER)
            ret.vomax = lst[ii].data.detail.VOTURNOVER;
        if (ret.vamax < lst[ii].data.detail.VATURNOVER)
            ret.vamax = lst[ii].data.detail.VATURNOVER;
    }
    ret.infvo.max_val = ret.vomax;
    ret.infva.max_val = ret.vamax;

    /* 同步蜡烛图里的数据 */
    ret.volst = mem_talloc(can->count * 2, sRECT);
    if (ret.volst == NULL)
        return (FALSE);
    ret.valst = &ret.volst[can->count];
    ret.clrst = (cpix_t*)mem_dup(can->clrst, can->count * sizeof(cpix_t));
    if (ret.clrst == NULL) {
        mem_free(ret.clrst);
        return (FALSE);
    }
    struct_cpy(out, &ret, sQTRD_amount);

    uint_t  spy = can->cfg->text_sml.height;

    /* 图表的图片要去掉上面的文字区 */
    spy += can->cfg->win_space_betw;
    struct_cpy(&out->clnvo, ivo, sIMAGE);
    out->clnvo.data += spy * out->clnvo.bpl;
    out->clnvo.clip_win.y2 -= spy;
    out->clnvo.clip_win.hh -= spy;
    out->clnvo.position.y2 -= spy;
    out->clnvo.position.hh -= spy;
    out->infvo.height = out->clnvo.position.hh;
    struct_cpy(&out->clnva, iva, sIMAGE);
    out->clnva.data += spy * out->clnva.bpl;
    out->clnva.clip_win.y2 -= spy;
    out->clnva.clip_win.hh -= spy;
    out->clnva.position.y2 -= spy;
    out->clnva.position.hh -= spy;
    out->infva.height = out->clnva.position.hh;
    qtrd_amount_rebuild(out);
    return (TRUE);
}

/*
=======================================
    释放量能图
=======================================
*/
CR_API void_t
qtrd_amount_kill (
  __CR_IO__ sQTRD_amount*   mnt
    )
{
    mem_free(mnt->volst);
    mem_free(mnt->clrst);
    struct_zero(mnt, sQTRD_amount);
}

/*
=======================================
    重建量能图
=======================================
*/
CR_API void_t
qtrd_amount_rebuild (
  __CR_IO__ sQTRD_amount*   mnt
    )
{
    sint_t                  lft = 0;
    const asy::kday_node*   stt = mnt->candl->kdays->get(mnt->candl->enter);

    /* 开始生成量柱 */
    for (uint_t idx = 0; idx < mnt->candl->count; idx++, stt--)
    {
        /* 成交量 & 成交额方框计算 */
        stt->data.detail.value_box(&mnt->volst[idx], lft, &mnt->infvo, true);
        stt->data.detail.value_box(&mnt->valst[idx], lft, &mnt->infva, false);
        lft += mnt->candl->cfg->bar_width_full;
    }
}

/*
=======================================
    重置量能图
=======================================
*/
CR_API void_t
qtrd_amount_reset (
  __CR_IO__ sQTRD_amount*   mnt
    )
{
    mnt->infvo.max_val = mnt->vomax;
    mnt->infva.max_val = mnt->vamax;
    qtrd_amount_rebuild(mnt);
}

/*
=======================================
    绘制量能图
=======================================
*/
CR_API void_t
qtrd_amount_draw (
  __CR_IN__ const sQTRD_amount* mnt
    )
{
    sFILL   fll;
    sRECT   bx1;
    sRECT   bx2;
    cpix_t  clr;
    bool_t  hll;
    uint_t  dlt = mnt->candl->cfg->bar_hollow_sub;
    uint_t  sub = dlt * 2;

    /* 开始绘制量能图 */
    qtrd_title_draw(mnt);
    for (uint_t idx = 0; idx < mnt->candl->count; idx++) {
        clr.val = mnt->clrst[idx].val;
        struct_cpy(&bx1, &mnt->volst[idx], sRECT);
        struct_cpy(&bx2, &mnt->valst[idx], sRECT);
        hll = (bool_t)(!!(mnt->candl->atlst[idx] & QTRD_CANDLE_HOLLOW));

        /* 成交量柱 */
        if (!hll || bx1.ww <= sub || bx1.hh <= sub)
        {
            /* 实心量柱 */
            fll.dx = bx1.x1;
            fll.dy = bx1.y1;
            fll.dw = bx1.ww;
            fll.dh = bx1.hh;
            fill_set32_c(mnt->chtvo, &fll, clr, NULL);
        }
        else
        {
            /* 空心量柱 */
            draw_rect(mnt->chtvo, &bx1, clr, pixel_set32z);
            for (uint_t kk = 0; kk < dlt; kk++) {
                bx1.x1 += 1;
                bx1.y1 += 1;
                bx1.x2 -= 1;
                bx1.y2 -= 1;
                bx1.ww -= 2;
                bx1.hh -= 2;
                draw_rect(mnt->chtvo, &bx1, clr, pixel_set32z);
            }
        }

        /* 成交额柱 */
        if (!hll || bx2.ww <= sub || bx2.hh <= sub)
        {
            /* 实心量柱 */
            fll.dx = bx2.x1;
            fll.dy = bx2.y1;
            fll.dw = bx2.ww;
            fll.dh = bx2.hh;
            fill_set32_c(mnt->chtva, &fll, clr, NULL);
        }
        else
        {
            /* 空心量柱 */
            draw_rect(mnt->chtva, &bx2, clr, pixel_set32z);
            for (uint_t kk = 0; kk < dlt; kk++) {
                bx2.x1 += 1;
                bx2.y1 += 1;
                bx2.x2 -= 1;
                bx2.y2 -= 1;
                bx2.ww -= 2;
                bx2.hh -= 2;
                draw_rect(mnt->chtva, &bx2, clr, pixel_set32z);
            }
        }
    }
}

/*****************************************************************************/
/*                                    MA                                     */
/*****************************************************************************/

/*
=======================================
    设置量能图 MA 线
=======================================
*/
CR_API void_t
qtrd_amount_set_ma (
  __CR_IO__ sQTRD_amount*   mnt
    )
{
    ansi_t      name[64];
    /* --------------- */
    const double*   vo_1;
    const double*   vo_2;
    const double*   va_1;
    const double*   va_2;

    mnt->infvo.max_val = mnt->vomax;
    mnt->infva.max_val = mnt->vamax;
    sprintf(name, "VO%u", mnt->candl->cfg->line_amnt_level[0]);
    vo_1 = mnt->candl->kdays->line(name);
    if (vo_1 == NULL)
        return;
    sprintf(name, "VO%u", mnt->candl->cfg->line_amnt_level[1]);
    vo_2 = mnt->candl->kdays->line(name);
    if (vo_2 == NULL)
        return;
    sprintf(name, "VA%u", mnt->candl->cfg->line_amnt_level[0]);
    va_1 = mnt->candl->kdays->line(name);
    if (va_1 == NULL)
        return;
    sprintf(name, "VA%u", mnt->candl->cfg->line_amnt_level[1]);
    va_2 = mnt->candl->kdays->line(name);
    if (va_2 == NULL)
        return;
    qtrd_find_min_max(mnt, vo_1, 0, TRUE);
    qtrd_find_min_max(mnt, vo_2, 0, TRUE);
    qtrd_find_min_max(mnt, va_1, 0, FALSE);
    qtrd_find_min_max(mnt, va_2, 0, FALSE);
    qtrd_amount_rebuild(mnt);
}

/*
=======================================
    绘制量能图 MA 线
=======================================
*/
CR_API void_t
qtrd_amount_draw_ma (
  __CR_IN__ const sQTRD_amount* mnt
    )
{
    ansi_t      name[64];
    /* --------------- */
    const double*   vo_1;
    const double*   vo_2;
    const double*   va_1;
    const double*   va_2;

    sprintf(name, "VO%u", mnt->candl->cfg->line_amnt_level[0]);
    vo_1 = mnt->candl->kdays->line(name);
    if (vo_1 == NULL)
        return;
    sprintf(name, "VO%u", mnt->candl->cfg->line_amnt_level[1]);
    vo_2 = mnt->candl->kdays->line(name);
    if (vo_2 == NULL)
        return;
    sprintf(name, "VA%u", mnt->candl->cfg->line_amnt_level[0]);
    va_1 = mnt->candl->kdays->line(name);
    if (va_1 == NULL)
        return;
    sprintf(name, "VA%u", mnt->candl->cfg->line_amnt_level[1]);
    va_2 = mnt->candl->kdays->line(name);
    if (va_2 == NULL)
        return;
    qtrd_line_draw(mnt, vo_1, 0, mnt->candl->cfg->line_amnt_color[0], TRUE);
    qtrd_line_draw(mnt, vo_2, 0, mnt->candl->cfg->line_amnt_color[1], TRUE);
    qtrd_line_draw(mnt, va_1, 0, mnt->candl->cfg->line_amnt_color[0], FALSE);
    qtrd_line_draw(mnt, va_2, 0, mnt->candl->cfg->line_amnt_color[1], FALSE);
}
