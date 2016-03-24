/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-12-02  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>> CrHack 高度图函数库 <<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "memlib.h"
#include "gfx3int.h"

#ifndef _CR_NO_STDC_
    #if defined(_CR_OS_ANDROID_) && (ANDROID_API <= 19)
        #if defined(__pure2)
            #undef  __pure2
        #endif
    #endif
    #include <math.h>
#endif

/*
=======================================
    生成高度图
=======================================
*/
CR_API ht_map_t
height_map_new (
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          type,
  __CR_IN__ uint_t          width,
  __CR_IN__ uint_t          height,
  __CR_IN__ fp32_t          grid,
  __CR_IN__ fp32_t          scale
    )
{
    sint_t      tmp;
    leng_t      idx;
    leng_t      size;
    sHEIGHTMAP* rett;

    /* 参数过滤 */
    if (type > HTMAP_TYPE_REAL ||
        width < 2 || height < 2 || grid <= 0.0f)
        return (NULL);

    /* 创建对象 */
    rett = struct_new(sHEIGHTMAP);
    if (rett == NULL)
        return (NULL);
    if (cut_mul(&size, width, height)) {
        mem_free(rett);
        return (NULL);
    }
    rett->map = mem_talloc(size, fp32_t);
    if (rett->map == NULL) {
        mem_free(rett);
        return (NULL);
    }
    rett->ww = width;
    rett->x2 = width - 1;
    rett->hh = height;
    rett->z2 = height - 1;
    rett->grid = grid;
    rett->inv_grid = 1.0f / grid;

    /* 生成高度图 */
    if (data == NULL) {
        mem_zero(rett->map, size * sizeof(fp32_t));
    }
    else {
        switch (type)
        {
            case HTMAP_TYPE_BYTE:
                for (idx = 0; idx < size; idx++) {
                    tmp = *(byte_t*)data;
                    data = (byte_t*)data + sizeof(byte_t);
                    rett->map[idx] = (fp32_t)tmp;
                    rett->map[idx] *= scale;
                }
                break;

            case HTMAP_TYPE_WORD:
                for (idx = 0; idx < size; idx++) {
                    tmp = *(int16u*)data;
                    data = (byte_t*)data + sizeof(int16u);
                    rett->map[idx] = (fp32_t)tmp;
                    rett->map[idx] *= scale;
                }
                break;

            case HTMAP_TYPE_SINT:
                for (idx = 0; idx < size; idx++) {
                    tmp = *(sint_t*)data;
                    data = (byte_t*)data + sizeof(sint_t);
                    rett->map[idx] = (fp32_t)tmp;
                    rett->map[idx] *= scale;
                }
                break;

            default:
            case HTMAP_TYPE_REAL:
                mem_cpy(rett->map, data, size * sizeof(fp32_t));
                break;
        }
    }
    return ((ht_map_t)rett);
}

/*
=======================================
    释放高度图
=======================================
*/
CR_API void_t
height_map_del (
  __CR_IN__ ht_map_t    htmap
    )
{
    sHEIGHTMAP* real;

    real = (sHEIGHTMAP*)htmap;
    mem_free(real->map);
    mem_free(htmap);
}

/*
=======================================
    获取高度值
=======================================
*/
CR_API fp32_t
height_map_get (
  __CR_IN__ ht_map_t    htmap,
  __CR_IN__ fp32_t      x,
  __CR_IN__ fp32_t      z
    )
{
    sint_t  uu, vv;
    fp32_t  t1, t2, t3;
    fp32_t  h1, h2, h3, *map;
    /* ------------------- */
    sHEIGHTMAP* real = (sHEIGHTMAP*)htmap;

    x *= real->inv_grid;
    z *= real->inv_grid;
    uu = (sint_t)x;
    vv = (sint_t)z;
    if (uu < 0) {
        uu = 0;
        x = 0.0f;
    }
    else
    if (uu >= (sint_t)real->x2) {
        uu = real->x2 - 1;
        x = (fp32_t)real->x2;
    }
    if (vv < 0) {
        vv = 0;
        z = 0.0f;
    }
    else
    if (vv >= (sint_t)real->z2) {
        vv = real->z2 - 1;
        z = (fp32_t)real->z2;
    }
    map = &real->map[vv * real->ww + uu];

    h1 = map[0];
    h2 = map[1] - h1;
    h3 = map[real->ww + 1] - h2 - h1;

    t1 = x - uu;
    t2 = z - vv;
    t3 = t1 - t2;
    if (t3 > 0.0f)
        return (t1 * h2 + t2 * h3 + h1);
    h2 = map[real->ww + 0] - h1;
    h3 = map[real->ww + 1] - h2 - h1;
    return (t1 * h3 + t2 * h2 + h1);
}

