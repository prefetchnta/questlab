/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-11-21  */
/*     #######          ###    ###      [MATH]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 数学物理头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_PHYLIB_H__
#define __CR_PHYLIB_H__

#include "gfx2.h"
#include "datlib.h"

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
#define CR_PHY_RTOD     (180.0 / CR_PHY_PI) /* 弧度转角度 */
#define CR_PHY_OSAP     (101325.0) /* 标准大气压 (Pa) */
#define CR_PHY_UGCR     (8.31441) /* 理想气体常数 (J / (mol * K)) */
#define CR_PHY_WUGCR    (0.461526) /* 水的理想气体常数 (kJ / (kg * K)) */
#define CR_PHY_ABSZ     (273.15) /* 绝对零度相关的常数 (K) */
#define CR_PHY_CAL2J    (4.187) /* 卡路里转焦耳的常数 */
#define CR_PHY_GOLD1    (0.6180339887499) /* 黄金分割常数1 */
#define CR_PHY_GOLD2    (1.6180339887499) /* 黄金分割常数2 */

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
/*                                   磁场                                    */
/*****************************************************************************/

/* 磁场参数结构 */
typedef struct
{
        fp32_t  X;      /* 北向强度 */
        fp32_t  Y;      /* 东向强度 */
        fp32_t  Z;      /* 垂直强度 */
        fp32_t  H;      /* 水平强度 */
        fp32_t  F;      /* 总 强 度 */
        fp32_t  D;      /* 磁 偏 角 */
        fp32_t  I;      /* 磁 倾 角 */
        fp32_t  V[3];   /* 归一化向量 */

} sMAGGEO;

CR_API bool_t   maggeo_param (sMAGGEO *mag, sint_t x, sint_t z, sint_t y);
CR_API fp32_t   maggeo_angle (const sMAGGEO *mag1, const sMAGGEO *mag2);

/*****************************************************************************/
/*                                   颜色                                    */
/*****************************************************************************/

/* 用到的类型 */
typedef void_t*     cstep_t;

/* RGB 输入一律使用32色的小端模式 */
CR_API void_t   bgr2hsl (sint_t hsl[3], const byte_t bgr[3]);
CR_API void_t   bgr2hsv (sint_t hsv[3], const byte_t bgr[3]);
CR_API void_t   bgr2yuv (byte_t yuv[3], const byte_t bgr[3]);

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
CR_API void_t   color_step_pal (int32u *pal, uint_t count);

/*****************************************************************************/
/*                                   图像                                    */
/*****************************************************************************/

/* 抓图和回图 */
CR_API sIMAGE*  image_grab (const sIMAGE *img, const sRECT *box);
CR_API sIMAGE*  image_rotz (const sIMAGE *img, const sRECT *box,
                            fp32_t ccw, fp32_t scale, bool_t lerp);
CR_API void_t   image_back (const sIMAGE *dst, const sIMAGE *src,
                            sint_t left, sint_t top);
/* 扩展/收缩图片边框 */
CR_API sIMAGE*  image_bound (const sIMAGE *img, uint_t xsize, uint_t ysize);
CR_API bool_t   image_unbound (const sIMAGE *dst, const sIMAGE *src,
                               uint_t xsize, uint_t ysize);
/* 图片自定义运算 */
typedef sint_t  (*pix_ops_t) (byte_t, byte_t);

CR_API bool_t   image_oper (const sIMAGE *dst, const sIMAGE *src,
                            pix_ops_t dopix);
/* 直方图阈值计算 */
CR_API byte_t   histo_avge (const leng_t tab[256]);
CR_API byte_t   histo_otsu (const leng_t tab[256]);

/* 转换到特殊图 */
CR_API bool_t   image_to_hsl (const sIMAGE *img);
CR_API bool_t   image_to_hsv (const sIMAGE *img);
CR_API bool_t   image_to_yuv (const sIMAGE *img);

/* 转换到灰度图 */
CR_API sIMAGE*  image_graying (const sIMAGE *img);

/* 转换到索引图 */
typedef byte_t  (*idx_bgr_t) (void_t*, uint_t, uint_t, const byte_t*);

CR_API sIMAGE*  image_indexed (const sIMAGE *img, idx_bgr_t dopix,
                         const byte_t *pal, leng_t num, void_t *param);
/* 灰度直方图计算 */
CR_API bool_t   image_histo (leng_t tab[256], const sIMAGE *gray);

/* 彩色直方图计算 */
CR_API bool_t   image_histo3 (leng_t t_r[256], leng_t t_g[256],
                              leng_t t_b[256], const sIMAGE *img);
/* 灰度图二值化 */
CR_API bool_t   image_binary0 (const sIMAGE *gray, bool_t is_he);
CR_API bool_t   image_binary1 (const sIMAGE *gray, byte_t gate);
CR_API bool_t   image_binary2 (const sIMAGE *gray, const sIMAGE *gate,
                               sint_t offset);
