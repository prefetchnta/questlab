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

#include "gfx3.h"
#include "datlib.h"
#include "msclib.h"
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
    释放材质单元
---------------------------------------
*/
static void_t
wfront_m_free (
  __CR_IN__ void_t* obj
    )
{
    sWAVEFRONT_M*   unit;

    unit = (sWAVEFRONT_M*)obj;
    mem_free(unit->name);
    TRY_FREE(unit->map_ka);
    TRY_FREE(unit->map_kd);
    TRY_FREE(unit->map_ks);
    TRY_FREE(unit->map_ns);
    TRY_FREE(unit->map_d);
    TRY_FREE(unit->bump);
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
    vx->idx[3] = 1;
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
---------------------------------------
    解析名称字符串
---------------------------------------
*/
static ansi_t*
wfront_parse_name (
  __CR_IN__ const ansi_t*   str
    )
{
    ansi_t* ptr;
    ansi_t* val;

    val = str_dupA(str);
    if (val == NULL)
        return (NULL);
    ptr = val;
    while (*ptr != CR_AC(NIL)) {
        if (*ptr == CR_AC('#')) {
            *ptr = CR_AC(NIL);
            break;
        }
        ptr++;
    }
    str_trimRA(val);
    if (str_lenA(val) == 0) {
        mem_free(val);
        return (NULL);
    }
    return (val);
}

/*
---------------------------------------
    统计顶点个数
---------------------------------------
*/
static void_t
wfront_count_vertex (
  __CR_IO__ const sWAVEFRONT*   obj
    )
{
    leng_t  idx, ii, jj;

    /* 剔除重复的顶点 */
    for (idx = 0; idx < obj->n_g; idx++) {
        obj->p_g[idx].num = 1;
        for (ii = obj->p_g[idx].beg + 1; ii < obj->p_g[idx].end; ii++) {
            for (jj = obj->p_g[idx].beg; jj < ii; jj++) {
                if (mem_cmp(&obj->p_f[ii], &obj->p_f[jj],
                        sizeof(int32u) * 3) == 0) {
                    obj->p_f[ii].idx[3] = obj->p_f[jj].idx[3];
                    break;
                }
            }
            if (jj >= ii) {
                obj->p_g[idx].num += 1;
                obj->p_f[ii].idx[3] = (int32u)obj->p_g[idx].num;
            }
        }
    }
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
    sINIu*  ini;
    fp32_t  ttt;
    uint_t  cnt, ii;
    leng_t  idx, kk;
    sARRAY  a_v, a_vt, *aa;
    sARRAY  a_vn, a_f, a_g;
    /* ----------------- */
    vec3d_t         vtmp;
    sWAVEFRONT_G    gtmp;
    const ansi_t*   line;
    sWAVEFRONT_F    ff[6];

    /* 逐行分割 */
    ini = ini_parseU(str);
    if (ini == NULL)
        return (FALSE);

    /* 初始化列表 */
    array_initT(&a_v, vec3d_t);
    array_initT(&a_vt, vec3d_t);
    array_initT(&a_vn, vec3d_t);
    array_initT(&a_f, sWAVEFRONT_F);
    array_initT(&a_g, sWAVEFRONT_G);
    a_g.free = wfront_g_free;

    /* 清空对象 */
    struct_zero(obj, sWAVEFRONT);
    struct_zero(&gtmp, sWAVEFRONT_G);

    /* 逐行解析 */
    for (kk = 0, idx = 0; idx < ini->count; idx++)
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
                if (!is_spaceA(line[2]))
                    goto _failure;

                /* 贴图坐标 */
                if (!wfront_parse_v3d(&vtmp, line + 3, TRUE))
                    goto _failure;
                aa = &a_vt;
            }
            else
            if (line[1] == CR_AC('n'))
            {
                /* 非法的行 */
                if (!is_spaceA(line[2]))
                    goto _failure;

                /* 法线向量 */
                if (!wfront_parse_v3d(&vtmp, line + 3, FALSE))
                    goto _failure;
                if (neg_z)
                    vtmp.z = -vtmp.z;
                if (swap_yz) {
                    CR_SWAP(vtmp.y, vtmp.z, ttt);
                }
                ttt = FSQRT(vtmp.x * vtmp.x + vtmp.y * vtmp.y +
                                     vtmp.z * vtmp.z);
                vtmp.x /= ttt;
                vtmp.y /= ttt;
                vtmp.z /= ttt;
                aa = &a_vn;
            }
            else if (is_spaceA(line[1]))
            {
                /* 空间坐标 */
                if (!wfront_parse_v3d(&vtmp, line + 2, FALSE))
                    goto _failure;
                if (neg_z)
                    vtmp.z = -vtmp.z;
                if (swap_yz) {
                    CR_SWAP(vtmp.y, vtmp.z, ttt);
                }
                aa = &a_v;
            }
            else
            {
                /* 非法的行 */
                goto _failure;
            }

            /* 压入列表 */
            if (array_push_growT(aa, vec3d_t, &vtmp) == NULL)
                goto _failure;
            continue;
        }

        /* 三角形或面 */
        if (line[0] == CR_AC('f'))
        {
            /* 非法的行 */
            if (!is_spaceA(line[1]))
                goto _failure;
            cnt = wfront_parse_face(ff, line + 2);
            if (cnt == 0)
                goto _failure;

            /* 压入列表 */
            for (ii = 0; ii < cnt; ii++) {
                if (ff[ii].idx[0] > array_get_sizeT(&a_v, vec3d_t) ||
                    ff[ii].idx[1] > array_get_sizeT(&a_vt, vec3d_t) ||
                    ff[ii].idx[2] > array_get_sizeT(&a_vn, vec3d_t))
                    goto _failure;
                if (array_push_growT(&a_f, sWAVEFRONT_F, &ff[ii]) == NULL)
                    goto _failure;
            }
            continue;
        }

        /* 模型开始 */
        if (line[0] == CR_AC('g'))
        {
            /* 非法的行 */
            if (!is_spaceA(line[1]))
                goto _failure;

            /* 压入上个模型 */
            gtmp.end = array_get_sizeT(&a_f, sWAVEFRONT_F);
            if (gtmp.beg < gtmp.end && gtmp.name != NULL) {
                if (array_push_growT(&a_g, sWAVEFRONT_G, &gtmp) == NULL)
                    goto _failure;
                gtmp.name = NULL;
                gtmp.mtl = NULL;
            }

            /* 填充模型参数 */
            TRY_FREE(gtmp.name);
            gtmp.name = wfront_parse_name(skip_spaceA(line + 2));
            if (gtmp.name == NULL)
                goto _failure;
            gtmp.beg = gtmp.end;
            SAFE_FREE(gtmp.mtl);
            continue;
        }

        /* 模型材质 */
        if (mem_cmp(line, "usemtl", 6) == 0)
        {
            /* 非法的行 */
            if (!is_spaceA(line[6]))
                goto _failure;

            /* 根据材质拆分几何体 */
            if (gtmp.name == NULL)
                goto _failure;
            if (gtmp.mtl != NULL) {
                gtmp.end = array_get_sizeT(&a_f, sWAVEFRONT_F);
                if (array_push_growT(&a_g, sWAVEFRONT_G, &gtmp) == NULL)
                    goto _failure;
                gtmp.name = str_fmtA("split_mesh_%05u", kk++);
                if (gtmp.name == NULL)
                    goto _failure;
                gtmp.beg = gtmp.end;
            }
            gtmp.mtl = wfront_parse_name(skip_spaceA(line + 7));
            if (gtmp.mtl == NULL)
                goto _failure;
            continue;
        }

        /* 材质文件 */
        if (mem_cmp(line, "mtllib", 6) == 0)
        {
            /* 非法的行 */
            if (!is_spaceA(line[6]))
                goto _failure;

            /* 必须不重复 */
            if (obj->mtl != NULL)
                goto _failure;
            obj->mtl = wfront_parse_name(skip_spaceA(line + 7));
            if (obj->mtl == NULL)
                goto _failure;
            continue;
        }

        /* 非法的行 */
        goto _failure;
    }

    /* 压入最后一个模型 */
    gtmp.end = array_get_sizeT(&a_f, sWAVEFRONT_F);
    if (gtmp.beg < gtmp.end && gtmp.name != NULL) {
        if (array_push_growT(&a_g, sWAVEFRONT_G, &gtmp) == NULL)
            goto _failure;
        gtmp.name = NULL;
        gtmp.mtl = NULL;
    }
    else {
        SAFE_FREE(gtmp.name);
        SAFE_FREE(gtmp.mtl);
    }

    /* 固定缓冲大小 */
    if (!array_no_growT(&a_v, vec3d_t))
        goto _failure;
    if (!array_no_growT(&a_vt, vec3d_t))
        goto _failure;
    if (!array_no_growT(&a_vn, vec3d_t))
        goto _failure;
    if (!array_no_growT(&a_f, sWAVEFRONT_F))
        goto _failure;
    if (!array_no_growT(&a_g, sWAVEFRONT_G))
        goto _failure;

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
        obj->n_g == 0 || obj->p_g == NULL)
        goto _failure;
    wfront_count_vertex(obj);
    ini_closeU(ini);
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
    ini_closeU(ini);
    return (FALSE);
}

