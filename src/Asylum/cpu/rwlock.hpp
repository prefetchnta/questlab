/*
***************************************
*   CPU STUFF @ 2014-03-27
***************************************
*/

#ifndef __RWLOCK_HPP__
#define __RWLOCK_HPP__

/* Asylum Namespace */
namespace asy {

/*******************/
/* Read Write Lock */
/*******************/
class rwlock : public asylum
{
private:
    volatile int    m_wlock;
    volatile int    m_rlock;

public:
    /* ====== */
    void init ()
    {
        m_wlock = 0;
        m_rlock = 0;
    }

public:
    /* =========== */
    void r_acquire ()
    {
        for (;;) {
            while (m_wlock)
                atom_rw_barrier();
            atom_inc_fetch32(&m_rlock);
            if (m_wlock)
                atom_dec_fetch32(&m_rlock);
            else
                break;
        }
    }

    /* =========== */
    void r_release ()
    {
        atom_dec_fetch32(&m_rlock);
    }

    /* =========== */
    void w_acquire ()
    {
        while (atom_exchange32(&m_wlock, 1));
        while (m_rlock)
            atom_rw_barrier();
    }

    /* =========== */
    void w_release ()
    {
        atom_set_zero32(&m_wlock);
    }
};

}   /* namespace */

#endif  /* __RWLOCK_HPP__ */
