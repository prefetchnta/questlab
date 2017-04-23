/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-06-17  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack D3D8 接口函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#define _CR_USE_D3D8_
#include "gfx3.h"
#include "memlib.h"
#include "strlib.h"
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "d3d8.lib")
    #pragma comment (lib, "d3dx8.lib")
    #pragma comment (lib, "dxguid8.lib")
#endif

/*****************************************************************************/
/*                                 接口实现                                  */
/*****************************************************************************/

#if defined(_CR_HAVE_D3D8_)
/*
=======================================
    生成设备对象
=======================================
*/
CR_API sD3D8_MAIN*
d3d8_create_main (
  __CR_IN__ HWND                hwnd,
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
    sD3D8_MAIN*             rett;
    D3DDISPLAYMODE          mode;
    D3DPRESENT_PARAMETERS   parm;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_MAIN);
    if (rett == NULL)
        return (NULL);

    /* 建立 D3D8 对象 */
    rett->obj = Direct3DCreate8(D3D_SDK_VERSION);
    if (rett->obj == NULL)
        goto _failure1;
    retc = rett->obj->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
    if (FAILED(retc))
        goto _failure2;

    /* 设置后台缓冲参数 */
    if (format == D3DFMT_UNKNOWN)
        format = mode.Format;
    parm.BackBufferWidth = width;
    parm.BackBufferHeight = height;
    if (full) mode.Format = format;
    retc = rett->obj->CheckDeviceType(D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL, mode.Format, format, !full);
    if (FAILED(retc))
        goto _failure2;
    parm.BackBufferFormat = format;
    parm.BackBufferCount = 1;

    /* 设置深度缓冲参数 */
    if (depth != D3DFMT_UNKNOWN) {
        retc = rett->obj->CheckDeviceFormat(D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL, mode.Format, D3DUSAGE_DEPTHSTENCIL,
                                    D3DRTYPE_SURFACE, depth);
        if (FAILED(retc))
            goto _failure2;
        retc = rett->obj->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,
                            D3DDEVTYPE_HAL, mode.Format, format, depth);
        if (FAILED(retc))
            goto _failure2;
        parm.EnableAutoDepthStencil = TRUE;
    }
    else {
        parm.EnableAutoDepthStencil = FALSE;
    }
    parm.AutoDepthStencilFormat = depth;

    /* 设置抗锯齿参数 */
    if (fsaa != D3DMULTISAMPLE_NONE) {
        retc = rett->obj->CheckDeviceMultiSampleType(
                    D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, format, !full, fsaa);
        if (SUCCEEDED(retc)) {
            if (depth != D3DFMT_UNKNOWN) {
                retc = rett->obj->CheckDeviceMultiSampleType(
                    D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, depth, !full, fsaa);
                if (FAILED(retc))
                    fsaa = D3DMULTISAMPLE_NONE;
            }
        }
        else {
            fsaa = D3DMULTISAMPLE_NONE;
        }
    }
    parm.MultiSampleType = fsaa;

    /* 设置其他参数 */
    parm.SwapEffect = D3DSWAPEFFECT_DISCARD;
    parm.hDeviceWindow = hwnd;
    parm.Windowed = !full;
    parm.Flags = 0;
    parm.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    if (!full) {
        parm.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    }
    else {
        parm.FullScreen_PresentationInterval = vsync ?
                D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    /* 获取顶点的硬件加速能力 */
    retc = rett->obj->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                    &rett->cap);
    if (FAILED(retc))
        goto _failure2;

    /* 生成 D3D8 设备对象 */
    if (rett->cap.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    retc = rett->obj->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                   hwnd, vp, &parm, &rett->dev);
    if (FAILED(retc))
        goto _failure2;

    /* 没有 Z 缓冲关闭之 */
    rett->dev->SetRenderState(D3DRS_LIGHTING, FALSE);
    if (depth == D3DFMT_UNKNOWN) {
        rett->dev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
        rett->dev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    }
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
d3d8_release_main (
  __CR_IN__ const sD3D8_MAIN*   main
    )
{
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
d3d8_main_reset (
  __CR_IO__ sD3D8_MAIN*         main,
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
    if (FAILED(retc))
        return (FALSE);

    /* 设置后台缓冲参数 */
    if (format == D3DFMT_UNKNOWN)
        format = mode.Format;
    parm.BackBufferWidth = width;
    parm.BackBufferHeight = height;
    if (full) mode.Format = format;
    retc = main->obj->CheckDeviceType(D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL, mode.Format, format, !full);
    if (FAILED(retc))
        return (FALSE);
    parm.BackBufferFormat = format;
    parm.BackBufferCount = 1;

    /* 设置深度缓冲参数 */
    if (depth != D3DFMT_UNKNOWN) {
        retc = main->obj->CheckDeviceFormat(D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL, mode.Format, D3DUSAGE_DEPTHSTENCIL,
                                    D3DRTYPE_SURFACE, depth);
        if (FAILED(retc))
            return (FALSE);
        retc = main->obj->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,
                            D3DDEVTYPE_HAL, mode.Format, format, depth);
        if (FAILED(retc))
            return (FALSE);
        parm.EnableAutoDepthStencil = TRUE;
    }
    else {
        parm.EnableAutoDepthStencil = FALSE;
    }
    parm.AutoDepthStencilFormat = depth;

    /* 设置抗锯齿参数 */
    if (fsaa != D3DMULTISAMPLE_NONE) {
        retc = main->obj->CheckDeviceMultiSampleType(
                    D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, format, !full, fsaa);
        if (SUCCEEDED(retc)) {
            if (depth != D3DFMT_UNKNOWN) {
                retc = main->obj->CheckDeviceMultiSampleType(
                    D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, depth, !full, fsaa);
                if (FAILED(retc))
                    fsaa = D3DMULTISAMPLE_NONE;
            }
        }
        else {
            fsaa = D3DMULTISAMPLE_NONE;
        }
    }
    parm.MultiSampleType = fsaa;

    /* 设置其他参数 */
    parm.SwapEffect = D3DSWAPEFFECT_DISCARD;
    parm.hDeviceWindow = main->parm.hDeviceWindow;
    parm.Windowed = !full;
    parm.Flags = 0;
    parm.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    if (!full) {
        parm.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    }
    else {
        parm.FullScreen_PresentationInterval = vsync ?
                D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    /* 重置 D3D8 设备对象 */
    retc = main->dev->Reset(&parm);
    if (FAILED(retc))
        return (FALSE);

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
    生成网格对象 (VB)
=======================================
*/
CR_API sD3D8_MESH*
d3d8_create_mesh_vb (
  __CR_IN__ sD3D8_MAIN*         main,
  __CR_IN__ uint_t              vnum,
  __CR_IN__ uint_t              bpv,
  __CR_IN__ D3DPOOL             pool,
  __CR_IN__ int32u              usage,
  __CR_IN__ uint_t              fvf,
  __CR_IN__ const void_t*       data
    )
{
    BYTE*       copy;
    HRESULT     retc;
    sD3D8_MESH* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_MESH);
    if (rett == NULL)
        return (NULL);
    struct_zero(rett, sD3D8_MESH);

    /* 生成 D3D8 VB 对象 */
    rett->vnum  = vnum;
    rett->nbpv  = bpv;
    rett->vsize = vnum * bpv;
    rett->fvf   = fvf;
    retc = main->dev->CreateVertexBuffer(rett->vsize, usage, fvf, pool,
                                         &rett->vbuf);
    if (FAILED(retc)) {
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
CR_API sD3D8_MESH*
d3d8_create_mesh_ib (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ uint_t          inum,
  __CR_IN__ uint_t          bpi,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ const void_t*   data
    )
{
    BYTE*       copy;
    HRESULT     retc;
    D3DFORMAT   ifmt;
    sD3D8_MESH* rett;

    /* 过滤参数 */
    if (bpi != 2 && bpi != 4)
        return (NULL);

    /* 分配对象结构 */
    rett = struct_new(sD3D8_MESH);
    if (rett == NULL)
        return (NULL);
    struct_zero(rett, sD3D8_MESH);

    /* 生成 D3D8 IB 对象 */
    rett->inum  = inum;
    rett->nbpi  = bpi;
    rett->isize = inum * bpi;
    rett->ntri  = inum / 3;
    ifmt = (bpi == 2) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
    retc = main->dev->CreateIndexBuffer(rett->isize, usage, ifmt, pool,
                                        &rett->ibuf);
    if (FAILED(retc)) {
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
CR_API sD3D8_MESH*
d3d8_create_mesh_vib (
  __CR_IN__ sD3D8_MAIN*         main,
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
    BYTE*       copy;
    HRESULT     retc;
    D3DFORMAT   ifmt;
    sD3D8_MESH* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_MESH);
    if (rett == NULL)
        return (NULL);

    /* 生成 D3D8 VB 对象 */
    rett->vnum  = vnum;
    rett->nbpv  = bpv;
    rett->vsize = vnum * bpv;
    rett->fvf   = fvf;
    retc = main->dev->CreateVertexBuffer(rett->vsize, vusage, fvf, vpool,
                                         &rett->vbuf);
    if (FAILED(retc))
        goto _failure1;

    /* 生成 D3D8 IB 对象 */
    rett->inum = inum;
    if (vnum <= 65536) {
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
                                        &rett->ibuf);
    if (FAILED(retc))
        goto _failure2;

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
d3d8_release_mesh (
  __CR_IN__ const sD3D8_MESH*   mesh
    )
{
    TRY_XREL(mesh->ibuf);
    TRY_XREL(mesh->vbuf);
    mem_free(mesh);
}

/*
=======================================
    设置 VB 数据
=======================================
*/
CR_API bool_t
d3d8_mesh_vb_set (
  __CR_IN__ sD3D8_MESH*     mesh,
  __CR_IN__ uint_t          start,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          count,
  __CR_IN__ int32u          flags
    )
{
    BYTE*   copy;
    HRESULT retc;

    if (mesh->vbuf == NULL)
        return (FALSE);
    start *= mesh->nbpv;
    count *= mesh->nbpv;
    retc = mesh->vbuf->Lock(start, count, &copy, flags);
    if (FAILED(retc))
        return (FALSE);
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
d3d8_mesh_ib_set (
  __CR_IN__ sD3D8_MESH*     mesh,
  __CR_IN__ uint_t          start,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          count,
  __CR_IN__ int32u          flags
    )
{
    BYTE*   copy;
    HRESULT retc;

    if (mesh->ibuf == NULL)
        return (FALSE);
    start *= mesh->nbpi;
    count *= mesh->nbpi;
    retc = mesh->ibuf->Lock(start, count, &copy, flags);
    if (FAILED(retc))
        return (FALSE);
    mem_cpy(copy, data, count);
    mesh->ibuf->Unlock();
    return (TRUE);
}

/*
---------------------------------------
    计算 X 网格的包围体
---------------------------------------
*/
static void_t
d3d8_xmsh_get_bound (
  __CR_IO__ sD3D8_XMSH* xmsh
    )
{
    BYTE*   dat;
    DWORD   fvf;
    DWORD   num;

    xmsh->xcall = NULL;
    fvf = xmsh->xmesh->GetFVF();
    num = xmsh->xmesh->GetNumVertices();
    xmsh->xmesh->LockVertexBuffer(D3DLOCK_READONLY, &dat);
    D3DXComputeBoundingBox(dat, num, fvf, &xmsh->min, &xmsh->max);
    D3DXComputeBoundingSphere(dat, num, fvf, &xmsh->center, &xmsh->radius);
    xmsh->xmesh->UnlockVertexBuffer();
}

/*
=======================================
    生成 X 网格对象 (文件A)
=======================================
*/
CR_API sD3D8_XMSH*
d3d8_create_xmsh_fileA (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ int32u          flags,
  __CR_IN__ const ansi_t*   name
    )
{
    HRESULT     retc;
    sD3D8_XMSH* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_XMSH);
    if (rett == NULL)
        return (NULL);

    /* 加载 X 网格文件 */
    retc = D3DXLoadMeshFromX((LPSTR)name, flags, main->dev, &rett->adjcy,
                                &rett->xattr, &rett->nattr, &rett->xmesh);
    if (FAILED(retc)) {
        mem_free(rett);
        return (NULL);
    }
    d3d8_xmsh_get_bound(rett);
    return (rett);
}

/*
=======================================
    生成 X 网格对象 (文件W)
=======================================
*/
CR_API sD3D8_XMSH*
d3d8_create_xmsh_fileW (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ int32u          flags,
  __CR_IN__ const wide_t*   name
    )
{
    ansi_t*     cnvt;
    sD3D8_XMSH* rett;

    cnvt = utf16_to_local(CR_LOCAL, name);
    if (cnvt == NULL)
        return (NULL);
    rett = d3d8_create_xmsh_fileA(main, flags, cnvt);
    mem_free(cnvt);
    if (rett == NULL)
        return (NULL);
    d3d8_xmsh_get_bound(rett);
    return (rett);
}

/*
=======================================
    生成 X 网格对象 (内存)
=======================================
*/
CR_API sD3D8_XMSH*
d3d8_create_xmsh_mem (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ int32u          flags,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    HRESULT     retc;
    sD3D8_XMSH* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_XMSH);
    if (rett == NULL)
        return (NULL);

    /* 加载 X 网格文件 */
    retc = D3DXLoadMeshFromXInMemory((PBYTE)data, (uint_t)size, flags,
                                    main->dev, &rett->adjcy, &rett->xattr,
                                        &rett->nattr, &rett->xmesh);
    if (FAILED(retc)) {
        mem_free(rett);
        return (NULL);
    }
    d3d8_xmsh_get_bound(rett);
    return (rett);
}

/*
=======================================
    释放 X 网格对象
=======================================
*/
CR_API void_t
d3d8_release_xmsh (
  __CR_IN__ sD3D8_XMSH* xmsh
    )
{
    xmsh->xattr->Release();
    xmsh->adjcy->Release();
    xmsh->xmesh->Release();
    mem_free(xmsh);
}

/*
=======================================
    生成 2D 纹理对象
=======================================
*/
CR_API sD3D8_TEXR*
d3d8_create_tex2 (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ uint_t      width,
  __CR_IN__ uint_t      height,
  __CR_IN__ D3DFORMAT   format,
  __CR_IN__ D3DPOOL     pool,
  __CR_IN__ int32u      usage,
  __CR_IN__ uint_t      level
    )
{
    HRESULT     retc;
    sD3D8_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_TEXR);
    if (rett == NULL)
        return (NULL);

    /* 生成 D3D8 2D 纹理对象 */
    rett->face = 1;
    rett->info.Width = width;
    rett->info.Height = height;
    rett->info.Depth = 0;
    rett->info.MipLevels = level;
    rett->info.Format = format;
    retc = main->dev->CreateTexture(width, height, level, usage, format,
                                    pool, &rett->obj.tex2);
    if (FAILED(retc)) {
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
CR_API sD3D8_TEXR*
d3d8_create_tex2_fileA (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D8_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_TEXR);
    if (rett == NULL)
        return (NULL);

    /* 生成 D3D8 2D 纹理对象 */
    rett->face = 1;
    retc = D3DXCreateTextureFromFileExA(main->dev, name, 0, 0,
                level, usage, D3DFMT_UNKNOWN, pool, D3DX_DEFAULT, mip_type,
                        keycolor, &rett->info, NULL, &rett->obj.tex2);
    if (FAILED(retc)) {
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
CR_API sD3D8_TEXR*
d3d8_create_tex2_fileW (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D8_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_TEXR);
    if (rett == NULL)
        return (NULL);

    /* 生成 D3D8 2D 纹理对象 */
    rett->face = 1;
    retc = D3DXCreateTextureFromFileExW(main->dev, name, 0, 0,
                level, usage, D3DFMT_UNKNOWN, pool, D3DX_DEFAULT, mip_type,
                        keycolor, &rett->info, NULL, &rett->obj.tex2);
    if (FAILED(retc)) {
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
CR_API sD3D8_TEXR*
d3d8_create_tex2_mem (
  __CR_IN__ sD3D8_MAIN*     main,
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
    sD3D8_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_TEXR);
    if (rett == NULL)
        return (NULL);

    /* 生成 D3D8 2D 纹理对象 */
    rett->face = 1;
    retc = D3DXCreateTextureFromFileInMemoryEx(main->dev, data,
                    (uint_t)size, 0, 0, level, usage, D3DFMT_UNKNOWN,
                        pool, D3DX_DEFAULT, mip_type, keycolor,
                            &rett->info, NULL, &rett->obj.tex2);
    if (FAILED(retc)) {
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
CR_API sD3D8_TEXR*
d3d8_create_tex2_crh (
  __CR_IN__ sD3D8_MAIN*     main,
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
    sD3D8_TEXR*     rett;
    D3DLOCKED_RECT  info;

    tfmt = (D3DFORMAT)image_crh_to_d3d(image->fmt);
    if (tfmt == D3DFMT_UNKNOWN)
        return (NULL);
    rett = d3d8_create_tex2(main, image->position.ww,
                    image->position.hh, tfmt, pool, usage, level);
    if (rett == NULL)
        return (NULL);

    /* 锁定纹理对象复制数据 */
    retc = rett->obj.tex2->LockRect(0, &info, NULL, 0);
    if (FAILED(retc)) {
        rett->obj.tex2->Release();
        mem_free(rett);
        return (NULL);
    }

    /* DXTC 格式直接复制, 否则逐行复制 */
    if (isCrTypeCompr(image->fmt)) {
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
    D3DXFilterTexture(rett->obj.base, NULL, 0, mip_type);
    return (rett);
}

/*
=======================================
    更新 2D 纹理对象
=======================================
*/
CR_API bool_t
d3d8_tex2_image_set (
  __CR_IO__ sD3D8_TEXR*     texr,
  __CR_IN__ const sIMAGE*   image,
  __CR_IN__ bool_t          mip_map,
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
    D3DLOCKED_RECT  info;

    tfmt = (D3DFORMAT)image_crh_to_d3d(image->fmt);
    if (tfmt == D3DFMT_UNKNOWN)
        return (FALSE);

    /* 锁定纹理对象复制数据 */
    retc = texr->obj.tex2->LockRect(0, &info, NULL, 0);
    if (FAILED(retc))
        return (FALSE);

    /* DXTC 格式直接复制, 否则逐行复制 */
    if (isCrTypeCompr(image->fmt)) {
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
    texr->obj.tex2->UnlockRect(0);
    if (mip_map)
        D3DXFilterTexture(texr->obj.base, NULL, 0, mip_type);
    return (TRUE);
}

/*
=======================================
    生成 3D 纹理对象
=======================================
*/
CR_API sD3D8_TEXR*
d3d8_create_tex3 (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ uint_t      size,
  __CR_IN__ D3DFORMAT   format,
  __CR_IN__ D3DPOOL     pool,
  __CR_IN__ int32u      usage,
  __CR_IN__ uint_t      level
    )
{
    HRESULT     retc;
    sD3D8_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_TEXR);
    if (rett == NULL)
        return (NULL);

    /* 生成 D3D8 3D 纹理对象 */
    rett->face = 6;
    rett->info.Width = size;
    rett->info.Height = size;
    rett->info.Depth = 0;
    rett->info.MipLevels = level;
    rett->info.Format = format;
    retc = main->dev->CreateCubeTexture(size, level, usage, format,
                                        pool, &rett->obj.tex3);
    if (FAILED(retc)) {
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
CR_API sD3D8_TEXR*
d3d8_create_tex3_fileA (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D8_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_TEXR);
    if (rett == NULL)
        return (NULL);

    /* 生成 D3D8 3D 纹理对象 */
    rett->face = 6;
    retc = D3DXCreateCubeTextureFromFileExA(main->dev, name, 0, level,
                    usage, D3DFMT_UNKNOWN, pool, D3DX_DEFAULT, mip_type,
                        keycolor, &rett->info, NULL, &rett->obj.tex3);
    if (FAILED(retc)) {
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
CR_API sD3D8_TEXR*
d3d8_create_tex3_fileW (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ D3DPOOL         pool,
  __CR_IN__ int32u          usage,
  __CR_IN__ uint_t          level,
  __CR_IN__ int32u          mip_type,
  __CR_IN__ cl32_t          keycolor
    )
{
    HRESULT     retc;
    sD3D8_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_TEXR);
    if (rett == NULL)
        return (NULL);

    /* 生成 D3D8 3D 纹理对象 */
    rett->face = 6;
    retc = D3DXCreateCubeTextureFromFileExW(main->dev, name, 0, level,
                    usage, D3DFMT_UNKNOWN, pool, D3DX_DEFAULT, mip_type,
                        keycolor, &rett->info, NULL, &rett->obj.tex3);
    if (FAILED(retc)) {
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
CR_API sD3D8_TEXR*
d3d8_create_tex3_mem (
  __CR_IN__ sD3D8_MAIN*     main,
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
    sD3D8_TEXR* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_TEXR);
    if (rett == NULL)
        return (NULL);

    /* 生成 D3D8 3D 纹理对象 */
    rett->face = 6;
    retc = D3DXCreateCubeTextureFromFileInMemoryEx(main->dev, data,
                    (uint_t)size, 0, level, usage, D3DFMT_UNKNOWN, pool,
                            D3DX_DEFAULT, mip_type, keycolor,
                            &rett->info, NULL, &rett->obj.tex3);
    if (FAILED(retc)) {
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
d3d8_release_texr (
  __CR_IN__ const sD3D8_TEXR*   texr
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
CR_API sD3D8_VSH*
d3d8_create_vs_data (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const void_t*   decl,
  __CR_IN__ const void_t*   data
    )
{
    HRESULT     retc;
    sD3D8_VSH*  rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_VSH);
    if (rett == NULL)
        return (NULL);

    /* 生成 D3D8 VS 对象 */
    retc = main->dev->CreateVertexShader((DWORD*)decl, (DWORD*)data,
                                         &rett->obj, 0);
    if (FAILED(retc)) {
        mem_free(rett);
        return (NULL);
    }
    return (rett);
}

/*
=======================================
    生成 VS 对象 (文件A)
=======================================
*/
CR_API sD3D8_VSH*
d3d8_create_vs_fileA (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const void_t*   decl,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ int32u          flags,
  __CR_IN__ const ansi_t*   entry,
  __CR_IN__ const ansi_t*   profile
    )
{
    HRESULT         retc;
    sD3D8_VSH*      rett;
    LPD3DXBUFFER    code;

    CR_NOUSE(entry);
    CR_NOUSE(profile);

    /* 只支持汇编 */
    retc = D3DXAssembleShaderFromFileA(name, flags, NULL, &code, NULL);
    if (FAILED(retc))
        return (NULL);

    /* 生成 D3D8 VS 对象 */
    rett = d3d8_create_vs_data(main, decl, code->GetBufferPointer());
    code->Release();
    return (rett);
}

/*
=======================================
    生成 VS 对象 (文件W)
=======================================
*/
CR_API sD3D8_VSH*
d3d8_create_vs_fileW (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const void_t*   decl,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ int32u          flags,
  __CR_IN__ const ansi_t*   entry,
  __CR_IN__ const ansi_t*   profile
    )
{
    HRESULT         retc;
    sD3D8_VSH*      rett;
    LPD3DXBUFFER    code;

    CR_NOUSE(entry);
    CR_NOUSE(profile);

    /* 只支持汇编 */
    retc = D3DXAssembleShaderFromFileW(name, flags, NULL, &code, NULL);
    if (FAILED(retc))
        return (NULL);

    /* 生成 D3D8 VS 对象 */
    rett = d3d8_create_vs_data(main, decl, code->GetBufferPointer());
    code->Release();
    return (rett);
}

/*
=======================================
    生成 VS 对象 (文本)
=======================================
*/
CR_API sD3D8_VSH*
d3d8_create_vs_text (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const void_t*   decl,
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ int32u          flags,
  __CR_IN__ const ansi_t*   entry,
  __CR_IN__ const ansi_t*   profile
    )
{
    HRESULT         retc;
    sD3D8_VSH*      rett;
    LPD3DXBUFFER    code;

    CR_NOUSE(entry);
    CR_NOUSE(profile);

    /* 只支持汇编 */
    retc = D3DXAssembleShader(text, str_sizeA(text), flags,
                              NULL, &code, NULL);
    if (FAILED(retc))
        return (NULL);

    /* 生成 D3D8 VS 对象 */
    rett = d3d8_create_vs_data(main, decl, code->GetBufferPointer());
    code->Release();
    return (rett);
}

/*
=======================================
    释放 VS 对象
=======================================
*/
CR_API void_t
d3d8_release_vs (
  __CR_IN__ sD3D8_MAIN*         main,
  __CR_IN__ const sD3D8_VSH*    vsh
    )
{
    main->dev->DeleteVertexShader(vsh->obj);
    mem_free(vsh);
}

/*
=======================================
    生成 PS 对象 (数据)
=======================================
*/
CR_API sD3D8_PSH*
d3d8_create_ps_data (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const void_t*   data
    )
{
    HRESULT     retc;
    sD3D8_PSH*  rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_PSH);
    if (rett == NULL)
        return (NULL);

    /* 生成 D3D8 PS 对象 */
    retc = main->dev->CreatePixelShader((DWORD*)data, &rett->obj);
    if (FAILED(retc)) {
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
CR_API sD3D8_PSH*
d3d8_create_ps_fileA (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ int32u          flags,
  __CR_IN__ const ansi_t*   entry,
  __CR_IN__ const ansi_t*   profile
    )
{
    HRESULT         retc;
    sD3D8_PSH*      rett;
    LPD3DXBUFFER    code;

    CR_NOUSE(entry);
    CR_NOUSE(profile);

    /* 只支持汇编 */
    retc = D3DXAssembleShaderFromFileA(name, flags, NULL, &code, NULL);
    if (FAILED(retc))
        return (NULL);

    /* 生成 D3D8 PS 对象 */
    rett = d3d8_create_ps_data(main, code->GetBufferPointer());
    code->Release();
    return (rett);
}

/*
=======================================
    生成 PS 对象 (文件W)
=======================================
*/
CR_API sD3D8_PSH*
d3d8_create_ps_fileW (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ int32u          flags,
  __CR_IN__ const ansi_t*   entry,
  __CR_IN__ const ansi_t*   profile
    )
{
    HRESULT         retc;
    sD3D8_PSH*      rett;
    LPD3DXBUFFER    code;

    CR_NOUSE(entry);
    CR_NOUSE(profile);

    /* 只支持汇编 */
    retc = D3DXAssembleShaderFromFileW(name, flags, NULL, &code, NULL);
    if (FAILED(retc))
        return (NULL);

    /* 生成 D3D8 PS 对象 */
    rett = d3d8_create_ps_data(main, code->GetBufferPointer());
    code->Release();
    return (rett);
}

/*
=======================================
    生成 PS 对象 (文本)
=======================================
*/
CR_API sD3D8_PSH*
d3d8_create_ps_text (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ int32u          flags,
  __CR_IN__ const ansi_t*   entry,
  __CR_IN__ const ansi_t*   profile
    )
{
    HRESULT         retc;
    sD3D8_PSH*      rett;
    LPD3DXBUFFER    code;

    CR_NOUSE(entry);
    CR_NOUSE(profile);

    /* 只支持汇编 */
    retc = D3DXAssembleShader(text, str_sizeA(text), flags,
                              NULL, &code, NULL);
    if (FAILED(retc))
        return (NULL);

    /* 生成 D3D8 PS 对象 */
    rett = d3d8_create_ps_data(main, code->GetBufferPointer());
    code->Release();
    return (rett);
}

/*
=======================================
    释放 PS 对象
=======================================
*/
CR_API void_t
d3d8_release_ps (
  __CR_IN__ sD3D8_MAIN*         main,
  __CR_IN__ const sD3D8_PSH*    psh
    )
{
    main->dev->DeletePixelShader(psh->obj);
    mem_free(psh);
}

/*
=======================================
    生成变换对象
=======================================
*/
CR_API sD3D8_TRAN*
d3d8_create_tran (
  __CR_IN__ fp32_t  fovy,
  __CR_IN__ sint_t  width,
  __CR_IN__ sint_t  height
    )
{
    sD3D8_TRAN* rett;

    /* 分配对象结构 */
    rett = struct_new(sD3D8_TRAN);
    if (rett == NULL)
        return (NULL);

    /* 初始化成员 */
    rett->fovy = fovy * CR_DTOR;
    rett->aspect = (fp32_t)width / height;
    rett->znear = 1.0f;
    rett->zfar = 1000.0f;
    D3DXMatrixPerspectiveFovLH(&rett->proj, rett->fovy, rett->aspect,
                                rett->znear, rett->zfar);
    D3DXMatrixTranspose(&rett->tproj, &rett->proj);
    rett->eye.x =  0.0f;
    rett->eye.y =  0.0f;
    rett->eye.z = -10.0f;
    rett->eye.w =  1.0f;
    rett->lookat.x = 0.0f;
    rett->lookat.y = 0.0f;
    rett->lookat.z = 0.0f;
    rett->lookat.w = 1.0f;
    rett->upvec.x = 0.0f;
    rett->upvec.y = 1.0f;
    rett->upvec.z = 0.0f;
    rett->upvec.w = 1.0f;
    rett->vvec.x = rett->eye.x - rett->lookat.x;
    rett->vvec.y = rett->eye.y - rett->lookat.y;
    rett->vvec.z = rett->eye.z - rett->lookat.z;
    rett->vvec.w = 1.0f;
    D3DXVec3Normalize((D3DXVECTOR3*)(&rett->vvec),
                      (D3DXVECTOR3*)(&rett->vvec));
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
    D3DXMatrixIdentity(&rett->one);
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
d3d8_release_tran (
  __CR_IN__ const sD3D8_TRAN*   tran
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
d3d8_tran_update_wrld (
  __CR_IO__ sD3D8_TRAN* tran
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
d3d8_tran_update_view (
  __CR_IO__ sD3D8_TRAN* tran
    )
{
    tran->vvec.x = tran->eye.x - tran->lookat.x;
    tran->vvec.y = tran->eye.y - tran->lookat.y;
    tran->vvec.z = tran->eye.z - tran->lookat.z;
    D3DXVec3Normalize((D3DXVECTOR3*)(&tran->vvec),
                      (D3DXVECTOR3*)(&tran->vvec));
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
d3d8_tran_update_proj (
  __CR_IO__ sD3D8_TRAN* tran
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
d3d8_tran_update_port (
  __CR_IO__ sD3D8_TRAN* tran
    )
{
    sint_t  width, height;

    width  = tran->view_port.Width;
    height = tran->view_port.Height;
    tran->aspect = (fp32_t)width / height;
    d3d8_tran_update_proj(tran);
}

/*
=======================================
    更新 WVP 变换
=======================================
*/
CR_API void_t
d3d8_tran_update_mwvp (
  __CR_IO__ sD3D8_TRAN* tran
    )
{
    D3DXMatrixMultiply(&tran->wvp, &tran->world, &tran->view);
    D3DXMatrixMultiplyTranspose(&tran->wvp, &tran->wvp, &tran->proj);
}

/*
=======================================
    获取拾取射线
=======================================
*/
CR_API void_t
d3d8_tran_pickup (
  __CR_IN__ sD3D8_TRAN* tran,
  __CR_OT__ sRADIAL*    dest,
  __CR_IN__ sint_t      scn_x,
  __CR_IN__ sint_t      scn_y
    )
{
    D3DXVECTOR3     vec;
    D3DXMATRIXA16   mat;

    scn_x -= tran->view_port.X;
    scn_y -= tran->view_port.Y;
    vec.x  =  (2.0f * scn_x) / tran->view_port.Width;
    vec.x  =  (vec.x - 1.0f) / tran->proj._11;
    vec.y  =  (2.0f * scn_y) / tran->view_port.Height;
    vec.y  = -(vec.y - 1.0f) / tran->proj._22;
    vec.z  =   1.0f;
    D3DXMatrixInverse(&mat, NULL, &tran->view);

    dest->pos.x = mat._41;
    dest->pos.y = mat._42;
    dest->pos.z = mat._43;
    dest->dir.x = vec.x * mat._11 + vec.y * mat._21 + vec.z * mat._31;
    dest->dir.y = vec.x * mat._12 + vec.y * mat._22 + vec.z * mat._32;
    dest->dir.z = vec.x * mat._13 + vec.y * mat._23 + vec.z * mat._33;
}

/*
=======================================
    空间转屏幕坐标
=======================================
*/
CR_API void_t
d3d8_tran_convert (
  __CR_IN__ sD3D8_TRAN*     tran,
  __CR_OT__ vec2d_t*        dest,
  __CR_IN__ const vec3d_t*  space
    )
{
    D3DXVECTOR3     scn;

    D3DXVec3Project(&scn, (D3DXVECTOR3*)space, &tran->view_port,
                    &tran->proj, &tran->view, &tran->one);
    dest->x = scn.x;
    dest->y = scn.y;
}

/*
=======================================
    获取视锥对象
=======================================
*/
CR_API void_t
d3d8_tran_frustum (
  __CR_IN__ sD3D8_TRAN* tran,
  __CR_OT__ sFRUSTUM*   dest,
  __CR_IN__ fp32_t      bias
    )
{
    D3DXPLANE       pln;
    D3DXMATRIXA16   mat;

    dest->bias = bias;
    D3DXMatrixMultiply(&mat, &tran->view, &tran->proj);

    /* near */
    pln.a = mat._14 + mat._13;
    pln.b = mat._24 + mat._23;
    pln.c = mat._34 + mat._33;
    pln.d = mat._44 + mat._43;
    D3DXPlaneNormalize((D3DXPLANE*)(&dest->p[0]), &pln);

    /* far */
    pln.a = mat._14 - mat._13;
    pln.b = mat._24 - mat._23;
    pln.c = mat._34 - mat._33;
    pln.d = mat._44 - mat._43;
    D3DXPlaneNormalize((D3DXPLANE*)(&dest->p[1]), &pln);

    /* left */
    pln.a = mat._14 + mat._11;
    pln.b = mat._24 + mat._21;
    pln.c = mat._34 + mat._31;
    pln.d = mat._44 + mat._41;
    D3DXPlaneNormalize((D3DXPLANE*)(&dest->p[2]), &pln);

    /* right */
    pln.a = mat._14 - mat._11;
    pln.b = mat._24 - mat._21;
    pln.c = mat._34 - mat._31;
    pln.d = mat._44 - mat._41;
    D3DXPlaneNormalize((D3DXPLANE*)(&dest->p[3]), &pln);

    /* top */
    pln.a = mat._14 - mat._12;
    pln.b = mat._24 - mat._22;
    pln.c = mat._34 - mat._32;
    pln.d = mat._44 - mat._42;
    D3DXPlaneNormalize((D3DXPLANE*)(&dest->p[4]), &pln);

    /* bottom */
    pln.a = mat._14 + mat._12;
    pln.b = mat._24 + mat._22;
    pln.c = mat._34 + mat._32;
    pln.d = mat._44 + mat._42;
    D3DXPlaneNormalize((D3DXPLANE*)(&dest->p[5]), &pln);
}

/*
=======================================
    设置 XOZ 公告牌变换
=======================================
*/
CR_API void_t
d3d8_tran_billboardv (
  __CR_IO__ sD3D8_TRAN* tran
    )
{
    fp32_t  dx, dz, ang;
    /* -------------- */
    D3DXMATRIXA16   rmat;

    /* 平面的初始法线为 (0, 0, -1) */
    dx = tran->eye.x - tran->lookat.x;
    dz = tran->eye.z - tran->lookat.z;

    /* 计算 Y 轴旋转角度 */
    if (dz >= 0.0f) {
        if (dx >  CR_ABIT32)
            ang = -CR_PI / 2.0f - FATAN(dz / dx);
        else
        if (dx < -CR_ABIT32)
            ang =  CR_PI / 2.0f - FATAN(dz / dx);
        else
            ang = -CR_PI;
    }
    else {
        ang = FATAN(dx / dz);
    }
    D3DXMatrixRotationY(&rmat, ang);
    D3DXMatrixMultiply(&tran->world, &tran->world, &rmat);
}

/*
=======================================
    设置自由的公告牌变换
=======================================
*/
CR_API void_t
d3d8_tran_billboardh (
  __CR_IO__ sD3D8_TRAN* tran
    )
{
    fp32_t          ang;
    D3DXVECTOR3     eye;
    D3DXVECTOR3     face;
    D3DXVECTOR3     axis;
    D3DXVECTOR4     tvec;
    D3DXMATRIXA16   tmat;
    D3DXMATRIXA16   rmat;

    /* 平面的初始法线为 (0, 0, -1) */
    eye.x = tran->eye.x - tran->lookat.x;
    eye.y = tran->eye.y - tran->lookat.y;
    eye.z = tran->eye.z - tran->lookat.z;
    face.x =  0.0f;
    face.y =  0.0f;
    face.z = -1.0f;

    /* 计算 Y 轴旋转角度 */
    if (eye.z >= 0.0f) {
        if (eye.x >  CR_ABIT32)
            ang = -CR_PI / 2.0f - FATAN(eye.z / eye.x);
        else
        if (eye.x < -CR_ABIT32)
            ang =  CR_PI / 2.0f - FATAN(eye.z / eye.x);
        else
            ang = -CR_PI;
    }
    else {
        ang = FATAN(eye.x / eye.z);
    }
    D3DXMatrixRotationY(&rmat, ang);

    /* 使平面的法线与视线平行 */
    D3DXVec3Transform(&tvec, &face, &rmat);
    face.x = tvec.x;
    face.y = tvec.y;
    face.z = tvec.z;
    D3DXVec3Normalize(&eye, &eye);
    D3DXVec3Cross(&axis, &face, &eye);
    D3DXMatrixRotationAxis(&tmat, &axis, FACOS(D3DXVec3Dot(&eye, &face)));
    D3DXMatrixMultiply(&rmat, &rmat, &tmat);
    D3DXMatrixMultiply(&tran->world, &tran->world, &rmat);
}

/*
=======================================
    世界变换矩阵清零
=======================================
*/
CR_API void_t
d3d8_tran_wrld_clear (
  __CR_IO__ sD3D8_TRAN* tran
    )
{
    D3DXMatrixIdentity(&tran->world);
}

/*
=======================================
    世界变换矩阵右乘
=======================================
*/
CR_API void_t
d3d8_tran_wrld_rtmul (
  __CR_IO__ sD3D8_TRAN*     tran,
  __CR_IN__ const mat4x4_t* mats,
  __CR_IN__ uint_t          count
    )
{
    for (; count != 0; count--, mats++)
        D3DXMatrixMultiply(&tran->world, &tran->world, (D3DXMATRIX*)mats);
}

/*
=======================================
    合成变换矩阵1
=======================================
*/
CR_API void_t
d3d8_util_make_tran1 (
  __CR_OT__ mat4x4_t*       mat,
  __CR_IN__ const vec3d_t*  scale,
  __CR_IN__ fp32_t          pitch,
  __CR_IN__ fp32_t          yaw,
  __CR_IN__ fp32_t          roll,
  __CR_IN__ const vec3d_t*  move
    )
{
    D3DXQUATERNION  rote;

    pitch *= CR_DTOR;
    yaw   *= CR_DTOR;
    roll  *= CR_DTOR;
    D3DXQuaternionRotationYawPitchRoll(&rote, yaw, pitch, roll);
    D3DXMatrixTransformation((D3DXMATRIX*)mat, NULL, NULL,
                             (D3DXVECTOR3*)scale, NULL, &rote,
                             (D3DXVECTOR3*)move);
}

/*
=======================================
    合成变换矩阵2
=======================================
*/
CR_API void_t
d3d8_util_make_tran2 (
  __CR_OT__ mat4x4_t*       mat,
  __CR_IN__ const vec3d_t*  scale,
  __CR_IN__ const vec3d_t*  axis,
  __CR_IN__ fp32_t          angle,
  __CR_IN__ const vec3d_t*  move
    )
{
    D3DXQUATERNION  rote;

    angle *= CR_DTOR;
    if (axis != NULL) {
        D3DXQuaternionRotationAxis(&rote, (D3DXVECTOR3*)axis, angle);
        D3DXMatrixTransformation((D3DXMATRIX*)mat, NULL, NULL,
                                 (D3DXVECTOR3*)scale, NULL, &rote,
                                 (D3DXVECTOR3*)move);
    }
    else {
        D3DXMatrixTransformation((D3DXMATRIX*)mat, NULL, NULL,
                                 (D3DXVECTOR3*)scale, NULL, NULL,
                                 (D3DXVECTOR3*)move);
    }
}

/*
=======================================
    合成变换矩阵3
=======================================
*/
CR_API void_t
d3d8_util_make_tran3 (
  __CR_OT__ mat4x4_t*       mat,
  __CR_IN__ const vec3d_t*  scale,
  __CR_IN__ const vec4d_t*  quat,
  __CR_IN__ const vec3d_t*  move
    )
{
    D3DXMatrixTransformation((D3DXMATRIX*)mat, NULL,
                             NULL, (D3DXVECTOR3*)scale,
                             NULL, (D3DXQUATERNION*)quat,
                             (D3DXVECTOR3*)move);
}

/*
=======================================
    变换 3D 向量
=======================================
*/
CR_API void_t
d3d8_util_tran_vec3d (
  __CR_OT__ vec3d_t*        output,
  __CR_IN__ const vec3d_t*  input,
  __CR_IN__ const mat4x4_t* mat
    )
{
    D3DXVECTOR4     tmp;

    D3DXVec3Transform(&tmp, (D3DXVECTOR3*)input, (D3DXMATRIX*)mat);
    output->x = tmp.x;  output->y = tmp.y;  output->z = tmp.z;
}

/*
=======================================
    矩阵求逆
=======================================
*/
CR_API bool_t
d3d8_util_matx_inverse (
  __CR_OT__ mat4x4_t*       dst,
  __CR_IN__ const mat4x4_t* src
    )
{
    if (D3DXMatrixInverse((D3DXMATRIX*)dst, NULL, (D3DXMATRIX*)src) == NULL)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    矩阵转置
=======================================
*/
CR_API void_t
d3d8_util_matx_transpose (
  __CR_OT__ mat4x4_t*       dst,
  __CR_IN__ const mat4x4_t* src
    )
{
    D3DXMatrixTranspose((D3DXMATRIX*)dst, (D3DXMATRIX*)src);
}

/*
=======================================
    射线求交 (包围盒)
=======================================
*/
CR_API bool_t
d3d8_util_intersect_aabb (
  __CR_IN__ const sAABB*    aabb,
  __CR_IN__ const sRADIAL*  ray
    )
{
    return (D3DXBoxBoundProbe((const D3DXVECTOR3*)(&aabb->v[AABB_MIN_IDX]),
                              (const D3DXVECTOR3*)(&aabb->v[AABB_MAX_IDX]),
                              (const D3DXVECTOR3*)(&ray->pos),
                              (const D3DXVECTOR3*)(&ray->dir)));
}

/*
=======================================
    射线求交 (包围球)
=======================================
*/
CR_API bool_t
d3d8_util_intersect_ball (
  __CR_IN__ const sSPHERE*  ball,
  __CR_IN__ const sRADIAL*  ray
    )
{
    return (D3DXSphereBoundProbe((const D3DXVECTOR3*)(&ball->center),
                   ball->radius, (const D3DXVECTOR3*)(&ray->pos),
                                 (const D3DXVECTOR3*)(&ray->dir)));
}

/*
=======================================
    射线求交 (三角形)
=======================================
*/
CR_API bool_t
d3d8_util_intersect_tri (
  __CR_OT__ sINTERSECT*     ret,
  __CR_IN__ const vec3d_t*  p0,
  __CR_IN__ const vec3d_t*  p1,
  __CR_IN__ const vec3d_t*  p2,
  __CR_IN__ const sRADIAL*  ray
    )
{
    FLOAT   uu, vv, dd;

    if (!D3DXIntersectTri((D3DXVECTOR3*)p0, (D3DXVECTOR3*)p1,
                    (D3DXVECTOR3*)p2, (const D3DXVECTOR3*)(&ray->pos),
                    (const D3DXVECTOR3*)(&ray->dir), &uu, &vv, &dd))
        return (FALSE);
    if (ret != NULL) {
        D3DXVec3BaryCentric((D3DXVECTOR3*)(&ret->pos), (D3DXVECTOR3*)p0,
                            (D3DXVECTOR3*)p1, (D3DXVECTOR3*)p2, uu, vv);
        ret->dist = dd;
    }
    return (TRUE);
}

/*
=======================================
    生成精灵绘制对象
=======================================
*/
CR_API LPD3DXSPRITE
d3d8_util_create_sprite (
  __CR_IN__ sD3D8_MAIN* main
    )
{
    HRESULT         retc;
    LPD3DXSPRITE    rett;

    retc = D3DXCreateSprite(main->dev, &rett);
    if (FAILED(retc))
        return (NULL);
    return (rett);
}

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

static const sD3D8_CALL s_d3d8call =
{
    /* 设备对象 */
    d3d8_create_main,
    d3d8_release_main,
    d3d8_main_reset,

    /* 网格对象 */
    d3d8_create_mesh_vb,
    d3d8_create_mesh_ib,
    d3d8_create_mesh_vib,
    d3d8_release_mesh,
    d3d8_mesh_vb_set,
    d3d8_mesh_ib_set,
    d3d8_create_xmsh_fileA,
    d3d8_create_xmsh_fileW,
    d3d8_create_xmsh_mem,
    d3d8_release_xmsh,

    /* 纹理对象 */
    d3d8_create_tex2,
    d3d8_create_tex2_fileA,
    d3d8_create_tex2_fileW,
    d3d8_create_tex2_mem,
    d3d8_create_tex2_crh,
    d3d8_tex2_image_set,
    d3d8_create_tex3,
    d3d8_create_tex3_fileA,
    d3d8_create_tex3_fileW,
    d3d8_create_tex3_mem,
    d3d8_release_texr,

    /* VS 对象 */
    d3d8_create_vs_data,
    d3d8_create_vs_fileA,
    d3d8_create_vs_fileW,
    d3d8_create_vs_text,
    d3d8_release_vs,

    /* PS 对象 */
    d3d8_create_ps_data,
    d3d8_create_ps_fileA,
    d3d8_create_ps_fileW,
    d3d8_create_ps_text,
    d3d8_release_ps,

    /* 变换对象 */
    d3d8_create_tran,
    d3d8_release_tran,
    d3d8_tran_update_wrld,
    d3d8_tran_update_view,
    d3d8_tran_update_proj,
    d3d8_tran_update_port,
    d3d8_tran_update_mwvp,
    d3d8_tran_pickup,
    d3d8_tran_convert,
    d3d8_tran_frustum,
    d3d8_tran_billboardv,
    d3d8_tran_billboardh,
    d3d8_tran_wrld_clear,
    d3d8_tran_wrld_rtmul,

    /* 助手函数 */
    d3d8_util_make_tran1,
    d3d8_util_make_tran2,
    d3d8_util_make_tran3,
    d3d8_util_tran_vec3d,
    d3d8_util_matx_inverse,
    d3d8_util_matx_transpose,
    d3d8_util_intersect_aabb,
    d3d8_util_intersect_ball,
    d3d8_util_intersect_tri,
    d3d8_util_create_sprite,
};

/*
=======================================
    获取 D3D8 调用接口表
=======================================
*/
CR_API const sD3D8_CALL*
d3d8call_get (void_t)
{
    return (&s_d3d8call);
}

#endif  /* _CR_HAVE_D3D8_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
