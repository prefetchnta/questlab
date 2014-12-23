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
class effect_i : public asylum
{
public:
    /* ================= */
    virtual ~effect_i () {}

public:
    /* ==================== */
    virtual void enter () = 0;

    /* ==================== */
    virtual void leave () = 0;
};

/*****************/
/* Material Port */
/*****************/
class material_i : public asylum
{
public:
    /* =================== */
    virtual ~material_i () {}

public:
    /* ===================== */
    virtual void commit () = 0;
};

}   /* namespace */

#endif  /* __ASYLUM3D_HPP__ */
