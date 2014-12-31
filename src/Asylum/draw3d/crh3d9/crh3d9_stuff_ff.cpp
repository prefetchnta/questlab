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
class crh3d9_ffct_light_d_fixed : public IEffect
{
private:
    BOOL*               m_onoff;
    DWORD               m_count;
    D3DCOLOR*           m_color;
    D3DLIGHT9*          m_light;
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ============================================================================================================ */
    crh3d9_ffct_light_d_fixed (D3DCOLOR* ambient, D3DLIGHT9* light, BOOL* onoff, DWORD count, const crh3d9_main* main)
    {
        m_onoff = onoff;
        m_count = count;
        m_light = light;
        m_color = ambient;
        m_devcs = main->get_main()->dev;
    }

    /* =============================== */
    virtual ~crh3d9_ffct_light_d_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        m_devcs->SetRenderState(D3DRS_LIGHTING, TRUE);
        m_devcs->SetRenderState(D3DRS_AMBIENT, m_color[0]);
        for (DWORD idx = 0; idx < m_count; idx++) {
            m_devcs->SetLight(idx, &m_light[idx]);
            m_devcs->LightEnable(idx, m_onoff[idx]);
        }
    }

    /* =============== */
    virtual void leave ()
    {
        m_devcs->SetRenderState(D3DRS_LIGHTING, FALSE);
    }
};

/*******************/
/* Specular Effect */
/*******************/
class crh3d9_ffct_light_s_fixed : public IEffect
{
private:
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ============================================= */
    crh3d9_ffct_light_s_fixed (const crh3d9_main* main)
    {
        m_devcs = main->get_main()->dev;
    }

    /* =============================== */
    virtual ~crh3d9_ffct_light_s_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        m_devcs->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
    }

    /* =============== */
    virtual void leave ()
    {
        m_devcs->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
    }
};

/**********************/
/* Alpha Blend Effect */
/**********************/
class crh3d9_ffct_alpha_fixed : public IEffect
{
private:
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* =========================================== */
    crh3d9_ffct_alpha_fixed (const crh3d9_main* main)
    {
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
        m_devcs->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_devcs->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        m_devcs->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_devcs->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }

    /* =============== */
    virtual void leave ()
    {
        m_devcs->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    }
};

/*********************/
/* Alpha Test Effect */
/*********************/
class crh3d9_ffct_atest_fixed : public IEffect
{
private:
    DWORD               m_value;
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ======================================================== */
    crh3d9_ffct_atest_fixed (DWORD value, const crh3d9_main* main)
    {
        m_value = value;
        m_devcs = main->get_main()->dev;
    }

    /* ============================= */
    virtual ~crh3d9_ffct_atest_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        m_devcs->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        m_devcs->SetRenderState(D3DRS_ALPHAREF, m_value);
        m_devcs->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    }

    /* =============== */
    virtual void leave ()
    {
        m_devcs->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    }
};

}   /* namespace */

/* =================================================================================== */
CR_API asy::IEffect* create_crh3d9_ffct_light_fixed (D3DCOLOR* ambient, D3DLIGHT9* light,
                                BOOL* onoff, DWORD count, const asy::crh3d9_main* main)
{
    asy::IEffect*   ffct;

    if (ambient == NULL || light == NULL || onoff == NULL || count == 0)
        ffct = new asy::crh3d9_ffct_light_s_fixed (main);
    else
        ffct = new asy::crh3d9_ffct_light_d_fixed (ambient, light, onoff, count, main);
    return (ffct);
}

/* ========================================================================================= */
CR_API asy::IEffect* create_crh3d9_ffct_alpha_fixed (DWORD value, const asy::crh3d9_main* main)
{
    asy::IEffect*   ffct;

    if (value < 256)
        ffct = new asy::crh3d9_ffct_atest_fixed (value, main);
    else
        ffct = new asy::crh3d9_ffct_alpha_fixed (main);
    return (ffct);
}