/*
=======================================
    获取顶点法线
=======================================
*/
CR_API void_t
height_map_nrm (
  __CR_IN__ ht_map_t    htmap,
  __CR_OT__ vec3d_t*    normal,
  __CR_IN__ uint_t      x,
  __CR_IN__ uint_t      z,
  __CR_IN__ bool_t      is_nrm
    )
{
    fp32_t  length;
    sint_t  delta1 = -1, delta2 = 1;
    sint_t  delta3 = -1, delta4 = 1;
    /* -------------------------- */
    sHEIGHTMAP* real = (sHEIGHTMAP*)htmap;

    if (x == 0) {
        delta1 = 0;
    }
    else
    if (x >= real->x2) {
        x = real->x2;
        delta2 = 0;
    }
    if (z == 0) {
        delta3 = 0;
    }
    else
    if (z >= real->z2) {
        z = real->z2;
        delta4 = 0;
    }
    normal->x = real->map[z * real->ww + x + delta1] -
                real->map[z * real->ww + x + delta2];
    normal->y = 2.0f * real->grid;
    normal->z = real->map[(z + delta4) * real->ww + x] -
                real->map[(z + delta3) * real->ww + x];
    if (is_nrm) {
        length = FSQRT(normal->x * normal->x +
                       normal->y * normal->y +
                       normal->z * normal->z);
        normal->x /= length;
        normal->y /= length;
        normal->z /= length;
    }
}

/*
=======================================
    获取块包围盒
=======================================
*/
CR_API void_t
height_map_aabb (
  __CR_IN__ ht_map_t    htmap,
  __CR_OT__ sAABB*      aabb,
  __CR_IN__ uint_t      x,
  __CR_IN__ uint_t      z,
  __CR_IN__ uint_t      w,
  __CR_IN__ uint_t      h
    )
{
    fp32_t  tmp;
    uint_t  tx, tz;
    vec3d_t box[2];
    /* --------- */
    sHEIGHTMAP* real;

    real = (sHEIGHTMAP*)htmap;
    if (x >= real->x2)
        x = real->x2 - 1;
    if (z >= real->z2)
        z = real->z2 - 1;
    if (w < 2)
        w = 2;
    else
    if (w > real->ww - x)
        w = real->ww - x;
    if (h < 2)
        h = 2;
    else
    if (h > real->hh - z)
        h = real->hh - z;

    box[0].x = x * real->grid;
    box[0].y = real->map[z * real->ww + x];
    box[0].z = z * real->grid;
    box[1].x = (x + w - 1) * real->grid;
    box[1].y = box[0].y;
    box[1].z = (z + h - 1) * real->grid;

    for (tz = z; tz < z + h; tz++)
    for (tx = x; tx < x + w; tx++) {
        tmp = real->map[tz * real->ww + tx];
        if (tmp < box[0].y)
            box[0].y = tmp;
        if (tmp > box[1].y)
            box[1].y = tmp;
    }
    bound_gen_aabb(aabb, &box[0], &box[1]);
}

/*
=======================================
    放置包围盒
=======================================
*/
CR_API fp32_t
height_map_place (
  __CR_IN__ ht_map_t        htmap,
  __CR_IN__ const sAABB*    aabb,
  __CR_IN__ fp32_t          x,
  __CR_IN__ fp32_t          z,
  __CR_IN__ bool_t          large
    )
{
    fp32_t  min, tmp;

    if (large)
    {
        /* 四个边框的最小高度 */
        min = height_map_get(htmap, aabb->v[AABB_MAX_IDX].x + x,
                                    aabb->v[AABB_MAX_IDX].z + z);
        tmp = height_map_get(htmap, aabb->v[AABB_MIN_IDX].x + x,
                                    aabb->v[AABB_MIN_IDX].z + z);
        if (min > tmp)
            min = tmp;
        tmp = height_map_get(htmap, aabb->v[AABB_MAX_IDX].x + x,
                                    aabb->v[AABB_MIN_IDX].z + z);
        if (min > tmp)
            min = tmp;
        tmp = height_map_get(htmap, aabb->v[AABB_MIN_IDX].x + x,
                                    aabb->v[AABB_MAX_IDX].z + z);
        if (min > tmp)
            min = tmp;
    }
    else
    {
        /* 以物体中心点取高度 */
        min = height_map_get(htmap, x, z);
    }

    /* 计算底面的偏移值 */
    return (min - aabb->v[AABB_MIN_IDX].y);
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
