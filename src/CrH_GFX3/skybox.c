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

#ifndef __CR_SKYBOX_C__
#define __CR_SKYBOX_C__ 0x012D7A3BUL

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
  __CR_IN__ leng_t      bpv,
  __CR_IN__ fp32_t      xsize,
  __CR_IN__ fp32_t      ysize,
  __CR_IN__ fp32_t      zsize
    )
{
    if (xyz == NULL)
        return (36);

    /* left */
    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    /* right */
    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    /* front */
    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    /* back */
    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    /* top */
    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    /* bottom */
    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 0.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 1.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = 0.0f;
        uvw->y = 1.0f;
        uvw = (vec2d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
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
  __CR_IN__ leng_t      bpv,
  __CR_IN__ fp32_t      xsize,
  __CR_IN__ fp32_t      ysize,
  __CR_IN__ fp32_t      zsize
    )
{
    if (xyz == NULL)
        return (36);

    /* left */
    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y =  0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y =  0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y = -0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y =  0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y = -0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -1.000f;
        uvw->y = -0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    /* right */
    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y =  0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y = -0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y =  0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y =  0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y = -0.999f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  1.000f;
        uvw->y = -0.999f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    /* front */
    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y =  0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y = -0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -0.999f;
        uvw->z =  1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    /* back */
    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y =  0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y = -0.999f;
        uvw->z = -1.000f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    /* top */
    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y =  1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y =  1.000f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y =  1.000f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y =  ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y =  1.000f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    /* bottom */
    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y = -1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -1.000f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z =  zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y = -1.000f;
        uvw->z =  0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x =  xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    xyz = (vec3d_t*)((byte_t*)xyz + bpv);
    if (uvw != NULL) {
        uvw->x =  0.999f;
        uvw->y = -1.000f;
        uvw->z = -0.999f;
        uvw = (vec3d_t*)((byte_t*)uvw + bpv);
    }

    xyz->x = -xsize / 2.0f;
    xyz->y = -ysize / 2.0f;
    xyz->z = -zsize / 2.0f;
    if (uvw != NULL) {
        uvw->x = -0.999f;
        uvw->y = -1.000f;
        uvw->z = -0.999f;
    }
    return (36);
}

#endif  /* !__CR_SKYBOX_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
