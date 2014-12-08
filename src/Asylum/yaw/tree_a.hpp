/*
***************************************
*   Yet Another Wheel @ 2014-02-10
***************************************
*/

#ifndef __TREE_A_HPP__
#define __TREE_A_HPP__

/* Asylum Namespace */
namespace asy {

/*************************/
/* Tree Unit (Use Array) */
/*************************/
template<class T>
struct ntree_unit
{
    T                   user;
    size_t              deep;
    ntree_unit<T>*      uppe;
    array<cnode_ptr>    next;
    size_t              node;
};

/********************/
/* Tree (Use Array) */
/********************/
template<class T, size_t N = 0>
class tree_a : public asylum
{
private:
    size_t          m_cnts;
    ntree_unit<T>*  m_root;

public:
    /* ================== */
    bool init (const T* obj)
    {
        m_root = struct_new(ntree_unit<T>);
        if (m_root == NULL)
            return (false);
        if (N >= 2) {
            if (!m_root->next.init(N)) {
                mem_free(m_root);
                return (false);
            }
        }
        else {
            m_root->next.init();
        }
        m_cnts = 1;
        m_root->node = 0;
        m_root->uppe = NULL;
        m_root->deep = 0;
        mem_cpy(&m_root->user, obj, sizeof(T));
        return (true);
    }

    /* ====== */
    void free ()
    {
        this->del(m_root);
    }

public:
    /* ====================== */
    ntree_unit<T>* root () const
    {
        return (m_root);
    }

    /* =============== */
    size_t total () const
    {
        return (m_cnts);
    }

    /* ====================================== */
    size_t child_num (ntree_unit<T>* node) const
    {
        return (node->next.size());
    }

    /* =================================================================== */
    ntree_unit<T>* get (ntree_unit<T>* node, size_t idx, T* obj = NULL) const
    {
        cnode_ptr*      unt;
        ntree_unit<T>*  dat;

        unt = node->next.get(idx);
        dat = static_cast<ntree_unit<T>*>(unt->ptr);
        if (obj != NULL && dat != NULL)
            mem_cpy(obj, &dat->user, sizeof(T));
        return (dat);
    }

    /* ======================================================================== */
    ntree_unit<T>* get_safe (ntree_unit<T>* node, size_t idx, T* obj = NULL) const
    {
        if (idx >= node->next.size())
            return (NULL);
        return (this->get(node, idx, obj));
    }

    /* =========================================== */
    ntree_unit<T>* parent (ntree_unit<T>* node) const
    {
        return (node->uppe);
    }

    /* ========================================= */
    ntree_unit<T>* prev (ntree_unit<T>* node) const
    {
        cnode_ptr*  unt;

        if (node->uppe == NULL || node->node == 0)
            return (NULL);
        unt = node->uppe->next.get(node->node - 1);
        if (unt != NULL)
            return (static_cast<ntree_unit<T>*>(unt->ptr));
        return (NULL);
    }

    /* ========================================= */
    ntree_unit<T>* next (ntree_unit<T>* node) const
    {
        cnode_ptr*  unt;

        if (node->uppe == NULL)
            return (NULL);
        unt = node->uppe->next.get_safe(node->node + 1);
        if (unt != NULL)
            return (static_cast<ntree_unit<T>*>(unt->ptr));
        return (NULL);
    }

    /* ======================== */
    void del (ntree_unit<T>* node)
    {
        if (m_cnts != 0)
        {
            cnode_ptr*          unt;
            array<cnode_ptr>*   lst;

            lst = &node->next;
            unt = lst->data() + lst->size() - 1;
            for (size_t idx = lst->size(); idx != 0; idx--) {
                ntree_unit<T>*  child;
                child = static_cast<ntree_unit<T>*>(unt->ptr);
                unt -= 1;
                if (child != NULL)
                    this->del(child);
            }
            node->next.free();
            node->user.free();
            if (node->uppe != NULL) {
                lst = &node->uppe->next;
                unt = lst->get(node->node);
                unt->ptr = NULL;
            }
            m_cnts -= 1;
            mem_free(node);
        }
    }

