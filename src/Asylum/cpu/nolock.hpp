/*
***************************************
*   CPU STUFF @ 2014-12-12
***************************************
*/

#ifndef __NOLOCK_HPP__
#define __NOLOCK_HPP__

/* Asylum Namespace */
namespace asy {

/***********/
/* No Lock */
/***********/
class nolock : public asylum
{
public:
    /* ====== */
    void init ()
    {
    }

public:
    /* ========= */
    void acquire ()
    {
    }

    /* ========= */
    void release ()
    {
    }
};

}   /* namespace */

#endif  /* __NOLOCK_HPP__ */
