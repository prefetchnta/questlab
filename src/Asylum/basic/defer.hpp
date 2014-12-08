/*
***************************************
*   BASIC STUFF @ 2014-04-28
***************************************
*/

#ifndef __DEFER_HPP__
#define __DEFER_HPP__

/* Asylum Namespace */
namespace asy {

/******************************/
/* Auto Ptr for CrHack Memory */
/******************************/
class auto_mem : public asylum
{
private:
    const void* m_ptr;

public:
    /* ==================== */
    auto_mem (const void* ptr)
    {
        m_ptr = ptr;
    }
    ~auto_mem ()
    {
        mem_free(m_ptr);
    }
};

/******************************/
/* Auto Ptr for Asylum Object */
/******************************/
template<class T>
class auto_obj : public asylum
{
private:
    T*  m_obj;

public:
    /* =========== */
    auto_obj (T* obj)
    {
        m_obj = obj;
    }
    ~auto_obj ()
    {
        m_obj->free();
    }
};

}   /* namespace */

#endif  /* __DEFER_HPP__ */
