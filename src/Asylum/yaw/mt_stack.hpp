/*
***************************************
*   Yet Another Wheel @ 2023-08-29
***************************************
*/

#ifndef __MT_STACK_HPP__
#define __MT_STACK_HPP__

/* Asylum Namespace */
namespace asy {

/************/
/* MT Stack */
/************/
template<size_t N, class TLCK = exlock<> >
class mt_stack : public asylum
{
private:
    TLCK                    m_lock;
    array<msg_pack<N> >     m_list;

public:
    /* ================== */
    bool init (size_t count)
    {
        m_lock.init();
        m_list.init();
        return (m_list.reserve(count));
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

    /* =========================== */
    bool top (msg_pack<N>* obj) const
    {
        bool    ret;
        size_t  cnt;

        ret = false;
        m_lock.acquire();
        cnt = m_list.size();
        if (cnt != 0) {
            m_list.get(cnt - 1, obj);
            ret = true;
        }
        m_lock.release();
        return (ret);
    }

    /* ============================ */
    bool pop (msg_pack<N>* obj = NULL)
    {
        bool    ret;

        m_lock.acquire();
        ret = m_list.pop(obj);
        m_lock.release();
        return (ret);
    }

    /* ============================ */
    bool push (const msg_pack<N>* obj)
    {
        bool    ret;

        m_lock.acquire();
        ret = (m_list.append(obj) != NULL) ? true : false;
        m_lock.release();
        return (ret);
    }
};

}   /* namespace */

#endif  /* __MT_STACK_HPP__ */
