/*
***************************************
*   Yet Another Wheel @ 2014-02-08
***************************************
*/

#ifndef __TABLE_HPP__
#define __TABLE_HPP__

/* Asylum Namespace */
namespace asy {

/*********/
/* Table */
/*********/
template<class T, class K, class TCMP>
class table_c : public asylum
{
private:
    T*      m_list;
    char*   m_flag;
    size_t  m_cnts;
    size_t  m_size;

private:
    /* ============== */
    void _clean () const
    {
        size_t  idx = m_size;

        while (idx-- != 0) {
            if (m_flag[idx] == CR_PLTABLE_HAVE)
                m_list[idx].free();
        }
    }

public:
    /* ================== */
    bool init (size_t count)
    {
        m_list = mem_talloc(count, T);
        if (m_list == NULL)
            return (false);
        m_flag = mem_talloc(count, char);
        if (m_flag == NULL) {
            mem_free(m_list);
            return (false);
        }
        mem_set(m_flag, 0, count * sizeof(char));
        m_cnts = 0;
        m_size = count;
        return (true);
    }

    /* ====== */
    void free ()
    {
        this->_clean();
        mem_free(m_flag);
        mem_free(m_list);
    }

public:
    /* ======= */
    void clear ()
    {
        this->_clean();
        mem_set(m_flag, 0, m_size * sizeof(char));
        m_cnts = 0;
    }

    /* ========== */
    T* data () const
    {
        return (m_list);
    }

    /* ============= */
    char* flag () const
    {
        return (m_flag);
    }

    /* ============== */
    size_t size () const
    {
        return (m_cnts);
    }

    /* ================== */
    size_t capacity () const
    {
        return (m_size);
    }

    /* ======================================== */
    void replace (const table_c<T, K, TCMP>* copy)
    {
        mem_free(m_flag);
        mem_free(m_list);
        m_list = copy->data();
        m_flag = copy->flag();
        m_size = copy->capacity();
        m_cnts = copy->size();
    }

    /* ============================== */
    T* get (K* key, T* obj = NULL) const
    {
        if (m_cnts != 0)
        {
            T*      unt;
            TCMP    cmp;
            size_t  beg;

            beg = cmp.hash(key) % m_size;
            for (size_t idx = m_size; idx != 0; idx--) {
                if (m_flag[beg] == CR_PLTABLE_NONE)
                    break;
                if (m_flag[beg] == CR_PLTABLE_HAVE) {
                    unt = &m_list[beg];
                    if (cmp.match(key, unt)) {
                        if (obj != NULL)
                            mem_cpy(obj, unt, sizeof(T));
                        return (unt);
                    }
                }
                if (++beg >= m_size)
                    beg = 0;
            }
        }
        return (NULL);
    }

    /* =========== */
    void del (K* key)
    {
        if (m_cnts != 0)
        {
            T*      unt;
            TCMP    cmp;
            size_t  beg;

            beg = cmp.hash(key) % m_size;
            for (size_t idx = m_size; idx != 0; idx--) {
                if (m_flag[beg] == CR_PLTABLE_NONE)
                    break;
                if (m_flag[beg] == CR_PLTABLE_HAVE) {
                    unt = &m_list[beg];
                    if (cmp.match(key, unt)) {
                        unt->free();
                        m_flag[beg] = CR_PLTABLE_SKIP;
                        m_cnts -= 1;
                        break;
                    }
                }
                if (++beg >= m_size)
                    beg = 0;
            }
        }
    }

    /* ========================== */
    T* insert (K* key, const T* obj)
    {
        T*      unt;
        TCMP    cmp;
        size_t  beg;

        beg = cmp.hash(key) % m_size;
        for (size_t idx = m_size; idx != 0; idx--) {
            unt = &m_list[beg];
            if (m_flag[beg] != CR_PLTABLE_HAVE) {
                m_flag[beg]  = CR_PLTABLE_HAVE;
                mem_cpy(unt, obj, sizeof(T));
                m_cnts += 1;
                return (unt);
            }
            if (cmp.match(key, unt)) {
                unt->free();
                mem_cpy(unt, obj, sizeof(T));
                return (unt);
            }
            if (++beg >= m_size)
                beg = 0;
        }
        return (NULL);
    }

