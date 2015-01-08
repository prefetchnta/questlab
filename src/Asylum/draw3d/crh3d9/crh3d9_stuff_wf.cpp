/*
***************************************
*   Asylum3D @ 2014-12-24
***************************************
*/

#include "../../asylum.hpp"

/*****************************************************************************/
/*                                  Effect                                   */
/*****************************************************************************/

/* Asylum Namespace */
namespace asy {

/***********************************/
/* Wavefront Effect Vertex (Fixed) */
/***********************************/
class crh3d9_ffct_wf_v_fixed : public IEffect
{
private:
    uint_t              m_vxfvf;
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ====================================================== */
    crh3d9_ffct_wf_v_fixed (uint_t fvf, const crh3d9_main* main)
    {
        m_vxfvf = fvf;
        m_devcs = main->get_main()->dev;
    }

    /* ============================ */
    virtual ~crh3d9_ffct_wf_v_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        m_devcs->SetRenderState(D3DRS_COLORVERTEX, FALSE);
        m_devcs->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
        m_devcs->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
        m_devcs->SetFVF(m_vxfvf);
    }

    /* =============== */
    virtual void leave ()
    {
        m_devcs->SetRenderState(D3DRS_COLORVERTEX, TRUE);
        m_devcs->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
        m_devcs->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2);
    }
};

/**********************************/
/* Wavefront Effect Color (Fixed) */
/**********************************/
class crh3d9_ffct_wf_c_fixed : public IEffect
{
private:
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ========================================== */
    crh3d9_ffct_wf_c_fixed (const crh3d9_main* main)
    {
        m_devcs = main->get_main()->dev;
    }

    /* ============================ */
    virtual ~crh3d9_ffct_wf_c_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        m_devcs->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        m_devcs->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    }

    /* =============== */
    virtual void leave ()
    {
    }
};

/************************************/
/* Wavefront Effect Texture (Fixed) */
/************************************/
class crh3d9_ffct_wf_t_fixed : public IEffect
{
private:
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ========================================== */
    crh3d9_ffct_wf_t_fixed (const crh3d9_main* main)
    {
        m_devcs = main->get_main()->dev;
    }

    /* ============================ */
    virtual ~crh3d9_ffct_wf_t_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        m_devcs->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        m_devcs->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        m_devcs->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }

    /* =============== */
    virtual void leave ()
    {
    }
};

}   /* namespace */

/* =================================================================== */
CR_API asy::IEffect* create_crh3d9_ffct_wf_fixed (uint_t fvf, bool_t tex,
                                                  const asy::crh3d9_main* main)
{
    asy::IEffect*   ffct;

    if (fvf != 0)
        ffct = new asy::crh3d9_ffct_wf_v_fixed (fvf, main);
    else if (tex)
        ffct = new asy::crh3d9_ffct_wf_t_fixed (main);
    else
        ffct = new asy::crh3d9_ffct_wf_c_fixed (main);
    return (ffct);
}

/*****************************************************************************/
/*                                Attribute                                  */
/*****************************************************************************/

/* Asylum Namespace */
namespace asy {

/*******************************/
/* Wavefront Attribute (Fixed) */
/*******************************/
class crh3d9_attr_wf_fixed : public IAttrib
{
protected:
    D3DMATERIAL9        m_mtl;
    sD3D9_TEXR*         m_map_kd;
    LPDIRECT3DDEVICE9   m_device;

public:
    /* ======================================== */
    crh3d9_attr_wf_fixed (const crh3d9_main* main)
    {
        m_type = 0;
        m_device = main->get_main()->dev;
    }

