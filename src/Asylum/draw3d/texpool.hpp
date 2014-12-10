/*
***************************************
*   Asylum3D @ 2014-12-09
***************************************
*/

#ifndef __TEXPOOL_HPP__
#define __TEXPOOL_HPP__

/* Asylum Namespace */
namespace asy {

/***************/
/* TexPool Key */
/***************/
struct texpool_key
{
    int32u      hash;
    const char* name;
};

/****************/
/* TexPool Unit */
/****************/
struct texpool_unit
{
    size_t      idx;
    texpool_key key;

    /* ====== */
    void free ()
    {
        mem_free(this->key.name);
    }
};

/***************/
/* TexPool Cmp */
/***************/
class texpool_cmp
{
public:
    /* ======================== */
    size_t hash (texpool_key* key)
    {
        key->hash = hash_crc32i_total(key->name, str_lenA(key->name));
        return ((size_t)key->hash);
    }

    /* ========================================== */
    bool match (texpool_key* key, texpool_unit* obj)
    {
        if (key->hash != obj->key.hash)
            return (false);
        if (str_cmpIA(key->name, obj->key.name) != 0)
            return (false);
        return (true);
    }
};

/******************/
/* TexPool Rehash */
/******************/
class texpool_rehash
{
public:
    /* ================================== */
    bool doit (void* ctx, texpool_unit* obj)
    {
        table_c<texpool_unit, texpool_key, texpool_cmp>*    tbl2;

        tbl2 = (table_c<texpool_unit, texpool_key, texpool_cmp>*)ctx;
        tbl2->insert(&obj->key, obj, false);
        return (true);
    }
};

/****************/
/* Texture Pool */
/****************/
template<class TTEX, class TA3D>
class texpool : public asylum
{
private:
    size_t                                          m_cnt;
    array<TTEX>                                     m_lst;
    table_c<texpool_unit, texpool_key, texpool_cmp> m_tbl;

public:
    /* ====== */
    bool init ()
    {
        m_cnt = hash_count(0);
        if (!m_tbl.init(m_cnt))
            return (false);
        m_lst.init();
        return (true);
    }

    /* ====== */
    void free ()
    {
        m_tbl.free();
        m_lst.free();
    }

public:
    /* ============== */
    size_t size () const
    {
        return (m_lst.size());
    }

    /* ======================= */
    TTEX* get2 (size_t idx) const
    {
        return (m_lst.get(idx));
    }

    /* =========================== */
    TTEX* get_safe (size_t idx) const
    {
        return (m_lst.get_safe(idx));
    }

    /* ============================ */
    TTEX* get (const char* name) const
    {
        texpool_key     key;
        texpool_unit*   unt;

        key.name = name;
        unt = m_tbl.get(&key);
        if (unt != NULL)
            return (m_lst.get(unt->idx));
        return (NULL);
    }

    /* ==================================================== */
    TTEX* get (const char* name, const char* type, TA3D *main)
    {
        texpool_key     key;
        texpool_unit*   unt;

        key.name = name;
        unt = m_tbl.get(&key);
        if (unt != NULL)
            return (m_lst.get(unt->idx));

        TTEX    tex, *ret;

        if (!tex.init(name, type, main))
            return (NULL);
        ret = m_lst.append(&tex);
        if (ret == NULL) {
            tex.free();
            return (NULL);
        }

        size_t          cnt;
        texpool_unit    tmp;

        key.name = str_dupA(name);
        if (key.name == NULL) {
            m_lst.pop();
            return (NULL);
        }
        mem_cpy(&tmp.key, &key, sizeof(key));
        tmp.idx = m_lst.size() - 1;
        if (m_tbl.insert(&key, &tmp, false) == NULL)
        {
            table_c<texpool_unit, texpool_key, texpool_cmp> tbl2;

            cnt = hash_count(m_cnt + 1);
            if (cnt == m_cnt || !tbl2.init(cnt)) {
                mem_free(key.name);
                m_lst.pop();
                return (NULL);
            }
            m_tbl.traverse<texpool_rehash>(&tbl2);
            m_tbl.setup(&tbl2);
            m_cnt = cnt;
            m_tbl.insert(&key, &tmp, false);
        }
        return (ret);
    }
};

}   /* namespace */

#endif  /* __TEXPOOL_HPP__ */
