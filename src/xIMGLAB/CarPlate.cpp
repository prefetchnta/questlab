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
 *             ##       CREATE: 2013-04-07
 *              #
 ================================================
        车牌识别图像实验插件【废弃】
 ================================================
 */

/* 取消建立 DLL 定义 */
#define _CR_USE_DLL_
#undef  _CR_BUILD_DLL_
#include "xIMGLAB.h"
#include "util/egui.h"
#include "../QstLibs/QstLibs.h"
#include "../TessOCR/TessOCR.h"
#define _CR_BUILD_DLL_

#include <math.h>

/* 外部库引用 */
#pragma comment (lib, "TessOCR.lib")
#pragma comment (lib, "CrH_GFX2.lib")
#pragma comment (lib, "CrH_MATH.lib")

/* 调试配置开关 */
#define OUTPUT_PRJ_X    0
#define STEP_BY_STEP    0

/*
=======================================
    插件 DLL 入口点
=======================================
*/
BOOL WINAPI
DllMain (
  __CR_IN__ HANDLE  hinst,
  __CR_IN__ DWORD   reason,
  __CR_UU__ LPVOID  reserved
    )
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}

/****************/
/* 矩形过滤参数 */
/****************/
#define EXPAND_IN   8
#define EXPAND_OT   16
#define MAX_POINT   512
#define BOX_MIN_S   2.0f
#define BOX_MID_S   3.0f
#define BOX_MAX_S   5.0f
#define BOX_MERGE   0.4f
#define BOX_SCALE   3.14f
#define BOX_MAX_W   (win_w / 5)
#define BOX_MAX_H   (win_h / 8)
#define BOX_MIN_W   (win_w / 8)
#define BOX_MIN_H   (win_h / 20)

/****************/
/* 颜色区分参数 */
/****************/
static cpix_t   s_clbox;
static cpix_t   s_cllne;
static cstep_t  s_cstep;
static byte_t   s_cstep_pal[60];
#define PIX_NONE   0x80 /* 忽略颜色 */
#define PIX_FONT   0x02 /* 字体颜色 */
#define PIX_BACK   0x0B /* 底版颜色 */
#define PIX_MIN_SAT  32 /* 饱和分割 */
#define PIX_MIN_LIT  20 /* 视为黑色 */
#define PIX_MAX_LIT 128 /* 视为白色 */

/****************/
/* 卷积矩阵参数 */
/****************/
static const sint_t s_blur[9] = {
                                    1, 2, 1,
                                    2, 4, 2,
                                    1, 2, 1,
                                };
static const sint_t s_edge[9] = {
                                    -1, -1, -1,
                                    -1,  8, -1,
                                    -1, -1, -1,
                                };
static const sCONVO_MAT s_mat_blur = { 16, 3, 3, s_blur };
static const sCONVO_MAT s_mat_edge = {  1, 3, 3, s_edge };

/****************/
/* 形态矩阵参数 */
/****************/
#define MAT_WW  3
#define MAT_HH  3
#define MAT_W2  1
#define MAT_H2  1
static const byte_t s_comp[9] = {
                                    0xFF, 0xFF, 0xFF,
                                    0xFF, 0xFF, 0xFF,
                                    0xFF, 0xFF, 0xFF,
                                };
static const byte_t s_rotz[9] = {
                                    0x80, 0x80, 0x80,
                                    0xFF, 0xFF, 0xFF,
                                    0x80, 0x80, 0x80,
                                };
static const byte_t s_splt[9] = {
                                    PIX_BACK, PIX_BACK, PIX_NONE,
                                    PIX_BACK, PIX_NONE, PIX_FONT,
                                    PIX_NONE, PIX_FONT, PIX_FONT,
                                };
static const byte_t s_sprb[9] = {
                                    PIX_FONT, PIX_FONT, PIX_NONE,
                                    PIX_FONT, PIX_NONE, PIX_BACK,
                                    PIX_NONE, PIX_BACK, PIX_BACK,
                                };
static const byte_t s_splb[9] = {
                                    PIX_NONE, PIX_FONT, PIX_FONT,
                                    PIX_BACK, PIX_NONE, PIX_FONT,
                                    PIX_BACK, PIX_BACK, PIX_NONE,
                                };
