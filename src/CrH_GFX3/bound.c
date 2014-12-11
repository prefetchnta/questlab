/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-05-30  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 包围体计算函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_BOUND_C__
#define __CR_BOUND_C__ 0x6FBEB11EUL

#include "gfx3.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
=======================================
    找出 XYZ 的极值
=======================================
*/
CR_API void_t
bound_min_max (
  __CR_OT__ vec3d_t*        min,
  __CR_OT__ vec3d_t*        max,
  __CR_IN__ const vec3d_t*  pos,
  __CR_IN__ leng_t          count,
  __CR_IN__ leng_t          bpv
    )
{
    min->x = pos->x;
    min->y = pos->y;
    min->z = pos->z;
    count--;
    max->x = pos->x;
    max->y = pos->y;
    max->z = pos->z;
    pos = (vec3d_t*)((byte_t*)pos + bpv);
    while (count != 0) {
        if (pos->x < min->x) min->x = pos->x;
        if (pos->y < min->y) min->y = pos->y;
        if (pos->z < min->z) min->z = pos->z;
        count--;
        if (pos->x > max->x) max->x = pos->x;
        if (pos->y > max->y) max->y = pos->y;
        if (pos->z > max->z) max->z = pos->z;
        pos = (vec3d_t*)((byte_t*)pos + bpv);
    }
}

/*
=======================================
    合成 AABB
=======================================
*/
CR_API void_t
bound_gen_aabb (
  __CR_OT__ sAABB*          aabb,
  __CR_IN__ const vec3d_t*  min,
  __CR_IN__ const vec3d_t*  max
    )
{
    aabb->v[0].x = min->x;
    aabb->v[0].y = max->y;
    aabb->v[0].z = min->z;

    aabb->v[1].x = max->x;
    aabb->v[1].y = max->y;
    aabb->v[1].z = min->z;

    aabb->v[2].x = max->x;
    aabb->v[2].y = max->y;
    aabb->v[2].z = max->z;

    aabb->v[3].x = min->x;
    aabb->v[3].y = max->y;
    aabb->v[3].z = max->z;

    aabb->v[4].x = min->x;
    aabb->v[4].y = min->y;
    aabb->v[4].z = min->z;

    aabb->v[5].x = max->x;
    aabb->v[5].y = min->y;
    aabb->v[5].z = min->z;

    aabb->v[6].x = max->x;
    aabb->v[6].y = min->y;
    aabb->v[6].z = max->z;

    aabb->v[7].x = min->x;
    aabb->v[7].y = min->y;
    aabb->v[7].z = max->z;
}

/*
=======================================
    计算 AABB
=======================================
*/
CR_API void_t
bound_get_aabb (
  __CR_OT__ sAABB*          aabb,
  __CR_IN__ const vec3d_t*  pos,
  __CR_IN__ leng_t          count,
  __CR_IN__ leng_t          bpv
    )
{
    vec3d_t     min, max;

    bound_min_max(&min, &max, pos, count, bpv);
    bound_gen_aabb(aabb, &min, &max);
}

/*
=======================================
    计算包围球
=======================================
*/
CR_API void_t
bound_get_ball (
  __CR_OT__ sSPHERE*        ball,
  __CR_IN__ const vec3d_t*  pos,
  __CR_IN__ leng_t          count,
  __CR_IN__ leng_t          bpv
    )
{
    fp32_t      radius;
    fp32_t      xx, yy, zz;
    vec3d_t     min, max;

    bound_min_max(&min, &max, pos, count, bpv);

    ball->center.x = (min.x + max.x) / 2.0f;
    ball->center.y = (min.y + max.y) / 2.0f;
    ball->center.z = (min.z + max.z) / 2.0f;

    ball->radius = -1.0f;
    for (; count != 0; count--) {
        xx = ball->center.x - pos->x;
        yy = ball->center.y - pos->y;
        zz = ball->center.z - pos->z;
        radius = FSQRT(xx * xx + yy * yy + zz * zz);
        if (radius > ball->radius)
            ball->radius = radius;
        pos = (vec3d_t*)((byte_t*)pos + bpv);
    }
}

#endif  /* !__CR_BOUND_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
