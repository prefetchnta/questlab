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
    /* =============================== */
    virtual void enter (int64u type) = 0;

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
#define ATTR_TYPE_TRANS     0x00000001ULL
#define ATTR_TYPE_TEXTURE   0x00000002ULL
#define ATTR_TYPE_NORMAL    0x00000004ULL
#define ATTR_TYPE_SPECULAR  0x00000008ULL
#define ATTR_TYPE_NRML_MAP  0x00000010ULL

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

}   /* namespace */

#endif  /* __ASYLUM3D_HPP__ */
