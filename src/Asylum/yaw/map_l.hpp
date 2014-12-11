/*
***************************************
*   Yet Another Wheel @ 2014-02-07
***************************************
*/

#ifndef __MAP_L_HPP__
#define __MAP_L_HPP__

/* Asylum Namespace */
namespace asy {

/*******************/
/* Map (Use List2) */
/*******************/
template<class T, class K, class TCMP>
class map_lc : public asylum
{
private:
    size_t      m_cnts;
    size_t      m_size;
    list2<T>*   m_list;

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
        m_list = mem_talloc(count, list2<T>);
        if (m_list == NULL)
            return (false);
        mem_set(m_list, 0, count * sizeof(list2<T>));
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
        mem_set(m_list, 0, m_size * sizeof(list2<T>));
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
            TCMP            cmp;
            list2<T>*       lst;
            list2_unit<T>*  unt;

            lst = &m_list[cmp.hash(key) % m_size];
            unt = lst->head();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, &unt->user)) {
                    if (obj != NULL)
                        mem_cpy(obj, &unt->user, sizeof(T));
                    return (&unt->user);
                }
                unt = unt->next;
            }
        }
        return (NULL);
    }

    /* =========== */
    void del (K* key)
    {
        if (m_cnts != 0)
        {
            TCMP            cmp;
            list2<T>*       lst;
            list2_unit<T>*  unt;

            lst = &m_list[cmp.hash(key) % m_size];
            unt = lst->head();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, &unt->user)) {
                    lst->del(unt);
                    m_cnts -= 1;
                    break;
                }
                unt = unt->next;
            }
        }
    }

    /* =============================================== */
    T* insert (K* key, const T* obj, bool replace = true)
    {
        TCMP            cmp;
        list2<T>*       lst;
        list2_unit<T>*  unt;

        lst = &m_list[cmp.hash(key) % m_size];
        if (replace) {
            unt = lst->head();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, &unt->user)) {
                    unt->user.free();
                    mem_cpy(&unt->user, obj, sizeof(T));
                    return (&unt->user);
                }
                unt = unt->next;
            }
        }
        unt = lst->append(obj);
        if (unt != NULL)
            m_cnts += 1;
        return (&unt->user);
    }

    /* ============================================= */
    template<class TRUN>void traverse (void* ctx) const
    {
        if (m_cnts != 0)
        {
            TRUN        run;
            size_t      tot = m_cnts;
            list2<T>*   lst = m_list;

            for (size_t yy = m_size; yy != 0; yy--) {
                list2_unit<T>*  unt = lst->head();
                for (size_t xx = lst->size(); xx != 0; xx--) {
                    if (!run.doit(ctx, &unt->user))
                        return;
                    if (--tot == 0)
                        return;
                    unt = unt->next;
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
            TCMP            cmp;
            TRUN            run;
            list2<T>*       lst;
            list2_unit<T>*  unt;

            lst = &m_list[cmp.hash(key) % m_size];
            unt = lst->head();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, &unt->user)) {
                    if (!run.doit(ctx, &unt->user))
                        return;
                }
                unt = unt->next;
            }
        }
    }
};

/********************/
/* MapN (Use List2) */
/********************/
template<class T, class K, class TCMP, size_t N>
class map_ln : public asylum
{
private:
    size_t      m_cnts;
    list2<T>    m_list[N];

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
        mem_set(m_list, 0, N * sizeof(list2<T>));
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
            TCMP            cmp;
            list2<T>*       lst;
            list2_unit<T>*  unt;

            lst = &m_list[cmp.hash(key) % N];
            unt = lst->head();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, &unt->user)) {
                    if (obj != NULL)
                        mem_cpy(obj, &unt->user, sizeof(T));
                    return (&unt->user);
                }
                unt = unt->next;
            }
        }
        return (NULL);
    }

    /* =========== */
    void del (K* key)
    {
        if (m_cnts != 0)
        {
            TCMP            cmp;
            list2<T>*       lst;
            list2_unit<T>*  unt;

            lst = &m_list[cmp.hash(key) % N];
            unt = lst->head();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, &unt->user)) {
                    lst->del(unt);
                    m_cnts -= 1;
                    break;
                }
                unt = unt->next;
            }
        }
    }

    /* =============================================== */
    T* insert (K* key, const T* obj, bool replace = true)
    {
        TCMP            cmp;
        list2<T>*       lst;
        list2_unit<T>*  unt;

        lst = &m_list[cmp.hash(key) % N];
        if (replace) {
            unt = lst->head();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, &unt->user)) {
                    unt->user.free();
                    mem_cpy(&unt->user, obj, sizeof(T));
                    return (&unt->user);
                }
                unt = unt->next;
            }
        }
        unt = lst->append(obj);
        if (unt != NULL)
            m_cnts += 1;
        return (&unt->user);
    }

    /* ============================================= */
    template<class TRUN>void traverse (void* ctx) const
    {
        if (m_cnts != 0)
        {
            TRUN        run;
            size_t      tot = m_cnts;
            list2<T>*   lst = m_list;

            for (size_t yy = N; yy != 0; yy--) {
                list2_unit<T>*  unt = lst->head();
                for (size_t xx = lst->size(); xx != 0; xx--) {
                    if (!run.doit(ctx, &unt->user))
                        return;
                    if (--tot == 0)
                        return;
                    unt = unt->next;
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
            TCMP            cmp;
            TRUN            run;
            list2<T>*       lst;
            list2_unit<T>*  unt;

            lst = &m_list[cmp.hash(key) % N];
            unt = lst->head();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                if (cmp.match(key, &unt->user)) {
                    if (!run.doit(ctx, &unt->user))
                        return;
                }
                unt = unt->next;
            }
        }
    }
};

}   /* namespace */

#endif  /* __MAP_L_HPP__ */
