/*
 ================================================
 *      ######
 *    #########                            ##
 *   ####    ###                           ##
 *   ##       ##                           ##
 *  ##         ## ##   ##   ####     ### ######
 *  ##         ## ##   ##  ######  ##### ######
 *  ##         ## ##   ## ###  ## ###      ##
 *  ##    #    ## ##   ## ##  ##  #####    ##
 *   ##   ##   ## ##   ## ## ##    #####   ##
 *   ###   ## ##  ##   ## ###   #     ##   ##
 *    ##########  ####### ####### ######   ##
 *      #### ##    ######   ####   ####    ##
 *            ##
 *             ##       CREATE: 2022-11-08
 *              #
 ================================================
        QuestLAB 交易系统
 ================================================
 */

#ifndef __QL_QSTTRADE_H__
#define __QL_QSTTRADE_H__

#include "../QstLibs/QstLibs.h"
#include "../Asylum/asylum.hpp"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstTrade"
    #define WIN_TITLE   "QstTrade"
    #define WIN_CLASS   "QstTradeCLSS"
    #define WIN_ICONF   "QstTrade.ini"
    #define WIN_XCONF   "QstTrade.xml"
#endif

/* 字体的参数 */
typedef struct
{
        sint_t  height;
        uint_t  weight;
        wide_t  name[LF_FACESIZE];

} sQTRD_font;

/* 文字的结构 */
typedef struct
{
        sint_t  x, y;
        ansi_t  str[256];

        /* 生成两位小数值 */
        void_t  num2 (double value)
        {
            sprintf(this->str, "%.2f", value);
        }

        /* 生成日期字符串 */
        void_t  date (const sDATETIME *dttm)
        {
            sprintf(this->str, "%04u/%02u/%02u", dttm->year,
                                    dttm->month, dttm->day);
        }

        /* 生成日期字符串 */
        void_t  date (const asy::kday_node *kday)
        {
            sDATETIME   dttm;

            kday->date(&dttm);
            this->date(&dttm);
        }
} sQTRD_text;

/*****************************************************************************/
/*                                 工具函数                                  */
/*****************************************************************************/

/* 公用的函数 */
CR_API void_t   qtrd_setup_min_max (asy::kday_draw *info, size_t leave,
                                size_t count, const double *data, size_t step);

CR_API void_t   qtrd_pixel_draw (const sIMAGE *dst, sint_t x, sint_t y,
                                 cpix_t color);

CR_API pixdraw_t    qtrd_line_width_func (sIMAGE *image, uint_t width);

CR_API void_t   qtrd_line_draw_plus (sIMAGE *image, const sRECT *rect,
                        const asy::kday_draw *info, size_t enter, uint_t count,
                            const double *data, size_t step, uint_t width,
                                        cpix_t color);

CR_API void_t   qtrd_line_draw_full (sIMAGE *image, const sRECT *rect,
                        const asy::kday_draw *info, size_t enter, uint_t count,
                            const double *data, size_t step, uint_t width,
                                        cpix_t color);

CR_API sint_t   qtrd_grid_h_draw (sIMAGE *image, const asy::kday_draw *info,
                                  double value, bool_t xfull, uint_t dash,
                                  uint_t width, cpix_t color);

CR_API sint_t   qtrd_grid_v_draw (sIMAGE *image, const sRECT *rect,
                                  size_t enter, size_t leave, size_t time,
                                  uint_t dash, uint_t width, cpix_t color);

CR_API void_t   qtrd_font_param (sQTRD_font *param, const wide_t *name,
                                 sint_t height = 0, uint_t weight = 0);

CR_API iFONT*   qtrd_font_get (const sQTRD_font *param);

CR_API iFONT*   qtrd_font_get2 (const wide_t *name, sint_t height = 0,
                                uint_t weight = 0);

CR_API void_t   qtrd_font_rel (iFONT *font);

CR_API void_t   qtrd_text_box (sRECT *rect, const ansi_t *text, iFONT *font,
                               uint_t cpage = CR_LOCAL);

CR_API void_t   qtrd_text_out (sIMAGE *image, const ansi_t *text, cpix_t color,
                               cpix_t bkcolor, const sRECT *rect, uint_t align,
                               iFONT *font, uint_t cpage = CR_LOCAL);

/*****************************************************************************/
/*                                 系统配置                                  */
/*****************************************************************************/