/* 灰度查表变换 */
CR_API bool_t   image_lookup (const sIMAGE *gray, const byte_t tab[256]);

/* 彩色查表变换 */
CR_API bool_t   image_lookup3 (const sIMAGE *img, const byte_t t_r[256],
                               const byte_t t_g[256], const byte_t t_b[256]);
/* 颜色分量变换 */
CR_API void_t   dot_gamma (byte_t tab[256], fp32_t gamma, fp32_t comp);
CR_API void_t   dot_muladd (byte_t tab[256], fp32_t fmul, fp32_t fadd);
CR_API void_t   dot_contrast (byte_t tab[256], fp32_t contrast);
CR_API void_t   dot_solarize (byte_t tab[256], sint_t light);
CR_API void_t   dot_histo_ave (byte_t tab[256], const leng_t hist[256],
                               uint_t width, uint_t height);
/* 图片像素变换 */
CR_API bool_t   pic_white_bl (const sIMAGE *img, sint_t light);
CR_API bool_t   pic_saturation (const sIMAGE *img, const sIMAGE *gray,
                                fp32_t fsat);
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

/* 图像形态运算 (矩阵忽略点值为 0x80 只支持二值图像) */
CR_API sIMAGE*  image_shape (const sIMAGE *img, const sSHAPE_MAT *mat,
                             bool_t expand);
/* 自定义形态运算 */
#define SHAPE_OP_NOP    0   /* 忽略 */
#define SHAPE_OP_DEL    1   /* 删除 */
#define SHAPE_OP_SET    2   /* 设置 */
typedef uint_t  (*shape_core_t) (void_t*, const byte_t*, leng_t);

CR_API sIMAGE*  image_shape_ex (const sIMAGE *img, shape_core_t score,
                                uint_t mat_w, uint_t mat_h, void_t *param);
/* 开闭形态运算 */
CR_API sIMAGE*  shape_open  (const sIMAGE *img, const sSHAPE_MAT *rotz,
                             uint_t rot_times, const sSHAPE_MAT *expn,
                             uint_t exp_times);
CR_API sIMAGE*  shape_close (const sIMAGE *img, const sSHAPE_MAT *expn,
                             uint_t exp_times, const sSHAPE_MAT *rotz,
                             uint_t rot_times);
/* 形态杂点清除 */
#define SHAPE_CL_XX     0   /* X--轴 */
#define SHAPE_CL_YY     1   /* Y--轴 */
#define SHAPE_CL_XY     2   /* XY-轴 */
CR_API sIMAGE*  shape_clean (const sIMAGE *img, uint_t type, uint_t times);

/* 形态查找匹配 (矩阵忽略点值为 0x80 可以支持索引图像) */
CR_API bool_t   shape_match_and (const byte_t *left_top, leng_t img_bpl,
                                 const sSHAPE_MAT *shape_mat);
CR_API bool_t   shape_match_orr (const byte_t *left_top, leng_t img_bpl,
                                 const sSHAPE_MAT *shape_mat);
CR_API bool_t   shape_match_cnt (const byte_t *left_top, leng_t img_bpl,
                                 const sSHAPE_MAT *shape_mat, uint_t gate);
CR_API bool_t   shape_match_wet (const byte_t *left_top, leng_t img_bpl,
                    const sSHAPE_MAT *shape_mat, const sCONVO_MAT *weight);

/*****************************************************************************/
/*                                   几何                                    */
/*****************************************************************************/

/* 线段方向倒转 */
CR_API void_t   line_swap (sPNT2 *pnts, leng_t count);

/* 线段点集压缩 */
CR_API leng_t   line_compress (sPNT2 *pnts, leng_t count);

/* Hough 变换结果结构 */
typedef struct
{
        uint_t  asize;  /* 变换图大小 */
        uint_t  n_rho;  /* 变换图宽度 */
        uint_t  n_ang;  /* 变换图高度 */
        uint_t  xmove;  /* 平移用参数 */
        uint_t* accum;  /* 变换图 (需要释放) */

} sLINE_HOUGH;

/* 标准直线 Hough 变换 */
CR_API bool_t   line_hough (sLINE_HOUGH *hough, const sIMAGE *img,
                            fp32_t rho, fp32_t theta, uint_t gate,
                            byte_t index);
/* 获取直线极坐标参数 */
CR_API bool_t   line_hough_get (uint_t idx, fp32_t *rho, fp32_t *theta,
                                const sLINE_HOUGH *hough);
/* 获取最长直线索引值 */
CR_API uint_t   line_hough_max (const sLINE_HOUGH *hough);

