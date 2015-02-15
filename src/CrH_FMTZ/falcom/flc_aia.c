/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-08-20  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack FALCOM AIA 图片读取函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "strlib.h"
#include "fmtz/falcom.h"

/*
---------------------------------------
    FALCOM AIA 图片解码
---------------------------------------
*/
static bool_t
decode_aia (
  __CR_OT__ int32u*         dst,
  __CR_IN__ const byte_t*   src,
  __CR_IN__ int32u          srclen,
  __CR_IN__ const int32u*   pal,
  __CR_IN__ uint_t          width,
  __CR_IN__ uint_t          height,
  __CR_IN__ uint_t          rest
    )
{
    byte_t  alpha;
    uint_t  line, val;

    do
    {
        line = 0;
        do
        {
            if (srclen < 2)
                return (FALSE);
            srclen -= 2;
            val  = *src++;
            val <<= 8;
            val |= *src++;

            if (val & 0x8000)
            {
                val &= 0xFFF;
                if (val > width - line)
                    return (FALSE);
                line += val;

                if (val & 1)
                {
                    if (srclen < 1)
                        return (FALSE);
                    srclen -= 1;
                    dst[0] = pal[*src++];
                    dst += 1;
                    val ^= 1;
                }
                if (val & 2)
                {
                    if (srclen < 2)
                        return (FALSE);
                    srclen -= 2;
                    dst[0] = pal[*src++];
                    dst[1] = pal[*src++];
                    dst += 2;
                    val ^= 2;
                }
                if (val != 0)
                {
                    val = (val - 1) / 4 + 1;
                    if (srclen < val * 4)
                        return (FALSE);
                    srclen -= val * 4;
                    for (; val != 0; val--) {
                        dst[0] = pal[*src++];
                        dst[1] = pal[*src++];
                        dst[2] = pal[*src++];
                        dst[3] = pal[*src++];
                        dst += 4;
                    }
                }
            }
            else
            if (val & 0x4000)
            {
                val &= 0xFFF;
                if (val > width - line)
                    return (FALSE);
                line += val;

                if (srclen < val * 2)
                    return (FALSE);
                srclen -= val * 2;
                for (; val != 0; val--) {
                    alpha = *src++;
                    dst[0] = pal[*src++];
                    ((byte_t*)dst)[3] = alpha;
                    dst++;
                }
            }
            else
            {
                if (val > width - line)
                    return (FALSE);
                line += val;
                for (; val != 0; val--)
                    *dst++ = 0UL;
            }
        } while (line < width);

        height--;
        dst += rest;

    } while (height != 0);

    return (TRUE);
}

/* AIA 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* AIA 图片项结构 */
CR_TYPEDEF struct
{
        int32u  offset;     /* 图片数据相对偏移 */
        int16u  pal_idx;    /* 使用第几个调色板 */
        int16u  img_attr;   /* 帧属性？=0x0080 为修饰帧 */
        fp32_t  coord[4];   /* 四个浮点值, 坐标？ */
        int16u  x1, y1;     /* 图像数据左上角坐标 */
        int16u  x2, y2;     /* 图像数据右下角坐标+1 */

} CR_PACKED sAIA_IDX;

