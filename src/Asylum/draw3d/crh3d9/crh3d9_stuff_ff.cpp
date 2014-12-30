/*
***************************************
*   Asylum3D @ 2014-12-30
***************************************
*/

#include "../../asylum.hpp"

/*****************************************************************************/
/*                                  Effect                                   */
/*****************************************************************************/

/* Asylum Namespace */
namespace asy {

/****************/
/* Light Effect */
/****************/
class crh3d9_ffct_light_fixed : public IEffect
{
private:
    BOOL*               m_onoff;
    DWORD               m_count;
    DWORD               m_spclr;
    D3DCOLOR*           m_color;
    D3DLIGHT9*          m_light;
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ========================================================================================================== */
    crh3d9_ffct_light_fixed (D3DCOLOR* ambient, D3DLIGHT9* light, BOOL* onoff, DWORD count, const crh3d9_main* main)
    {
        m_onoff = onoff;
        m_count = count;
        m_light = light;
        m_color = ambient;
        if (ambient == NULL || light == NULL || onoff == NULL || count == 0)
            m_spclr = TRUE;
        else
            m_spclr = FALSE;
        m_devcs = main->get_main()->dev;
    }

    /* ============================= */
    virtual ~crh3d9_ffct_light_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        if (m_spclr) {
            m_devcs->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
        }
        else {
            m_devcs->SetRenderState(D3DRS_LIGHTING, TRUE);
            m_devcs->SetRenderState(D3DRS_AMBIENT, m_color[0]);
            for (DWORD idx = 0; idx < m_count; idx++) {
                m_devcs->SetLight(idx, &m_light[idx]);
                m_devcs->LightEnable(idx, m_onoff[idx]);
            }
        }
    }

    /* =============== */
    virtual void leave ()
    {
        if (m_spclr)
            m_devcs->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
        else
            m_devcs->SetRenderState(D3DRS_LIGHTING, FALSE);
    }
};

/****************/
/* Alpha Effect */
/****************/
class crh3d9_ffct_alpha_fixed : public IEffect
{
private:
    DWORD               m_value;
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ======================================================== */
    crh3d9_ffct_alpha_fixed (DWORD value, const crh3d9_main* main)
    {
        m_value = value;
        m_devcs = main->get_main()->dev;
    }

    /* ============================= */
    virtual ~crh3d9_ffct_alpha_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        if (m_value < 256) {
            m_devcs->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
            m_devcs->SetRenderState(D3DRS_ALPHAREF, m_value);
            m_devcs->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        }
        else {
            m_devcs->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            m_devcs->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
            m_devcs->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            m_devcs->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        }
    }

    /* =============== */
    virtual void leave ()
    {
        if (m_value < 256)
            m_devcs->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        else
            m_devcs->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    }
};

}   /* namespace */

/* =================================================================================== */
CR_API asy::IEffect* create_crh3d9_ffct_light_fixed (D3DCOLOR* ambient, D3DLIGHT9* light,
                                BOOL* onoff, DWORD count, const asy::crh3d9_main* main)
{
    asy::crh3d9_ffct_light_fixed*   ffct;

    ffct = new asy::crh3d9_ffct_light_fixed (ambient, light, onoff, count, main);
    return ((asy::IEffect*)ffct);
}

/* ========================================================================================= */
CR_API asy::IEffect* create_crh3d9_ffct_alpha_fixed (DWORD value, const asy::crh3d9_main* main)
{
    asy::crh3d9_ffct_alpha_fixed*   ffct;

    ffct = new asy::crh3d9_ffct_alpha_fixed (value, main);
    return ((asy::IEffect*)ffct);
}
