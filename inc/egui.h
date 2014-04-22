/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-03-21  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 界面辅助头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_EGUI_H__
#define __CR_EGUI_H__

#include "fmtz.h"

/*****************************************************************************/
/*                                 资源加载                                  */
/*****************************************************************************/

/* 资源表类型 */
typedef void_t*     egui_t;

/* 建立资源表 */
CR_API egui_t       egui_res_init (leng_t count);

/* 释放资源表 */
CR_API void_t       egui_res_free (egui_t egui);

/* 加载资源文件 */
CR_API bool_t       egui_res_load_s (egui_t egui, const ansi_t *text,
                                     uint_t type, engine_init_t port);
CR_API bool_t       egui_res_load_f (egui_t egui, const ansi_t *name,
                                     uint_t type, engine_init_t port);
CR_API bool_t       egui_res_set_root (egui_t egui, const ansi_t *root);
CR_API void_t       egui_res_set_cpage (egui_t egui, uint_t cpage);

/* 查找资源对象 */
CR_API sFMTZ*       egui_res_get (egui_t egui, const ansi_t *name);
CR_API bool_t       egui_res_set (egui_t egui, const ansi_t *name,
                                  sFMTZ *resz);
CR_API void_t*      egui_res_get_dat (egui_t egui, const ansi_t *name,
                                      leng_t *size);
CR_API sIMAGE*      egui_res_get_img (egui_t egui, const ansi_t *name,
                                      int32u index, sRECT *rect);
CR_API sRECT*       egui_res_get_rct (egui_t egui, const ansi_t *name);
CR_API ansi_t*      egui_res_get_txt (egui_t egui, const ansi_t *name);
CR_API iPICTURE*    egui_res_get_anm (egui_t egui, const ansi_t *name);
CR_API iPACKAGE*    egui_res_get_pak (egui_t egui, const ansi_t *name);
CR_API iXMMEDIA*    egui_res_get_xmm (egui_t egui, const ansi_t *name);

/*****************************************************************************/
/*                                 绘制布局                                  */
/*****************************************************************************/

/* 对齐的掩码值 */
#define EGUI_ALN_MASKH  0x03    /* 水平对齐 */
#define EGUI_ALN_MASKV  0x0C    /* 垂直对齐 */

/* 元素对齐方式 */
#define EGUI_ALN_L  (1 << 0)    /* 水平左对齐 */
#define EGUI_ALN_R  (2 << 0)    /* 水平右对齐 */
#define EGUI_ALN_C  (3 << 0)    /* 水平中对齐 */
#define EGUI_ALN_T  (1 << 2)    /* 垂直顶对齐 */
#define EGUI_ALN_B  (2 << 2)    /* 垂直底对齐 */
#define EGUI_ALN_M  (3 << 2)    /* 垂直中对齐 */
#define EGUI_ALN_LT (EGUI_ALN_L | EGUI_ALN_T)
#define EGUI_ALN_LB (EGUI_ALN_L | EGUI_ALN_B)
#define EGUI_ALN_LM (EGUI_ALN_L | EGUI_ALN_M)
#define EGUI_ALN_RT (EGUI_ALN_R | EGUI_ALN_T)
#define EGUI_ALN_RB (EGUI_ALN_R | EGUI_ALN_B)
#define EGUI_ALN_RM (EGUI_ALN_R | EGUI_ALN_M)
#define EGUI_ALN_CT (EGUI_ALN_C | EGUI_ALN_T)
#define EGUI_ALN_CB (EGUI_ALN_C | EGUI_ALN_B)
#define EGUI_ALN_CM (EGUI_ALN_C | EGUI_ALN_M)

/* 区域的对齐计算 */
CR_API void_t   egui_rect_align (sRECT *dest, const sRECT *rect,
                                 uint_t type);
/* 文字抽象输出 (实体) */
CR_API bool_t   egui_draw_text (iFONT *font, const void_t *text,
                    const sRECT *rect, uint_t type CR_DEFAULT(EGUI_ALN_CM),
                            uint_t cpage CR_DEFAULT(CR_LOCAL));
/* 文字抽象输出 (透明) */
CR_API bool_t   egui_draw_tran (iFONT *font, const void_t *text,
                    const sRECT *rect, uint_t type CR_DEFAULT(EGUI_ALN_CM),
                            uint_t cpage CR_DEFAULT(CR_LOCAL));
/* 文字抽象输出 (实体) */
CR_API bool_t   egui_draw_text2 (iFONT *font, const void_t *text,
                    sint_t x, sint_t y, uint_t cpage CR_DEFAULT(CR_LOCAL));

/* 文字抽象输出 (透明) */
CR_API bool_t   egui_draw_tran2 (iFONT *font, const void_t *text,
                    sint_t x, sint_t y, uint_t cpage CR_DEFAULT(CR_LOCAL));
/* 进度条的方向 */
#define EGUI_PRO_L2R    0   /* 从左到右 (默认) */
#define EGUI_PRO_R2L    1   /* 从右到左 */
#define EGUI_PRO_T2B    2   /* 从上到下 */
#define EGUI_PRO_B2T    3   /* 从下到上 */

/* 进度条区域计算 (整数) */
CR_API void_t   egui_rect_probar_i (sRECT *dest, const sRECT *rect,
                        uint_t type, dist_t vmin, dist_t vmax, dist_t vnow);

/* 进度条区域计算 (浮点) */
CR_API void_t   egui_rect_probar_f (sRECT *dest, const sRECT *rect,
                        uint_t type, real_t vmin, real_t vmax, real_t vnow);

#endif  /* !__CR_EGUI_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
