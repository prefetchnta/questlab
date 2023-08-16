
#include "QstTrade_Int.h"

/*
=======================================
    默认参数设置
=======================================
*/
CR_API void_t
qtrd_configure_reset (
  __CR_OT__ sQTRD_conf* cfg
    )
{
    /* 参数清零 */
    struct_zero(cfg, sQTRD_conf);

    /* 默认参数 */
    cfg->win_space_betw = 3;
    cfg->win_text_color.val = 0xFF9C9CAA;

    qtrd_font_param(&cfg->text_sml, CR_WS("YaHei Consolas Hybrid"), 25, 0);
    qtrd_font_param(&cfg->text_mdl, CR_WS("YaHei Consolas Hybrid"), 50, 0);
    qtrd_font_param(&cfg->text_lrg, CR_WS("YaHei Consolas Hybrid"), 100, 0);

    cfg->bar_width_full = 6;
    cfg->bar_side_space = 1;
    cfg->bar_updn_wline = 1;
    cfg->bar_color_rise.val = 0xFFC80000;
    cfg->bar_color_drop.val = 0xFF00C800;
    cfg->bar_color_eque.val = 0xFFE1E1E1;
    cfg->bar_is_rhollow = TRUE;
    cfg->bar_is_dhollow = FALSE;
    cfg->bar_hollow_sub = 0;

    cfg->gap_is_render = TRUE;
    cfg->gap_blk_color.val = 0xFF484848;

    cfg->can_grid_h_count = 3;
    cfg->can_grid_v_count = 4;
    cfg->can_up_dn_padded = 50;

    cfg->line_width = 1;
    cfg->line_grid_dash = 5;
    cfg->line_grid_width = 1;
    cfg->line_grid_color.val = 0xFF383838;

    cfg->line_avg_count = 8;
    cfg->line_avg_atime[0] = 5;
    cfg->line_avg_atime[1] = 10;
    cfg->line_avg_atime[2] = 20;
    cfg->line_avg_atime[3] = 30;
    cfg->line_avg_atime[4] = 60;
    cfg->line_avg_atime[5] = 120;
    cfg->line_avg_atime[6] = 125;
    cfg->line_avg_atime[7] = 250;

    cfg->line_avg_ashow[0] = TRUE;
    cfg->line_avg_ashow[1] = TRUE;
    cfg->line_avg_ashow[2] = TRUE;
    cfg->line_avg_ashow[3] = TRUE;
    cfg->line_avg_ashow[4] = TRUE;
    cfg->line_avg_ashow[5] = TRUE;
    cfg->line_avg_ashow[6] = FALSE;
    cfg->line_avg_ashow[7] = TRUE;

    cfg->line_avg_color[0].val = 0xFFE1E1E1;
    cfg->line_avg_color[1].val = 0xFFFFAF00;
    cfg->line_avg_color[2].val = 0xFFEB00AF;
    cfg->line_avg_color[3].val = 0xFF00987D;
    cfg->line_avg_color[4].val = 0xFF00AFC8;
    cfg->line_avg_color[5].val = 0xFF8746AF;
    cfg->line_avg_color[6].val = 0xFFF07646;
    cfg->line_avg_color[7].val = 0xFF874600;

    cfg->line_price_color[0].val = 0xFFE1E1E1;
    cfg->line_price_color[1].val = 0xFFDC6E9C;
    cfg->line_price_color[2].val = 0xFFC80000;
    cfg->line_price_color[3].val = 0xFF00C800;

    cfg->line_bbi_level[0] = 3;
    cfg->line_bbi_level[1] = 6;
    cfg->line_bbi_level[2] = 12;
    cfg->line_bbi_level[3] = 24;
    cfg->line_bbi_color.val = 0xFFFFAF00;

    cfg->line_boll_days = 20;
    cfg->line_boll_mult = 2;
    cfg->line_boll_color[0].val = 0xFF00AFC8;
    cfg->line_boll_color[1].val = 0xFFFFAF00;
    cfg->line_boll_color[2].val = 0xFFEB00AF;

    cfg->line_amnt_level[0] = 5;
    cfg->line_amnt_level[1] = 10;
    cfg->line_amnt_color[0].val = 0xFFE1E1E1;
    cfg->line_amnt_color[1].val = 0xFFFFAF00;

    cfg->macd_days_fast = 12;
    cfg->macd_days_slow = 26;
    cfg->macd_days_diff = 9;
    cfg->macd_line_dif_color.val = 0xFFE1E1E1;
    cfg->macd_line_dea_color.val = 0xFFFFAF00;

    cfg->kdj_days_r = 9;
    cfg->kdj_days_k = 3;
    cfg->kdj_days_d = 3;
    cfg->kdj_line_k_color.val = 0xFFE1E1E1;
    cfg->kdj_line_d_color.val = 0xFFF07646;
    cfg->kdj_line_j_color.val = 0xFFDC6E9C;
    cfg->kdj_line_j_is_render = TRUE;

    cfg->rsi_line_level[0] = 6;
    cfg->rsi_line_level[1] = 12;
    cfg->rsi_line_level[2] = 24;
    cfg->rsi_line_color[0].val = 0xFFE1E1E1;
    cfg->rsi_line_color[1].val = 0xFFF07646;
    cfg->rsi_line_color[2].val = 0xFFDC6E9C;

    cfg->wr_line_level = 10;
    cfg->wr_line_color.val = 0xFFE1E1E1;

    cfg->cci_line_level = 14;
    cfg->cci_line_color.val = 0xFFE1E1E1;
}