/* 释放 Hough 变换结果 */
CR_API void_t   line_hough_free (sLINE_HOUGH *hough);

/* 矩形过滤参数结构 */
typedef struct
{
        fp32_t  merge;          /* 合并面积阈 */
        fp32_t  min_s, max_s;   /* 极限宽高比 */
        uint_t  min_w, min_h;   /* 最小的宽高 */
        uint_t  max_w, max_h;   /* 最大的宽高 */

} sRECT_FILTER;

/* 矩形整理 */
CR_API leng_t   rect_clean (sRECT *result, leng_t count);

/* 矩形搜索 */
CR_API leng_t   rect_get_lt_rb (sRECT *result, leng_t limit,
                            const sPNT2 *pnt_lt, leng_t cnt_lt,
                            const sPNT2 *pnt_rb, leng_t cnt_rb,
                                const sRECT_FILTER *param);
CR_API leng_t   rect_get_lb_rt (sRECT *result, leng_t limit,
                            const sPNT2 *pnt_lb, leng_t cnt_lb,
                            const sPNT2 *pnt_rt, leng_t cnt_rt,
                                const sRECT_FILTER *param);
/* 矩形合并 */
CR_API leng_t   rect_merge (sRECT *result, leng_t count,
                            const sRECT_FILTER* param);
/* 取点密度最大矩形 */
CR_API bool_t   rect_max_density (sRECT *result, const sRECT *list,
                                  leng_t count, const sPNT2 *pnts1,
                                  leng_t cnts1, const sPNT2 *pnts2,
                                  leng_t cnts2);
/* 计算点集的重心 */
CR_API leng_t   rect_get_focus (sPNT2 *pnt, const sIMAGE *img,
                                const sRECT *roi, bool_t gray);
/* 连通区域标识与提取 */
CR_API byte_t   rect_area_marks (const sIMAGE *img);
CR_API bool_t   rect_area_bound (sRECT *bound, const sIMAGE *img,
                                 byte_t type);

/*****************************************************************************/
/*                                   纹理                                    */
/*****************************************************************************/

/* 像素投影直方图 */
CR_API bool_t   tex_project_x (uint_t *prj, const sIMAGE *img, byte_t idx);
CR_API bool_t   tex_project_y (uint_t *prj, const sIMAGE *img, byte_t idx);

/* 交替统计参数结果结构 */
typedef struct
{
        /* 输入参数 */
        sint_t  step;           /* 像素的间隔 */
        bool_t  altr;           /* 是否交替统计 */
        uint_t  gmin, gmax;     /* 交替的阈值 */
        byte_t  idx1, idx2;     /* 形态边界值组
                                   进入形态 [idx1, idx2]
                                   退出形态 [idx2, idx1] */
        const sRECT*    win;    /* 局部位置 */
        const sIMAGE*   img;    /* 输入图片 */

        /* 返回结果 */
        byte_t* result;     /* 结果状态列表 (需要释放) */
        uint_t  nhit, ntot;     /* 符合数和总数 */

} sTEX_PATTERN;

/* 像素交替统计 */
CR_API bool_t   tex_altern_x  (sTEX_PATTERN *patt);
CR_API bool_t   tex_altern_y  (sTEX_PATTERN *patt);
CR_API void_t   tex_patt_free (sTEX_PATTERN *patt);

/* 纹理网格压缩 */
CR_API sIMAGE*  tex_compress (const sIMAGE *img, uint_t tile_w,
                              uint_t tile_h, byte_t gate);

/*****************************************************************************/
/*                                   空间                                    */
/*****************************************************************************/

/* 点的空间划分 */
CR_API uint_t   space_divide (byte_t *bits, double min, double max,
                              double value, uint_t times);
CR_API uint_t   space_genbit (byte_t *bits, uint_t size, byte_t **list,
                              uint_t count);
/* 空间坐标哈希 */
CR_API ansi_t*  space_hash2d (double min_x, double max_x,
                              double min_y, double max_y,
                              double x, double y, uint_t times);
CR_API ansi_t*  space_hash3d (double min_x, double max_x,
                              double min_y, double max_y,
                              double min_z, double max_z,
                              double x, double y, double z, uint_t times);

/*****************************************************************************/
/*                                   寻路                                    */
/*****************************************************************************/

/*********************/
/* AStar pathfinding */
/*********************/
#define ASNL_ADDOPEN        0
#define ASNL_STARTOPEN      1
#define ASNL_DELETEOPEN     2
#define ASNL_ADDCLOSED      3
/* ----------------------- */
#define ASNC_INITIALADD     0
#define ASNC_OPENADD_UP     1
#define ASNC_OPENADD        2
#define ASNC_CLOSEDADD_UP   3
#define ASNC_CLOSEDADD      4
#define ASNC_NEWADD         5
/* ----------------------- */

