/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-06-17  */
/*     #######          ###    ###      [GFX3]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-06-17  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack D3D8 接口内联模板 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_D3D8API_INL__
#define __CR_D3D8API_INL__ 0xFB0455CFUL

/*
=======================================
    绑定网格 (VB)
=======================================
*/
inline void_t
d3d8_mesh_vb_bind (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ uint_t      stream,
  __CR_IN__ sD3D8_MESH* vmesh
    )
{
    main->dev->SetStreamSource(stream, vmesh->vbuf, vmesh->nbpv);
}

/*
=======================================
    绑定网格 (IB)
=======================================
*/
inline void_t
d3d8_mesh_ib_bind (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ sD3D8_MESH* imesh
    )
{
    main->dev->SetIndices(imesh->ibuf, 0);
}

/*
=======================================
    绑定纹理
=======================================
*/
inline void_t
d3d8_texr_bind (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ int32u      sampler,
  __CR_IN__ sD3D8_TEXR* texr CR_DEFAULT(NULL)
    )
{
    if (texr != NULL)
        main->dev->SetTexture(sampler, texr->obj.base);
    else
        main->dev->SetTexture(sampler, NULL);
}

/*
=======================================
    提交网格对象 (DP)
=======================================
*/
inline void_t
d3d8_mesh_dp (
  __CR_IN__ sD3D8_MAIN*         main,
  __CR_IN__ sD3D8_MESH*         vmesh,
  __CR_IN__ D3DPRIMITIVETYPE    type,
  __CR_IN__ uint_t              start,
  __CR_IN__ uint_t              count,
  __CR_IN__ sD3D8_VSH*          vsh CR_DEFAULT(NULL)
    )
{
    if (vsh == NULL)
        main->dev->SetVertexShader(vmesh->fvf);
    else
        main->dev->SetVertexShader(vsh->obj);
    main->dev->DrawPrimitive(type, start, count);
}

/*
=======================================
    提交网格对象 (DIP)
=======================================
*/
inline void_t
d3d8_mesh_dip (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ sD3D8_MESH* vmesh,
  __CR_IN__ sD3D8_MESH* imesh CR_DEFAULT(NULL),
  __CR_IN__ sD3D8_VSH*  vsh CR_DEFAULT(NULL)
    )
{
    if (vsh == NULL)
        main->dev->SetVertexShader(vmesh->fvf);
    else
        main->dev->SetVertexShader(vsh->obj);
    if (imesh != NULL) {
        main->dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,
                                        vmesh->vnum, 0, imesh->ntri);
    } else {
        main->dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,
                                        vmesh->vnum, 0, vmesh->ntri);
    }
}

/*
=======================================
    应用 PS 对象
=======================================
*/
inline void_t
d3d8_ps_apply (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ sD3D8_PSH*  psh CR_DEFAULT(NULL)
    )
{
    if (psh != NULL)
        main->dev->SetPixelShader(psh->obj);
    else
        main->dev->SetPixelShader(NULL);
}

/*
=======================================
    设置 VS 浮点常数缓冲
=======================================
*/
inline void_t
d3d8_vs_set_vec4f (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ uint_t          start,
  __CR_IN__ const fp32_t*   data,
  __CR_IN__ uint_t          count
    )
{
    main->dev->SetVertexShaderConstant(start, data, count);
}

/*
=======================================
    设置 PS 浮点常数缓冲
=======================================
*/
inline void_t
d3d8_ps_set_vec4f (
  __CR_IN__ sD3D8_MAIN*     main,
  __CR_IN__ uint_t          start,
  __CR_IN__ const fp32_t*   data,
  __CR_IN__ uint_t          count
    )
{
    main->dev->SetPixelShaderConstant(start, data, count);
}

/******************/
/* 使用 FF 寄存器 */
/******************/
#define FIXED_REG   (-1)

/*
=======================================
    设置世界变换
=======================================
*/
inline void_t
d3d8_tran_set_world (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ sint_t      regn,
  __CR_IN__ sD3D8_TRAN* tran
    )
{
    if (regn < 0)
        main->dev->SetTransform(D3DTS_WORLD, &tran->world);
    else
        d3d8_vs_set_vec4f(main, regn, (fp32_t*)(&tran->tworld), 4);
}

/*
=======================================
    设置观察变换
=======================================
*/
inline void_t
d3d8_tran_set_view (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ sint_t      regn,
  __CR_IN__ sD3D8_TRAN* tran
    )
{
    if (regn < 0)
        main->dev->SetTransform(D3DTS_VIEW, &tran->view);
    else
        d3d8_vs_set_vec4f(main, regn, (fp32_t*)(&tran->tview), 4);
}

/*
=======================================
    设置投影变换
=======================================
*/
inline void_t
d3d8_tran_set_proj (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ sint_t      regn,
  __CR_IN__ sD3D8_TRAN* tran
    )
{
    if (regn < 0)
        main->dev->SetTransform(D3DTS_PROJECTION, &tran->proj);
    else
        d3d8_vs_set_vec4f(main, regn, (fp32_t*)(&tran->tproj), 4);
}

/*
=======================================
    设置视口变换
=======================================
*/
inline void_t
d3d8_tran_set_port (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ sD3D8_TRAN* tran
    )
{
    main->dev->SetViewport(&tran->view_port);
}

/*
=======================================
    设置 WVP 变换 (PP Only)
=======================================
*/
inline void_t
d3d8_tran_set_mwvp (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ uint_t      regn,
  __CR_IN__ sD3D8_TRAN* tran
    )
{
    d3d8_vs_set_vec4f(main, regn, (fp32_t*)(&tran->wvp), 4);
}

/*
=======================================
    开始渲染
=======================================
*/
inline void_t
d3d8_do_enter (
  __CR_IN__ sD3D8_MAIN* main,
  __CR_IN__ int32u      flags,
  __CR_IN__ cl32_t      color   CR_DEFAULT(0xFF000000UL),
  __CR_IN__ fp32_t      valz    CR_DEFAULT(1.0f),
  __CR_IN__ int32u      stencil CR_DEFAULT(0x00000000UL)
    )
{
    if (color != 0x00000000UL)
        main->dev->Clear(0, NULL, flags, color, valz, stencil);
    main->dev->BeginScene();
}

/*
=======================================
    结束渲染
=======================================
*/
inline void_t
d3d8_do_leave (
  __CR_IN__ sD3D8_MAIN* main
    )
{
    main->dev->EndScene();
}

/*
=======================================
    显示结果
=======================================
*/
inline HRESULT
d3d8_do_flip (
  __CR_IN__ sD3D8_MAIN* main
    )
{
    return (main->dev->Present(NULL, NULL, NULL, NULL));
}

#endif  /* !__CR_D3D8API_INL__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/