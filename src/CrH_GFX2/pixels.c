/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-29  */
/*     #######          ###    ###      [GFX2]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 像素绘制函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "blit.h"
#include "blends.h"

/*****************************************************************************/
/*                                 整数坐标                                  */
/*****************************************************************************/

/*
=======================================
    正向像素绘制
=======================================
*/
#undef  CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_SET
#define CR_NAME_PIXEL(n) pixel_set##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    反向像素绘制
=======================================
*/
#define CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_SET
#define CR_NAME_PIXEL(n) pixel_set##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    正向像素与绘制
=======================================
*/
#undef  CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_AND
#define CR_NAME_PIXEL(n) pixel_and##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    反向像素与绘制
=======================================
*/
#define CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_AND
#define CR_NAME_PIXEL(n) pixel_and##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    正向像素或绘制
=======================================
*/
#undef  CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_ORR
#define CR_NAME_PIXEL(n) pixel_orr##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    反向像素或绘制
=======================================
*/
#define CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_ORR
#define CR_NAME_PIXEL(n) pixel_orr##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    正向像素非绘制
=======================================
*/
#undef  CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_NOT
#define CR_NAME_PIXEL(n) pixel_not##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    反向像素非绘制
=======================================
*/
#define CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_NOT
#define CR_NAME_PIXEL(n) pixel_not##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    正向像素异或绘制
=======================================
*/
#undef  CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_XOR
#define CR_NAME_PIXEL(n) pixel_xor##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    反向像素异或绘制
=======================================
*/
#define CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_XOR
#define CR_NAME_PIXEL(n) pixel_xor##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    正向像素饱和加法绘制
=======================================
*/
#undef  CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_ADD
#define CR_NAME_PIXEL(n) pixel_add##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    反向像素饱和加法绘制
=======================================
*/
#define CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_ADD
#define CR_NAME_PIXEL(n) pixel_add##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    正向像素饱和减法绘制
=======================================
*/
#undef  CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_SUB
#define CR_NAME_PIXEL(n) pixel_sub##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    反向像素饱和减法绘制
=======================================
*/
#define CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_SUB
#define CR_NAME_PIXEL(n) pixel_sub##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    正向像素插值混合绘制
=======================================
*/
#undef  CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_ALP
#define CR_NAME_PIXEL(n) pixel_lrp##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*
=======================================
    反向像素插值混合绘制
=======================================
*/
#define CR_PIXEL_FLIP
#define CR_BLT_MODE CR_BLT_ALP
#define CR_NAME_PIXEL(n) pixel_lrp##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_PIXEL
#undef  CR_PIXEL_FLIP

/*****************************************************************************/
/*                                 浮点坐标                                  */
/*****************************************************************************/

/*
=======================================
    抗锯齿读点 (fmt = 08)
=======================================
*/
CR_API byte_t
dot_wu_get08 (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y
    )
{
    byte_t* ptr;
    leng_t  bpl;
    bool_t  over;
    fp32_t  pix[4];
    fp32_t  ax, ay;
    sint_t  bx, by;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax = x - bx;
    ay = y - by;

    /* 获取四个点的值 */
    if (!dst->gdi) {
        bpl = dst->bpl;
        ptr = pixel_addr1(dst, bx, by);
    }
    else {
        bpl = 0 - dst->bpl;
        ptr = pixel_addr1(dst, bx, dst->clip_win.y2 - by);
    }
    pix[0] = ptr[0];
    if (bx < dst->clip_win.x2) {
        over = FALSE;
        pix[1] = ptr[1];
    }
    else {
        over = TRUE;
        pix[1] = pix[0];
    }
    if (by < dst->clip_win.y2) {
        ptr += bpl;
        pix[2] = ptr[0];
        pix[3] = over ? pix[2] : ptr[1];
    }
    else {
        pix[2] = pix[0];
        pix[3] = pix[1];
    }

    /* 双线性插值 */
    pix[0] = pix[0] + (pix[1] - pix[0]) * ax;
    pix[2] = pix[2] + (pix[3] - pix[2]) * ax;
    pix[0] = pix[0] + (pix[2] - pix[0]) * ay + 0.5f;
    return ((byte_t)pix[0]);
}

