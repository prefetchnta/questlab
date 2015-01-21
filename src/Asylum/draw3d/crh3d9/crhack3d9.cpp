/*
***************************************
*   Asylum3D @ 2014-12-30
***************************************
*/

#include "../../asylum.hpp"

#define FRT_BIAS    -1.0f

static const fp32_t s_no_trans[16] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

void crhack3d9_commit_fixed (asy::crh3d9_main* main, const asy::tree_l<asy::commit_pipe>* pipe);

/*************************/
/* CrHack3D9 Effect Node */
/*************************/
struct crhack3d9_node
{
    asy::atree_unit<asy::commit_pipe>*  ptr;

    /* ========= */
    void free () {}
};

/*************************/
/* CrHack3D9 Main Object */
/*************************/
struct crhack3d9_main
{
    asy::crh3d9_main                main;
    asy::map_acs<asy::crh3d9_texr>  texs;
    asy::map_acs<asy::object_base>  base;
    asy::map_acs<asy::object_inst>  inst;
    asy::map_acs<crhack3d9_node>    node;
    asy::tree_l<asy::commit_pipe>   pipe;

    void (*commit) (asy::crh3d9_main* main, const asy::tree_l<asy::commit_pipe>* pipe);

    sCAMERA     cam;
    sFRUSTUM    frt;
};

/* ============================================================== */
CR_API crh3d9_t crhack3d9_init (HWND hwnd, fp32_t fovy, fp32_t zfar)
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
    if (!rett->main.init(&hdle, fovy, zfar, FALSE, 0, 0, D3DFMT_UNKNOWN,
                        D3DFMT_D24X8, TRUE, D3DMULTISAMPLE_NONE)) {
        if (!rett->main.init(&hdle, fovy, zfar, FALSE, 0, 0, D3DFMT_UNKNOWN,
                            D3DFMT_D16, TRUE, D3DMULTISAMPLE_NONE))
            goto _failure1;
    }
    if (!rett->texs.init())
        goto _failure2;
    if (!rett->base.init())
        goto _failure3;
    if (!rett->inst.init())
        goto _failure4;
    if (!rett->node.init())
        goto _failure5;
    rett->main.set_camera(&rett->cam);
    rett->main.get_frustum(&rett->frt, FRT_BIAS);
    mem_set(&rett->pipe, 0, sizeof(rett->pipe));
    rett->commit = crhack3d9_commit_fixed;
    return ((crh3d9_t)rett);

_failure5:
    rett->inst.free();
_failure4:
    rett->base.free();
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
    if (real->pipe.size() != 0)
        real->pipe.free();
    real->node.free();
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

/* ======================================== */
CR_API void crhack3d9_commit (crh3d9_t render)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    real->commit(&real->main, &real->pipe);
}

/* Asylum Namespace */
namespace asy {

/**************************/
/* CrHack3D9 Render Clear */
/**************************/
class crhack3d9_render_clear : public asylum
{
public:
    /* ================================= */
    bool doit (void* ctx, commit_pipe* obj)
    {
        CR_NOUSE(ctx);
        obj->stuffz.clear();
        return (true);
    }

    /* ================================= */
    void back (void* ctx, commit_pipe* obj)
    {
        CR_NOUSE(ctx);
        CR_NOUSE(obj);
    }
};

}   /* namespace */

/* ======================================= */
CR_API void crhack3d9_clean (crh3d9_t render)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    real->pipe.trav_bfs<asy::crhack3d9_render_clear>(NULL);
}

/* ============================================= */
CR_API sFRUSTUM* crhack3d9_update (crh3d9_t render)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    real->main.get_call()->tran_upd_view(real->main.get_tran());
    real->main.get_frustum(&real->frt, FRT_BIAS);
    return (&real->frt);
}

/* ====================================== */
CR_API void crhack3d9_show (crh3d9_t render)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    real->main.draw_show();
}

/* ======================================================== */
CR_API bool crhack3d9_mode (crh3d9_t render, const char* mode)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    if (mode == NULL || strcmp(mode, "fixed") == 0) {
        real->commit = crhack3d9_commit_fixed;
        return (true);
    }
    return (false);
}

/* ===================================================== */
CR_API asy::crh3d9_main* crhack3d9_device (crh3d9_t render)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    return (&real->main);
}

/* ============================================ */
CR_API sCAMERA* crhack3d9_camera (crh3d9_t render)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    return (&real->cam);
}

/* ============================================== */
CR_API size_t crhack3d9_inst_count (crh3d9_t render)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    return (real->inst.size());
}

/* ======================================================== */
CR_API asy::object_inst* crhack3d9_inst_list (crh3d9_t render)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    return (real->inst.data());
}

/* ========================================================================= */
CR_API asy::object_inst* crhack3d9_inst_get (crh3d9_t render, const char* name)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    return (real->inst.get(name));
}

/* ====================================================================== */
CR_API bool crhack3d9_inst_visable (crh3d9_t render, asy::object_inst* inst)
{
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    if (inst->type == INST_TYPE_STATIC) {
        if (!frustum_aabb(&real->frt, &inst->bound.aabb))
            return (false);
    }
    else {
        if (!frustum_ball(&real->frt, &inst->bound.ball))
            return (false);
    }
    return (true);
}

