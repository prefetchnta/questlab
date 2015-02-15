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
    aabb->v[0].x = max->x;
    aabb->v[0].y = max->y;
    aabb->v[0].z = max->z;

    aabb->v[1].x = max->x;
    aabb->v[1].y = max->y;
    aabb->v[1].z = min->z;

    aabb->v[2].x = max->x;
    aabb->v[2].y = min->y;
    aabb->v[2].z = max->z;

    aabb->v[3].x = max->x;
    aabb->v[3].y = min->y;
    aabb->v[3].z = min->z;

    aabb->v[4].x = min->x;
    aabb->v[4].y = max->y;
    aabb->v[4].z = max->z;

    aabb->v[5].x = min->x;
    aabb->v[5].y = max->y;
    aabb->v[5].z = min->z;

    aabb->v[6].x = min->x;
    aabb->v[6].y = min->y;
    aabb->v[6].z = max->z;

    aabb->v[7].x = min->x;
    aabb->v[7].y = min->y;
    aabb->v[7].z = min->z;
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
    leng_t          idx;
    const vec3d_t*  ptr;
    /* -------------- */
    fp32_t  min_x, min_y, min_z;
    fp32_t  max_x, max_y, max_z;
    leng_t  idx_min_x, idx_min_y, idx_min_z;
    leng_t  idx_max_x, idx_max_y, idx_max_z;

    ptr = pos;
    min_x = max_x = ptr->x;
    min_y = max_y = ptr->y;
    min_z = max_z = ptr->z;
    idx_min_x = idx_min_y = idx_min_z = 0;
    idx_max_x = idx_max_y = idx_max_z = 0;
    ptr = (vec3d_t*)((byte_t*)ptr + bpv);
    for (idx = 1; idx < count; idx++) {
        if (ptr->x < min_x) { min_x = ptr->x; idx_min_x = idx; }
        if (ptr->y < min_y) { min_y = ptr->y; idx_min_y = idx; }
        if (ptr->z < min_z) { min_z = ptr->z; idx_min_z = idx; }
        if (ptr->x > max_x) { max_x = ptr->x; idx_max_x = idx; }
        if (ptr->y > max_y) { max_y = ptr->y; idx_max_y = idx; }
        if (ptr->z > max_z) { max_z = ptr->z; idx_max_z = idx; }
        ptr = (vec3d_t*)((byte_t*)ptr + bpv);
    }

    idx = idx_max_x * bpv;
    ptr = (vec3d_t*)((byte_t*)pos + idx);
    max_x = ptr->x;
    max_y = ptr->y;
    max_z = ptr->z;
    idx = idx_min_x * bpv;
    ptr = (vec3d_t*)((byte_t*)pos + idx);
    max_x -= ptr->x;
    max_y -= ptr->y;
    max_z -= ptr->z;
    min_x = max_x * max_x + max_y * max_y + max_z * max_z;

    idx = idx_max_y * bpv;
    ptr = (vec3d_t*)((byte_t*)pos + idx);
    max_x = ptr->x;
    max_y = ptr->y;
    max_z = ptr->z;
    idx = idx_min_y * bpv;
    ptr = (vec3d_t*)((byte_t*)pos + idx);
    max_x -= ptr->x;
    max_y -= ptr->y;
    max_z -= ptr->z;
    min_y = max_x * max_x + max_y * max_y + max_z * max_z;

    idx = idx_max_z * bpv;
    ptr = (vec3d_t*)((byte_t*)pos + idx);
    max_x = ptr->x;
    max_y = ptr->y;
    max_z = ptr->z;
    idx = idx_min_z * bpv;
    ptr = (vec3d_t*)((byte_t*)pos + idx);
    max_x -= ptr->x;
    max_y -= ptr->y;
    max_z -= ptr->z;
    min_z = max_x * max_x + max_y * max_y + max_z * max_z;

    if (min_z > min_x && min_z > min_y) {
        ball->radius = min_z;
        idx_max_x = idx_max_z;
        idx_min_x = idx_min_z;
    }
    else
    if (min_y > min_x && min_y > min_z) {
        ball->radius = min_y;
        idx_max_x = idx_max_y;
        idx_min_x = idx_min_y;
    }
    else {
        ball->radius = min_x;
    }
    idx = idx_max_x * bpv;
    ptr = (vec3d_t*)((byte_t*)pos + idx);
    ball->center.x = ptr->x;
    ball->center.y = ptr->y;
    ball->center.z = ptr->z;
    idx = idx_min_x * bpv;
    ptr = (vec3d_t*)((byte_t*)pos + idx);
    ball->center.x += ptr->x;
    ball->center.y += ptr->y;
    ball->center.z += ptr->z;
    ball->center.x *= 0.5f;
    ball->center.y *= 0.5f;
    ball->center.z *= 0.5f;
    ball->radius = FSQRT(ball->radius) * 0.5f;
    min_y = ball->radius * ball->radius;

    for (idx = 0; idx < count; idx++) {
        max_x = ball->center.x - pos->x;
        max_y = ball->center.y - pos->y;
        max_z = ball->center.z - pos->z;
        pos = (vec3d_t*)((byte_t*)pos + bpv);
        min_x = max_x * max_x + max_y * max_y + max_z * max_z;
        if (min_x > min_y) {
            min_x = FSQRT(min_x);
            min_z = (min_x + ball->radius) * 0.5f;
            min_x = (min_z - ball->radius) / min_x;
            ball->radius = min_z;
            ball->center.x += max_x * min_x;
            ball->center.y += max_y * min_x;
            ball->center.z += max_z * min_x;
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
