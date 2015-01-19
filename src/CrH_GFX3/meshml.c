/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2015-01-19  */
/*     #######          ###    ###      [GFX3]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>>> CrHack MESHML 模型解析函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_MESHML_C__
#define __CR_MESHML_C__ 0x1D90C33DUL

#include "gfx3.h"
#include "datlib.h"
#include "parser.h"
#include "strlib.h"

/*
---------------------------------------
    释放模型单元
---------------------------------------
*/
static void_t
meshml_g_free (
  __CR_IN__ void_t* obj
    )
{
    sMESHML_MESH*   unit;

    unit = (sMESHML_MESH*)obj;
    mem_free(unit->name);
    mem_free(unit->ibuf);
}

/*
---------------------------------------
    释放材质单元
---------------------------------------
*/
static void_t
meshml_m_free (
  __CR_IN__ void_t* obj
    )
{
    sMESHML_MTRL*   unit;

    unit = (sMESHML_MTRL*)obj;
    TRY_FREE(unit->map_kd);
    TRY_FREE(unit->map_d);
    TRY_FREE(unit->bump);
}

/*
---------------------------------------
    释放骨骼单元
---------------------------------------
*/
static void_t
meshml_b_free (
  __CR_IN__ void_t* obj
    )
{
    sMESHML_BONE*   unit;

    unit = (sMESHML_BONE*)obj;
    mem_free(unit->name);
}

/*
---------------------------------------
    解析浮点向量
---------------------------------------
*/
static uint_t
meshml_parse_vecf (
  __CR_OT__ fp32_t*         vec,
  __CR_IN__ const ansi_t*   str,
  __CR_IN__ uint_t          count
    )
{
    uint_t  idx;
    leng_t  skip;

    for (idx = 0; idx < count; idx++, vec++) {
        str = skip_spaceA(str);
        *vec = str2fp32A(str, &skip);
        if (skip == 0)
            break;
        str += skip;
    }
    return (idx);
}

/*
---------------------------------------
    解析整数向量
---------------------------------------
*/
static uint_t
meshml_parse_veci (
  __CR_OT__ int32u*         vec,
  __CR_IN__ const ansi_t*   str,
  __CR_IN__ uint_t          count
    )
{
    uint_t  idx;
    leng_t  skip;

    for (idx = 0; idx < count; idx++, vec++) {
        str = skip_spaceA(str);
        *vec = str2int32A(str, &skip);
        if (skip == 0)
            break;
        str += skip;
    }
    return (idx);
}

/* 标签类型 */
#define MESHML_TAG_NONE 0
#define MESHML_TAG_BONE 1
#define MESHML_TAG_MESH 2
#define MESHML_TAG_MTRL 3
#define MESHML_TAG_DONE 9