static const byte_t s_sprt[9] = {
                                    PIX_NONE, PIX_BACK, PIX_BACK,
                                    PIX_FONT, PIX_NONE, PIX_BACK,
                                    PIX_FONT, PIX_FONT, PIX_NONE,
                                };
static const sSHAPE_MAT s_mat_comp = { 3, 3, s_comp };
static const sSHAPE_MAT s_mat_rotz = { 3, 3, s_rotz };
static const sSHAPE_MAT s_mat_splt = { 3, 3, s_splt };
static const sSHAPE_MAT s_mat_sprb = { 3, 3, s_sprb };
static const sSHAPE_MAT s_mat_splb = { 3, 3, s_splb };
static const sSHAPE_MAT s_mat_sprt = { 3, 3, s_sprt };

/****************/
/* OCR 识别相关 */
/****************/
#define FNT_MOV         2
#define SCN_PPI         72
static tessocr_t    s_tess_al;
static tessocr_t    s_tess_an;
static ansi_t   s_ang[16], s_txt[16];

/*
---------------------------------------
    特征颜色识别
---------------------------------------
*/
static byte_t
do_pixel (
  __CR_IN__ void_t*         ctx,
  __CR_IN__ uint_t          px,
  __CR_IN__ uint_t          py,
  __CR_IN__ const byte_t*   bgr
    )
{
    byte_t  clr;
    sint_t  hsl[3];

    CR_NOUSE(px);
    CR_NOUSE(py);
    bgr2hsl(hsl, bgr);
    if (hsl[2] < PIX_MIN_LIT)
        return (0x01);
    if (hsl[2] > PIX_MAX_LIT)
        return (0x02);
    if (hsl[1] < PIX_MIN_SAT)
        return (0x02);
    clr = color_step_do((cstep_t)ctx, NULL, hsl[0]);
    if (clr >= 0x0A && clr <= 0x0C)
        return (0x0B);
    return (clr);
}

/*
---------------------------------------
    绘制粗方框
---------------------------------------
*/
static void_t
draw_box3 (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sRECT*    box,
  __CR_IN__ cpix_t          color
    )
{
    sRECT   draw;
    uint_t  bound = 3;

    struct_cpy(&draw, box, sRECT);
    for (; bound != 0; bound--) {
        draw.x1 -= 1;   draw.y1 -= 1;
        draw.x2 += 1;   draw.y2 += 1;
        draw.ww += 2;   draw.hh += 2;
        draw_rect(dst, &draw, color, pixel_set32z);
    }
}

