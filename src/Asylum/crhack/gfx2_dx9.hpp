/*
***************************************
*   CrHack C++ Wrapper @ 2014-02-27
***************************************
*/

#ifndef __GFX2_DX9_HPP__
#define __GFX2_DX9_HPP__

#if defined(_CR_HAVE_D3D9_)

/* Asylum Namespace */
namespace asy {

/****************************/
/* iGFX2_DX9M Wrapper (LIB) */
/****************************/
class IGfx2DX9M : public IGfx2
{
public:
    /* ============================================ */
    IGfx2DX9M (iGFX2_DX9M* that) : IGfx2((iGFX2*)that)
    {
    }
    ~IGfx2DX9M ()
    {
    }

public:
    /* ======================== */
    void_t do_enter (void_t) const
    {
        do_dx9_enter(this->get_port<iGFX2_DX9M>());
    }

    /* ======================== */
    void_t do_leave (void_t) const
    {
        do_dx9_leave(this->get_port<iGFX2_DX9M>());
    }

    /* ======================== */
    void_t mode_set (void_t) const
    {
        mode_dx9_set(this->get_port<iGFX2_DX9M>());
    }

    /* ======================== */
    void_t mode_alp (void_t) const
    {
        mode_dx9_alp(this->get_port<iGFX2_DX9M>());
    }

    /* ======================== */
    void_t mode_add (void_t) const
    {
        mode_dx9_add(this->get_port<iGFX2_DX9M>());
    }

    /* ======================== */
    void_t mode_sub (void_t) const
    {
        mode_dx9_sub(this->get_port<iGFX2_DX9M>());
    }

    /* ======================== */
    void_t mode_end (void_t) const
    {
        mode_dx9_end(this->get_port<iGFX2_DX9M>());
    }

    /* ================================================== */
    bool_t fill_draw (const sFILL* fill, cl32_t color) const
    {
        return (fill_dx9_draw(this->get_port<iGFX2_DX9M>(), fill, color));
    }

    /* ====================================================================================== */
    bool_t blit_copy (const iGFX2_DX9S* src, const sBLIT* blit, cl32_t color = 0xFFFFFFFF) const
    {
        return (blit_dx9_copy(this->get_port<iGFX2_DX9M>(), src, blit, color));
    }

    /* ====================================================================================== */
    bool_t blit_zoom (const iGFX2_DX9S* src, const sZOOM* zoom, cl32_t color = 0xFFFFFFFF) const
    {
        return (blit_dx9_zoom(this->get_port<iGFX2_DX9M>(), src, zoom, color));
    }

    /* ======================================================================================================================== */
    bool_t blit_rote (const iGFX2_DX9S* src, const sBLIT* blit, fp32_t cx, fp32_t cy, fp32_t ccw, cl32_t color = 0xFFFFFFFF) const
    {
        return (blit_dx9_rote(this->get_port<iGFX2_DX9M>(), src, blit, cx, cy, ccw, color));
    }

    /* ======================================================================================================================== */
    bool_t blit_full (const iGFX2_DX9S* src, const sZOOM* zoom, fp32_t cx, fp32_t cy, fp32_t ccw, cl32_t color = 0xFFFFFFFF) const
    {
        return (blit_dx9_full(this->get_port<iGFX2_DX9M>(), src, zoom, cx, cy, ccw, color));
    }

    /* ============================================================================================================= */
    bool_t blit_matx (const iGFX2_DX9S* src, const RECT* rect, const D3DXMATRIX* matx, cl32_t color = 0xFFFFFFFF) const
    {
        return (blit_dx9_matx(this->get_port<iGFX2_DX9M>(), src, rect, matx, color));
    }
};

/****************************/
/* iGFX2_DX9M Wrapper (DLL) */
/****************************/
class IGfx2DX9M2 : public IGfx2DX9M
{
private:
    const sDX9_CALL*    m_call;

public:
    /* ================================================================ */
    IGfx2DX9M2 (iGFX2_DX9M* that, const sDX9_CALL* call) : IGfx2DX9M(that)
    {
        m_call = call;
    }
    ~IGfx2DX9M2 ()
    {
    }

public:
    /* ================================================== */
    bool_t fill_draw (const sFILL* fill, cl32_t color) const
    {
        return (m_call->fill_draw(this->get_port<iGFX2_DX9M>(), fill, color));
    }

    /* ====================================================================================== */
    bool_t blit_copy (const iGFX2_DX9S* src, const sBLIT* blit, cl32_t color = 0xFFFFFFFF) const
    {
        return (m_call->blit_copy(this->get_port<iGFX2_DX9M>(), src, blit, color));
    }

    /* ====================================================================================== */
    bool_t blit_zoom (const iGFX2_DX9S* src, const sZOOM* zoom, cl32_t color = 0xFFFFFFFF) const
    {
        return (m_call->blit_zoom(this->get_port<iGFX2_DX9M>(), src, zoom, color));
    }

    /* ======================================================================================================================== */
    bool_t blit_rote (const iGFX2_DX9S* src, const sBLIT* blit, fp32_t cx, fp32_t cy, fp32_t ccw, cl32_t color = 0xFFFFFFFF) const
    {
        return (m_call->blit_rote(this->get_port<iGFX2_DX9M>(), src, blit, cx, cy, ccw, color));
    }

    /* ======================================================================================================================== */
    bool_t blit_full (const iGFX2_DX9S* src, const sZOOM* zoom, fp32_t cx, fp32_t cy, fp32_t ccw, cl32_t color = 0xFFFFFFFF) const
    {
        return (m_call->blit_full(this->get_port<iGFX2_DX9M>(), src, zoom, cx, cy, ccw, color));
    }

    /* ============================================================================================================= */
    bool_t blit_matx (const iGFX2_DX9S* src, const RECT* rect, const D3DXMATRIX* matx, cl32_t color = 0xFFFFFFFF) const
    {
        return (m_call->blit_matx(this->get_port<iGFX2_DX9M>(), src, rect, matx, color));
    }
};

/*********************/
/* iFONT_DX9 Wrapper */
/*********************/
class IFontDX9 : public IFont
{
public:
    /* ============================== */
    IFontDX9 (iFONT* that) : IFont(that)
    {
    }
    ~IFontDX9 ()
    {
    }

public:
    /* ========================== */
    void_t font_losts (void_t) const
    {
        font_dx9_losts(this->get_port<iFONT_DX9>());
    }

    /* ========================== */
    void_t font_reset (void_t) const
    {
        font_dx9_reset(this->get_port<iFONT_DX9>());
    }
};

}   /* namespace */

#endif  /* _CR_HAVE_D3D9_ */

#endif  /* __GFX2_DX9_HPP__ */
