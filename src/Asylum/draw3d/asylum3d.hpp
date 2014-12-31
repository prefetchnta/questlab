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
public:
    /* ================ */
    virtual ~IEffect () {}

public:
    /* ==================== */
    virtual void enter () = 0;

    /* ==================== */
    virtual void leave () = 0;
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
#define ATTR_TYPE_ALPHAOP   0x00000001ULL   // Alpha Blend
#define ATTR_TYPE_TEXTURE   0x00000002ULL   // Have Texture
#define ATTR_TYPE_SPECULAR  0x00000004ULL   // Specular Light
#define ATTR_TYPE_NRML_MAP  0x00000008ULL   // Normal Map
#define ATTR_TYPE_LGHT_MAP  0x00000010ULL   // Light Map

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
struct object_base
{
    void*               real;
    sAABB               aabb;
    sSPHERE             ball;
    array<commit_batch> list;
    void (*kill) (void* real);

    /* ====== */
    void free ()
    {
        this->list.free();
        if (this->real != NULL &&
            this->kill != NULL)
            this->kill(this->real);
    }
};

/*******************/
/* Object Instance */
/*******************/
struct object_inst
{
    object_base*    base;

    union {
        mat4x4_t    mat;
        struct {
            vec3d_t rote;
            vec3d_t move;
            vec3d_t scale;
        } sep;
    } trans;

    union {
        sAABB   aabb;
        sSPHERE ball;
    } bound;

    /* ========= */
    void free () {}
};

/***************/
/* Commit Pipe */
/***************/
struct commit_pipe
{
    IEffect*            effect;
    array<cnode_ptr>    stuffz;     // <commit_batch*>

    void free ()
    {
        this->stuffz.free();
        delete this->effect;
    }
};

}   /* namespace */

#endif  /* __ASYLUM3D_HPP__ */