/* 绘图的参数 */
typedef struct
{
        /* 文字绘制参数 */
        sQTRD_font  text_sml;       /*  小字字体的参数  */
        sQTRD_font  text_mdl;       /*  中字字体的参数  */
        sQTRD_font  text_lrg;       /*  大字字体的参数  */

        /* 窗口绘制参数 */
        cpix_t  win_tran_color;     /* 透明的背景颜色值 */
        cpix_t  win_text_color;     /* 一般文字的颜色值 */
        uint_t  win_space_betw;     /* 窗口间的填充间隔 */

        /* 柱子绘制参数 */
        uint_t  bar_width_full;     /* 实体柱子的像素宽 */
        uint_t  bar_side_space;     /* 柱子间的像素间隔 */
        uint_t  bar_updn_wline;     /* 柱子上下细线的宽 */
        cpix_t  bar_color_rise;     /*  上涨柱子的颜色  */
        cpix_t  bar_color_drop;     /*  下跌柱子的颜色  */
        cpix_t  bar_color_eque;     /*  平盘柱子的颜色  */
        bool_t  bar_is_rhollow;     /* 上涨柱子是否挖空 */
        bool_t  bar_is_dhollow;     /* 下跌柱子是否挖空 */
        uint_t  bar_hollow_sub;     /* 挖空柱减去的像素 */

        /* 缺口绘制参数 */
        bool_t  gap_is_render;      /*  是否绘制跳空口  */
        cpix_t  gap_blk_color;      /*  跳空缺口的颜色  */

        /* 蜡烛网格参数 */
        uint_t  can_grid_h_count;   /* 蜡烛图水平网格数 */
        uint_t  can_grid_v_count;   /* 蜡烛图垂直网格数 */
        uint_t  can_up_dn_padded;   /* 蜡烛图上下填充值 */

        /* 线段绘制参数 */
        uint_t  line_width;             /* 统一线宽 */
        uint_t  line_grid_dash;         /* 网格间隔 */
        uint_t  line_grid_width;        /* 网格线宽 */
        cpix_t  line_grid_color;        /* 网格颜色 */

        /* 均线绘制参数 */
        uint_t  line_avg_count;         /* 均线数量 */
        uint_t  line_avg_atime[16];     /* 均线时间 */
        bool_t  line_avg_ashow[16];     /* 均线开关 */
        cpix_t  line_avg_color[16];     /* 均线颜色 */

        /* 价格线绘制参数 */
        cpix_t  line_price_color[4];    /* 线的颜色 */

        /* BBI 线绘制参数 */
        cpix_t  line_bbi_color;         /* BBI 颜色 */
        uint_t  line_bbi_level[4];      /* 四个级别 */

        /* BOLL 线绘制参数 */
        uint_t  line_boll_days;         /* BOLL 参数 */
        uint_t  line_boll_mult;         /* BOLL 参数 */
        cpix_t  line_boll_color[3];     /* BOLL 颜色 */

        /* 量能线绘制参数 */
        uint_t  line_amnt_level[2];     /* 量能均线级别 */
        cpix_t  line_amnt_color[2];     /* 量能均线颜色 */

        /* MACD 绘制参数 */
        uint_t  macd_days_fast;         /* 快线天数 */
        uint_t  macd_days_slow;         /* 慢线天数 */
        uint_t  macd_days_diff;         /* 差值天数 */
        cpix_t  macd_line_dif_color;    /* DIF 颜色 */
        cpix_t  macd_line_dea_color;    /* DEA 颜色 */

        /* KDJ 线绘制参数 */
        uint_t  kdj_days_r;             /* RSV 天数 */
        uint_t  kdj_days_k;             /* K 的天数 */
        uint_t  kdj_days_d;             /* D 的天数 */
        cpix_t  kdj_line_k_color;       /* K 的颜色 */
        cpix_t  kdj_line_d_color;       /* D 的颜色 */
        cpix_t  kdj_line_j_color;       /* J 的颜色 */
        bool_t  kdj_line_j_is_render;   /* J 显示否 */

        /* RSI 线绘制参数 */
        uint_t  rsi_line_level[3];      /* RSI 级别 */
        cpix_t  rsi_line_color[3];      /* RSI 颜色 */

        /* WR 线绘制参数 */
        uint_t  wr_line_level;          /* WR 级别 */
        cpix_t  wr_line_color;          /* WR 颜色 */

        /* CCI 线绘制参数 */
        uint_t  cci_line_level;         /* CCI 级别 */
        cpix_t  cci_line_color;         /* CCI 颜色 */

} sQTRD_conf;

