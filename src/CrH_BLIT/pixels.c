/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-29  */
/*     #######          ###    ###      [BLIT]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2012-07-08  */
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

#ifndef __CR_PIXELS_C__
#define __CR_PIXELS_C__ 0x3CA59411UL

#include "blit.h"
#include "blends.h"

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

#endif  /* !__CR_PIXELS_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
