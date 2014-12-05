/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-12-04  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack WAVEFRONT 模型解析函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_WAVEFRONT_C__
#define __CR_WAVEFRONT_C__ 0x390259D6UL

#include "gfx3.h"
#include "datlib.h"
#include "parser.h"
#include "strlib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
---------------------------------------
    释放模型单元
---------------------------------------
*/
static void_t
wfront_g_free (
  __CR_IN__ void_t* obj
    )
{
    sWAVEFRONT_G*   unit;

    unit = (sWAVEFRONT_G*)obj;
    mem_free(unit->name);
    TRY_FREE(unit->mtl);
}

/*
---------------------------------------
    解析三维向量
---------------------------------------
*/
static bool_t
wfront_parse_v3d (
  __CR_OT__ vec3d_t*        v3d,
  __CR_IN__ const ansi_t*   str,
  __CR_IN__ bool_t          no_z
    )
{
    leng_t  skip;

    str = skip_spaceA(str);
    v3d->x = str2fp32A(str, &skip);
    if (skip == 0)
        return (FALSE);
    str += skip;
    if (!is_spaceA(*str++))
        return (FALSE);
    str = skip_spaceA(str);
    v3d->y = str2fp32A(str, &skip);
    if (skip == 0)
        return (FALSE);
    str = skip_spaceA(str + skip);
    v3d->z = str2fp32A(str, &skip);
    if (skip == 0)
        return (no_z);
    return (TRUE);
}

/*
---------------------------------------
    解析顶点索引
---------------------------------------
*/
static const ansi_t*
wfront_parse_vertex (
  __CR_OT__ sWAVEFRONT_F*   vx,
  __CR_IN__ const ansi_t*   str
    )
{
    uint_t  idx;
    leng_t  skip;

    vx->idx[1] = 0;
    vx->idx[2] = 0;
    for (idx = 0; idx < 3; idx++) {
        vx->idx[idx] = str2int32A(str, &skip);
        str = skip_spaceA(str + skip);
        if (*str != CR_AC('/'))
            break;
        str += 1;
        if (*str != CR_AC('/') && !is_digitA(*str))
            return (NULL);
    }
    return (skip_spaceA(str));
}

/*
---------------------------------------
    解析面索引
---------------------------------------
*/
static uint_t
wfront_parse_face (
  __CR_OT__ sWAVEFRONT_F*   vxs,
  __CR_IN__ const ansi_t*   str
    )
{
    uint_t          idx;
    uint_t          cnt = 0;
    sWAVEFRONT_F    ftmp[4];

    for (idx = 0; idx < 4; idx++) {
        str = wfront_parse_vertex(&ftmp[idx], str);
        if (str == NULL || ftmp[idx].idx[0] == 0)
            return (0);
        cnt += 1;
        if (!is_digitA(*str))
            break;
    }
    if (cnt != 3 && cnt != 4)
        return (0);
    mem_cpy(vxs, ftmp, sizeof(ftmp));
    if (cnt == 4) {
        mem_cpy(&vxs[4], &ftmp[0], sizeof(sWAVEFRONT_F));
        mem_cpy(&vxs[5], &ftmp[2], sizeof(sWAVEFRONT_F));
        return (6);
    }
    return (3);
}

