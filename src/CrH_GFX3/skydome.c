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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 天空顶生成函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
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
    生成天空顶
=======================================
*/
CR_API leng_t
gen_skydome (
  __CR_OT__ vec3d_t*    xyz,
  __CR_OT__ vec2d_t*    uvw,
  __CR_IN__ leng_t      bpv1,
  __CR_IN__ leng_t      bpv2,
  __CR_OT__ void_t*     ibuf,
  __CR_OT__ leng_t*     inum,
  __CR_IN__ fp32_t      radius,
  __CR_IN__ fp32_t      scale,
  __CR_IN__ fp32_t      offset,
  __CR_IN__ sint_t      dtheta,
  __CR_IN__ sint_t      dphi
    )
{
    int16u* ti;
    leng_t  nv, ni;
    fp32_t  vx, vz;
    sint_t  phi, theta;

    nv = (360 / dtheta + 1) * (90 / dphi) + 1;
    ni = (360 / dtheta) * (90 / dphi - 1) * 6 + (360 / dtheta) * 3;
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
    for (phi = 0; phi < 90; phi += dphi)
    for (theta = 0; theta <= 360; theta += dtheta) {
        vx = radius * FCOS(phi * CR_DTOR) * FCOS(theta * CR_DTOR);
        vz = radius * FCOS(phi * CR_DTOR) * FSIN(theta * CR_DTOR);
        xyz->x = vx;
        xyz->y = radius * FSIN(phi * CR_DTOR) * scale + offset;
        xyz->z = vz;
        xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
        if (uvw != NULL) {
            vx /= radius;
            vz /= radius;
            uvw->x = (vx + 1.0f) * 0.5f;
            uvw->y = (vz + 1.0f) * 0.5f;
            uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
        }
    }
    xyz->x = 0.0f;
    xyz->y = radius + offset;
    xyz->z = 0.0f;
    if (uvw != NULL) {
        uvw->x = 0.5f;
        uvw->y = 0.5f;
    }

    /* 生成索引 */
    if (ibuf == NULL)
        return (nv);
    ti = (int16u*)ibuf;
    for (phi = 0; phi < 90 / dphi - 1; phi++)
    for (theta = 0; theta < 360 / dtheta; theta++) {
        ti[0] = (int16u)(phi * (360 / dtheta + 1) + theta + 1);
        ti[1] = (int16u)((phi + 1) * (360 / dtheta + 1) + theta);
        ti[2] = (int16u)(phi * (360 / dtheta + 1) + theta);
        ti[3] = (int16u)(phi * (360 / dtheta + 1) + theta + 1);
        ti[4] = (int16u)((phi + 1) * (360 / dtheta + 1) + theta + 1);
        ti[5] = (int16u)((phi + 1) * (360 / dtheta + 1) + theta);
        ti += 6;
    }
    for (theta = 0; theta < 360 / dtheta; theta++) {
        ti[0] = (int16u)((90 / dphi - 1) * (360 / dtheta + 1) + theta + 1);
        ti[1] = (int16u)(nv - 1);
        ti[2] = (int16u)((90 / dphi - 1) * (360 / dtheta + 1) + theta);
        ti += 3;
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
