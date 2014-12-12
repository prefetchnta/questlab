/*
***************************************
*   Asylum3D @ 2014-12-10
***************************************
*/

#ifndef __CRH3D9_MAIN_HPP__
#define __CRH3D9_MAIN_HPP__

/* Asylum Namespace */
namespace asy {

/***************/
/* Main Object */
/***************/
class crh3d9_main : public asylum
{
private:
    sD3D9_MAIN*         m_main;
    sD3D9_TRAN*         m_tran;
    const sD3D9_CALL*   m_call;

public:
    /* ========================================================================================================== */
    bool init (const sDX9_HDLE* hdle, fp32_t fovy = 60.0f, bool_t full = FALSE, uint_t width = 0, uint_t height = 0,
               D3DFORMAT format = D3DFMT_UNKNOWN, D3DFORMAT depth = D3DFMT_D24X8, bool_t vsync = TRUE,
               D3DMULTISAMPLE_TYPE fsaa = D3DMULTISAMPLE_NONE)
    {
        RECT    rect;

        m_call = hdle->call;
        if (!full) {
            if (!GetClientRect(hwnd, &rect))
                return (false);
            width  = rect.right;
            height = rect.bottom;
        }
        m_main = m_call->create_main(hdle->hwnd, full, width, height, format, depth, vsync, fsaa);
        if (m_main == NULL)
            return (false);
        m_tran = m_call->create_tran(fovy, width, height);
        if (m_tran == NULL) {
            m_call->release_main(m_main);
            return (false);
        }
#if defined(ASY_USE_FIXED_3D)
        d3d9_tran_set_proj(m_main, FIXED_REG, m_tran);
#endif
        d3d9_tran_set_port(m_main, m_tran);
        return (true);
    }

    /* ====== */
    void free ()
    {
        m_call->release_tran(m_tran);
        m_call->release_main(m_main);
    }

public:
    /* ======================================================================================================================= */
    bool reset (bool_t full = FALSE, uint_t width = 0, uint_t height = 0, D3DFORMAT format = D3DFMT_UNKNOWN, bool_t vsync = TRUE,
                D3DMULTISAMPLE_TYPE fsaa = D3DMULTISAMPLE_NONE) const
    {
        if (!full) {
            if (!GetClientRect(m_main->parm.hDeviceWindow, &rect))
                return (false);
            width  = rect.right;
            height = rect.bottom;
        }
        if (!m_call->main_reset(m_main, full, width, height, format, m_main->parm.AutoDepthStencilFormat, vsync, fsaa))
            return (false);
        m_tran->view_port.Width  = width;
        m_tran->view_port.Height = height;
        m_call->tran_upd_port(m_tran);
#if defined(ASY_USE_FIXED_3D)
        d3d9_tran_set_proj(m_main, FIXED_REG, m_tran);
#endif
        d3d9_tran_set_port(m_main, m_tran);
        return (true);
    }

    /* ====================================================================================================================================== */
    void draw_enter (int32u flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, cl32_t color = 0xFF000000, fp32_t valz = 1.0f, int32u stencil = 0) const
    {
        d3d9_do_enter(m_main, flags, color, valz, stencil);
    }

    /* ================== */
    void draw_leave () const
    {
        d3d9_do_leave(m_main);
    }

    /* ================= */
    void draw_show () const
    {
        d3d9_do_flip(m_main);
    }

    /* ============================ */
    void apply_mwvp (uint_t reg) const
    {
        m_call->tran_upd_view(m_tran);
#if defined(ASY_USE_FIXED_3D)
        CR_NOUSE(reg);
        d3d9_tran_set_world(m_main, FIXED_REG, m_tran);
        d3d9_tran_set_view(m_main, FIXED_REG, m_tran);
#else
        m_call->tran_upd_wrld(m_tran);
        m_call->tran_upd_mwvp(m_tran);
        d3d9_tran_set_mwvp(m_main, reg, m_tran);
#endif
    }

