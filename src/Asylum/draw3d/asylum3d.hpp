/*
***************************************
*   Asylum3D @ 2014-12-17
***************************************
*/

#ifndef __ASYLUM3D_HPP__
#define __ASYLUM3D_HPP__

/* Asylum Namespace */
namespace asy {

/***********/
/* VertexN */
/***********/
struct vertex_n
{
    vec3d_t pos;
    vec3d_t nrm;
};

/************/
/* VertexT2 */
/************/
struct vertex_t2
{
    vec3d_t pos;
    vec2d_t tex;
};

/************/
/* VertexT3 */
/************/
struct vertex_t3
{
    vec3d_t pos;
    vec3d_t tex;
};

/*************/
/* VertexNT2 */
/*************/
struct vertex_nt2
{
    vec3d_t pos;
    vec3d_t nrm;
    vec2d_t tex;
};

/*************/
/* VertexNT3 */
/*************/
struct vertex_nt3
{
    vec3d_t pos;
    vec3d_t nrm;
    vec3d_t tex;
};

/***************/
/* Effect Port */
/***************/
class IEffect : public asylum
{
public:
    /* ================ */
    virtual ~IEffect () {}

public:
    /* ==================== */
    virtual void enter () = 0;

    /* ==================== */
    virtual void leave () = 0;

    /* ===================== */
    virtual void update () = 0;
};

/******************/
/* Attribute Port */
/******************/
class IAttrib : public asylum
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
#define ATTR_TYPE_TRANS     0x00000001ULL   // Alpha Blend
#define ATTR_TYPE_TEXTURE   0x00000002ULL   // Have Texture
#define ATTR_TYPE_NORMAL    0x00000004ULL   // Diffuse Light
#define ATTR_TYPE_SPECULAR  0x00000008ULL   // Specular Light
#define ATTR_TYPE_NRML_MAP  0x00000010ULL   // Normal Map
#define ATTR_TYPE_LGHT_MAP  0x00000020ULL   // Light Map

/*************/
/* Mesh Port */
/*************/
class IMesh : public asylum
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
        idx = 0;
        for (;;) {
            tmp = this->mesh[idx++];
            if (tmp == NULL)
                break;
            delete tmp;
        }
        mem_free(this->mesh);
    }
};

/***************/
/* Object Base */
/***************/
struct object_base
{
    sAABB               aabb;
    sSPHERE             ball;
    array<commit_batch> list;

    /* ====== */
    void free ()
    {
        this->list.free();
    }
};

/*******************/
/* Object Instance */
/*******************/
struct object_inst
{
    union {
        mat4x4_t    mat;
        struct {
            vec4d_t rotation;
            vec3d_t move, scale;
        } sep;
    } trans;

    sAABB   aabb;
    sSPHERE ball;

    /* ========= */
    void free () {}
};

/****************/
/* Object Group */
/****************/
struct object_group
{
    object_base*        base;
    array<object_inst>  inst;

    /* ====== */
    void free ()
    {
        this->inst.free();
    }
};

}   /* namespace */

#endif  /* __ASYLUM3D_HPP__ */
