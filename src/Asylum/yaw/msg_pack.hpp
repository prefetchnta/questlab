/*
***************************************
*   Yet Another Wheel @ 2023-08-29
***************************************
*/

#ifndef __MSG_PACK_HPP__
#define __MSG_PACK_HPP__

/* Asylum Namespace */
namespace asy {

/****************/
/* Message Pack */
/****************/
template<size_t N>
class msg_pack : public asylum
{
private:
    void*   m_data;
    size_t  m_size;
    byte_t  m_buff[N];

public:
    /* ==================================== */
    void* init (const void* data, size_t size)
    {
        if (size <= sizeof(m_buff)) {
            mem_cpy(m_buff, data, size);
            m_size = (size_t)(0-size);
            return ((void*)m_buff);
        }
        m_data = mem_malloc(size);
        if (m_data == NULL)
            return (NULL);
        mem_cpy(m_data, data, size);
        m_size = size;
        return (m_data);
    }

    /* ================================= */
    template<class T>T* init (const T* obj)
    {
        return ((T*)(this->init((const void*)obj, sizeof(T))));
    }

    /* ====== */
    void free ()
    {
        if ((ssize_t)m_size > 0)
            mem_free(m_data);
    }

public:
    /* ============== */
    size_t size () const
    {
        if ((ssize_t)m_size > 0)
            return (m_size);
        return ((size_t)(0-m_size));
    }

    /* ============= */
    void* data () const
    {
        if ((ssize_t)m_size > 0)
            return (m_data);
        return ((void*)m_buff);
    }

    /* ========================== */
    template<class T>T* obj () const
    {
        if ((ssize_t)m_size > 0)
            return ((T*)m_data);
        return ((T*)m_buff);
    }
};

}   /* namespace */

#endif  /* __MSG_PACK_HPP__ */
