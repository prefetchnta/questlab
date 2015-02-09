/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-06-14  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack D3D9 接口内联模板 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    绑定网格 (VB)
=======================================
*/
cr_inline void_t
d3d9_mesh_vb_bind (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ uint_t      stream,
  __CR_IN__ sD3D9_MESH* vmesh
    )
{
    main->dev->SetStreamSource(stream, vmesh->vbuf, 0, vmesh->nbpv);
}

/*
=======================================
    绑定网格 (IB)
=======================================
*/
cr_inline void_t
d3d9_mesh_ib_bind (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ sD3D9_MESH* imesh
    )
{
    main->dev->SetIndices(imesh->ibuf);
}

/*
=======================================
    绑定纹理
=======================================
*/
cr_inline void_t
d3d9_texr_bind (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ int32u      sampler,
  __CR_IN__ sD3D9_TEXR* texr CR_DEFAULT(NULL)
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
cr_inline void_t
d3d9_mesh_dp (
  __CR_IN__ sD3D9_MAIN*         main,
  __CR_IN__ sD3D9_MESH*         vmesh,
  __CR_IN__ D3DPRIMITIVETYPE    type,
  __CR_IN__ uint_t              start,
  __CR_IN__ uint_t              count,
  __CR_IN__ sD3D9_VSH*          vsh CR_DEFAULT(NULL),
  __CR_IN__ bool_t              bare CR_DEFAULT(FALSE)
    )
{
    if (!bare) {
        if (vsh == NULL) {
            main->dev->SetVertexShader(NULL);
            main->dev->SetFVF(vmesh->fvf);
        }
        else {
            main->dev->SetVertexDeclaration(vsh->decl);
            main->dev->SetVertexShader(vsh->obj);
        }
    }
    main->dev->DrawPrimitive(type, start, count);
}

/*
=======================================
    提交网格对象 (DIP)
=======================================
*/
cr_inline void_t
d3d9_mesh_dip (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ sD3D9_MESH* vmesh,
  __CR_IN__ sD3D9_MESH* imesh CR_DEFAULT(NULL),
  __CR_IN__ sD3D9_VSH*  vsh CR_DEFAULT(NULL),
  __CR_IN__ bool_t      bare CR_DEFAULT(FALSE)
    )
{
    if (!bare) {
        if (vsh == NULL) {
            main->dev->SetVertexShader(NULL);
            main->dev->SetFVF(vmesh->fvf);
        }
        else {
            main->dev->SetVertexDeclaration(vsh->decl);
            main->dev->SetVertexShader(vsh->obj);
        }
    }
    if (imesh != NULL) {
        main->dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                        vmesh->vnum, 0, imesh->ntri);
    } else {
        main->dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                        vmesh->vnum, 0, vmesh->ntri);
    }
}

/*
=======================================
    应用 VS 对象
=======================================
*/
cr_inline void_t
d3d9_vs_apply (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ sD3D9_VSH*  vsh CR_DEFAULT(NULL)
    )
{
    if (vsh != NULL) {
        main->dev->SetVertexDeclaration(vsh->decl);
        main->dev->SetVertexShader(vsh->obj);
    }
    else {
        main->dev->SetVertexShader(NULL);
    }
}

