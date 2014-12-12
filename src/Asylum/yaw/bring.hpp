/*
***************************************
*   Yet Another Wheel @ 2014-03-19
***************************************
*/

#ifndef __BRING_HPP__
#define __BRING_HPP__

/* Asylum Namespace */
namespace asy {

/*************/
/* Byte Ring */
/*************/
template<class T, class TLCK = nolock>
class bring_c : public asylum
{
private:
    T*      m_list;
    TLCK    m_lock;
    bool    m_full;
    size_t  m_head;
    size_t  m_tail;
    size_t  m_size;

public:
    /* ================== */
    bool init (size_t count)
    {
        m_list = mem_talloc(count, T);
        if (m_list == NULL)
            return (false);
        m_head = 0;
        m_tail = 0;
        m_full = false;
        m_size = count;
        m_lock.init();
        return (true);
    }

    /* ====== */
    void free ()
    {
        mem_free(m_list);
        m_lock.free();
    }

public:
    /* ======= */
    void clear ()
    {
        m_lock.acquire();
        m_head = 0;
        m_tail = 0;
        m_full = false;
        m_lock.release();
    }

    /* ============================== */
    size_t size (bool lock = true) const
    {
        size_t  ret;

        if (lock) m_lock.acquire();
        if (m_tail == m_head)
            ret = m_full ? m_size : 0;
        else
        if (m_tail  < m_head)
            ret = m_size - m_head + m_tail;
        else
            ret = m_tail - m_head;
        if (lock) m_lock.release();
        return (ret);
    }

    /* ================== */
    size_t capacity () const
    {
        return (m_size);
    }

    /* =========================== */
    size_t get (T* data, size_t size)
    {
        size_t  total;

        if (size == 0)
            return (0);
        m_lock.acquire();
        total = this->size(false);
        if (size > total)
            size = total;
        for (total = size; size != 0; size--) {
            mem_cpy(data++, &m_list[m_head++], sizeof(T));
            if (m_head >= m_size)
                m_head = 0;
        }
        m_full = false;
        m_lock.release();
        return (total);
    }

    /* ================================= */
    size_t see (T* data, size_t size) const
    {
        size_t  head, total;

        if (size == 0)
            return (0);
        m_lock.acquire();
        total = this->size(false);
        if (size > total)
            size = total;
        head = m_head;
        for (total = size; size != 0; size--) {
            mem_cpy(data++, &m_list[head++], sizeof(T));
            if (head >= m_size)
                head = 0;
        }
        m_lock.release();
        return (total);
    }

    /* ================================= */
    size_t put (const T* data, size_t size)
    {
        size_t  total;

        if (size == 0)
            return (0);
        m_lock.acquire();
        for (total = size; size != 0; size--) {
            mem_cpy(&m_list[m_tail++], data++, sizeof(T));
            if (m_tail >= m_size)
                m_tail = 0;
            if (m_tail == m_head)
                m_full = true;
        }
        if (m_full)
            m_head = m_tail;
        m_lock.release();
        return (total);
    }
};

/**************/
/* Byte RingN */
/**************/
template<class T, size_t N, TLCK = nolock>
class bring_n : public asylum
{
private:
    bool    m_full;
    TLCK    m_lock;
    size_t  m_head;
    size_t  m_tail;
    T       m_list[N];

public:
    /* ====== */
    void init ()
    {
        m_head = 0;
        m_tail = 0;
        m_full = false;
        m_lock.init();
    }

public:
    /* ======= */
    void clear ()
    {
        m_lock.acquire();
        m_head = 0;
        m_tail = 0;
        m_full = false;
        m_lock.release();
    }

    /* ============================== */
    size_t size (bool lock = true) const
    {
        size_t  ret;

        if (lock) m_lock.acquire();
        if (m_tail == m_head)
            ret = m_full ? N : 0;
        else
        if (m_tail  < m_head)
            ret = N - m_head + m_tail;
        else
            ret = m_tail - m_head;
        if (lock) m_lock.release();
        return (ret);
    }

    /* ================== */
    size_t capacity () const
    {
        return (N);
    }

    /* =========================== */
    size_t get (T* data, size_t size)
    {
        size_t  total;

        if (size == 0)
            return (0);
        m_lock.acquire();
        total = this->size(false);
        if (size > total)
            size = total;
        for (total = size; size != 0; size--) {
            mem_cpy(data++, &m_list[m_head++], sizeof(T));
            if (m_head >= N)
                m_head = 0;
        }
        m_full = false;
        m_lock.release();
        return (total);
    }

    /* ================================= */
    size_t see (T* data, size_t size) const
    {
        size_t  head, total;

        if (size == 0)
            return (0);
        m_lock.acquire();
        total = this->size(false);
        if (size > total)
            size = total;
        head = m_head;
        for (total = size; size != 0; size--) {
            mem_cpy(data++, &m_list[head++], sizeof(T));
            if (head >= N)
                head = 0;
        }
        m_lock.release();
        return (total);
    }

    /* ================================= */
    size_t put (const T* data, size_t size)
    {
        size_t  total;

        if (size == 0)
            return (0);
        m_lock.acquire();
        for (total = size; size != 0; size--) {
            mem_cpy(&m_list[m_tail++], data++, sizeof(T));
            if (m_tail >= N)
                m_tail = 0;
            if (m_tail == m_head)
                m_full = true;
        }
        if (m_full)
            m_head = m_tail;
        m_lock.release();
        return (total);
    }
};

}   /* namespace */

#endif  /* __BRING_HPP__ */
