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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack D3D9 接口函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_D3D9API_CPP__
#define __CR_D3D9API_CPP__ 0xFB8C5CE8UL

#define _CR_USE_D3D9_
#include "gfx3.h"
#include "memlib.h"

/*****************************************************************************/
/*                                 接口实现                                  */
/*****************************************************************************/

#if defined(_CR_HAVE_D3D9_)
/*
=======================================
    生成设备对象
=======================================
*/
CR_API sD3D9_MAIN*
d3d9_create_main (
  __CR_IN__ hwnd_t              hwnd,
  __CR_IN__ bool_t              full,
  __CR_IN__ uint_t              width,
  __CR_IN__ uint_t              height,
  __CR_IN__ D3DFORMAT           format,
  __CR_IN__ D3DFORMAT           depth,
  __CR_IN__ bool_t              vsync,
  __CR_IN__ D3DMULTISAMPLE_TYPE fsaa
    )
{
    DWORD                   vp;
    HRESULT                 retc;
    sD3D9_MAIN*             rett;
    D3DDISPLAYMODE          mode;
    D3DPRESENT_PARAMETERS   parm;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_MAIN);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_main()", "struct_new() failure");
        return (NULL);
    }

    /* 建立 D3D9 对象 */
    rett->obj = Direct3DCreate9(D3D_SDK_VERSION);
    if (rett->obj == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_main()", "Direct3DCreate9() failure");
        goto _failure1;
    }
    retc = rett->obj->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_main()",
                "IDirect3D9::GetAdapterDisplayMode() failure");
        goto _failure2;
    }

    /* 设置后台缓冲参数 */
    if (format == D3DFMT_UNKNOWN)
        format = mode.Format;
    if (!full) {
        parm.BackBufferWidth = 0;
        parm.BackBufferHeight = 0;
    }
    else {
        mode.Format = format;
        parm.BackBufferWidth = width;
        parm.BackBufferHeight = height;
    }
    retc = rett->obj->CheckDeviceType(D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL, mode.Format, format, !full);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_main()", "IDirect3D9::CheckDeviceType() failure");
        goto _failure2;
    }
    parm.BackBufferFormat = format;
    parm.BackBufferCount = 1;

    /* 设置深度缓冲参数 */
    if (depth != D3DFMT_UNKNOWN) {
        retc = rett->obj->CheckDeviceFormat(D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL, mode.Format, D3DUSAGE_DEPTHSTENCIL,
                                    D3DRTYPE_SURFACE, depth);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_main()",
                    "IDirect3D9::CheckDeviceFormat() failure");
            goto _failure2;
        }
        retc = rett->obj->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,
                            D3DDEVTYPE_HAL, mode.Format, format, depth);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_main()",
                    "IDirect3D9::CheckDepthStencilMatch() failure");
            goto _failure2;
        }
        parm.EnableAutoDepthStencil = TRUE;
    }
    else {
        parm.EnableAutoDepthStencil = FALSE;
    }
    parm.AutoDepthStencilFormat = depth;

    /* 设置抗锯齿参数 */
    if (fsaa != D3DMULTISAMPLE_NONE) {
        retc = rett->obj->CheckDeviceMultiSampleType(
                        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, format, !full,
                                    fsaa, NULL);
        if (SUCCEEDED(retc)) {
            if (depth != D3DFMT_UNKNOWN) {
                retc = rett->obj->CheckDeviceMultiSampleType(
                        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, depth, !full,
                                    fsaa, NULL);
                if (FAILED(retc))
                    fsaa = D3DMULTISAMPLE_NONE;
            }
        }
        else {
            fsaa = D3DMULTISAMPLE_NONE;
        }
    }
    parm.MultiSampleType = fsaa;
    parm.MultiSampleQuality = 0;

    /* 设置其他参数 */
    parm.SwapEffect = D3DSWAPEFFECT_DISCARD;
    parm.hDeviceWindow = (HWND)hwnd;
    parm.Windowed = !full;
    parm.Flags = 0;
    parm.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    if (vsync)
        parm.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    else
        parm.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    /* 获取顶点的硬件加速能力 */
    retc = rett->obj->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                    &rett->cap);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_main()", "IDirect3D9::GetDeviceCaps() failure");
        goto _failure2;
    }

    /* 生成 D3D9 设备对象 */
    if (rett->cap.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    retc = rett->obj->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                   (HWND)hwnd, vp, &parm, &rett->dev);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_main()", "IDirect3D9::CreateDevice() failure");
        goto _failure2;
    }

    /* 没有 Z 缓冲关闭之 */
    rett->dev->SetRenderState(D3DRS_LIGHTING, FALSE);
    if (depth == D3DFMT_UNKNOWN) {
        rett->dev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
        rett->dev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    }
    rett->bak = NULL;
    struct_cpy(&rett->parm, &parm, D3DPRESENT_PARAMETERS);
    return (rett);

_failure2:
    rett->obj->Release();
_failure1:
    mem_free(rett);
    return (NULL);
}

/*
=======================================
    释放设备对象
=======================================
*/
CR_API void_t
d3d9_release_main (
  __CR_IN__ const sD3D9_MAIN*   main
    )
{
    TRY_XREL(main->bak)
    main->dev->Release();
    main->obj->Release();
    mem_free(main);
}

