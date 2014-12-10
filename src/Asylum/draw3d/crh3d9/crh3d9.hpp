/*
***************************************
*   Asylum3D @ 2014-12-10
***************************************
*/

#ifndef __CRH3D9_HPP__
#define __CRH3D9_HPP__

/* Asylum Namespace */
namespace asy {

/***************/
/* Main Object */
/***************/
class crh3d9_main : public asylum
{
public:
    sD3D9_MAIN*         m_main;
    sD3D9_TRAN*         m_tran;
    const sD3D9_CALL*   m_call;

public:
    /* ================================================================================================================================= */
    bool init (HWND hwnd, fp32_t fovy = 60.0f, bool_t full = FALSE, uint_t width = 0, uint_t height = 0, D3DFORMAT format = D3DFMT_UNKNOWN,
               D3DFORMAT depth = D3DFMT_D24X8, bool_t vsync = TRUE, D3DMULTISAMPLE_TYPE fsaa = D3DMULTISAMPLE_NONE)
    {
        RECT    rect;

        m_call = d3d9call_get();
        if (!full) {
            if (!GetClientRect(hwnd, &rect))
                return (false);
            width  = rect.right;
            height = rect.bottom;
        }
        m_main = m_call->create_main(hwnd, full, width, height, format, depth, vsync, fsaa);
        if (m_main == NULL)
            return (false);
        m_tran = m_call->create_tran(fovy, width, height);
        if (m_tran == NULL) {
            m_call->release_main(m_main);
            return (false);
        }
        return (true);
    }

    /* ====== */
    void free ()
    {
        m_call->release_tran(m_tran);
        m_call->release_main(m_main);
    }

public:
    /* ================================================================================================================================== */
    bool reset (bool_t full = FALSE, uint_t width = 0, uint_t height = 0, D3DFORMAT format = D3DFMT_UNKNOWN, D3DFORMAT depth = D3DFMT_D24X8,
                bool_t vsync = TRUE, D3DMULTISAMPLE_TYPE fsaa = D3DMULTISAMPLE_NONE) const
    {
        if (!full) {
            if (!GetClientRect(hwnd, &rect))
                return (false);
            width  = rect.right;
            height = rect.bottom;
        }
        if (!m_call->main_reset(m_main, full, width, height, format, depth, vsync, fsaa))
            return (false);
        m_tran->view_port.Width  = width;
        m_tran->view_port.Height = height;
        m_call->tran_upd_port(m_tran);
        d3d9_tran_set_port(m_main, m_tran);
        return (true);
    }

    /* ================================== */
    void bind_camera (sCAMERA *camera) const
    {
        camera_init(camera, &m_tran->eye, &m_tran->lookat);
    }
};

}   /* namespace */

#endif  /* __CRH3D9_HPP__ */
