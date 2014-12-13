/*
***************************************
*   Yet Another Wheel @ 2013-12-30
***************************************
*/

#ifndef __LIST2_HPP__
#define __LIST2_HPP__

/* Asylum Namespace */
namespace asy {

/**************/
/* List2 Unit */
/**************/
template<class T>
struct list2_unit
{
    T               user;
    list2_unit<T>*  prev;
    list2_unit<T>*  next;
};

/*********/
/* List2 */
/*********/
template<class T, bool DIR = false>
class list2 : public asylum
{
private:
    size_t          m_cnts;
    list2_unit<T>*  m_head;
    list2_unit<T>*  m_tail;

private:
    /* ==================================== */
    void _del (list2_unit<T>* node, bool rels)
    {
        list2_unit<T>*  prev;
        list2_unit<T>*  next;

        if (m_cnts == 0)
            return;
        prev = node->prev;
        next = node->next;
        if (prev != NULL)
            prev->next = next;
        else
            m_head = next;
        if (next != NULL)
            next->prev = prev;
        else
            m_tail = prev;
        m_cnts -= 1;
        if (rels)
            node->user.free();
        mem_free(node);
    }

    /* =================================== */
    list2_unit<T>* _node (const T* obj) const
    {
        list2_unit<T>*  nnew;

        if (DIR) {
            nnew = static_cast<list2_unit<T>*>(obj);
        }
        else {
            nnew = struct_new(list2_unit<T>);
            if (nnew == NULL)
                return (NULL);
            mem_cpy(&nnew->user, obj, sizeof(T));
        }
        return (nnew);
    }

public:
    /* ====== */
    void init ()
    {
        m_cnts = 0;
        m_head = NULL;
        m_tail = NULL;
    }

    /* ====== */
    void free ()
    {
        list2_unit<T>*  prev;
        list2_unit<T>*  node;

        if (m_cnts != 0) {
            node = m_tail;
            do {
                prev = node->prev;
                node->user.free();
                mem_free(node);
                node = prev;
            } while (node != NULL);
        }
    }

public:
    /* ====================== */
    list2_unit<T>* head () const
    {
        return (m_head);
    }

    /* ====================== */
    list2_unit<T>* tail () const
    {
        return (m_tail);
    }

    /* ============== */
    size_t size () const
    {
        return (m_cnts);
    }

    /* ============================================== */
    list2_unit<T>* get (size_t idx, T* obj = NULL) const
    {
        list2_unit<T>*  node = m_head;

        for (; idx != 0; idx--)
            node = node->next;
        if (obj != NULL)
            mem_cpy(obj, &node->user, sizeof(T));
        return (node);
    }

    /* =================================================== */
    list2_unit<T>* get_safe (size_t idx, T* obj = NULL) const
    {
        if (idx >= m_cnts)
            return (NULL);
        return (this->get(idx, obj));
    }

    /* =================================== */
    list2_unit<T>* swap (list2_unit<T>* node)
    {
        list2_unit<T>*  prev;

        if (m_cnts <= 1 || node == m_head)
            return (NULL);
        prev = node->prev;
        if (prev->prev != NULL)
            prev->prev->next = node;
        else
            m_head = node;
        if (node->next != NULL)
            node->next->prev = prev;
        else
            m_tail = prev;
        node->prev = prev->prev;
        prev->prev = node;
        prev->next = node->next;
        node->next = prev;
        return (prev);
    }

    /* ======================== */
    void del (list2_unit<T>* node)
    {
        this->_del(node, true);
    }

    /* ================== */
    bool pop (T* obj = NULL)
    {
        if (m_cnts == 0)
            return (false);
        if (obj != NULL) {
            mem_cpy(obj, &m_tail->user, sizeof(T));
            this->_del(m_tail, false);
        }
        else {
            this->_del(m_tail, true);
        }
        return (true);
    }

    /* =================================================== */
    list2_unit<T>* insert (list2_unit<T>* node, const T* obj)
    {
        list2_unit<T>*  nnew;

        nnew = this->_node(obj);
        if (node == m_head) {
            nnew->prev = NULL;
            nnew->next = m_head;
            if (m_head != NULL)
                m_head->prev = nnew;
            else
                m_tail = nnew;
            m_head = nnew;
        }
        else {
            nnew->next = node;
            nnew->prev = node->prev;
            node->prev->next = nnew;
            node->prev = nnew;
        }
        m_cnts += 1;
        return (nnew);
    }

    /* ============================== */
    list2_unit<T>* sthead (const T* obj)
    {
        list2_unit<T>*  nnew;

        nnew = this->_node(obj);
        nnew->prev = NULL;
        nnew->next = m_head;
        if (m_head != NULL)
            m_head->prev = nnew;
        else
            m_tail = nnew;
        m_head = nnew;
        m_cnts += 1;
        return (nnew);
    }

    /* ============================== */
    list2_unit<T>* append (const T* obj)
    {
        list2_unit<T>*  nnew;

        nnew = this->_node(obj);
        nnew->next = NULL;
        nnew->prev = m_tail;
        if (m_tail != NULL)
            m_tail->next = nnew;
        else
            m_head = nnew;
        m_tail = nnew;
        m_cnts += 1;
        return (nnew);
    }

    /* ============================================= */
    template<class TRUN>void trav_h2t (void* ctx) const
    {
        if (m_cnts != 0)
        {
            TRUN            run;
            list2_unit<T>*  unt;

            unt = m_head;
            for (size_t idx = m_cnts; idx != 0; idx--) {
                if (!run.doit(ctx, &unt->user))
                    return;
                unt = unt->next;
            }
        }
    }

    /* ============================================= */
    template<class TRUN>void trav_t2h (void* ctx) const
    {
        if (m_cnts != 0)
        {
            TRUN            run;
            list2_unit<T>*  unt;

            unt = m_tail;
            for (size_t idx = m_cnts; idx != 0; idx--) {
                if (!run.doit(ctx, &unt->user))
                    return;
                unt = unt->prev;
            }
        }
    }
};

}   /* namespace */

#endif  /* __LIST2_HPP__ */