/*
---------------------------------------
    特征矩形搜索
---------------------------------------
*/
static bool_t
find_rect (
  __CR_OT__ sRECT*              result,
  __CR_OT__ bool_t*             found,
  __CR_OT__ sIMAGE**            img_pnts,
  __CR_IN__ const sIMAGE*       img_idxz,
  __CR_IN__ const sIMAGE*       img_edge,
  __CR_IN__ const sRECT_FILTER* f_rect
    )
{
    byte_t  *bin_line;
    byte_t  *clr_line;
    leng_t  count, limit;
    uint_t  win_w, win_h;
    uint_t  xx, yy, tx, ty;
    leng_t  cnt_lt, cnt_rb;
    leng_t  cnt_lb, cnt_rt;
    sPNT2   pnts1[MAX_POINT * 2];
    sPNT2   pnts2[MAX_POINT * 2];
    sRECT   rects[MAX_POINT * 2];

    /* 生成特征点图 */
    img_pnts[0] = image_dup(img_edge);
    if (img_pnts[0] == NULL)
        return (FALSE);
    img_pnts[0]->pal[1] = vclRed;
    img_pnts[0]->pal[2] = vclYellow;
    img_pnts[0]->pal[3] = vclLime;
    img_pnts[0]->pal[4] = vclAqua;
    mem_zero(img_pnts[0]->data, img_pnts[0]->size);
    win_w = img_idxz->position.ww - MAT_WW;
    win_h = img_idxz->position.hh - MAT_HH;
    cnt_lt = cnt_rb = cnt_lb = cnt_rt = 0;

    /* 第一轮查找左上-右下 */
    clr_line = img_idxz->data + img_idxz->bpl;
    bin_line = img_edge->data + img_edge->bpl;
    for (yy = 1; yy < win_h; yy++, clr_line += img_idxz->bpl,
                                   bin_line += img_edge->bpl)
    for (xx = 1; xx < win_w; xx++)
    {
        /* 搜索边界点 */
        if (!shape_match_orr(&bin_line[xx], img_edge->bpl, &s_mat_comp))
            continue;

        /* 查找左上角 */
        if (cnt_lt < MAX_POINT &&
            shape_match_and(&clr_line[xx], img_idxz->bpl, &s_mat_splt)) {
            tx = xx + MAT_W2;
            ty = yy + MAT_H2;
            (pixel_addr1(img_pnts[0], tx, ty))[0] = 0x01;
            pnts1[cnt_lt].x = tx;
            pnts1[cnt_lt++].y = ty;
            continue;
        }

        /* 查找右下角 */
        if (cnt_rb < MAX_POINT &&
            shape_match_and(&clr_line[xx], img_idxz->bpl, &s_mat_sprb)) {
            tx = xx + MAT_W2;
            ty = yy + MAT_H2;
            (pixel_addr1(img_pnts[0], tx, ty))[0] = 0x02;
            pnts2[cnt_rb].x = tx;
            pnts2[cnt_rb++].y = ty;
            continue;
        }
    }

    /* 第二轮查找左下-右上 */
    clr_line = img_idxz->data + img_idxz->bpl;
    bin_line = img_edge->data + img_edge->bpl;
    for (yy = 1; yy < win_h; yy++, clr_line += img_idxz->bpl,
                                   bin_line += img_edge->bpl)
    for (xx = 1; xx < win_w; xx++)
    {
        /* 搜索边界点 */
        if (!shape_match_orr(&bin_line[xx], img_edge->bpl, &s_mat_comp))
            continue;

        /* 查找右上角 */
        if (cnt_rt < MAX_POINT &&
            shape_match_and(&clr_line[xx], img_idxz->bpl, &s_mat_sprt)) {
            tx = xx + MAT_W2;
            ty = yy + MAT_H2;
            (pixel_addr1(img_pnts[0], tx, ty))[0] = 0x04;
            pnts2[cnt_rb + cnt_rt].x = tx;
            pnts2[cnt_rb + cnt_rt++].y = ty;
            continue;
        }

        /* 查找左下角 */
        if (cnt_lb < MAX_POINT &&
            shape_match_and(&clr_line[xx], img_idxz->bpl, &s_mat_splb)) {
            tx = xx + MAT_W2;
            ty = yy + MAT_H2;
            (pixel_addr1(img_pnts[0], tx, ty))[0] = 0x03;
            pnts1[cnt_lt + cnt_lb].x = tx;
            pnts1[cnt_lt + cnt_lb++].y = ty;
            continue;
        }
    }

    /* 搜索特征矩形 */
    limit = cntsof(rects);
    count = rect_get_lt_rb(&rects[0], limit, &pnts1[0], cnt_lt,
                           &pnts2[0], cnt_rb, f_rect);
    count += rect_get_lb_rt(&rects[count], limit - count,
                &pnts1[cnt_lt], cnt_lb, &pnts2[cnt_rb], cnt_rt, f_rect);
    count = rect_merge(rects, count, f_rect);
    found[0] = rect_max_density(result, rects, count,
                pnts1, cnt_lt + cnt_lb, pnts2, cnt_rb + cnt_rt);
    return (TRUE);
}

/*
---------------------------------------
    字符高度确定
---------------------------------------
*/
static bool_t
text_height (
  __CR_OT__ sint_t*         y0,
  __CR_OT__ sint_t*         y1,
  __CR_IN__ const uint_t*   proj_y,
  __CR_IN__ sint_t          length
    )
{
    sint_t  idx;
    uint_t  gate = 0;

    for (idx = 0; idx < length; idx++)
        gate += proj_y[idx];
    gate /= 4 * length;
    for (idx = length / 2; idx >= 0; idx--) {
        if (proj_y[idx] <= gate)
            break;
    }
    if (idx < 0)
        idx += 1;
    *y0 = idx;
    for (idx = length / 2; idx < length; idx++) {
        if (proj_y[idx] <= gate)
            break;
    }
    if (idx == length)
        idx -= 1;
    *y1 = idx;
    if (*y0 == 0 && *y1 == length - 1)
        return (FALSE);
    return (TRUE);
}