    /* ========================== */
    virtual ~crh3d9_attr_wf_fixed ()
    {
    }

public:
    /* ===================================================================== */
    bool init_ff (const sWAVEFRONT_M* mtl, const map_acs<crh3d9_texr>* texpool)
    {
        crh3d9_texr*    texr;

        if (mtl->map_kd != NULL) {
            texr = texpool->get(mtl->map_kd);
            if (texr == NULL)
                return (false);
            m_map_kd = texr->get_texr();
        }
        else {
            m_map_kd = NULL;
        }
        m_mtl.Diffuse.r = mtl->kd.x;
        m_mtl.Diffuse.g = mtl->kd.y;
        m_mtl.Diffuse.b = mtl->kd.z;
        m_mtl.Diffuse.a = mtl->d;
        if (m_mtl.Diffuse.a < 0.0f)
            m_mtl.Diffuse.a = 0.0f;
        else
        if (m_mtl.Diffuse.a > 1.0f)
            m_mtl.Diffuse.a = 1.0f;
        m_mtl.Ambient.r = mtl->ka.x;
        m_mtl.Ambient.g = mtl->ka.y;
        m_mtl.Ambient.b = mtl->ka.z;
        m_mtl.Ambient.a = 0.0f;
        m_mtl.Specular.r = mtl->ks.x;
        m_mtl.Specular.g = mtl->ks.y;
        m_mtl.Specular.b = mtl->ks.z;
        m_mtl.Specular.a = 0.0f;
        m_mtl.Emissive.r = 0.0f;
        m_mtl.Emissive.g = 0.0f;
        m_mtl.Emissive.b = 0.0f;
        m_mtl.Emissive.a = 0.0f;
        m_mtl.Power = mtl->ns;
        if (m_mtl.Diffuse.a < 1.0f)
            m_type |= ATTR_TYPE_ALPHAOP;
        if (m_map_kd != NULL)
            m_type |= ATTR_TYPE_TEXTURE;
        if (m_mtl.Specular.r <= 0.0f && m_mtl.Specular.g <= 0.0f &&
            m_mtl.Specular.b <= 0.0f && m_mtl.Specular.a <= 0.0f)
            m_type |= ATTR_TYPE_SPECULAR;
        return (true);
    }

    /* ================ */
    virtual void commit ()
    {
        if (m_map_kd != NULL)
            m_device->SetTexture(0, m_map_kd->obj.base);
        else
            m_device->SetTexture(0, NULL);
        m_device->SetMaterial(&m_mtl);
    }
};

}   /* namespace */

/* ==================================================================== */
CR_API asy::IAttrib* create_crh3d9_attr_wf_fixed (const sWAVEFRONT_M* mtl,
    const asy::map_acs<asy::crh3d9_texr>* texpool, const asy::crh3d9_main* main)
{
    asy::crh3d9_attr_wf_fixed*  attr;

    attr = new asy::crh3d9_attr_wf_fixed (main);
    if (attr != NULL) {
        if (!attr->init_ff(mtl, texpool)) {
            delete attr;
            attr = NULL;
        }
    }
    return ((asy::IAttrib*)attr);
}

/*****************************************************************************/
/*                                   Mesh                                    */
/*****************************************************************************/

/* Asylum Namespace */
namespace asy {

/**********************************/
/* Wavefront Mesh (Single Stream) */
/**********************************/
class crh3d9_mesh_wf_ss : public IMesh
{
private:
    sD3D9_MESH*         m_mesh;
    const sD3D9_CALL*   m_call;
    LPDIRECT3DDEVICE9   m_devs;

public:
    /* ===================================== */
    crh3d9_mesh_wf_ss (const crh3d9_main* main)
    {
        m_mesh = NULL;
        m_call = main->get_call();
        m_devs = main->get_main()->dev;
    }

