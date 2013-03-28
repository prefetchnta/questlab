
#include "../QstLibs/QstLibs.h"

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
    uint_t  ww, hh, ii;

    CR_NOUSE(nouse);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    line = dest->data;
    ww = dest->position.ww;
    hh = dest->position.hh;
    gate = xml_attr_intxU("gate", 127, param);
    for (uint_t yy = 0; yy < hh; yy++) {
        ptr = line;
        for (uint_t xx = 0; xx < ww; xx++) {
            ii = rgb2light(ptr[2], ptr[1], ptr[0]);
            ii = (ii > gate) ? 255 : 0;
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
    ansi_t* str;
    byte_t* ptr;
    byte_t* temp;
    byte_t* prev;
    byte_t* curt;
    byte_t* next;
    sIMAGE* dest;
    sint_t  csum;
    sint_t  mat[9];
    uint_t  xx, yy;
    uint_t  ww, hh;

    CR_NOUSE(nouse);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    ww = dest->position.ww;
    hh = dest->position.hh;
    if (ww < 3 || hh < 3)
        return (TRUE);
    ww -= 2;
    hh -= 2;
    temp = (byte_t*)mem_malloc(ww * hh * 4);
    if (temp == NULL)
        return (TRUE);
    ptr = temp;

    /* 输入参数 */
    str = xml_attr_bufferU("mat", param);
    if (str == NULL ||
        str2lstA((uint_t*)mat, 9, str, "[],") == NULL) {
        for (xx = 0; xx < 9; xx++)
            mat[xx] = 1;
        csum = 9;
    }
    else {
        for (csum = mat[0], xx = 1; xx < 9; xx++)
            csum += mat[xx];
        if (csum <= 0)
            csum = 1;
    }

    /* 计算卷积 */
    prev = dest->data + 4;
    curt = prev + dest->bpl;
    next = curt + dest->bpl;
    for (yy = 0; yy < hh; yy++)
    {
        for (xx = 0; xx < ww; xx++)
        {
            sint_t  bb, gg, rr;

            /* 第一排 */
            bb  = (sint_t)prev[(xx-1)*4+0] * mat[0];
            gg  = (sint_t)prev[(xx-1)*4+1] * mat[0];
            rr  = (sint_t)prev[(xx-1)*4+2] * mat[0];
            bb += (sint_t)prev[(xx+0)*4+0] * mat[1];
            gg += (sint_t)prev[(xx+0)*4+1] * mat[1];
            rr += (sint_t)prev[(xx+0)*4+2] * mat[1];
            bb += (sint_t)prev[(xx+1)*4+0] * mat[2];
            gg += (sint_t)prev[(xx+1)*4+1] * mat[2];
            rr += (sint_t)prev[(xx+1)*4+2] * mat[2];

            /* 第二排 */
            bb += (sint_t)curt[(xx-1)*4+0] * mat[3];
            gg += (sint_t)curt[(xx-1)*4+1] * mat[3];
            rr += (sint_t)curt[(xx-1)*4+2] * mat[3];
            bb += (sint_t)curt[(xx+0)*4+0] * mat[4];
            gg += (sint_t)curt[(xx+0)*4+1] * mat[4];
            rr += (sint_t)curt[(xx+0)*4+2] * mat[4];
            bb += (sint_t)curt[(xx+1)*4+0] * mat[5];
            gg += (sint_t)curt[(xx+1)*4+1] * mat[5];
            rr += (sint_t)curt[(xx+1)*4+2] * mat[5];

            /* 第三排 */
            bb += (sint_t)next[(xx-1)*4+0] * mat[6];
            gg += (sint_t)next[(xx-1)*4+1] * mat[6];
            rr += (sint_t)next[(xx-1)*4+2] * mat[6];
            bb += (sint_t)next[(xx+0)*4+0] * mat[7];
            gg += (sint_t)next[(xx+0)*4+1] * mat[7];
            rr += (sint_t)next[(xx+0)*4+2] * mat[7];
            bb += (sint_t)next[(xx+1)*4+0] * mat[8];
            gg += (sint_t)next[(xx+1)*4+1] * mat[8];
            rr += (sint_t)next[(xx+1)*4+2] * mat[8];

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
        prev += dest->bpl;
        curt += dest->bpl;
        next += dest->bpl;
    }

    /* 回拷图片 */
    ww *= sizeof(int32u);
    curt = dest->data + dest->bpl + 4;
    for (ptr = temp, yy = 0; yy < hh; yy++) {
        mem_cpy(curt, ptr, ww);
        ptr  += ww;
        curt += dest->bpl;
    }
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
    { NULL, NULL },
};
