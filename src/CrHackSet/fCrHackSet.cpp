
#include "../QstLibs/QstLibs.h"

#include <math.h>

/*****************************************************************************/
/*                                 公用函数                                  */
/*****************************************************************************/

/*
---------------------------------------
    计算梯度长度
---------------------------------------
*/
static sint_t
grad_length (
  __CR_IN__ byte_t  p1,
  __CR_IN__ byte_t  p2
    )
{
    fp32_t  len;
    sint_t  tx = p1;
    sint_t  ty = p2;

    tx *= tx;
    ty *= ty;
    len = FSQRT((fp32_t)(tx + ty));
    return (fp32_to_sint(len));
}

/*
---------------------------------------
    计算平均亮度
---------------------------------------
*/
static uint_t
image_avg_light (
  __CR_IN__ const sIMAGE*   img
    )
{
    int64u  total = 0;
    uint_t  ww = img->position.ww;
    uint_t  hh = img->position.hh;
    byte_t* line = img->data;

    for (uint_t yy = 0; yy < hh; yy++) {
        byte_t* ptr = line;
        for (uint_t xx = 0; xx < ww; xx++) {
            total += rgb2light(ptr[2], ptr[1], ptr[0]);
            ptr += sizeof(int32u);
        }
        line += img->bpl;
    }
    total /= ww;
    total /= hh;
    return ((uint_t)total);
}

/*
---------------------------------------
    卷积运算 (3 x 3)
---------------------------------------
*/
static void_t
conv3x3_main (
  __CR_IN__ const sIMAGE*   image,
  __CR_IN__ const sint_t    mat[9]
    )
{
    sIMAGE*     stemp;
    sIMAGE*     bound;
    sCONVO_MAT  convo;

    /* 扩展边缘 */
    bound = image_bound(image, 1, 1);
    if (bound == NULL)
        return;

    /* 处理矩阵 */
    convo.dt = mat;
    convo.ww = convo.hh = 3;
    convo.kk = mat[0];
    for (uint_t idx = 1; idx < 9; idx++)
        convo.kk += mat[idx];
    if (convo.kk <= 0)
        convo.kk = 1;

    /* 卷积并回图 */
    stemp = image_convo(bound, &convo);
    image_del(bound);
    if (stemp == NULL)
        return;
    image_unbound(image, stemp, 1, 1);
    image_del(stemp);
}

/*
---------------------------------------
    形态运算 (3 x 3)
---------------------------------------
*/
static void_t
conv3x3_form (
  __CR_IO__ sIMAGE*         image,
  __CR_IN__ const sint_t    mat[9],
  __CR_IN__ bool_t          expand,
  __CR_IN__ bool_t          first
    )
{
    sIMAGE*     binz;
    sIMAGE*     stemp;
    sIMAGE*     bound;
    byte_t      shp[9];
    sSHAPE_MAT  shape;

    /* 先二值化 */
    binz = image_graying(image);
    if (binz == NULL)
        return;
    if (first)
        image_binary0(binz, TRUE);

    /* 扩展边缘 */
    bound = image_bound(binz, 1, 1);
    if (bound == NULL) {
        image_del(binz);
        return;
    }

    /* 处理形态 */
    for (uint_t idx = 0; idx < 9; idx++) {
        if (mat[idx] < 0)
            shp[idx] = 0x80;
        else
        if (mat[idx] > 0)
            shp[idx] = 0xFF;
        else
            shp[idx] = 0x00;
    }
    shape.dt = shp;
    shape.ww = shape.hh = 3;

    /* 形态并回图 */
    stemp = image_shape(bound, &shape, expand);
    image_del(bound);
    if (stemp == NULL) {
        image_del(binz);
        return;
    }
    image_unbound(binz, stemp, 1, 1);
    image_del(stemp);

    /* 转换到原图 */
    img_idx8_to_32(image, 0, 0, binz);
    image_del(binz);
}

/*****************************************************************************/
/*                                 滤镜接口                                  */
/*****************************************************************************/