/*
---------------------------------------
    设置默认材质
---------------------------------------
*/
static void_t
wfront_mtl_default (
  __CR_OT__ sWAVEFRONT_M*   mtl
    )
{
    struct_zero(mtl, sWAVEFRONT_M);
    mtl->d = mtl->tf.x = mtl->tf.y = mtl->tf.z = 1.0f;
    mtl->sharpness = 60;
}

/*
---------------------------------------
    网格排序比较
---------------------------------------
*/
static sint_t
wfront_mtl_comp (
  __CR_IN__ const void_t*   obj1,
  __CR_IN__ const void_t*   obj2
    )
{
    leng_t  attr1, attr2;

    /* 升序排列 */
    attr1 = ((sWAVEFRONT_G*)obj1)->attr;
    attr2 = ((sWAVEFRONT_G*)obj2)->attr;
    if (attr1 > attr2)
        return (1);
    if (attr1 < attr2)
        return (-1);
    return (0);
}

/*
=======================================
    解析 MTL 字符串
=======================================
*/
CR_API bool_t
wfront_mtl_load (
  __CR_IO__ sWAVEFRONT*     obj,
  __CR_IN__ const ansi_t*   str
    )
{
    sINIu*          ini;
    sARRAY          a_m;
    leng_t          idx;
    leng_t          skip;
    ansi_t**        stmp;
    vec3d_t*        vtmp;
    sWAVEFRONT_M    mtmp;
    const ansi_t*   line;

    /* 必须已经加载过模型 */
    if (obj->p_g == NULL)
        return (FALSE);

    /* 已经创建过了 */
    if (obj->p_m != NULL)
        return (TRUE);

    /* 逐行分割 */
    ini = ini_parseU(str);
    if (ini == NULL)
        return (FALSE);

    /* 初始化列表 */
    array_initT(&a_m, sWAVEFRONT_M);
    a_m.free = wfront_m_free;

    /* 清空对象 */
    wfront_mtl_default(&mtmp);

    /* 逐行解析 */
    for (idx = 0; idx < ini->count; idx++)
    {
        /* 跳过没用的行 */
        line = skip_spaceA(ini->lines[idx]);
        if (line[0] == CR_AC(NIL) || line[0] == CR_AC('#'))
            continue;

        /* 材质开始 */
        if (mem_cmp(line, "newmtl", 6) == 0)
        {
            /* 非法的行 */
            if (!is_spaceA(line[6]))
                goto _failure;

            /* 压入上个材质 */
            if (mtmp.name != NULL) {
                if (array_push_growT(&a_m, sWAVEFRONT_M, &mtmp) == NULL)
                    goto _failure;
                wfront_mtl_default(&mtmp);
            }

            /* 保存材质名称 */
            mtmp.name = wfront_parse_name(skip_spaceA(line + 7));
            if (mtmp.name == NULL)
                goto _failure;
            continue;
        }

        /* 必须有前后顺序且不重复 */
        if (mtmp.name == NULL)
            goto _failure;

        /* 颜色矢量 */
        if (line[0] == CR_AC('K'))
        {
            /* 非法的行 */
            if (!is_spaceA(line[2]))
                goto _failure;

            /* 解析颜色矢量 */
            if (line[1] == CR_AC('a')) {
                vtmp = &mtmp.ka;
                mtmp.flags |= WAVEFRONT_KA;
            }
            else
            if (line[1] == CR_AC('d')) {
                vtmp = &mtmp.kd;
                mtmp.flags |= WAVEFRONT_KD;
            }
            else
            if (line[1] == CR_AC('s')) {
                vtmp = &mtmp.ks;
                mtmp.flags |= WAVEFRONT_KS;
            }
            else
            if (line[1] == CR_AC('e')) {
                vtmp = &mtmp.ke;
            }
            else {
                goto _failure;
            }
            if (!wfront_parse_v3d(vtmp, line + 3, FALSE))
                goto _failure;
            continue;
        }
        if (line[0] == CR_AC('T'))
        {
            /* 非法的行 */
            if (!is_spaceA(line[2]))
                goto _failure;

            /* 解析两种参数 */
            if (line[1] == CR_AC('f')) {
                if (!wfront_parse_v3d(&mtmp.tf, line + 3, FALSE))
                    goto _failure;
            }
            else
            if (line[1] == CR_AC('r')) {
                line = skip_spaceA(line + 3);
                mtmp.tr = str2fp32A(line, &skip);
                if (skip == 0)
                    goto _failure;
            }
            else {
                goto _failure;
            }
            continue;
        }

        /* 浮点标量参数 */
        if (line[0] == CR_AC('N'))
        {
            /* 非法的行 */
            if (!is_spaceA(line[2]))
                goto _failure;

            /* 解析浮点数 */
            if (line[1] == CR_AC('s')) {
                line = skip_spaceA(line + 3);
                mtmp.ns = str2fp32A(line, &skip);
            }
            else
            if (line[1] == CR_AC('i')) {
                line = skip_spaceA(line + 3);
                mtmp.ni = str2fp32A(line, &skip);
            }
            else {
                goto _failure;
            }
            if (skip == 0)
                goto _failure;
            continue;
        }
        if (line[0] == CR_AC('d'))
        {
            /* 非法的行 */
            if (!is_spaceA(line[1]))
                goto _failure;

            /* 是否有 -halo 字符串 */
            line = skip_spaceA(line + 2);
            if (mem_cmp(line, "-halo", 5) == 0 && is_spaceA(line[5])) {
                line += 6;
                mtmp.halo = 1;
            }
            else {
                mtmp.halo = 0;
            }
            mtmp.d = str2fp32A(skip_spaceA(line), &skip);
            if (skip == 0)
                goto _failure;
            continue;
        }

        /* 整数标量参数 */
        if (mem_cmp(line, "illum", 5) == 0)
        {
            /* 非法的行 */
            if (!is_spaceA(line[5]))
                goto _failure;

            /* 取值范围为0-10 */
            mtmp.illum = str2intA(skip_spaceA(line + 6), &skip);
            if (skip == 0 || mtmp.illum > 10)
                goto _failure;
            continue;
        }
        if (mem_cmp(line, "sharpness", 9) == 0)
        {
            /* 非法的行 */
            if (!is_spaceA(line[9]))
                goto _failure;

            /* 取值范围为0-1000 */
            mtmp.sharpness = str2intA(skip_spaceA(line + 10), &skip);
            if (skip == 0 || mtmp.sharpness > 1000)
                goto _failure;
            continue;
        }

        /* 纹理参数 */
        if (mem_cmp(line, "map_", 4) == 0)
        {
            line += 4;
            if (line[0] == CR_AC('K'))
            {
                /* 非法的行 */
                if (!is_spaceA(line[2]))
                    goto _failure;
                if (line[1] == CR_AC('a')) {
                    if (mtmp.map_ka != NULL)
                        goto _failure;
                    stmp = &mtmp.map_ka;
                }
                else
                if (line[1] == CR_AC('d')) {
                    if (mtmp.map_kd != NULL)
                        goto _failure;
                    stmp = &mtmp.map_kd;
                }
                else
                if (line[1] == CR_AC('s')) {
                    if (mtmp.map_ks != NULL)
                        goto _failure;
                    stmp = &mtmp.map_ks;
                }
                else {
                    goto _failure;
                }
                line += 3;
            }
            else
            if (line[0] == CR_AC('d'))
            {
                /* 非法的行 */
                if (!is_spaceA(line[1]))
                    goto _failure;
                if (mtmp.map_d != NULL)
                    goto _failure;
                stmp = &mtmp.map_d;
                line += 2;
            }
            else
            if (line[0] == CR_AC('N') && line[1] == CR_AC('s'))
            {
                /* 非法的行 */
                if (!is_spaceA(line[2]))
                    goto _failure;
                if (mtmp.map_ns != NULL)
                    goto _failure;
                stmp = &mtmp.map_ns;
                line += 3;
            }
            else
            {
                /* 非法参数名称 */
                goto _failure;
            }

            /* 跳过参数直接取文件名 */
            *stmp = wfront_parse_name(skip_spaceA(line));
            if (*stmp == NULL)
                goto _failure;
            continue;
        }
        if (mem_cmp(line, "bump", 4) == 0)
        {
            /* 非法的行 */
            if (!is_spaceA(line[4]))
                goto _failure;

            /* 不能重复 */
            if (mtmp.bump != NULL)
                goto _failure;

            /* 跳过参数直接取文件名 */
            mtmp.bump = wfront_parse_name(skip_spaceA(line + 5));
            if (mtmp.bump == NULL)
                goto _failure;
            continue;
        }
    }

    /* 压入最后一个材质 */
    if (mtmp.name != NULL) {
        if (array_push_growT(&a_m, sWAVEFRONT_M, &mtmp) == NULL)
            goto _failure;
        struct_zero(&mtmp, sWAVEFRONT_M);
    }

    /* 固定缓冲大小 */
    if (!array_no_growT(&a_m, sWAVEFRONT_M))
        goto _failure;

    /* 返回读取的数据 */
    obj->n_m = array_get_sizeT(&a_m, sWAVEFRONT_M);
    obj->p_m = array_get_dataT(&a_m, sWAVEFRONT_M);

    /* 必须要有的数据 */
    if (obj->n_m == 0 || obj->p_m == NULL)
        goto _failure;

    /* 设定模型的材质 */
    for (idx = 0; idx < obj->n_g; idx++)
    {
        /* 逐个对比名字 */
        if (obj->p_g[idx].mtl == NULL)
        {
            /* 使用前一个材质 */
            if (idx != 0)
                obj->p_g[idx].attr = obj->p_g[idx - 1].attr;
            continue;
        }
        for (skip = 0; skip < obj->n_m; skip++) {
            if (str_cmpA(obj->p_g[idx].mtl, obj->p_m[skip].name) == 0) {
                obj->p_g[idx].attr = skip + 1;
                break;
            }
        }
    }

    /* 根据材质排序网格 */
    quick_sort(obj->p_g, obj->n_g, sizeof(sWAVEFRONT_G), wfront_mtl_comp);
    ini_closeU(ini);
    return (TRUE);

_failure:
    array_freeT(&a_m, sWAVEFRONT_M);
    TRY_FREE(mtmp.name);
    TRY_FREE(mtmp.map_ka);
    TRY_FREE(mtmp.map_kd);
    TRY_FREE(mtmp.map_ks);
    TRY_FREE(mtmp.map_ns);
    TRY_FREE(mtmp.map_d);
    TRY_FREE(mtmp.bump);
    ini_closeU(ini);
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
    for (idx = 0; idx < obj->n_g; idx++)
        wfront_g_free(&obj->p_g[idx]);
    mem_free(obj->p_g);
    if (obj->p_m != NULL) {
        for (idx = 0; idx < obj->n_m; idx++)
            wfront_m_free(&obj->p_m[idx]);
        mem_free(obj->p_m);
    }
}

