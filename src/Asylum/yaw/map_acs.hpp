/*
***************************************
*   Yet Another Wheel @ 2014-12-09
***************************************
*/

#ifndef __MAP_ACS_HPP__
#define __MAP_ACS_HPP__

/* Asylum Namespace */
namespace asy {

/**************/
/* MapACS Key */
/**************/
struct map_acs_key
{
#ifndef MAP_ACS_HASH_LEN
    int32u  hash;
#else
    byte_t  hash[MAP_ACS_HASH_LEN];
#endif
    const char* name;
};

/***************/
/* MapACS Unit */
/***************/
struct map_acs_unit
{
    size_t      idx;
    map_acs_key key;

    /* ====== */
    void free ()
    {
        mem_free(this->key.name);
    }
};

/**************/
/* MapACS Cmp */
/**************/
#ifndef map_acs_comp
    #define map_acs_comp(s1, s2)        strcmp(s1, s2)
#endif
#ifndef map_acs_hash
    #define map_acs_hash(hsh, str, len) (hsh) = hash_crc32i_total(str, len)
#endif
#ifndef map_acs_hash2index
    #define map_acs_hash2index(hsh)     (hsh)
#endif
#ifndef map_acs_hash_cmp
    #define map_acs_hash_cmp(h1, h2)    ((h1) != (h2))
#endif
class map_acs_cmp : public asylum
{
public:
    /* ======================== */
    size_t hash (map_acs_key* key)
    {
        map_acs_hash(key->hash, key->name, strlen(key->name));
        return ((size_t)map_acs_hash2index(key->hash));
    }

    /* ========================================== */
    bool match (map_acs_key* key, map_acs_unit* obj)
    {
        if (map_acs_hash_cmp(key->hash, obj->key.hash))
            return (false);
        if (map_acs_comp(key->name, obj->key.name) != 0)
            return (false);
        return (true);
    }
};

/*****************/
/* MapACS Rehash */
/*****************/
class map_acs_rehash : public asylum
{
public:
    /* ================================== */
    bool doit (void* ctx, map_acs_unit* obj)
    {
        table_c<map_acs_unit, map_acs_key, map_acs_cmp>*    tbl2;

        tbl2 = (table_c<map_acs_unit, map_acs_key, map_acs_cmp>*)ctx;
        tbl2->insert(&obj->key, obj);
        return (true);
    }
};

/**********/
/* MapACS */
/**********/
template<class T>
class map_acs : public asylum
{
private:
    size_t                                          m_cnt;
    array<T>                                        m_lst;
    table_c<map_acs_unit, map_acs_key, map_acs_cmp> m_tbl;

public:
    /* ====== */
    bool init ()
    {
        m_cnt = hash_count(0);
        if (!m_tbl.init(m_cnt))
            return (false);
        m_lst.init();
        if (!m_lst.reserve(m_cnt)) {
            m_tbl.free();
            return (false);
        }
        return (true);
    }

    /* ====== */
    void free ()
    {
        m_tbl.free();
        m_lst.free();
    }

public:
    /* ======= */
    void clear ()
    {
        m_tbl.clear();
        m_lst.clear();
    }

    /* ========== */
    T* data () const
    {
        return (m_lst.data());
    }

    /* ============== */
    size_t size () const
    {
        return (m_lst.size());
    }

    /* ================== */
    size_t capacity () const
    {
        return (m_cnt);
    }

    /* =================================== */
    T* get2 (size_t idx, T* obj = NULL) const
    {
        return (m_lst.get(idx, obj));
    }

    /* ======================================== */
    T* get2_safe (size_t idx, T* obj = NULL) const
    {
        return (m_lst.get_safe(idx, obj));
    }

    /* ======================================== */
    T* get (const char* name, T* obj = NULL) const
    {
        map_acs_key     key;
        map_acs_unit*   unt;

        key.name = name;
        unt = m_tbl.get(&key);
        if (unt != NULL)
            return (m_lst.get(unt->idx, obj));
        return (NULL);
    }

    /* ==================== */
    bool reserve (size_t size)
    {
        return (m_lst.reserve(size));
    }

    /* ========= */
    bool no_grow ()
    {
        return (m_lst.no_grow());
    }

    /* ============================== */
    T* insert (const char* name, T* obj)
    {
        T*  ret;

        ret = this->get(name);
        if (ret != NULL) {
            ret->free();
            mem_cpy(ret, obj, sizeof(T));
            return (ret);
        }

        ret = m_lst.append(obj);
        if (ret == NULL)
            return (NULL);

        size_t          cnt;
        map_acs_unit    tmp;

        tmp.key.name = str_dupA(name);
        if (tmp.key.name == NULL) {
            m_lst.pop(obj);
            return (NULL);
        }
        tmp.idx = m_lst.size() - 1;
        if (m_tbl.insert(&tmp.key, &tmp) != NULL)
            return (ret);

        table_c<map_acs_unit, map_acs_key, map_acs_cmp> tbl2;

        cnt = hash_count(m_cnt + 1);
        if (cnt == m_cnt || !tbl2.init(cnt)) {
            mem_free(tmp.key.name);
            m_lst.pop(obj);
            return (NULL);
        }
        m_tbl.traverse<map_acs_rehash>(&tbl2);
        m_tbl.replace(&tbl2);
        m_cnt = cnt;
        m_tbl.insert(&tmp.key, &tmp);
        return (ret);
    }
};

}   /* namespace */

#endif  /* __MAP_ACS_HPP__ */