/*
=======================================
    抗锯齿读点 (fmt = 12)
=======================================
*/
CR_API int16u
dot_wu_get12 (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y
    )
{
    int16u* ptr;
    uint_t  idx;
    leng_t  bpl;
    bool_t  over;
    int16u  pix[4];
    fp32_t  aaa[4];
    fp32_t  rrr[4];
    fp32_t  ggg[4];
    fp32_t  bbb[4];
    fp32_t  ax, ay;
    sint_t  bx, by;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax = x - bx;
    ay = y - by;

    /* 获取四个点的值 */
    if (!dst->gdi) {
        bpl = dst->bpl;
        ptr = pixel_addr2(dst, bx, by);
    }
    else {
        bpl = 0 - dst->bpl;
        ptr = pixel_addr2(dst, bx, dst->clip_win.y2 - by);
    }
    pix[0] = ptr[0];
    if (bx < dst->clip_win.x2) {
        over = FALSE;
        pix[1] = ptr[1];
    }
    else {
        over = TRUE;
        pix[1] = pix[0];
    }
    if (by < dst->clip_win.y2) {
        ptr += bpl;
        pix[2] = ptr[0];
        pix[3] = over ? pix[2] : ptr[1];
    }
    else {
        pix[2] = pix[0];
        pix[3] = pix[1];
    }

    /* 拆分颜色分量 */
    for (idx = 0; idx < 4; idx++) {
        pix[idx] = WORD_LE(pix[idx]);
        bbb[idx] = (fp32_t)((pix[idx] >>  0) & 0x000F);
        ggg[idx] = (fp32_t)((pix[idx] >>  4) & 0x000F);
        rrr[idx] = (fp32_t)((pix[idx] >>  8) & 0x000F);
        aaa[idx] = (fp32_t)((pix[idx] >> 12));
    }

    /* 双线性插值 */
    bbb[0] = bbb[0] + (bbb[1] - bbb[0]) * ax;
    bbb[2] = bbb[2] + (bbb[3] - bbb[2]) * ax;
    bbb[0] = bbb[0] + (bbb[2] - bbb[0]) * ay + 0.5f;
    ggg[0] = ggg[0] + (ggg[1] - ggg[0]) * ax;
    ggg[2] = ggg[2] + (ggg[3] - ggg[2]) * ax;
    ggg[0] = ggg[0] + (ggg[2] - ggg[0]) * ay + 0.5f;
    rrr[0] = rrr[0] + (rrr[1] - rrr[0]) * ax;
    rrr[2] = rrr[2] + (rrr[3] - rrr[2]) * ax;
    rrr[0] = rrr[0] + (rrr[2] - rrr[0]) * ay + 0.5f;
    aaa[0] = aaa[0] + (aaa[1] - aaa[0]) * ax;
    aaa[2] = aaa[2] + (aaa[3] - aaa[2]) * ax;
    aaa[0] = aaa[0] + (aaa[2] - aaa[0]) * ay + 0.5f;

    /* 合成结果像素 */
    pix[0] = (byte_t)bbb[0];
    pix[1] = (byte_t)ggg[0];
    pix[2] = (byte_t)rrr[0];
    pix[3] = (byte_t)aaa[0];
    pix[1] <<= 4;
    pix[2] <<= 8;
    pix[3] <<= 12;
    return (WORD_LE(pix[0] | pix[1] | pix[2] | pix[3]));
}

