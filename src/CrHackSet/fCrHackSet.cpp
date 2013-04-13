
#include "../QstLibs/QstLibs.h"

#include <math.h>

/*
---------------------------------------
    图片逻辑-与
---------------------------------------
*/
static bool_t
fill_and_x86 (
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(nouse);
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(nouse);
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(param);
    CR_NOUSE(nouse);
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(nouse);
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(nouse);
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(nouse);
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE* dest;

    CR_NOUSE(nouse);
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    CR_NOUSE(nouse);
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    leng_t  size;
    byte_t* data;
    sIMAGE* dest;

    CR_NOUSE(nouse);
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
  __CR_UU__ void_t*     nouse,
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

    CR_NOUSE(nouse);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    minx = (sint_t)xml_attr_intxU("min_x", (uint_t)-5, param);
    maxx = (sint_t)xml_attr_intxU("max_x", (uint_t)+5, param);
    miny = (sint_t)xml_attr_intxU("min_y", (uint_t)-5, param);
    maxy = (sint_t)xml_attr_intxU("max_y", (uint_t)+5, param);
    maxx -= minx;
    maxy -= miny;
    rand_seed(timer_get32());
    ww = (sint_t)dest->position.ww;
    hh = (sint_t)dest->position.hh;
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    byte_t* ptr;
    byte_t* line;
    sIMAGE* dest;
    uint_t  ww, hh, ii;

    CR_NOUSE(nouse);
    CR_NOUSE(param);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    line = dest->data;
    ww = dest->position.ww;
    hh = dest->position.hh;
    for (uint_t yy = 0; yy < hh; yy++) {
        ptr = line;
        for (uint_t xx = 0; xx < ww; xx++) {
            ii = rgb2light(ptr[2], ptr[1], ptr[0]);
            ptr[0] = (byte_t)ii;
            ptr[1] = (byte_t)ii;
            ptr[2] = (byte_t)ii;
            ptr += sizeof(int32u);
        }
        line += dest->bpl;
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    byte_t* ptr;
    byte_t* line;
    sIMAGE* dest;
    uint_t  gate;
    uint_t  ww, hh;

    CR_NOUSE(nouse);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    line = dest->data;
    ww = dest->position.ww;
    hh = dest->position.hh;
    gate = xml_attr_intxU("gate", 127, param);
    if (param->found)
    {
        /* 有参数使用指定参数 */
        for (uint_t yy = 0; yy < hh; yy++) {
            ptr = line;
            for (uint_t xx = 0; xx < ww; xx++) {
                if (ptr[0] > gate)
                    ptr[0] = 0xFF;
                else
                    ptr[0] = 0x00;
                ptr[1] = ptr[0];
                ptr[2] = ptr[0];
                ptr += sizeof(int32u);
            }
            line += dest->bpl;
        }
    }
    else
    {
        int64u  total = 0;

        /* 没有参数使用平均值 */
        for (uint_t yy = 0; yy < hh; yy++) {
            ptr = line;
            for (uint_t xx = 0; xx < ww; xx++) {
                total += ptr[0];
                ptr += sizeof(int32u);
            }
            line += dest->bpl;
        }
        total /= ww;
        total /= hh;
        gate = (uint_t)total;
        line = dest->data;
        for (uint_t yy = 0; yy < hh; yy++) {
            ptr = line;
            for (uint_t xx = 0; xx < ww; xx++) {
                if (ptr[0] > gate)
                    ptr[0] = 0xFF;
                else
                    ptr[0] = 0x00;
                ptr[1] = ptr[0];
                ptr[2] = ptr[0];
                ptr += sizeof(int32u);
            }
            line += dest->bpl;
        }
    }
    return (TRUE);
}

/*
---------------------------------------
    卷积运算 (3 x 3)
---------------------------------------
*/
static byte_t*
conv3x3_main (
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sint_t    mat[9]
    )
{
    byte_t* ptr;
    byte_t* dst;
    byte_t* prev;
    byte_t* curt;
    byte_t* next;
    sint_t  csum;
    uint_t  xx, yy;
    uint_t  ww, hh;

    /* 不处理边框 */
    ww = src->position.ww;
    hh = src->position.hh;
    if (ww < 3 || hh < 3)
        return (NULL);
    ww -= 2;
    hh -= 2;
    dst = (byte_t*)mem_malloc(ww * hh * 4);
    if (dst == NULL)
        return (NULL);
    ptr = dst;

    /* 求矩阵的和 */
    for (csum = mat[0], xx = 1; xx < 9; xx++)
        csum += mat[xx];
    if (csum <= 0)
        csum = 1;

    /* 开始计算卷积 */
    prev = src->data + 4;
    curt = prev + src->bpl;
    next = curt + src->bpl;
    for (yy = 0; yy < hh; yy++)
    {
        for (xx = 0; xx < ww; xx++)
        {
            sint_t  bb, gg, rr;

            /* 第一排 */
            bb  = prev[(xx-1)*4+0] * mat[0];
            gg  = prev[(xx-1)*4+1] * mat[0];
            rr  = prev[(xx-1)*4+2] * mat[0];
            bb += prev[(xx+0)*4+0] * mat[1];
            gg += prev[(xx+0)*4+1] * mat[1];
            rr += prev[(xx+0)*4+2] * mat[1];
            bb += prev[(xx+1)*4+0] * mat[2];
            gg += prev[(xx+1)*4+1] * mat[2];
            rr += prev[(xx+1)*4+2] * mat[2];

            /* 第二排 */
            bb += curt[(xx-1)*4+0] * mat[3];
            gg += curt[(xx-1)*4+1] * mat[3];
            rr += curt[(xx-1)*4+2] * mat[3];
            bb += curt[(xx+0)*4+0] * mat[4];
            gg += curt[(xx+0)*4+1] * mat[4];
            rr += curt[(xx+0)*4+2] * mat[4];
            bb += curt[(xx+1)*4+0] * mat[5];
            gg += curt[(xx+1)*4+1] * mat[5];
            rr += curt[(xx+1)*4+2] * mat[5];

            /* 第三排 */
            bb += next[(xx-1)*4+0] * mat[6];
            gg += next[(xx-1)*4+1] * mat[6];
            rr += next[(xx-1)*4+2] * mat[6];
            bb += next[(xx+0)*4+0] * mat[7];
            gg += next[(xx+0)*4+1] * mat[7];
            rr += next[(xx+0)*4+2] * mat[7];
            bb += next[(xx+1)*4+0] * mat[8];
            gg += next[(xx+1)*4+1] * mat[8];
            rr += next[(xx+1)*4+2] * mat[8];

            /* 中心-B */
            bb /= csum;
            if (bb < 0)
                bb = 0;
            else
            if (bb > 255)
                bb = 255;
            *ptr++ = (byte_t)bb;

            /* 中心-G */
            gg /= csum;
            if (gg < 0)
                gg = 0;
            else
            if (gg > 255)
                gg = 255;
            *ptr++ = (byte_t)gg;

            /* 中心-R */
            rr /= csum;
            if (rr < 0)
                rr = 0;
            else
            if (rr > 255)
                rr = 255;
            *ptr++ = (byte_t)rr;

            /* 中心-A */
            *ptr++ = curt[xx * 4 + 3];
        }
        prev += src->bpl;
        curt += src->bpl;
        next += src->bpl;
    }
    return (dst);
}

/*
---------------------------------------
    形态运算 (3 x 3)
---------------------------------------
*/
static byte_t*
conv3x3_form (
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sint_t    mat[9],
  __CR_IN__ bool_t          expand
    )
{
    byte_t* ptr;
    byte_t* dst;
    byte_t* prev;
    byte_t* curt;
    byte_t* next;
    sint_t  csum;
    uint_t  xx, yy;
    uint_t  ww, hh;

    /* 不处理边框 */
    ww = src->position.ww;
    hh = src->position.hh;
    if (ww < 3 || hh < 3)
        return (NULL);
    ww -= 2;
    hh -= 2;
    dst = (byte_t*)mem_malloc(ww * hh * 4);
    if (dst == NULL)
        return (NULL);
    ptr = dst;

    /* 求矩阵的和 */
    for (csum = mat[0], xx = 1; xx < 9; xx++)
        csum += mat[xx];

    /* 开始计算卷积 */
    prev = src->data + 4;
    curt = prev + src->bpl;
    next = curt + src->bpl;
    for (yy = 0; yy < hh; yy++)
    {
        for (xx = 0; xx < ww; xx++)
        {
            sint_t  bb, gg, rr;

            /* 第一排 */
            bb  = prev[(xx-1)*4+0] & mat[0];
            gg  = prev[(xx-1)*4+1] & mat[0];
            rr  = prev[(xx-1)*4+2] & mat[0];
            bb += prev[(xx+0)*4+0] & mat[1];
            gg += prev[(xx+0)*4+1] & mat[1];
            rr += prev[(xx+0)*4+2] & mat[1];
            bb += prev[(xx+1)*4+0] & mat[2];
            gg += prev[(xx+1)*4+1] & mat[2];
            rr += prev[(xx+1)*4+2] & mat[2];

            /* 第二排 */
            bb += curt[(xx-1)*4+0] & mat[3];
            gg += curt[(xx-1)*4+1] & mat[3];
            rr += curt[(xx-1)*4+2] & mat[3];
            bb += curt[(xx+0)*4+0] & mat[4];
            gg += curt[(xx+0)*4+1] & mat[4];
            rr += curt[(xx+0)*4+2] & mat[4];
            bb += curt[(xx+1)*4+0] & mat[5];
            gg += curt[(xx+1)*4+1] & mat[5];
            rr += curt[(xx+1)*4+2] & mat[5];

            /* 第三排 */
            bb += next[(xx-1)*4+0] & mat[6];
            gg += next[(xx-1)*4+1] & mat[6];
            rr += next[(xx-1)*4+2] & mat[6];
            bb += next[(xx+0)*4+0] & mat[7];
            gg += next[(xx+0)*4+1] & mat[7];
            rr += next[(xx+0)*4+2] & mat[7];
            bb += next[(xx+1)*4+0] & mat[8];
            gg += next[(xx+1)*4+1] & mat[8];
            rr += next[(xx+1)*4+2] & mat[8];

            /* 中心-B */
            if (expand) {
                if (bb == 0)
                    *ptr++ = 0x00;
                else
                    *ptr++ = 0xFF;
            }
            else {
                if (bb == csum)
                    *ptr++ = 0xFF;
                else
                    *ptr++ = 0x00;
            }

            /* 中心-G */
            if (expand) {
                if (gg == 0)
                    *ptr++ = 0x00;
                else
                    *ptr++ = 0xFF;
            }
            else {
                if (gg == csum)
                    *ptr++ = 0xFF;
                else
                    *ptr++ = 0x00;
            }

            /* 中心-R */
            if (expand) {
                if (rr == 0)
                    *ptr++ = 0x00;
                else
                    *ptr++ = 0xFF;
            }
            else {
                if (rr == csum)
                    *ptr++ = 0xFF;
                else
                    *ptr++ = 0x00;
            }

            /* 中心-A */
            *ptr++ = curt[xx * 4 + 3];
        }
        prev += src->bpl;
        curt += src->bpl;
        next += src->bpl;
    }
    return (dst);
}

/*
---------------------------------------
    回拷图片
---------------------------------------
*/
static void_t
conv3x3_back (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const byte_t*   src
    )
{
    uint_t  hh;
    leng_t  bpl;
    byte_t* ptr;

    hh = dst->position.hh - 2;
    bpl = (dst->position.ww - 2) * 4;
    ptr = dst->data + dst->bpl + 4;
    for (; hh != 0; hh--) {
        mem_cpy(ptr, src, bpl);
        src += bpl;
        ptr += dst->bpl;
    }
}

/*
---------------------------------------
    图片卷积运算 (3 x 3)
---------------------------------------
*/
static bool_t
image_conv3x3 (
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    uint_t  idx;
    ansi_t* str;
    byte_t* temp;
    sIMAGE* dest;
    sint_t  mat[9];

    CR_NOUSE(nouse);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    str = xml_attr_bufferU("mat", param);
    if (str == NULL ||
        str2lstA((uint_t*)mat, 9, str, "[],") == NULL) {
        for (idx = 0; idx < 9; idx++)
            mat[idx] = 1;
    }
    temp = conv3x3_main(dest, mat);
    if (temp == NULL)
        return (TRUE);
    conv3x3_back(dest, temp);
    mem_free(temp);
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    byte_t* gx;
    byte_t* gy;
    leng_t  idx;
    leng_t  size;
    sIMAGE* dest;

    CR_NOUSE(nouse);
    CR_NOUSE(param);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    gx = conv3x3_main(dest, s_sobel_matx);
    if (gx == NULL)
        return (TRUE);
    gy = conv3x3_main(dest, s_sobel_maty);
    if (gy == NULL) {
        mem_free(gx);
        return (TRUE);
    }

    /* 计算梯度图 */
    size  = dest->position.ww - 2;
    size *= dest->position.hh - 2;
    size *= sizeof(int32u);
    for (idx = 0; idx < size; idx += 4)
    {
        sint_t  tx, ty;

        /* 分量-B */
        tx = gx[idx + 0];
        ty = gy[idx + 0];
        tx *= tx;
        ty *= ty;
        ty += tx;
        tx = (sint_t)(sqrtf((float)ty) + 0.5f);
        if (tx < 0)
            tx = 0;
        else
        if (tx > 255)
            tx = 255;
        gx[idx + 0] = (byte_t)tx;

        /* 分量-G */
        tx = gx[idx + 1];
        ty = gy[idx + 1];
        tx *= tx;
        ty *= ty;
        ty += tx;
        tx = (sint_t)(sqrtf((float)ty) + 0.5f);
        if (tx < 0)
            tx = 0;
        else
        if (tx > 255)
            tx = 255;
        gx[idx + 1] = (byte_t)tx;

        /* 分量-R */
        tx = gx[idx + 2];
        ty = gy[idx + 2];
        tx *= tx;
        ty *= ty;
        ty += tx;
        tx = (sint_t)(sqrtf((float)ty) + 0.5f);
        if (tx < 0)
            tx = 0;
        else
        if (tx > 255)
            tx = 255;
        gx[idx + 2] = (byte_t)tx;
    }
    conv3x3_back(dest, gx);
    mem_free(gy);
    mem_free(gx);
    return (TRUE);
}

/*
---------------------------------------
    图片门限过滤
---------------------------------------
*/
static bool_t
image_cut_down (
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    byte_t* ptr;
    byte_t* line;
    sIMAGE* dest;
    uint_t  gate;
    uint_t  ww, hh;

    CR_NOUSE(nouse);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    line = dest->data;
    ww = dest->position.ww;
    hh = dest->position.hh;
    gate = xml_attr_intxU("gate", 97, param);
    if (param->found)
    {
        /* 有参数使用指定参数 */
        for (uint_t yy = 0; yy < hh; yy++) {
            ptr = line;
            for (uint_t xx = 0; xx < ww; xx++) {
                if (ptr[0] <= gate) {
                    ptr[0] = 0;
                    ptr[1] = 0;
                    ptr[2] = 0;
                }
                ptr += sizeof(int32u);
            }
            line += dest->bpl;
        }
    }
    else
    {
        int64u  total = 0;

        /* 没有参数使用平均值 */
        for (uint_t yy = 0; yy < hh; yy++) {
            ptr = line;
            for (uint_t xx = 0; xx < ww; xx++) {
                total += ptr[0];
                ptr += sizeof(int32u);
            }
            line += dest->bpl;
        }
        total /= ww;
        total /= hh;
        gate = (uint_t)total;
        line = dest->data;
        for (uint_t yy = 0; yy < hh; yy++) {
            ptr = line;
            for (uint_t xx = 0; xx < ww; xx++) {
                if (ptr[0] <= gate) {
                    ptr[0] = 0;
                    ptr[1] = 0;
                    ptr[2] = 0;
                }
                ptr += sizeof(int32u);
            }
            line += dest->bpl;
        }
    }
    return (TRUE);
}

/*
---------------------------------------
    RGB 转到 HSL
---------------------------------------
*/
static void_t
rgb2hsl (
  __CR_OT__ sint_t*         dst,
  __CR_IN__ const byte_t*   src
    )
{
    sint_t  dlt;
    sint_t  bb = src[0];
    sint_t  gg = src[1];
    sint_t  rr = src[2];
    sint_t  max = bb;
    sint_t  min = bb;

    if (max < gg) max = gg;
    if (min > gg) min = gg;
    if (max < rr) max = rr;
    if (min > rr) min = rr;
    dlt = max - min;

    /* 分量-H */
    if (dlt == 0) {
        dst[0] = 0;
    }
    else
    if (max == rr) {
        dst[0] = (60 * (gg - bb)) / dlt;
        if (gg < bb)
            dst[0] += 360;
    }
    else
    if (max == gg) {
        dst[0] = (60 * (bb - rr)) / dlt + 120;
    }
    else {
        dst[0] = (60 * (rr - gg)) / dlt + 240;
    }

    /* 分量-L */
    dst[2] = (min + max) / 2;

    /* 分量-S */
    if (dst[2] == 0 || dlt == 0) {
        dst[1] = 0;
    }
    else
    if (dst[2] <= 127) {
        dst[1] = 255 * dlt / (2 * dst[2]);
    }
    else {
        dst[1] = 255 * dlt / (2 * 255 - 2 * dst[2]);
    }
}

/* 标准的色相表 */
static const sint_t _rom_ s_hue[12] =
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
static const byte_t _rom_ s_color[13 * 3] =
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    ansi_t* str;
    byte_t* ptr;
    byte_t* line;
    sIMAGE* dest;
    uint_t  idx, ww, hh;
    sint_t  gate_s, gate_l;
    sint_t  hsl[3], hue[12];

    CR_NOUSE(nouse);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    line = dest->data;
    ww = dest->position.ww;
    hh = dest->position.hh;
    gate_s = (sint_t)xml_attr_intxU("gate_sat", 192, param);
    gate_l = (sint_t)xml_attr_intxU("gate_lit", 128, param);
    str = xml_attr_bufferU("table", param);
    if (str == NULL ||
        str2lstA((uint_t*)hue, 12, str, "[],") == NULL)
        mem_cpy(hue, s_hue, sizeof(hue));
    for (uint_t yy = 0; yy < hh; yy++) {
        ptr = line;
        for (uint_t xx = 0; xx < ww; xx++) {
            rgb2hsl(hsl, ptr);
            if (hsl[0] == 0 || hsl[1] < gate_s) {
                if (hsl[2] < gate_l) {
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
            else {
                for (idx = 0; idx < cntsof(hue); idx++) {
                    if (hsl[0] < hue[idx])
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    byte_t* ptr;
    byte_t* line;
    sIMAGE* dest;
    fp32_t  fval;
    uint_t  ww, hh;
    fp32_t  fr, fg, fb;

    CR_NOUSE(nouse);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    line = dest->data;
    ww = dest->position.ww;
    hh = dest->position.hh;
    fr = xml_attr_fp32U("red", 2.0f, param);
    fg = xml_attr_fp32U("green", 2.0f, param);
    fb = xml_attr_fp32U("blue", 2.0f, param);
    for (uint_t yy = 0; yy < hh; yy++) {
        ptr = line;
        for (uint_t xx = 0; xx < ww; xx++) {
            fval = fb * ptr[0];
            if (fval >= 255.0f)
                ptr[0] = 0xFF;
            else
                ptr[0] = (byte_t)fval;
            fval = fg * ptr[1];
            if (fval >= 255.0f)
                ptr[1] = 0xFF;
            else
                ptr[1] = (byte_t)fval;
            fval = fr * ptr[2];
            if (fval >= 255.0f)
                ptr[2] = 0xFF;
            else
                ptr[2] = (byte_t)fval;
            ptr += sizeof(int32u);
        }
        line += dest->bpl;
    }
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    byte_t* ptr;
    byte_t* line;
    ansi_t* data;
    sIMAGE* dest;
    uint_t  ww, hh, only;
    leng_t  ssize, dsize;
    byte_t  src[LOOKUP_MAX * 3];
    byte_t  dst[LOOKUP_MAX * 3];

    CR_NOUSE(nouse);
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
        ptr = line;
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
  __CR_UU__ void_t*     nouse,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    uint_t  idx;
    ansi_t* str;
    byte_t* temp;
    sIMAGE* dest;
    sint_t  mat[9];
    uint_t  expand;

    CR_NOUSE(nouse);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    str = xml_attr_bufferU("mat", param);
    if (str == NULL ||
        str2lstA((uint_t*)mat, 9, str, "[],") == NULL) {
        for (idx = 0; idx < 9; idx++)
            mat[idx] = 1;
    }
    for (idx = 0; idx < 9; idx++) {
        if (mat[idx])
            mat[idx] = 0xFF;
        else
            mat[idx] = 0x00;
    }
    expand = xml_attr_intxU("exp", TRUE, param);
    temp = conv3x3_form(dest, mat, !!expand);
    if (temp == NULL)
        return (TRUE);
    conv3x3_back(dest, temp);
    mem_free(temp);
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
    { NULL, NULL },
};