/*
---------------------------------------
    解析 Font 属性
---------------------------------------
*/
static void_t
qtrd_font_attr (
  __CR_OT__ sQTRD_font* ft,
  __CR_IN__ sXNODEw*    nd
    )
{
    sint_t  height;
    uint_t  weight;
    wide_t* name = xml_node_stringW(nd);

    height = xml_attr_intxW(CR_WS("height"), ft->height, nd);
    weight = xml_attr_intxW(CR_WS("weight"), ft->weight, nd);
    if (name == NULL) {
        qtrd_font_param(ft, ft->name, height, weight);
        return;
    }
    str_trimW(name);
    if (*name != 0)
        qtrd_font_param(ft, name, height, weight);
    else
        qtrd_font_param(ft, ft->name, height, weight);
    mem_free(name);
}

/*
---------------------------------------
    解析 Window 标签
---------------------------------------
*/
static void_t
qtrd_window_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("win_space_betw")) == 0)
        cf->win_space_betw = xml_node_intxW(cf->win_space_betw, nd);
    else
    if (str_cmpW(nd->name, CR_WS("win_text_color")) == 0)
        cf->win_text_color.val = xml_node_intx32W(cf->win_text_color.val, nd);
    else
    if (str_cmpW(nd->name, CR_WS("text_sml")) == 0)
        qtrd_font_attr(&cf->text_sml, nd);
    else
    if (str_cmpW(nd->name, CR_WS("text_mdl")) == 0)
        qtrd_font_attr(&cf->text_mdl, nd);
    else
    if (str_cmpW(nd->name, CR_WS("text_lrg")) == 0)
        qtrd_font_attr(&cf->text_lrg, nd);
}