/*
=======================================
    抗锯齿读点 (fmt = 15)
=======================================
*/
CR_API int16u
dot_wu_get15 (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y
    )
{
    int16u* ptr;
    uint_t  idx;
    leng_t  bpl;
    bool_t  over;
    int16u  pix[4];
    fp32_t  aaa[4];
    fp32_t  rrr[4];
    fp32_t  ggg[4];
    fp32_t  bbb[4];
    fp32_t  ax, ay;
    sint_t  bx, by;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax = x - bx;
    ay = y - by;

    /* 获取四个点的值 */
    if (!dst->gdi) {
        bpl = dst->bpl;
        ptr = pixel_addr2(dst, bx, by);
    }
    else {
        bpl = 0 - dst->bpl;
        ptr = pixel_addr2(dst, bx, dst->clip_win.y2 - by);
    }
    pix[0] = ptr[0];
    if (bx < dst->clip_win.x2) {
        over = FALSE;
        pix[1] = ptr[1];
    }
    else {
        over = TRUE;
        pix[1] = pix[0];
    }
    if (by < dst->clip_win.y2) {
        ptr += bpl;
        pix[2] = ptr[0];
        pix[3] = over ? pix[2] : ptr[1];
    }
    else {
        pix[2] = pix[0];
        pix[3] = pix[1];
    }

    /* 拆分颜色分量 */
    for (idx = 0; idx < 4; idx++) {
        pix[idx] = WORD_LE(pix[idx]);
        bbb[idx] = (fp32_t)((pix[idx] >>  0) & 0x001F);
        ggg[idx] = (fp32_t)((pix[idx] >>  5) & 0x001F);
        rrr[idx] = (fp32_t)((pix[idx] >> 10) & 0x001F);
        aaa[idx] = (fp32_t)((pix[idx] >> 15));
    }

    /* 双线性插值 */
    bbb[0] = bbb[0] + (bbb[1] - bbb[0]) * ax;
    bbb[2] = bbb[2] + (bbb[3] - bbb[2]) * ax;
    bbb[0] = bbb[0] + (bbb[2] - bbb[0]) * ay + 0.5f;
    ggg[0] = ggg[0] + (ggg[1] - ggg[0]) * ax;
    ggg[2] = ggg[2] + (ggg[3] - ggg[2]) * ax;
    ggg[0] = ggg[0] + (ggg[2] - ggg[0]) * ay + 0.5f;
    rrr[0] = rrr[0] + (rrr[1] - rrr[0]) * ax;
    rrr[2] = rrr[2] + (rrr[3] - rrr[2]) * ax;
    rrr[0] = rrr[0] + (rrr[2] - rrr[0]) * ay + 0.5f;
    aaa[0] = aaa[0] + (aaa[1] - aaa[0]) * ax;
    aaa[2] = aaa[2] + (aaa[3] - aaa[2]) * ax;
    aaa[0] = aaa[0] + (aaa[2] - aaa[0]) * ay + 0.5f;

    /* 合成结果像素 */
    pix[0] = (byte_t)bbb[0];
    pix[1] = (byte_t)ggg[0];
    pix[2] = (byte_t)rrr[0];
    pix[3] = (byte_t)aaa[0];
    pix[1] <<= 5;
    pix[2] <<= 10;
    pix[3] <<= 15;
    return (WORD_LE(pix[0] | pix[1] | pix[2] | pix[3]));
}