    /* ======================= */
    virtual ~crh3d9_mesh_wf_ss ()
    {
        if (m_mesh != NULL)
            m_call->release_mesh(m_mesh);
    }

public:
    /* ==================================================================== */
    bool init2_ss (const sWAVEFRONT* obj, leng_t idx, const crh3d9_main* main)
    {
        leng_t  nv, ni, bpv;

        nv = wfront_gen_mesh2(NULL, NULL, NULL, 0, 0, 0, NULL, &ni, obj, idx);
        if (nv == 0)
            return (false);
        bpv = sizeof(vec3d_t);
        if (obj->p_f[0].idx[2] != 0)
            bpv += sizeof(vec3d_t);
        if (obj->p_f[0].idx[1] != 0)
            bpv += sizeof(vec2d_t);

        void_t* vb;
        void_t* ib;

        vb = mem_calloc(nv, bpv);
        if (vb == NULL)
            return (false);
        if (nv > 65536)
            ib = mem_calloc(ni, sizeof(int32u));
        else
            ib = mem_calloc(ni, sizeof(int16u));
        if (ib == NULL) {
            mem_free(vb);
            return (false);
        }

        uint_t      fvf;
        vec3d_t*    xyz;
        vec3d_t*    nrm;
        vec2d_t*    uvw;

        fvf = D3DFVF_XYZ;
        xyz = (vec3d_t*)vb;
        if (obj->p_f[0].idx[2] != 0) {
            fvf |= D3DFVF_NORMAL;
            nrm = xyz + 1;
            uvw = (vec2d_t*)(nrm + 1);
        }
        else {
            nrm = NULL;
            uvw = (vec2d_t*)(xyz + 1);
        }
        if (obj->p_f[0].idx[1] != 0)
            fvf |= D3DFVF_TEX1;
        else
            uvw = NULL;
        wfront_gen_mesh2(xyz, nrm, uvw, bpv, 0, 0, ib, NULL, obj, idx);
        m_mesh = m_call->create_mesh_vib(main->get_main(), nv, bpv, ni,
                            D3DPOOL_MANAGED, D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED,
                                    D3DUSAGE_WRITEONLY, fvf, vb, ib);
        mem_free(vb);
        mem_free(ib);
        if (m_mesh == NULL)
            return (false);
        return (true);
    }

    /* ================ */
    virtual void commit ()
    {
        m_devs->SetStreamSource(0, m_mesh->vbuf, 0, m_mesh->nbpv);
        m_devs->SetIndices(m_mesh->ibuf);
        m_devs->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_mesh->vnum, 0, m_mesh->ntri);
    }
};

}   /* namespace */

/* ========================================================================= */
CR_API asy::IMesh* create_crh3d9_mesh_wf_ss (const sWAVEFRONT* obj, leng_t idx,
                                             const asy::crh3d9_main* main)
{
    asy::crh3d9_mesh_wf_ss* mesh;

    mesh = new asy::crh3d9_mesh_wf_ss (main);
    if (mesh != NULL) {
        if (!mesh->init2_ss(obj, idx, main)) {
            delete mesh;
            mesh = NULL;
        }
    }
    return ((asy::IMesh*)mesh);
}

/*****************************************************************************/
/*                               Object Base                                 */
/*****************************************************************************/

/* ================================= */
static void wavefront_kill (void* real)
{
    wfront_obj_free((sWAVEFRONT*)real);
    mem_free(real);
}