/*
---------------------------------------
    图片逻辑-与
---------------------------------------
*/
static bool_t
fill_and_x86 (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    fill.dx = fill.dy = 0;
    fill.dw = dest->position.ww;
    fill.dh = dest->position.hh;
    clrs.val = xml_attr_intx32U("mask", 0xFFFFFFFF, param);
    fill_and32_c(dest, &fill, clrs, NULL);
    return (TRUE);
}

/*
---------------------------------------
    图片逻辑-或
---------------------------------------
*/
static bool_t
fill_orr_x86 (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    fill.dx = fill.dy = 0;
    fill.dw = dest->position.ww;
    fill.dh = dest->position.hh;
    clrs.val = xml_attr_intx32U("mask", 0x00000000, param);
    fill_orr32_c(dest, &fill, clrs, NULL);
    return (TRUE);
}

/*
---------------------------------------
    图片逻辑-非
---------------------------------------
*/
static bool_t
fill_not_x86 (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(netw);
    CR_NOUSE(param);
    dest = (sIMAGE*)image;
    fill.dx = fill.dy = 0;
    fill.dw = dest->position.ww;
    fill.dh = dest->position.hh;
    clrs.val = 0x00000000;
    fill_not32_c(dest, &fill, clrs, NULL);
    return (TRUE);
}

/*
---------------------------------------
    图片逻辑异或
---------------------------------------
*/
static bool_t
fill_xor_x86 (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    fill.dx = fill.dy = 0;
    fill.dw = dest->position.ww;
    fill.dh = dest->position.hh;
    clrs.val = xml_attr_intx32U("mask", 0x00000000, param);
    fill_xor32_c(dest, &fill, clrs, NULL);
    return (TRUE);
}

/*
---------------------------------------
    图片饱和加法
---------------------------------------
*/
static bool_t
fill_add_x86 (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    fill.dx = fill.dy = 0;
    fill.dw = dest->position.ww;
    fill.dh = dest->position.hh;
    clrs.val = xml_attr_intx32U("color", 0x00000000, param);
    fill_add32_c(dest, &fill, clrs, NULL);
    return (TRUE);
}

/*
---------------------------------------
    图片饱和减法
---------------------------------------
*/
static bool_t
fill_sub_x86 (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    fill.dx = fill.dy = 0;
    fill.dw = dest->position.ww;
    fill.dh = dest->position.hh;
    clrs.val = xml_attr_intx32U("color", 0x00000000, param);
    fill_sub32_c(dest, &fill, clrs, NULL);
    return (TRUE);
}

/*
---------------------------------------
    图片插值填充
---------------------------------------
*/
static bool_t
fill_lrp_x86 (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    fill.dx = fill.dy = 0;
    fill.dw = dest->position.ww;
    fill.dh = dest->position.hh;
    clrs.val = xml_attr_intx32U("color", 0x00000000, param);
    fill_lrp32_c(dest, &fill, clrs, NULL);
    return (TRUE);
}

/*
---------------------------------------
    图片垂直翻转
---------------------------------------
*/
static bool_t
flip_vertical (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    CR_NOUSE(netw);
    CR_NOUSE(param);
    image_flp((sIMAGE*)image, FALSE);
    return (TRUE);
}

/*
---------------------------------------
    图片 RB 互换
---------------------------------------
*/
static bool_t
swap_red_blue (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    leng_t  size;
    byte_t* data;
    sIMAGE* dest;

    CR_NOUSE(netw);
    CR_NOUSE(param);
    dest = (sIMAGE*)image;
    if (dest->fmt == CR_ARGB8888) {
        size = dest->size;
        data = dest->data;
        for (size /= 4; size != 0; size--) {
            swap_rb32(data);
            data += sizeof(int32u);
        }
    }
    return (TRUE);
}

/* 颜色扩散细分 */
#define DIFFUSE_MAX     256