/*
=======================================
    解析 OBJ 字符串
=======================================
*/
CR_API bool_t
wfront_obj_load (
  __CR_OT__ sWAVEFRONT*     obj,
  __CR_IN__ const ansi_t*   str,
  __CR_IN__ bool_t          swap_yz,
  __CR_IN__ bool_t          neg_z
    )
{
    fp32_t  tt;
    leng_t  idx;
    sINIu*  ini;
    uint_t  cnt, ii;
    sARRAY  a_v, a_vt, *aa;
    sARRAY  a_vn, a_f, a_g;
    /* ----------------- */
    vec3d_t         vtmp;
    sWAVEFRONT_G    gtmp;
    const ansi_t*   line;
    sWAVEFRONT_F    ff[6];

    /* 清空对象 */
    struct_zero(obj, sWAVEFRONT);
    struct_zero(&gtmp, sWAVEFRONT_G);

    /* 逐行分割 */
    ini = ini_parseU(str);
    if (ini == NULL) {
        err_set(__CR_WAVEFRONT_C__, CR_NULL,
                "wfront_obj_load()", "ini_parseU() failure");
        return (FALSE);
    }

    /* 初始化列表 */
    array_initT(&a_v, vec3d_t);
    array_initT(&a_vt, vec3d_t);
    array_initT(&a_vn, vec3d_t);
    array_initT(&a_f, sWAVEFRONT_F);
    array_initT(&a_g, sWAVEFRONT_G);
    a_g.free = wfront_g_free;

    /* 逐行解析 */
    for (idx = 0; idx < ini->count; idx++)
    {
        /* 跳过没用的行 */
        line = skip_spaceA(ini->lines[idx]);
        if (line[0] == CR_AC(NIL) || line[0] == CR_AC('#') ||
            line[0] == CR_AC('s') || line[0] == CR_AC('o'))
            continue;

        /* 顶点信息 */
        if (line[0] == CR_AC('v'))
        {
            if (line[1] == CR_AC('t'))
            {
                /* 非法的行 */
                if (!is_spaceA(line[2])) {
                    err_set(__CR_WAVEFRONT_C__, idx,
                            "wfront_obj_load()", "invalid <vt>");
                    goto _failure;
                }

                /* 贴图坐标 */
                if (!wfront_parse_v3d(&vtmp, line + 3, TRUE)) {
                    err_set(__CR_WAVEFRONT_C__, idx,
                            "wfront_obj_load()", "invalid <vt>");
                    goto _failure;
                }
                aa = &a_vt;
            }
            else
            if (line[1] == CR_AC('n'))
            {
                /* 非法的行 */
                if (!is_spaceA(line[2])) {
                    err_set(__CR_WAVEFRONT_C__, idx,
                            "wfront_obj_load()", "invalid <vn>");
                    goto _failure;
                }

                /* 法线向量 */
                if (!wfront_parse_v3d(&vtmp, line + 3, FALSE)) {
                    err_set(__CR_WAVEFRONT_C__, idx,
                            "wfront_obj_load()", "invalid <vn>");
                    goto _failure;
                }
                if (neg_z)
                    vtmp.z = -vtmp.z;
                if (swap_yz) {
                    CR_SWAP(vtmp.y, vtmp.z, tt);
                }
                tt = FSQRT(vtmp.x * vtmp.x + vtmp.y * vtmp.y +
                                    vtmp.z * vtmp.z);
                vtmp.x /= tt;
                vtmp.y /= tt;
                vtmp.z /= tt;
                aa = &a_vn;
            }
            else if (is_spaceA(line[1]))
            {
                /* 空间坐标 */
                if (!wfront_parse_v3d(&vtmp, line + 2, FALSE)) {
                    err_set(__CR_WAVEFRONT_C__, idx,
                            "wfront_obj_load()", "invalid <v>");
                    goto _failure;
                }
                if (neg_z)
                    vtmp.z = -vtmp.z;
                if (swap_yz) {
                    CR_SWAP(vtmp.y, vtmp.z, tt);
                }
                aa = &a_v;
            }
            else
            {
                /* 非法的行 */
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "invalid <v>");
                goto _failure;
            }

            /* 压入列表 */
            if (!array_push_growT(aa, vec3d_t, &vtmp) == NULL) {
                err_set(__CR_WAVEFRONT_C__, CR_NULL,
                        "wfront_obj_load()", "array_push_growT() failure");
                goto _failure;
            }
            continue;
        }

        /* 三角形或面 */
        if (line[0] == CR_AC('f'))
        {
            /* 非法的行 */
            if (!is_spaceA(line[1])) {
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "invalid <f>");
                goto _failure;
            }
            cnt = wfront_parse_face(ff, line + 2);
            if (cnt == 0) {
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "invalid <f>");
                goto _failure;
            }

            /* 压入列表 */
            for (ii = 0; ii < cnt; ii++) {
                if (ff[ii].idx[0] > array_get_sizeT(&a_v, vec3d_t)) {
                    err_set(__CR_WAVEFRONT_C__, idx,
                            "wfront_obj_load()", "invalid <f>");
                    goto _failure;
                }
                if (ff[ii].idx[1] > array_get_sizeT(&a_vt, vec3d_t)) {
                    err_set(__CR_WAVEFRONT_C__, idx,
                            "wfront_obj_load()", "invalid <f>");
                    goto _failure;
                }
                if (ff[ii].idx[2] > array_get_sizeT(&a_vn, vec3d_t)) {
                    err_set(__CR_WAVEFRONT_C__, idx,
                            "wfront_obj_load()", "invalid <f>");
                    goto _failure;
                }
                if (!array_push_growT(&a_f, sWAVEFRONT_F, &ff[ii]) == NULL) {
                    err_set(__CR_WAVEFRONT_C__, CR_NULL,
                            "wfront_obj_load()", "array_push_growT() failure");
                    goto _failure;
                }
            }
            continue;
        }

        /* 模型开始 */
        if (line[0] == CR_AC('g'))
        {
            /* 非法的行 */
            if (!is_spaceA(line[1])) {
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "invalid <g>");
                goto _failure;
            }

            /* 压入上个模型 */
            gtmp.end = array_get_sizeT(&a_f, sWAVEFRONT_F);
            if (gtmp.beg < gtmp.end && gtmp.name != NULL) {
                if (!array_push_growT(&a_g, sWAVEFRONT_G, &gtmp) == NULL) {
                    err_set(__CR_WAVEFRONT_C__, CR_NULL,
                            "wfront_obj_load()", "array_push_growT() failure");
                    goto _failure;
                }
                gtmp.name = NULL;
                gtmp.mtl = NULL;
            }

            /* 填充模型参数 */
            line = skip_spaceA(line + 2);
            if (*line == CR_AC('#') || str_lenA(line) == 0) {
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "invalid <g>");
                goto _failure;
            }
            TRY_FREE(gtmp.name);
            gtmp.name = str_dupA(line);
            if (gtmp.name == NULL) {
                err_set(__CR_WAVEFRONT_C__, CR_NULL,
                        "wfront_obj_load()", "str_dupA() failure");
                goto _failure;
            }
            str_trimRA(gtmp.name);
            gtmp.beg = gtmp.end;
            SAFE_FREE(gtmp.mtl);
            continue;
        }

        /* 模型材质 */
        if (mem_cmp(line, "usemtl", 6) == 0) {
            if (!is_spaceA(line[6])) {
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "invalid <usemtl>");
                goto _failure;
            }
            if (gtmp.name == NULL) {
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "invalid <usemtl>");
                goto _failure;
            }
            if (gtmp.mtl != NULL) {
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "repeat <usemtl>");
                goto _failure;
            }
            line = skip_spaceA(line + 7);
            if (*line == CR_AC('#') || str_lenA(line) == 0) {
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "invalid <usemtl>");
                goto _failure;
            }
            gtmp.mtl = str_dupA(line);
            if (gtmp.mtl == NULL) {
                err_set(__CR_WAVEFRONT_C__, CR_NULL,
                        "wfront_obj_load()", "str_dupA() failure");
                goto _failure;
            }
            str_trimRA(gtmp.mtl);
            continue;
        }

        /* 材质文件 */
        if (mem_cmp(line, "mtllib", 6) == 0) {
            if (!is_spaceA(line[6])) {
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "invalid <mtllib>");
                goto _failure;
            }
            if (obj->mtl != NULL) {
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "repeat <mtllib>");
                goto _failure;
            }
            line = skip_spaceA(line + 7);
            if (*line == CR_AC('#') || str_lenA(line) == 0) {
                err_set(__CR_WAVEFRONT_C__, idx,
                        "wfront_obj_load()", "invalid <mtllib>");
                goto _failure;
            }
            obj->mtl = str_dupA(line);
            if (obj->mtl == NULL) {
                err_set(__CR_WAVEFRONT_C__, CR_NULL,
                        "wfront_obj_load()", "str_dupA() failure");
                goto _failure;
            }
            str_trimRA(obj->mtl);
            continue;
        }

        /* 非法的行 */
        err_set(__CR_WAVEFRONT_C__, idx,
                "wfront_obj_load()", "invalid OBJ format");
        goto _failure;
    }

    /* 压入最后一个模型 */
    gtmp.end = array_get_sizeT(&a_f, sWAVEFRONT_F);
    if (gtmp.beg < gtmp.end && gtmp.name != NULL) {
        if (!array_push_growT(&a_g, sWAVEFRONT_G, &gtmp) == NULL) {
            err_set(__CR_WAVEFRONT_C__, CR_NULL,
                    "wfront_obj_load()", "array_push_growT() failure");
            goto _failure;
        }
        gtmp.name = NULL;
        gtmp.mtl = NULL;
    }
    else {
        SAFE_FREE(gtmp.name);
        SAFE_FREE(gtmp.mtl);
    }

    /* 固定缓冲大小 */
    if (!array_no_growT(&a_v, vec3d_t)) {
        err_set(__CR_WAVEFRONT_C__, FALSE,
                "wfront_obj_load()", "array_no_growT() failure");
        goto _failure;
    }
    if (!array_no_growT(&a_vt, vec3d_t)) {
        err_set(__CR_WAVEFRONT_C__, FALSE,
                "wfront_obj_load()", "array_no_growT() failure");
        goto _failure;
    }
    if (!array_no_growT(&a_vn, vec3d_t)) {
        err_set(__CR_WAVEFRONT_C__, FALSE,
                "wfront_obj_load()", "array_no_growT() failure");
        goto _failure;
    }
    if (!array_no_growT(&a_f, sWAVEFRONT_F)) {
        err_set(__CR_WAVEFRONT_C__, FALSE,
                "wfront_obj_load()", "array_no_growT() failure");
        goto _failure;
    }
    if (!array_no_growT(&a_g, sWAVEFRONT_G)) {
        err_set(__CR_WAVEFRONT_C__, FALSE,
                "wfront_obj_load()", "array_no_growT() failure");
        goto _failure;
    }

    /* 返回读取的数据 */
    obj->n_v = array_get_sizeT(&a_v, vec3d_t);
    obj->p_v = array_get_dataT(&a_v, vec3d_t);
    obj->n_vt = array_get_sizeT(&a_vt, vec3d_t);
    obj->p_vt = array_get_dataT(&a_vt, vec3d_t);
    obj->n_vn = array_get_sizeT(&a_vn, vec3d_t);
    obj->p_vn = array_get_dataT(&a_vn, vec3d_t);
    obj->n_f = array_get_sizeT(&a_f, sWAVEFRONT_F);
    obj->p_f = array_get_dataT(&a_f, sWAVEFRONT_F);
    obj->n_g = array_get_sizeT(&a_g, sWAVEFRONT_G);
    obj->p_g = array_get_dataT(&a_g, sWAVEFRONT_G);

    /* 必须要有的数据 */
    if (obj->n_v == 0 || obj->p_v == NULL ||
        obj->n_f == 0 || obj->p_f == NULL ||
        obj->n_g == 0 || obj->p_g == NULL) {
        err_set(__CR_WAVEFRONT_C__, CR_ERROR,
                "wfront_obj_load()", "invalid OBJ format");
        goto _failure;
    }
    return (TRUE);

