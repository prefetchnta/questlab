/*
***************************************
*   CPU STUFF @ 2014-04-05
***************************************
*/

#ifndef __EVENTS_HPP__
#define __EVENTS_HPP__

/* Asylum Namespace */
namespace asy {

/****************/
/* System Event */
/****************/
class events : public asylum
{
private:
#if defined(_CR_OS_MSWIN_)
    HANDLE          m_evts[2];
#else
    volatile bool   m_evts[2];
    pthread_cond_t  m_thrd_cd;
    pthread_mutex_t m_thrd_mt;
#endif

public:
    /* ====== */
    void init ()
    {
#if defined(_CR_OS_MSWIN_)
        m_evts[0] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
        m_evts[1] = ::CreateEvent(NULL, TRUE, FALSE, NULL);
#else
        m_evts[0] = m_evts[1] = false;
        ::pthread_cond_init(&m_thrd_cd, NULL);
        ::pthread_mutex_init(&m_thrd_mt, NULL);
#endif
    }

    /* ====== */
    void free ()
    {
#if defined(_CR_OS_MSWIN_)
        ::CloseHandle(m_evts[1]);
        ::CloseHandle(m_evts[0]);
#else
        ::pthread_cond_destroy(m_thrd_cd);
        ::pthread_mutex_destroy(m_thrd_mt);
#endif
    }

public:
    /* ====== */
    void wait ()
    {
#if defined(_CR_OS_MSWIN_)
        ::WaitForMultipleObjects(2, m_evts, FALSE, INFINITE);
#else
        ::pthread_mutex_lock(&m_thrd_mt);
        while (!m_evts[0] && !m_evts[1])
            ::pthread_cond_wait(&m_thrd_cd, &m_thrd_mt);
        if (m_evts[0])
            m_evts[0] = false;
        ::pthread_mutex_unlock(&m_thrd_mt);
#endif
    }

    /* ==================== */
    void fire (bool broadcast)
    {
#if defined(_CR_OS_MSWIN_)
        if (broadcast)
            ::SetEvent(m_evts[1]);
        else
            ::SetEvent(m_evts[0]);
#else
        if (broadcast) {
            ::pthread_mutex_lock(&m_thrd_mt);
            m_evts[1] = true;
            ::pthread_cond_broadcast(&m_thrd_cd);
            ::pthread_mutex_unlock(&m_thrd_mt);
        }
        else {
            ::pthread_mutex_lock(&m_thrd_mt);
            m_evts[0] = true;
            ::pthread_cond_signal(&m_thrd_cd);
            ::pthread_mutex_unlock(&m_thrd_mt);
        }
#endif
    }
};

}   /* namespace */

#endif  /* __EVENTS_HPP__ */
