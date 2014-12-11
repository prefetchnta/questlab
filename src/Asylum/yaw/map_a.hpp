/*
***************************************
*   Yet Another Wheel @ 2014-02-07
***************************************
*/

#ifndef __MAP_A_HPP__
#define __MAP_A_HPP__

/* Asylum Namespace */
namespace asy {

/*******************/
/* Map (Use Array) */
/*******************/
template<class T, class K, class TCMP>
class map_ac : public asylum
{
private:
    size_t      m_cnts;
    size_t      m_size;
    array<T>*   m_list;

private:
    /* ============== */
    void _clean () const
    {
        for (size_t idx = 0; idx < m_size; idx++)
            m_list[idx].free();
    }

public:
    /* ================== */
    bool init (size_t count)
    {
        m_list = mem_talloc(count, array<T>);
        if (m_list == NULL)
            return (false);
        mem_set(m_list, 0, count * sizeof(array<T>));
        m_cnts = 0;
        m_size = count;
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
        mem_set(m_list, 0, m_size * sizeof(array<T>));
        m_cnts = 0;
    }

    /* ============== */
    size_t size () const
    {
        return (m_cnts);
    }

    /* ================ */
    size_t bucket () const
    {
        return (m_size);
    }

    /* ============================ */
    size_t conflict (size_t idx) const
    {
        if (idx >= m_size)
            return (0);
        return (m_list[idx].size());
    }

    /* ============================== */
    T* get (K* key, T* obj = NULL) const
    {
        if (m_cnts != 0)
        {
            T*          unt;
            TCMP        cmp;
            array<T>*   lst;

            lst = &m_list[cmp.hash(key) % m_size];
            unt = lst->data();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, unt)) {
                    if (obj != NULL)
                        mem_cpy(obj, unt, sizeof(T));
                    return (unt);
                }
                unt += 1;
            }
        }
        return (NULL);
    }

    /* =========== */
    void del (K* key)
    {
        if (m_cnts != 0)
        {
            T*          unt;
            TCMP        cmp;
            size_t      len;
            array<T>*   lst;

            lst = &m_list[cmp.hash(key) % m_size];
            unt = lst->data();
            len = lst->size();
            for (size_t idx = 0; idx < len; idx++) {
                if (cmp.match(key, unt)) {
                    lst->del(idx);
                    m_cnts -= 1;
                    break;
                }
                unt += 1;
            }
        }
    }

    /* =============================================== */
    T* insert (K* key, const T* obj, bool replace = true)
    {
        T*          unt;
        TCMP        cmp;
        array<T>*   lst;

        lst = &m_list[cmp.hash(key) % m_size];
        if (replace) {
            unt = lst->data();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, unt)) {
                    unt->free();
                    mem_cpy(unt, obj, sizeof(T));
                    return (unt);
                }
                unt += 1;
            }
        }
        unt = lst->append(obj);
        if (unt != NULL)
            m_cnts += 1;
        return (unt);
    }

    /* ============================================= */
    template<class TRUN>void traverse (void* ctx) const
    {
        if (m_cnts != 0)
        {
            TRUN        run;
            size_t      tot = m_cnts;
            array<T>*   lst = m_list;

            for (size_t yy = m_size; yy != 0; yy--) {
                T*  unt = lst->data();
                for (size_t xx = lst->size(); xx != 0; xx--) {
                    if (!run.doit(ctx, unt))
                        return;
                    if (--tot == 0)
                        return;
                    unt += 1;
                }
                lst += 1;
            }
        }
    }

    /* ===================================================== */
    template<class TRUN>void traverse (void* ctx, K* key) const
    {
        if (m_cnts != 0)
        {
            T*          unt;
            TCMP        cmp;
            TRUN        run;
            array<T>*   lst;

            lst = &m_list[cmp.hash(key) % m_size];
            unt = lst->data();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, unt)) {
                    if (!run.doit(ctx, unt))
                        return;
                }
                unt += 1;
            }
        }
    }
};

/********************/
/* MapN (Use Array) */
/********************/
template<class T, class K, class TCMP, size_t N>
class map_an : public asylum
{
private:
    size_t      m_cnts;
    array<T>    m_list[N];

private:
    /* ======== */
    void _clean ()
    {
        for (size_t idx = 0; idx < N; idx++)
            m_list[idx].free();
    }

public:
    /* ====== */
    void init ()
    {
        m_cnts = 0;
        mem_set(m_list, 0, N * sizeof(array<T>));
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
        return (m_cnts);
    }

    /* ================ */
    size_t bucket () const
    {
        return (N);
    }

    /* ============================ */
    size_t conflict (size_t idx) const
    {
        if (idx >= N)
            return (0);
        return (m_list[idx].size());
    }

    /* ============================== */
    T* get (K* key, T* obj = NULL) const
    {
        if (m_cnts != 0)
        {
            T*          unt;
            TCMP        cmp;
            array<T>*   lst;

            lst = &m_list[cmp.hash(key) % N];
            unt = lst->data();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, unt)) {
                    if (obj != NULL)
                        mem_cpy(obj, unt, sizeof(T));
                    return (unt);
                }
                unt += 1;
            }
        }
        return (NULL);
    }

    /* =========== */
    void del (K* key)
    {
        if (m_cnts != 0)
        {
            T*          unt;
            TCMP        cmp;
            size_t      len;
            array<T>*   lst;

            lst = &m_list[cmp.hash(key) % N];
            unt = lst->data();
            len = lst->size();
            for (size_t idx = 0; idx < len; idx++) {
                if (cmp.match(key, unt)) {
                    lst->del(idx);
                    m_cnts -= 1;
                    break;
                }
                unt += 1;
            }
        }
    }

    /* =============================================== */
    T* insert (K* key, const T* obj, bool replace = true)
    {
        T*          unt;
        TCMP        cmp;
        array<T>*   lst;

        lst = &m_list[cmp.hash(key) % N];
        if (replace) {
            unt = lst->data();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, unt)) {
                    unt->free();
                    mem_cpy(unt, obj, sizeof(T));
                    return (unt);
                }
                unt += 1;
            }
        }
        unt = lst->append(obj);
        if (unt != NULL)
            m_cnts += 1;
        return (unt);
    }

    /* ============================================= */
    template<class TRUN>void traverse (void* ctx) const
    {
        if (m_cnts != 0)
        {
            TRUN        run;
            size_t      tot = m_cnts;
            array<T>*   lst = m_list;

            for (size_t yy = N; yy != 0; yy--) {
                T*  unt = lst->data();
                for (size_t xx = lst->size(); xx != 0; xx--) {
                    if (!run.doit(ctx, unt))
                        return;
                    if (--tot == 0)
                        return;
                    unt += 1;
                }
                lst += 1;
            }
        }
    }

    /* ===================================================== */
    template<class TRUN>void traverse (void* ctx, K* key) const
    {
        if (m_cnts != 0)
        {
            T*          unt;
            TCMP        cmp;
            TRUN        run;
            array<T>*   lst;

            lst = &m_list[cmp.hash(key) % N];
            unt = lst->data();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, unt)) {
                    if (!run.doit(ctx, unt))
                        return;
                }
                unt += 1;
            }
        }
    }
};

}   /* namespace */

#endif  /* __MAP_A_HPP__ */