/*
=======================================
    重置设备对象
=======================================
*/
CR_API bool_t
d3d9_main_reset (
  __CR_IO__ sD3D9_MAIN*         main,
  __CR_IN__ bool_t              full,
  __CR_IN__ uint_t              width,
  __CR_IN__ uint_t              height,
  __CR_IN__ D3DFORMAT           format,
  __CR_IN__ D3DFORMAT           depth,
  __CR_IN__ bool_t              vsync,
  __CR_IN__ D3DMULTISAMPLE_TYPE fsaa
    )
{
    HRESULT                 retc;
    D3DDISPLAYMODE          mode;
    D3DPRESENT_PARAMETERS   parm;

    retc = main->obj->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_main_reset()",
                "IDirect3D9::GetAdapterDisplayMode() failure");
        return (FALSE);
    }

    /* 设置后台缓冲参数 */
    if (format == D3DFMT_UNKNOWN)
        format = mode.Format;
    if (!full) {
        parm.BackBufferWidth = 0;
        parm.BackBufferHeight = 0;
    }
    else {
        mode.Format = format;
        parm.BackBufferWidth = width;
        parm.BackBufferHeight = height;
    }
    retc = main->obj->CheckDeviceType(D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL, mode.Format, format, !full);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_main_reset()", "IDirect3D9::CheckDeviceType() failure");
        return (FALSE);
    }
    parm.BackBufferFormat = format;
    parm.BackBufferCount = 1;

    /* 设置深度缓冲参数 */
    if (depth != D3DFMT_UNKNOWN) {
        retc = main->obj->CheckDeviceFormat(D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL, mode.Format, D3DUSAGE_DEPTHSTENCIL,
                                    D3DRTYPE_SURFACE, depth);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_main_reset()",
                    "IDirect3D9::CheckDeviceFormat() failure");
            return (FALSE);
        }
        retc = main->obj->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,
                            D3DDEVTYPE_HAL, mode.Format, format, depth);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_main_reset()",
                    "IDirect3D9::CheckDepthStencilMatch() failure");
            return (FALSE);
        }
        parm.EnableAutoDepthStencil = TRUE;
    }
    else {
        parm.EnableAutoDepthStencil = FALSE;
    }
    parm.AutoDepthStencilFormat = depth;

    /* 设置抗锯齿参数 */
    if (fsaa != D3DMULTISAMPLE_NONE) {
        retc = main->obj->CheckDeviceMultiSampleType(
                        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, format, !full,
                                    fsaa, NULL);
        if (SUCCEEDED(retc)) {
            if (depth != D3DFMT_UNKNOWN) {
                retc = main->obj->CheckDeviceMultiSampleType(
                        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, depth, !full,
                                    fsaa, NULL);
                if (FAILED(retc))
                    fsaa = D3DMULTISAMPLE_NONE;
            }
        }
        else {
            fsaa = D3DMULTISAMPLE_NONE;
        }
    }
    parm.MultiSampleType = fsaa;
    parm.MultiSampleQuality = 0;

    /* 设置其他参数 */
    parm.SwapEffect = D3DSWAPEFFECT_DISCARD;
    parm.hDeviceWindow = main->parm.hDeviceWindow;
    parm.Windowed = !full;
    parm.Flags = 0;
    parm.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    if (vsync)
        parm.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    else
        parm.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    SAFE_XREL(main->bak)

    /* 重置 D3D9 设备对象 */
    retc = main->dev->Reset(&parm);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_main_reset()", "IDirect3DDevice9::Reset() failure");
        return (FALSE);
    }

    /* 没有 Z 缓冲关闭之 */
    main->dev->SetRenderState(D3DRS_LIGHTING, FALSE);
    if (depth == D3DFMT_UNKNOWN) {
        main->dev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
        main->dev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    }
    struct_cpy(&main->parm, &parm, D3DPRESENT_PARAMETERS);
    return (TRUE);
}

/*
=======================================
    保存后台缓冲
=======================================
*/
CR_API void_t
d3d9_rtt_enter (
  __CR_IO__ sD3D9_MAIN* main
    )
{
    if (main->bak != NULL)
        return;
    main->dev->GetRenderTarget(0, &main->bak);
}

/*
=======================================
    恢复后台缓冲
=======================================
*/
CR_API void_t
d3d9_rtt_leave (
  __CR_IO__ sD3D9_MAIN* main
    )
{
    if (main->bak == NULL)
        return;
    main->dev->SetRenderTarget(0, main->bak);
    main->bak->Release();
    main->bak = NULL;
}

/*
=======================================
    生成网格对象 (VB)
=======================================
*/
CR_API sD3D9_MESH*
d3d9_create_mesh_vb (
  __CR_IN__ sD3D9_MAIN*         main,
  __CR_IN__ uint_t              vnum,
  __CR_IN__ uint_t              bpv,
  __CR_IN__ D3DPOOL             pool,
  __CR_IN__ int32u              usage,
  __CR_IN__ uint_t              fvf,
  __CR_IN__ const void_t*       data
    )
{
    void_t*     copy;
    HRESULT     retc;
    sD3D9_MESH* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_MESH);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_mesh_vb()", "struct_new() failure");
        return (NULL);
    }
    struct_zero(rett, sD3D9_MESH);

    /* 生成 D3D9 VB 对象 */
    rett->vnum  = vnum;
    rett->nbpv  = bpv;
    rett->vsize = vnum * bpv;
    rett->fvf   = fvf;
    retc = main->dev->CreateVertexBuffer(rett->vsize, usage, fvf, pool,
                                         &rett->vbuf, NULL);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_mesh_vb()",
                "IDirect3DDevice9::CreateVertexBuffer() failure");
        mem_free(rett);
        return (NULL);
    }

    /* 根据需要初始化数据 */
    if (data != NULL) {
        rett->vbuf->Lock(0, rett->vsize, &copy, 0);
        mem_cpy(copy, data, rett->vsize);
        rett->vbuf->Unlock();
    }
    return (rett);
}

