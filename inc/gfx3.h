/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-06-05  */
/*     #######          ###    ###      [GFX3]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-06-06  */
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

} vec4d_t;

/* 4x4 矩阵 */
typedef struct
{
        fp32_t  m[4 * 4];

} mat4x4_t;

/*****************************************************************************/
/*                              Direct3D8 API                                */
/*****************************************************************************/
#if     defined(_CR_USE_D3D8_)

#include <d3d8.h>
#include <d3dx8.h>
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "d3d8.lib")
    #pragma comment (lib, "d3dx8.lib")
    #pragma comment (lib, "dxguid8.lib")
#endif

/*****************************************************************************/
/*                              Direct3D9 API                                */
/*****************************************************************************/
#elif   defined(_CR_USE_D3D9_)

#include <d3d9.h>
#include <d3dx9.h>
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "d3d9.lib")
    #pragma comment (lib, "d3dx9.lib")
    #pragma comment (lib, "dxguid.lib")
#endif

/* D3D9 设备对象 */
typedef struct
{
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
        LPDIRECT3DVERTEXBUFFER9         vbuf;
        LPDIRECT3DINDEXBUFFER9          ibuf;
        LPDIRECT3DVERTEXDECLARATION9    decl;

} sD3D9_MESH;

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

/* D3D9 调用接口表 */
typedef struct
{
/* =============================== 设备对象 ================================ */

        /* 生成 */
        sD3D9_MAIN* (*create_main) (hwnd_t hwnd, bool_t full, uint_t width,
                            uint_t height, D3DFORMAT format, D3DFORMAT depth,
                                    D3DMULTISAMPLE_TYPE fsaa);
        /* 释放 */
        void_t  (*release_main) (sD3D9_MAIN *main);

        /* 重置 */
        bool_t  (*main_reset) (sD3D9_MAIN *main, bool_t full, uint_t width,
                            uint_t height, D3DFORMAT format, D3DFORMAT depth,
                                    D3DMULTISAMPLE_TYPE fsaa);

/* =============================== 网格对象 ================================ */

        /* 生成 */
        sD3D9_MESH* (*create_mesh_vb) (sD3D9_MAIN *main, uint_t vnum,
                        uint_t bpv, D3DPOOL pool, int32u usage, uint_t fvf,
                            LPD3DVERTEXELEMENT9 decl, const void_t *data);

        sD3D9_MESH* (*create_mesh_ib) (sD3D9_MAIN *main, uint_t inum,
                                    uint_t bpi, D3DPOOL pool, int32u usage,
                                            const void_t *data);

        sD3D9_MESH* (*create_mesh_vib) (sD3D9_MAIN *main, uint_t vnum,
                        uint_t bpv, uint_t inum, D3DPOOL vpool, int32u vusage,
                            D3DPOOL ipool, int32u iusage, uint_t fvf,
                            LPD3DVERTEXELEMENT9 decl, const void_t *vbuf,
                                        const void_t *ibuf);
        /* 释放 */
        void_t  (*release_mesh) (sD3D9_MESH *mesh);

/* =============================== 纹理对象 ================================ */

        /* 生成 2D */
        sD3D9_TEXR* (*create_tex2) (sD3D9_MAIN *main, uint_t width,
                            uint_t height, D3DFORMAT format, D3DPOOL pool,
                                    int32u usage, uint_t level);

        sD3D9_TEXR* (*create_tex2_fileA) (sD3D9_MAIN *main,
                            const ansi_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, cl32_t keycolor);

        sD3D9_TEXR* (*create_tex2_fileW) (sD3D9_MAIN *main,
                            const wide_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, cl32_t keycolor);

        sD3D9_TEXR* (*create_tex2_mem) (sD3D9_MAIN *main,
                            const void_t *data, leng_t size, D3DPOOL pool,
                                int32u usage, uint_t level, cl32_t keycolor);
        /* 生成 3D */
        sD3D9_TEXR* (*create_tex3) (sD3D9_MAIN *main, uint_t size,
                                    D3DFORMAT format, D3DPOOL pool,
                                    int32u usage, uint_t level);

        sD3D9_TEXR* (*create_tex3_fileA) (sD3D9_MAIN *main,
                            const ansi_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, cl32_t keycolor);

        sD3D9_TEXR* (*create_tex3_fileW) (sD3D9_MAIN *main,
                            const wide_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, cl32_t keycolor);

        sD3D9_TEXR* (*create_tex3_mem) (sD3D9_MAIN *main,
                            const void_t *data, leng_t size, D3DPOOL pool,
                                int32u usage, uint_t level, cl32_t keycolor);
        /* 生成 VOLUME */
        sD3D9_TEXR* (*create_texv) (sD3D9_MAIN *main, uint_t width,
                            uint_t height, uint_t depth, D3DFORMAT format,
                                D3DPOOL pool, int32u usage, uint_t level);

        sD3D9_TEXR* (*create_texv_fileA) (sD3D9_MAIN *main,
                            const ansi_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, cl32_t keycolor);

        sD3D9_TEXR* (*create_texv_fileW) (sD3D9_MAIN *main,
                            const wide_t *name, D3DPOOL pool, int32u usage,
                                    uint_t level, cl32_t keycolor);

        sD3D9_TEXR* (*create_texv_mem) (sD3D9_MAIN *main,
                            const void_t *data, leng_t size, D3DPOOL pool,
                                int32u usage, uint_t level, cl32_t keycolor);
        /* 释放 */
        void_t  (*release_texr) (sD3D9_TEXR *texr);

/* ========================================================================= */

} sD3D9_CALL;

/* 生成 D3D9 调用接口表 */
CR_API const sD3D9_CALL*    d3d9call_get (void_t);

/*****************************************************************************/
/*                              Direct3D11 API                               */
/*****************************************************************************/
#elif   defined(_CR_USE_D3D11_)

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "dxgi.lib")
    #pragma comment (lib, "d3d11.lib")
    #pragma comment (lib, "d3dx11.lib")
    #pragma comment (lib, "d3dx10.lib")
#endif

#endif  /* GFX3 API predefines */

#endif  /* !__CR_GFX3_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