/*
---------------------------------------
    图片颜色扩散
---------------------------------------
*/
static bool_t
image_diffuse (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    int32u* pos1;
    int32u* pos2;
    sIMAGE* dest;
    sint_t  minx, miny;
    sint_t  maxx, maxy;
    sint_t  dx, dy, ww, hh;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    minx = xml_attr_intxU("min_x", (uint_t)-5, param);
    maxx = xml_attr_intxU("max_x", (uint_t)+5, param);
    miny = xml_attr_intxU("min_y", (uint_t)-5, param);
    maxy = xml_attr_intxU("max_y", (uint_t)+5, param);
    maxx -= minx;
    maxy -= miny;
    ww = dest->position.ww;
    hh = dest->position.hh;
    rand_seed(timer_get32());
    for (sint_t yy = 0; yy < hh; yy++)
    for (sint_t xx = 0; xx < ww; xx++)
    {
        dx = rand_getx(DIFFUSE_MAX);
        dy = rand_getx(DIFFUSE_MAX);
        dx = (dx * maxx) / DIFFUSE_MAX + minx;
        dy = (dy * maxy) / DIFFUSE_MAX + miny;
        pos1 = pixel_addr4(dest, xx, yy);
        if (xx + dx < 0 || xx + dx >= ww)
            dx = 0;
        if (yy + dy < 0 || yy + dy >= hh)
            dy = 0;
        pos2 = pixel_addr4(dest, xx + dx, yy + dy);
        pos2[0] = pos1[0];
    }
    return (TRUE);
}

/*
---------------------------------------
    图片灰度转换
---------------------------------------
*/
static bool_t
image_graying (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    byte_t* line;
    sIMAGE* dest;
    byte_t  tab[256];
    leng_t  hist[256];
    uint_t  histo_ave;
    uint_t  ww, hh, ii;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    mem_zero(hist, sizeof(hist));
    histo_ave = xml_attr_intxU("ave", TRUE, param);
    line = dest->data;
    ww = dest->position.ww;
    hh = dest->position.hh;
    for (uint_t yy = 0; yy < hh; yy++) {
        byte_t* ptr = line;
        for (uint_t xx = 0; xx < ww; xx++) {
            ii = rgb2light(ptr[2], ptr[1], ptr[0]);
            ptr[0] = (byte_t)ii;
            ptr[1] = (byte_t)ii;
            ptr[2] = (byte_t)ii;
            hist[ii] += 1;
            ptr += sizeof(int32u);
        }
        line += dest->bpl;
    }
    if (histo_ave) {
        dot_histo_ave(tab, hist, ww, hh);
        image_lookup3(dest, tab, tab, tab);
    }
    return (TRUE);
}

/*
---------------------------------------
    图片二值转换
---------------------------------------
*/
static bool_t
image_binaryz (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    byte_t* line;
    sIMAGE* dest;
    uint_t  gate;
    uint_t  ww, hh, ii;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    ww = dest->position.ww;
    hh = dest->position.hh;
    gate = xml_attr_intxU("gate", 127, param);
    if (!param->found)
        gate = image_avg_light(dest);

    /* 二值化计算 */
    line = dest->data;
    for (uint_t yy = 0; yy < hh; yy++) {
        byte_t* ptr = line;
        for (uint_t xx = 0; xx < ww; xx++) {
            ii = rgb2light(ptr[2], ptr[1], ptr[0]);
            ptr[0] = (ii > gate) ? 255 : 0;
            ptr[1] = ptr[0];
            ptr[2] = ptr[0];
            ptr += sizeof(int32u);
        }
        line += dest->bpl;
    }
    return (TRUE);
}

/*
---------------------------------------
    图片卷积运算 (3 x 3)
---------------------------------------
*/
static bool_t
image_conv3x3 (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    uint_t  idx;
    ansi_t* str;
    sIMAGE* dest;
    sint_t  mat[9];

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    str = xml_attr_bufferU("mat", param);
    if (str == NULL ||
        str2lstA((uint_t*)mat, 9, str, "[],") == NULL) {
        for (idx = 0; idx < 9; idx++)
            mat[idx] = 1;
    }
    conv3x3_main(dest, mat);
    return (TRUE);
}

