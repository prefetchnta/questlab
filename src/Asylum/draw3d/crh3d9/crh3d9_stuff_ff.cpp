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

/*********************/
/* Render State List */
/*********************/
class crh3d9_ffct_rs_fixed : public IEffect
{
private:
    uint_t              m_enter;
    uint_t              m_leave;
    const int32u*       m_value;
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ====================================================================================== */
    crh3d9_ffct_rs_fixed (const int32u* list, uint_t ninp, uint_t nout, const crh3d9_main* main)
    {
        m_value = list;
        m_enter = ninp * 2;
        m_leave = nout * 2;
        m_devcs = main->get_main()->dev;
    }

    /* ========================== */
    virtual ~crh3d9_ffct_rs_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        for (uint_t idx = 0; idx < m_enter; idx += 2)
            m_devcs->SetRenderState((D3DRENDERSTATETYPE)m_value[idx], m_value[idx + 1]);
    }

    /* =============== */
    virtual void leave ()
    {
        for (uint_t idx = m_enter; idx < m_leave; idx += 2)
            m_devcs->SetRenderState((D3DRENDERSTATETYPE)m_value[idx], m_value[idx + 1]);
    }
};

/**********************/
/* Sampler State List */
/**********************/
class crh3d9_ffct_ss_fixed : public IEffect
{
private:
    uint_t              m_enter;
    uint_t              m_leave;
    const int32u*       m_value;
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ====================================================================================== */
    crh3d9_ffct_ss_fixed (const int32u* list, uint_t ninp, uint_t nout, const crh3d9_main* main)
    {
        m_value = list;
        m_enter = ninp * 3;
        m_leave = nout * 3;
        m_devcs = main->get_main()->dev;
    }

    /* ========================== */
    virtual ~crh3d9_ffct_ss_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        for (uint_t idx = 0; idx < m_enter; idx += 3)
            m_devcs->SetSamplerState(m_value[idx], (D3DSAMPLERSTATETYPE)m_value[idx + 1], m_value[idx + 2]);
    }

    /* =============== */
    virtual void leave ()
    {
        for (uint_t idx = m_enter; idx < m_leave; idx += 3)
            m_devcs->SetSamplerState(m_value[idx], (D3DSAMPLERSTATETYPE)m_value[idx + 1], m_value[idx + 2]);
    }
};

/****************************/
/* Texture Stage State List */
/****************************/
class crh3d9_ffct_tss_fixed : public IEffect
{
private:
    uint_t              m_enter;
    uint_t              m_leave;
    const int32u*       m_value;
    LPDIRECT3DDEVICE9   m_devcs;

public:
    /* ======================================================================================= */
    crh3d9_ffct_tss_fixed (const int32u* list, uint_t ninp, uint_t nout, const crh3d9_main* main)
    {
        m_value = list;
        m_enter = ninp * 3;
        m_leave = nout * 3;
        m_devcs = main->get_main()->dev;
    }

    /* =========================== */
    virtual ~crh3d9_ffct_tss_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
        for (uint_t idx = 0; idx < m_enter; idx += 3)
            m_devcs->SetTextureStageState(m_value[idx], (D3DTEXTURESTAGESTATETYPE)m_value[idx + 1], m_value[idx + 2]);
    }

    /* =============== */
    virtual void leave ()
    {
        for (uint_t idx = m_enter; idx < m_leave; idx += 3)
            m_devcs->SetTextureStageState(m_value[idx], (D3DTEXTURESTAGESTATETYPE)m_value[idx + 1], m_value[idx + 2]);
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

/* ============================================================================================ */
CR_API asy::IEffect* create_crh3d9_ffct_state_fixed (const int32u* list, uint_t ninp, uint_t nout,
                                                     uint_t type, const asy::crh3d9_main* main)
{
    asy::IEffect*   ffct;

    switch (type)
    {
        default:
            return (NULL);

        case EFFECT_STT_RS:
            ffct = new asy::crh3d9_ffct_rs_fixed (list, ninp, nout, main);
            break;

        case EFFECT_STT_SS:
            ffct = new asy::crh3d9_ffct_ss_fixed (list, ninp, nout, main);
            break;

        case EFFECT_STT_TSS:
            ffct = new asy::crh3d9_ffct_tss_fixed (list, ninp, nout, main);
            break;
    }
    return (ffct);
}

/*****************************************************************************/
/*                                  Commit                                   */
/*****************************************************************************/

/* Asylum Namespace */
namespace asy {

/*************************/
/* Object Commit (Fixed) */
/*************************/
class crh3d9_cmmt_fixed : public asylum
{
public:
    /* ================================= */
    bool doit (void* ctx, commit_pipe* obj)
    {
        IMesh*              mesh;
        size_t              size;
        commit_unit*        list;
        LPDIRECT3DDEVICE9   devs;

        obj->effect->enter();
        size = obj->stuffz.size();
        list = obj->stuffz.data();
        devs = (LPDIRECT3DDEVICE9)ctx;
        for (size_t idx = 0; idx < size; idx++) {
            devs->SetTransform(D3DTS_WORLD, (D3DMATRIX*)(&list->inst->tran));
            if (list->unit->attr != NULL)
                list->unit->attr->commit();
            if (list->unit->mesh != NULL) {
                for (size_t ii = 0; ; ii++) {
                    mesh = list->unit->mesh[ii];
                    if (mesh == NULL)
                        break;
                    mesh->commit();
                }
            }
            list += 1;
        }
        return (true);
    }

    /* ================================= */
    void back (void* ctx, commit_pipe* obj)
    {
        CR_NOUSE(ctx);
        obj->effect->leave();
    }
};

}   /* namespace */

/* ============================================================================ */
void crhack3d9_commit_fixed (void* ctx, const asy::tree_l<asy::commit_pipe>* pipe)
{
    pipe->trav_dfs<asy::crh3d9_cmmt_fixed>(ctx);
}