/*
=======================================
    合并材质相同的网格
=======================================
*/
CR_API bool_t
wfront_obj_combine (
  __CR_IO__ sWAVEFRONT* obj
    )
{
    sARRAY  a_g;
    leng_t  ii, jj, kk;
    leng_t  nn, end, cnt;
    /* --------------- */
    sWAVEFRONT_F*   p_f;
    sWAVEFRONT_G    gtmp;

    /* 必须已经加载过模型和材质 */
    if (obj->p_g == NULL || obj->p_m == NULL)
        return (FALSE);

    /* 重新生成成员 */
    p_f = mem_talloc(obj->n_f, sWAVEFRONT_F);
    if (p_f == NULL)
        return (FALSE);
    end = 0;
    array_initT(&a_g, sWAVEFRONT_G);

    /* 合并相同材质的几何体 */
    for (ii = 0; ii < obj->n_g; ii += nn) {
        struct_cpy(&gtmp, &obj->p_g[ii], sWAVEFRONT_G);
        gtmp.beg = end;
        for (nn = 0, kk = ii; kk < obj->n_g; kk++, nn++) {
            if (obj->p_g[kk].attr != gtmp.attr)
                break;
            cnt = obj->p_g[kk].end - obj->p_g[kk].beg;
            for (jj = obj->p_g[kk].beg; cnt != 0; cnt--, end++, jj++) {
                struct_cpy(&p_f[end], &obj->p_f[jj], sWAVEFRONT_F);
                p_f[end].idx[3] = 1;
            }
        }
        gtmp.end = end;
        if (array_push_growT(&a_g, sWAVEFRONT_G, &gtmp) == NULL)
            goto _failure;
    }

    /* 固定缓冲大小 */
    if (!array_no_growT(&a_g, sWAVEFRONT_G))
        goto _failure;

    /* 释放掉多余的几何体 */
    for (ii = 0; ii < obj->n_g; ii += nn) {
        gtmp.attr = obj->p_g[ii].attr;
        for (nn = 1, kk = ii + 1; kk < obj->n_g; kk++, nn++) {
            if (obj->p_g[kk].attr != gtmp.attr)
                break;
            wfront_g_free(&obj->p_g[kk]);
        }
    }

    /* 替换掉原来的成员 */
    mem_free(obj->p_f);
    mem_free(obj->p_g);
    obj->p_f = p_f;
    obj->n_g = array_get_sizeT(&a_g, sWAVEFRONT_G);
    obj->p_g = array_get_dataT(&a_g, sWAVEFRONT_G);
    wfront_count_vertex(obj);
    return (TRUE);

_failure:
    array_freeT(&a_g, sWAVEFRONT_G);
    mem_free(p_f);
    return (FALSE);
}

