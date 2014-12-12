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
template<class T>
class bring_c : public asylum
{
private:
    T*      m_list;
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
        return (true);
    }

    /* ====== */
    void free ()
    {
        mem_free(m_list);
    }

public:
    /* ======= */
    void clear ()
    {
        m_head = 0;
        m_tail = 0;
        m_full = false;
    }

    /* ============== */
    size_t size () const
    {
        if (m_tail == m_head)
            return (m_full ? m_size : 0);
        if (m_tail  < m_head)
            return (m_size - m_head + m_tail);
        return (m_tail - m_head);
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
        total = this->size();
        if (size > total)
            size = total;
        for (total = size; size != 0; size--) {
            mem_cpy(data++, &m_list[m_head++], sizeof(T));
            if (m_head >= m_size)
                m_head = 0;
        }
        m_full = false;
        return (total);
    }

    /* ================================= */
    size_t see (T* data, size_t size) const
    {
        size_t  head, total;

        if (size == 0)
            return (0);
        total = this->size();
        if (size > total)
            size = total;
        head = m_head;
        for (total = size; size != 0; size--) {
            mem_cpy(data++, &m_list[head++], sizeof(T));
            if (head >= m_size)
                head = 0;
        }
        return (total);
    }

    /* ================================= */
    size_t put (const T* data, size_t size)
    {
        size_t  total;

        if (size == 0)
            return (0);
        for (total = size; size != 0; size--) {
            mem_cpy(&m_list[m_tail++], data++, sizeof(T));
            if (m_tail >= m_size)
                m_tail = 0;
            if (m_tail == m_head)
                m_full = true;
        }
        if (m_full)
            m_head = m_tail;
        return (total);
    }
};

/**************/
/* Byte RingN */
/**************/
template<class T, size_t N>
class bring_n : public asylum
{
private:
    bool    m_full;
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
    }

public:
    /* ======= */
    void clear ()
    {
        this->init();
    }

    /* ============== */
    size_t size () const
    {
        if (m_tail == m_head)
            return (m_full ? N : 0);
        if (m_tail  < m_head)
            return (N - m_head + m_tail);
        return (m_tail - m_head);
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
        total = this->size();
        if (size > total)
            size = total;
        for (total = size; size != 0; size--) {
            mem_cpy(data++, &m_list[m_head++], sizeof(T));
            if (m_head >= N)
                m_head = 0;
        }
        m_full = false;
        return (total);
    }

    /* ================================= */
    size_t see (T* data, size_t size) const
    {
        size_t  head, total;

        if (size == 0)
            return (0);
        total = this->size();
        if (size > total)
            size = total;
        head = m_head;
        for (total = size; size != 0; size--) {
            mem_cpy(data++, &m_list[head++], sizeof(T));
            if (head >= N)
                head = 0;
        }
        return (total);
    }

    /* ================================= */
    size_t put (const T* data, size_t size)
    {
        size_t  total;

        if (size == 0)
            return (0);
        for (total = size; size != 0; size--) {
            mem_cpy(&m_list[m_tail++], data++, sizeof(T));
            if (m_tail >= N)
                m_tail = 0;
            if (m_tail == m_head)
                m_full = true;
        }
        if (m_full)
            m_head = m_tail;
        return (total);
    }
};

}   /* namespace */

#endif  /* __BRING_HPP__ */
