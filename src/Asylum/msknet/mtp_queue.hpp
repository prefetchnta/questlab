/*
***************************************
*   Misaka Network @ 2014-04-03
***************************************
*/

#ifndef __MTP_QUEUE_HPP__
#define __MTP_QUEUE_HPP__

/* Asylum Namespace */
namespace asy {

/**************/
/* Poll Queue */
/**************/
template<class T, size_t N>
class mtp_queue : public asylum
{
private:
    T**             m_list;
    bool*           m_flag;
    unsigned int    m_head;
    unsigned int    m_tail;

public:
    /* ====== */
    bool init ()
    {
        if ((N < 4) || (N & (N - 1) != 0))
            return (false);
        m_list = mem_talloc(N, T*);
        if (m_list == NULL)
            return (false);
        m_flag = mem_talloc(N, bool);
        if (m_flag == NULL) {
            mem_free(m_list);
            return (false);
        }
        mem_set(m_flag, 0, N * sizeof(bool));
        m_head = 0;
        m_tail = 0;
        return (true);
    }

    /* ====== */
    void free ()
    {
        mem_free(m_flag);
        mem_free(m_list);
    }

public:
    /* ================== */
    size_t capacity () const
    {
        return (N);
    }

    /* =========== */
    void put (T* ptr)
    {
        unsigned int    tail;

        // queue never full
        tail = atom_fetch_add32((int*)&m_tail, 1) % N;
        m_list[tail] = ptr;
        atom_rw_barrier();
        m_flag[tail] = true;
    }

    /* ======= */
    T* try_get ()
    {
        unsigned int    head = m_head;
        unsigned int    hptr = head % N;

        if (hptr == m_tail % N)
            return (NULL);
        if (!m_flag[hptr])
            return (NULL);
        atom_rw_barrier();

        T*  back = m_list[hptr];

        if (!atom_cmp_and_swap32b((int*)&m_head, head + 1, head))
            return (NULL);
        m_flag[hptr] = false;
        return (back);
    }
};

}   /* namespace */

#endif  /* __MTP_QUEUE_HPP__ */
