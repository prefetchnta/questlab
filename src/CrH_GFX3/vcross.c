/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-12-02  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 点缀片生成函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
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
    生成点缀片
=======================================
*/
CR_API leng_t
gen_v_cross (
  __CR_OT__ vec3d_t*        xyz,
  __CR_OT__ vec2d_t*        uvw,
  __CR_IN__ leng_t          bpv1,
  __CR_IN__ leng_t          bpv2,
  __CR_OT__ void_t*         ibuf,
  __CR_OT__ leng_t*         inum,
  __CR_IN__ const vec3d_t*  pos,
  __CR_IN__ const vec2d_t*  tex,
  __CR_IN__ fp32_t          radius,
  __CR_IN__ fp32_t          height,
  __CR_IN__ sint_t          ndiv,
  __CR_IN__ leng_t          count
    )
{
    sint_t  jj;
    int16u* ti16;
    int32u* ti32;
    leng_t  nv, ni, ii;
    fp32_t  step, angle;

    nv = ndiv * 4 * count;
    ni = ndiv * 6 * count;
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
    step = CR_PI / (fp32_t)ndiv;
    for (ii = 0; ii < count; ii++) {
        angle = 0.0f;
        for (jj = 0; jj < ndiv; jj++) {
            xyz->x = pos->x - radius * FCOS(angle);
            xyz->y = pos->y + height;
            xyz->z = pos->z - radius * FSIN(angle);
            xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
            if (uvw != NULL) {
                uvw->x = tex->x;
                uvw->y = 0.0f;
                uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
            }

            xyz->x = pos->x + radius * FCOS(angle);
            xyz->y = pos->y + height;
            xyz->z = pos->z + radius * FSIN(angle);
            xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
            if (uvw != NULL) {
                uvw->x = tex->y;
                uvw->y = 0.0f;
                uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
            }

            xyz->x = pos->x - radius * FCOS(angle);
            xyz->y = pos->y;
            xyz->z = pos->z - radius * FSIN(angle);
            xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
            if (uvw != NULL) {
                uvw->x = tex->x;
                uvw->y = 1.0f;
                uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
            }

            xyz->x = pos->x + radius * FCOS(angle);
            xyz->y = pos->y;
            xyz->z = pos->z + radius * FSIN(angle);
            xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
            if (uvw != NULL) {
                uvw->x = tex->y;
                uvw->y = 1.0f;
                uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
            }
            angle += step;
        }
    }

    /* 生成索引 */
    if (ibuf == NULL)
        return (nv);
    if (nv > 65536) {
        ti32 = (int32u*)ibuf;
        for (ii = 0; ii < count * ndiv; ii++) {
            ti32[0] = (int32u)(ii * 4);
            ti32[1] = (int32u)(ii * 4 + 1);
            ti32[2] = (int32u)(ii * 4 + 3);
            ti32[3] = (int32u)(ii * 4);
            ti32[4] = (int32u)(ii * 4 + 3);
            ti32[5] = (int32u)(ii * 4 + 2);
            ti32 += 6;
        }
    }
    else {
        ti16 = (int16u*)ibuf;
        for (ii = 0; ii < count * ndiv; ii++) {
            ti16[0] = (int16u)(ii * 4);
            ti16[1] = (int16u)(ii * 4 + 1);
            ti16[2] = (int16u)(ii * 4 + 3);
            ti16[3] = (int16u)(ii * 4);
            ti16[4] = (int16u)(ii * 4 + 3);
            ti16[5] = (int16u)(ii * 4 + 2);
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
