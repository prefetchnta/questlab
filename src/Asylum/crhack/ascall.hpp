/*
***************************************
*   CrHack C++ Wrapper @ 2014-04-29
***************************************
*/

#ifndef __ASCALL_HPP__
#define __ASCALL_HPP__

/* Asylum Namespace */
namespace asy {

/****************/
/* Call Context */
/****************/
template<class T>
struct ascall_parm
{
    T               user;
    volatile sint_t copy;
};

/*
    Example:

        uint_t STDCALL ascall (void_t* param)
        {
            T   usr;

            async_copy(&usr, param);
            ascall_do_something(&usr);
            return (TRUE);
        }
*/

/* =========== */
template<class T>
cr_inline void async_copy (T* local, void_t* param)
{
    ascall_parm<T>* ctx;

    ctx = (ascall_parm<T>*)param;
    mem_cpy(local, &ctx->user, sizeof(T));
    atom_inc(&ctx->copy);
}

/* =========== */
template<class T>
cr_inline void async_call (mt_main_t func, const T* user, uint_t stacksize = 0)
{
    thrd_t          thrd;
    ascall_parm<T>  parm;

    parm.copy = FALSE;
    mem_cpy(&parm.user, user, sizeof(T));
    thrd = thread_new(stacksize, func, &parm, FALSE);
    if (thrd != NULL) {
        thread_del(thrd);
        while (!parm.copy);
    }
}

/* =========================================================== */
cr_inline void async_call2 (mt_main_t func, uint_t stacksize = 0)
{
    thrd_t  thrd;

    thrd = thread_new(stacksize, func, NULL, FALSE);
    if (thrd != NULL)
        thread_del(thrd);
}

}   /* namespace */

#endif  /* __ASCALL_HPP__ */
