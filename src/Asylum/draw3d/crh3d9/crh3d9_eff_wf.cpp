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
class crh3d9_eff_wf_fixed : public effect_i
{
private:
    BOOL                m_specr;
    BOOL*               m_onoff;
    DWORD               m_count;
    D3DCOLOR            m_color;
    D3DLIGHT9**         m_light;
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ==================================================================================================================== */
    crh3d9_eff_wf_fixed (cl32_t ambient, D3DLIGHT9** light, BOOL* on_off, DWORD count, BOOL specular, LPDIRECT3DDEVICE9 devcs)
    {
        m_specr = specular;
        m_onoff = on_off;
        m_count = count;
        m_color = (D3DCOLOR)ambient;
        m_light = light;
        m_devcs = devcs;
    }

    /* ========================= */
    virtual ~crh3d9_eff_wf_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        m_devcs->SetRenderState(D3DRS_AMBIENT, m_color);
        m_devcs->SetRenderState(D3DRS_COLORVERTEX, FALSE);
        m_devcs->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
        m_devcs->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
        m_devcs->SetRenderState(D3DRS_LIGHTING, TRUE);
        m_devcs->SetRenderState(D3DRS_SPECULARENABLE, m_specr);
        for (DWORD idx = 0; idx < m_count; idx++) {
            m_devcs->SetLight(idx, m_light[idx]);
            m_devcs->LightEnable(idx, m_onoff[idx]);
        }
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

/* ============================================================================== */
CR_API asy::effect_i* create_crh3d9_eff_wf_fixed (cl32_t ambient, D3DLIGHT9** light,
            BOOL* on_off, DWORD count, BOOL specular, const asy::crh3d9_main* main)
{
    asy::crh3d9_eff_wf_fixed*   ffct;

    ffct = new asy::crh3d9_eff_wf_fixed (ambient, light, on_off, count, specular, main->get_main()->dev);
    return ((effect_i*)ffct);
}
