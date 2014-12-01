/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-06-05  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 三维图形头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_GFX3_H__
#define __CR_GFX3_H__ 0x190ECB8EUL

#include "gfx2.h"

/*****************************************************************************/
/*                                 数据结构                                  */
/*****************************************************************************/

/* 二维向量 */
typedef struct
{
        fp32_t  x, y;

} vec2d_t;

/* 三维向量 */
typedef struct
{
        fp32_t  x, y, z;

} vec3d_t;

/* 四维向量 */
typedef struct
{
        fp32_t  x, y, z, w;

} vec4d_t, sPLANE, sQUATO;

/*
********************
    D3D 行主矩阵
    OGL 列主矩阵
********************
*/

/* 3x3 矩阵 */
typedef struct
{
        fp32_t  m[3 * 3];

} mat3x3_t;

/* 4x4 矩阵 */
typedef struct
{
        fp32_t  m[4 * 4];

} mat4x4_t;

/* 射线 */
typedef struct
{
        vec3d_t     pos;
        vec3d_t     dir;

} sRADIAL;

/* 四边形 (left-top, cw) */
typedef struct
{
        vec3d_t     v[4];
} sQUAD;

/* AABB 包围盒 (left-top-up, cw) */
typedef struct
{
        vec3d_t     v[8];
} sAABB;

/* 包围球 */
typedef struct
{
        vec3d_t     center;
        fp32_t      radius;

} sSPHERE;

/* 视锥 (near, far, left, right, top, bottom) */
typedef struct
{
        sPLANE  p[6];
        fp32_t  bias;   // 负数向外扩

} sFRUSTUM;

/*****************************************************************************/
/*                                 几何运算                                  */
/*****************************************************************************/

/* 包围盒与包围球的获取 */
CR_API void_t   bound_min_max (vec3d_t *min, vec3d_t *max,
                               const vec3d_t *pos, leng_t count,
                               leng_t bpv);
CR_API void_t   bound_get_aabb (sAABB *aabb, const vec3d_t *pos,
                                leng_t count, leng_t bpv);
CR_API void_t   bound_get_ball (sSPHERE *ball, const vec3d_t *pos,
                                leng_t count, leng_t bpv);

/*****************************************************************************/
/*                                 几何生成                                  */
/*****************************************************************************/

/* 天空盒 */
CR_API leng_t   gen_skybox2 (vec3d_t *xyz, vec2d_t *uvw, leng_t bpv,
                             fp32_t xsize, fp32_t ysize, fp32_t zsize);
CR_API leng_t   gen_skybox3 (vec3d_t *xyz, vec3d_t *uvw, leng_t bpv,
                             fp32_t xsize, fp32_t ysize, fp32_t zsize);
/* 天空顶 */
CR_API leng_t   gen_skydome (vec3d_t *xyz, vec2d_t *uvw, leng_t bpv,
                             void_t *ibuf, leng_t *inum, fp32_t radius,
                             fp32_t scale, fp32_t offset, sint_t dtheta,
                             sint_t dphi);
/* 天空面 */
CR_API leng_t   gen_skyplane (vec3d_t *xyz, vec2d_t *uvw, leng_t bpv,
                              void_t *ibuf, leng_t *inum, fp32_t a_radius,
                              fp32_t p_radius, fp32_t offset, fp32_t tex_scale,
                              sint_t ndiv);
/* 垂直点缀 */
CR_API leng_t   gen_adorn_v (vec3d_t *xyz, vec2d_t *uvw, leng_t bpv,
                             void_t *ibuf, leng_t *inum, const vec3d_t *pos,
                             const vec2d_t *tex, fp32_t radius, fp32_t height);

/*****************************************************************************/
/*                                摄像机控制                                 */
/*****************************************************************************/

typedef struct
{
        vec4d_t *eye;
        vec4d_t *lookat;
        fp32_t  distance;
        fp32_t  theta, phi;

} sCAMERA;

/* 旋转角度都从正轴开始 */
CR_API void_t   camera_refresh (sCAMERA *cam);
CR_API void_t   camera_init (sCAMERA *cam, vec4d_t *eye, vec4d_t *lookat);
CR_API void_t   camera_walkdir2 (const sCAMERA *cam, vec3d_t *vdir);
CR_API void_t   camera_walkdir3 (const sCAMERA *cam, vec3d_t *vdir);
CR_API void_t   camera_eye_a (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_eye_d (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_eye_w2 (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_eye_s2 (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_eye_w3 (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_eye_s3 (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_lookat_a (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_lookat_d (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_lookat_w2 (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_lookat_s2 (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_lookat_w3 (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_lookat_s3 (const sCAMERA *cam, fp32_t step);
CR_API void_t   camera_update_fps (sCAMERA *cam);
CR_API void_t   camera_update_tps (sCAMERA *cam);

/*****************************************************************************/
/*                                 视锥剔除                                  */
/*****************************************************************************/

CR_API bool_t   frustum_vec3 (const sFRUSTUM *frt, const vec3d_t *vec3);
CR_API bool_t   frustum_vec4 (const sFRUSTUM *frt, const vec4d_t *vec4);
CR_API bool_t   frustum_ball (const sFRUSTUM *frt, const sSPHERE *ball);
CR_API bool_t   frustum_quad (const sFRUSTUM *frt, const sQUAD *quad);
CR_API bool_t   frustum_aabb (const sFRUSTUM *frt, const sAABB *aabb);

#endif  /* !__CR_GFX3_H__ */

/* 只使用微软自家的编译器 */
#if     defined(_CR_CC_MSC_) && (_CR_CC_VER_ >= 1400)
/*****************************************************************************/
/*                              Direct3D8 API                                */
/*****************************************************************************/
#if     defined(_CR_USE_D3D8_) && (_CR_CC_VER_ < 1700)

#include "gfx3/d3d8api.h"
#include "gfx3/d3d8api.inl"

/*****************************************************************************/
/*                              Direct3D9 API                                */
/*****************************************************************************/
#elif   defined(_CR_USE_D3D9_) && (_CR_CC_VER_ < 1700)

#include "gfx3/d3d9api.h"
#include "gfx3/d3d9api.inl"

/*****************************************************************************/
/*                              Direct3D11 API                               */
/*****************************************************************************/
#elif   defined(_CR_USE_D3D11_)

#include "gfx3/d3d11api.h"
#include "gfx3/d3d11api.inl"

#endif  /* GFX3 API predefines */

#endif  /* _CR_CC_MSC_ && _CR_CC_VER_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
