/*
***************************************
*   CrHack C++ Wrapper @ 2014-02-27
***************************************
*/

#ifndef __GFX2_DX8_HPP__
#define __GFX2_DX8_HPP__

#if defined(_CR_HAVE_D3D8_)

/* Asylum Namespace */
namespace asy {

/****************************/
/* iGFX2_DX8M Wrapper (LIB) */
/****************************/
class IGfx2DX8M : public IGfx2
{
public:
    /* ============================================ */
    IGfx2DX8M (iGFX2_DX8M* that) : IGfx2((iGFX2*)that)
    {
    }
    ~IGfx2DX8M ()
    {
    }

public:
    /* ======================== */
    void_t do_enter (void_t) const
    {
        do_dx8_enter(this->get_port<iGFX2_DX8M>());
    }

    /* ======================== */
    void_t do_leave (void_t) const
    {
        do_dx8_leave(this->get_port<iGFX2_DX8M>());
    }

    /* ================================================== */
    bool_t fill_draw (const sFILL* fill, cl32_t color) const
    {
        return (fill_dx8_draw(this->get_port<iGFX2_DX8M>(), fill, color));
    }

    /* ====================================================================================== */
    bool_t blit_copy (const iGFX2_DX8S* src, const sBLIT* blit, cl32_t color = 0xFFFFFFFF) const
    {
        return (blit_dx8_copy(this->get_port<iGFX2_DX8M>(), src, blit, color));
    }

    /* ====================================================================================== */
    bool_t blit_zoom (const iGFX2_DX8S* src, const sZOOM* zoom, cl32_t color = 0xFFFFFFFF) const
    {
        return (blit_dx8_zoom(this->get_port<iGFX2_DX8M>(), src, zoom, color));
    }

    /* ======================================================================================================================== */
    bool_t blit_rote (const iGFX2_DX8S* src, const sBLIT* blit, fp32_t cx, fp32_t cy, fp32_t ccw, cl32_t color = 0xFFFFFFFF) const
    {
        return (blit_dx8_rote(this->get_port<iGFX2_DX8M>(), src, blit, cx, cy, ccw, color));
    }

    /* ======================================================================================================================== */
    bool_t blit_full (const iGFX2_DX8S* src, const sZOOM* zoom, fp32_t cx, fp32_t cy, fp32_t ccw, cl32_t color = 0xFFFFFFFF) const
    {
        return (blit_dx8_full(this->get_port<iGFX2_DX8M>(), src, zoom, cx, cy, ccw, color));
    }

    /* ============================================================================================================= */
    bool_t blit_matx (const iGFX2_DX8S* src, const RECT* rect, const D3DXMATRIX* matx, cl32_t color = 0xFFFFFFFF) const
    {
        return (blit_dx8_matx(this->get_port<iGFX2_DX8M>(), src, rect, matx, color));
    }
};

/****************************/
/* iGFX2_DX8M Wrapper (DLL) */
/****************************/
class IGfx2DX8M2 : public IGfx2DX8M
{
private:
    const sDX8_CALL*    m_call;

public:
    /* ================================================================ */
    IGfx2DX8M2 (iGFX2_DX8M* that, const sDX8_CALL* call) : IGfx2DX8M(that)
    {
        m_call = call;
    }
    ~IGfx2DX8M2 ()
    {
    }

public:
    /* ================================================== */
    bool_t fill_draw (const sFILL* fill, cl32_t color) const
    {
        return (m_call->fill_draw(this->get_port<iGFX2_DX8M>(), fill, color));
    }

    /* ====================================================================================== */
    bool_t blit_copy (const iGFX2_DX8S* src, const sBLIT* blit, cl32_t color = 0xFFFFFFFF) const
    {
        return (m_call->blit_copy(this->get_port<iGFX2_DX8M>(), src, blit, color));
    }

    /* ====================================================================================== */
    bool_t blit_zoom (const iGFX2_DX8S* src, const sZOOM* zoom, cl32_t color = 0xFFFFFFFF) const
    {
        return (m_call->blit_zoom(this->get_port<iGFX2_DX8M>(), src, zoom, color));
    }

    /* ======================================================================================================================== */
    bool_t blit_rote (const iGFX2_DX8S* src, const sBLIT* blit, fp32_t cx, fp32_t cy, fp32_t ccw, cl32_t color = 0xFFFFFFFF) const
    {
        return (m_call->blit_rote(this->get_port<iGFX2_DX8M>(), src, blit, cx, cy, ccw, color));
    }

    /* ======================================================================================================================== */
    bool_t blit_full (const iGFX2_DX8S* src, const sZOOM* zoom, fp32_t cx, fp32_t cy, fp32_t ccw, cl32_t color = 0xFFFFFFFF) const
    {
        return (m_call->blit_full(this->get_port<iGFX2_DX8M>(), src, zoom, cx, cy, ccw, color));
    }

    /* ============================================================================================================= */
    bool_t blit_matx (const iGFX2_DX8S* src, const RECT* rect, const D3DXMATRIX* matx, cl32_t color = 0xFFFFFFFF) const
    {
        return (m_call->blit_matx(this->get_port<iGFX2_DX8M>(), src, rect, matx, color));
    }
};

}   /* namespace */

#endif  /* _CR_HAVE_D3D8_ */

#endif  /* __GFX2_DX8_HPP__ */
