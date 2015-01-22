/*
***************************************
*   Asylum3D @ 2014-12-17
***************************************
*/

#ifndef __ASYLUM3D_HPP__
#define __ASYLUM3D_HPP__

/* Asylum Namespace */
namespace asy {

/***************/
/* Effect Port */
/***************/
class IEffect : public asylum
{
private:
    int m_ref_cnt;

public:
    /* ==== */
    IEffect ()
    {
        m_ref_cnt = 0;
    }

    /* ============= */
    virtual ~IEffect ()
    {
    }

public:
    /* ====== */
    void free ()
    {
        m_ref_cnt -= 1;
        if (m_ref_cnt <= 0)
            delete this;
    }

    /* ========= */
    void add_ref ()
    {
        m_ref_cnt += 1;
    }

public:
    /* ==================== */
    virtual void enter () = 0;

    /* ==================== */
    virtual void leave () = 0;
};

/***************/
/* Commit Base */
/***************/
class commit_base : public asylum
{
protected:
    size_t  m_nv, m_nt;

public:
    /* ================== */
    size_t get_vnum () const
    {
        return (m_nv);
    }

    /* ================== */
    size_t get_tnum () const
    {
        return (m_nt);
    }
};

/******************/
/* Attribute Port */
/******************/
class IAttrib : public commit_base
{
protected:
    int64u  m_type;

public:
    /* ================ */
    virtual ~IAttrib () {}

public:
    /* ===================== */
    virtual void commit () = 0;

    /* ============== */
    int64u type () const
    {
        return (m_type);
    }
};

// Attribute Type
#define ATTR_TYPE_ALPHAOP   0x00000001ULL   // Alpha Blend
#define ATTR_TYPE_TEXTURE   0x00000002ULL   // Have Texture
#define ATTR_TYPE_SPECULAR  0x00000004ULL   // Specular Light
#define ATTR_TYPE_NRML_MAP  0x00000008ULL   // Normal Map
#define ATTR_TYPE_LGHT_MAP  0x00000010ULL   // Light Map

/*************/
/* Mesh Port */
/*************/
class IMesh : public commit_base
{
public:
    /* ============== */
    virtual ~IMesh () {}

    /* ===================== */
    virtual void commit () = 0;
};

/****************/
/* Commit Batch */
/****************/
struct commit_batch
{
    IMesh**     mesh;
    IAttrib*    attr;

    /* ====== */
    void free ()
    {
        IMesh*  tmp;
        size_t  idx;

        if (this->attr != NULL)
            delete this->attr;
        if (this->mesh != NULL) {
            for (idx = 0; ; idx++) {
                tmp = this->mesh[idx];
                if (tmp == NULL)
                    break;
                delete tmp;
            }
            mem_free(this->mesh);
        }
    }
};

/***************/
/* Object Base */
/***************/
struct object_inst;
struct object_base
{
    int                 type;
    void*               real;
    sAABB               aabb;
    sSPHERE             ball;
    array<commit_batch> list;
    void (*kill) (void* real);
    void (*tran) (object_inst* dest, void* param,
                  const vec3d_t* rote, const vec3d_t* move,
                  const vec3d_t* scale);
    /* ====== */
    void free ()
    {
        if (this->real != NULL &&
            this->kill != NULL)
            this->kill(this->real);
        this->list.free();
    }
};

/*******************/
/* Object Instance */
/*******************/
struct object_inst
{
    byte_t          type;   // Instance Type
    byte_t          flag;   // Instance Flags
    int16u          user;   // For User Use
    mat4x4_t        tran;   // World Matrix
    object_base*    base;   // To Base Object

    union {
        sAABB   aabb;
        sSPHERE ball;
    } bound;

    /* ========= */
    void free () {}
};

// Instance Type
#define INST_TYPE_STATIC    0
#define INST_TYPE_DYNAMIC   1

// Instance Flags
#define INST_FLAG_NORMAL        0
#define INST_FLAG_BILLBOARDV    1
#define INST_FLAG_BILLBOARDH    2

/***************/
/* Commit Unit */
/***************/
struct commit_unit
{
    object_inst*    inst;
    commit_batch*   unit;

    /* ========= */
    void free () {}
};

/***************/
/* Commit Pipe */
/***************/
struct commit_pipe
{
    IEffect*            effect;
    array<commit_unit>  stuffz;

    /* ====== */
    void free ()
    {
        if (this->effect != NULL)
            this->effect->free();
        this->stuffz.free();
    }
};

}   /* namespace */

/******************/
/* Effect Factory */
/******************/
CR_API asy::IEffect* create_asy3d_ffct_array (asy::IEffect** list, size_t count);

#endif  /* __ASYLUM3D_HPP__ */
