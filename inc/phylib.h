/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-11-21  */
/*     #######          ###    ###      [MATH]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-05-20  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 数学物理头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_PHYLIB_H__
#define __CR_PHYLIB_H__ 0x73F4057FUL

#include "gfx2.h"

/*****************************************************************************/
/*                                 重要常数                                  */
/*****************************************************************************/

#define CR_PHY_INV      (-65536.0) /* 非法的计算结果 */
#define CR_PHY_E        (2.718281828459045235360287471352662497757) /* e */
#define CR_PHY_LN2      (0.69314718055994530941723212145818) /* ln(2) */
#define CR_PHY_LN10     (2.3025850929940456840179914546844) /* ln(10) */
#define CR_PHY_SQR2     (1.4142135623730950488016887242097) /* sqrt(2) */
#define CR_PHY_PI       (3.1415926535897932384626433832795) /* 圆周率 */
#define CR_PHY_DTOR     (CR_PHY_PI / 180.0) /* 角度转弧度 */
#define CR_PHY_OSAP     (101325.0) /* 标准大气压 (Pa) */
#define CR_PHY_UGCR     (8.31441) /* 理想气体常数 (J / (mol * K)) */
#define CR_PHY_WUGCR    (0.461526) /* 水的理想气体常数 (kJ / (kg * K)) */
#define CR_PHY_ABSZ     (273.15) /* 绝对零度相关的常数 (K) */
#define CR_PHY_CAL2J    (4.187) /* 卡路里转焦耳的常数 */

/*****************************************************************************/
/*                                  热电阻                                   */
/*****************************************************************************/

CR_API double   cu50_t2r  (double t);
CR_API double   cu50_r2t  (double r);
CR_API double   pt100_t2r (double t);
CR_API double   pt100_r2t (double r);

/*****************************************************************************/
/*                                  热电偶                                   */
/*****************************************************************************/

CR_API double   type_b_mv2t (double mv);
CR_API double   type_b_t2mv (double t);
CR_API double   type_e_mv2t (double mv);
CR_API double   type_e_t2mv (double t);
CR_API double   type_j_mv2t (double mv);
CR_API double   type_j_t2mv (double t);
CR_API double   type_k_mv2t (double mv);
CR_API double   type_k_t2mv (double t);
CR_API double   type_n_mv2t (double mv);
CR_API double   type_n_t2mv (double t);
CR_API double   type_r_mv2t (double mv);
CR_API double   type_r_t2mv (double t);
CR_API double   type_s_mv2t (double mv);
CR_API double   type_s_t2mv (double t);
CR_API double   type_t_mv2t (double mv);
CR_API double   type_t_t2mv (double t);

/*****************************************************************************/
/*                                  水蒸汽                                   */
/*****************************************************************************/

CR_API double   ifc67_ws_d (double t, double mpa);
CR_API double   iif97_1_ws_d (double t, double mpa);
CR_API double   iif97_1_ws_e (double t, double mpa);
CR_API double   iif97_2_ws_d (double t, double mpa);
CR_API double   iif97_2_ws_e (double t, double mpa);
CR_API double   iif97_2_3_t2mpa (double t);
CR_API double   iif97_2_3_mpa2t (double mpa);
CR_API double   iif97_3_ws_p (double t, double cp);
CR_API double   iif97_3_ws_e (double t, double cp);
CR_API double   iif97_3_ws_d (double t, double mpa, double wx);
CR_API double   iif97_4_t2mpa (double t);
CR_API double   iif97_4_mpa2t (double mpa);
CR_API double   iif97_5_ws_d (double t, double mpa);
CR_API double   iif97_5_ws_e (double t, double mpa);
CR_API double   iif97_ws_de (double *e, double t, double mpa, double wx,
                             ufast_t *sec CR_DEFAULT(NULL));

/*****************************************************************************/
/*                                   颜色                                    */
/*****************************************************************************/

/* 用到的类型 */
typedef void_t*     cstep_t;

/* RGB 输入一律使用32色的小端模式 */
CR_API void_t   bgr2hsl (sint_t hsl[3], const byte_t bgr[3]);
CR_API void_t   bgr2hsv (sint_t hsv[3], const byte_t bgr[3]);

/* 颜色分区判断 */
CR_API cstep_t  color_step_init (void_t);
CR_API void_t   color_step_kill (cstep_t cstep);
CR_API void_t   color_step_set (cstep_t cstep, const sint_t steps[12],
                                const byte_t color[36]);
CR_API void_t   color_step_bias (cstep_t cstep, sint_t bias);
CR_API byte_t   color_step_do (cstep_t cstep, byte_t dst[3], sint_t hue);

