/*
***************************************
*   CPU STUFF @ 2014-04-12
***************************************
*/

#ifndef __DOONCE_HPP__
#define __DOONCE_HPP__

/* Asylum Namespace */
namespace asy {

/*************/
/* Call Once */
/*************/
template<class TLCK = mtlock>
class doonce : public asylum
{
private:
    TLCK            m_lock;
    volatile bool   m_done;

public:
    /* ====== */
    void init ()
    {
        m_lock.init();
        m_done = false;
    }

    /* ====== */
    void free ()
    {
        m_lock.free();
    }

public:
    /* ========================== */
    template<class TSUB>void call ()
    {
        if (!m_done) {
            m_lock.acquire();
            if (!m_done)
            {
                TSUB    sub;

                sub.gosub();
                m_done = true;
            }
            m_lock.release();
        }
    }
};

}   /* namespace */

#endif  /* __DOONCE_HPP__ */