/* 路径节点 */
typedef struct  _asNode
{
        sint_t  f, g, h, x, y;
        sint_t  number, numchildren;
        struct _asNode* parent;
        struct _asNode* next;
        struct _asNode* children[8];

} asNode;

/* 节点栈对象 */
typedef struct  _asStack
{
        asNode*             data;
        struct _asStack*    next;

} asStack;

/* 用到的回调类型 */
typedef sint_t  (*asFunc) (asNode*, asNode*, sint_t, void_t*);

/* AStar 寻路对象 */
typedef struct
{
        /* 公用成员 */
        asFunc  udCost;         /* 代价计算回调 */
        asFunc  udValid;        /* 坐标有效性检查回调 */
        asFunc  udHeuristic;    /* 预估值计算回调 */
        asFunc  udNotifyChild;  /* 子节点增加/检查时调用 */
        asFunc  udNotifyList;   /* 节点加入 Open/Closed 列表时调用 */
        void_t* cbData;         /* 回调函数的用户数据 */
        void_t* ncData;         /* 回调函数的用户数据 */

        /* 私有成员 */
        sint_t      iSX, iSY, iDX, iDY, iCols, iRows, iDNum;
        asNode*     pOpen;      /* Open 列表 */
        asNode*     pClosed;    /* Closed 列表 */
        asNode*     pBest;      /* 最佳节点 */
        asStack*    pStack;     /* 用到的栈对象 */

} sASTAR;

CR_API void_t   astar_init (sASTAR *as, sint_t cols, sint_t rows);
CR_API void_t   astar_free (sASTAR *as);
CR_API void_t   astar_reset (sASTAR *as);
CR_API bool_t   astar_step_init (sASTAR *as, sint_t sx, sint_t sy,
                                             sint_t dx, sint_t dy);
CR_API sint_t   astar_step_next (sASTAR *as);
CR_API bool_t   astar_find_path (sASTAR *as, sint_t sx, sint_t sy,
                                             sint_t dx, sint_t dy);
CR_API sPNT2*   astar_get_path (const sASTAR *as, leng_t *count);

/************************/
/* Dijkstra pathfinding */
/************************/
#define DJNL_ADDOPEN        0
#define DJNL_STARTOPEN      1
#define DJNL_DELETEOPEN     2
#define DJNL_ADDCLOSED      3
/* ----------------------- */
#define DJNC_INITIALADD     0
#define DJNC_OPENADD_UP     1
#define DJNC_OPENADD        2
#define DJNC_CLOSEDADD_UP   3
#define DJNC_CLOSEDADD      4
#define DJNC_NEWADD         5
/* ----------------------- */

/* 路径节点 */
typedef struct  _djNode
{
        sARRAY  children;
        sint_t  g, number;
        sint_t  numchildren;
        struct _djNode* parent;
        struct _djNode* next;

} djNode;

/* 节点栈对象 */
typedef struct  _djStack
{
        djNode*             data;
        struct _djStack*    next;

} djStack;

/* 用到的回调类型 */
typedef sint_t  (*djFunc) (djNode*, djNode*, sint_t, void_t*);

/* Dijkstra 寻路对象 */
typedef struct
{
        /* 公用成员 */
        djFunc  udCost;         /* 代价计算回调 */
        djFunc  udValid;        /* 节点有效性检查回调 */
        djFunc  udNotifyChild;  /* 子节点增加/检查时调用 */
        djFunc  udNotifyList;   /* 节点加入 Open/Closed 列表时调用 */
        void_t* cbData;         /* 回调函数的用户数据 */
        void_t* ncData;         /* 回调函数的用户数据 */

        /* 私有成员 */
        sint_t      iDNum;
        djNode*     pOpen;      /* Open 列表 */
        djNode*     pClosed;    /* Closed 列表 */
        djNode*     pBest;      /* 最佳节点 */
        djStack*    pStack;     /* 用到的栈对象 */

} sDIJKSTRA;

CR_API void_t   dijkstra_init (sDIJKSTRA *dj);
CR_API void_t   dijkstra_free (sDIJKSTRA *dj);
CR_API void_t   dijkstra_reset (sDIJKSTRA *dj);
CR_API bool_t   dijkstra_step_init (sDIJKSTRA *dj, sint_t beg, sint_t end);
CR_API sint_t   dijkstra_step_next (sDIJKSTRA *dj);
CR_API bool_t   dijkstra_find_path (sDIJKSTRA *dj, sint_t beg, sint_t end);
CR_API sint_t*  dijkstra_get_path (const sDIJKSTRA *dj, leng_t *count);

/*****************************************************************************/
/*                                   医学                                    */
/*****************************************************************************/

/* HCG 指标 */
CR_API void_t   hcg_min_max (double base, sint_t days,
                             double *vmin, double *vmax);

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