/*
=======================================
    抗锯齿读点 (fmt = 16)
=======================================
*/
CR_API int16u
dot_wu_get16 (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y
    )
{
    int16u* ptr;
    uint_t  idx;
    leng_t  bpl;
    bool_t  over;
    int16u  pix[4];
    fp32_t  rrr[4];
    fp32_t  ggg[4];
    fp32_t  bbb[4];
    fp32_t  ax, ay;
    sint_t  bx, by;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax = x - bx;
    ay = y - by;

    /* 获取四个点的值 */
    if (!dst->gdi) {
        bpl = dst->bpl;
        ptr = pixel_addr2(dst, bx, by);
    }
    else {
        bpl = 0 - dst->bpl;
        ptr = pixel_addr2(dst, bx, dst->clip_win.y2 - by);
    }
    pix[0] = ptr[0];
    if (bx < dst->clip_win.x2) {
        over = FALSE;
        pix[1] = ptr[1];
    }
    else {
        over = TRUE;
        pix[1] = pix[0];
    }
    if (by < dst->clip_win.y2) {
        ptr += bpl;
        pix[2] = ptr[0];
        pix[3] = over ? pix[2] : ptr[1];
    }
    else {
        pix[2] = pix[0];
        pix[3] = pix[1];
    }

    /* 拆分颜色分量 */
    for (idx = 0; idx < 4; idx++) {
        pix[idx] = WORD_LE(pix[idx]);
        bbb[idx] = (fp32_t)((pix[idx] >>  0) & 0x001F);
        ggg[idx] = (fp32_t)((pix[idx] >>  5) & 0x003F);
        rrr[idx] = (fp32_t)((pix[idx] >> 11) & 0x001F);
    }

    /* 双线性插值 */
    bbb[0] = bbb[0] + (bbb[1] - bbb[0]) * ax;
    bbb[2] = bbb[2] + (bbb[3] - bbb[2]) * ax;
    bbb[0] = bbb[0] + (bbb[2] - bbb[0]) * ay + 0.5f;
    ggg[0] = ggg[0] + (ggg[1] - ggg[0]) * ax;
    ggg[2] = ggg[2] + (ggg[3] - ggg[2]) * ax;
    ggg[0] = ggg[0] + (ggg[2] - ggg[0]) * ay + 0.5f;
    rrr[0] = rrr[0] + (rrr[1] - rrr[0]) * ax;
    rrr[2] = rrr[2] + (rrr[3] - rrr[2]) * ax;
    rrr[0] = rrr[0] + (rrr[2] - rrr[0]) * ay + 0.5f;

    /* 合成结果像素 */
    pix[0] = (byte_t)bbb[0];
    pix[1] = (byte_t)ggg[0];
    pix[2] = (byte_t)rrr[0];
    pix[1] <<= 5;
    pix[2] <<= 11;
    return (WORD_LE(pix[0] | pix[1] | pix[2]));
}

/*
=======================================
    抗锯齿读点 (fmt = 24)
=======================================
*/
CR_API int32u
dot_wu_get24 (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y
    )
{
    byte_t* ptr;
    leng_t  bpl;
    bool_t  over;
    int32u  rett;
    fp32_t  rrr[4];
    fp32_t  ggg[4];
    fp32_t  bbb[4];
    fp32_t  ax, ay;
    sint_t  bx, by;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax = x - bx;
    ay = y - by;

    /* 获取四个点的值 */
    if (!dst->gdi) {
        bpl = dst->bpl;
        ptr = pixel_addr3(dst, bx, by);
    }
    else {
        bpl = 0 - dst->bpl;
        ptr = pixel_addr3(dst, bx, dst->clip_win.y2 - by);
    }
    bbb[0] = ptr[0];
    ggg[0] = ptr[1];
    rrr[0] = ptr[2];
    if (bx < dst->clip_win.x2) {
        over = FALSE;
        bbb[1] = ptr[3];
        ggg[1] = ptr[4];
        rrr[1] = ptr[5];
    }
    else {
        over = TRUE;
        bbb[1] = bbb[0];
        ggg[1] = ggg[0];
        rrr[1] = rrr[0];
    }
    if (by < dst->clip_win.y2) {
        ptr += bpl;
        bbb[2] = ptr[0];
        ggg[2] = ptr[1];
        rrr[2] = ptr[2];
        if (!over) {
            bbb[3] = ptr[3];
            ggg[3] = ptr[4];
            rrr[3] = ptr[5];
        }
        else {
            bbb[3] = bbb[2];
            ggg[3] = ggg[2];
            rrr[3] = rrr[2];
        }
    }
    else {
        bbb[2] = bbb[0];
        ggg[2] = ggg[0];
        rrr[2] = rrr[0];
        bbb[3] = bbb[1];
        ggg[3] = ggg[1];
        rrr[3] = rrr[1];
    }

    /* 双线性插值 */
    bbb[0] = bbb[0] + (bbb[1] - bbb[0]) * ax;
    bbb[2] = bbb[2] + (bbb[3] - bbb[2]) * ax;
    bbb[0] = bbb[0] + (bbb[2] - bbb[0]) * ay + 0.5f;
    ggg[0] = ggg[0] + (ggg[1] - ggg[0]) * ax;
    ggg[2] = ggg[2] + (ggg[3] - ggg[2]) * ax;
    ggg[0] = ggg[0] + (ggg[2] - ggg[0]) * ay + 0.5f;
    rrr[0] = rrr[0] + (rrr[1] - rrr[0]) * ax;
    rrr[2] = rrr[2] + (rrr[3] - rrr[2]) * ax;
    rrr[0] = rrr[0] + (rrr[2] - rrr[0]) * ay + 0.5f;

    /* 合成结果像素 */
    ptr = (byte_t*)(&rett);
    ptr[0] = (byte_t)bbb[0];
    ptr[1] = (byte_t)ggg[0];
    ptr[2] = (byte_t)rrr[0];
    ptr[3] = 0xFF;
    return (rett);
}

