/*
***************************************
*   Asylum3D @ 2014-12-11
***************************************
*/

#ifndef __CRH3D9_MTL_WF_HPP__
#define __CRH3D9_MTL_WF_HPP__

/* Asylum Namespace */
namespace asy {

/******************/
/* Wavefront Mesh */
/******************/
class crh3d9_mesh_wf : public asylum
{
private:
    sD3D9_MAIN*         m_main;
    sD3D9_MESH*         m_mesh;
    const sD3D9_CALL*   m_call;

public:
    /* ==================================================================== */
    bool init2_ss (const crh3d9_main* main, const sWAVEFRONT* obj, leng_t idx)
    {
        leng_t  nv, ni;

        nv = wfront_gen_mesh2(NULL, NULL, NULL, 0, 0, 0, NULL, &ni, obj, idx);
        if (nv == 0)
            return (false);

        void_t* vb;
        void_t* ib;
        leng_t  bpv;

        bpv = sizeof(vec3d_t);
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
        m_main = main->get_main();
        m_call = main->get_call();
        m_mesh = m_call->create_mesh_vib(m_main, nv, bpv, ni, D3DPOOL_MANAGED, D3DUSAGE_WRITEONLY,
                                         D3DPOOL_MANAGED, D3DUSAGE_WRITEONLY, fvf, vb, ib);
        mem_free(vb);
        mem_free(ib);
        if (m_mesh == NULL)
            return (false);
        return (true);
    }

    /* ====== */
    void free ()
    {
        m_call->release_mesh(m_mesh);
    }

    /* ================ */
    void apply_ss () const
    {
#if defined(ASY_USE_FIXED_3D)
        m_main->dev->SetFVF(m_mesh->fvf);
#endif
        m_main->dev->SetStreamSource(0, m_mesh->vbuf, 0, m_mesh->nbpv);
        m_main->dev->SetIndices(m_mesh->ibuf);
        m_main->dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_mesh->vnum, 0, m_mesh->ntri);
    }
};

}   /* namespace */

#endif  /* __CRH3D9_MTL_WF_HPP__ */
