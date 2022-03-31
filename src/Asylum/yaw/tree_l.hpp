/*
***************************************
*   Yet Another Wheel @ 2014-02-09
***************************************
*/

#ifndef __TREE_L_HPP__
#define __TREE_L_HPP__

/* Asylum Namespace */
namespace asy {

/*************************/
/* Tree Unit (Use List2) */
/*************************/
template<class T>
struct atree_unit
{
    T                       user;
    size_t                  deep;
    atree_unit<T>*          uppe;
    list2<cnode_ptr>        next;
    list2_unit<cnode_ptr>*  node;
};

/********************/
/* Tree (Use List2) */
/********************/
template<class T>
class tree_l : public asylum
{
private:
    size_t          m_cnts;
    atree_unit<T>*  m_root;

public:
    /* ================== */
    bool init (const T* obj)
    {
        m_root = struct_new(atree_unit<T>);
        if (m_root == NULL)
            return (false);
        m_cnts = 1;
        m_root->node = NULL;
        m_root->uppe = NULL;
        m_root->deep = 0;
        mem_cpy(&m_root->user, obj, sizeof(T));
        m_root->next.init();
        return (true);
    }

    /* ====== */
    void free ()
    {
        this->del(m_root);
    }

public:
    /* ====================== */
    atree_unit<T>* root () const
    {
        return (m_root);
    }

    /* ============== */
    size_t size () const
    {
        return (m_cnts);
    }

    /* ====================================== */
    size_t child_num (atree_unit<T>* node) const
    {
        return (node->next.size());
    }

    /* =================================================================== */
    atree_unit<T>* get (atree_unit<T>* node, size_t idx, T* obj = NULL) const
    {
        atree_unit<T>*          dat;
        list2_unit<cnode_ptr>*  unt;

        unt = node->next.get(idx);
        dat = static_cast<atree_unit<T>*>(unt->user.ptr);
        if (obj != NULL)
            mem_cpy(obj, &dat->user, sizeof(T));
        return (dat);
    }

    /* ======================================================================== */
    atree_unit<T>* get_safe (atree_unit<T>* node, size_t idx, T* obj = NULL) const
    {
        if (idx >= node->next.size())
            return (NULL);
        return (this->get(node, idx, obj));
    }

    /* =========================================== */
    atree_unit<T>* parent (atree_unit<T>* node) const
    {
        return (node->uppe);
    }

    /* ========================================= */
    atree_unit<T>* prev (atree_unit<T>* node) const
    {
        list2_unit<cnode_ptr>*  unt = node->node->prev;

        if (unt != NULL)
            return (static_cast<atree_unit<T>*>(unt->user.ptr));
        return (NULL);
    }

    /* ========================================= */
    atree_unit<T>* next (atree_unit<T>* node) const
    {
        list2_unit<cnode_ptr>*  unt = node->node->next;

        if (unt != NULL)
            return (static_cast<atree_unit<T>*>(unt->user.ptr));
        return (NULL);
    }

    /* ======================== */
    void del (atree_unit<T>* node)
    {
        if (m_cnts != 0)
        {
            list2<cnode_ptr>*       lst;
            list2_unit<cnode_ptr>*  unt;

            lst = &node->next;
            unt = lst->tail();
            for (size_t idx = lst->size(); idx != 0; idx--) {
                atree_unit<T>*  child;
                child = static_cast<atree_unit<T>*>(unt->user.ptr);
                unt = unt->prev;
                this->del(child);
            }
            node->user.free();
            if (node->uppe != NULL) {
                lst = &node->uppe->next;
                lst->del(node->node);
            }
            m_cnts -= 1;
            mem_free(node);
        }
    }

    /* =============================================================== */
    atree_unit<T>* insert (atree_unit<T>* node, const T* obj, size_t idx)
    {
        cnode_ptr               next;
        atree_unit<T>*          nnew;
        list2<cnode_ptr>*       list;
        list2_unit<cnode_ptr>*  unit;

        nnew = struct_new(atree_unit<T>);
        if (nnew == NULL)
            return (NULL);
        next.ptr = (void*)nnew;
        list = &node->next;
        if (idx >= list->size()) {
            unit = list->append(&next);
        }
        else
        if (idx == 0) {
            unit = list->sthead(&next);
        }
        else {
            unit = list->get(idx);
            unit = list->insert(unit, &next);
        }
        if (unit == NULL) {
            mem_free(nnew);
            return (NULL);
        }
        m_cnts += 1;
        nnew->node = unit;
        nnew->uppe = node;
        nnew->deep = node->deep + 1;
        mem_cpy(&nnew->user, obj, sizeof(T));
        nnew->next.init();
        return (nnew);
    }

    /* =================================================== */
    atree_unit<T>* sthead (atree_unit<T>* node, const T* obj)
    {
        return (this->insert(node, obj, 0));
    }

    /* =================================================== */
    atree_unit<T>* append (atree_unit<T>* node, const T* obj)
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
            this->trav_dfs_int<TRUN>(ctx, m_root);
    }

private:
    /* ====================================================================== */
    template<class TRUN>void trav_bfs_int (void* ctx, atree_unit<T>* node) const
    {
        TRUN                    run;
        list2<cnode_ptr>*       lst;
        list2_unit<cnode_ptr>*  unt;

        lst = &node->next;
        if (lst->size() == 0)
            return;
        unt = lst->head();
        for (size_t idx = lst->size(); idx != 0; idx--) {
            atree_unit<T>*  child;
            child = static_cast<atree_unit<T>*>(unt->user.ptr);
            if (!run.doit(ctx, &child->user))
                return;
            unt = unt->next;
        }
        unt = lst->head();
        for (size_t idx = lst->size(); idx != 0; idx--) {
            atree_unit<T>*  child;
            child = static_cast<atree_unit<T>*>(unt->user.ptr);
            this->trav_bfs_int<TRUN>(ctx, child);
            unt = unt->next;
        }
        run.back(ctx, &node->user);
    }

    /* ====================================================================== */
    template<class TRUN>void trav_dfs_int (void* ctx, atree_unit<T>* node) const
    {
        TRUN                    run;
        list2<cnode_ptr>*       lst;
        list2_unit<cnode_ptr>*  unt;

        if (!run.doit(ctx, &node->user))
            return;
        lst = &node->next;
        unt = lst->head();
        for (size_t idx = lst->size(); idx != 0; idx--) {
            atree_unit<T>*  child;
            child = static_cast<atree_unit<T>*>(unt->user.ptr);
            this->trav_dfs_int<TRUN>(ctx, child);
            unt = unt->next;
        }
        run.back(ctx, &node->user);
    }
};

}   /* namespace */

#endif  /* __TREE_L_HPP__ */