/*
=======================================
    生成网格数据 (Tex2D)
=======================================
*/
CR_API leng_t
wfront_gen_mesh2 (
  __CR_OT__ vec3d_t*            xyz,
  __CR_OT__ vec3d_t*            nrm,
  __CR_OT__ vec2d_t*            uvw,
  __CR_IN__ leng_t              bpv1,
  __CR_IN__ leng_t              bpv2,
  __CR_IN__ leng_t              bpv3,
  __CR_OT__ void_t*             ibuf,
  __CR_OT__ leng_t*             inum,
  __CR_IN__ const sWAVEFRONT*   obj,
  __CR_IN__ leng_t              idx
    )
{
    leng_t  ii;
    int32u  max;
    bool_t  ti16;

    /* 参数过滤 */
    if (idx >= obj->n_g)
        return (0);

    /* 返回参数 */
    if (inum != NULL)
        *inum = obj->p_g[idx].end - obj->p_g[idx].beg;
    if (xyz == NULL)
        return (obj->p_g[idx].num);
    ti16 = (obj->p_g[idx].num > 65536) ? FALSE : TRUE;

    /* 合成顶点与索引数据 */
    if (bpv1 == 0)
        bpv1 = sizeof(vec3d_t) * 2 + sizeof(vec2d_t);
    else
    if (bpv1 < sizeof(vec3d_t))
        bpv1 = sizeof(vec3d_t);
    if (bpv2 == 0)
        bpv2 = bpv1;
    else
    if (bpv2 < sizeof(vec3d_t))
        bpv2 = sizeof(vec3d_t);
    if (bpv3 == 0)
        bpv3 = bpv1;
    else
    if (bpv3 < sizeof(vec2d_t))
        bpv3 = sizeof(vec2d_t);
    max = 0;
    for (ii = obj->p_g[idx].beg; ii < obj->p_g[idx].end; ii++) {
        if (max < obj->p_f[ii].idx[3])
        {
            /* 未输出过的顶点 */
            max = obj->p_f[ii].idx[3];
            mem_cpy(xyz, &obj->p_v[obj->p_f[ii].idx[0] - 1], sizeof(vec3d_t));
            xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
            if (nrm != NULL) {
                if (obj->p_f[ii].idx[2] == 0) {
                    mem_zero(nrm, sizeof(vec3d_t));
                }
                else {
                    mem_cpy(nrm, &obj->p_vn[obj->p_f[ii].idx[2] - 1],
                            sizeof(vec3d_t));
                }
                nrm = (vec3d_t*)((byte_t*)nrm + bpv2);
            }
            if (uvw != NULL) {
                if (obj->p_f[ii].idx[1] == 0) {
                    mem_zero(uvw, sizeof(vec2d_t));
                }
                else {
                    mem_cpy(uvw, &obj->p_vt[obj->p_f[ii].idx[1] - 1],
                            sizeof(vec2d_t));
                }
                uvw = (vec2d_t*)((byte_t*)uvw + bpv3);
            }
        }

        /* 输出索引值 */
        if (ti16) {
            *(int16u*)ibuf = (int16u)(obj->p_f[ii].idx[3] - 1);
            ibuf = (byte_t*)ibuf + sizeof(int16u);
        }
        else {
            *(int32u*)ibuf = (int32u)(obj->p_f[ii].idx[3] - 1);
            ibuf = (byte_t*)ibuf + sizeof(int32u);
        }
    }
    return (obj->p_g[idx].num);
}