/* ========================================================================= */
CR_API asy::commit_pipe* crhack3d9_pipe_get (crh3d9_t render, const char* name)
{
    crhack3d9_node* node;
    crhack3d9_main* real;

    real = (crhack3d9_main*)render;
    node = real->node.get(name);
    if (node == NULL || node->ptr == NULL)
        return (NULL);
    return (&node->ptr->user);
}

/* ===================================================================================== */
CR_API bool crhack3d9_pipe_add (asy::commit_pipe* pipe, asy::object_inst* inst, size_t idx)
{
    asy::commit_unit    unit;

    unit.inst = inst;
    unit.unit = inst->base->list.get(idx);
    if (pipe->stuffz.append(&unit) == NULL)
        return (false);
    return (true);
}

/* ======================================================================= */
CR_API asy::commit_pipe* crhack3d9_effect (crh3d9_t render, const char* name,
                                        const char* uppe, asy::IEffect* effect)
{
    crhack3d9_node      node;
    crhack3d9_node*     nnew;
    crhack3d9_node*     find;
    crhack3d9_main*     real;
    asy::commit_pipe    pipe;

    node.ptr = NULL;
    real = (crhack3d9_main*)render;
    nnew = real->node.insert(name, &node);
    if (nnew == NULL)
        return (NULL);
    pipe.effect = effect;
    pipe.stuffz.init();
    if (real->pipe.size() == 0) {
        if (!real->pipe.init(&pipe))
            return (NULL);
        nnew->ptr = real->pipe.root();
    }
    else {
        find = real->node.get(uppe);
        if (find == NULL || find->ptr == NULL)
            return (NULL);
        nnew->ptr = real->pipe.append(find->ptr, &pipe);
        if (nnew->ptr == NULL)
            return (NULL);
    }
    if (effect != NULL)
        effect->add_ref();
    return (&nnew->ptr->user);
}

/* ============================================================ */
CR_API bool crhack3d9_instance (crh3d9_t render, const char* name,
                                const char* base, const vec3d_t* rote,
                                const vec3d_t* move, const vec3d_t* scale)
{
    crhack3d9_main*     real;
    asy::object_inst    inst;

    real = (crhack3d9_main*)render;
    inst.base = real->base.get(base);
    if (inst.base == NULL)
        return (false);
    if (name[0] == '$') {
        scale = NULL;
        inst.type = INST_TYPE_DYNAMIC;
    }
    else {
        inst.type = INST_TYPE_STATIC;
    }
    if (inst.base->tran != NULL) {
        inst.base->tran(&inst, (void*)(real->main.get_call()), rote, move, scale);
    }
    else {
        if (inst.type == INST_TYPE_STATIC) {
            mem_cpy(&inst.bound.aabb, &inst.base->aabb, sizeof(sAABB));
            if (move != NULL) {
                for (int idx = 0; idx < 8; idx++) {
                    inst.bound.aabb.v[idx].x += move->x;
                    inst.bound.aabb.v[idx].y += move->y;
                    inst.bound.aabb.v[idx].z += move->z;
                }
            }
        }
        else {
            mem_cpy(&inst.bound.ball, &inst.base->ball, sizeof(sSPHERE));
            if (move != NULL) {
                inst.bound.ball.center.x += move->x;
                inst.bound.ball.center.y += move->y;
                inst.bound.ball.center.z += move->z;
            }
        }
        mem_cpy(inst.tran.m, s_no_trans, sizeof(mat4x4_t));
        if (move != NULL) {
            inst.tran.m[12] = move->x;
            inst.tran.m[13] = move->y;
            inst.tran.m[14] = move->z;
        }
    }
    if (real->inst.insert(name, &inst) == NULL)
        return (false);
    return (true);
}

/* ============================================================= */
CR_API bool crhack3d9_wavefront (crh3d9_t render, const char* name,
                    const ansi_t* obj, const ansi_t* mtl, bool_t swap_yz,
                        bool_t neg_z, fp32_t scale, const char* type)
{
    sWAVEFRONT          mesh;
    crhack3d9_main*     real;
    asy::crh3d9_texr    texr;
    asy::object_base    base;

    if (!wfront_obj_load(&mesh, obj, swap_yz, neg_z))
        return (false);
    if (scale > 0.0f) {
        for (leng_t idx = 0; idx < mesh.n_v; idx++) {
            mesh.p_v[idx].x *= scale;
            mesh.p_v[idx].y *= scale;
            mesh.p_v[idx].z *= scale;
        }
    }
    if (!wfront_mtl_load(&mesh, mtl))
        goto _failure;
    if (!wfront_obj_combine(&mesh))
        goto _failure;
    real = (crhack3d9_main*)render;
    if (type == NULL || strcmp(type, "fixed") == 0) {
        for (leng_t idx = 0; idx < mesh.n_m; idx++) {
            const char* str = mesh.p_m[idx].map_kd;
            if (str == NULL)
                continue;
            if (!texr.init(&real->main, str, 1))
                goto _failure;
            if (real->texs.insert(str, &texr) == NULL) {
                texr.free();
                goto _failure;
            }
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