/* AIA 文件头结构 */
CR_TYPEDEF struct
{
        int32u  magic;      /* 文件标志 "AIA" */
        int16u  version;    /* 版本号？=0x140 | 0x150 */
        int16u  flags;      /* 文件标志？=0x00 | 0x01 */
        int32u  pal_num;    /* 调色板个数 */
        int32u  idx_num;    /* 索引项个数 */
        int32u  img_num;    /* 图片项个数 */
        int16u  ww1, hh1;   /* 图片宽高值 */
        int16u  ww2, hh2;   /* 未知宽高值 (140 版没有) */
        fp32_t  scale1;     /* 未知浮点数 */
        fp32_t  scale2;     /* 未知浮点数 (140 版没有) */
        int32u  img_size;   /* 图像数据大小 */
#if 0
        int32u      idx[idx_num * 4];       /* 未知数据, 索引？ */
        sAIA_IDX    img[img_num];           /* 图片帧描述 */
        int32u      pal[pal_num * 256];     /* 调色板数据 */
        byte_t      dat[img_size];          /* 压缩的图形数据 */
#endif
} CR_PACKED sAIA_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPICTURE    pics;

        /* 个性部分 */
        uint_t      m_ww;
        uint_t      m_hh;
        int32u      m_size;
        int32u*     m_pals;
        byte_t*     m_dats;
        sAIA_IDX*   m_attr;

} iPIC_AIA;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPIC_AIA_release (
  __CR_IN__ iPICTURE*   that
    )
{
    iPIC_AIA*   real;

    real = (iPIC_AIA*)that;
    mem_free(real->m_dats);
    mem_free(real->m_pals);
    mem_free(real->m_attr);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iPIC_AIA_getMore (
  __CR_IN__ iPICTURE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPICTURE::YS_AIA") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    获取图片帧
---------------------------------------
*/
static sFMT_PIC*
iPIC_AIA_get (
  __CR_IN__ iPICTURE*   that,
  __CR_IN__ int32u      index
    )
{
    leng_t      bpln;
    leng_t      paln;
    int32u*     dest;
    iPIC_AIA*   real;
    sFMT_PIC*   rett;
    sAIA_IDX*   attr;
    sFMT_FRAME  temp;

    /* 帧号过滤 */
    if (index >= that->__count__)
        return (NULL);

    /* 生成图片对象 */
    real = (iPIC_AIA*)that;
    mem_zero(temp.wh, sizeof(temp.wh));
    temp.fmt = CR_PIC_ARGB;
    temp.bpp = 32;
    temp.clr = "ARGB";
    temp.wh[0] = 8;
    temp.wh[1] = 8;
    temp.wh[2] = 8;
    temp.wh[3] = 8;
    temp.pic = image_new(0, 0, real->m_ww, real->m_hh,
                         CR_ARGB8888, FALSE, 4);
    if (temp.pic == NULL)
        return (NULL);
    mem_zero(temp.pic->data, temp.pic->size);

    /* 解码图形数据 */
    attr = &real->m_attr[index];
    bpln = (leng_t)real->m_ww;
    paln = (leng_t)attr->pal_idx;
    dest = (int32u*)(temp.pic->data);
    dest += attr->y1 * bpln + attr->x1;
    if (!decode_aia(dest, &real->m_dats[attr->offset],
            real->m_size - attr->offset, &real->m_pals[paln * 256],
                attr->x2, attr->y2, real->m_ww - attr->x2))
        goto _failure;

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL)
        goto _failure;
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        mem_free(rett);
        goto _failure;
    }
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = "FALCOM YS AIA Image File (*.AIA)";
    return (rett);

_failure:
    image_del(temp.pic);
    return (NULL);
}

/* 接口虚函数表 */
static const iPICTURE_vtbl _rom_ s_pics_vtbl =
{
    iPIC_AIA_release, iPIC_AIA_getMore, iPIC_AIA_get,
};

