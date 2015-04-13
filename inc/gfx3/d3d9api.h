/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-06-13  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack D3D9 接口头文件 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_D3D9API_H__
#define __CR_D3D9API_H__

#include <d3d9.h>
#include <d3dx9.h>
#define _CR_HAVE_D3D9_

/* D3D9 设备对象 */
typedef struct
{
        D3DCAPS9                cap;
        LPDIRECT3D9             obj;
        LPDIRECT3DDEVICE9       dev;
        LPDIRECT3DSURFACE9      bak;
        D3DPRESENT_PARAMETERS   parm;

} sD3D9_MAIN;

/* D3D9 网格对象 */
typedef struct
{
        uint_t      vnum, nbpv, vsize, fvf;
        uint_t      inum, nbpi, isize, ntri;
        LPDIRECT3DVERTEXBUFFER9     vbuf;
        LPDIRECT3DINDEXBUFFER9      ibuf;

} sD3D9_MESH;

/* D3D9X 网格对象 */
typedef struct
{
        DWORD           nattr;
        void_t*         xcall;
        LPD3DXMESH      xmesh;
        LPD3DXBUFFER    adjcy;
        LPD3DXBUFFER    xattr;
        LPD3DXBUFFER    xffct;
        D3DXVECTOR3     center;
        FLOAT           radius;
        D3DXVECTOR3     min, max;

} sD3D9_XMSH;

/* D3D9 纹理对象 */
typedef struct
{
        uint_t          face;
        D3DXIMAGE_INFO  info;

        union {
                LPDIRECT3DTEXTURE9          tex2;
                LPDIRECT3DCUBETEXTURE9      tex3;
                LPDIRECT3DVOLUMETEXTURE9    texv;
                LPDIRECT3DBASETEXTURE9      base;
        } obj;

} sD3D9_TEXR;

/* D3D9 VSH 对象 */
typedef struct
{
        LPDIRECT3DVERTEXSHADER9         obj;
        LPDIRECT3DVERTEXDECLARATION9    decl;
        bool_t                          copy;

} sD3D9_VSH;

/* D3D9 PSH 对象 */
typedef struct
{
        LPDIRECT3DPIXELSHADER9  obj;

} sD3D9_PSH;

/* D3D9 变换对象 */
typedef struct
{
        D3DXMATRIXA16   world, view, proj, one;
        D3DXMATRIXA16   tworld, tview, tproj, wvp;
        vec4d_t         eye, lookat, upvec, vvec;
        fp32_t          fovy, aspect, znear, zfar;
        D3DVIEWPORT9    view_port;

} sD3D9_TRAN;