    /* ============================================= */
    template<class TRUN>void traverse (void* ctx) const
    {
        if (m_cnts != 0)
        {
            TRUN    run;
            size_t  tot = m_cnts;

            for (size_t idx = 0; idx < m_size; idx++) {
                if (m_flag[idx] == CR_PLTABLE_HAVE) {
                    if (!run.doit(ctx, &m_list[idx]))
                        return;
                    if (--tot == 0)
                        return;
                }
            }
        }
    }
};

/**********/
/* TableN */
/**********/
template<class T, class K, class TCMP, size_t N>
class table_n : public asylum
{
private:
    size_t  m_cnts;
    T       m_list[N];
    char    m_flag[N];

private:
    /* ======== */
    void _clean ()
    {
        size_t  idx = N;

        while (idx-- != 0) {
            if (m_flag[idx] == CR_PLTABLE_HAVE)
                m_list[idx].free();
        }
    }

public:
    /* ====== */
    void init ()
    {
        m_cnts = 0;
        mem_set(m_flag, 0, N * sizeof(char));
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

    /* ========== */
    T* data () const
    {
        return (m_list);
    }

    /* ============= */
    char* flag () const
    {
        return (m_flag);
    }

    /* ============== */
    size_t size () const
    {
        return (m_cnts);
    }

    /* ================== */
    size_t capacity () const
    {
        return (N);
    }

    /* ============================== */
    T* get (K* key, T* obj = NULL) const
    {
        if (m_cnts != 0)
        {
            T*      unt;
            TCMP    cmp;
            size_t  beg;

            beg = cmp.hash(key) % N;
            for (size_t idx = N; idx != 0; idx--) {
                if (m_flag[beg] == CR_PLTABLE_NONE)
                    break;
                if (m_flag[beg] == CR_PLTABLE_HAVE) {
                    unt = &m_list[beg];
                    if (cmp.match(key, unt)) {
                        if (obj != NULL)
                            mem_cpy(obj, unt, sizeof(T));
                        return (unt);
                    }
                }
                if (++beg >= N)
                    beg = 0;
            }
        }
        return (NULL);
    }

    /* =========== */
    void del (K* key)
    {
        if (m_cnts != 0)
        {
            T*      unt;
            TCMP    cmp;
            size_t  beg;

            beg = cmp.hash(key) % N;
            for (size_t idx = N; idx != 0; idx--) {
                if (m_flag[beg] == CR_PLTABLE_NONE)
                    break;
                if (m_flag[beg] == CR_PLTABLE_HAVE) {
                    unt = &m_list[beg];
                    if (cmp.match(key, unt)) {
                        unt->free();
                        m_flag[beg] = CR_PLTABLE_SKIP;
                        m_cnts -= 1;
                        break;
                    }
                }
                if (++beg >= N)
                    beg = 0;
            }
        }
    }

    /* ========================== */
    T* insert (K* key, const T* obj)
    {
        T*      unt;
        TCMP    cmp;
        size_t  beg;

        beg = cmp.hash(key) % N;
        for (size_t idx = N; idx != 0; idx--) {
            unt = &m_list[beg];
            if (m_flag[beg] != CR_PLTABLE_HAVE) {
                m_flag[beg]  = CR_PLTABLE_HAVE;
                mem_cpy(unt, obj, sizeof(T));
                m_cnts += 1;
                return (unt);
            }
            if (cmp.match(key, unt)) {
                unt->free();
                mem_cpy(unt, obj, sizeof(T));
                return (unt);
            }
            if (++beg >= N)
                beg = 0;
        }
        return (NULL);
    }

    /* ============================================= */
    template<class TRUN>void traverse (void* ctx) const
    {
        if (m_cnts != 0)
        {
            TRUN    run;
            size_t  tot = m_cnts;

            for (size_t idx = 0; idx < N; idx++) {
                if (m_flag[idx] == CR_PLTABLE_HAVE) {
                    if (!run.doit(ctx, &m_list[idx]))
                        return;
                    if (--tot == 0)
                        return;
                }
            }
        }
    }
};

}   /* namespace */

#endif  /* __TABLE_HPP__ */
