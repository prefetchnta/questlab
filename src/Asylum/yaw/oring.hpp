/*
***************************************
*   Yet Another Wheel @ 2014-04-03
***************************************
*/

#ifndef __ORING_HPP__
#define __ORING_HPP__

/* Asylum Namespace */
namespace asy {

/***************/
/* Object Ring */
/***************/
template<class T>
class oring_c : public asylum
{
private:
    T*      m_list;
    size_t  m_head;
    size_t  m_tail;
    size_t  m_size;

private:
    /* ============== */
    void _clean () const
    {
        size_t  hd, tl;

        hd = m_head;
        tl = m_tail;
        while (hd != tl) {
            m_list[hd++].free();
            if (hd >= m_size)
                hd = 0;
        }
    }

public:
    /* ================== */
    bool init (size_t count)
    {
        m_list = mem_talloc(count + 1, T);
        if (m_list == NULL)
            return (false);
        m_head = 0;
        m_tail = 0;
        m_size = count + 1;
        return (true);
    }

    /* ====== */
    void free ()
    {
        this->_clean();
        mem_free(m_list);
    }

public:
    /* ======= */
    void clear ()
    {
        this->_clean();
        m_head = 0;
        m_tail = 0;
    }

    /* ============== */
    size_t size () const
    {
        if (m_tail < m_head)
            return (m_size - m_head + m_tail);
        return (m_tail - m_head);
    }

    /* ================== */
    size_t capacity () const
    {
        return (m_size - 1);
    }

    /* ============ */
    bool get (T* data)
    {
        if (m_head == m_tail)
            return (false);
        mem_cpy(data, &m_list[m_head++], sizeof(T));
        if (m_head >= m_size)
            m_head = 0;
        return (true);
    }

    /* ==================================== */
    bool put (const T* data, bool grow = true)
    {
        T*      temp;
        size_t  ii, tl = m_tail + 1;

        if (tl >= m_size)
            tl = 0;
        if (tl == m_head) {
            if (grow) {
                temp = mem_talloc(m_size * 2, T);
                if (temp == NULL)
                    return (false);
                for (tl = 0, ii = m_head; tl < m_size - 1; tl++) {
                    mem_cpy(&temp[tl], &m_list[ii++], sizeof(T));
                    if (ii >= m_size)
                        ii = 0;
                }
                mem_free(m_list);
                m_list = temp;
                m_head = 0;
                m_tail = m_size - 1;
                m_size *= 2;
            }
            else {
                return (false);
            }
        }
        mem_cpy(&m_list[m_tail++], data, sizeof(T));
        if (m_tail >= m_size)
            m_tail = 0;
        return (true);
    }
};

/****************/
/* Object RingN */
/****************/
template<class T, size_t N>
class oring_n : public asylum
{
private:
    size_t  m_head;
    size_t  m_tail;
    T       m_list[N + 1];

private:
    /* ======== */
    void _clean ()
    {
        size_t  hd, tl;

        hd = m_head;
        tl = m_tail;
        while (hd != tl) {
            m_list[hd++].free();
            if (hd >= N + 1)
                hd = 0;
        }
    }

public:
    /* ====== */
    void init ()
    {
        m_head = 0;
        m_tail = 0;
    }

    /* ====== */
    void free ()
    {
        this->_clean();
    }

public:
    /* ======= */
    void clear ()
    {
        this->_clean();
        this->init();
    }

    /* ============== */
    size_t size () const
    {
        if (m_tail < m_head)
            return (N + 1 - m_head + m_tail);
        return (m_tail - m_head);
    }

    /* ================== */
    size_t capacity () const
    {
        return (N);
    }

    /* ============ */
    bool get (T* data)
    {
        if (m_head == m_tail)
            return (false);
        mem_cpy(data, &m_list[m_head++], sizeof(T));
        if (m_head >= N + 1)
            m_head = 0;
        return (true);
    }

    /* ================== */
    bool put (const T* data)
    {
        size_t  tl = m_tail + 1;

        if (tl >= N + 1)
            tl = 0;
        if (tl == m_head)
            return (false);
        mem_cpy(&m_list[m_tail++], data, sizeof(T));
        if (m_tail >= N + 1)
            m_tail = 0;
        return (true);
    }
};

}   /* namespace */

#endif  /* __ORING_HPP__ */