/*
=======================================
    生成网格对象 (IB)
=======================================
*/
CR_API sD3D9_MESH*
d3d9_create_mesh_ib (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ uint_t          inum,
  __CR_IN__ uint_t          bpi,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ const void_t*   data
    )
{
    void_t*     copy;
    HRESULT     retc;
    D3DFORMAT   ifmt;
    sD3D9_MESH* rett;

    /* 过滤参数 */
    if (bpi != 2 && bpi != 4) {
        err_set(__CR_D3D9API_CPP__, bpi,
                "d3d9_create_mesh_ib()", "invalid param: bpi");
        return (NULL);
    }

    /* 分配对象结构 */
    rett = struct_new(sD3D9_MESH);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_mesh_ib()", "struct_new() failure");
        return (NULL);
    }
    struct_zero(rett, sD3D9_MESH);

    /* 生成 D3D9 IB 对象 */
    rett->inum  = inum;
    rett->nbpi  = bpi;
    rett->isize = inum * bpi;
    rett->ntri  = inum / 3;
    ifmt = (bpi == 2) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
    retc = main->dev->CreateIndexBuffer(rett->isize, usage, ifmt, pool,
                                        &rett->ibuf, NULL);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_mesh_ib()",
                "IDirect3DDevice9::CreateIndexBuffer() failure");
        mem_free(rett);
        return (NULL);
    }

    /* 根据需要初始化数据 */
    if (data != NULL) {
        rett->ibuf->Lock(0, rett->isize, &copy, 0);
        mem_cpy(copy, data, rett->isize);
        rett->ibuf->Unlock();
    }
    return (rett);
}

/*
=======================================
    生成网格对象 (VB & IB)
=======================================
*/
CR_API sD3D9_MESH*
d3d9_create_mesh_vib (
  __CR_IN__ sD3D9_MAIN*         main,
  __CR_IN__ uint_t              vnum,
  __CR_IN__ uint_t              bpv,
  __CR_IN__ uint_t              inum,
  __CR_IN__ D3DPOOL             vpool,
  __CR_IN__ int32u              vusage,
  __CR_IN__ D3DPOOL             ipool,
  __CR_IN__ int32u              iusage,
  __CR_IN__ uint_t              fvf,
  __CR_IN__ const void_t*       vbuf,
  __CR_IN__ const void_t*       ibuf
    )
{
    void_t*     copy;
    HRESULT     retc;
    D3DFORMAT   ifmt;
    sD3D9_MESH* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_MESH);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_mesh_vib()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 VB 对象 */
    rett->vnum  = vnum;
    rett->nbpv  = bpv;
    rett->vsize = vnum * bpv;
    rett->fvf   = fvf;
    retc = main->dev->CreateVertexBuffer(rett->vsize, vusage, fvf, vpool,
                                         &rett->vbuf, NULL);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_mesh_vib()",
                "IDirect3DDevice9::CreateVertexBuffer() failure");
        goto _failure1;
    }

    /* 生成 D3D9 IB 对象 */
    rett->inum = inum;
    if (vnum <= 65535) {
        ifmt = D3DFMT_INDEX16;
        rett->nbpi = sizeof(int16u);
        rett->isize = inum * 2;
    }
    else {
        ifmt = D3DFMT_INDEX32;
        rett->nbpi = sizeof(int32u);
        rett->isize = inum * 4;
    }
    rett->ntri = inum / 3;
    retc = main->dev->CreateIndexBuffer(rett->isize, iusage, ifmt, ipool,
                                        &rett->ibuf, NULL);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_mesh_vib()",
                "IDirect3DDevice9::CreateIndexBuffer() failure");
        goto _failure2;
    }

    /* 根据需要初始化数据 */
    if (vbuf != NULL) {
        rett->vbuf->Lock(0, rett->vsize, &copy, 0);
        mem_cpy(copy, vbuf, rett->vsize);
        rett->vbuf->Unlock();
    }
    if (ibuf != NULL) {
        rett->ibuf->Lock(0, rett->isize, &copy, 0);
        mem_cpy(copy, ibuf, rett->isize);
        rett->ibuf->Unlock();
    }
    return (rett);

_failure2:
    rett->vbuf->Release();
_failure1:
    mem_free(rett);
    return (NULL);
}

/*
=======================================
    释放网格对象
=======================================
*/
CR_API void_t
d3d9_release_mesh (
  __CR_IN__ const sD3D9_MESH*   mesh
    )
{
    TRY_XREL(mesh->ibuf)
    TRY_XREL(mesh->vbuf)
    mem_free(mesh);
}

/*
=======================================
    设置 VB 数据
=======================================
*/
CR_API bool_t
d3d9_mesh_vb_set (
  __CR_IN__ sD3D9_MESH*     mesh,
  __CR_IN__ uint_t          start,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          count,
  __CR_IN__ int32u          flags
    )
{
    void_t* copy;
    HRESULT retc;

    if (mesh->vbuf == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_mesh_vb_set()", "invalid param: mesh");
        return (FALSE);
    }
    start *= mesh->nbpv;
    count *= mesh->nbpv;
    retc = mesh->vbuf->Lock(start, count, &copy, flags);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_mesh_vb_set()",
                "IDirect3DVertexBuffer9::Lock() failure");
        return (FALSE);
    }
    mem_cpy(copy, data, count);
    mesh->vbuf->Unlock();
    return (TRUE);
}

/*
=======================================
    设置 IB 数据
=======================================
*/
CR_API bool_t
d3d9_mesh_ib_set (
  __CR_IN__ sD3D9_MESH*     mesh,
  __CR_IN__ uint_t          start,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          count,
  __CR_IN__ int32u          flags
    )
{
    void_t* copy;
    HRESULT retc;

    if (mesh->ibuf == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_mesh_ib_set()", "invalid param: mesh");
        return (FALSE);
    }
    start *= mesh->nbpi;
    count *= mesh->nbpi;
    retc = mesh->ibuf->Lock(start, count, &copy, flags);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_mesh_ib_set()",
                "IDirect3DIndexBuffer9::Lock() failure");
        return (FALSE);
    }
    mem_cpy(copy, data, count);
    mesh->ibuf->Unlock();
    return (TRUE);
}

