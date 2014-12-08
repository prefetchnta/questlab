/*
***************************************
*   BASIC STUFF @ 2014-04-28
***************************************
*/

#ifndef __BASIC_HPP__
#define __BASIC_HPP__

/* Asylum Namespace */
namespace asy {

/*******************/
/* Common Node Ptr */
/*******************/
struct cnode_ptr
{
    void*   ptr;

    /* ========= */
    void free () {}
};

/************************/
/* Asylum Common Object */
/************************/
class asylum
{
public:
    /* ====== */
    void free ()
    {
    }

public:
    /* ======================= */
    void* operator new (size_t n)
    {
        if (n == 0) n = 1;
        return (mem_malloc(n));
    }

    /* ========================== */
    void* operator new [] (size_t n)
    {
        if (n == 0) n = 1;
        return (mem_malloc(n));
    }

    /* ======================== */
    void operator delete (void* p)
    {
        mem_free(p);
    }

    /* =========================== */
    void operator delete [] (void* p)
    {
        mem_free(p);
    }
};

}   /* namespace */

#endif  /* __BASIC_HPP__ */