/*
=======================================
    应用 PS 对象
=======================================
*/
cr_inline void_t
d3d9_ps_apply (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ sD3D9_PSH*  psh CR_DEFAULT(NULL)
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
cr_inline void_t
d3d9_vs_set_vec4f (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ uint_t          start,
  __CR_IN__ const fp32_t*   data,
  __CR_IN__ uint_t          count
    )
{
    main->dev->SetVertexShaderConstantF(start, data, count);
}

/*
=======================================
    设置 VS 整数常数缓冲
=======================================
*/
cr_inline void_t
d3d9_vs_set_vec4i (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ uint_t          start,
  __CR_IN__ const sint_t*   data,
  __CR_IN__ uint_t          count
    )
{
    main->dev->SetVertexShaderConstantI(start, data, count);
}

/*
=======================================
    设置 VS 布尔常数缓冲
=======================================
*/
cr_inline void_t
d3d9_vs_set_vec4b (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ uint_t          start,
  __CR_IN__ const sint_t*   data,
  __CR_IN__ uint_t          count
    )
{
    main->dev->SetVertexShaderConstantB(start, data, count);
}

/*
=======================================
    设置 PS 浮点常数缓冲
=======================================
*/
cr_inline void_t
d3d9_ps_set_vec4f (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ uint_t          start,
  __CR_IN__ const fp32_t*   data,
  __CR_IN__ uint_t          count
    )
{
    main->dev->SetPixelShaderConstantF(start, data, count);
}

/*
=======================================
    设置 PS 整数常数缓冲
=======================================
*/
cr_inline void_t
d3d9_ps_set_vec4i (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ uint_t          start,
  __CR_IN__ const sint_t*   data,
  __CR_IN__ uint_t          count
    )
{
    main->dev->SetPixelShaderConstantI(start, data, count);
}

/*
=======================================
    设置 PS 布尔常数缓冲
=======================================
*/
cr_inline void_t
d3d9_ps_set_vec4b (
  __CR_IN__ sD3D9_MAIN*     main,
  __CR_IN__ uint_t          start,
  __CR_IN__ const sint_t*   data,
  __CR_IN__ uint_t          count
    )
{
    main->dev->SetPixelShaderConstantB(start, data, count);
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
cr_inline void_t
d3d9_tran_set_world (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ sint_t      regn,
  __CR_IN__ sD3D9_TRAN* tran
    )
{
    if (regn < 0)
        main->dev->SetTransform(D3DTS_WORLD, &tran->world);
    else
        d3d9_vs_set_vec4f(main, regn, (fp32_t*)(&tran->tworld), 4);
}

/*
=======================================
    设置观察变换
=======================================
*/
cr_inline void_t
d3d9_tran_set_view (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ sint_t      regn,
  __CR_IN__ sD3D9_TRAN* tran
    )
{
    if (regn < 0)
        main->dev->SetTransform(D3DTS_VIEW, &tran->view);
    else
        d3d9_vs_set_vec4f(main, regn, (fp32_t*)(&tran->tview), 4);
}

/*
=======================================
    设置投影变换
=======================================
*/
cr_inline void_t
d3d9_tran_set_proj (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ sint_t      regn,
  __CR_IN__ sD3D9_TRAN* tran
    )
{
    if (regn < 0)
        main->dev->SetTransform(D3DTS_PROJECTION, &tran->proj);
    else
        d3d9_vs_set_vec4f(main, regn, (fp32_t*)(&tran->tproj), 4);
}

/*
=======================================
    设置视口变换
=======================================
*/
cr_inline void_t
d3d9_tran_set_port (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ sD3D9_TRAN* tran
    )
{
    main->dev->SetViewport(&tran->view_port);
}

/*
=======================================
    设置 WVP 变换 (PP Only)
=======================================
*/
cr_inline void_t
d3d9_tran_set_mwvp (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ uint_t      regn,
  __CR_IN__ sD3D9_TRAN* tran
    )
{
    d3d9_vs_set_vec4f(main, regn, (fp32_t*)(&tran->wvp), 4);
}

/*
=======================================
    开始渲染
=======================================
*/
cr_inline void_t
d3d9_do_enter (
  __CR_IN__ sD3D9_MAIN* main,
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
cr_inline void_t
d3d9_do_leave (
  __CR_IN__ sD3D9_MAIN* main
    )
{
    main->dev->EndScene();
}

/*
=======================================
    显示结果
=======================================
*/
cr_inline HRESULT
d3d9_do_flip (
  __CR_IN__ sD3D9_MAIN* main
    )
{
    return (main->dev->Present(NULL, NULL, NULL, NULL));
}

/*
=======================================
    打开 Alpha 测试
=======================================
*/
cr_inline void_t
d3d9_at_enter (
  __CR_IN__ sD3D9_MAIN* main,
  __CR_IN__ uint_t      value
    )
{
    main->dev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    main->dev->SetRenderState(D3DRS_ALPHAREF, value);
    main->dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

/*
=======================================
    关闭 Alpha 测试
=======================================
*/
cr_inline void_t
d3d9_at_leave (
  __CR_IN__ sD3D9_MAIN* main
    )
{
    main->dev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

/*
=======================================
    打开 Alpha 混合
=======================================
*/
cr_inline void_t
d3d9_ab_enter (
  __CR_IN__ sD3D9_MAIN* main
    )
{
    main->dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    main->dev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    main->dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    main->dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

/*
=======================================
    关闭 Alpha 混合
=======================================
*/
cr_inline void_t
d3d9_ab_leave (
  __CR_IN__ sD3D9_MAIN* main
    )
{
    main->dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

#endif  /* !_CR_SICK_INLINE_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
