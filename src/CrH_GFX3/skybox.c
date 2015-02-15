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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 天空盒生成函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "gfx3.h"

/*
=======================================
    生成 2D 贴图的天空盒
=======================================
*/
CR_API leng_t
gen_skybox2 (
  __CR_OT__ vec3d_t*    xyz,
  __CR_OT__ vec2d_t*    uvw,
  __CR_IN__ leng_t      bpv1,
  __CR_IN__ leng_t      bpv2,
  __CR_IN__ fp32_t      xsize,
  __CR_IN__ fp32_t      ysize,
  __CR_IN__ fp32_t      zsize
    )
{
    if (xyz == NULL)
        return (36);
    xsize /= 2.0f;
    ysize /= 2.0f;
    zsize /= 2.0f;
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

    /* left */
    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    /* right */
    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    /* front */
    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    /* back */
    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    /* top */
    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    /* bottom */
    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
    }
    return (36);
}

/*
=======================================
    生成 3D 贴图的天空盒
=======================================
*/
CR_API leng_t
gen_skybox3 (
  __CR_OT__ vec3d_t*    xyz,
  __CR_OT__ vec3d_t*    uvw,
  __CR_IN__ leng_t      bpv1,
  __CR_IN__ leng_t      bpv2,
  __CR_IN__ fp32_t      xsize,
  __CR_IN__ fp32_t      ysize,
  __CR_IN__ fp32_t      zsize
    )
{
    if (xyz == NULL)
        return (36);
    xsize /= 2.0f;
    ysize /= 2.0f;
    zsize /= 2.0f;
    if (bpv1 == 0)
        bpv1 = sizeof(vec3d_t) + sizeof(vec3d_t);
    else
    if (bpv1 < sizeof(vec3d_t))
        bpv1 = sizeof(vec3d_t);
    if (bpv2 == 0)
        bpv2 = bpv1;
    else
    if (bpv2 < sizeof(vec3d_t))
        bpv2 = sizeof(vec3d_t);

    /* left */
    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y =  0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y =  0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y = -0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y =  0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y = -0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y = -0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    /* right */
    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y =  0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y = -0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y =  0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y =  0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y = -0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y = -0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    /* front */
    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y =  0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y = -0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    /* back */
    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y =  0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y = -0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    /* top */
    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y =  1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y =  1.000f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y =  1.000f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y =  ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  1.000f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    /* bottom */
    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y = -1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -1.000f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z =  zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y = -1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x =  xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y = -1.000f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv2);
    }

    xyz->x = -xsize;
    xyz->y = -ysize;
    xyz->z = -zsize;
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -1.000f;
        uvw->z = -0.999f;
    }
    return (36);
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
