/*
***************************************
*   Asylum3D @ 2014-12-30
***************************************
*/

#include "../../asylum.hpp"

/*************************/
/* CrHack3D9 Main Object */
/*************************/
struct crhack3d9_main
{
    asy::crh3d9_main                main;
    asy::map_acs<asy::crh3d9_texr>  texs;
    asy::map_acs<asy::object_base>  base;
    asy::array<asy::object_inst>    inst;

    sCAMERA     cam;
    sFRUSTUM    frt;
};

/* ==================================== */
CR_API crh3d9_t crhack3d9_init (HWND hwnd)
{
    sDX9_HDLE   hdle;

#if defined(ASY_USE_STATIC)
    hdle.call = d3d9call_get();
#else
    sbin_t      sbin;
    nopfunc_t   func;

    sbin = sbin_testA("GFX3_D3D9.dll");
    if (sbin == NULL) {
        sbin = sbin_loadA("GFX3_D3D9.dll");
        if (sbin == NULL)
            return (NULL);
    }
    func = sbin_exportT(sbin, "d3d9call_get", nopfunc_t);
    if (func == NULL)
        return (NULL);
    hdle.call = (const sD3D9_CALL*)func();
#endif
    hdle.hwnd = hwnd;

    crhack3d9_main* rett;

    rett = struct_new(crhack3d9_main);
    if (rett == NULL)
        return (NULL);
    if (!rett->main.init(&hdle, 90.0f, FALSE, 0, 0, D3DFMT_UNKNOWN,
                        D3DFMT_D24X8, TRUE, D3DMULTISAMPLE_NONE)) {
        if (!rett->main.init(&hdle, 90.0f, FALSE, 0, 0, D3DFMT_UNKNOWN,
                            D3DFMT_D16, TRUE, D3DMULTISAMPLE_NONE))
            goto _failure1;
    }
    if (!rett->texs.init())
        goto _failure2;
    if (!rett->base.init())
        goto _failure3;
    rett->inst.init();
    rett->main.set_camera(&rett->cam);
    rett->main.get_frustum(&rett->frt, -1.0f);
    return ((crh3d9_t)rett);

_failure3:
    rett->texs.free();
_failure2:
    rett->main.free();
_failure1:
    mem_free(rett);
    return (NULL);
}

/* ====================================== */
CR_API void crhack3d9_kill (crh3d9_t render)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    real->inst.free();
    real->base.free();
    real->texs.free();
    real->main.free();
    mem_free(render);
}

/* ======================================= */
CR_API bool crhack3d9_reset (crh3d9_t render)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    if (!real->main.reset())
        return (false);

    size_t              num;
    asy::crh3d9_texr*   tex;

    num = real->texs.size();
    tex = real->texs.data();
    for (size_t idx = 0; idx < num; idx++) {
        if (!tex[idx].reset())
            return (false);
    }
    return (true);
}

/* ============================================================ */
CR_API bool crhack3d9_instance (crh3d9_t render, const char* base,
            const vec3d_t* rote; const vec3d_t* move, const vec3d_t* scale)
{
}

/* ============================================================= */
CR_API bool crhack3d9_wavefront (crh3d9_t render, const char* name,
                    const ansi_t* obj, const ansi_t* mtl, bool_t swap_yz,
                                bool_t neg_z, const char* type)
{
    sWAVEFRONT          mesh;
    crhack3d9_main*     real;
    asy::crh3d9_texr    texr;
    asy::object_base    base;

    if (!wfront_obj_load(&mesh, obj, swap_yz, neg_z))
        return (false);
    if (!wfront_mtl_load(&mesh, mtl))
        goto _failure;
    real = (crhack3d9_main*)render;
    if (type == NULL || strcmp(type, "fixed") == 0) {
        for (leng_t idx = 0; idx < mesh.n_m; idx++) {
            if (mesh.p_m[idx].map_kd == NULL)
                continue;
            if (!texr.init(&real->main, mesh.p_m[idx].map_kd, 1))
                goto _failure;
        }
        if (!create_crh3d9_base_wf(&base, &mesh, create_crh3d9_attr_wf_fixed,
                        create_crh3d9_mesh_wf_ss, &real->texs, &real->main))
            goto _failure;
    }
    else {
        goto _failure;
    }
    if (real->base.insert(name, &base) == NULL) {
        base.free();
        goto _failure;
    }
    return (true);

_failure:
    wfront_obj_free(&mesh);
    return (false);
}
