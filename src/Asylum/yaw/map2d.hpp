/*
***************************************
*   Yet Another Wheel @ 2014-02-11
***************************************
*/

#ifndef __MAP2D_HPP__
#define __MAP2D_HPP__

/* Asylum Namespace */
namespace asy {

/*********/
/* Map2D */
/*********/
template<class T>
class map2d_c : public asylum
{
private:
    T*      m_list;
    size_t  m_size;
    size_t  m_size_x;
    size_t  m_size_y;

public:
    /* ========================== */
    bool init (size_t nx, size_t ny)
    {
        size_t  total;

        total = nx * ny;
        if (nx > 1 && ny > 1) {
            if (total / nx != ny)
                return (false);
        }
        m_list = mem_talloc(total, T);
        if (m_list == NULL)
            return (false);
        mem_set(m_list, 0, total * sizeof(T));
        m_size = total;
        m_size_x = nx;
        m_size_y = ny;
        return (true);
    }

    /* ====== */
    void free ()
    {
        mem_free(m_list);
    }

public:
    /* ============= */
    void clear () const
    {
        mem_set(m_list, 0, m_size * sizeof(T));
    }

    /* ========== */
    T* data () const
    {
        return (m_list);
    }

    /* ================== */
    T* line (size_t y) const
    {
        return (&m_list[y * m_size_x]);
    }

    /* ======================= */
    T* line_safe (size_t y) const
    {
        if (y >= m_size_y)
            return (NULL);
        return (this->line(y));
    }

    /* ============== */
    size_t size () const
    {
        return (m_size);
    }

    /* =============== */
    size_t width () const
    {
        return (m_size_x);
    }

    /* ================ */
    size_t height () const
    {
        return (m_size_y);
    }

    /* ========================================== */
    T* get (size_t x, size_t y, T* obj = NULL) const
    {
        T*  unt = this->line(y) + x;

        if (obj != NULL)
            mem_cpy(obj, unt, sizeof(T));
        return (unt);
    }

    /* =============================================== */
    T* get_safe (size_t x, size_t y, T* obj = NULL) const
    {
        if (x >= m_size_x || y >= m_size_y)
            return (NULL);
        return (this->get(x, y, obj));
    }
};

/***********/
/* Map2DXY */
/***********/
template<class T, size_t NX, size_t NY>
class map2d_n : public asylum
{
private:
    T   m_list[NX * NY];

public:
    /* ====== */
    void init ()
    {
        mem_set(m_list, 0, NX * NY * sizeof(T));
    }

public:
    /* ======= */
    void clear ()
    {
        this->init();
    }

    /* ========== */
    T* data () const
    {
        return (m_list);
    }

    /* ================== */
    T* line (size_t y) const
    {
        return (&m_list[y * NX]);
    }

    /* ======================= */
    T* line_safe (size_t y) const
    {
        if (y >= NY)
            return (NULL);
        return (this->line(y));
    }

    /* ============== */
    size_t size () const
    {
        return (NX * NY);
    }

    /* =============== */
    size_t width () const
    {
        return (NX);
    }

    /* ================ */
    size_t height () const
    {
        return (NY);
    }

    /* ========================================== */
    T* get (size_t x, size_t y, T* obj = NULL) const
    {
        T*  unt = this->line(y) + x;

        if (obj != NULL)
            mem_cpy(obj, unt, sizeof(T));
        return (unt);
    }

    /* =============================================== */
    T* get_safe (size_t x, size_t y, T* obj = NULL) const
    {
        if (x >= NX || y >= NY)
            return (NULL);
        return (this->get(x, y, obj));
    }
};

}   /* namespace */

#endif  /* __MAP2D_HPP__ */