/*
=======================================
    生成 2D 纹理对象
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_tex2 (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ uint_t      width,
  __CR_IN__ uint_t      height,
  __CR_IN__ D3DFORMAT   format,
  __CR_IN__ D3DPOOL     pool,
  __CR_IN__ int32u      usage,
  __CR_IN__ uint_t      level
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_tex2()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 2D 纹理对象 */
    rett->face = 1;
    rett->info.Width = width;
    rett->info.Height = height;
    rett->info.Depth = 0;
    rett->info.MipLevels = level;
    rett->info.Format = format;
    rett->info.ResourceType = D3DRTYPE_TEXTURE;
    rett->info.ImageFileFormat = D3DXIFF_FORCE_DWORD;
    retc = main->dev->CreateTexture(width, height, level, usage, format,
                                    pool, &rett->obj.tex2, NULL);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_tex2()",
                "IDirect3DDevice9::CreateTexture() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 2D 纹理对象 (文件A)
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_tex2_fileA (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_tex2_fileA()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 2D 纹理对象 */
    rett->face = 1;
    retc = D3DXCreateTextureFromFileExA(main->dev, name, 0, 0,
                level, usage, D3DFMT_UNKNOWN, pool, D3DX_DEFAULT, mip_type,
                        keycolor, &rett->info, NULL, &rett->obj.tex2);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_tex2_fileA()",
                "D3DXCreateTextureFromFileExA() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 2D 纹理对象 (文件W)
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_tex2_fileW (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_tex2_fileW()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 2D 纹理对象 */
    rett->face = 1;
    retc = D3DXCreateTextureFromFileExW(main->dev, name, 0, 0,
                level, usage, D3DFMT_UNKNOWN, pool, D3DX_DEFAULT, mip_type,
                        keycolor, &rett->info, NULL, &rett->obj.tex2);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_tex2_fileW()",
                "D3DXCreateTextureFromFileExW() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 2D 纹理对象 (内存)
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_tex2_mem (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_tex2_mem()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 2D 纹理对象 */
    rett->face = 1;
    retc = D3DXCreateTextureFromFileInMemoryEx(main->dev, data,
                    (uint_t)size, 0, 0, level, usage, D3DFMT_UNKNOWN,
                        pool, D3DX_DEFAULT, mip_type, keycolor,
                            &rett->info, NULL, &rett->obj.tex2);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_tex2_mem()",
                "D3DXCreateTextureFromFileInMemoryEx() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 2D 纹理对象 (对象)
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_tex2_crh (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const sIMAGE*   image,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type
    )
{
    uint_t          line;
    leng_t          copy;
    leng_t          sbpl;
    byte_t*         sptr;
    byte_t*         dptr;
    HRESULT         retc;
    D3DFORMAT       tfmt;
    sD3D9_TEXR*     rett;
    D3DLOCKED_RECT  info;

    tfmt = (D3DFORMAT)image_crh_to_d3d(image->fmt);
    if (tfmt == D3DFMT_UNKNOWN) {
        err_set(__CR_D3D9API_CPP__, image->fmt,
                "d3d9_create_tex2_crh()", "invalid param: image");
        return (NULL);
    }
    rett = d3d9_create_tex2(main, image->position.ww,
                    image->position.hh, tfmt, pool, usage, level);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_tex2_crh()", "d3d9_create_tex2() failure");
        return (NULL);
    }

    /* 锁定纹理对象复制数据 */
    retc = rett->obj.tex2->LockRect(0, &info, NULL, 0);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_tex2_crh()",
                "IDirect3DTexture9::LockRect() failure");
        rett->obj.tex2->Release();
        mem_free(rett);
        return (NULL);
    }

    /* DXTC 格式直接复制, 否则逐行复制 */
    if (image->fmt <= CR_DXT5) {
        mem_cpy(info.pBits, image->data, image->size);
    }
    else {
        dptr = (byte_t*)info.pBits;
        if (!image->gdi) {
            sbpl = image->bpl;
            sptr = image->data;
        }
        else {
            sbpl = 0 - image->bpl;
            sptr = image->data + image->clip_win.y2 * image->bpl;
        }
        line = image->position.hh;
        copy = image->position.ww * image->bpc;
        for (; line != 0; line--) {
            mem_cpy(dptr, sptr, copy);
            sptr += sbpl;
            dptr += info.Pitch;
        }
    }
    rett->obj.tex2->UnlockRect(0);
    retc = D3DXFilterTexture(rett->obj.base, NULL, 0, mip_type);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_tex2_crh()", "D3DXFilterTexture() failure");
    }
    return (rett);
}