    /* ============================================= */
    void apply_matw (uint_t reg, bool vsh = true) const
    {
#if defined(ASY_USE_FIXED_3D)
        CR_NOUSE(reg);  CR_NOUSE(vsh);
        d3d9_tran_set_world(m_main, FIXED_REG, m_tran);
#else
        if (vsh) d3d9_vs_set_vec4f(m_main, reg, (fp32_t*)(&m_tran->tworld), 4);
        else     d3d9_ps_set_vec4f(m_main, reg, (fp32_t*)(&m_tran->tworld), 4);
#endif
    }

    /* ============================================= */
    void apply_matv (uint_t reg, bool vsh = true) const
    {
#if defined(ASY_USE_FIXED_3D)
        CR_NOUSE(reg);  CR_NOUSE(vsh);
        d3d9_tran_set_view(m_main, FIXED_REG, m_tran);
#else
        if (vsh) d3d9_vs_set_vec4f(m_main, reg, (fp32_t*)(&m_tran->tview), 4);
        else     d3d9_ps_set_vec4f(m_main, reg, (fp32_t*)(&m_tran->tview), 4);
#endif
    }

    /* ============================================= */
    void apply_matp (uint_t reg, bool vsh = true) const
    {
#if defined(ASY_USE_FIXED_3D)
        CR_NOUSE(reg);  CR_NOUSE(vsh);
        d3d9_tran_set_proj(m_main, FIXED_REG, m_tran);
#else
        if (vsh) d3d9_vs_set_vec4f(m_main, reg, (fp32_t*)(&m_tran->tproj), 4);
        else     d3d9_ps_set_vec4f(m_main, reg, (fp32_t*)(&m_tran->tproj), 4);
#endif
    }

    /* ============================================= */
    void apply_veye (uint_t reg, bool vsh = true) const
    {
        if (vsh) d3d9_vs_set_vec4f(m_main, reg, (fp32_t*)(&m_tran->eye), 1);
        else     d3d9_ps_set_vec4f(m_main, reg, (fp32_t*)(&m_tran->eye), 1);
    }

    /* ============================================= */
    void apply_vvec (uint_t reg, bool vsh = true) const
    {
        if (vsh) d3d9_vs_set_vec4f(m_main, reg, (fp32_t*)(&m_tran->vvec), 1);
        else     d3d9_ps_set_vec4f(m_main, reg, (fp32_t*)(&m_tran->vvec), 1);
    }

public:
    /* ================================= */
    void set_camera (sCAMERA* camera) const
    {
        camera_init(camera, &m_tran->eye, &m_tran->lookat);
    }

    /* ========================================================= */
    void get_frustum (sFRUSTUM* frustum, fp32_t bias = -1.0f) const
    {
        m_call->tran_frustum(m_tran, frustum, bias);
    }

    /* ============================================================= */
    void get_pickup (sRADIAL* pickup, sint_t scn_x, sint_t scn_y) const
    {
        m_call->tran_pickup(m_tran, pickup, scn_x, scn_y);
    }

public:
    /* ====================== */
    void set_billboardv () const
    {
        m_call->tran_billboardv(m_tran);
    }

    /* ====================== */
    void set_billboardh () const
    {
        m_call->tran_billboardh(m_tran);
    }

    /* =============== */
    void set_mat () const
    {
        m_call->tran_wrld_clear(m_tran);
    }

    /* ============================== */
    void set_mat (const void* mat) const
    {
        mem_cpy(&m_tran->world, mat, sizeof(mat4x4_t));
    }

    /* ================================================= */
    void mul_mat (const void* mats, uint_t count = 1) const
    {
        m_call->tran_wrld_rtmul(m_tran, (const mat4x4_t*)mats, count);
    }

    /* ============================== */
    template<class T>T* get_mat () const
    {
        return ((T*)(&m_tran->world));
    }

    /* ======================= */
    sD3D9_MAIN* get_main () const
    {
        return (m_main);
    }

    /* ======================= */
    sD3D9_TRAN* get_tran () const
    {
        return (m_tran);
    }

    /* ============================= */
    const sD3D9_CALL* get_call () const
    {
        return (m_call);
    }
};

}   /* namespace */

#endif  /* __CRH3D9_MAIN_HPP__ */
