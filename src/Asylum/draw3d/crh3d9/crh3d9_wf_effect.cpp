/*
***************************************
*   Asylum3D @ 2014-12-23
***************************************
*/

#include "../../asylum.hpp"

/* Asylum Namespace */
namespace asy {

/****************************/
/* Wavefront Effect (Fixed) */
/****************************/
class crh3d9_ffct_wf_fixed : public IEffect
{
private:
    BOOL*               m_onoff;
    DWORD               m_count;
    D3DCOLOR*           m_color;
    D3DLIGHT9*          m_light;
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ======================================================================================================= */
    crh3d9_ffct_wf_fixed (D3DCOLOR* ambient, D3DLIGHT9* light, BOOL* onoff, DWORD count, LPDIRECT3DDEVICE9 devcs)
    {
        m_onoff = onoff;
        m_count = count;
        m_color = ambient;
        m_light = light;
        m_devcs = devcs;
    }

    /* ========================== */
    virtual ~crh3d9_ffct_wf_fixed ()
    {
    }

public:
    /* ========================== */
    virtual void enter (int64u type)
    {
        uint_t  fvf = D3DFVF_XYZ;

        if (m_color != NULL)
            m_devcs->SetRenderState(D3DRS_AMBIENT, m_color[0]);
        m_devcs->SetRenderState(D3DRS_COLORVERTEX, FALSE);
        m_devcs->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
        m_devcs->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
        if (type & ATTR_TYPE_TEXTURE)
            fvf |= D3DFVF_TEX1;
        if (type & ATTR_TYPE_NORMAL) {
            fvf |= D3DFVF_NORMAL;
            if (m_light != NULL && m_onoff != NULL) {
                m_devcs->SetRenderState(D3DRS_LIGHTING, TRUE);
                if (type & ATTR_TYPE_SPECULAR)
                    m_devcs->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
                for (DWORD idx = 0; idx < m_count; idx++) {
                    m_devcs->SetLight(idx, &m_light[idx]);
                    m_devcs->LightEnable(idx, m_onoff[idx]);
                }
            }
        }
        m_devcs->SetFVF(fvf);
    }

    /* =============== */
    virtual void leave ()
    {
        m_devcs->SetRenderState(D3DRS_COLORVERTEX, TRUE);
        m_devcs->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
        m_devcs->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2);
        m_devcs->SetRenderState(D3DRS_LIGHTING, FALSE);
        m_devcs->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
    }
};

}   /* namespace */

/* ================================================================================ */
CR_API asy::IEffect* create_crh3d9_ffct_wf_fixed (D3DCOLOR* ambient, D3DLIGHT9* light,
                        BOOL* onoff, DWORD count, const asy::crh3d9_main* main)
{
    asy::crh3d9_ffct_wf_fixed*  ffct;

    ffct = new asy::crh3d9_ffct_wf_fixed (ambient, light, onoff, count, main->get_main()->dev);
    return ((asy::IEffect*)ffct);
}