/*
---------------------------------------
    解析 Candle 标签
---------------------------------------
*/
static void_t
qtrd_candle_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("bar_width_full")) == 0)
        cf->bar_width_full = xml_node_intxW(cf->bar_width_full, nd);
    else
    if (str_cmpW(nd->name, CR_WS("bar_side_space")) == 0)
        cf->bar_side_space = xml_node_intxW(cf->bar_side_space, nd);
    else
    if (str_cmpW(nd->name, CR_WS("bar_updn_wline")) == 0)
        cf->bar_updn_wline = xml_node_intxW(cf->bar_updn_wline, nd);
    else
    if (str_cmpW(nd->name, CR_WS("bar_color_rise")) == 0)
        cf->bar_color_rise.val = xml_node_intx32W(cf->bar_color_rise.val, nd);
    else
    if (str_cmpW(nd->name, CR_WS("bar_color_drop")) == 0)
        cf->bar_color_drop.val = xml_node_intx32W(cf->bar_color_drop.val, nd);
    else
    if (str_cmpW(nd->name, CR_WS("bar_color_eque")) == 0)
        cf->bar_color_eque.val = xml_node_intx32W(cf->bar_color_eque.val, nd);
    else
    if (str_cmpW(nd->name, CR_WS("bar_is_rhollow")) == 0)
        cf->bar_is_rhollow = xml_node_intxW(cf->bar_is_rhollow, nd);
    else
    if (str_cmpW(nd->name, CR_WS("bar_is_dhollow")) == 0)
        cf->bar_is_dhollow = xml_node_intxW(cf->bar_is_dhollow, nd);
    else
    if (str_cmpW(nd->name, CR_WS("bar_hollow_sub")) == 0)
        cf->bar_hollow_sub = xml_node_intxW(cf->bar_hollow_sub, nd);
    else
    if (str_cmpW(nd->name, CR_WS("can_grid_h_count")) == 0)
        cf->can_grid_h_count = xml_node_intxW(cf->can_grid_h_count, nd);
    else
    if (str_cmpW(nd->name, CR_WS("can_grid_v_count")) == 0)
        cf->can_grid_v_count = xml_node_intxW(cf->can_grid_v_count, nd);
    else
    if (str_cmpW(nd->name, CR_WS("can_up_dn_padded")) == 0)
        cf->can_up_dn_padded = xml_node_intxW(cf->can_up_dn_padded, nd);
}

/*
---------------------------------------
    解析 CandleGap 标签
---------------------------------------
*/
static void_t
qtrd_candle_gap_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("gap_is_render")) == 0)
        cf->gap_is_render = xml_node_intxW(cf->gap_is_render, nd);
    else
    if (str_cmpW(nd->name, CR_WS("gap_blk_color")) == 0)
        cf->gap_blk_color.val = xml_node_intx32W(cf->gap_blk_color.val, nd);
}

/*
---------------------------------------
    解析 Lines 标签
---------------------------------------
*/
static void_t
qtrd_lines_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("line_width")) == 0) {
        cf->line_width = xml_node_intxW(cf->line_width, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_grid_dash")) == 0) {
        cf->line_grid_dash = xml_node_intxW(cf->line_grid_dash, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_grid_width")) == 0) {
        cf->line_grid_width = xml_node_intxW(cf->line_grid_width, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_grid_color")) == 0) {
        cf->line_grid_color.val = xml_node_intx32W(
                cf->line_grid_color.val, nd);
    }
}

/*
---------------------------------------
    解析 LineAvg 属性
---------------------------------------
*/
static void_t
qtrd_line_avg_attr (
  __CR_OT__ uint_t*     at,
  __CR_OT__ bool_t*     as,
  __CR_OT__ cpix_t*     cl,
  __CR_IN__ sXNODEw*    nd
    )
{
    cl->val = xml_node_intx32W(cl->val, nd);
    at[0] = xml_attr_intxW(CR_WS("atime"), *at, nd);
    as[0] = xml_attr_intxW(CR_WS("ashow"), *as, nd);
}

/*
---------------------------------------
    解析 LineAvg 标签
---------------------------------------
*/
static void_t
qtrd_line_avg_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    uint_t  idx;

    if (str_cmpW(nd->name, CR_WS("line_avg_count")) == 0) {
        cf->line_avg_count = xml_node_intxW(cf->line_avg_count, nd);
    }
    else
    if (str_strW(nd->name, CR_WS("line_avg_attr0")) == nd->name ||
        str_strW(nd->name, CR_WS("line_avg_attr1")) == nd->name) {
        idx = str2intW(nd->name + 13);
        if (idx == 0 || idx > 16)
            return;
        idx -= 1;
        qtrd_line_avg_attr(&cf->line_avg_atime[idx], &cf->line_avg_ashow[idx],
                           &cf->line_avg_color[idx], nd);
    }
}

/*
---------------------------------------
    解析 LinePrice 标签
---------------------------------------
*/
static void_t
qtrd_line_price_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("line_price_tclose_color")) == 0) {
        cf->line_price_color[0].val = xml_node_intx32W(
                cf->line_price_color[0].val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_price_topen_color")) == 0) {
        cf->line_price_color[1].val = xml_node_intx32W(
                cf->line_price_color[1].val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_price_high_color")) == 0) {
        cf->line_price_color[2].val = xml_node_intx32W(
                cf->line_price_color[2].val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_price_low_color")) == 0) {
        cf->line_price_color[3].val = xml_node_intx32W(
                cf->line_price_color[3].val, nd);
    }
}

/*
---------------------------------------
    解析 LineBBI 标签
---------------------------------------
*/
static void_t
qtrd_line_bbi_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("line_bbi_color")) == 0)
        cf->line_bbi_color.val = xml_node_intx32W(cf->line_bbi_color.val, nd);
    else
    if (str_cmpW(nd->name, CR_WS("line_bbi_level1")) == 0)
        cf->line_bbi_level[0] = xml_node_intxW(cf->line_bbi_level[0], nd);
    else
    if (str_cmpW(nd->name, CR_WS("line_bbi_level2")) == 0)
        cf->line_bbi_level[1] = xml_node_intxW(cf->line_bbi_level[1], nd);
    else
    if (str_cmpW(nd->name, CR_WS("line_bbi_level3")) == 0)
        cf->line_bbi_level[2] = xml_node_intxW(cf->line_bbi_level[2], nd);
    else
    if (str_cmpW(nd->name, CR_WS("line_bbi_level4")) == 0)
        cf->line_bbi_level[3] = xml_node_intxW(cf->line_bbi_level[3], nd);
}