/*
=======================================
    解析 MESHML 字符串
=======================================
*/
CR_API bool_t
meshml_load (
  __CR_OT__ sMESHML*        msh,
  __CR_IN__ const ansi_t*   str
    )
{
    sXMLu*  xml;
    leng_t  idx;
    uint_t  type;
    ansi_t* name;
    ansi_t* value;
    sARRAY  a_g, a_m, a_b;
    sARRAY  a_v, a_vt, a_vn;
    /* ------------------ */
    vec2d_t         vtp2;
    vec3d_t         vtp3;
    vec4d_t         vtp4;
    sXNODEu*        node;
    sMESHML_MESH    gtmp;
    sMESHML_MTRL    mtmp;
    sMESHML_BONE    btmp;

    /* 解析 XML 字符串 */
    xml = xml_parseU(str);
    if (xml == NULL) {
        err_set(__CR_MESHML_C__, CR_NULL,
                "meshml_load()", "xml_parseU() failure");
        return (FALSE);
    }

    /* 初始化列表 */
    array_initT(&a_v, vec3d_t);
    array_initT(&a_vt, vec2d_t);
    array_initT(&a_vn, vec4d_t);
    array_initT(&a_g, sMESHML_MESH);
    array_initT(&a_m, sMESHML_MTRL);
    array_initT(&a_b, sMESHML_BONE);
    a_g.free = meshml_g_free;
    a_m.free = meshml_m_free;
    a_b.free = meshml_b_free;

    /* 清空对象 */
    struct_zero(msh, sMESHML);
    struct_zero(&gtmp, sMESHML_MESH);
    struct_zero(&mtmp, sMESHML_MTRL);
    struct_zero(&btmp, sMESHML_BONE);

    /* 检查首标签 */
    if (xml->nodes[0].name[0] == CR_AC('?'))
        idx = 1;
    else
        idx = 0;
    if (xml->nodes[idx].closed ||
        str_cmpIA(xml->nodes[idx].name, CR_AS("model")) != 0) {
        err_set(__CR_MESHML_C__, CR_ERROR,
                "meshml_load()", "invalid XML root tag");
        goto _failure;
    }
    type = MESHML_TAG_NONE;
    msh->ver = xml_attr_intxU(CR_AS("version"), 0, &xml->nodes[idx]);

    /* 逐行标签解析 */
    for (idx++; idx < xml->count; idx++)
    {
        /* 结束解析 */
        node = &xml->nodes[idx];
        if (str_cmpIA(node->name, CR_AS("/model")) == 0) {
            if (type == MESHML_TAG_NONE)
                type = MESHML_TAG_DONE;
            break;
        }

        /* 解析数据标签 */
        if (type == MESHML_TAG_BONE)
        {
            /* 解析骨骼数据 */
            if (str_cmpIA(node->name, CR_AS("/bones_chunk")) == 0) {
                if (btmp.name != NULL)
                    break;
                type = MESHML_TAG_NONE;
                continue;
            }
            if (str_cmpIA(node->name, CR_AS("bone")) == 0) {
                if (btmp.name != NULL)
                    break;
                btmp.name = xml_attr_stringU(CR_AS("name"), node);
                if (btmp.name == NULL)
                    break;
                btmp.parent = xml_attr_intxU(CR_AS("parent"), 0, node);
                if (!node->found)
                    break;
                continue;
            }
            if (btmp.name != NULL) {
                if (str_cmpIA(node->name, CR_AS("real")) == 0) {
                    value = xml_attr_bufferU(CR_AS("v"), node);
                    if (value == NULL ||
                        meshml_parse_vecf(&btmp.real.x, value, 4) != 4)
                        break;
                    continue;
                }
                if (str_cmpIA(node->name, CR_AS("dual")) == 0) {
                    value = xml_attr_bufferU(CR_AS("v"), node);
                    if (value == NULL ||
                        meshml_parse_vecf(&btmp.dual.x, value, 4) != 4)
                        break;
                    continue;
                }
                /* 压入骨骼列表 */
                if (str_cmpIA(node->name, CR_AS("/bone")) == 0) {
                    if (array_push_growT(&a_b, sMESHML_BONE, &btmp) == NULL) {
                        err_set(__CR_MESHML_C__, CR_NULL,
                                "meshml_load()", "array_push_growT() failure");
                        goto _failure;
                    }
                    struct_zero(&btmp, sMESHML_BONE);
                    continue;
                }
            }
        }
        else
        if (type == MESHML_TAG_MTRL)
        {
            /* 解析材质数据 */
            if (str_cmpIA(node->name, CR_AS("/materials_chunk")) == 0) {
                if (mtmp.flags)
                    break;
                type = MESHML_TAG_NONE;
                continue;
            }
            if (str_cmpIA(node->name, CR_AS("material")) == 0) {
                value = xml_attr_bufferU(CR_AS("ambient"), node);
                if (value == NULL) {
                    struct_zero(&mtmp.ka, vec3d_t);
                }
                else {
                    if (meshml_parse_vecf(&mtmp.ka.x, value, 3) != 3)
                        break;
                    mtmp.flags |= MESHML_KA;
                }
                value = xml_attr_bufferU(CR_AS("diffuse"), node);
                if (value == NULL) {
                    struct_zero(&mtmp.kd, vec3d_t);
                }
                else {
                    if (meshml_parse_vecf(&mtmp.kd.x, value, 3) != 3)
                        break;
                    mtmp.flags |= MESHML_KD;
                }
                value = xml_attr_bufferU(CR_AS("specular"), node);
                if (value == NULL) {
                    struct_zero(&mtmp.ks, vec3d_t);
                }
                else {
                    if (meshml_parse_vecf(&mtmp.ks.x, value, 3) != 3)
                        break;
                    mtmp.flags |= MESHML_KS;
                }
                value = xml_attr_bufferU(CR_AS("emit"), node);
                if (value == NULL)
                    struct_zero(&mtmp.ke, vec3d_t);
                else
                if (meshml_parse_vecf(&mtmp.ke.x, value, 3) != 3)
                    break;
                mtmp.opacity = xml_attr_fp32U(CR_AS("opacity"),1,node);
                mtmp.splevel = xml_attr_fp32U(CR_AS("specular_level"),0,node);
                mtmp.shininess = xml_attr_fp32U(CR_AS("shininess"),0,node);
                mtmp.flags |= MESHML_SET;
                continue;
            }
            if (mtmp.flags) {
                if (str_cmpIA(node->name, CR_AS("texture")) == 0) {
                    value = xml_attr_bufferU(CR_AS("type"), node);
                    if (value == NULL)
                        break;
                    name = xml_attr_stringU(CR_AS("name"), node);
                    if (name == NULL)
                        break;
                    if (mem_cmp(value, "Diffuse Color\"", 14) == 0)
                        mtmp.map_kd = name;
                    else if (mem_cmp(value, "Opacity\"", 8) == 0)
                        mtmp.map_d = name;
                    else if (mem_cmp(value, "Bump\"", 5) == 0)
                        mtmp.bump = name;
                    else
                        mem_free(name);
                    continue;
                }
                /* 压入材质列表 */
                if (str_cmpIA(node->name, CR_AS("/material")) == 0) {
                    if (array_push_growT(&a_m, sMESHML_MTRL, &mtmp) == NULL) {
                        err_set(__CR_MESHML_C__, CR_NULL,
                                "meshml_load()", "array_push_growT() failure");
                        goto _failure;
                    }
                    struct_zero(&mtmp, sMESHML_MTRL);
                    continue;
                }
            }
        }
        else
        if (type == MESHML_TAG_MESH)
        {
            /* 解析网格数据 */
            if (str_cmpIA(node->name, CR_AS("/meshes_chunk")) == 0) {
                type = MESHML_TAG_NONE;
                continue;
            }
        }
        else
        {
            /* 数据块标签的开始 */
            if (str_cmpIA(node->name, CR_AS("bones_chunk")) == 0)
            {
                /* 骨骼数据开始 */
                if (type != MESHML_TAG_NONE)
                    break;
                type = MESHML_TAG_BONE;
                continue;
            }
            if (str_cmpIA(node->name, CR_AS("materials_chunk")) == 0)
            {
                /* 材质数据开始 */
                if (type != MESHML_TAG_NONE)
                    break;
                type = MESHML_TAG_MTRL;
                continue;
            }
            if (str_cmpIA(node->name, CR_AS("meshes_chunk")) == 0)
            {
                /* 网格数据开始 */
                if (type != MESHML_TAG_NONE)
                    break;
                type = MESHML_TAG_MESH;
                continue;
            }
        }

        /* 跳过无效的标签 */
        idx = xml_skip_tagU(xml->nodes, idx, xml->count) - 1;
    }

    /* 节点必须闭合 */
    if (type != MESHML_TAG_DONE) {
        err_set(__CR_MESHML_C__, type,
                "meshml_load()", "invalid MESHML format");
        goto _failure;
    }

    /* 固定缓冲大小 */
    if (!array_no_growT(&a_v, vec3d_t)) {
        err_set(__CR_MESHML_C__, FALSE,
                "meshml_load()", "array_no_growT() failure");
        goto _failure;
    }
    if (!array_no_growT(&a_vt, vec2d_t)) {
        err_set(__CR_MESHML_C__, FALSE,
                "meshml_load()", "array_no_growT() failure");
        goto _failure;
    }
    if (!array_no_growT(&a_vn, vec4d_t)) {
        err_set(__CR_MESHML_C__, FALSE,
                "meshml_load()", "array_no_growT() failure");
        goto _failure;
    }
    if (!array_no_growT(&a_g, sMESHML_MESH)) {
        err_set(__CR_MESHML_C__, FALSE,
                "meshml_load()", "array_no_growT() failure");
        goto _failure;
    }
    if (!array_no_growT(&a_m, sMESHML_MTRL)) {
        err_set(__CR_MESHML_C__, FALSE,
                "meshml_load()", "array_no_growT() failure");
        goto _failure;
    }
    if (!array_no_growT(&a_b, sMESHML_BONE)) {
        err_set(__CR_MESHML_C__, FALSE,
                "meshml_load()", "array_no_growT() failure");
        goto _failure;
    }

    /* 返回读取的数据 */
    msh->n_v = array_get_sizeT(&a_v, vec3d_t);
    msh->p_v = array_get_dataT(&a_v, vec3d_t);
    msh->p_vt = array_get_dataT(&a_vt, vec2d_t);
    msh->p_vn = array_get_dataT(&a_vn, vec4d_t);
    msh->n_g = array_get_sizeT(&a_g, sMESHML_MESH);
    msh->p_g = array_get_dataT(&a_g, sMESHML_MESH);
    msh->n_m = array_get_sizeT(&a_m, sMESHML_MTRL);
    msh->p_m = array_get_dataT(&a_m, sMESHML_MTRL);
    msh->n_b = array_get_sizeT(&a_b, sMESHML_BONE);
    msh->p_b = array_get_dataT(&a_b, sMESHML_BONE);

    /* 必须要有的数据 */
    if (msh->n_v == 0 || msh->p_v == NULL ||
        msh->n_g == 0 || msh->p_g == NULL ||
        msh->n_m == 0 || msh->p_m == NULL) {
        err_set(__CR_MESHML_C__, CR_NULL,
                "meshml_load()", "invalid MESHML format");
        goto _failure;
    }
    xml_closeU(xml);
    return (TRUE);

_failure:
    array_freeT(&a_b, sMESHML_BONE);
    array_freeT(&a_m, sMESHML_MTRL);
    array_freeT(&a_g, sMESHML_MESH);
    array_freeT(&a_vn, vec4d_t);
    array_freeT(&a_vt, vec2d_t);
    array_freeT(&a_v, vec3d_t);
    TRY_FREE(gtmp.name);
    TRY_FREE(gtmp.ibuf);
    TRY_FREE(mtmp.map_kd);
    TRY_FREE(mtmp.map_d);
    TRY_FREE(mtmp.bump);
    TRY_FREE(btmp.name);
    xml_closeU(xml);
    return (FALSE);
}

/*
=======================================
    释放 MESHML 对象
=======================================
*/
CR_API void_t
meshml_free (
  __CR_IN__ const sMESHML*  msh
    )
{
    leng_t  idx;

    mem_free(msh->p_v);
    TRY_FREE(msh->p_vn);
    TRY_FREE(msh->p_vt);
    for (idx = 0; idx < msh->n_g; idx++)
        meshml_g_free(&msh->p_g[idx]);
    mem_free(msh->p_g);
    for (idx = 0; idx < msh->n_m; idx++)
        meshml_m_free(&msh->p_m[idx]);
    mem_free(msh->p_m);
    if (msh->p_b != NULL) {
        for (idx = 0; idx < msh->n_b; idx++)
            meshml_b_free(&msh->p_b[idx]);
        mem_free(msh->p_b);
    }
}

#endif  /* !__CR_MESHML_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
