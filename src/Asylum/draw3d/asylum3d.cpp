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

/******************/
/* Effect Profile */
/******************/
class asy3d_ffct_profile : public IEffect
{
private:
    fp32_t*     m_delta;
    xtime_t     m_timer;

public:
    /* =========================================== */
    asy3d_ffct_profile (xtime_t timer, fp32_t* delta)
    {
        m_delta = delta;
        m_timer = timer;
    }

    /* ======================== */
    virtual ~asy3d_ffct_profile ()
    {
        timer_del(m_timer);
    }

public:
    /* =============== */
    virtual void enter ()
    {
        timer_set_base(m_timer);
    }

    /* =============== */
    virtual void leave ()
    {
        *m_delta = timer_get_delta(m_timer);
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

/* ======================================================== */
CR_API asy::IEffect* create_asy3d_ffct_profile (fp32_t* delta)
{
    xtime_t         xtim;
    asy::IEffect*   ffct;

    if (delta == NULL) {
        ffct = new asy::asy3d_ffct_dummy ();
    }
    else {
        xtim = timer_new();
        if (xtim == NULL)
            return (NULL);
        ffct = new asy::asy3d_ffct_profile (xtim, delta);
        if (ffct == NULL)
            timer_del(xtim);
    }
    return (ffct);
}

/* ========================================================================== */
CR_API asy::IEffect* create_asy3d_ffct_array (asy::IEffect** list, size_t count)
{
    asy::IEffect*   ffct;

    ffct = new asy::asy3d_ffct_array (list, count);
    return (ffct);
}