/*
=======================================
    FALCOM AIA 文件读取
=======================================
*/
CR_API sFMT_PRT*
load_flc_aia (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    leng_t  back;
    leng_t  size;
    byte_t* pals;
    byte_t* dats;
    uint_t  ww, hh;
    int32u  idx, cnt;
    /* ----------- */
    fdist_t     offs;
    sAIA_HDR    head;
    sAIA_IDX*   attr;
    sFMT_PRT*   rett;
    iPIC_AIA*   port;

    /* 这个参数可能为空 */
    if (datin == NULL)
        return (NULL);

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->geType(datin, &head, sAIA_HDR)))
        return (NULL);
    if (head.magic != mk_tag4("AIA"))
        return (NULL);
    offs = DWORD_LE(head.idx_num);
    offs *= sizeof(int32u) * 4;
    if (head.version == CWORD_LE(0x140)) {
        offs += 32;
        pals = (byte_t*)(&head.img_size);
        dats = (byte_t*)(&head.scale1);
        mem_cpy(pals, dats, 4);
        pals = (byte_t*)(&head.ww2);
        mem_cpy(dats, pals, 4);
        head.ww2 = head.hh2 = 0;
        head.scale2 = 0.0f;
    }
    else
    if (head.version == CWORD_LE(0x150)) {
        offs += 40;
    }
    else {
        return (NULL);
    }

    /* 定位到已知数据区域 */
    if (!CR_VCALL(datin)->seek(datin, offs, SEEK_SET))
        return (NULL);

    /* 读取所有帧属性数据 */
    head.img_num = DWORD_LE(head.img_num);
    attr = mem_talloc32(head.img_num, sAIA_IDX);
    if (attr == NULL)
        return (NULL);

    /* 准备好一些属性值 */
    ww = WORD_LE(head.ww1);
    hh = WORD_LE(head.hh1);
    head.pal_num  = DWORD_LE(head.pal_num);
    head.img_size = DWORD_LE(head.img_size);

    /* 逐个读入有效的帧属性 */
    for (cnt = idx = 0; idx < head.img_num; idx++) {
        back = CR_VCALL(datin)->read(datin, &attr[cnt], sizeof(sAIA_IDX));
        if (back != sizeof(sAIA_IDX))
            goto _failure1;

        /* 跳过非法的废帧 */
        attr[cnt].offset = DWORD_LE(attr[cnt].offset);
        if (attr[cnt].offset >= head.img_size)
            continue;
        attr[cnt].pal_idx = WORD_LE(attr[cnt].pal_idx);
        if ((int32u)attr[cnt].pal_idx >= head.pal_num)
            continue;

        /* 跳过废帧 (请自己定义帧序号) */
        attr[cnt].x1 = WORD_LE(attr[cnt].x1);
        attr[cnt].y1 = WORD_LE(attr[cnt].y1);
        attr[cnt].x2 = WORD_LE(attr[cnt].x2);
        attr[cnt].y2 = WORD_LE(attr[cnt].y2);
        if ((uint_t)attr[cnt].x2 > ww ||
            (uint_t)attr[cnt].y2 > hh ||
            attr[cnt].x1 >= attr[cnt].x2 ||
            attr[cnt].y1 >= attr[cnt].y2)
            continue;
        attr[cnt].x2 = attr[cnt].x2 - attr[cnt].x1;
        attr[cnt].y2 = attr[cnt].y2 - attr[cnt].y1;
        cnt += 1;
    }

    /* 空图片检查 */
    if (cnt == 0)
        goto _failure1;

    /* 读取所有调色板数据 */
    pals = (byte_t*)mem_calloc32(head.pal_num, 1024);
    if (pals == NULL)
        goto _failure1;
    size = (leng_t)head.pal_num;
    size *= 1024;
    if (CR_VCALL(datin)->read(datin, pals, size) != size)
        goto _failure2;
    for (back = 0; back < size; back += 4)
        pals[back + 3] = 0xFF;

    /* 读取所有图形数据 */
    dats = (byte_t*)mem_malloc32(head.img_size);
    if (dats == NULL)
        goto _failure2;
    size = (leng_t)head.img_size;
    if (CR_VCALL(datin)->read(datin, dats, size) != size)
        goto _failure3;

    /* 生成多帧图片接口对象 */
    port = struct_new(iPIC_AIA);
    if (port == NULL)
        goto _failure3;
    port->m_ww = ww;
    port->m_hh = hh;
    port->m_attr = attr;
    port->m_dats = dats;
    port->m_pals = (int32u*)pals;
    port->m_size = head.img_size;
    port->pics.__count__ = cnt;
    port->pics.__vptr__ = &s_pics_vtbl;

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iPIC_AIA_release((iPICTURE*)port);
        return (NULL);
    }
    CR_NOUSE(param);
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iPICTURE";
    rett->infor = "FALCOM YS AIA Image File (*.AIA)";
    return (rett);

_failure3:
    mem_free(dats);
_failure2:
    mem_free(pals);
_failure1:
    mem_free(attr);
    return (NULL);
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
