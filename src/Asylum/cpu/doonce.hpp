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
class doonce : public asylum
{
private:
    volatile int    m_done;

public:
    /* ====== */
    void init ()
    {
        m_done = 0;
    }

    /* ====== */
    void free ()
    {
    }

public:
    /* ========================== */
    template<class TSUB>void call ()
    {
        if (!atom_cmp_and_swap32v(&m_done, 1, 0))
        {
            TSUB    sub;

            sub.gosub();
        }
    }
};

}   /* namespace */

#endif  /* __DOONCE_HPP__ */
