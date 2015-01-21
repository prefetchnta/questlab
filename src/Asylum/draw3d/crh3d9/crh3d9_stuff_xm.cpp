/*
***************************************
*   Asylum3D @ 2015-01-21
***************************************
*/

#include "../../asylum.hpp"

/*****************************************************************************/
/*                                Attribute                                  */
/*****************************************************************************/

/* Asylum Namespace */
namespace asy {

/****************************/
/* X Mesh Attribute (Fixed) */
/****************************/
class crh3d9_attr_xm_fixed : public IAttrib
{
protected:
    DWORD               m_idx;
    LPD3DXMESH          m_msh;
    D3DMATERIAL9*       m_mtl;
    sD3D9_TEXR*         m_map_kd;
    LPDIRECT3DDEVICE9   m_device;

public:
    /* ======================================== */
    crh3d9_attr_xm_fixed (const crh3d9_main* main)
    {
        m_type = 0;
        m_device = main->get_main()->dev;
    }

    /* ========================== */
    virtual ~crh3d9_attr_xm_fixed ()
    {
    }

public:
    /* ================================================================================ */
    bool init_ff (const sD3D9_XMSH* xmesh, DWORD idx, const map_acs<crh3d9_texr>* texpool)
    {
        int32u*         ptr;
        crh3d9_texr*    texr;
        D3DXMATERIAL*   mtrl;

        mtrl = (D3DXMATERIAL*)xmesh->xattr->GetBufferPointer();
        mtrl += idx;
        if (mtrl->pTextureFilename != NULL) {
            texr = texpool->get(mtrl->pTextureFilename);
            if (texr == NULL)
                return (false);
            m_map_kd = texr->get_texr();
        }
        else {
            m_map_kd = NULL;
        }
        m_idx = idx;
        m_msh = xmesh->xmesh;
        m_mtl = &mtrl->MatD3D;
        m_mtl->Ambient.r = m_mtl->Diffuse.r;
        m_mtl->Ambient.g = m_mtl->Diffuse.g;
        m_mtl->Ambient.b = m_mtl->Diffuse.b;
        m_mtl->Ambient.a = 0.0f;
        if (m_mtl->Diffuse.a < 1.0f)
            m_type |= ATTR_TYPE_ALPHAOP;
        if (m_map_kd != NULL)
            m_type |= ATTR_TYPE_TEXTURE;
        ptr = (int32u*)(&m_mtl->Specular);
        for (idx = 0; idx < 3; idx++) {
            if (*ptr != 0) {
                m_type |= ATTR_TYPE_SPECULAR;
                break;
            }
            ptr += 1;
        }
        return (true);
    }

    /* ================ */
    virtual void commit ()
    {
        if (m_map_kd != NULL)
            m_device->SetTexture(0, m_map_kd->obj.base);
        else
            m_device->SetTexture(0, NULL);
        m_device->SetMaterial(m_mtl);
        m_msh->DrawSubset(m_idx);
    }
};

}   /* namespace */

/* =============================================================================== */
CR_API asy::IAttrib* create_crh3d9_attr_xm_fixed (const sD3D9_XMSH* xmesh, DWORD idx,
    const asy::map_acs<asy::crh3d9_texr>* texpool, const asy::crh3d9_main* main)
{
    asy::crh3d9_attr_xm_fixed*  attr;

    if (idx >= xmesh->nattr)
        return (NULL);
    attr = new asy::crh3d9_attr_xm_fixed (main);
    if (attr != NULL) {
        if (!attr->init_ff(xmesh, idx, texpool)) {
            delete attr;
            attr = NULL;
        }
    }
    return ((asy::IAttrib*)attr);
}

/*****************************************************************************/
/*                               Object Base                                 */
/*****************************************************************************/

/* ============================= */
static void xmesh_kill (void* real)
{
    sD3D9_XMSH*         mesh;
    const sD3D9_CALL*   call;

    mesh = (sD3D9_XMSH*)real;
    call = (const sD3D9_CALL*)mesh->xcall;
    call->release_xmsh(mesh);
}

/* ====================================================== */
static void xmesh_tran (asy::object_inst* dest, void* param,
                        const vec3d_t* rote, const vec3d_t* move,
                        const vec3d_t* scale)
{
    sD3D9_XMSH*         mesh;
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
            DWORD   bpv, count;
            vec3d_t min, max, tmp, *p_v;

            // rebuild AABB box
            mesh = (sD3D9_XMSH*)dest->base->real;
            bpv = mesh->GetNumBytesPerVertex();
            count = mesh->GetNumVertices();
            mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)(&p_v));
            call->util_tran_vec3d(&tmp, p_v, &dest->tran);
            p_v = (vec3d_t*)((byte_t*)p_v + bpv);
            mem_cpy(&min, &tmp, sizeof(vec3d_t));
            mem_cpy(&max, &tmp, sizeof(vec3d_t));
            for (DWORD idx = 1; idx < count; idx++) {
                call->util_tran_vec3d(&tmp, p_v, &dest->tran);
                p_v = (vec3d_t*)((byte_t*)p_v + bpv);
                if (tmp.x < min.x) min.x = tmp.x;
                if (tmp.y < min.y) min.y = tmp.y;
                if (tmp.z < min.z) min.z = tmp.z;
                if (tmp.x > max.x) max.x = tmp.x;
                if (tmp.y > max.y) max.y = tmp.y;
                if (tmp.z > max.z) max.z = tmp.z;
            }
            bound_gen_aabb(&dest->bound.aabb, &min, &max);
            mesh->UnlockVertexBuffer();
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

/* ======================================================================= */
CR_API bool create_crh3d9_base_xm (asy::object_base* base, sD3D9_XMSH* xmesh,
                create_crh3d9_attr_xm_t fattr, const asy::map_acs<asy::crh3d9_texr>* texpool,
                                    const asy::crh3d9_main* main)
{
    asy::commit_batch   cb;

    base->list.init();
    base->real = NULL;
    base->kill = NULL;
    for (DWORD idx = 0; idx < xmesh->nattr; idx++) {
        cb.attr = fattr(xmesh, idx, texpool, main);
        if (cb.attr == NULL)
            goto _failure1;
        cb.mesh = NULL;
        if (base->list.append(&cb) == NULL)
            goto _failure2;
    }
    if (base->list.size() == 0)
        goto _failure1;
    if (!base->list.no_grow())
        goto _failure1;
    base->real = xmesh;
    xmesh->xcall = (void_t*)main->get_call();
    base->type = MODEL_TYPE_X_MESH;
    base->kill = xmesh_kill;
    base->tran = xmesh_tran;
    bound_gen_aabb(&base->aabb, (vec3d_t*)(&xmesh->min),
                                (vec3d_t*)(&xmesh->max));
    base->ball.center.x = xmesh->center.x;
    base->ball.center.y = xmesh->center.y;
    base->ball.center.z = xmesh->center.z;
    base->ball.radius = xmesh->radius;
    return (true);

_failure2:
    cb.free();
_failure1:
    base->free();
    return (false);
}
