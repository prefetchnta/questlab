/*
***************************************
*   Yet Another Wheel @ 2018-08-06
***************************************
*/

#ifndef __COMPR_HPP__
#define __COMPR_HPP__

/* Asylum Namespace */
namespace asy {

/* ======================= */
template<class T, size_t MAX>
cr_inline size_t rle_match (const T* data, size_t size)
{
    size_t  len;

    if (size < sizeof(T))
        return (0);
    len = 1;
    size /= sizeof(T);
    for (; len < MAX && len < size && data[0] == data[1]; data++, len++);
    return (len);
}

/********/
/* LZ77 */
/********/
template<size_t WIN_SIZE, size_t MIN>
class LZ77 : public asylum
{
private:
    size_t  m_cur;
    byte_t* m_win;

public:
    /* ================ */
    void init (void* data)
    {
        m_cur = 0;
        m_win = (byte_t*)data;
    }

    /* ================== */
    size_t win_size () const
    {
        return (m_cur);
    }

    /* ================= */
    void move (size_t size)
    {
        if (size <= WIN_SIZE - m_cur)
        {
            // window is not full
            m_cur += size;
        }
        else
        {
            // window is full
            m_win += size - (WIN_SIZE - m_cur);
            m_cur = WIN_SIZE;
        }
    }

    /* ============================================= */
    ssize_t find (const void* data, size_t &size) const
    {
        if (m_cur < MIN)
            return (-1);

        size_t  pos, max = MIN - 1;

        if (size > m_cur)
            size = m_cur;

        // find repeat
        for (size_t off = m_cur - MIN; (ssize_t)off >= 0; off--)
        {
            size_t  len, end;

            // window cut down
            if (off + size > m_cur)
                end = m_cur;
            else
                end = off + size;

            for (len = off; len < end; len++) {
                if (m_win[len] != ((byte_t*)data)[len - off])
                    break;
            }
            len -= off;

            if (len > max)
            {
                // bingo!!!
                pos = off;
                max = len;
                if (len == size)
                    break;
            }
        }

        // no repeat
        if (max < MIN)
            return (-1);

        // return longest match
        size = max;
        return (ssize_t)(m_cur - pos);
    }
};

/**********/
/* BitOUT */
/**********/
template<class UNIT, size_t INIT>
class BitOUT : public asylum
{
private:
    UNIT*   m_flag;
    size_t  m_bits;
    size_t  m_curt;
    byte_t* m_data;

public:
    /* ================= */
    bool init (size_t size)
    {
        m_data = (byte_t*)mem_malloc(size);
        if (m_data == NULL)
            return (false);
        mem_set(m_data, 0, size);

        m_bits =  INIT;
        m_flag = (UNIT*)m_data;
        m_curt = (INIT == 0) ? 0 : sizeof(UNIT);
        return (true);
    }

    /* ====== */
    void free ()
    {
        mem_free(m_data);
    }

    /* ============= */
    void* data () const
    {
        return ((void*)m_data);
    }

    /* ============== */
    size_t size () const
    {
        return (m_curt);
    }

    /* ======================== */
    void put_bit_fl (size_t value)
    {
        if (m_bits == 0) {
            *(UNIT*)(&m_data[m_curt]) = 0;
            m_flag  = (UNIT*)(&m_data[m_curt]);
            m_bits  = sizeof(UNIT) * 8;
            m_curt += sizeof(UNIT);
        }
        m_flag[0] |= (UNIT)(value & 1) << (sizeof(UNIT) * 8 - m_bits--);
    }

    /* ====================================== */
    void put_bit_fl (size_t value, size_t count)
    {
        for (; count != 0; count--, value >>= 1)
            put_bit_fl(value);
    }

    /* ====================================== */
    void put_bit_fh (size_t value, size_t count)
    {
        for (; count != 0; count--)
            put_bit_fl(value >> (count - 1));
    }

    /* ======================================= */
    void put_data (const void* data, size_t size)
    {
        mem_cpy(&m_data[m_curt], data, size);
        m_curt += size;
    }

    /* ================================== */
    template<class T>void put_value (T data)
    {
        this->put_data(&data, sizeof(T));
    }

    /* ======================================== */
    template<class T>void put_data (const T& data)
    {
        this->put_data(&data, sizeof(T));
    }
};

}   /* namespace */

#endif  /* __COMPR_HPP__ */