/*
=======================================
    生成 3D 纹理对象
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_tex3 (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ uint_t      size,
  __CR_IN__ D3DFORMAT   format,
  __CR_IN__ D3DPOOL     pool,
  __CR_IN__ int32u      usage,
  __CR_IN__ uint_t      level
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_tex3()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 3D 纹理对象 */
    rett->face = 6;
    rett->info.Width = size;
    rett->info.Height = size;
    rett->info.Depth = 0;
    rett->info.MipLevels = level;
    rett->info.Format = format;
    rett->info.ResourceType = D3DRTYPE_CUBETEXTURE;
    rett->info.ImageFileFormat = D3DXIFF_FORCE_DWORD;
    retc = main->dev->CreateCubeTexture(size, level, usage, format,
                                        pool, &rett->obj.tex3, NULL);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_tex3()",
                "IDirect3DDevice9::CreateCubeTexture() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 3D 纹理对象 (文件A)
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_tex3_fileA (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_tex3_fileA()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 3D 纹理对象 */
    rett->face = 6;
    retc = D3DXCreateCubeTextureFromFileExA(main->dev, name, 0, level,
                    usage, D3DFMT_UNKNOWN, pool, D3DX_DEFAULT, mip_type,
                        keycolor, &rett->info, NULL, &rett->obj.tex3);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_tex3_fileA()",
                "D3DXCreateCubeTextureFromFileExA() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 3D 纹理对象 (文件W)
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_tex3_fileW (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_tex3_fileW()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 3D 纹理对象 */
    rett->face = 6;
    retc = D3DXCreateCubeTextureFromFileExW(main->dev, name, 0, level,
                    usage, D3DFMT_UNKNOWN, pool, D3DX_DEFAULT, mip_type,
                        keycolor, &rett->info, NULL, &rett->obj.tex3);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_tex3_fileW()",
                "D3DXCreateCubeTextureFromFileExW() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 3D 纹理对象 (内存)
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_tex3_mem (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_tex3_mem()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 3D 纹理对象 */
    rett->face = 6;
    retc = D3DXCreateCubeTextureFromFileInMemoryEx(main->dev, data,
                    (uint_t)size, 0, level, usage, D3DFMT_UNKNOWN, pool,
                            D3DX_DEFAULT, mip_type, keycolor,
                            &rett->info, NULL, &rett->obj.tex3);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_tex3_mem()",
                "D3DXCreateCubeTextureFromFileInMemoryEx() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成体积纹理对象
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_texv (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ uint_t      width,
  __CR_IN__ uint_t      height,
  __CR_IN__ uint_t      depth,
  __CR_IN__ D3DFORMAT   format,
  __CR_IN__ D3DPOOL     pool,
  __CR_IN__ int32u      usage,
  __CR_IN__ uint_t      level
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_texv()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 体积纹理对象 */
    rett->face = 3;
    rett->info.Width = width;
    rett->info.Height = height;
    rett->info.Depth = depth;
    rett->info.MipLevels = level;
    rett->info.Format = format;
    rett->info.ResourceType = D3DRTYPE_VOLUMETEXTURE;
    rett->info.ImageFileFormat = D3DXIFF_FORCE_DWORD;
    retc = main->dev->CreateVolumeTexture(width, height, depth, level,
                                usage, format, pool, &rett->obj.texv, NULL);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_texv()",
                "IDirect3DDevice9::CreateVolumeTexture() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成体积纹理对象 (文件A)
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_texv_fileA (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_texv_fileA()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 体积纹理对象 */
    rett->face = 3;
    retc = D3DXCreateVolumeTextureFromFileExA(main->dev, name, 0, 0, 0,
                level, usage, D3DFMT_UNKNOWN, pool, D3DX_DEFAULT, mip_type,
                        keycolor, &rett->info, NULL, &rett->obj.texv);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_texv_fileA()",
                "D3DXCreateVolumeTextureFromFileExA() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成体积纹理对象 (文件W)
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_texv_fileW (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_texv_fileW()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 体积纹理对象 */
    rett->face = 3;
    retc = D3DXCreateVolumeTextureFromFileExW(main->dev, name, 0, 0, 0,
                level, usage, D3DFMT_UNKNOWN, pool, D3DX_DEFAULT, mip_type,
                        keycolor, &rett->info, NULL, &rett->obj.texv);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_texv_fileW()",
                "D3DXCreateVolumeTextureFromFileExW() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成体积纹理对象 (内存)
=======================================
*/
CR_API sD3D9_TEXR*
d3d9_create_texv_mem (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D9_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TEXR);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_texv_mem()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 体积纹理对象 */
    rett->face = 3;
    retc = D3DXCreateVolumeTextureFromFileInMemoryEx(main->dev, data,
                    (uint_t)size, 0, 0, 0, level, usage, D3DFMT_UNKNOWN,
                        pool, D3DX_DEFAULT, mip_type, keycolor,
                            &rett->info, NULL, &rett->obj.texv);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_texv_mem()",
                "D3DXCreateVolumeTextureFromFileInMemoryEx() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    释放纹理对象
=======================================
*/
CR_API void_t
d3d9_release_texr (
  __CR_IN__ const sD3D9_TEXR*   texr
    )
{
    texr->obj.base->Release();
    mem_free(texr);
}

/*
=======================================
    生成 VS 对象 (数据)
=======================================
*/
CR_API sD3D9_VSH*
d3d9_create_vs_data (
  __CR_IN__ sD3D9_MAIN*         main,
  __CR_IN__ LPD3DVERTEXELEMENT9 decl,
  __CR_IN__ const void_t*       data
    )
{
    HRESULT     retc;
    sD3D9_VSH*  rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_VSH);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_vs_data()", "struct_new() failure");
        return (NULL);
    }

    /* 生成顶点声明对象 */
    retc = main->dev->CreateVertexDeclaration(decl, &rett->decl);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_vs_data()",
                "IDirect3DDevice9::CreateVertexDeclaration() failure");
        goto _failure1;
    }

    /* 生成 D3D9 VS 对象 */
    retc = main->dev->CreateVertexShader((DWORD*)data, &rett->obj);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_vs_data()",
                "IDirect3DDevice9::CreateVertexShader() failure");
        goto _failure2;
    }
    return (rett);

_failure2:
    rett->decl->Release();
_failure1:
    mem_free(rett);
    return (NULL);
}