/* SOBEL 的两个卷积矩阵 */
static const sint_t s_sobel_matx[9] =
{
    -1,  0,  1,
    -2,  0,  2,
    -1,  0,  1,
};
static const sint_t s_sobel_maty[9] =
{
     1,  2,  1,
     0,  0,  0,
    -1, -2, -1,
};

/*
---------------------------------------
    SOBEL 边缘检测
---------------------------------------
*/
static bool_t
image_edge_sobel (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE* temp;
    sIMAGE* dest;

    CR_NOUSE(netw);
    CR_NOUSE(param);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    temp = image_dup(dest);
    if (temp == NULL)
        return (TRUE);
    conv3x3_main(dest, s_sobel_matx);
    conv3x3_main(temp, s_sobel_maty);
    image_oper(dest, temp, grad_length);
    image_del(temp);
    return (TRUE);
}

/*
---------------------------------------
    图片门限过滤
---------------------------------------
*/
static bool_t
image_cut_down (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    byte_t* line;
    sIMAGE* dest;
    uint_t  gate;
    uint_t  ww, hh, ii;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    ww = dest->position.ww;
    hh = dest->position.hh;
    gate = xml_attr_intxU("gate", 97, param);
    if (!param->found)
        gate = image_avg_light(dest);

    /* 掐掉暗的像素 */
    line = dest->data;
    for (uint_t yy = 0; yy < hh; yy++) {
        byte_t* ptr = line;
        for (uint_t xx = 0; xx < ww; xx++) {
            ii = rgb2light(ptr[2], ptr[1], ptr[0]);
            if (ii <= gate) {
                ptr[0] = 0x00;
                ptr[1] = 0x00;
                ptr[2] = 0x00;
            }
            ptr += sizeof(int32u);
        }
        line += dest->bpl;
    }
    return (TRUE);
}

/* 标准的色相表 */
static const sint_t s_hue[12] =
{
    15,     /* [  0 -  15] 红色区 0xFF0000 */
    45,     /* [ 15 -  45] 橙色区 0xFF8000 */
    75,     /* [ 45 -  75] 黄色区 0xFFFF00 */
    105,    /* [ 75 - 105] 黄绿区 0x80FF00 */
    135,    /* [105 - 135] 绿色区 0x00FF00 */
    165,    /* [135 - 165] 青绿区 0x00FF80 */
    195,    /* [165 - 195] 青色区 0x00FFFF */
    225,    /* [195 - 225] 青蓝区 0x0080FF */
    255,    /* [225 - 255] 蓝色区 0x0000FF */
    285,    /* [255 - 285] 蓝紫区 0x8000FF */
    315,    /* [285 - 315] 紫色区 0xFF00FF */
    345,    /* [315 - 345] 紫红区 0xFF0080 */
            /* [345 - 360] 红色区 0xFF0000 */
};
static const byte_t s_color[13 * 3] =
{
    0xFF, 0x00, 0x00,
    0xFF, 0x80, 0x00,
    0xFF, 0xFF, 0x00,
    0x80, 0xFF, 0x00,
    0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x80,
    0x00, 0xFF, 0xFF,
    0x00, 0x80, 0xFF,
    0x00, 0x00, 0xFF,
    0x80, 0x00, 0xFF,
    0xFF, 0x00, 0xFF,
    0xFF, 0x00, 0x80,
    0xFF, 0x00, 0x00,
};