/*
---------------------------------------
    字符宽度确定
---------------------------------------
*/
static bool_t
text_width (
  __CR_OT__ sint_t  cut[9],
  __CR_IN__ sint_t  x0,
  __CR_IN__ sint_t  x1,
  __CR_IO__ uint_t* proj_x
    )
{
    sint_t  cut1[7];
    sint_t  cut2[7];
    uint_t  move1 = 0;
    uint_t  move2 = 0;
    sint_t  idx, flag = 1;

    /* 查找字符边界 */
    for (idx = x0; idx < x1; idx++) {
        if (flag > 0) {
            if (proj_x[idx] == 0 && proj_x[idx + 1] != 0) {
                if (move1 == 7)
                    return (FALSE);
                cut1[move1++] = idx;
                flag = -flag;
            }
        }
        else {
            if (proj_x[idx] != 0 && proj_x[idx + 1] == 0) {
                if (move2 == 7)
                    return (FALSE);
                cut2[move2++] = idx + 1;
                flag = -flag;
            }
        }
    }
    if (move1 != 7 || move2 != 7)
        return (FALSE);

    /* 计算分隔符 */
    cut[0] = (cut1[0] - x0);
    cut[1] = (cut2[0] + cut1[1]) / 2 - x0;
    cut[2] = (cut2[1] - x0);
    cut[3] = (cut1[2] - x0);
    for (idx = 4; idx <= 7; idx++)
        cut[idx] = (cut2[idx - 2] + cut1[idx - 1]) / 2 - x0;
    cut[8] = (cut2[6] - x0);
    return (TRUE);
}

/*
---------------------------------------
    过滤水平投影
---------------------------------------
*/
static void_t
project_filter (
  __CR_OT__ sint_t* x0,
  __CR_OT__ sint_t* x1,
  __CR_IN__ uint_t  gate,
  __CR_IO__ uint_t* proj_x,
  __CR_IN__ sint_t  length
    )
{
    sint_t  idx;

    /* 去除极小值 */
    for (idx = 0; idx < length; idx++) {
        if (proj_x[idx] <= 2)
            proj_x[idx] = 0;
    }

    /* 去除孤立值 */
    if ((proj_x[0] != 0 || proj_x[1] != 0) && proj_x[2] == 0)
        proj_x[0] = proj_x[1] = 0;
    for (idx = 1; idx < length - 1; idx++) {
        if (proj_x[idx] != 0 && proj_x[idx] <= gate &&
            proj_x[idx - 1] == 0 && proj_x[idx + 1] == 0)
            proj_x[idx] = 0;
    }

    /* 查找起始边 */
    for (idx = 0; idx < length - 3; idx++) {
        if (proj_x[idx + 0] == 0 &&
            proj_x[idx + 1] == 0 && proj_x[idx + 3] != 0)
            break;
    }
    if (idx == length - 3)
        idx -= 1;
    *x0 = idx + 2;

    /* 查找结束边 */
    for (idx = length - 4; idx >= 0; idx--) {
        if (proj_x[idx + 0] != 0 &&
            proj_x[idx + 2] == 0 && proj_x[idx + 3] == 0)
            break;
    }
    if (idx < 0)
        idx += 1;
    *x1 = idx + 1;

#if OUTPUT_PRJ_X
    FILE*   fp;
    fp = fopen("test.txt", "w");
    for (idx = *x0; idx <= *x1; idx++)
        fprintf(fp, "%d\n", proj_x[idx]);
    fclose(fp);
#endif
}