/*
=======================================
    生成 VS 对象 (文件A)
=======================================
*/
CR_API sD3D9_VSH*
d3d9_create_vs_fileA (
  __CR_IN__ sD3D9_MAIN*         main,
  __CR_IN__ LPD3DVERTEXELEMENT9 decl,
  __CR_IN__ const ansi_t*       name,
  __CR_IN__ int32u              flags,
  __CR_IN__ const ansi_t*       entry,
  __CR_IN__ const ansi_t*       profile
    )
{
    HRESULT         retc;
    sD3D9_VSH*      rett;
    LPD3DXBUFFER    code;

    /* 未指定版本使用最高版本编译 HLSL */
    if (entry != NULL && profile == NULL) {
        profile = D3DXGetVertexShaderProfile(main->dev);
        if (profile == NULL) {
            err_set(__CR_D3D9API_CPP__, CR_NULL,
                    "d3d9_create_vs_fileA()",
                    "device not support vertex shader");
            return (NULL);
        }
    }

    /* 不指定入口点使用汇编 */
    if (entry == NULL) {
        retc = D3DXAssembleShaderFromFileA(name, NULL, NULL,
                                           flags, &code, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_vs_fileA()",
                    "D3DXAssembleShaderFromFileA() failure");
            return (NULL);
        }
    }
    else {
        retc = D3DXCompileShaderFromFileA(name, NULL, NULL, entry, profile,
                                          flags, &code, NULL, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_vs_fileA()",
                    "D3DXCompileShaderFromFileA() failure");
            return (NULL);
        }
    }

    /* 生成 D3D9 VS 对象 */
    rett = d3d9_create_vs_data(main, decl, code->GetBufferPointer());
    code->Release();
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_vs_fileA()", "d3d9_create_vs_data() failure");
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 VS 对象 (文件W)
=======================================
*/
CR_API sD3D9_VSH*
d3d9_create_vs_fileW (
  __CR_IN__ sD3D9_MAIN*         main,
  __CR_IN__ LPD3DVERTEXELEMENT9 decl,
  __CR_IN__ const wide_t*       name,
  __CR_IN__ int32u              flags,
  __CR_IN__ const ansi_t*       entry,
  __CR_IN__ const ansi_t*       profile
    )
{
    HRESULT         retc;
    sD3D9_VSH*      rett;
    LPD3DXBUFFER    code;

    /* 未指定版本使用最高版本编译 HLSL */
    if (entry != NULL && profile == NULL) {
        profile = D3DXGetVertexShaderProfile(main->dev);
        if (profile == NULL) {
            err_set(__CR_D3D9API_CPP__, CR_NULL,
                    "d3d9_create_vs_fileW()",
                    "device not support vertex shader");
            return (NULL);
        }
    }

    /* 不指定入口点使用汇编 */
    if (entry == NULL) {
        retc = D3DXAssembleShaderFromFileW(name, NULL, NULL,
                                           flags, &code, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_vs_fileW()",
                    "D3DXAssembleShaderFromFileW() failure");
            return (NULL);
        }
    }
    else {
        retc = D3DXCompileShaderFromFileW(name, NULL, NULL, entry, profile,
                                          flags, &code, NULL, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_vs_fileW()",
                    "D3DXCompileShaderFromFileW() failure");
            return (NULL);
        }
    }

    /* 生成 D3D9 VS 对象 */
    rett = d3d9_create_vs_data(main, decl, code->GetBufferPointer());
    code->Release();
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_vs_fileW()", "d3d9_create_vs_data() failure");
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 VS 对象 (文本)
=======================================
*/
CR_API sD3D9_VSH*
d3d9_create_vs_text (
  __CR_IN__ sD3D9_MAIN*         main,
  __CR_IN__ LPD3DVERTEXELEMENT9 decl,
  __CR_IN__ const ansi_t*       text,
  __CR_IN__ int32u              flags,
  __CR_IN__ const ansi_t*       entry,
  __CR_IN__ const ansi_t*       profile
    )
{
    HRESULT         retc;
    sD3D9_VSH*      rett;
    LPD3DXBUFFER    code;

    /* 未指定版本使用最高版本编译 HLSL */
    if (entry != NULL && profile == NULL) {
        profile = D3DXGetVertexShaderProfile(main->dev);
        if (profile == NULL) {
            err_set(__CR_D3D9API_CPP__, CR_NULL,
                    "d3d9_create_vs_text()",
                    "device not support vertex shader");
            return (NULL);
        }
    }

    /* 不指定入口点使用汇编 */
    if (entry == NULL) {
        retc = D3DXAssembleShader(text, str_sizeA(text), NULL, NULL,
                                  flags, &code, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_vs_text()", "D3DXAssembleShader() failure");
            return (NULL);
        }
    }
    else {
        retc = D3DXCompileShader(text, str_sizeA(text), NULL, NULL, entry,
                                 profile, flags, &code, NULL, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_vs_text()", "D3DXCompileShader() failure");
            return (NULL);
        }
    }

    /* 生成 D3D9 VS 对象 */
    rett = d3d9_create_vs_data(main, decl, code->GetBufferPointer());
    code->Release();
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_vs_text()", "d3d9_create_vs_data() failure");
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    释放 VS 对象
=======================================
*/
CR_API void_t
d3d9_release_vs (
  __CR_IN__ sD3D9_MAIN*         main,
  __CR_IN__ const sD3D9_VSH*    vsh
    )
{
    CR_NOUSE(main);
    vsh->decl->Release();
    vsh->obj->Release();
    mem_free(vsh);
}

/*
=======================================
    生成 PS 对象 (数据)
=======================================
*/
CR_API sD3D9_PSH*
d3d9_create_ps_data (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const void_t*   data
    )
{
    HRESULT     retc;
    sD3D9_PSH*  rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_PSH);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_ps_data()", "struct_new() failure");
        return (NULL);
    }

    /* 生成 D3D9 PS 对象 */
    retc = main->dev->CreatePixelShader((DWORD*)data, &rett->obj);
    if (FAILED(retc)) {
        err_set(__CR_D3D9API_CPP__, retc,
                "d3d9_create_ps_data()",
                "IDirect3DDevice9::CreatePixelShader() failure");
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 PS 对象 (文件A)
=======================================
*/
CR_API sD3D9_PSH*
d3d9_create_ps_fileA (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ int32u          flags,
  __CR_IN__ const ansi_t*   entry,
  __CR_IN__ const ansi_t*   profile
    )
{
    HRESULT         retc;
    sD3D9_PSH*      rett;
    LPD3DXBUFFER    code;

    /* 未指定版本使用最高版本编译 HLSL */
    if (entry != NULL && profile == NULL) {
        profile = D3DXGetPixelShaderProfile(main->dev);
        if (profile == NULL) {
            err_set(__CR_D3D9API_CPP__, CR_NULL,
                    "d3d9_create_ps_fileA()",
                    "device not support pixel shader");
            return (NULL);
        }
    }

    /* 不指定入口点使用汇编 */
    if (entry == NULL) {
        retc = D3DXAssembleShaderFromFileA(name, NULL, NULL,
                                           flags, &code, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_ps_fileA()",
                    "D3DXAssembleShaderFromFileA() failure");
            return (NULL);
        }
    }
    else {
        retc = D3DXCompileShaderFromFileA(name, NULL, NULL, entry, profile,
                                          flags, &code, NULL, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_ps_fileA()",
                    "D3DXCompileShaderFromFileA() failure");
            return (NULL);
        }
    }

    /* 生成 D3D9 PS 对象 */
    rett = d3d9_create_ps_data(main, code->GetBufferPointer());
    code->Release();
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_ps_fileA()", "d3d9_create_ps_data() failure");
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 PS 对象 (文件W)
=======================================
*/
CR_API sD3D9_PSH*
d3d9_create_ps_fileW (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ int32u          flags,
  __CR_IN__ const ansi_t*   entry,
  __CR_IN__ const ansi_t*   profile
    )
{
    HRESULT         retc;
    sD3D9_PSH*      rett;
    LPD3DXBUFFER    code;

    /* 未指定版本使用最高版本编译 HLSL */
    if (entry != NULL && profile == NULL) {
        profile = D3DXGetPixelShaderProfile(main->dev);
        if (profile == NULL) {
            err_set(__CR_D3D9API_CPP__, CR_NULL,
                    "d3d9_create_ps_fileW()",
                    "device not support pixel shader");
            return (NULL);
        }
    }

    /* 不指定入口点使用汇编 */
    if (entry == NULL) {
        retc = D3DXAssembleShaderFromFileW(name, NULL, NULL,
                                           flags, &code, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_ps_fileW()",
                    "D3DXAssembleShaderFromFileW() failure");
            return (NULL);
        }
    }
    else {
        retc = D3DXCompileShaderFromFileW(name, NULL, NULL, entry, profile,
                                          flags, &code, NULL, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_ps_fileW()",
                    "D3DXCompileShaderFromFileW() failure");
            return (NULL);
        }
    }

    /* 生成 D3D9 PS 对象 */
    rett = d3d9_create_ps_data(main, code->GetBufferPointer());
    code->Release();
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_ps_fileW()", "d3d9_create_ps_data() failure");
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 PS 对象 (文本)
=======================================
*/
CR_API sD3D9_PSH*
d3d9_create_ps_text (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ int32u          flags,
  __CR_IN__ const ansi_t*   entry,
  __CR_IN__ const ansi_t*   profile
    )
{
    HRESULT         retc;
    sD3D9_PSH*      rett;
    LPD3DXBUFFER    code;

    /* 未指定版本使用最高版本编译 HLSL */
    if (entry != NULL && profile == NULL) {
        profile = D3DXGetPixelShaderProfile(main->dev);
        if (profile == NULL) {
            err_set(__CR_D3D9API_CPP__, CR_NULL,
                    "d3d9_create_ps_text()",
                    "device not support pixel shader");
            return (NULL);
        }
    }

    /* 不指定入口点使用汇编 */
    if (entry == NULL) {
        retc = D3DXAssembleShader(text, str_sizeA(text), NULL, NULL,
                                  flags, &code, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_ps_text()", "D3DXAssembleShader() failure");
            return (NULL);
        }
    }
    else {
        retc = D3DXCompileShader(text, str_sizeA(text), NULL, NULL, entry,
                                 profile, flags, &code, NULL, NULL);
        if (FAILED(retc)) {
            err_set(__CR_D3D9API_CPP__, retc,
                    "d3d9_create_ps_text()", "D3DXCompileShader() failure");
            return (NULL);
        }
    }

    /* 生成 D3D9 PS 对象 */
    rett = d3d9_create_ps_data(main, code->GetBufferPointer());
    code->Release();
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_ps_text()", "d3d9_create_ps_data() failure");
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    释放 PS 对象
=======================================
*/
CR_API void_t
d3d9_release_ps (
  __CR_IN__ sD3D9_MAIN*         main,
  __CR_IN__ const sD3D9_PSH*    psh
    )
{
    CR_NOUSE(main);
    psh->obj->Release();
    mem_free(psh);
}

/*
=======================================
    生成变换对象
=======================================
*/
CR_API sD3D9_TRAN*
d3d9_create_tran (
  __CR_IN__ fp32_t  fovy,
  __CR_IN__ sint_t  width,
  __CR_IN__ sint_t  height
    )
{
    sD3D9_TRAN* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D9_TRAN);
    if (rett == NULL) {
        err_set(__CR_D3D9API_CPP__, CR_NULL,
                "d3d9_create_tran()", "struct_new() failure");
        return (NULL);
    }

    /* 初始化成员 */
    rett->fovy = fovy * CR_DTOR;
    rett->aspect = (fp32_t)width / height;
    rett->znear = 1.0f;
    rett->zfar = 100000.0f;
    D3DXMatrixPerspectiveFovLH(&rett->proj, rett->fovy, rett->aspect,
                                rett->znear, rett->zfar);
    D3DXMatrixTranspose(&rett->tproj, &rett->proj);
    rett->eye.x =  0.0f;
    rett->eye.y =  0.0f;
    rett->eye.z = -100.0f;
    rett->eye.w =  1.0f;
    rett->lookat.x = 0.0f;
    rett->lookat.y = 0.0f;
    rett->lookat.z = 0.0f;
    rett->lookat.w = 1.0f;
    rett->upvec.x = 0.0f;
    rett->upvec.y = 1.0f;
    rett->upvec.z = 0.0f;
    rett->upvec.w = 1.0f;
    D3DXMatrixLookAtLH(&rett->view, (D3DXVECTOR3*)(&rett->eye),
                                    (D3DXVECTOR3*)(&rett->lookat),
                                    (D3DXVECTOR3*)(&rett->upvec));
    D3DXMatrixTranspose(&rett->tview, &rett->view);
    rett->view_port.X = 0;
    rett->view_port.Y = 0;
    rett->view_port.Width  = width;
    rett->view_port.Height = height;
    rett->view_port.MinZ = 0.0f;
    rett->view_port.MaxZ = 1.0f;
    D3DXMatrixIdentity(&rett->world);
    D3DXMatrixIdentity(&rett->tworld);
    D3DXMatrixMultiply(&rett->wvp, &rett->world, &rett->view);
    D3DXMatrixMultiplyTranspose(&rett->wvp, &rett->wvp, &rett->proj);
    return (rett);
}