/*
=======================================
    生成网格数据 (Tex3D)
=======================================
*/
CR_API leng_t
wfront_gen_mesh3 (
  __CR_OT__ vec3d_t*            xyz,
  __CR_OT__ vec3d_t*            nrm,
  __CR_OT__ vec3d_t*            uvw,
  __CR_IN__ leng_t              bpv1,
  __CR_IN__ leng_t              bpv2,
  __CR_IN__ leng_t              bpv3,
  __CR_OT__ void_t*             ibuf,
  __CR_OT__ leng_t*             inum,
  __CR_IN__ const sWAVEFRONT*   obj,
  __CR_IN__ leng_t              idx
    )
{
    leng_t  ii;
    int32u  max;
    bool_t  ti16;

    /* 参数过滤 */
    if (idx >= obj->n_g)
        return (0);

    /* 返回参数 */
    if (inum != NULL)
        *inum = obj->p_g[idx].end - obj->p_g[idx].beg;
    if (xyz == NULL)
        return (obj->p_g[idx].num);
    ti16 = (obj->p_g[idx].num > 65536) ? FALSE : TRUE;

    /* 合成顶点与索引数据 */
    if (bpv1 == 0)
        bpv1 = sizeof(vec3d_t) * 2 + sizeof(vec3d_t);
    else
    if (bpv1 < sizeof(vec3d_t))
        bpv1 = sizeof(vec3d_t);
    if (bpv2 == 0)
        bpv2 = bpv1;
    else
    if (bpv2 < sizeof(vec3d_t))
        bpv2 = sizeof(vec3d_t);
    if (bpv3 == 0)
        bpv3 = bpv1;
    else
    if (bpv3 < sizeof(vec3d_t))
        bpv3 = sizeof(vec3d_t);
    max = 0;
    for (ii = obj->p_g[idx].beg; ii < obj->p_g[idx].end; ii++) {
        if (max < obj->p_f[ii].idx[3])
        {
            /* 未输出过的顶点 */
            max = obj->p_f[ii].idx[3];
            mem_cpy(xyz, &obj->p_v[obj->p_f[ii].idx[0] - 1], sizeof(vec3d_t));
            xyz = (vec3d_t*)((byte_t*)xyz + bpv1);
            if (nrm != NULL) {
                if (obj->p_f[ii].idx[2] == 0) {
                    mem_zero(nrm, sizeof(vec3d_t));
                }
                else {
                    mem_cpy(nrm, &obj->p_vn[obj->p_f[ii].idx[2] - 1],
                            sizeof(vec3d_t));
                }
                nrm = (vec3d_t*)((byte_t*)nrm + bpv2);
            }
            if (uvw != NULL) {
                if (obj->p_f[ii].idx[1] == 0) {
                    mem_zero(uvw, sizeof(vec3d_t));
                }
                else {
                    mem_cpy(uvw, &obj->p_vt[obj->p_f[ii].idx[1] - 1],
                            sizeof(vec3d_t));
                }
                uvw = (vec3d_t*)((byte_t*)uvw + bpv3);
            }
        }

        /* 输出索引值 */
        if (ti16) {
            *(int16u*)ibuf = (int16u)(obj->p_f[ii].idx[3] - 1);
            ibuf = (byte_t*)ibuf + sizeof(int16u);
        }
        else {
            *(int32u*)ibuf = (int32u)(obj->p_f[ii].idx[3] - 1);
            ibuf = (byte_t*)ibuf + sizeof(int32u);
        }
    }
    return (obj->p_g[idx].num);
}

