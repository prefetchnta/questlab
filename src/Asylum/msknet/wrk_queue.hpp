/*
***************************************
*   Misaka Network @ 2014-04-04
***************************************
*/

#ifndef __WRK_QUEUE_HPP__
#define __WRK_QUEUE_HPP__

/* Asylum Namespace */
namespace asy {

/**************/
/* Work Queue */
/**************/
template<class T, class TLCK = splock>
class wrk_queue : private oring_c<T>
{
private:
    TLCK    m_lock;

public:
    /* ================== */
    bool init (size_t count)
    {
        m_lock.init();
        return (oring_c<T>::init(count));
    }

    /* ====== */
    void free ()
    {
        oring_c<T>::free();
        m_lock.free();
    }

public:
    /* ======= */
    void clear ()
    {
        m_lock.acquire();
        oring_c<T>::clear();
        m_lock.release();
    }

    /* ======== */
    size_t size ()
    {
        size_t  ret;

        m_lock.acquire();
        ret = oring_c<T>::size();
        m_lock.release();
        return (ret);
    }

    /* ============ */
    size_t capacity ()
    {
        size_t  ret;

        m_lock.acquire();
        ret = oring_c<T>::capacity();
        m_lock.release();
        return (ret);
    }

    /* ============ */
    bool get (T* data)
    {
        bool    ret;

        m_lock.acquire();
        ret = oring_c<T>::get(data);
        m_lock.release();
        return (ret);
    }

    /* ================== */
    bool put (const T* data)
    {
        bool    ret;

        m_lock.acquire();
        ret = oring_c<T>::put(data);
        m_lock.release();
        return (ret);
    }
};

}   /* namespace */

#endif  /* __WRK_QUEUE_HPP__ */
