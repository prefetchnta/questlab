/*
***************************************
*   CPU STUFF @ 2014-04-01
***************************************
*/

#ifndef __THREAD_HPP__
#define __THREAD_HPP__

/* Asylum Namespace */
namespace asy {

/* Types */
typedef void*   asy_thrd_t;
#if defined(_CR_OS_MSWIN_)
typedef unsigned int (WINAPI *w32_main_t) (void*);
#endif
typedef void* (STDCALL *thrd_main_t) (void*);

/* ================================================================================ */
cr_inline asy_thrd_t thread_run (thrd_main_t start, void* param, size_t stacksize = 0)
{
#if defined(_CR_OS_MSWIN_)
    return ((asy_thrd_t)::_beginthreadex(NULL, (int)stacksize, (w32_main_t)start, param, 0, NULL));
#else
    int         retc;
    pthread_t   thrd;

    if (stacksize != 0)
    {
        pthread_attr_t  attr;

        ::pthread_attr_init(&attr);
        ::pthread_attr_setstacksize(&attr, stacksize);
        retc = ::pthread_create(&thrd, &attr, start, param);
        ::pthread_attr_destroy(&attr);
    }
    else
    {
        retc = ::pthread_create(&thrd, NULL, start, param);
    }
    return ((retc != 0) ? NULL : (asy_thrd_t)thrd);
#endif
}

/* ===================================== */
cr_inline void thread_end (asy_thrd_t thrd)
{
#if defined(_CR_OS_MSWIN_)
    ::WaitForSingleObject((HANDLE)thrd, INFINITE);
    ::CloseHandle((HANDLE)thrd);
#else
    ::pthread_join((pthread_t)thrd, NULL);
#endif
}

/* ======================================= */
cr_inline void thread_yield (unsigned int ms)
{
#if defined(_CR_OS_MSWIN_)
    ::Sleep(ms);
#else
    struct timespec req, rem;

    if (ms == 0) {
        ::sched_yield();
        return;
    }
    req.tv_sec  = (ms / 1000);
    req.tv_nsec = (ms % 1000) * 1000000;
    while (::nanosleep(&req, &rem) && errno == EINTR)
        req = rem;
#endif
}

}   /* namespace */

#endif  /* __THREAD_HPP__ */
