/*
***************************************
*   CPU STUFF @ 2014-03-21
***************************************
*/

#ifndef __CRISEC_HPP__
#define __CRISEC_HPP__

/* Asylum Namespace */
namespace asy {

/********************/
/* Critical Section */
/********************/
class crisec : public asylum
{
private:
#if defined(_CR_OS_MSWIN_)
    CRITICAL_SECTION    m_csec;
#else
    pthread_mutex_t     m_csec;
#endif

public:
    /* ====== */
    void init ()
    {
#if defined(_CR_OS_MSWIN_)
        ::InitializeCriticalSection(&m_csec);
#else
        pthread_mutexattr_t attr;

        ::pthread_mutexattr_init(&attr);
#if !defined(PTHREAD_MUTEX_RECURSIVE_NP)
        ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#else
        ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#endif
        ::pthread_mutex_init(&m_csec, &attr);
        ::pthread_mutexattr_destroy(&attr);
#endif
    }

    /* ====== */
    void free ()
    {
#if defined(_CR_OS_MSWIN_)
        ::DeleteCriticalSection(&m_csec);
#else
        ::pthread_mutex_destroy(&m_csec);
#endif
    }

public:
    /* ========= */
    void acquire ()
    {
#if defined(_CR_OS_MSWIN_)
        ::EnterCriticalSection(&m_csec);
#else
        ::pthread_mutex_lock(&m_csec);
#endif
    }

    /* ========= */
    void release ()
    {
#if defined(_CR_OS_MSWIN_)
        ::LeaveCriticalSection(&m_csec);
#else
        ::pthread_mutex_unlock(&m_csec);
#endif
    }
};

}   /* namespace */

#endif  /* __CRISEC_HPP__ */