/*
---------------------------------------
    解析 LineBOLL 标签
---------------------------------------
*/
static void_t
qtrd_line_boll_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("line_boll_days")) == 0) {
        cf->line_boll_days = xml_node_intxW(cf->line_boll_days, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_boll_mult")) == 0) {
        cf->line_boll_mult = xml_node_intxW(cf->line_boll_mult, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_boll_upper_color")) == 0) {
        cf->line_boll_color[0].val = xml_node_intx32W(
                cf->line_boll_color[0].val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_boll_mid_color")) == 0) {
        cf->line_boll_color[1].val = xml_node_intx32W(
                cf->line_boll_color[1].val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_boll_lower_color")) == 0) {
        cf->line_boll_color[2].val = xml_node_intx32W(
                cf->line_boll_color[2].val, nd);
    }
}

/*
---------------------------------------
    解析 ChartAmount 标签
---------------------------------------
*/
static void_t
qtrd_chart_amount_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("line_amnt_level1")) == 0) {
        cf->line_amnt_level[0] = xml_node_intxW(cf->line_amnt_level[0], nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_amnt_level2")) == 0) {
        cf->line_amnt_level[1] = xml_node_intxW(cf->line_amnt_level[1], nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_amnt_color1")) == 0) {
        cf->line_amnt_color[0].val = xml_node_intx32W(
                cf->line_amnt_color[0].val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("line_amnt_color2")) == 0) {
        cf->line_amnt_color[1].val = xml_node_intx32W(
                cf->line_amnt_color[1].val, nd);
    }
}

/*
---------------------------------------
    解析 ChartMACD 标签
---------------------------------------
*/
static void_t
qtrd_chart_macd_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("macd_days_fast")) == 0) {
        cf->macd_days_fast = xml_node_intxW(cf->macd_days_fast, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("macd_days_slow")) == 0) {
        cf->macd_days_slow = xml_node_intxW(cf->macd_days_slow, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("macd_days_diff")) == 0) {
        cf->macd_days_diff = xml_node_intxW(cf->macd_days_diff, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("macd_line_dif_color")) == 0) {
        cf->macd_line_dif_color.val = xml_node_intx32W(
                cf->macd_line_dif_color.val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("macd_line_dea_color")) == 0) {
        cf->macd_line_dea_color.val = xml_node_intx32W(
                cf->macd_line_dea_color.val, nd);
    }
}