/*
=======================================
    释放变换对象
=======================================
*/
CR_API void_t
d3d9_release_tran (
  __CR_IN__ const sD3D9_TRAN*   tran
    )
{
    mem_free(tran);
}

/*
=======================================
    更新世界变换
=======================================
*/
CR_API void_t
d3d9_tran_update_wrld (
  __CR_IO__ sD3D9_TRAN* tran
    )
{
    D3DXMatrixTranspose(&tran->tworld, &tran->world);
}

/*
=======================================
    更新观察变换
=======================================
*/
CR_API void_t
d3d9_tran_update_view (
  __CR_IO__ sD3D9_TRAN* tran
    )
{
    D3DXMatrixLookAtLH(&tran->view, (D3DXVECTOR3*)(&tran->eye),
                                    (D3DXVECTOR3*)(&tran->lookat),
                                    (D3DXVECTOR3*)(&tran->upvec));
    D3DXMatrixTranspose(&tran->tview, &tran->view);
}

/*
=======================================
    更新投影变换
=======================================
*/
CR_API void_t
d3d9_tran_update_proj (
  __CR_IO__ sD3D9_TRAN* tran
    )
{
    D3DXMatrixPerspectiveFovLH(&tran->proj, tran->fovy, tran->aspect,
                                tran->znear, tran->zfar);
    D3DXMatrixTranspose(&tran->tproj, &tran->proj);
}