/*
---------------------------------------
    加载二进制格式 OBJ 模型
---------------------------------------
*/
static bool_t
wfront_bobj_load_g (
  __CR_OT__ sWAVEFRONT* obj,
  __CR_IO__ iDATIN*     datin
    )
{
    byte_t  by;
    int32u  dw;
    leng_t  idx;
    leng_t  size;

    for (idx = 0; idx < obj->n_g; idx++)
    {
        /* 模型名称 */
        if (!CR_VCALL(datin)->getb_no(datin, &by))
            goto _failure1;
        if (by <= 1)
            goto _failure1;
        size = (leng_t)by;
        obj->p_g[idx].name = str_allocA(size);
        if (obj->p_g[idx].name == NULL)
            goto _failure1;
        if (CR_VCALL(datin)->read(datin, obj->p_g[idx].name, size) != size)
            goto _failure2;
        if (obj->p_g[idx].name[size - 1] != 0x00)
            goto _failure2;

        /* 材质名称 */
        if (!CR_VCALL(datin)->getb_no(datin, &by))
            goto _failure2;
        if (by <= 1) {
            obj->p_g[idx].mtl = NULL;
        }
        else {
            size = (leng_t)by;
            obj->p_g[idx].mtl = str_allocA(size);
            if (obj->p_g[idx].mtl == NULL)
                goto _failure2;
            if (CR_VCALL(datin)->read(datin, obj->p_g[idx].mtl, size) != size)
                goto _failure3;
            if (obj->p_g[idx].mtl[size - 1] != 0x00)
                goto _failure3;
        }

        /* 模型顶点相关的参数 */
        if (!CR_VCALL(datin)->getd_le(datin, &dw))
            goto _failure3;
        if (dw >= obj->n_f)
            goto _failure3;
        obj->p_g[idx].beg = (leng_t)dw;
        if (!CR_VCALL(datin)->getd_le(datin, &dw))
            goto _failure3;
        if (dw > obj->n_f || dw <= obj->p_g[idx].beg)
            goto _failure3;
        obj->p_g[idx].end = (leng_t)dw;
        if (!CR_VCALL(datin)->getd_le(datin, &dw))
            goto _failure3;
        obj->p_g[idx].num = (leng_t)dw;

        /* 材质索引号 */
        if (!CR_VCALL(datin)->getd_le(datin, &dw))
            goto _failure3;
        if (dw > obj->n_m)
            goto _failure3;
        obj->p_g[idx].attr = (leng_t)dw;
    }
    return (TRUE);

_failure3:
    TRY_FREE(obj->p_g[idx].mtl);
_failure2:
    mem_free(obj->p_g[idx].name);
_failure1:
    for (size = 0; size < idx; size++)
        wfront_g_free(&obj->p_g[size]);
    return (FALSE);
}