/*
---------------------------------------
    解析 ChartKDJ 标签
---------------------------------------
*/
static void_t
qtrd_chart_kdj_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("kdj_days_r")) == 0) {
        cf->kdj_days_r = xml_node_intxW(cf->kdj_days_r, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("kdj_days_k")) == 0) {
        cf->kdj_days_k = xml_node_intxW(cf->kdj_days_k, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("kdj_days_d")) == 0) {
        cf->kdj_days_d = xml_node_intxW(cf->kdj_days_d, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("kdj_line_k_color")) == 0) {
        cf->kdj_line_k_color.val = xml_node_intx32W(
                cf->kdj_line_k_color.val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("kdj_line_d_color")) == 0) {
        cf->kdj_line_d_color.val = xml_node_intx32W(
                cf->kdj_line_d_color.val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("kdj_line_j_color")) == 0) {
        cf->kdj_line_j_color.val = xml_node_intx32W(
                cf->kdj_line_j_color.val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("kdj_line_j_is_render")) == 0) {
        cf->kdj_line_j_is_render = xml_node_intxW(
                cf->kdj_line_j_is_render, nd);
    }
}

/*
---------------------------------------
    解析 ChartRSI 标签
---------------------------------------
*/
static void_t
qtrd_chart_rsi_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("rsi_line_level1")) == 0) {
        cf->rsi_line_level[0] = xml_node_intxW(cf->rsi_line_level[0], nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("rsi_line_level2")) == 0) {
        cf->rsi_line_level[1] = xml_node_intxW(cf->rsi_line_level[1], nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("rsi_line_level3")) == 0) {
        cf->rsi_line_level[2] = xml_node_intxW(cf->rsi_line_level[2], nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("rsi_line_color1")) == 0) {
        cf->rsi_line_color[0].val = xml_node_intx32W(
                cf->rsi_line_color[0].val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("rsi_line_color2")) == 0) {
        cf->rsi_line_color[1].val = xml_node_intx32W(
                cf->rsi_line_color[1].val, nd);
    }
    else
    if (str_cmpW(nd->name, CR_WS("rsi_line_color3")) == 0) {
        cf->rsi_line_color[2].val = xml_node_intx32W(
                cf->rsi_line_color[2].val, nd);
    }
}

/*
---------------------------------------
    解析 ChartWR 标签
---------------------------------------
*/
static void_t
qtrd_chart_wr_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("wr_line_level")) == 0)
        cf->wr_line_level = xml_node_intxW(cf->wr_line_level, nd);
    else
    if (str_cmpW(nd->name, CR_WS("wr_line_color")) == 0)
        cf->wr_line_color.val = xml_node_intx32W(cf->wr_line_color.val, nd);
}

/*
---------------------------------------
    解析 ChartCCI 标签
---------------------------------------
*/
static void_t
qtrd_chart_cci_parse (
  __CR_OT__ sQTRD_conf* cf,
  __CR_IN__ sXNODEw*    nd
    )
{
    if (str_cmpW(nd->name, CR_WS("cci_line_level")) == 0)
        cf->cci_line_level = xml_node_intxW(cf->cci_line_level, nd);
    else
    if (str_cmpW(nd->name, CR_WS("cci_line_color")) == 0)
        cf->cci_line_color.val = xml_node_intx32W(cf->cci_line_color.val, nd);
}