/*
    颜色索引值说明 (颜色滤波器用)
****************************************************************
    0 - 无效    1 - 黑色    2 - 白色    3 - 红色    4 - 橙色
    5 - 黄色    6 - 黄绿    7 - 绿色    8 - 青绿    9 - 青色
    A - 青蓝    B - 蓝色    C - 蓝紫    D - 紫色    E - 紫红
****************************************************************
*/
CR_API const byte_t _rom_ g_cstep_pal[64];

/*****************************************************************************/
/*                                   图像                                    */
/*****************************************************************************/

/* 抓图和回图 */
CR_API sIMAGE*  image_grab (const sIMAGE *img, const sRECT *box);
CR_API void_t   image_back (const sIMAGE *dst, const sIMAGE *src,
                            sint_t left, sint_t top);
/* 直方图阈值计算 */
CR_API byte_t   histo_avge (const leng_t tab[256]);
CR_API byte_t   histo_otsu (const leng_t tab[256]);

/* 转换到灰度图 */
CR_API sIMAGE*  image_graying (const sIMAGE *img);

/* 转换到索引图 */
typedef byte_t  (*idx_bgr_t) (void_t*, const byte_t*);

CR_API sIMAGE*  image_indexed (const sIMAGE *img, idx_bgr_t dopix,
                         const byte_t *pal, leng_t num, void_t *param);
/* 灰度直方图计算 */
CR_API bool_t   image_histo (leng_t tab[256], const sIMAGE *gray);

/* 灰度图二值化 */
CR_API bool_t   image_binary1 (const sIMAGE *gray, byte_t gate);
CR_API bool_t   image_binary2 (const sIMAGE *gray, const sIMAGE *gate,
                               sint_t offset);
/* 卷积运算矩阵结构 */
typedef struct
{
        sint_t          kk;     /* K 系数 */
        uint_t          ww;     /* 矩阵宽 (奇数) */
        uint_t          hh;     /* 矩阵高 (奇数) */
        const sint_t*   dt;     /* 指向矩阵数据 */

} sCONVO_MAT;

/* 图像卷积运算 */
CR_API sIMAGE*  image_convo (const sIMAGE *img, const sCONVO_MAT *mat);

/* 形态运算矩阵结构 */
typedef struct
{
        uint_t          ww;     /* 矩阵宽 (奇数) */
        uint_t          hh;     /* 矩阵高 (奇数) */
        const byte_t*   dt;     /* 指向矩阵数据 */

} sSHAPE_MAT;

/* 图像形态运算 */
CR_API sIMAGE*  image_shape (const sIMAGE *img, const sSHAPE_MAT *mat,
                             bool_t expand);
/* 形态查找匹配 */
CR_API bool_t   shape_match_and (const byte_t *left_top, leng_t img_bpl,
                                 const sSHAPE_MAT *shape_mat);
CR_API bool_t   shape_match_orr (const byte_t *left_top, leng_t img_bpl,
                                 const sSHAPE_MAT *shape_mat);
CR_API bool_t   shape_match_cnt (const byte_t *left_top, leng_t img_bpl,
                                 const sSHAPE_MAT *shape_mat, uint_t gate);

/*****************************************************************************/
/*                                   几何                                    */
/*****************************************************************************/

/* 矩形过滤参数结构 */
typedef struct
{
        fp32_t  merge;          /* 合并面积阈 */
        fp32_t  min_s, max_s;   /* 极限宽高比 */
        uint_t  min_w, min_h;   /* 最小的宽高 */
        uint_t  max_w, max_h;   /* 最大的宽高 */

} sRECT_FILTER;

/* 矩形过滤器 (结果中宽为0的矩形表示此矩形已删除) */
CR_API leng_t   rect_filter_lt_rb (sRECT *result,
                             const sPNT2 *pnt_lt, leng_t cnt_lt,
                             const sPNT2 *pnt_rb, leng_t cnt_rb,
                                const sRECT_FILTER *param);
CR_API leng_t   rect_filter_lb_rt (sRECT *result,
                             const sPNT2 *pnt_lb, leng_t cnt_lb,
                             const sPNT2 *pnt_rt, leng_t cnt_rt,
                                const sRECT_FILTER *param);
/* 矩形合并 (已包含在过滤器里) */
CR_API void_t   rect_merge (sRECT *result, leng_t count,
                            const sRECT_FILTER* param);
/* 取点密度最大的矩形 */
CR_API bool_t   rect_max_density (sRECT *result,
                            const sRECT *list, leng_t count,
                            const sPNT2 *pnts1, leng_t cnts1,
                            const sPNT2 *pnts2, leng_t cnts2);

#endif  /* !__CR_PHYLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