/*
---------------------------------------
    图片色阶过滤
---------------------------------------
*/
static bool_t
image_clr_step (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    ansi_t* str;
    byte_t* line;
    sIMAGE* dest;
    uint_t  idx, ww, hh;
    sint_t  hsl[3], hue[12];
    sint_t  min_st, min_lt, max_lt;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    line = dest->data;
    ww = dest->position.ww;
    hh = dest->position.hh;
    min_st = xml_attr_intxU("min_sat", 63, param);
    min_lt = xml_attr_intxU("min_lit", 15, param);
    max_lt = xml_attr_intxU("max_lit", 199, param);
    str = xml_attr_bufferU("table", param);
    if (str == NULL ||
        str2lstA((uint_t*)hue, 12, str, "[],") == NULL)
        mem_cpy(hue, s_hue, sizeof(hue));
    for (uint_t yy = 0; yy < hh; yy++) {
        byte_t* ptr = line;
        for (uint_t xx = 0; xx < ww; xx++) {
            bgr2hsl(hsl, ptr);
            if (hsl[1] <= min_st) {
                if (hsl[2] <= 128) {
                    ptr[0] = 0x00;
                    ptr[1] = 0x00;
                    ptr[2] = 0x00;
                }
                else {
                    ptr[0] = 0xFF;
                    ptr[1] = 0xFF;
                    ptr[2] = 0xFF;
                }
            }
            else
            if (hsl[2] <= min_lt) {
                ptr[0] = 0x00;
                ptr[1] = 0x00;
                ptr[2] = 0x00;
            }
            else
            if (hsl[2] > max_lt) {
                ptr[0] = 0xFF;
                ptr[1] = 0xFF;
                ptr[2] = 0xFF;
            }
            else {
                for (idx = 0; idx < cntsof(hue); idx++) {
                    if (hsl[0] <= hue[idx])
                        break;
                }
                idx *= 3;
                ptr[0] = s_color[idx + 2];
                ptr[1] = s_color[idx + 1];
                ptr[2] = s_color[idx + 0];
            }
            ptr += sizeof(int32u);
        }
        line += dest->bpl;
    }
    return (TRUE);
}

/*
---------------------------------------
    图片乘法运算
---------------------------------------
*/
static bool_t
image_multiply (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE* dest;
    byte_t  t_r[256];
    byte_t  t_g[256];
    byte_t  t_b[256];
    fp32_t  fr, fg, fb;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    fr = xml_attr_fp32U("red", 2.0f, param);
    fg = xml_attr_fp32U("green", 2.0f, param);
    fb = xml_attr_fp32U("blue", 2.0f, param);
    dot_muladd(t_r, fr, 0.0f);
    dot_muladd(t_g, fg, 0.0f);
    dot_muladd(t_b, fb, 0.0f);
    image_lookup3(dest, t_r, t_g, t_b);
    return (TRUE);
}

/* 最大替换颜色数 */
#define LOOKUP_MAX  32

/*
---------------------------------------
    图片颜色替换
---------------------------------------
*/
static bool_t
image_replace (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    byte_t* line;
    ansi_t* data;
    sIMAGE* dest;
    uint_t  ww, hh, only;
    leng_t  ssize, dsize;
    byte_t  src[LOOKUP_MAX * 3];
    byte_t  dst[LOOKUP_MAX * 3];

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    data = xml_attr_bufferU("src", param);
    if (data == NULL)
        return (TRUE);
    ssize = sizeof(src);
    str2datA(src, &ssize, data);
    if (ssize % 3 != 0)
        return (TRUE);
    data = xml_attr_bufferU("dst", param);
    if (data == NULL)
        return (TRUE);
    dsize = sizeof(dst);
    str2datA(dst, &dsize, data);
    if (dsize != ssize)
        return (TRUE);
    only = xml_attr_intxU("only", FALSE, param);

    /* src 颜色替换成 dst 颜色 */
    line = dest->data;
    ww = dest->position.ww;
    hh = dest->position.hh;
    for (uint_t yy = 0; yy < hh; yy++) {
        byte_t* ptr = line;
        for (uint_t xx = 0; xx < ww; xx++) {
            for (ssize = 0; ssize < dsize; ssize += 3) {
                if (ptr[0] == src[ssize + 0] &&
                    ptr[1] == src[ssize + 1] &&
                    ptr[2] == src[ssize + 2]) {
                    ptr[0]  = dst[ssize + 0];
                    ptr[1]  = dst[ssize + 1];
                    ptr[2]  = dst[ssize + 2];
                    break;
                }
            }
            if (only && ssize >= dsize) {
                ptr[0] = 0x00;
                ptr[1] = 0x00;
                ptr[2] = 0x00;
            }
            ptr += sizeof(int32u);
        }
        line += dest->bpl;
    }
    return (TRUE);
}

