/*
***************************************
*   Yet Another Wheel @ 2014-01-03
***************************************
*/

#ifndef __STACK_HPP__
#define __STACK_HPP__

/* Asylum Namespace */
namespace asy {

/*********************/
/* Stack (Use Array) */
/*********************/
template<class T>
class stack_a : public array<T>
{
public:
    /* ================ */
    T* push (const T* obj)
    {
        return (this->append(obj));
    }
};

/*********************/
/* Stack (Use List2) */
/*********************/
template<class T>
class stack_l : public list2<T>
{
public:
    /* ================ */
    T* push (const T* obj)
    {
        return (&((this->append(obj))->user));
    }
};

}   /* namespace */

#endif  /* __STACK_HPP__ */
