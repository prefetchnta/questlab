/*
***************************************
*   CrHack C++ Wrapper @ 2014-04-04
***************************************
*/

#ifndef __MTPOOL_HPP__
#define __MTPOOL_HPP__

/* Asylum Namespace */
namespace asy {

/***************/
/* Thread Unit */
/***************/
template<class T>
struct mtpool_unit
{
    T               user;
    thrd_t          thrd;
    volatile bool_t empty;
    volatile sint_t is_free;
    volatile sint_t is_over;
    volatile sint_t is_quit;

    /* ======= */
    void _done ()
    {
        this->user.free();
        atom_inc(&this->is_free);
        thread_stop(this->thrd);
    }

    /* ======= */
    void _over ()
    {
        atom_inc(&this->is_over);
    }
};

/*
    Example:

        uint_t STDCALL worker (void_t* param)
        {
            mtpool_unit<T>* ctx;

            ctx = (mtpool_unit<T>*)param;
            if (!ctx->is_quit || !ctx->empty) {
                worker_init();
                while (!ctx->is_quit) {
                    job_done = worker_todo();
                    if (job_done)
                        ctx->_done();
                }
                worker_free();
            }
            ctx->_over();
            return (TRUE);
        }
*/

/************************/
/* Thread Pool (Manual) */
/************************/
template<class T, uint_t N>
class CMtPool : public asylum
{
private:
    uint_t          m_count;
    mtpool_unit<T>  m_list[N];

public:
    /* ============================================================= */
    void init (mt_main_t start, uint_t count = N, uint_t stacksize = 0)
    {
        mtpool_unit<T>* ptr = m_list;

        if (count > N)
            count = N;
        m_count = count;
        for (uint_t idx = 0; idx < count; idx++, ptr++) {
            ptr->thrd = thread_new(stacksize, start, ptr, TRUE);
            if (ptr->thrd == NULL) {
                ptr->is_free = FALSE;
                ptr->is_over = TRUE;
            }
            else {
                ptr->is_free = TRUE;
                ptr->is_over = FALSE;
            }
            ptr->empty = TRUE;
            ptr->is_quit = FALSE;
        }
    }

    /* ====== */
    void free ()
    {
        uint_t          total;
        mtpool_unit<T>* ptr = m_list;

        for (uint_t idx = 0; idx < m_count; idx++, ptr++)
            atom_inc(&ptr->is_quit);
        do {
            thread_sleep(N);
            total = 0;
            ptr = m_list;
            for (uint_t idx = 0; idx < m_count; idx++, ptr++) {
                if (ptr->is_over || ptr->is_free)
                    total += 1;
            }
        } while (total < m_count);

        thread_sleep(N);
        ptr = m_list;
        for (uint_t idx = 0; idx < m_count; idx++, ptr++) {
            if (ptr->thrd != NULL) {
                if (ptr->is_free)
                    thread_wake(ptr->thrd);
                thread_wait(ptr->thrd, CR_INFINITE);
                thread_del(ptr->thrd);
            }
        }
    }

public:
    /* ======================================================== */
    bool wakeup (const T* param, uint_t times = 3, sint_t ms = -1)
    {
        uint_t          ii;
        mtpool_unit<T>* ptr;

        while (times != 0) {
            ii = (uint_t)(timer_get32() % m_count);
            for (uint_t idx = 0; idx < m_count; idx++) {
                ptr = &m_list[ii];
                if (ptr->is_free) {
                    ptr->is_free = FALSE;
                    mem_cpy(&ptr->user, param, sizeof(T));
                    ptr->empty = FALSE;
                    thread_wake(ptr->thrd);
                    return (true);
                }
                if (++ii >= m_count)
                    ii = 0;
            }
            if (ms >= 0)
                thread_sleep(ms);
            times -= 1;
        }
        return (false);
    }
};

}   /* namespace */

#endif  /* __MTPOOL_HPP__ */
