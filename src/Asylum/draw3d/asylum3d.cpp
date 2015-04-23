/*
***************************************
*   Asylum3D @ 2015-01-09
***************************************
*/

#include "../asylum.hpp"

/* Asylum Namespace */
namespace asy {

/****************/
/* Effect Dummy */
/****************/
class asy3d_ffct_dummy : public IEffect
{
public:
    /* ============= */
    asy3d_ffct_dummy ()
    {
    }

    /* ====================== */
    virtual ~asy3d_ffct_dummy ()
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

/****************/
/* Effect Array */
/****************/
class asy3d_ffct_array : public IEffect
{
private:
    size_t      m_cnts;
    IEffect**   m_list;

public:
    /* ========================================= */
    asy3d_ffct_array (IEffect** list, size_t count)
    {
        for (size_t idx = 0; idx < count; idx++)
            list[idx]->add_ref();
        m_list = list;
        m_cnts = count;
    }

    /* ====================== */
    virtual ~asy3d_ffct_array ()
    {
        for (size_t idx = 0; idx < m_cnts; idx++)
            m_list[idx]->free();
    }

public:
    /* =============== */
    virtual void enter ()
    {
        for (size_t idx = 0; idx < m_cnts; idx++)
            m_list[idx]->enter();
    }

    /* =============== */
    virtual void leave ()
    {
        for (size_t idx = m_cnts; idx != 0; idx--)
            m_list[idx - 1]->leave();
    }
};

}   /* namespace */

/* ============================================= */
CR_API asy::IEffect* create_asy3d_ffct_dummy (void)
{
    asy::IEffect*   ffct;

    ffct = new asy::asy3d_ffct_dummy ();
    return (ffct);
}

/* ========================================================================== */
CR_API asy::IEffect* create_asy3d_ffct_array (asy::IEffect** list, size_t count)
{
    asy::IEffect*   ffct;

    ffct = new asy::asy3d_ffct_array (list, count);
    return (ffct);
}
