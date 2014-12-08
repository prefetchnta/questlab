/*
***************************************
*   Yet Another Wheel @ 2014-01-03
***************************************
*/

#ifndef __QUEUE_HPP__
#define __QUEUE_HPP__

/* Asylum Namespace */
namespace asy {

/*********************/
/* Queue (Use Array) */
/*********************/
template<class T>
class queue_a : public array<T>
{
public:
    /* ================= */
    T* input (const T* obj)
    {
        return (this->sthead(obj));
    }

    /* ===================== */
    bool output (T* obj = NULL)
    {
        return (this->pop(obj));
    }
};

/*********************/
/* Queue (Use List2) */
/*********************/
template<class T>
class queue_l : public list2<T>
{
public:
    /* ================= */
    T* input (const T* obj)
    {
        return (&((this->sthead(obj))->user));
    }

    /* ===================== */
    bool output (T* obj = NULL)
    {
        return (this->pop(obj));
    }
};

}   /* namespace */

#endif  /* __QUEUE_HPP__ */
