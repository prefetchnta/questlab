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

#ifndef __CR_TERRAIN_C__
#define __CR_TERRAIN_C__ 0x9B064B12UL

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
  __CR_IN__ leng_t      bpv,
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
    if (x >= real->x2) {
        err_set(__CR_TERRAIN_C__, x,
                "gen_terrain_tile()", "invalid param: x");
        return (0);
    }
    if (z >= real->z2) {
        err_set(__CR_TERRAIN_C__, z,
                "gen_terrain_tile()", "invalid param: z");
        return (0);
    }
    if (w < 2 || w > real->ww - x) {
        err_set(__CR_TERRAIN_C__, w,
                "gen_terrain_tile()", "invalid param: w");
        return (0);
    }
    if (h < 2 || h > real->hh - z) {
        err_set(__CR_TERRAIN_C__, h,
                "gen_terrain_tile()", "invalid param: h");
        return (0);
    }

    /* 生成顶点 */
    count  = w;
    count *= h;
    if (xyz == NULL)
        return (count);
    uu = tex_scale / (fp32_t)(w - 1);
    vv = tex_scale / (fp32_t)(h - 1);
    for (tz = z; tz < z + h; tz++)
    for (tx = x; tx < x + w; tx++) {
        xyz->x = tx * real->grid;
        xyz->y = real->map[tz * real->ww + tx];
        xyz->z = tz * real->grid;
        xyz = (vec3d_t*)((byte_t*)xyz + bpv);
        if (nrm != NULL) {
            height_map_nrm(htmap, nrm, tx, tz, TRUE);
            nrm = (vec3d_t*)((byte_t*)nrm + bpv);
        }
        if (uvw != NULL) {
            uvw->x = (fp32_t)(tx - x) * uu;
            uvw->y = (fp32_t)(tz - z) * vv;
            uvw = (vec2d_t*)((byte_t*)uvw + bpv);
        }
    }
    return (count);
}

#endif  /* !__CR_TERRAIN_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/