    /* =============================================================== */
    ntree_unit<T>* insert (ntree_unit<T>* node, const T* obj, size_t idx)
    {
        cnode_ptr           next;
        cnode_ptr*          unit;
        ntree_unit<T>*      nnew;
        array<cnode_ptr>*   list;

        nnew = struct_new(ntree_unit<T>);
        if (nnew == NULL)
            return (NULL);
        if (N >= 2) {
            if (!nnew->next.init(N)) {
                mem_free(nnew);
                return (NULL);
            }
        }
        else {
            nnew->next.init();
        }
        list = &node->next;
        if (idx >= list->size()) {
            next.ptr = (void*)nnew;
            unit = list->append(&next);
            if (unit == NULL) {
                if (N >= 2)
                    nnew->next.free();
                mem_free(nnew);
                return (NULL);
            }
            nnew->node = list->size() - 1;
        }
        else {
            unit = list->get(idx);
            if (unit->ptr != NULL) {
                if (N >= 2)
                    nnew->next.free();
                mem_free(nnew);
                return (NULL);
            }
            nnew->node = idx;
            unit->ptr = (void*)nnew;
        }
        m_cnts += 1;
        nnew->uppe = node;
        nnew->deep = node->deep + 1;
        mem_cpy(&nnew->user, obj, sizeof(T));
        return (nnew);
    }

    /* =================================================== */
    ntree_unit<T>* append (ntree_unit<T>* node, const T* obj)
    {
        return (this->insert(node, obj, node->next.size()));
    }

    /* ============================================= */
    template<class TRUN>void trav_bfs (void* ctx) const
    {
        if (m_cnts != 0)
        {
            TRUN    run;

            if (!run.doit(ctx, &m_root->user))
                return;
            this->trav_bfs_int<TRUN>(ctx, m_root);
        }
    }

    /* ============================================= */
    template<class TRUN>void trav_dfs (void* ctx) const
    {
        if (m_cnts != 0)
        {
            TRUN    run;

            this->trav_dfs_int<TRUN>(ctx, m_root);
        }
    }

private:
    /* ====================================================================== */
    template<class TRUN>void trav_bfs_int (void* ctx, ntree_unit<T>* node) const
    {
        TRUN                run;
        cnode_ptr*          unt;
        array<cnode_ptr>*   lst;

        lst = &node->next;
        if (lst->size() == 0)
            return;
        unt = lst->data();
        for (size_t idx = lst->size(); idx != 0; idx--) {
            ntree_unit<T>*  child;
            child = static_cast<ntree_unit<T>*>(unt->ptr);
            if (child != NULL) {
                if (!run.doit(ctx, &child->user))
                    return;
            }
            unt += 1;
        }
        unt = lst->data();
        for (size_t idx = lst->size(); idx != 0; idx--) {
            ntree_unit<T>*  child;
            child = static_cast<ntree_unit<T>*>(unt->ptr);
            if (child != NULL)
                this->trav_bfs_int<TRUN>(ctx, child);
            unt += 1;
        }
    }

    /* ====================================================================== */
    template<class TRUN>void trav_dfs_int (void* ctx, ntree_unit<T>* node) const
    {
        TRUN                run;
        cnode_ptr*          unt;
        array<cnode_ptr>*   lst;

        if (!run.doit(ctx, &node->user))
            return;
        lst = &node->next;
        unt = lst->data();
        for (size_t idx = lst->size(); idx != 0; idx--) {
            ntree_unit<T>*  child;
            child = static_cast<ntree_unit<T>*>(unt->ptr);
            if (child != NULL)
                this->trav_dfs_int<TRUN>(ctx, child);
            unt += 1;
        }
    }
};

}   /* namespace */

#endif  /* __TREE_A_HPP__ */