/*
=======================================
    加载配置文件
=======================================
*/
CR_API bool_t
qtrd_configure_load (
  __CR_OT__ sQTRD_conf*     cfg,
  __CR_IN__ const ansi_t*   file
    )
{
    sXMLw*  xml;
    sint_t  tag;
    leng_t  idx;
    ansi_t* str;

    /* 读入整个 XML 文件 */
    str = file_load_as_strA(file);
    if (str == NULL)
        return (FALSE);

    /* 开始解析 XML 文件 */
    xml = xml_from_textW(str);
    mem_free(str);
    if (xml == NULL)
        return (FALSE);

    /* 检查根标签 */
    if (xml->nodes[0].name[0] == CR_WC('?'))
        idx = 1;
    else
        idx = 0;
    if (xml->nodes[idx].closed ||
        str_cmpW(xml->nodes[idx].name, CR_WS("QstTrade")) != 0) {
        xml_closeW(xml);
        return (FALSE);
    }

    /* 逐个标签解析 */
    qtrd_configure_reset(cfg);
    for (tag = 0, idx++; idx < xml->count; idx++)
    {
        sXNODEw*    nd = &xml->nodes[idx];

        /* 分级状态机 */
        if (tag == 0)
        {
            /* 一级标签 */
            if (str_cmpW(nd->name, CR_WS("/QstTrade")) == 0)
                break;
            if (!nd->closed) {
                if (str_cmpW(nd->name, CR_WS("Window")) == 0) {
                    tag = 1;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("Candle")) == 0) {
                    tag = 2;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("CandleGap")) == 0) {
                    tag = 3;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("Lines")) == 0) {
                    tag = 4;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("LineAvg")) == 0) {
                    tag = 5;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("LinePrice")) == 0) {
                    tag = 6;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("LineBBI")) == 0) {
                    tag = 7;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("LineBOLL")) == 0) {
                    tag = 8;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("ChartAmount")) == 0) {
                    tag = 100;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("ChartMACD")) == 0) {
                    tag = 101;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("ChartKDJ")) == 0) {
                    tag = 102;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("ChartRSI")) == 0) {
                    tag = 103;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("ChartWR")) == 0) {
                    tag = 104;
                    continue;
                }
                if (str_cmpW(nd->name, CR_WS("ChartCCI")) == 0) {
                    tag = 105;
                    continue;
                }
            }
        }
        else
        if (tag == 1)
        {
            /* Window 标签 */
            if (str_cmpW(nd->name, CR_WS("/Window")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_window_parse(cfg, nd);
        }
        else
        if (tag == 2)
        {
            /* Candle 标签 */
            if (str_cmpW(nd->name, CR_WS("/Candle")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_candle_parse(cfg, nd);
        }
        else
        if (tag == 3)
        {
            /* CandleGap 标签 */
            if (str_cmpW(nd->name, CR_WS("/CandleGap")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_candle_gap_parse(cfg, nd);
        }
        else
        if (tag == 4)
        {
            /* Lines 标签 */
            if (str_cmpW(nd->name, CR_WS("/Lines")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_lines_parse(cfg, nd);
        }
        else
        if (tag == 5)
        {
            /* LineAvg 标签 */
            if (str_cmpW(nd->name, CR_WS("/LineAvg")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_line_avg_parse(cfg, nd);
        }
        else
        if (tag == 6)
        {
            /* LinePrice 标签 */
            if (str_cmpW(nd->name, CR_WS("/LinePrice")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_line_price_parse(cfg, nd);
        }
        else
        if (tag == 7)
        {
            /* LineBBI 标签 */
            if (str_cmpW(nd->name, CR_WS("/LineBBI")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_line_bbi_parse(cfg, nd);
        }
        else
        if (tag == 8)
        {
            /* LineBOLL 标签 */
            if (str_cmpW(nd->name, CR_WS("/LineBOLL")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_line_boll_parse(cfg, nd);
        }
        else
        if (tag == 100)
        {
            /* ChartAmount 标签 */
            if (str_cmpW(nd->name, CR_WS("/ChartAmount")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_chart_amount_parse(cfg, nd);
        }
        else
        if (tag == 101)
        {
            /* ChartMACD 标签 */
            if (str_cmpW(nd->name, CR_WS("/ChartMACD")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_chart_macd_parse(cfg, nd);
        }
        else
        if (tag == 102)
        {
            /* ChartKDJ 标签 */
            if (str_cmpW(nd->name, CR_WS("/ChartKDJ")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_chart_kdj_parse(cfg, nd);
        }
        else
        if (tag == 103)
        {
            /* ChartRSI 标签 */
            if (str_cmpW(nd->name, CR_WS("/ChartRSI")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_chart_rsi_parse(cfg, nd);
        }
        else
        if (tag == 104)
        {
            /* ChartWR 标签 */
            if (str_cmpW(nd->name, CR_WS("/ChartWR")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_chart_wr_parse(cfg, nd);
        }
        else
        if (tag == 105)
        {
            /* ChartCCI 标签 */
            if (str_cmpW(nd->name, CR_WS("/ChartCCI")) == 0) {
                tag = 0;
                continue;
            }
            qtrd_chart_cci_parse(cfg, nd);
        }

        /* 跳过不识别的标签 */
        idx = xml_skip_tagW(xml->nodes, idx, xml->count) - 1;
    }
    xml_closeW(xml);
    return (TRUE);
}

/*
=======================================
    应用配置参数
=======================================
*/
CR_API bool_t
qtrd_configure_apply (
  __CR_IN__ const sQTRD_conf*   cfg
    )
{
    iGFX2*  gfx;

    /* 临时表面创建 */
    if (g_bmp_temp == NULL) {
        gfx = (iGFX2*)create_gdi_bitmap(16, 16, CR_ARGB8888);
        if (gfx == NULL)
            return (FALSE);
        g_bmp_temp = gfx;
    }

    iFONT*  sml = qtrd_font_get(&cfg->text_sml);
    iFONT*  mdl = qtrd_font_get(&cfg->text_mdl);
    iFONT*  lrg = qtrd_font_get(&cfg->text_lrg);

    /* 全局字体创建 */
    if (sml == NULL || mdl == NULL || lrg == NULL) {
        if (sml != NULL) CR_VCALL(sml)->release(sml);
        if (mdl != NULL) CR_VCALL(mdl)->release(mdl);
        if (lrg != NULL) CR_VCALL(lrg)->release(lrg);
        CR_VCALL(g_bmp_temp)->release(g_bmp_temp);
        g_bmp_temp = NULL;
        return (FALSE);
    }
    if (g_sml_font != NULL)
        CR_VCALL(g_sml_font)->release(g_sml_font);
    if (g_mdl_font != NULL)
        CR_VCALL(g_mdl_font)->release(g_mdl_font);
    if (g_lrg_font != NULL)
        CR_VCALL(g_lrg_font)->release(g_lrg_font);
    g_sml_font = sml;
    g_mdl_font = mdl;
    g_lrg_font = lrg;
    return (TRUE);
}

/*
=======================================
    创建数据指标
=======================================
*/
CR_API void_t
qtrd_indicator_make (
  __CR_IO__ asy::kdays*         dat,
  __CR_IN__ const sQTRD_conf*   cfg
    )
{
    /* MA & EMA */
    for (uint_t idx = 0; idx < cfg->line_avg_count; idx++) {
        if (cfg->line_avg_atime[idx] <= 1)
            continue;
        dat->make_MA(cfg->line_avg_atime[idx]);
        dat->make_EMA(cfg->line_avg_atime[idx]);
    }

    /* BBI */
    dat->make_BBI(cfg->line_bbi_level[0], cfg->line_bbi_level[1],
                  cfg->line_bbi_level[2], cfg->line_bbi_level[3]);
    /* BOLL */
    dat->make_BOLL(cfg->line_boll_days, cfg->line_boll_mult);

    /* 量能均线 */
    dat->make_VO(cfg->line_amnt_level[0], cfg->line_amnt_level[1]);
    dat->make_VA(cfg->line_amnt_level[0], cfg->line_amnt_level[1]);

    /* MACD */
    dat->make_MACD(cfg->macd_days_fast, cfg->macd_days_slow,
                   cfg->macd_days_diff);
    /* KDJ */
    dat->make_KDJ(cfg->kdj_days_r, cfg->kdj_days_k, cfg->kdj_days_d);

    /* RSI */
    dat->make_RSI3(cfg->rsi_line_level[0], cfg->rsi_line_level[1],
                   cfg->rsi_line_level[2]);
    /* WR */
    dat->make_WR(cfg->wr_line_level);

    /* CCI */
    dat->make_CCI(cfg->cci_line_level);
}