/* D3D9 调用接口表 */
typedef struct
{
/* =============================== 设备对象 ================================ */

        /* 生成 */
        sD3D9_MAIN* (*create_main) (HWND hwnd, bool_t full, uint_t width,
                            uint_t height, D3DFORMAT format, D3DFORMAT depth,
                                bool_t vsync, D3DMULTISAMPLE_TYPE fsaa);
        /* 释放 */
        void_t  (*release_main) (const sD3D9_MAIN *main);

        /* 重置 */
        bool_t  (*main_reset) (sD3D9_MAIN *main, bool_t full, uint_t width,
                            uint_t height, D3DFORMAT format, D3DFORMAT depth,
                                bool_t vsync, D3DMULTISAMPLE_TYPE fsaa);
        /* RTT */
        void_t  (*rtt_enter) (sD3D9_MAIN *main);
        void_t  (*rtt_leave) (sD3D9_MAIN *main);

/* =============================== 网格对象 ================================ */

        /* 生成 */
        sD3D9_MESH* (*create_mesh_vb) (sD3D9_MAIN *main, uint_t vnum,
                        uint_t bpv, D3DPOOL pool, int32u usage, uint_t fvf,
                                       const void_t *data);

        sD3D9_MESH* (*create_mesh_ib) (sD3D9_MAIN *main, uint_t inum,
                                    uint_t bpi, D3DPOOL pool, int32u usage,
                                            const void_t *data);

        sD3D9_MESH* (*create_mesh_vib) (sD3D9_MAIN *main, uint_t vnum,
                        uint_t bpv, uint_t inum, D3DPOOL vpool, int32u vusage,
                            D3DPOOL ipool, int32u iusage, uint_t fvf,
                            const void_t *vbuf, const void_t *ibuf);
        /* 释放 */
        void_t  (*release_mesh) (const sD3D9_MESH *mesh);

        /* 设置 */
        bool_t  (*mesh_vb_set) (sD3D9_MESH *mesh, uint_t start,
                                const void_t *data, uint_t count,
                                int32u flags);

        bool_t  (*mesh_ib_set) (sD3D9_MESH *mesh, uint_t start,
                                const void_t *data, uint_t count,
                                int32u flags);
        /* X 网格 */
        sD3D9_XMSH* (*create_xmsh_fileA) (sD3D9_MAIN *main, int32u flags,
                                          const ansi_t *name);

        sD3D9_XMSH* (*create_xmsh_fileW) (sD3D9_MAIN *main, int32u flags,
                                          const wide_t *name);

        sD3D9_XMSH* (*create_xmsh_mem) (sD3D9_MAIN *main, int32u flags,
                                        const void_t *data, leng_t size);

        void_t  (*release_xmsh) (sD3D9_XMSH *xmsh);

/* =============================== 纹理对象 ================================ */

        /* 生成 2D */
        sD3D9_TEXR* (*create_tex2) (sD3D9_MAIN *main, uint_t width,
                            uint_t height, D3DFORMAT format, D3DPOOL pool,
                                    int32u usage, uint_t level);

        sD3D9_TEXR* (*create_tex2_fileA) (sD3D9_MAIN *main,
                            const ansi_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, int32u mip_type,
                                            cl32_t keycolor);

        sD3D9_TEXR* (*create_tex2_fileW) (sD3D9_MAIN *main,
                            const wide_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, int32u mip_type,
                                            cl32_t keycolor);

        sD3D9_TEXR* (*create_tex2_mem) (sD3D9_MAIN *main,
                            const void_t *data, leng_t size, D3DPOOL pool,
                                int32u usage, uint_t level, int32u mip_type,
                                            cl32_t keycolor);

        sD3D9_TEXR* (*create_tex2_crh) (sD3D9_MAIN *main,
                            const sIMAGE *image, D3DPOOL pool, int32u usage,
                                    uint_t level, int32u mip_type);

        bool_t  (*tex2_image_set) (sD3D9_TEXR *texr, const sIMAGE *image,
                                   bool_t mip_map, int32u mip_type);
        /* 生成 3D */
        sD3D9_TEXR* (*create_tex3) (sD3D9_MAIN *main, uint_t size,
                                    D3DFORMAT format, D3DPOOL pool,
                                    int32u usage, uint_t level);

        sD3D9_TEXR* (*create_tex3_fileA) (sD3D9_MAIN *main,
                            const ansi_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, int32u mip_type,
                                            cl32_t keycolor);

        sD3D9_TEXR* (*create_tex3_fileW) (sD3D9_MAIN *main,
                            const wide_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, int32u mip_type,
                                            cl32_t keycolor);

        sD3D9_TEXR* (*create_tex3_mem) (sD3D9_MAIN *main,
                            const void_t *data, leng_t size, D3DPOOL pool,
                                int32u usage, uint_t level, int32u mip_type,
                                            cl32_t keycolor);
        /* 生成 VOLUME */
        sD3D9_TEXR* (*create_texv) (sD3D9_MAIN *main, uint_t width,
                            uint_t height, uint_t depth, D3DFORMAT format,
                                D3DPOOL pool, int32u usage, uint_t level);

        sD3D9_TEXR* (*create_texv_fileA) (sD3D9_MAIN *main,
                            const ansi_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, int32u mip_type,
                                            cl32_t keycolor);

        sD3D9_TEXR* (*create_texv_fileW) (sD3D9_MAIN *main,
                            const wide_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, int32u mip_type,
                                            cl32_t keycolor);

        sD3D9_TEXR* (*create_texv_mem) (sD3D9_MAIN *main,
                            const void_t *data, leng_t size, D3DPOOL pool,
                                int32u usage, uint_t level, int32u mip_type,
                                            cl32_t keycolor);
        /* 释放 */
        void_t  (*release_texr) (const sD3D9_TEXR *texr);

/* =============================== V.S.对象 ================================ */

        /* 生成 */
        sD3D9_VSH*  (*create_vs_data) (sD3D9_MAIN *main,
                        const D3DVERTEXELEMENT9 *decl, const void_t *data);

        sD3D9_VSH*  (*create_vs_fileA) (sD3D9_MAIN *main,
                        const D3DVERTEXELEMENT9 *decl, const ansi_t *name,
                                    int32u flags, const ansi_t *entry,
                                        const ansi_t *profile);

        sD3D9_VSH*  (*create_vs_fileW) (sD3D9_MAIN *main,
                        const D3DVERTEXELEMENT9 *decl, const wide_t *name,
                                    int32u flags, const ansi_t *entry,
                                        const ansi_t *profile);

        sD3D9_VSH*  (*create_vs_text) (sD3D9_MAIN *main,
                        const D3DVERTEXELEMENT9 *decl, const ansi_t *text,
                                    int32u flags, const ansi_t *entry,
                                        const ansi_t *profile);
        /* 释放 */
        void_t  (*release_vs) (sD3D9_MAIN *main, const sD3D9_VSH *vsh);

/* =============================== P.S.对象 ================================ */

        /* 生成 */
        sD3D9_PSH*  (*create_ps_data) (sD3D9_MAIN *main, const void_t *data);

        sD3D9_PSH*  (*create_ps_fileA) (sD3D9_MAIN *main, const ansi_t *name,
                                        int32u flags, const ansi_t *entry,
                                        const ansi_t *profile);

        sD3D9_PSH*  (*create_ps_fileW) (sD3D9_MAIN *main, const wide_t *name,
                                        int32u flags, const ansi_t *entry,
                                        const ansi_t *profile);

        sD3D9_PSH*  (*create_ps_text) (sD3D9_MAIN *main, const ansi_t *text,
                                       int32u flags, const ansi_t *entry,
                                       const ansi_t *profile);
        /* 释放 */
        void_t  (*release_ps) (sD3D9_MAIN *main, const sD3D9_PSH *psh);

/* =============================== 变换对象 ================================ */

        /* 生成 */
        sD3D9_TRAN* (*create_tran) (fp32_t fovy, sint_t width, sint_t height);

        /* 释放 */
        void_t  (*release_tran) (const sD3D9_TRAN *tran);

        /* 更新 */
        void_t  (*tran_upd_wrld) (sD3D9_TRAN *tran);
        void_t  (*tran_upd_view) (sD3D9_TRAN *tran);
        void_t  (*tran_upd_proj) (sD3D9_TRAN *tran);
        void_t  (*tran_upd_port) (sD3D9_TRAN *tran);
        void_t  (*tran_upd_mwvp) (sD3D9_TRAN *tran);

        /* 信息 */
        void_t  (*tran_pickup) (sD3D9_TRAN *tran, sRADIAL *dest,
                                sint_t scn_x, sint_t scn_y);

        void_t  (*tran_convert) (sD3D9_TRAN *tran, vec2d_t *dest,
                                 const vec3d_t *space);

        void_t  (*tran_frustum) (sD3D9_TRAN *tran, sFRUSTUM *dest,
                                 fp32_t bias);
        /* 变换 */
        void_t  (*tran_billboardv) (sD3D9_TRAN *tran);
        void_t  (*tran_billboardh) (sD3D9_TRAN *tran);
        void_t  (*tran_wrld_clear) (sD3D9_TRAN *tran);
        void_t  (*tran_wrld_rtmul) (sD3D9_TRAN *tran, const mat4x4_t *mats,
                                    uint_t count);

/* ========================================================================= */

        /* 变换 */
        void_t  (*util_make_tran1) (mat4x4_t *mat, const vec3d_t *scale,
                                    fp32_t pitch, fp32_t yaw, fp32_t roll,
                                    const vec3d_t *move);

        void_t  (*util_make_tran2) (mat4x4_t *mat, const vec3d_t *scale,
                                    const vec3d_t *axis, fp32_t angle,
                                    const vec3d_t *move);

        void_t  (*util_make_tran3) (mat4x4_t *mat, const vec3d_t *scale,
                                    const vec4d_t *quat, const vec3d_t *move);

        void_t  (*util_tran_vec3d) (vec3d_t *output, const vec3d_t *input,
                                    const mat4x4_t *mat);
        /* 矩阵 */
        bool_t  (*util_matx_inverse) (mat4x4_t *dst, const mat4x4_t *src);
        void_t  (*util_matx_transpose) (mat4x4_t *dst, const mat4x4_t *src);

        /* 求交 */
        bool_t  (*util_intersect_aabb) (const sAABB *aabb,
                                        const sRADIAL *ray);

        bool_t  (*util_intersect_ball) (const sSPHERE *ball,
                                        const sRADIAL *ray);

        bool_t  (*util_intersect_tri) (sINTERSECT *ret, const vec3d_t *p0,
                                       const vec3d_t *p1, const vec3d_t *p2,
                                       const sRADIAL *ray);
        /* 精灵 */
        LPD3DXSPRITE    (*util_create_sprite) (sD3D9_MAIN *main);

        /* 字体 */
        LPD3DXFONT  (*util_create_fontA) (sD3D9_MAIN *main,
                                    const D3DXFONT_DESCA *desc);
        LPD3DXFONT  (*util_create_fontW) (sD3D9_MAIN *main,
                                    const D3DXFONT_DESCW *desc);

/* ========================================================================= */

} sD3D9_CALL;

/* 获取 D3D9 调用接口表 */
CR_API const sD3D9_CALL*    d3d9call_get (void_t);

#endif  /* !__CR_D3D9API_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