/* ========================================================== */
static void wavefront_tran (asy::object_inst* dest, void* param,
                            const vec3d_t* rote, const vec3d_t* move,
                            const vec3d_t* scale)
{
    sWAVEFRONT*         mesh;
    const sD3D9_CALL*   call;

    call = (const sD3D9_CALL*)param;
    if (rote == NULL)
        call->util_make_tran2(&dest->tran, scale, NULL, 0.0f, move);
    else
        call->util_make_tran1(&dest->tran, scale, rote->x, rote->y, rote->z, move);
    if (dest->type == INST_TYPE_STATIC) {
        if (rote == NULL && scale == NULL) {
            mem_cpy(&dest->bound.aabb, &dest->base->aabb, sizeof(sAABB));
            if (move != NULL) {
                for (int idx = 0; idx < 8; idx++) {
                    dest->bound.aabb.v[idx].x += move->x;
                    dest->bound.aabb.v[idx].y += move->y;
                    dest->bound.aabb.v[idx].z += move->z;
                }
            }
        }
        else
        {
            vec3d_t min, max, tmp;

            // rebuild AABB box
            mesh = (sWAVEFRONT*)dest->base->real;
            call->util_tran_vec3d(&tmp, mesh->p_v, &dest->tran);
            mem_cpy(&min, &tmp, sizeof(vec3d_t));
            mem_cpy(&max, &tmp, sizeof(vec3d_t));
            for (leng_t idx = 1; idx < mesh->n_v; idx++) {
                call->util_tran_vec3d(&tmp, &mesh->p_v[idx], &dest->tran);
                if (tmp.x < min.x) min.x = tmp.x;
                if (tmp.y < min.y) min.y = tmp.y;
                if (tmp.z < min.z) min.z = tmp.z;
                if (tmp.x > max.x) max.x = tmp.x;
                if (tmp.y > max.y) max.y = tmp.y;
                if (tmp.z > max.z) max.z = tmp.z;
            }
            bound_gen_aabb(&dest->bound.aabb, &min, &max);
        }
    }
    else {  // dynamic object don't support scaling
        mem_cpy(&dest->bound.ball, &dest->base->ball, sizeof(sSPHERE));
        if (move != NULL) {
            dest->bound.ball.center.x += move->x;
            dest->bound.ball.center.y += move->y;
            dest->bound.ball.center.z += move->z;
        }
    }
}

/* =========================================================================== */
CR_API bool create_crh3d9_base_wf (asy::object_base* base, const sWAVEFRONT* obj,
                            create_crh3d9_attr_wf_t fattr, create_crh3d9_mesh_wf_t fmesh,
                                    const asy::map_acs<asy::crh3d9_texr>* texpool,
                                            const asy::crh3d9_main* main)
{
    asy::commit_batch   cb;

    base->list.init();
    base->real = NULL;
    base->kill = NULL;
    for (leng_t idx = 0; idx < obj->n_m; idx++)
    {
        leng_t  ii, cnt = 0, cmp = idx + 1;

        for (ii = 0; ii < obj->n_g; ii++) {
            if (obj->p_g[ii].attr < cmp)
                break;
            if (obj->p_g[ii].attr == cmp)
                cnt += 1;
        }
        if (cnt == 0)
            continue;
        cb.attr = fattr(&obj->p_m[idx], texpool, main);
        if (cb.attr == NULL)
            goto _failure1;
        cb.mesh = mem_talloc(cnt + 1, asy::IMesh*);
        if (cb.mesh == NULL) {
            delete cb.attr;
            goto _failure1;
        }
        for (cnt = 0, ii = 0; ii < obj->n_g; ii++) {
            if (obj->p_g[ii].attr < cmp)
                break;
            if (obj->p_g[ii].attr == cmp) {
                cb.mesh[cnt] = fmesh(obj, ii, main);
                if (cb.mesh[cnt] == NULL)
                    goto _failure2;
                cnt += 1;
            }
        }
        cb.mesh[cnt] = NULL;
        if (base->list.append(&cb) == NULL)
            goto _failure2;
    }
    if (base->list.size() == 0)
        goto _failure1;
    if (!base->list.no_grow())
        goto _failure1;
    base->real = struct_dup(obj, sWAVEFRONT);
    if (base->real == NULL)
        goto _failure1;
    base->kill = wavefront_kill;
    base->tran = wavefront_tran;
    bound_get_aabb(&base->aabb, obj->p_v, obj->n_v, sizeof(vec3d_t));
    bound_get_ball(&base->ball, obj->p_v, obj->n_v, sizeof(vec3d_t));
    return (true);

_failure2:
    cb.free();
_failure1:
    base->free();
    return (false);
}
