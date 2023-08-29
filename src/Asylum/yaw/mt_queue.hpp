/*
***************************************
*   Yet Another Wheel @ 2023-08-29
***************************************
*/

#ifndef __MT_QUEUE_HPP__
#define __MT_QUEUE_HPP__

/* Asylum Namespace */
namespace asy {

/************/
/* MT Queue */
/************/
template<size_t N, class TLCK = exlock<> >
class mt_queue : public asylum
{
private:
    TLCK                    m_lock;
    oring_c<msg_pack<N> >   m_list;

public:
    /* ================== */
    bool init (size_t count)
    {
        m_lock.init();
        return (m_list.init(count));
    }

    /* ====== */
    void free ()
    {
        m_list.free();
        m_lock.free();
    }

public:
    /* ======= */
    void clear ()
    {
        m_lock.acquire();
        m_list.clear();
        m_lock.release();
    }

    /* ============== */
    size_t size () const
    {
        size_t  ret;

        m_lock.acquire();
        ret = m_list.size();
        m_lock.release();
        return (ret);
    }

    /* ================== */
    size_t capacity () const
    {
        size_t  ret;

        m_lock.acquire();
        ret = m_list.capacity();
        m_lock.release();
        return (ret);
    }

    /* ===================== */
    bool get (msg_pack<N>* obj)
    {
        bool    ret;

        m_lock.acquire();
        ret = m_list.get(obj);
        m_lock.release();
        return (ret);
    }

    /* =========================== */
    bool see (msg_pack<N>* obj) const
    {
        bool    ret;

        m_lock.acquire();
        ret = m_list.see(obj);
        m_lock.release();
        return (ret);
    }

    /* =========================== */
    bool put (const msg_pack<N>* obj)
    {
        bool    ret;

        m_lock.acquire();
        ret = m_list.put(obj);
        m_lock.release();
        return (ret);
    }
};

}   /* namespace */

#endif  /* __MT_QUEUE_HPP__ */
