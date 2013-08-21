/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-08-20  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-08-21  */
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

#ifndef __CR_FLC_AIA_C__
#define __CR_FLC_AIA_C__ 0x3367C9E0UL

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
            if (srclen < 2) {
                err_set(__CR_FLC_AIA_C__, srclen,
                        "decode_aia()", "source buffer overflow");
                return (FALSE);
            }
            srclen -= 2;
            val  = *src++;
            val <<= 8;
            val |= *src++;

            if (val & 0x8000)
            {
                val &= 0xFFF;
                if (val > width - line) {
                    err_set(__CR_FLC_AIA_C__, val,
                            "decode_aia()", "dest buffer overflow");
                    return (FALSE);
                }
                line += val;

                if (val & 1)
                {
                    if (srclen < 1) {
                        err_set(__CR_FLC_AIA_C__, srclen,
                                "decode_aia()", "source buffer overflow");
                        return (FALSE);
                    }
                    srclen -= 1;
                    dst[0] = pal[*src++];
                    dst += 1;
                    val ^= 1;
                }
                if (val & 2)
                {
                    if (srclen < 2) {
                        err_set(__CR_FLC_AIA_C__, srclen,
                                "decode_aia()", "source buffer overflow");
                        return (FALSE);
                    }
                    srclen -= 2;
                    dst[0] = pal[*src++];
                    dst[1] = pal[*src++];
                    dst += 2;
                    val ^= 2;
                }
                if (val != 0)
                {
                    val = (val - 1) / 4 + 1;
                    if (srclen < val * 4) {
                        err_set(__CR_FLC_AIA_C__, srclen,
                                "decode_aia()", "source buffer overflow");
                        return (FALSE);
                    }
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
                if (val > width - line) {
                    err_set(__CR_FLC_AIA_C__, val,
                            "decode_aia()", "dest buffer overflow");
                    return (FALSE);
                }
                line += val;

                if (srclen < val * 2) {
                    err_set(__CR_FLC_AIA_C__, srclen,
                            "decode_aia()", "source buffer overflow");
                    return (FALSE);
                }
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
                if (val > width - line) {
                    err_set(__CR_FLC_AIA_C__, val,
                            "decode_aia()", "dest buffer overflow");
                    return (FALSE);
                }
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
        int16u  width;      /* 图片像素宽 */
        int16u  height;     /* 图片像素高 */
        int16u  ww, hh;     /* 未知宽高值？(140 版全部没有) */
        fp32_t  fw, fh;     /* 浮点宽高值？(140 版没有 fh) */
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

/*
=======================================
    FALCOM AIA 文件读取
=======================================
*/
CR_API sFMT_PIC*
load_flc_aia (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    leng_t  read;
    leng_t  size;
    byte_t* pals;
    byte_t* dats;
    uint_t  ww, hh;
    int32u* dst_ptr;
    int32u  idx, cnt;
    /* ----------- */
    fdist_t     offs;
    sAIA_HDR    head;
    sAIA_IDX*   attr;
    sFMT_PIC*   rett;
    sFMT_FRAME* list;

    /* 这个参数可能为空 */
    if (datin == NULL) {
        err_set(__CR_FLC_AIA_C__, CR_NULL,
                "load_flc_aia()", "invalid param: datin");
        return (NULL);
    }

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->getT(datin, &head, sAIA_HDR))) {
        err_set(__CR_FLC_AIA_C__, FALSE,
                "load_flc_aia()", "iDATIN::getT() failure");
        return (NULL);
    }
    if (head.magic != mk_tag4("AIA")) {
        err_set(__CR_FLC_AIA_C__, head.magic,
                "load_flc_aia()", "invalid AIA format");
        return (NULL);
    }
    offs = DWORD_LE(head.idx_num);
    offs *= sizeof(int32u) * 4;
    if (head.version == CWORD_LE(0x140)) {
        offs += 32;
        mem_cpy(&head.img_size, &head.fw, 4);
    }
    else
    if (head.version == CWORD_LE(0x150)) {
        offs += 40;
    }
    else {
        err_set(__CR_FLC_AIA_C__, head.version,
                "load_flc_aia()", "invalid AIA format");
        return (NULL);
    }

    /* 跳过未知数据 */
    if (!CR_VCALL(datin)->seek(datin, offs, SEEK_SET)) {
        err_set(__CR_FLC_AIA_C__, FALSE,
                "load_flc_aia()", "iDATIN::seek() failure");
        return (NULL);
    }

    /* 读取所有帧属性数据 */
    head.img_num = DWORD_LE(head.img_num);
    attr = mem_talloc32(head.img_num, sAIA_IDX);
    if (attr == NULL) {
        err_set(__CR_FLC_AIA_C__, CR_NULL,
                "load_flc_aia()", "mem_talloc32() failure");
        return (NULL);
    }
    size = (leng_t)head.img_num;
    size *= sizeof(sAIA_IDX);
    read = CR_VCALL(datin)->read(datin, attr, size);
    if (read != size) {
        err_set(__CR_FLC_AIA_C__, read,
                "load_flc_aia()", "iDATIN::read() failure");
        goto _failure1;
    }

    /* 获取图片统一宽高 */
    ww = WORD_LE(head.width);
    hh = WORD_LE(head.height);

    /* 统计真正有图像的帧数 */
    for (cnt = idx = 0; idx < head.img_num; idx++) {
        attr[idx].x1 = WORD_LE(attr[idx].x1);
        attr[idx].y1 = WORD_LE(attr[idx].y1);
        attr[idx].x2 = WORD_LE(attr[idx].x2);
        attr[idx].y2 = WORD_LE(attr[idx].y2);
        if ((uint_t)attr[idx].x2 > ww ||
            (uint_t)attr[idx].y2 > hh ||
            attr[idx].x1 >= attr[idx].x2 ||
            attr[idx].y1 >= attr[idx].y2) {
            attr[idx].x2 = attr[idx].y2 = 0;
            continue;
        }
        attr[idx].x2 = attr[idx].x2 - attr[idx].x1;
        attr[idx].y2 = attr[idx].y2 - attr[idx].y1;
        cnt += 1;
    }

    /* 读取所有调色板数据 */
    head.pal_num = DWORD_LE(head.pal_num);
    pals = (byte_t*)mem_calloc32(head.pal_num, 1024);
    if (pals == NULL) {
        err_set(__CR_FLC_AIA_C__, CR_NULL,
                "load_flc_aia()", "mem_calloc32() failure");
        goto _failure1;
    }
    size = (leng_t)head.pal_num;
    size *= 1024;
    read = CR_VCALL(datin)->read(datin, pals, size);
    if (read != size) {
        err_set(__CR_FLC_AIA_C__, read,
                "load_flc_aia()", "iDATIN::read() failure");
        goto _failure2;
    }
    for (read = 0; read < size; read += 4)
        pals[read + 3] = 0xFF;

    /* 读取所有图形数据 */
    head.img_size = DWORD_LE(head.img_size);
    dats = (byte_t*)mem_malloc32(head.img_size);
    if (dats == NULL) {
        err_set(__CR_FLC_AIA_C__, CR_NULL,
                "load_flc_aia()", "mem_malloc32() failure");
        goto _failure2;
    }
    size = (leng_t)head.img_size;
    read = CR_VCALL(datin)->read(datin, dats, size);
    if (read != size) {
        err_set(__CR_FLC_AIA_C__, read,
                "load_flc_aia()", "iDATIN::read() failure");
        goto _failure3;
    }

    /* 分配图片列表 */
    list = mem_talloc32(cnt, sFMT_FRAME);
    if (list == NULL) {
        err_set(__CR_FLC_AIA_C__, CR_NULL,
                "load_flc_aia()", "mem_talloc32() failure");
        goto _failure3;
    }
    size = (leng_t)ww;
    mem_tzero(list, cnt, sFMT_FRAME);

    /* 逐帧加载图片数据 */
    for (cnt = idx = 0; idx < head.img_num; idx++)
    {
        /* 跳过废帧 (请自己定义帧序号) */
        if (attr[idx].x2 == 0 || attr[idx].y2 == 0)
            continue;

        /* 安全检查 */
        attr[idx].offset = DWORD_LE(attr[idx].offset);
        if (attr[idx].offset >= head.img_size) {
            err_set(__CR_FLC_AIA_C__, attr[idx].offset,
                    "load_flc_aia()", "invalid AIA format");
            goto _failure4;
        }
        attr[idx].pal_idx = WORD_LE(attr[idx].pal_idx);
        if ((int32u)attr[idx].pal_idx >= head.pal_num) {
            err_set(__CR_FLC_AIA_C__, attr[idx].pal_idx,
                    "load_flc_aia()", "invalid AIA format");
            goto _failure4;
        }

        /* 生成图片对象 */
        list[cnt].fmt = CR_PIC_ARGB;
        list[cnt].bpp = 32;
        list[cnt].clr = "ARGB";
        list[cnt].wh[0] = 8;
        list[cnt].wh[1] = 8;
        list[cnt].wh[2] = 8;
        list[cnt].wh[3] = 8;
        list[cnt].pic = image_new(0, 0, ww, hh, CR_ARGB8888, FALSE, 4);
        if (list[cnt].pic == NULL) {
            err_set(__CR_FLC_AIA_C__, CR_NULL,
                    "load_flc_aia()", "image_new() failure");
            goto _failure4;
        }
        mem_zero(list[cnt].pic->data, list[cnt].pic->size);

        /* 解码图形数据 */
        read = (leng_t)attr[idx].pal_idx;
        dst_ptr = (int32u*)(list[cnt].pic->data);
        if (!decode_aia(dst_ptr + attr[idx].y1 * size + attr[idx].x1,
                &dats[attr[idx].offset], head.img_size - attr[idx].offset,
                &(((int32u*)pals)[read * 256]), attr[idx].x2, attr[idx].y2,
                                ww - attr[idx].x2)) {
            err_set(__CR_FLC_AIA_C__, FALSE,
                    "load_flc_aia()", "decode_aia() failure");
            image_del(list[cnt].pic);
            goto _failure4;
        }
        cnt += 1;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL) {
        err_set(__CR_FLC_AIA_C__, CR_NULL,
                "load_flc_aia()", "struct_new() failure");
        goto _failure4;
    }
    mem_free(dats);
    mem_free(pals);
    mem_free(attr);
    CR_NOUSE(param);
    rett->type = CR_FMTZ_PIC;
    rett->count = cnt;
    rett->frame = list;
    rett->infor = "FALCOM YS AIA Image File (*.AIA)";
    return (rett);

_failure4:
    for (idx = 0; idx < cnt; idx++)
        image_del(list[idx].pic);
    mem_free(list);
_failure3:
    mem_free(dats);
_failure2:
    mem_free(pals);
_failure1:
    mem_free(attr);
    return (NULL);
}

#endif  /* !__CR_FLC_AIA_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
