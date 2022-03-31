/*
***************************************
*   BASIC STUFF @ 2014-04-28
***************************************
*/

#ifndef __SAFER_HPP__
#define __SAFER_HPP__

/* Asylum Namespace */
namespace asy {

/* =========== */
template<class T>
cr_inline bool safe_addu (T val1, T val2, T* dst = NULL)
{
    T   tmp = val1 + val2;

    if (dst != NULL)
        *dst = tmp;
    if (tmp >= val1)
        return (true);
    return (false);
}

/* =========== */
template<class T>
cr_inline bool safe_adds (T val1, T val2, T* dst = NULL)
{
    T   tmp = val1 + val2;

    if (dst != NULL)
        *dst = tmp;
    if ((tmp ^ val1) >= 0 || (tmp ^ val2) >= 0)
        return (true);
    return (false);
}

/* ====================== */
template<class TS, class TD>
cr_inline bool safe_cast (TS src, TD* dst = NULL)
{
    TD  tmp = (TD)src;

    if (dst != NULL)
        *dst = tmp;
    if (tmp == src)
        return (true);
    return (false);
}

/* =========== */
template<class T>
cr_inline bool safe_mult (T val1, T val2, T* dst = NULL)
{
    T   tmp = val1 * val2;

    if (dst != NULL)
        *dst = tmp;
    if (val1 <= 1 || val2 <= 1)
        return (true);
    if ((tmp / val1) == val2)
        return (true);
    return (false);
}

/* =========== */
template<class T>
cr_inline bool safe_madu (T mul1, T mul2, T addu, T* dst = NULL)
{
    T   tmp;

    if (!safe_mult<T>(mul1, mul2, &tmp))
        return (false);
    return (safe_addu<T>(addu, tmp, dst));
}

/* =========== */
template<class T>
cr_inline bool safe_mads (T mul1, T mul2, T adds, T* dst = NULL)
{
    T   tmp;

    if (!safe_mult<T>(mul1, mul2, &tmp))
        return (false);
    return (safe_adds<T>(adds, tmp, dst));
}

}   /* namespace */

#endif  /* __SAFER_HPP__ */