/*
=======================================
    ILAB 初始化
=======================================
*/
CR_DLL bool_t
ilab_init (
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 加载字母 OCR */
    s_tess_al = tessocr_init(QST_PATH_SOURCE "tessdata/", "eng");
    if (s_tess_al == NULL)
        return (FALSE);
    if (!tessocr_set_param(s_tess_al, "tessedit_char_whitelist",
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
        return (FALSE);
    tessocr_set_ppi(s_tess_al, SCN_PPI);

    /* 加载字母数字 OCR */
    s_tess_an = tessocr_init(QST_PATH_SOURCE "tessdata/", "eng");
    if (s_tess_an == NULL)
        return (FALSE);
    if (!tessocr_set_param(s_tess_an, "tessedit_char_whitelist",
                            "0123456789ABCDEFGHJKLMNPQRSTUVWXYZ"))
        return (FALSE);
    tessocr_set_ppi(s_tess_an, SCN_PPI);

    /* 杂项初始化 */
    s_cstep = color_step_init();
    if (s_cstep == NULL)
        return (FALSE);
    color_step_pal((int32u*)s_cstep_pal, 15);
    s_clbox.val = 0xFFFF0000;
    s_cllne.val = 0xFF00FF00;
    return (TRUE);
}

/*
=======================================
    ILAB 主循环
=======================================
*/
CR_DLL bool_t
ilab_main (
  __CR_OT__ sILAB_OUTPUT*       result,
  __CR_IN__ const sILAB_INPUT*  ximglab
    )
{
    sBLIT   blit;
    sLINE   line;
    sRECT   inside;
    sRECT   otside;
    ansi_t* strtmp;
    ansi_t* string;
    uint_t* project;
    sIMAGE* img_orgz;
    sIMAGE* img_clrz;
    sIMAGE* img_binz;
    sIMAGE* img_idxz;
    sIMAGE* img_edge;
    sIMAGE* img_pnts;
    sIMAGE* img_temp;
    sint_t  chr_cut[9];
    fp32_t  rho, theta;
    bool_t  okay1, okay2;
    uint_t  win_w, win_h;
    leng_t  len, tab[256];
    sint_t  aa, bb, x0, y0;
    /* ----------------- */
    sLINE_HOUGH     hough;
    sRECT_FILTER    f_rect;

    /********************/
    /* Init: 环境初始化 */
    /********************/
    result->n_img = 5;
    result->frame = mem_talloc(result->n_img, sIMAGE*);
    if (result->frame == NULL)
        return (FALSE);
    result->n_txt = 2;
    result->texts = mem_talloc(result->n_txt, sILAB_TXTOUT);
    if (result->texts == NULL)
        return (FALSE);
    img_orgz = ximglab->input;
    win_w = img_orgz->position.ww;
    win_h = img_orgz->position.hh;
    result->texts[0].idx = 4;
    result->texts[0].aln = EGUI_ALN_RM;
    result->texts[0].pge = CR_LOCAL;
    result->texts[0].fgc = 0x00FFFF00UL;
    result->texts[0].bkc = 0x000000FFUL;
    result->texts[0].trn = FALSE;
    result->texts[0].kll = FALSE;
    result->texts[0].txt = s_ang;
    rect_set_wh(&result->texts[0].pos, 0, 10, win_w * 7 / 8, 16);
    struct_cpy(&result->texts[1], &result->texts[0], sILAB_TXTOUT);
    result->texts[1].txt = s_txt;
    result->texts[1].pos.y1 += 16;
    result->texts[1].pos.y2 += 16;
    s_ang[0] = s_txt[0] = 0x00;

    /*****************/
    /* Step0: 二值化 */
    /*****************/
    img_clrz = image_convo(img_orgz, &s_mat_blur);
    if (img_clrz == NULL)
        return (FALSE);
    img_binz = image_graying(img_clrz);
    if (img_binz == NULL)
        return (FALSE);
    image_histo(tab, img_binz);
    image_binary1(img_binz, histo_otsu(tab));
    {
        /* 返回中间调试结果 */
        result->frame[0] = img_idx8_to_32(NULL, 0, 0, img_binz);
        if (result->frame[0] == NULL)
            return (FALSE);
    }

    /*******************/
    /* Step1: 颜色滤波 */
    /*******************/
    img_idxz = image_indexed(img_clrz, do_pixel, s_cstep_pal, 15, s_cstep);
    if (img_idxz == NULL)
        return (FALSE);
    {
        /* 返回中间调试结果 */
        result->frame[1] = img_idx8_to_32(NULL, 0, 0, img_idxz);
        if (result->frame[1] == NULL)
            return (FALSE);
    }

    /*******************/
    /* Step2: 边缘提取 */
    /*******************/
    img_edge = image_convo(img_binz, &s_mat_edge);
    if (img_edge == NULL)
        return (FALSE);
    {
        /* 返回中间调试结果 */
        result->frame[2] = img_idx8_to_32(NULL, 0, 0, img_edge);
        if (result->frame[2] == NULL)
            return (FALSE);
    }

    /*********************/
    /* Step3: 车牌粗定位 */
    /*********************/
    f_rect.merge = BOX_MERGE;
    f_rect.min_s = BOX_MIN_S;
    f_rect.max_s = BOX_MAX_S;
    f_rect.min_w = BOX_MIN_W;
    f_rect.min_h = BOX_MIN_H;
    f_rect.max_w = BOX_MAX_W;
    f_rect.max_h = BOX_MAX_H;
    if (!find_rect(&inside, &okay1, &img_pnts, img_idxz, img_edge, &f_rect))
        return (FALSE);
    {
        /* 返回中间调试结果 */
        result->frame[3] = img_idx8_to_32(NULL, 0, 0, img_pnts);
        if (result->frame[3] == NULL)
            return (FALSE);
    }

    /***********************/
    /* Step4: 车牌倾斜修正 */
    /***********************/
    okay2 = okay1;
    rho = 1.0f;
    theta = CR_DTOR / 2.0f;
    if (okay1)
    {
        /* 适当扩大范围完成粗定位 */
        inside.x1 -= EXPAND_IN;
        inside.x2 += EXPAND_IN;
        inside.ww += EXPAND_IN * 2;
        struct_cpy(&otside, &inside, sRECT);
        otside.x1 -= EXPAND_OT;
        otside.y1 -= EXPAND_OT;
        otside.x2 += EXPAND_OT;
        otside.y2 += EXPAND_OT;
        otside.ww += EXPAND_OT * 2;
        otside.hh += EXPAND_OT * 2;
        if (otside.x1 < 0 || (uint_t)otside.x2 >= win_w ||
            otside.y1 < 0 || (uint_t)otside.y2 >= win_h) {
            okay2 = FALSE;
        }
        else {
            img_temp = image_grab(img_edge, &otside);
            if (img_temp == NULL)
                return (FALSE);
            if (!line_hough(&hough, img_temp, rho, theta,
                                otside.ww / 2, 255))
                return (FALSE);
            okay2 = line_hough_get(line_hough_max(&hough),
                                   &rho, &theta, &hough);
            line_hough_free(&hough);
            image_del(img_temp);
            if (okay2)
            {
                /* 逆时针转过的角度 */
                theta = CR_PI / 2.0f - theta;
                if (theta > CR_DTOR *  25.0f ||
                    theta < CR_DTOR * -25.0f)
                    okay2 = FALSE;
            }
        }
    }
    {
        /* 返回中间调试结果 */
        result->frame[4] = image_dup(img_orgz);
        if (result->frame[4] == NULL)
            return (FALSE);
        if (okay1)
        {
            /* 绘制车牌方框 */
            draw_box3(result->frame[4], &inside, s_cllne);
            draw_box3(result->frame[4], &otside, s_clbox);
            if (okay2)
            {
                /* 绘制倾斜角度线和角度值 */
                x0 = (otside.x1 + otside.x2) / 2;
                y0 = (otside.y1 + otside.y2) / 2;
                aa = fp32_to_sint(FCOS(theta) * 1024.0f);
                bb = fp32_to_sint(FSIN(theta) * 1024.0f);
                line_set_xy(&line, x0 + aa, y0 - bb, x0 - aa, y0 + bb);
                draw_line(result->frame[4], &line, s_cllne, pixel_set32z);
                sprintf(s_ang, "%.2f", theta / CR_DTOR);
            }
        }
    }

    /*********************/
    /* Step5: 车牌细定位 */
    /*********************/
    if (!okay2) goto _kill;

    /* 旋转抽出原始图像并二值化 */
    image_del(img_clrz);
    img_clrz = image_rotz(img_orgz, &inside, -theta, 1.0f, FALSE);
    if (img_clrz == NULL)
        return (FALSE);

    /* 利用宽高比切掉旋转截图的多余部分 */
    aa = img_clrz->position.ww;
    bb = img_clrz->position.hh;
    rho = (fp32_t)aa;
    if (rho / bb < BOX_SCALE) {
        bb = fp32_to_sint(rho / BOX_SCALE);
        rect_set_wh(&otside, 0, 0, aa, bb);
        img_temp = image_grab(img_clrz, &otside);
        if (img_temp == NULL)
            return (FALSE);
        image_del(img_clrz);
        img_clrz = img_temp;
    }

    /* 二值化抽出的图片 */
    image_del(img_binz);
    img_binz = image_graying(img_clrz);
    if (img_binz == NULL)
        return (FALSE);
    image_histo(tab, img_binz);
    image_binary1(img_binz, histo_otsu(tab));

    /* 使用 Y 投影去除上下边界 */
    project = mem_talloc(img_binz->position.hh, uint_t);
    if (project == NULL)
        return (FALSE);
    tex_project_y(project, img_binz, 255);
    okay1 = text_height(&aa, &bb, project, img_binz->position.hh);
    mem_free(project);
    if (okay1) {
        if (bb - aa + 1 < (sint_t)BOX_MIN_H)
            goto _kill;
        rect_set_xy(&otside, 0, aa, img_binz->clip_win.x2, bb);
        img_temp = image_grab(img_binz, &otside);
        if (img_temp == NULL)
            return (FALSE);
        image_del(img_binz);
        img_binz = img_temp;
    }

    /* 腐蚀结果图片, 加大字之间的边距 */
    image_del(img_pnts);
    img_pnts = image_shape(img_binz, &s_mat_rotz, FALSE);
    if (img_pnts == NULL)
        return (FALSE);

    /* 使用 X 投影水平分割字符 */
    project = mem_talloc(img_pnts->position.ww, uint_t);
    if (project == NULL)
        return (FALSE);
    tex_project_x(project, img_pnts, 255);
    project_filter(&aa, &bb, img_pnts->position.hh / 3,
                    project, img_pnts->position.ww);
    if (bb - aa + 1 < (sint_t)BOX_MIN_W) {
        mem_free(project);
        goto _kill;
    }
    rect_set_xy(&otside, aa, 0, bb, img_binz->clip_win.y2);
    rho = (fp32_t)otside.ww / (fp32_t)otside.hh;
    if (rho <= BOX_MID_S || rho >= BOX_MAX_S) {
        mem_free(project);
        goto _kill;
    }
    img_temp = image_grab(img_binz, &otside);
    if (img_temp == NULL)
        return (FALSE);
    image_del(img_binz);
    img_binz = img_temp;
    for (len = 0; len < img_binz->size; len++)
        img_binz->data[len] = ~img_binz->data[len];
    okay1 = text_width(chr_cut, aa, bb, project);
    mem_free(project);
    if (!okay1)
        goto _kill;
    {
        /* 返回中间调试结果 */
        img_idx8_to_32(result->frame[4], 0, 0, img_binz);
        for (x0 = 0; x0 < cntsof(chr_cut); x0++) {
            draw_linev(result->frame[4], chr_cut[x0],
                       img_binz->clip_win.y1, img_binz->clip_win.y2,
                       s_clbox, pixel_set32z);
        }
#if STEP_BY_STEP
        while (!key_input_click('A'))
            thread_sleep(1);
#endif
    }

    /*******************/
    /* Step6: 字符识别 */
    /*******************/

    /* 汉字 */
    len = 0x00;
    rect_set_xy(&otside, chr_cut[0], img_binz->clip_win.y1,
                         chr_cut[1], img_binz->clip_win.y2);
    img_temp = image_new(0, 0, otside.ww + FNT_MOV * 2,
                               otside.hh + FNT_MOV * 2,
                                CR_INDEX8, FALSE, 4);
    if (img_temp == NULL)
        return (FALSE);
    blit.dx = FNT_MOV;
    blit.dy = FNT_MOV;
    blit.sx = otside.x1;
    blit.sy = otside.y1;
    blit.sw = otside.ww;
    blit.sh = otside.hh;
    mem_set(img_temp->data, 255, img_temp->size);
    blit_set08_c(img_temp, img_binz, &blit, NULL);
    {
        /* 返回中间调试结果 */
        img_temp->pal[0x00] = 0xFF000000UL;
        img_temp->pal[0xFF] = 0xFFFFFFFFUL;
        aa = img_binz->position.hh + FNT_MOV * 2;
        img_idx8_to_32(result->frame[4], 0, aa, img_temp);
        aa += img_temp->position.hh + FNT_MOV;
    }
    image_del(img_temp);

    /* 字母 */
    rect_set_xy(&otside, chr_cut[1], img_binz->clip_win.y1,
                         chr_cut[2], img_binz->clip_win.y2);
    img_temp = image_new(0, 0, otside.ww + FNT_MOV * 2,
                               otside.hh + FNT_MOV * 2,
                                CR_INDEX8, FALSE, 4);
    if (img_temp == NULL)
        return (FALSE);
    blit.sx = otside.x1;
    blit.sy = otside.y1;
    blit.sw = otside.ww;
    blit.sh = otside.hh;
    mem_set(img_temp->data, 255, img_temp->size);
    blit_set08_c(img_temp, img_binz, &blit, NULL);
    {
        /* 返回中间调试结果 */
        img_temp->pal[0x00] = 0xFF000000UL;
        img_temp->pal[0xFF] = 0xFFFFFFFFUL;
        img_idx8_to_32(result->frame[4], 0, aa, img_temp);
        aa += img_temp->position.hh + FNT_MOV;
    }
    tessocr_set_image(s_tess_al, img_temp);
    string = tessocr_get_utf8(s_tess_al);
    if (string == NULL)
        return (FALSE);
    strtmp = skip_spaceA(string);
    s_txt[len++] = *strtmp;
    tessocr_str_free(string);
    image_del(img_temp);
    s_txt[len++] = 0x20;

    /* 字母数字 */
    for (x0 = 3; x0 <= 7; x0++) {
        rect_set_xy(&otside, chr_cut[x0 + 0], img_binz->clip_win.y1,
                             chr_cut[x0 + 1], img_binz->clip_win.y2);
        img_temp = image_new(0, 0, otside.ww + FNT_MOV * 2,
                                   otside.hh + FNT_MOV * 2,
                                    CR_INDEX8, FALSE, 4);
        if (img_temp == NULL)
            return (FALSE);
        blit.sx = otside.x1;
        blit.sy = otside.y1;
        blit.sw = otside.ww;
        blit.sh = otside.hh;
        mem_set(img_temp->data, 255, img_temp->size);
        blit_set08_c(img_temp, img_binz, &blit, NULL);
        {
            /* 返回中间调试结果 */
            img_temp->pal[0x00] = 0xFF000000UL;
            img_temp->pal[0xFF] = 0xFFFFFFFFUL;
            img_idx8_to_32(result->frame[4], 0, aa, img_temp);
            aa += img_temp->position.hh + FNT_MOV;
        }
        tessocr_set_image(s_tess_an, img_temp);
        string = tessocr_get_utf8(s_tess_an);
        if (string == NULL)
            return (FALSE);
        strtmp = skip_spaceA(string);
        s_txt[len++] = *strtmp;
        tessocr_str_free(string);
        image_del(img_temp);
    }
    s_txt[len] = 0x00;

_kill:
    /**********************/
    /* Kill: 释放所有资源 */
    /**********************/
    image_del(img_pnts);
    image_del(img_edge);
    image_del(img_idxz);
    image_del(img_binz);
    image_del(img_clrz);
    return (TRUE);
}

/*
=======================================
    ILAB 资源销毁
=======================================
*/
CR_DLL void_t
ilab_kill (void_t)
{
    color_step_kill(s_cstep);
    tessocr_kill(s_tess_al);
    tessocr_kill(s_tess_an);
}