/* 默认参数设置 */
CR_API void_t   qtrd_configure_reset (sQTRD_conf *cfg);

/* 加载配置文件 */
CR_API bool_t   qtrd_configure_load (sQTRD_conf *cfg, const ansi_t *file);

/* 应用配置参数 */
CR_API bool_t   qtrd_configure_apply (const sQTRD_conf *cfg);

/* 创建数据指标 */
CR_API void_t   qtrd_indicator_make (asy::kdays *dat, const sQTRD_conf *cfg);

/*****************************************************************************/
/*                                  蜡烛图                                   */
/*****************************************************************************/

/* 蜡烛图结果 */
typedef struct
{
        /* 基本信息 */
        size_t          index;      /* 选中的索引 */
        size_t          enter;      /* 开始的索引 */
        size_t          leave;      /* 结束的索引 */
        uint_t          count;      /* 最大绘制数 */
        sIMAGE*         image;      /* 绑定的图片 */
        asy::kdays*     kdays;      /* 绑定的Ｋ线 */
        asy::kday_draw  dinfo;      /* 绘图的信息 */
        asy::kday_data  vinfo;      /* 统计的信息 */

        /* 缺口信息 */
        uint_t          gpcnt;      /* 缺口的个数 */
        asy::kday_gap*  gpdat;      /* 缺口的数据 */

        /* 图素信息 */
        sRECT*          prlst;      /* 价格的图素 */
        sRECT*          hllst;      /* 高低的图素 */
        sRECT*          gplst;      /* 缺口的图素 */
        cpix_t*         clrst;      /* 颜色的列表 */
        int64u*         atlst;      /* 属性的列表 */

        /* 时间信息 */
        sDATETIME   enter_date;     /* 开始的日期 */
        sDATETIME   leave_date;     /* 结束的日期 */

        /* 配置信息 */
        const sQTRD_conf*   cfg;    /* 全局的配置 */

} sQTRD_candle;

/* 图素的属性 */
#define QTRD_CANDLE_HOLLOW      0x00000001UL    /* 是否挖空 */

/* 创建蜡烛图 */
CR_API bool_t   qtrd_candle_init (sIMAGE *img, sQTRD_candle *out,
                                  asy::kdays *dat, const sDATETIME *beg,
                                  const sQTRD_conf *cfg);
/* 释放蜡烛图 */
CR_API void_t   qtrd_candle_kill (sQTRD_candle *can);

/* 重建蜡烛图 */
CR_API void_t   qtrd_candle_rebuild (sQTRD_candle *can);

/* 重置蜡烛图 */
CR_API void_t   qtrd_candle_reset (sQTRD_candle *can);

/* 绘制蜡烛图 */
CR_API void_t   qtrd_candle_draw (const sQTRD_candle *can);

/* 价格的类型 */
#define QTRD_PRICE_TCLOSE   0   /* TCLOSE */
#define QTRD_PRICE_TOPEN    1   /* TOPEN */
#define QTRD_PRICE_HIGH     2   /* HIGH */
#define QTRD_PRICE_LOW      3   /* LOW */

/* 绘制蜡烛图价格线 */
CR_API void_t   qtrd_candle_draw_price (const sQTRD_candle *can, uint_t type);

/* 绘制蜡烛图 MA 线 */
CR_API void_t   qtrd_candle_set_ma (sQTRD_candle *can);
CR_API void_t   qtrd_candle_draw_ma (const sQTRD_candle *can);

/* 绘制蜡烛图 EMA 线 */
CR_API void_t   qtrd_candle_set_ema (sQTRD_candle *can);
CR_API void_t   qtrd_candle_draw_ema (const sQTRD_candle *can);

/* 绘制蜡烛图 BBI 线 */
CR_API void_t   qtrd_candle_set_bbi (sQTRD_candle *can);
CR_API void_t   qtrd_candle_draw_bbi (const sQTRD_candle *can);

/* 绘制蜡烛图 BOLL 线 */
CR_API void_t   qtrd_candle_set_boll (sQTRD_candle *can);
CR_API void_t   qtrd_candle_draw_boll (const sQTRD_candle *can);

/*****************************************************************************/
/*                                  量能图                                   */
/*****************************************************************************/