/*
---------------------------------------
    加载二进制格式 MTL 材质
---------------------------------------
*/
static bool_t
wfront_bobj_load_m (
  __CR_OT__ sWAVEFRONT* obj,
  __CR_IO__ iDATIN*     datin
    )
{
    byte_t  by;
    ansi_t  **lst;
    int32u  dw, *ptr;
    leng_t  idx, size, kk;

    for (idx = 0; idx < obj->n_m; idx++)
    {
        /* 材质标志 */
        if (!CR_VCALL(datin)->getd_le(datin, &dw))
            goto _failure1;
        obj->p_m[idx].flags = (uint_t)dw;

        /* 材质名称 */
        if (!CR_VCALL(datin)->getb_no(datin, &by))
            goto _failure1;
        if (by <= 1)
            goto _failure1;
        size = (leng_t)by;
        obj->p_m[idx].name = str_allocA(size);
        if (obj->p_m[idx].name == NULL)
            goto _failure1;
        if (CR_VCALL(datin)->read(datin, obj->p_m[idx].name, size) != size)
            goto _failure2;
        if (obj->p_m[idx].name[size - 1] != 0x00)
            goto _failure2;

        /* 一堆浮点数 */
        ptr = (int32u*)((void_t*)(&obj->p_m[idx].d));
        for (kk = 0; kk < 4 + 5 * 3; kk++, ptr++) {
            if (!CR_VCALL(datin)->getd_le(datin, ptr))
                goto _failure2;
        }

        /* 一堆整数 */
        if (!CR_VCALL(datin)->getd_le(datin, &dw))
            goto _failure2;
        if (dw > 10)
            goto _failure2;
        obj->p_m[idx].illum = (uint_t)dw;
        if (!CR_VCALL(datin)->getd_le(datin, &dw))
            goto _failure2;
        if (dw > 1)
            goto _failure2;
        obj->p_m[idx].halo = (uint_t)dw;
        if (!CR_VCALL(datin)->getd_le(datin, &dw))
            goto _failure2;
        if (dw > 1000)
            goto _failure2;
        obj->p_m[idx].sharpness = (uint_t)dw;

        /* 一堆字符串 */
        lst = &obj->p_m[idx].map_ka;
        for (kk = 0; kk < 6; kk++) {
            if (!CR_VCALL(datin)->getb_no(datin, &by))
                goto _failure3;
            if (by <= 1) {
                lst[kk] = NULL;
            }
            else {
                size = (leng_t)by;
                lst[kk] = str_allocA(size);
                if (lst[kk] == NULL)
                    goto _failure3;
                if (CR_VCALL(datin)->read(datin, lst[kk], size) != size) {
                    mem_free(lst[kk]);
                    goto _failure3;
                }
                if ((lst[kk])[size - 1] != 0x00) {
                    mem_free(lst[kk]);
                    goto _failure3;
                }
            }
        }
    }
    return (TRUE);

_failure3:
    for (size = 0; size < kk; size++)
        TRY_FREE(lst[size]);
_failure2:
    mem_free(obj->p_m[idx].name);
_failure1:
    for (size = 0; size < idx; size++)
        wfront_m_free(&obj->p_m[size]);
    return (FALSE);
}

