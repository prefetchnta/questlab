/*
***************************************
*   CPU STUFF @ 2014-03-21
***************************************
*/

#ifndef __MTLOCK_HPP__
#define __MTLOCK_HPP__

/* Asylum Namespace */
namespace asy {

/*********************/
/* Multi-Thread Lock */
/*********************/
class mtlock : public asylum
{
private:
    volatile int    m_lock;

public:
    /* ====== */
    void init ()
    {
        m_lock = 0;
    }

public:
    /* ============================ */
    void acquire (unsigned int ms = 0)
    {
        while (atom_exchange32(&m_lock, 1))
            thread_yield(ms);
    }

    /* ========= */
    void release ()
    {
        atom_set_zero32(&m_lock);
    }
};

}   /* namespace */

#endif  /* __MTLOCK_HPP__ */
