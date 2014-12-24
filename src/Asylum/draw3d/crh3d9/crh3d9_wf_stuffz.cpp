/*
***************************************
*   Asylum3D @ 2014-12-24
***************************************
*/

#include "../../asylum.hpp"

/* Asylum Namespace */
namespace asy {

/*******************************/
/* Wavefront Attribute (Fixed) */
/*******************************/
class crh3d9_attr_wf_fixed : public IAttrib
{
private:
#if !defined(ASY_USE_FIXED_3D)
    crh3d9_texr*    m_bump;
    crh3d9_texr*    m_map_ka;
    crh3d9_texr*    m_map_ks;
    crh3d9_texr*    m_map_ns;
#endif
    D3DMATERIAL9        m_mtl;
    crh3d9_texr*        m_map_kd;
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ======================================== */
    crh3d9_attr_wf_fixed (const crh3d9_main* main)
    {
        m_devcs = main->get_main()->dev;
    }

    /* ========================== */
    virtual ~crh3d9_attr_wf_fixed ()
    {
    }

public:
    /* ================================================================== */
    bool init (const sWAVEFRONT_M* mtl, const map_acs<crh3d9_texr>* texpool)
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
#if defined(ASY_USE_FIXED_3D)
        m_mtl.Specular.a = 0.0f;
        m_mtl.Emissive.r = 0.0f;
        m_mtl.Emissive.g = 0.0f;
        m_mtl.Emissive.b = 0.0f;
        m_mtl.Emissive.a = 0.0f;
        m_mtl.Power = mtl->ns;
#else
        m_mtl.Specular.a = mtl->ns;
        m_mtl.Emissive.r = mtl->tf.x;
        m_mtl.Emissive.g = mtl->tf.y;
        m_mtl.Emissive.b = mtl->tf.z;
        m_mtl.Emissive.a = 1.0f;
        if (mtl->map_ka != NULL) {
            m_map_ka = texpool->get(mtl->map_ka);
            if (m_map_ka == NULL)
                return (false);
        }
        else {
            m_map_ka = NULL;
        }
        if (mtl->map_ks != NULL) {
            m_map_ks = texpool->get(mtl->map_ks);
            if (m_map_ks == NULL)
                return (false);
        }
        else {
            m_map_ks = NULL;
        }
        if (mtl->map_ns != NULL) {
            m_map_ns = texpool->get(mtl->map_ns);
            if (m_map_ns == NULL)
                return (false);
        }
        else {
            m_map_ns = NULL;
        }
        if (mtl->bump != NULL) {
            m_bump = texpool->get(mtl->bump);
            if (m_bump == NULL)
                return (false);
        }
        else {
            m_bump = NULL;
        }
#endif
        return (true);
    }

    /* ================ */
    virtual void commit ()
    {
        if (m_map_kd != NULL)
            m_map_kd->apply(0);
        else
            m_devcs->SetTexture(0, NULL);
        m_devcs->SetMaterial(&m_mtl);
    }

    /* ================ */
    virtual int64u type ()
    {
        int64u  flag = 0ULL;

        if (m_mtl.Diffuse.a < 1.0f)
            flag |= ATTR_TYPE_TRANS;
        if (m_mtl.Specular.r <= 0.0f && m_mtl.Specular.g <= 0.0f &&
            m_mtl.Specular.b <= 0.0f && m_mtl.Specular.a <= 0.0f)
            flag |= ATTR_TYPE_SPECULAR;
        if (m_bump != NULL)
            flag |= ATTR_TYPE_NORMAL;
        return (flag);
    }
};

}   /* namespace */

/* ===================================================================== */
CR_API asy::IAttrib* create_crh3d9_attr_wf_fixed (const sWAVEFRONT_M* mtrl,
    const asy::map_acs<asy::crh3d9_texr>* texpool, const asy::crh3d9_main* main)
{
    asy::crh3d9_attr_wf_fixed*  attr;

    attr = new asy::crh3d9_attr_wf_fixed (main);
    if (attr != NULL) {
        if (!attr->init(mtrl, texpool)) {
            delete attr;
            attr = NULL;
        }
    }
    return ((asy::IAttrib*)attr);
}