/*
=======================================
    加载二进制格式 OBJ 模型
=======================================
*/
CR_API bool_t
wfront_bobj_load (
  __CR_OT__ sWAVEFRONT* obj,
  __CR_IO__ iDATIN*     datin
    )
{
    byte_t  by;
    leng_t  size;
    int32u  dw, *ptr;

    /* 文件标志 */
    if (!CR_VCALL(datin)->getd_no(datin, &dw))
        return (FALSE);
    if (dw != mk_tag4("BOBJ"))
        return (FALSE);

    /* 材质文件 */
    if (!CR_VCALL(datin)->getb_no(datin, &by))
        return (FALSE);
    if (by <= 1) {
        obj->mtl = NULL;
    }
    else {
        size = (leng_t)by;
        obj->mtl = str_allocA(size);
        if (obj->mtl == NULL)
            return (FALSE);
        if (CR_VCALL(datin)->read(datin, obj->mtl, size) != size)
            goto _failure1;
        if (obj->mtl[size - 1] != 0x00)
            goto _failure1;
    }

    /* 空间坐标 */
    if (!CR_VCALL(datin)->getd_le(datin, &dw))
        goto _failure1;
    obj->p_v = mem_talloc32(dw, vec3d_t);
    if (obj->p_v == NULL)
        goto _failure1;
    obj->n_v = (leng_t)dw;
    size = obj->n_v * sizeof(vec3d_t);
    if (CR_VCALL(datin)->read(datin, obj->p_v, size) != size)
        goto _failure2;
#if defined(_CR_ORDER_BE_)
    ptr = (int32u*)obj->p_v;
    for (size = 0; size < obj->n_v; size++) {
        ptr[0] = xchg_int32u(ptr[0]);
        ptr[1] = xchg_int32u(ptr[1]);
        ptr[2] = xchg_int32u(ptr[2]);
        ptr += 3;
    }
#endif

    /* 贴图坐标 */
    if (!CR_VCALL(datin)->getd_le(datin, &dw))
        goto _failure2;
    if (dw == 0) {
        obj->n_vt = 0;
        obj->p_vt = NULL;
    }
    else {
        obj->p_vt = mem_talloc32(dw, vec3d_t);
        if (obj->p_vt == NULL)
            goto _failure2;
        obj->n_vt = (leng_t)dw;
        size = obj->n_vt * sizeof(vec3d_t);
        if (CR_VCALL(datin)->read(datin, obj->p_vt, size) != size)
            goto _failure3;
#if defined(_CR_ORDER_BE_)
        ptr = (int32u*)obj->p_vt;
        for (size = 0; size < obj->n_vt; size++) {
            ptr[0] = xchg_int32u(ptr[0]);
            ptr[1] = xchg_int32u(ptr[1]);
            ptr[2] = xchg_int32u(ptr[2]);
            ptr += 3;
        }
#endif
    }

    /* 法线向量 */
    if (!CR_VCALL(datin)->getd_le(datin, &dw))
        goto _failure3;
    if (dw == 0) {
        obj->n_vn = 0;
        obj->p_vn = NULL;
    }
    else {
        obj->p_vn = mem_talloc32(dw, vec3d_t);
        if (obj->p_vn == NULL)
            goto _failure3;
        obj->n_vn = (leng_t)dw;
        size = obj->n_vn * sizeof(vec3d_t);
        if (CR_VCALL(datin)->read(datin, obj->p_vn, size) != size)
            goto _failure4;
#if defined(_CR_ORDER_BE_)
        ptr = (int32u*)obj->p_vn;
        for (size = 0; size < obj->n_vn; size++) {
            ptr[0] = xchg_int32u(ptr[0]);
            ptr[1] = xchg_int32u(ptr[1]);
            ptr[2] = xchg_int32u(ptr[2]);
            ptr += 3;
        }
#endif
    }

    /* 顶点索引 */
    if (!CR_VCALL(datin)->getd_le(datin, &dw))
        goto _failure4;
    obj->p_f = mem_talloc32(dw, sWAVEFRONT_F);
    if (obj->p_f == NULL)
        goto _failure4;
    obj->n_f = (leng_t)dw;
    size = obj->n_f * sizeof(sWAVEFRONT_F);
    if (CR_VCALL(datin)->read(datin, obj->p_f, size) != size)
        goto _failure5;
    ptr = (int32u*)obj->p_f;
    for (size = 0; size < obj->n_f; size++) {
        ptr[0] = DWORD_LE(ptr[0]);
        if (ptr[0] == 0 || ptr[0] > obj->n_v)
            goto _failure5;
        ptr[1] = DWORD_LE(ptr[1]);
        if (ptr[1] > obj->n_vt)
            goto _failure5;
        ptr[2] = DWORD_LE(ptr[2]);
        if (ptr[2] > obj->n_vn)
            goto _failure5;
        ptr[3] = DWORD_LE(ptr[3]);
        ptr += 4;
    }

    /* 材质列表 */
    if (!CR_VCALL(datin)->getd_le(datin, &dw))
        goto _failure5;
    if (dw == 0) {
        obj->n_m = 0;
        obj->p_m = NULL;
    }
    else {
        obj->p_m = mem_talloc32(dw, sWAVEFRONT_M);
        if (obj->p_m == NULL)
            goto _failure5;
        obj->n_m = (leng_t)dw;
        if (!wfront_bobj_load_m(obj, datin)) {
            mem_free(obj->p_m);
            goto _failure5;
        }
    }

    /* 模型列表 */
    if (!CR_VCALL(datin)->getd_le(datin, &dw))
        goto _failure6;
    obj->p_g = mem_talloc32(dw, sWAVEFRONT_G);
    if (obj->p_g == NULL)
        goto _failure6;
    obj->n_g = (leng_t)dw;
    if (!wfront_bobj_load_g(obj, datin))
        goto _failure7;
    return (TRUE);

_failure7:
    mem_free(obj->p_g);
_failure6:
    if (obj->p_m != NULL) {
        for (size = 0; size < obj->n_m; size++)
            wfront_m_free(&obj->p_m[size]);
        mem_free(obj->p_m);
    }
_failure5:
    mem_free(obj->p_f);
_failure4:
    TRY_FREE(obj->p_vn);
_failure3:
    TRY_FREE(obj->p_vt);
_failure2:
    mem_free(obj->p_v);
_failure1:
    TRY_FREE(obj->mtl);
    return (FALSE);
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
