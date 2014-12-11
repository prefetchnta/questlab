/*
***************************************
*   Asylum3D @ 2014-12-11
***************************************
*/

#ifndef __CRH3D9_MTL_WF_HPP__
#define __CRH3D9_MTL_WF_HPP__

/* Asylum Namespace */
namespace asy {

/**********************/
/* Wavefront Material */
/**********************/
class crh3d9_mtl_wf : public asylum
{
private:
#if defined(ASY_USE_FIXED_3D)
    D3DMATERIAL9    m_mtl;
#else
    vec4d_t         m_ka;
    vec4d_t         m_kd;
    vec4d_t         m_ks;
    vec4d_t         m_tf;
    crh3d9_texr*    m_bump;
    crh3d9_texr*    m_map_ka;
    crh3d9_texr*    m_map_ks;
    crh3d9_texr*    m_map_ns;
#endif
    crh3d9_texr*    m_map_kd;

public:
    /* ============================================================== */
    bool init (const sWAVEFRONT_M* mtl, const map_acs<crh3d9_texr>* tex)
    {
        if (mtl->map_kd != NULL) {
            m_map_kd = tex->get(mtl->map_kd);
            if (m_map_kd == NULL)
                return (false);
        }
        else {
            m_map_kd = NULL;
        }
#if defined(ASY_USE_FIXED_3D)
        m_mtl.Diffuse.r = mtl->kd.x;
        m_mtl.Diffuse.g = mtl->kd.y;
        m_mtl.Diffuse.b = mtl->kd.z;
        m_mtl.Diffuse.a = mtl->d;
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
#else
        if (mtl->bump != NULL) {
            m_bump = tex->get(mtl->bump);
            if (m_bump == NULL)
                return (false);
        }
        else {
            m_bump = NULL;
        }
        if (mtl->map_ka != NULL) {
            m_map_ka = tex->get(mtl->map_ka);
            if (m_map_ka == NULL)
                return (false);
        }
        else {
            m_map_ka = NULL;
        }
        if (mtl->map_ks != NULL) {
            m_map_ks = tex->get(mtl->map_ks);
            if (m_map_ks == NULL)
                return (false);
        }
        else {
            m_map_ks = NULL;
        }
        if (mtl->map_ns != NULL) {
            m_map_ns = tex->get(mtl->map_ns);
            if (m_map_ns == NULL)
                return (false);
        }
        else {
            m_map_ns = NULL;
        }
        m_ka.x = mtl->ka.x;
        m_ka.y = mtl->ka.y;
        m_ka.z = mtl->ka.z;
        m_ka.w = 0.0f;
        m_kd.x = mtl->kd.x;
        m_kd.y = mtl->kd.y;
        m_kd.z = mtl->kd.z;
        m_kd.w = mtl->d;
        m_ks.x = mtl->ks.x;
        m_ks.y = mtl->ks.y;
        m_ks.z = mtl->ks.z;
        m_ks.w = mtl->ns;
        m_tf.x = mtl->tf.x;
        m_tf.y = mtl->tf.y;
        m_tf.z = mtl->tf.z;
        m_tf.w = 1.0f;
#endif
        return (true);
    }
};

}   /* namespace */

#endif  /* __CRH3D9_MTL_WF_HPP__ */