/*
=======================================
    抗锯齿读点 (fmt = 32)
=======================================
*/
CR_API int32u
dot_wu_get32 (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y
    )
{
    byte_t* ptr;
    leng_t  bpl;
    bool_t  over;
    int32u  rett;
    fp32_t  aaa[4];
    fp32_t  rrr[4];
    fp32_t  ggg[4];
    fp32_t  bbb[4];
    fp32_t  ax, ay;
    sint_t  bx, by;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax = x - bx;
    ay = y - by;

    /* 获取四个点的值 */
    if (!dst->gdi) {
        bpl = dst->bpl;
        ptr = (byte_t*)pixel_addr4(dst, bx, by);
    }
    else {
        bpl = 0 - dst->bpl;
        ptr = (byte_t*)pixel_addr4(dst, bx, dst->clip_win.y2 - by);
    }
    bbb[0] = ptr[0];
    ggg[0] = ptr[1];
    rrr[0] = ptr[2];
    aaa[0] = ptr[3];
    if (bx < dst->clip_win.x2) {
        over = FALSE;
        bbb[1] = ptr[4];
        ggg[1] = ptr[5];
        rrr[1] = ptr[6];
        aaa[1] = ptr[7];
    }
    else {
        over = TRUE;
        bbb[1] = bbb[0];
        ggg[1] = ggg[0];
        rrr[1] = rrr[0];
        aaa[1] = aaa[0];
    }
    if (by < dst->clip_win.y2) {
        ptr += bpl;
        bbb[2] = ptr[0];
        ggg[2] = ptr[1];
        rrr[2] = ptr[2];
        aaa[2] = ptr[3];
        if (!over) {
            bbb[3] = ptr[4];
            ggg[3] = ptr[5];
            rrr[3] = ptr[6];
            aaa[3] = ptr[7];
        }
        else {
            bbb[3] = bbb[2];
            ggg[3] = ggg[2];
            rrr[3] = rrr[2];
            aaa[3] = aaa[2];
        }
    }
    else {
        bbb[2] = bbb[0];
        ggg[2] = ggg[0];
        rrr[2] = rrr[0];
        aaa[2] = aaa[0];
        bbb[3] = bbb[1];
        ggg[3] = ggg[1];
        rrr[3] = rrr[1];
        aaa[3] = aaa[1];
    }

    /* 双线性插值 */
    bbb[0] = bbb[0] + (bbb[1] - bbb[0]) * ax;
    bbb[2] = bbb[2] + (bbb[3] - bbb[2]) * ax;
    bbb[0] = bbb[0] + (bbb[2] - bbb[0]) * ay + 0.5f;
    ggg[0] = ggg[0] + (ggg[1] - ggg[0]) * ax;
    ggg[2] = ggg[2] + (ggg[3] - ggg[2]) * ax;
    ggg[0] = ggg[0] + (ggg[2] - ggg[0]) * ay + 0.5f;
    rrr[0] = rrr[0] + (rrr[1] - rrr[0]) * ax;
    rrr[2] = rrr[2] + (rrr[3] - rrr[2]) * ax;
    rrr[0] = rrr[0] + (rrr[2] - rrr[0]) * ay + 0.5f;
    aaa[0] = aaa[0] + (aaa[1] - aaa[0]) * ax;
    aaa[2] = aaa[2] + (aaa[3] - aaa[2]) * ax;
    aaa[0] = aaa[0] + (aaa[2] - aaa[0]) * ay + 0.5f;

    /* 合成结果像素 */
    ptr = (byte_t*)(&rett);
    ptr[0] = (byte_t)bbb[0];
    ptr[1] = (byte_t)ggg[0];
    ptr[2] = (byte_t)rrr[0];
    ptr[3] = (byte_t)aaa[0];
    return (rett);
}

