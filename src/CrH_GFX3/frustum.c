/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-05-22  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 视锥剔除函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "gfx3.h"

/*
=======================================
    检查点是否在视锥内
=======================================
*/
CR_API bool_t
frustum_vec3 (
  __CR_IN__ const sFRUSTUM* frt,
  __CR_IN__ const vec3d_t*  vec3
    )
{
    uint_t  idx;
    fp32_t  len;

    for (idx = 0; idx < 6; idx++) {
        len = frt->p[idx].x * vec3->x +
              frt->p[idx].y * vec3->y +
              frt->p[idx].z * vec3->z + frt->p[idx].w;
        if (len < frt->bias)
            return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    检查点是否在视锥内
=======================================
*/
CR_API bool_t
frustum_vec4 (
  __CR_IN__ const sFRUSTUM* frt,
  __CR_IN__ const vec4d_t*  vec4
    )
{
    return (frustum_vec3(frt, (const vec3d_t*)vec4));
}

/*
=======================================
    检查包围球是否在视锥内
=======================================
*/
CR_API bool_t
frustum_ball (
  __CR_IN__ const sFRUSTUM* frt,
  __CR_IN__ const sSPHERE*  ball
    )
{
    uint_t  idx;
    fp32_t  len;

    for (idx = 0; idx < 6; idx++) {
        len = frt->p[idx].x * ball->center.x +
              frt->p[idx].y * ball->center.y +
              frt->p[idx].z * ball->center.z + frt->p[idx].w;
        if (len + ball->radius < frt->bias)
            return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    检查四边形平面是否在视锥内
=======================================
*/
CR_API bool_t
frustum_quad (
  __CR_IN__ const sFRUSTUM* frt,
  __CR_IN__ const sQUAD*    quad
    )
{
    uint_t  idx;
    fp32_t  len;

    for (idx = 0; idx < 6; idx++) {
        len = frt->p[idx].x * quad->v[0].x +
              frt->p[idx].y * quad->v[0].y +
              frt->p[idx].z * quad->v[0].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        len = frt->p[idx].x * quad->v[1].x +
              frt->p[idx].y * quad->v[1].y +
              frt->p[idx].z * quad->v[1].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        len = frt->p[idx].x * quad->v[2].x +
              frt->p[idx].y * quad->v[2].y +
              frt->p[idx].z * quad->v[2].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        len = frt->p[idx].x * quad->v[3].x +
              frt->p[idx].y * quad->v[3].y +
              frt->p[idx].z * quad->v[3].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    检查 AABB 是否在视锥内
=======================================
*/
CR_API bool_t
frustum_aabb (
  __CR_IN__ const sFRUSTUM* frt,
  __CR_IN__ const sAABB*    aabb
    )
{
    uint_t  idx;
    fp32_t  len;

    for (idx = 0; idx < 6; idx++) {
        len = frt->p[idx].x * aabb->v[0].x +
              frt->p[idx].y * aabb->v[0].y +
              frt->p[idx].z * aabb->v[0].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        len = frt->p[idx].x * aabb->v[1].x +
              frt->p[idx].y * aabb->v[1].y +
              frt->p[idx].z * aabb->v[1].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        len = frt->p[idx].x * aabb->v[2].x +
              frt->p[idx].y * aabb->v[2].y +
              frt->p[idx].z * aabb->v[2].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        len = frt->p[idx].x * aabb->v[3].x +
              frt->p[idx].y * aabb->v[3].y +
              frt->p[idx].z * aabb->v[3].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        len = frt->p[idx].x * aabb->v[4].x +
              frt->p[idx].y * aabb->v[4].y +
              frt->p[idx].z * aabb->v[4].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        len = frt->p[idx].x * aabb->v[5].x +
              frt->p[idx].y * aabb->v[5].y +
              frt->p[idx].z * aabb->v[5].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        len = frt->p[idx].x * aabb->v[6].x +
              frt->p[idx].y * aabb->v[6].y +
              frt->p[idx].z * aabb->v[6].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        len = frt->p[idx].x * aabb->v[7].x +
              frt->p[idx].y * aabb->v[7].y +
              frt->p[idx].z * aabb->v[7].z + frt->p[idx].w;
        if (len >= frt->bias)
            continue;
        return (FALSE);
    }
    return (TRUE);
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
