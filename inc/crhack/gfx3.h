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
#define __CR_GFX3_H__

#include "gfx2.h"
#include "fileio.h"

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

/* 交点 */
typedef struct
{
        vec3d_t     pos;
        fp32_t      dist;

} sINTERSECT;

/* 四边形 */
typedef struct
{
        vec3d_t     v[4];
} sQUAD;

/* AABB 包围盒 (D3D Style) */
typedef struct
{
        vec3d_t     v[8];
} sAABB;

#define AABB_MAX_IDX    0
#define AABB_MIN_IDX    7

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
        fp32_t  bias;   /* 负数向外扩 */

} sFRUSTUM;

/* 模型的类型 */
#define MODEL_TYPE_WF_OBJ   0   /* WAVEFRONT OBJ */
#define MODEL_TYPE_MESHML   1   /* KlayGE MeshML */
#define MODEL_TYPE_X_MESH   2   /* DirectX Mesh */
#define MODEL_TYPE_FMTLIB   3   /* FMTZ Engine OBJ */

/*****************************************************************************/
/*                                  高度图                                   */
/*****************************************************************************/

typedef void_t* ht_map_t;

#define HTMAP_TYPE_BYTE     0   /* 字节型 */
#define HTMAP_TYPE_WORD     1   /* 单字型 */
#define HTMAP_TYPE_SINT     2   /* 整数型 */
#define HTMAP_TYPE_REAL     3   /* 实数型 */
CR_API ht_map_t height_map_new (const void_t *data, uint_t type, uint_t width,
                                uint_t height, fp32_t grid, fp32_t scale);
CR_API void_t   height_map_del (ht_map_t htmap);
CR_API fp32_t   height_map_get (ht_map_t htmap, fp32_t x, fp32_t z);
CR_API void_t   height_map_nrm (ht_map_t htmap, vec3d_t *normal,
                                uint_t x, uint_t z, bool_t is_nrm);
CR_API void_t   height_map_aabb (ht_map_t htmap, sAABB *aabb, uint_t x,
                                 uint_t z, uint_t w, uint_t h);
CR_API fp32_t   height_map_place (ht_map_t htmap, const sAABB *aabb,
                                  fp32_t x, fp32_t z, bool_t large);

/*****************************************************************************/
/*                                 几何运算                                  */
/*****************************************************************************/

/* 包围盒与包围球的获取 */
CR_API void_t   bound_min_max (vec3d_t *min, vec3d_t *max,
                               const vec3d_t *pos, leng_t count,
                               leng_t bpv);
CR_API void_t   bound_gen_aabb (sAABB *aabb, const vec3d_t *min,
                                const vec3d_t *max);
CR_API void_t   bound_get_aabb (sAABB *aabb, const vec3d_t *pos,
                                leng_t count, leng_t bpv);
CR_API void_t   bound_get_ball (sSPHERE *ball, const vec3d_t *pos,
                                leng_t count, leng_t bpv);

/*****************************************************************************/
/*                                 几何生成                                  */
/*****************************************************************************/

/* 天空盒 */
CR_API leng_t   gen_skybox2 (vec3d_t *xyz, vec2d_t *uvw, leng_t bpv1,
                             leng_t bpv2, fp32_t xsize, fp32_t ysize,
                             fp32_t zsize);
CR_API leng_t   gen_skybox3 (vec3d_t *xyz, vec3d_t *uvw, leng_t bpv1,
                             leng_t bpv2, fp32_t xsize, fp32_t ysize,
                             fp32_t zsize);
/* 天空顶 */
CR_API leng_t   gen_skydome (vec3d_t *xyz, vec2d_t *uvw, leng_t bpv1,
                             leng_t bpv2, void_t *ibuf, leng_t *inum,
                             fp32_t radius, fp32_t scale, fp32_t offset,
                             sint_t dtheta, sint_t dphi);
/* 天空面 */
CR_API leng_t   gen_skyplane (vec3d_t *xyz, vec2d_t *uvw, leng_t bpv1,
                              leng_t bpv2, void_t *ibuf, leng_t *inum,
                              fp32_t a_radius, fp32_t p_radius, fp32_t offset,
                              fp32_t tex_scale, sint_t ndiv);
/* 点缀片 */
CR_API leng_t   gen_v_cross (vec3d_t *xyz, vec2d_t *uvw, leng_t bpv1,
                             leng_t bpv2, void_t *ibuf, leng_t *inum,
                             const vec3d_t *pos, const vec2d_t *tex,
                             fp32_t radius, fp32_t height, sint_t ndiv,
                             leng_t count);
/* 地形分片 */
CR_API leng_t   gen_terrain_tile (vec3d_t *xyz, vec3d_t *nrm, vec2d_t *uvw,
                                  leng_t bpv1, leng_t bpv2, leng_t bpv3,
                                  ht_map_t htmap, uint_t x, uint_t z,
                                  uint_t w, uint_t h, fp32_t tex_scale);
/* 连接索引 */
CR_API uint_t   gen_interlock (int16u **pibuf, leng_t *inum, uint_t *level,
                               uint_t tile_size);

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

/*****************************************************************************/
/*                                WAVEFRONT                                  */
/*****************************************************************************/

/* OBJ 材质结构 */
typedef struct
{
        uint_t  flags;
        ansi_t  *name;
        fp32_t  d, ns, ni, tr;
        vec3d_t ka, kd, ks, ke, tf;
        uint_t  illum, halo, sharpness;
        ansi_t  *map_ka, *map_kd, *map_ks;
        ansi_t  *map_d, *map_ns, *bump;

} sWAVEFRONT_M;

