/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-12-01  */
/*     #######          ###    ###      [GFX3]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 天空面生成函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "gfx3.h"

#ifndef _CR_NO_STDC_
    #if defined(_CR_OS_ANDROID_) && (ANDROID_API <= 19)
        #if defined(__pure2)
            #undef  __pure2
        #endif
    #endif
    #include <math.h>
#endif

/*
=======================================
    生成天空面
=======================================
*/
CR_API leng_t
gen_skyplane (
  __CR_OT__ vec3d_t*    xyz,
  __CR_OT__ vec2d_t*    uvw,
  __CR_IN__ leng_t      bpv1,
  __CR_IN__ leng_t      bpv2,
  __CR_OT__ void_t*     ibuf,
  __CR_OT__ leng_t*     inum,
  __CR_IN__ fp32_t      a_radius,
  __CR_IN__ fp32_t      p_radius,
  __CR_IN__ fp32_t      offset,
  __CR_IN__ fp32_t      tex_scale,
  __CR_IN__ sint_t      ndiv
    )
{
    int16u* ti16;
    int32u* ti32;
    leng_t  nv, ni;
    sint_t  ii, jj, start;
    fp32_t  x_dist, z_dist;
    fp32_t  x_height, z_height, height;
    fp32_t  tex_delta, delta, plane_size;

    nv = (ndiv + 1) * (ndiv + 1);
    ni = ndiv * ndiv * 6;
    if (inum != NULL)
        *inum = ni;
    if (xyz == NULL)
        return (nv);

    /* 生成顶点 */
    if (bpv1 == 0)
        bpv1 = sizeof(vec3d_t) + sizeof(vec2d_t);
    else
    if (bpv1 < sizeof(vec3d_t))
        bpv1 = sizeof(vec3d_t);
    if (bpv2 == 0)
        bpv2 = bpv1;
    else
    if (bpv2 < sizeof(vec2d_t))
        bpv2 = sizeof(vec2d_t);
    plane_size = 2.0f * FSQRT(a_radius * a_radius - p_radius * p_radius);
    delta = plane_size / (fp32_t)ndiv;
    tex_delta = tex_scale / (fp32_t)ndiv;
    for (ii = 0; ii <= ndiv; ii++)
    for (jj = 0; jj <= ndiv; jj++) {
        x_dist = (-0.5f * plane_size) + ((fp32_t)jj * delta);
        z_dist = (-0.5f * plane_size) + ((fp32_t)ii * delta);
        x_height = x_dist * x_dist / a_radius;
        z_height = z_dist * z_dist / a_radius;
        height = x_height + z_height;

        xyz->x =  x_dist;
        xyz->y = -height + offset;
        xyz->z =  z_dist;
        xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
        if (uvw != NULL) {
            uvw->x = (fp32_t)jj * tex_delta;
            uvw->y = (fp32_t)ii * tex_delta;
            uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
        }
    }

    /* 生成索引 */
    if (ibuf == NULL)
        return (nv);
    if (nv > 65536) {
        ti32 = (int32u*)ibuf;
        for (ii = 0; ii < ndiv; ii++)
        for (jj = 0; jj < ndiv; jj++) {
            start = (ii * (ndiv + 1) + jj);
            ti32[0] = (int32u)(start);
            ti32[1] = (int32u)(start + 1);
            ti32[2] = (int32u)(start + ndiv + 1);
            ti32[3] = (int32u)(start + 1);
            ti32[4] = (int32u)(start + ndiv + 2);
            ti32[5] = (int32u)(start + ndiv + 1);
            ti32 += 6;
        }
    }
    else {
        ti16 = (int16u*)ibuf;
        for (ii = 0; ii < ndiv; ii++)
        for (jj = 0; jj < ndiv; jj++) {
            start = (ii * (ndiv + 1) + jj);
            ti16[0] = (int16u)(start);
            ti16[1] = (int16u)(start + 1);
            ti16[2] = (int16u)(start + ndiv + 1);
            ti16[3] = (int16u)(start + 1);
            ti16[4] = (int16u)(start + ndiv + 2);
            ti16[5] = (int16u)(start + ndiv + 1);
            ti16 += 6;
        }
    }
    return (nv);
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
