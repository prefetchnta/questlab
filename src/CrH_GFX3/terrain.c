/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-12-03  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 地形生成函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "gfx3int.h"

/*
=======================================
    生成地形分片
=======================================
*/
CR_API leng_t
gen_terrain_tile (
  __CR_OT__ vec3d_t*    xyz,
  __CR_OT__ vec3d_t*    nrm,
  __CR_OT__ vec2d_t*    uvw,
  __CR_IN__ leng_t      bpv1,
  __CR_IN__ leng_t      bpv2,
  __CR_IN__ leng_t      bpv3,
  __CR_IN__ ht_map_t    htmap,
  __CR_IN__ uint_t      x,
  __CR_IN__ uint_t      z,
  __CR_IN__ uint_t      w,
  __CR_IN__ uint_t      h,
  __CR_IN__ fp32_t      tex_scale
    )
{
    leng_t  count;
    fp32_t  uu, vv;
    uint_t  tx, tz;
    /* --------- */
    sHEIGHTMAP* real;

    /* 参数过滤 */
    real = (sHEIGHTMAP*)htmap;
    if (x >= real->x2 ||
        z >= real->z2 ||
        w < 2 || w > real->ww - x ||
        h < 2 || h > real->hh - z)
        return (0);
    count  = w;
    count *= h;
    if (xyz == NULL)
        return (count);

    /* 生成顶点 */
    if (bpv1 == 0)
        bpv1 = sizeof(vec3d_t) * 2 + sizeof(vec2d_t);
    else
    if (bpv1 < sizeof(vec3d_t))
        bpv1 = sizeof(vec3d_t);
    if (bpv2 == 0)
        bpv2 = bpv1;
    else
    if (bpv2 < sizeof(vec3d_t))
        bpv2 = sizeof(vec3d_t);
    if (bpv3 == 0)
        bpv3 = bpv1;
    else
    if (bpv3 < sizeof(vec2d_t))
        bpv3 = sizeof(vec2d_t);
    uu = tex_scale / (fp32_t)(w - 1);
    vv = tex_scale / (fp32_t)(h - 1);
    for (tz = z; tz < z + h; tz++)
    for (tx = x; tx < x + w; tx++) {
        xyz->x = tx * real->grid;
        xyz->y = real->map[tz * real->ww + tx];
        xyz->z = tz * real->grid;
        xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
        if (nrm != NULL) {
            height_map_nrm(htmap, nrm, tx, tz, TRUE);
            nrm = (vec3d_t*)((byte_t*)nrm + bpv2);
        }
        if (uvw != NULL) {
            uvw->x = (fp32_t)(tx - x) * uu;
            uvw->y = (fp32_t)(tz - z) * vv;
            uvw = (vec2d_t*)((byte_t*)uvw + bpv3);
        }
    }
    return (count);
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
