/*
***************************************
*   CPU STUFF @ 2014-04-02
***************************************
*/

#ifndef __EXLOCK_HPP__
#define __EXLOCK_HPP__

/* Asylum Namespace */
namespace asy {

/* ASY_SMT_PAUSE for X86 & X64 */
#if defined(_CR_AR_X86_) || defined(_CR_AR_X64_)
    #if defined(_CR_CC_MSC_)
        #define ASY_SMT_PAUSE   _mm_pause();
    #else
        #define ASY_SMT_PAUSE   __asm__ __volatile__("rep; nop" : : : "memory");
    #endif
#endif

/***********/
/* Yield_K */
/***********/
class yield_k : public asylum
{
public:
    /* ========================== */
    void yield (unsigned int repeat)
    {
        if (repeat < 4)
        {
        }
#if defined(ASY_SMT_PAUSE)
        else
        if (repeat < 16)
        {
            ASY_SMT_PAUSE
        }
#endif
        else
#if defined(_CR_OS_MSWIN_)
        if (repeat < 32)
        {
            ::Sleep(0);
        }
        else
        {
            ::Sleep(1);
        }
#else
        if ((repeat < 32) || (repeat & 1))
        {
            ::sched_yield();
        }
        else
        {
            struct timespec req;

            req.tv_sec  = 0;
            req.tv_nsec = 1000;
            ::nanosleep(&req, NULL);
        }
#endif
    }
};

/********************/
/* Custom Spin Lock */
/********************/
template<class TSLP = yield_k>
class exlock : public asylum
{
private:
    TSLP            m_todo;
    volatile int    m_lock;

public:
    /* ====== */
    void init ()
    {
        m_lock = 0;
    }

public:
    /* ========= */
    void acquire ()
    {
        for (unsigned int kk = 0; atom_exchange32(&m_lock, 1); kk++)
            m_todo.yield(kk);
    }

    /* ========= */
    void release ()
    {
        atom_set_zero32(&m_lock);
    }
};

}   /* namespace */

#endif  /* __EXLOCK_HPP__ */
