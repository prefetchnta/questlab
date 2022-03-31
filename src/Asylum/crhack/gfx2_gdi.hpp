/*
***************************************
*   CrHack C++ Wrapper @ 2014-02-27
***************************************
*/

#ifndef __GFX2_GDI_HPP__
#define __GFX2_GDI_HPP__

/* Asylum Namespace */
namespace asy {

/***************************/
/* iGFX2_GDI Wrapper (LIB) */
/***************************/
class IGfx2GDI : public IGfx2
{
public:
    /* ========================================== */
    IGfx2GDI (iGFX2_GDI* that) : IGfx2((iGFX2*)that)
    {
    }
    ~IGfx2GDI ()
    {
    }

public:
    /* ================================================== */
    bool_t fill_draw (const sFILL* fill, cl32_t color) const
    {
        return (fill_gdi_draw(this->get_port<iGFX2_GDI>(), fill, color));
    }

    /* ========================================================== */
    bool_t blit_copy (const iGFX2_GDI* src, const sBLIT* blit) const
    {
        return (blit_gdi_copy(this->get_port<iGFX2_GDI>(), src, blit));
    }

    /* ========================================================== */
    bool_t blit_zoom (const iGFX2_GDI* src, const sZOOM* zoom) const
    {
        return (blit_gdi_zoom(this->get_port<iGFX2_GDI>(), src, zoom));
    }

    /* ======================================================================== */
    bool_t blit_tran (const iGFX2_GDI* src, const sBLIT* blit, cl32_t trans) const
    {
        return (blit_gdi_tran(this->get_port<iGFX2_GDI>(), src, blit, trans));
    }

    /* =========================================================== */
    bool_t blit_blend (const iGFX2_GDI* src, const sBLIT* blit) const
    {
        return (blit_gdi_blend(this->get_port<iGFX2_GDI>(), src, blit));
    }

    /* ========================================================================= */
    bool_t blit_alpha (const iGFX2_GDI* src, const sBLIT* blit, uint_t alpha) const
    {
        return (blit_gdi_alpha(this->get_port<iGFX2_GDI>(), src, blit, alpha));
    }
};

/***************************/
/* iGFX2_GDI Wrapper (DLL) */
/***************************/
class IGfx2GDI2 : public IGfx2GDI
{
private:
    const sGDI_CALL*    m_call;

public:
    /* ============================================================= */
    IGfx2GDI2 (iGFX2_GDI* that, const sGDI_CALL* call) : IGfx2GDI(that)
    {
        m_call = call;
    }
    ~IGfx2GDI2 ()
    {
    }

public:
    /* ================================================== */
    bool_t fill_draw (const sFILL* fill, cl32_t color) const
    {
        return (m_call->fill_draw(this->get_port<iGFX2_GDI>(), fill, color));
    }

    /* ========================================================== */
    bool_t blit_copy (const iGFX2_GDI* src, const sBLIT* blit) const
    {
        return (m_call->blit_copy(this->get_port<iGFX2_GDI>(), src, blit));
    }

    /* ========================================================== */
    bool_t blit_zoom (const iGFX2_GDI* src, const sZOOM* zoom) const
    {
        return (m_call->blit_zoom(this->get_port<iGFX2_GDI>(), src, zoom));
    }

    /* ======================================================================== */
    bool_t blit_tran (const iGFX2_GDI* src, const sBLIT* blit, cl32_t trans) const
    {
        return (m_call->blit_tran(this->get_port<iGFX2_GDI>(), src, blit, trans));
    }

    /* =========================================================== */
    bool_t blit_blend (const iGFX2_GDI* src, const sBLIT* blit) const
    {
        return (m_call->blit_blend(this->get_port<iGFX2_GDI>(), src, blit));
    }

    /* ========================================================================= */
    bool_t blit_alpha (const iGFX2_GDI* src, const sBLIT* blit, uint_t alpha) const
    {
        return (m_call->blit_alpha(this->get_port<iGFX2_GDI>(), src, blit, alpha));
    }
};

}   /* namespace */

#endif  /* __GFX2_GDI_HPP__ */