/* 颜色标志 */
#define WAVEFRONT_KA    1   /* 存在 Ka */
#define WAVEFRONT_KD    2   /* 存在 Kd */
#define WAVEFRONT_KS    4   /* 存在 Ks */
#define WAVEFRONT_KE    8   /* 存在 Ke */

/* OBJ 模型结构 */
typedef struct
{
        /* OBJ 内容 */
        ansi_t  *name, *mtl;    /* 模型名称与材质名称 */
        leng_t  beg, end, num;  /* 模型顶点相关的参数 */

        /* MTL 内容 */
        leng_t  attr;   /* 材质索引号 */

} sWAVEFRONT_G;

/* OBJ 顶点结构 */
typedef struct
{
        int32u  idx[4];     /* 从1开始, 0表示不存在 */

} sWAVEFRONT_F;

/* OBJ 模型文件 */
typedef struct
{
        /* 材质文件 */
        ansi_t*         mtl;

        /* 空间坐标 */
        leng_t          n_v;
        vec3d_t*        p_v;

        /* 贴图坐标 */
        leng_t          n_vt;
        vec3d_t*        p_vt;

        /* 法线向量 */
        leng_t          n_vn;
        vec3d_t*        p_vn;

        /* 顶点索引 */
        leng_t          n_f;
        sWAVEFRONT_F*   p_f;

        /* 模型列表 */
        leng_t          n_g;
        sWAVEFRONT_G*   p_g;

        /* 材质列表 */
        leng_t          n_m;
        sWAVEFRONT_M*   p_m;

} sWAVEFRONT;

/* 基本的格式 */
CR_API bool_t   wfront_obj_load (sWAVEFRONT *obj, const ansi_t *str,
                                 bool_t swap_yz, bool_t neg_z);
CR_API bool_t   wfront_mtl_load (sWAVEFRONT *obj, const ansi_t *str);
CR_API void_t   wfront_obj_free (const sWAVEFRONT *obj);
CR_API bool_t   wfront_obj_combine (sWAVEFRONT *obj);
CR_API leng_t   wfront_gen_mesh2 (vec3d_t *xyz, vec3d_t *nrm, vec2d_t *uvw,
                        leng_t bpv1, leng_t bpv2, leng_t bpv3, void_t *ibuf,
                            leng_t *inum, const sWAVEFRONT *obj, leng_t idx);
CR_API leng_t   wfront_gen_mesh3 (vec3d_t *xyz, vec3d_t *nrm, vec3d_t *uvw,
                        leng_t bpv1, leng_t bpv2, leng_t bpv3, void_t *ibuf,
                            leng_t *inum, const sWAVEFRONT *obj, leng_t idx);
/* 二进制格式 */
CR_API bool_t   wfront_bobj_load (sWAVEFRONT *obj, iDATIN *datin);

/*****************************************************************************/
/*                              KlayGE MeshML                                */
/*****************************************************************************/

/* MeshML 材质结构 */
typedef struct
{
        uint_t  flags;
        fp32_t  d, ns, shine;
        vec3d_t ka, kd, ks, ke;
        ansi_t  *map_kd, *map_ks, *map_d, *bump;
        ansi_t  *map_ns, *map_ke, *gloss, *color;

} sMESHML_MTRL;

/* 颜色标志 */
#define MESHML_KA   1   /* 存在 Ka */
#define MESHML_KD   2   /* 存在 Kd */
#define MESHML_KS   4   /* 存在 Ks */
#define MESHML_KE   8   /* 存在 Ke */

/* MeshML 骨骼结构 */
typedef struct
{
        ansi_t  *name;
        sint_t  parent;
        vec4d_t real, dual;

} sMESHML_BONE;

/* MeshML 模型结构 */
typedef struct
{
        ansi_t  *name;
        int32u  *ibuf;
        leng_t  vnum, inum;
        leng_t  start, mtl_id;
        vec2d_t tc_bb_min, tc_bb_max;
        vec3d_t pos_bb_min, pos_bb_max;

} sMESHML_MESH;

/* MeshML 权重结构 */
typedef struct
{
        byte_t  jid[4];
        vec4d_t weight;

} sMESHML_WGHT;

/* MeshML 模型文件 */
typedef struct
{
        /* 版本号 */
        uint_t          ver;

        /* 顶点数据 */
        leng_t          n_v;
        vec3d_t*        p_v;
        vec4d_t*        p_vn;
        vec2d_t*        p_vt;
        sMESHML_WGHT*   p_jd;

        /* 网格数据 */
        leng_t          n_g;
        sMESHML_MESH*   p_g;

        /* 材质数据 */
        leng_t          n_m;
        sMESHML_MTRL*   p_m;

        /* 骨骼数据 */
        leng_t          n_b;
        sMESHML_BONE*   p_b;

} sMESHML;

CR_API bool_t   meshml_load (sMESHML *msh, const ansi_t *str);
CR_API void_t   meshml_free (const sMESHML *msh);

#endif  /* !__CR_GFX3_H__ */

/* 只使用微软自家的编译器 */
#if defined(_CR_CC_MSC_) && (_CR_CC_VER_ >= 1400) && !defined(_CR_OS_WINCE_)
/*****************************************************************************/
/*                              Direct3D8 API                                */
/*****************************************************************************/
#if     defined(_CR_USE_D3D8_) && (_CR_CC_VER_ < 1900) && \
        defined(_CR_OS_WIN32_)

#include "gfx3/d3d8api.h"
#include "gfx3/d3d8api.inl"

/*****************************************************************************/
/*                              Direct3D9 API                                */
/*****************************************************************************/
#elif   defined(_CR_USE_D3D9_)

#include "gfx3/d3d9api.h"
#include "gfx3/d3d9api.inl"

#endif  /* GFX3 API predefines */

#endif  /* _CR_CC_MSC_ && _CR_CC_VER_ && !_CR_OS_WINCE_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