/* 量能图结果 */
typedef struct
{
        /* 基本信息 */
        sIMAGE*         imgvo;      /*  绑定的图片  */
        sIMAGE*         imgva;      /*  绑定的图片  */
        sIMAGE*         chtvo;      /*  图表的图片  */
        sIMAGE*         chtva;      /*  图表的图片  */
        sIMAGE          clnvo;      /*  图表的图片  */
        sIMAGE          clnva;      /*  图表的图片  */
        sQTRD_candle*   candl;      /* 绑定的蜡烛图 */
        double          vomax;      /* 最大的成交量 */
        double          vamax;      /* 最大的成交额 */
        asy::kday_draw  infvo;      /*  绘图的信息  */
        asy::kday_draw  infva;      /*  绘图的信息  */

        /* 图素信息 */
        sRECT*          volst;  /* 成交量的图素 */
        sRECT*          valst;  /* 成交额的图素 */
        cpix_t*         clrst;  /*  颜色的列表  */

} sQTRD_amount;

/* 创建量能图 */
CR_API bool_t   qtrd_amount_init (sIMAGE *ivo, sIMAGE *iva, sQTRD_amount *out,
                                  sQTRD_candle *can);
/* 释放量能图 */
CR_API void_t   qtrd_amount_kill (sQTRD_amount *mnt);

/* 重建量能图 */
CR_API void_t   qtrd_amount_rebuild (sQTRD_amount *mnt);

/* 重置量能图 */
CR_API void_t   qtrd_amount_reset (sQTRD_amount *mnt);

/* 绘制量能图 */
CR_API void_t   qtrd_amount_draw (const sQTRD_amount *mnt);

/* 绘制量能图 MA 线 */
CR_API void_t   qtrd_amount_set_ma (sQTRD_amount *mnt);
CR_API void_t   qtrd_amount_draw_ma (const sQTRD_amount *mnt);

/*****************************************************************************/
/*                                  指标图                                   */
/*****************************************************************************/

/* 指标图结果 */
typedef struct
{
        /* 基本信息 */
        sIMAGE*         image;      /*  绑定的图片  */
        sIMAGE*         chart;      /*  图表的图片  */
        sIMAGE          clone;      /*  图表的图片  */
        sQTRD_candle*   candl;      /* 绑定的蜡烛图 */
        asy::kday_draw  dinfo;      /*  绘图的信息  */
        bool_t          xfull;      /* 是否是对称轴 */

} sQTRD_indctr;

/* 指标的类型 */
#define QTRD_INDCTR_MACD    0   /* MACD */
#define QTRD_INDCTR_KDJ     1   /* KDJ */
#define QTRD_INDCTR_RSI     2   /* RSI */
#define QTRD_INDCTR_WR      3   /* WR */
#define QTRD_INDCTR_CCI     4   /* CCI */
#define QTRD_INDCTR_VMAX    5   /* 总数 */

/* 创建指标图 */
CR_API bool_t   qtrd_indctr_make (sIMAGE *img, sQTRD_indctr *out,
                                  sQTRD_candle *can);
/* 绘制 MACD 指标图 */
CR_API void_t   qtrd_indctr_set_macd (sQTRD_indctr *ind);
CR_API void_t   qtrd_indctr_draw_macd (const sQTRD_indctr *ind);

/* 绘制 KDJ 指标图 */
CR_API void_t   qtrd_indctr_set_kdj (sQTRD_indctr *ind);
CR_API void_t   qtrd_indctr_draw_kdj (const sQTRD_indctr *ind);

/* 绘制 RSI 指标图 */
CR_API void_t   qtrd_indctr_set_rsi (sQTRD_indctr *ind);
CR_API void_t   qtrd_indctr_draw_rsi (const sQTRD_indctr *ind);

/* 绘制 WR 指标图 */
CR_API void_t   qtrd_indctr_set_wr (sQTRD_indctr *ind);
CR_API void_t   qtrd_indctr_draw_wr (const sQTRD_indctr *ind);

/* 绘制 CCI 指标图 */
CR_API void_t   qtrd_indctr_set_cci (sQTRD_indctr *ind);
CR_API void_t   qtrd_indctr_draw_cci (const sQTRD_indctr *ind);

/*****************************************************************************/
/*                                 系统函数                                  */
/*****************************************************************************/

/* 系统初始化 */
CR_API bool_t   QstTrade_Init (uint_t can_width, uint_t can_height,
                               uint_t ind_height, const ansi_t *cfg_file);
/* 系统释放 */
CR_API void_t   QstTrade_Free (void_t);

/* 全局变量清空 */
CR_API void_t   QstTrade_Clean (void_t);

#endif  /* !__QL_QSTTRADE_H__ */