_failure:
    array_freeT(&a_g, sWAVEFRONT_G);
    array_freeT(&a_f, sWAVEFRONT_F);
    array_freeT(&a_vn, vec3d_t);
    array_freeT(&a_vt, vec3d_t);
    array_freeT(&a_v, vec3d_t);
    TRY_FREE(gtmp.name);
    TRY_FREE(gtmp.mtl);
    TRY_FREE(obj->mtl);
    return (FALSE);
}

/*
=======================================
    释放 OBJ 对象
=======================================
*/
CR_API void_t
wfront_obj_free (
  __CR_IN__ const sWAVEFRONT*   obj
    )
{
    leng_t  idx;

    TRY_FREE(obj->mtl);
    mem_free(obj->p_v);
    TRY_FREE(obj->p_vt);
    TRY_FREE(obj->p_vn);
    mem_free(obj->p_f);
    for (idx = 0; idx < obj->n_g; idx++) {
        mem_free(obj->p_g[idx].name);
        TRY_FREE(obj->p_g[idx].mtl);
    }
    mem_free(obj->p_g);
    if (obj->p_m != NULL) {
        for (idx = 0; idx < obj->n_m; idx++) {
            mem_free(obj->p_m[idx].name);
            TRY_FREE(obj->p_m[idx].map_ka);
            TRY_FREE(obj->p_m[idx].map_kd);
            TRY_FREE(obj->p_m[idx].map_ks);
            TRY_FREE(obj->p_m[idx].map_d);
            TRY_FREE(obj->p_m[idx].map_ns);
            TRY_FREE(obj->p_m[idx].decal);
            TRY_FREE(obj->p_m[idx].disp);
            TRY_FREE(obj->p_m[idx].bump);
        }
        mem_free(obj->p_m);
    }
}

#endif  /* !__CR_WAVEFRONT_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