/*
=======================================
    抗锯齿写点 (fmt = 08)
=======================================
*/
CR_API void_t
dot_wu_set08 (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y,
  __CR_IN__ cpix_t          color
    )
{
    bool_t  over;
    sint_t  bx, by;
    fp32_t  ax1, ay1;
    fp32_t  ax2, ay2;
    byte_t  lrp0, lrp1;
    byte_t  lrp2, lrp3;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax1 = x - bx;
    ay1 = y - by;
    ax2 = 1.0f - ax1;
    ay2 = 1.0f - ay1;
    lrp0 = (byte_t)(255.0f * ax2 * ay2 + 0.5f);
    lrp1 = (byte_t)(255.0f * ax1 * ay2 + 0.5f);
    lrp2 = (byte_t)(255.0f * ax2 * ay1 + 0.5f);
    lrp3 = (byte_t)(255.0f * ax1 * ay1 + 0.5f);

    /* 输出四个点值 */
    if (!dst->gdi) {
        color.c08.lrp = lrp0;
        pixel_lrp08z(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c08.lrp = lrp1;
            pixel_lrp08z(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c08.lrp = lrp2;
            pixel_lrp08z(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c08.lrp = lrp3;
            pixel_lrp08z(dst, bx + 1, by + 1, color);
        }
    }
    else {
        color.c08.lrp = lrp0;
        pixel_lrp08n(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c08.lrp = lrp1;
            pixel_lrp08n(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c08.lrp = lrp2;
            pixel_lrp08n(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c08.lrp = lrp3;
            pixel_lrp08n(dst, bx + 1, by + 1, color);
        }
    }
}

/*
=======================================
    抗锯齿写点 (fmt = 12)
=======================================
*/
CR_API void_t
dot_wu_set12 (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y,
  __CR_IN__ cpix_t          color
    )
{
    bool_t  over;
    sint_t  bx, by;
    fp32_t  ax1, ay1;
    fp32_t  ax2, ay2;
    byte_t  lrp0, lrp1;
    byte_t  lrp2, lrp3;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax1 = x - bx;
    ay1 = y - by;
    ax2 = 1.0f - ax1;
    ay2 = 1.0f - ay1;
    lrp0 = (byte_t)(15.0f * ax2 * ay2 + 0.5f);
    lrp1 = (byte_t)(15.0f * ax1 * ay2 + 0.5f);
    lrp2 = (byte_t)(15.0f * ax2 * ay1 + 0.5f);
    lrp3 = (byte_t)(15.0f * ax1 * ay1 + 0.5f);

    /* 输出四个点值 */
    if (!dst->gdi) {
        color.c16.lrp = lrp0;
        pixel_lrp12z(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c16.lrp = lrp1;
            pixel_lrp12z(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c16.lrp = lrp2;
            pixel_lrp12z(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c16.lrp = lrp3;
            pixel_lrp12z(dst, bx + 1, by + 1, color);
        }
    }
    else {
        color.c16.lrp = lrp0;
        pixel_lrp12n(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c16.lrp = lrp1;
            pixel_lrp12n(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c16.lrp = lrp2;
            pixel_lrp12n(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c16.lrp = lrp3;
            pixel_lrp12n(dst, bx + 1, by + 1, color);
        }
    }
}

/*
=======================================
    抗锯齿写点 (fmt = 15)
=======================================
*/
CR_API void_t
dot_wu_set15 (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y,
  __CR_IN__ cpix_t          color
    )
{
    bool_t  over;
    sint_t  bx, by;
    fp32_t  ax1, ay1;
    fp32_t  ax2, ay2;
    byte_t  lrp0, lrp1;
    byte_t  lrp2, lrp3;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax1 = x - bx;
    ay1 = y - by;
    ax2 = 1.0f - ax1;
    ay2 = 1.0f - ay1;
    lrp0 = (byte_t)(31.0f * ax2 * ay2 + 0.5f);
    lrp1 = (byte_t)(31.0f * ax1 * ay2 + 0.5f);
    lrp2 = (byte_t)(31.0f * ax2 * ay1 + 0.5f);
    lrp3 = (byte_t)(31.0f * ax1 * ay1 + 0.5f);

    /* 输出四个点值 */
    if (!dst->gdi) {
        color.c16.lrp = lrp0;
        pixel_lrp15z(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c16.lrp = lrp1;
            pixel_lrp15z(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c16.lrp = lrp2;
            pixel_lrp15z(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c16.lrp = lrp3;
            pixel_lrp15z(dst, bx + 1, by + 1, color);
        }
    }
    else {
        color.c16.lrp = lrp0;
        pixel_lrp15n(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c16.lrp = lrp1;
            pixel_lrp15n(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c16.lrp = lrp2;
            pixel_lrp15n(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c16.lrp = lrp3;
            pixel_lrp15n(dst, bx + 1, by + 1, color);
        }
    }
}

/*
=======================================
    抗锯齿写点 (fmt = 16)
=======================================
*/
CR_API void_t
dot_wu_set16 (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y,
  __CR_IN__ cpix_t          color
    )
{
    bool_t  over;
    sint_t  bx, by;
    fp32_t  ax1, ay1;
    fp32_t  ax2, ay2;
    byte_t  lrp0, lrp1;
    byte_t  lrp2, lrp3;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax1 = x - bx;
    ay1 = y - by;
    ax2 = 1.0f - ax1;
    ay2 = 1.0f - ay1;
    lrp0 = (byte_t)(63.0f * ax2 * ay2 + 0.5f);
    lrp1 = (byte_t)(63.0f * ax1 * ay2 + 0.5f);
    lrp2 = (byte_t)(63.0f * ax2 * ay1 + 0.5f);
    lrp3 = (byte_t)(63.0f * ax1 * ay1 + 0.5f);

    /* 输出四个点值 */
    if (!dst->gdi) {
        color.c16.lrp = lrp0;
        pixel_lrp16z(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c16.lrp = lrp1;
            pixel_lrp16z(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c16.lrp = lrp2;
            pixel_lrp16z(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c16.lrp = lrp3;
            pixel_lrp16z(dst, bx + 1, by + 1, color);
        }
    }
    else {
        color.c16.lrp = lrp0;
        pixel_lrp16n(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c16.lrp = lrp1;
            pixel_lrp16n(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c16.lrp = lrp2;
            pixel_lrp16n(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c16.lrp = lrp3;
            pixel_lrp16n(dst, bx + 1, by + 1, color);
        }
    }
}

/*
=======================================
    抗锯齿写点 (fmt = 24)
=======================================
*/
CR_API void_t
dot_wu_set24 (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y,
  __CR_IN__ cpix_t          color
    )
{
    bool_t  over;
    sint_t  bx, by;
    fp32_t  ax1, ay1;
    fp32_t  ax2, ay2;
    byte_t  lrp0, lrp1;
    byte_t  lrp2, lrp3;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax1 = x - bx;
    ay1 = y - by;
    ax2 = 1.0f - ax1;
    ay2 = 1.0f - ay1;
    lrp0 = (byte_t)(255.0f * ax2 * ay2 + 0.5f);
    lrp1 = (byte_t)(255.0f * ax1 * ay2 + 0.5f);
    lrp2 = (byte_t)(255.0f * ax2 * ay1 + 0.5f);
    lrp3 = (byte_t)(255.0f * ax1 * ay1 + 0.5f);

    /* 输出四个点值 */
    if (!dst->gdi) {
        color.c32.lrp = lrp0;
        pixel_lrp24z(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c32.lrp = lrp1;
            pixel_lrp24z(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c32.lrp = lrp2;
            pixel_lrp24z(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c32.lrp = lrp3;
            pixel_lrp24z(dst, bx + 1, by + 1, color);
        }
    }
    else {
        color.c32.lrp = lrp0;
        pixel_lrp24n(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c32.lrp = lrp1;
            pixel_lrp24n(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c32.lrp = lrp2;
            pixel_lrp24n(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c32.lrp = lrp3;
            pixel_lrp24n(dst, bx + 1, by + 1, color);
        }
    }
}

/*
=======================================
    抗锯齿写点 (fmt = 32)
=======================================
*/
CR_API void_t
dot_wu_set32 (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          y,
  __CR_IN__ cpix_t          color
    )
{
    bool_t  over;
    sint_t  bx, by;
    fp32_t  ax1, ay1;
    fp32_t  ax2, ay2;
    byte_t  lrp0, lrp1;
    byte_t  lrp2, lrp3;

    /* 计算插值参数 */
    bx = (sint_t)x;
    by = (sint_t)y;
    ax1 = x - bx;
    ay1 = y - by;
    ax2 = 1.0f - ax1;
    ay2 = 1.0f - ay1;
    lrp0 = (byte_t)(255.0f * ax2 * ay2 + 0.5f);
    lrp1 = (byte_t)(255.0f * ax1 * ay2 + 0.5f);
    lrp2 = (byte_t)(255.0f * ax2 * ay1 + 0.5f);
    lrp3 = (byte_t)(255.0f * ax1 * ay1 + 0.5f);

    /* 输出四个点值 */
    if (!dst->gdi) {
        color.c32.lrp = lrp0;
        pixel_lrp32z(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c32.lrp = lrp1;
            pixel_lrp32z(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c32.lrp = lrp2;
            pixel_lrp32z(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c32.lrp = lrp3;
            pixel_lrp32z(dst, bx + 1, by + 1, color);
        }
    }
    else {
        color.c32.lrp = lrp0;
        pixel_lrp32n(dst, bx + 0, by + 0, color);
        if (bx < dst->clip_win.x2) {
            over = FALSE;
            color.c32.lrp = lrp1;
            pixel_lrp32n(dst, bx + 1, by + 0, color);
        }
        else {
            over = TRUE;
        }
        if (by < dst->clip_win.y2) {
            color.c32.lrp = lrp2;
            pixel_lrp32n(dst, bx + 0, by + 1, color);
        }
        else {
            over = TRUE;
        }
        if (!over) {
            color.c32.lrp = lrp3;
            pixel_lrp32n(dst, bx + 1, by + 1, color);
        }
    }
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