/*
---------------------------------------
    图片形态运算 (3 x 3)
---------------------------------------
*/
static bool_t
image_form3x3 (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    uint_t  idx;
    ansi_t* str;
    sIMAGE* dest;
    sint_t  mat[9];
    uint_t  first;
    uint_t  expand;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    str = xml_attr_bufferU("mat", param);
    if (str == NULL ||
        str2lstA((uint_t*)mat, 9, str, "[],") == NULL) {
        for (idx = 0; idx < 9; idx++)
            mat[idx] = 1;
    }
    expand = xml_attr_intxU("exp", TRUE, param);
    first = xml_attr_intxU("fst", FALSE, param);
    conv3x3_form(dest, mat, !!expand, !!first);
    return (TRUE);
}

/*
---------------------------------------
    图片曝光处理
---------------------------------------
*/
static bool_t
image_solarize (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sint_t  gate;
    sIMAGE* dest;
    byte_t  tab[256];

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    gate = xml_attr_intxU("gate", 127, param);
    dot_solarize(tab, gate);
    image_lookup3(dest, tab, tab, tab);
    return (TRUE);
}

/*
---------------------------------------
    图片白色平衡
---------------------------------------
*/
static bool_t
image_whitebl (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sint_t  gate;
    sIMAGE* dest;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    gate = xml_attr_intxU("gate", 127, param);
    if (!param->found)
        gate = -1;
    pic_white_bl(dest, gate);
    return (TRUE);
}

/*
---------------------------------------
    图片伽玛校正
---------------------------------------
*/
static bool_t
image_gamma (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE* dest;
    fp32_t  fg, fc;
    byte_t  tab[256];

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    fg = xml_attr_fp32U("gamma", 2.5f, param);
    fc = xml_attr_fp32U("comp", 0.0f, param);
    dot_gamma(tab, fg, fc);
    image_lookup3(dest, tab, tab, tab);
    return (TRUE);
}

/*
---------------------------------------
    图片对比度调整
---------------------------------------
*/
static bool_t
image_contrast (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE* dest;
    fp32_t  value;
    byte_t  tab[256];

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    value = xml_attr_fp32U("value", 4.0f, param);
    dot_contrast(tab, value);
    image_lookup3(dest, tab, tab, tab);
    return (TRUE);
}

/*
---------------------------------------
    图片饱和度调整
---------------------------------------
*/
static bool_t
image_saturation (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE* dest;
    sIMAGE* gray;
    fp32_t  value;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    gray = image_graying(dest);
    if (gray == NULL)
        return (TRUE);
    value = xml_attr_fp32U("value", 3.0f, param);
    pic_saturation(dest, gray, value);
    image_del(gray);
    return (TRUE);
}

/*
=======================================
    滤镜接口导出表
=======================================
*/
CR_API const sXC_PORT   qst_v2d_filter[] =
{
    { "crhack_and_x86", fill_and_x86 },
    { "crhack_orr_x86", fill_orr_x86 },
    { "crhack_not_x86", fill_not_x86 },
    { "crhack_xor_x86", fill_xor_x86 },
    { "crhack_add_x86", fill_add_x86 },
    { "crhack_sub_x86", fill_sub_x86 },
    { "crhack_lrp_x86", fill_lrp_x86 },
    { "crhack_flip_vv", flip_vertical },
    { "crhack_swap_rb", swap_red_blue },
    { "crhack_diffuse", image_diffuse },
    { "crhack_graying", image_graying },
    { "crhack_binaryz", image_binaryz },
    { "crhack_conv3x3", image_conv3x3 },
    { "crhack_sobel", image_edge_sobel },
    { "crhack_cutdown", image_cut_down },
    { "crhack_imgstep", image_clr_step },
    { "crhack_multiply", image_multiply },
    { "crhack_replace", image_replace },
    { "crhack_form3x3", image_form3x3 },
    { "crhack_solarize", image_solarize },
    { "crhack_whitebl", image_whitebl },
    { "crhack_gamma", image_gamma },
    { "crhack_contrast", image_contrast },
    { "crhack_saturation", image_saturation },
    { NULL, NULL },
};
