/*
***************************************
*   CPU STUFF @ 2014-03-21
***************************************
*/

#ifndef __SPLOCK_HPP__
#define __SPLOCK_HPP__

/* Asylum Namespace */
namespace asy {

#ifndef ASY_SPLOCK_INIT
    #define ASY_SPLOCK_INIT
#endif
#ifndef ASY_SPLOCK_LOOP
    #define ASY_SPLOCK_LOOP
#endif

/*************/
/* Spin Lock */
/*************/
class splock : public asylum
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
    /* ========= */
    void acquire ()
    {
        ASY_SPLOCK_INIT
        while (atom_exchange32(&m_lock, 1)) {
            ASY_SPLOCK_LOOP
        }
    }

    /* ========= */
    void release ()
    {
        atom_set_zero32(&m_lock);
    }
};

}   /* namespace */

#endif  /* __SPLOCK_HPP__ */
