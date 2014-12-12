/*
***************************************
*   Yet Another Wheel @ 2013-12-19
***************************************
*/

#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

/* Asylum Namespace */
namespace asy {

/*********/
/* Array */
/*********/
template<class T, class TLCK = nolock>
class array : public asylum
{
private:
    T*      m_list;
    TLCK    m_lock;
    size_t  m_cnts;
    size_t  m_size;

private:
    /* ============== */
    void _clean () const
    {
        size_t  idx = m_cnts;

        while (idx-- != 0)
            m_list[idx].free();
    }

public:
    /* ====== */
    void init ()
    {
        m_cnts = 0;
        m_size = 0;
        m_list = NULL;
        m_lock.init();
    }

    /* ================== */
    bool init (size_t count)
    {
        m_size = 0;
        m_list = NULL;
        m_lock.init();
        if (!this->reserve(count, false))
            return (false);
        mem_set(m_list, 0, count * sizeof(T));
        m_cnts = count;
        return (true);
    }

    /* ====== */
    void free ()
    {
        if (m_list != NULL) {
            this->_clean();
            mem_free(m_list);
        }
        m_lock.free();
    }

public:
    /* ======= */
    void clear ()
    {
        m_lock.acquire();
        this->_clean();
        m_cnts = 0;
        m_lock.release();
    }

    /* ========== */
    T* data () const
    {
        T*  ret;

        m_lock.acquire();
        ret = m_list;
        m_lock.release();
        return (ret);
    }

    /* ============== */
    size_t size () const
    {
        size_t  ret;

        m_lock.acquire();
        ret = m_cnts;
        m_lock.release();
        return (ret);
    }

    /* ================== */
    size_t capacity () const
    {
        size_t  ret;

        m_lock.acquire();
        ret = m_size;
        m_lock.release();
        return (ret);
    }

    /* ================================== */
    T* get (size_t idx, T* obj = NULL) const
    {
        T*  ret;

        m_lock.acquire();
        ret = &m_list[idx];
        if (obj != NULL)
            mem_cpy(obj, ret, sizeof(T));
        m_lock.release();
        return (ret);
    }

    /* ====================================== */
    bool reserve (size_t size, bool lock = true)
    {
        T*  temp;

        if (lock) m_lock.acquire();
        if (size > m_size) {
            temp = mem_talloc(size, T);
            if (temp == NULL) {
                if (lock) m_lock.release();
                return (false);
            }
            if (m_list != NULL) {
                mem_cpy(temp, m_list, m_cnts * sizeof(T));
                mem_free(m_list);
            }
            m_size = size;
            m_list = temp;
        }
        if (lock) m_lock.release();
        return (true);
    }

    /* ========= */
    bool no_grow ()
    {
        T*  temp;

        m_lock.acquire();
        if (m_cnts != m_size) {
            if (m_cnts == 0) {
                mem_free(m_list);
                m_list = NULL;
                m_size = 0;
                m_lock.release();
                return (true);
            }
            temp = mem_talloc(m_cnts, T);
            if (temp == NULL) {
                m_lock.release();
                return (false);
            }
            mem_cpy(temp, m_list, m_cnts * sizeof(T));
            mem_free(m_list);
            m_size = m_cnts;
            m_list = temp;
        }
        m_lock.release();
        return (true);
    }

    /* ==================== */
    T* swap (size_t idx) const
    {
        T   tmp, *ret;

        if (idx == 0)
            return (NULL);
        m_lock.acquire();
        if (idx >= m_cnts) {
            m_lock.release();
            return (NULL);
        }
        ret = &m_list[idx];
        mem_cpy(&tmp, ret - 1, sizeof(T));
        mem_cpy(ret - 1, ret, sizeof(T));
        mem_cpy(ret, &tmp, sizeof(T));
        m_lock.release();
        return (ret);
    }

    /* =============== */
    void del (size_t idx)
    {
        m_lock.acquire();
        if (idx < m_cnts) {
            m_list[idx].free();
            m_cnts -= 1;
            if (m_cnts > idx) {
                mem_cpy(&m_list[idx], &m_list[idx + 1],
                        (m_cnts - idx) * sizeof(T));
            }
        }
        m_lock.release();
    }

    /* ================== */
    bool pop (T* obj = NULL)
    {
        m_lock.acquire();
        if (m_cnts == 0) {
            m_lock.release();
            return (false);
        }
        m_cnts -= 1;
        if (obj != NULL)
            mem_cpy(obj, &m_list[m_cnts], sizeof(T));
        else
            m_list[m_cnts].free();
        m_lock.release();
        return (true);
    }

    /* ================================================ */
    T* insert (size_t idx, const T* obj, bool grow = true)
    {
        T*  ret;

        m_lock.acquire();
        if (grow) {
            if (m_cnts >= m_size &&
                !this->reserve(m_size * 2 + 1, false)) {
                m_lock.release();
                return (NULL);
            }
        }
        else {
            if (m_cnts >= m_size) {
                m_lock.release();
                return (NULL);
            }
        }
        if (idx >= m_cnts) {
            ret = &m_list[m_cnts];
        }
        else {
            ret = &m_list[idx];
            mem_mov(ret + 1, ret, (m_cnts - idx) * sizeof(T));
        }
        m_cnts += 1;
        mem_cpy(ret, obj, sizeof(T));
        m_lock.release();
        return (ret);
    }

    /* ==================================== */
    T* sthead (const T* obj, bool grow = true)
    {
        return (this->insert(0, obj, grow));
    }

    /* ==================================== */
    T* append (const T* obj, bool grow = true)
    {
        T*  ret;

        m_lock.acquire();
        if (grow) {
            if (m_cnts >= m_size &&
                !this->reserve(m_size * 2 + 1, false)) {
                m_lock.release();
                return (NULL);
            }
        }
        else {
            if (m_cnts >= m_size) {
                m_lock.release();
                return (NULL);
            }
        }
        ret = &m_list[m_cnts++];
        mem_cpy(ret, obj, sizeof(T));
        m_lock.release();
        return (ret);
    }

    /* ============================================= */
    template<class TRUN>void trav_h2t (void* ctx) const
    {
        m_lock.acquire();
        if (m_cnts != 0)
        {
            T*      unt;
            TRUN    run;

            unt = m_list;
            for (size_t idx = m_cnts; idx != 0; idx--) {
                if (!run.doit(ctx, unt))
                    break;
                unt += 1;
            }
        }
        m_lock.release();
    }

    /* ============================================= */
    template<class TRUN>void trav_t2h (void* ctx) const
    {
        m_lock.acquire();
        if (m_cnts != 0)
        {
            T*      unt;
            TRUN    run;

            unt = m_list + m_cnts - 1;
            for (size_t idx = m_cnts; idx != 0; idx--) {
                if (!run.doit(ctx, unt))
                    break;
                unt -= 1;
            }
        }
        m_lock.release();
    }
};

}   /* namespace */

#endif  /* __ARRAY_HPP__ */
