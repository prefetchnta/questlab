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
    size_t      m_count;
    D3DCOLOR    m_color;
    D3DLIGHT9** m_light;

public:
    /* =============================================================== */
    crh3d9_eff_wf_fixed (cl32_t ambient, D3DLIGHT9** light, size_t count)
    {
    }

    /* ========================= */
    virtual ~crh3d9_eff_wf_fixed ()
    {
    }

public:
    /* =============== */
    virtual void enter ()
    {
    }

    /* =============== */
    virtual void leave ()
    {
    }
};

}   /* namespace */

/* ============================================================================================ */
CR_API asy::effect_i* create_crh3d9_eff_wf_fixed (cl32_t ambient, D3DLIGHT9** light, size_t count)
{
    asy::crh3d9_eff_wf_fixed*   effect;

    effect = new asy::crh3d9_eff_wf_fixed (ambient, light, count);
    return ((effect_i*)effect);
}