/*
=======================================
    更新视口变换
=======================================
*/
CR_API void_t
d3d9_tran_update_port (
  __CR_IO__ sD3D9_TRAN* tran
    )
{
    sint_t  width, height;

    width  = tran->view_port.Width;
    height = tran->view_port.Height;
    tran->aspect = (fp32_t)width / height;
    d3d9_tran_update_proj(tran);
}

/*
=======================================
    更新 WVP 变换
=======================================
*/
CR_API void_t
d3d9_tran_update_mwvp (
  __CR_IO__ sD3D9_TRAN* tran
    )
{
    D3DXMatrixMultiply(&tran->wvp, &tran->world, &tran->view);
    D3DXMatrixMultiplyTranspose(&tran->wvp, &tran->wvp, &tran->proj);
}

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

static const sD3D9_CALL _rom_ s_d3d9call =
{
    /* 设备对象 */
    d3d9_create_main,
    d3d9_release_main,
    d3d9_main_reset,
    d3d9_rtt_enter,
    d3d9_rtt_leave,

    /* 网格对象 */
    d3d9_create_mesh_vb,
    d3d9_create_mesh_ib,
    d3d9_create_mesh_vib,
    d3d9_release_mesh,
    d3d9_mesh_vb_set,
    d3d9_mesh_ib_set,

    /* 纹理对象 */
    d3d9_create_tex2,
    d3d9_create_tex2_fileA,
    d3d9_create_tex2_fileW,
    d3d9_create_tex2_mem,
    d3d9_create_tex2_crh,
    d3d9_create_tex3,
    d3d9_create_tex3_fileA,
    d3d9_create_tex3_fileW,
    d3d9_create_tex3_mem,
    d3d9_create_texv,
    d3d9_create_texv_fileA,
    d3d9_create_texv_fileW,
    d3d9_create_texv_mem,
    d3d9_release_texr,

    /* VS 对象 */
    d3d9_create_vs_data,
    d3d9_create_vs_fileA,
    d3d9_create_vs_fileW,
    d3d9_create_vs_text,
    d3d9_release_vs,

    /* PS 对象 */
    d3d9_create_ps_data,
    d3d9_create_ps_fileA,
    d3d9_create_ps_fileW,
    d3d9_create_ps_text,
    d3d9_release_ps,

    /* 变换对象 */
    d3d9_create_tran,
    d3d9_release_tran,
    d3d9_tran_update_wrld,
    d3d9_tran_update_view,
    d3d9_tran_update_proj,
    d3d9_tran_update_port,
    d3d9_tran_update_mwvp,
};

/*
=======================================
    生成 D3D9 调用接口表
=======================================
*/
CR_API const sD3D9_CALL*
d3d9call_get (void_t)
{
    return (&s_d3d9call);
}

#endif  /* _CR_HAVE_D3D9_ */

#endif  /* !__CR_D3D9API_CPP__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/