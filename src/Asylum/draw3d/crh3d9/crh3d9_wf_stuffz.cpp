/*
***************************************
*   Asylum3D @ 2014-12-24
***************************************
*/

#include "../../asylum.hpp"

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
    crh3d9_texr*        m_map_kd;
    LPDIRECT3DDEVICE9   m_device;

public:
    /* ======================================== */
    crh3d9_attr_wf_fixed (const crh3d9_main* main)
    {
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
        if (mtl->map_kd != NULL) {
            m_map_kd = texpool->get(mtl->map_kd);
            if (m_map_kd == NULL)
                return (false);
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
        m_type = ATTR_TYPE_NORMAL;
        if (m_mtl.Diffuse.a < 1.0f)
            m_type |= ATTR_TYPE_TRANS;
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
            m_map_kd->apply(0);
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
        if (!attr->init(mtl, texpool)) {
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
    sD3D9_MAIN*         m_main;
    const sD3D9_CALL*   m_call;
    LPDIRECT3DDEVICE9   m_devs;

public:
    /* ===================================== */
    crh3d9_mesh_wf_ss (const crh3d9_main* main)
    {
        m_mesh = NULL;
        m_main = main->get_main();
        m_call = main->get_call();
        m_devs = m_main->dev;
    }

    /* ======================= */
    virtual ~crh3d9_mesh_wf_ss ()
    {
        if (m_mesh != NULL)
            m_call->release_mesh(m_mesh);
    }

public:
    /* =========================================== */
    bool init2_ss (const sWAVEFRONT* obj, leng_t idx)
    {
        leng_t  nv, ni, bpv;

        nv = wfront_gen_mesh2(NULL, NULL, NULL, 0, 0, 0, NULL, &ni, obj, idx);
        if (nv == 0)
            return (false);
        bpv = sizeof(vec3d_t);

        void_t* vb;
        void_t* ib;

        if (obj->p_f[0].idx[2] != 0)
            bpv += sizeof(vec3d_t);
        if (obj->p_f[0].idx[1] != 0)
            bpv += sizeof(vec2d_t);
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
        m_mesh = m_call->create_mesh_vib(m_main, nv, bpv, ni, D3DPOOL_MANAGED, D3DUSAGE_WRITEONLY,
                                         D3DPOOL_MANAGED, D3DUSAGE_WRITEONLY, fvf, vb, ib);
        mem_free(ib);
        if (m_mesh == NULL) {
            mem_free(vb);
            return (false);
        }
        bound_get_aabb(&m_aabb, (vec3d_t*)vb, nv, bpv);
        bound_get_ball(&m_ball, (vec3d_t*)vb, nv, bpv);
        mem_free(vb);
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
        if (!mesh->init2_ss(obj, idx)) {
            delete mesh;
            mesh = NULL;
        }
    }
    return ((asy::IMesh*)mesh);
}