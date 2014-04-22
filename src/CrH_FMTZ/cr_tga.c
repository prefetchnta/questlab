/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-07-08  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack TGA 图片读取函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_TGA_C__
#define __CR_TGA_C__ 0xFC42B9D3UL

#include "fmtz.h"

/* TGA 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* TGA 文件头结构 */
CR_TYPEDEF struct
{
        byte_t  id_len;         /* 图片标识长度 */
        byte_t  map_type;       /* 是否使用调色板 */
        byte_t  img_type;       /* 图片类型: 0 - 无图像
                                             1 - 无压缩调色板
                                             2 - 无压缩真彩
                                             3 - 无压缩灰度
                                             9 - RLE 调色板
                                            10 - RLE 真彩
                                            11 - RLE 灰度
                                */
        int16u  first_idx;      /* 调色板起始项 */
        int16u  map_length;     /* 调色板总项数 */
        byte_t  map_item_size;  /* 调色板一项位数 */
        int16u  x1, y1;         /* 左上角坐标 */
        int16u  ww, hh;         /* 图片的宽高 */
        byte_t  bpp;            /* 图片的位数 */
        byte_t  img_attr;       /* 图片属性: bit0-bit3 - 用法不一定
                                             bit4 - 图片是否从右到左
                                             bit5 - 图片是否从上到下
                                             bit6-bit7 - 保留为0
                                */
#if 0
        byte_t  img_id[id_len];     /* 图片标识 */
        byte_t  color_map[];        /* 调色板数据 */
#endif
} CR_PACKED sTGA_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    TGA 文件读取
=======================================
*/
CR_API sFMT_PIC*
load_cr_tga (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    byte_t* ptr;
    byte_t* line;
    bool_t  flip;
    uint_t  fcrh;
    uint_t  ii, jj;
    uint_t  ww, hh;
    byte_t  pixel[4];
    leng_t  bpl, read;
    leng_t  by, repeat;
    /* ------------- */
    sTGA_HDR    head;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 这个参数可能为空 */
    if (datin == NULL) {
        err_set(__CR_TGA_C__, CR_NULL,
                "load_cr_tga()", "invalid param: datin");
        return (NULL);
    }

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->geType(datin, &head, sTGA_HDR))) {
        err_set(__CR_TGA_C__, FALSE,
                "load_cr_tga()", "iDATIN::geType() failure");
        return (NULL);
    }
    if (head.map_type != 0) {
        err_set(__CR_TGA_C__, head.map_type,
                "load_cr_tga()", "invalid TGA format");
        return (NULL);
    }
    if (head.img_type != 2 && head.img_type != 10) {
        err_set(__CR_TGA_C__, head.img_type,
                "load_cr_tga()", "invalid TGA format");
        return (NULL);
    }
    if (head.first_idx != 0) {
        err_set(__CR_TGA_C__, head.first_idx,
                "load_cr_tga()", "invalid TGA format");
        return (NULL);
    }
    if (head.map_length != 0) {
        err_set(__CR_TGA_C__, head.map_length,
                "load_cr_tga()", "invalid TGA format");
        return (NULL);
    }
    if (head.map_item_size != 0) {
        err_set(__CR_TGA_C__, head.map_item_size,
                "load_cr_tga()", "invalid TGA format");
        return (NULL);
    }
    if (head.bpp == 0) {
        head.bpp = 32;
    }
    else
    if (head.bpp != 24 && head.bpp != 32) {
        err_set(__CR_TGA_C__, head.bpp,
                "load_cr_tga()", "invalid TGA format");
        return (NULL);
    }
    if ((head.img_attr >> 6) != 0) {
        err_set(__CR_TGA_C__, head.img_attr,
                "load_cr_tga()", "invalid TGA format");
        return (NULL);
    }

    /* 跳过图片标识数据 */
    if (head.id_len != 0) {
        if (!CR_VCALL(datin)->seek(datin, head.id_len, SEEK_CUR)) {
            err_set(__CR_TGA_C__, FALSE,
                    "load_cr_tga()", "iDATIN::seek() failure");
            return (NULL);
        }
    }

    /* 获取图片宽高 */
    ww = WORD_LE(head.ww);
    hh = WORD_LE(head.hh);
    if ((head.img_attr >> 5) & 1)
        flip = FALSE;
    else
        flip = TRUE;

    /* 生成图片对象 */
    mem_zero(temp.wh, sizeof(temp.wh));
    if (head.bpp == 24) {
        fcrh = CR_ARGB888;
        temp.fmt = CR_PIC_ARGB;
        temp.bpp = 24;
        temp.clr = "RGB";
        temp.wh[0] = 8;
        temp.wh[1] = 8;
        temp.wh[2] = 8;
    }
    else {
        fcrh = CR_ARGB8888;
        temp.fmt = CR_PIC_ARGB;
        temp.bpp = 32;
        temp.clr = "ARGB";
        temp.wh[0] = 8;
        temp.wh[1] = 8;
        temp.wh[2] = 8;
        temp.wh[3] = 8;
    }
    /* TGA 没有行字节对齐 */
    temp.pic = image_new(WORD_LE(head.x1), WORD_LE(head.y1),
                         ww, hh, fcrh, flip, 4);
    if (temp.pic == NULL) {
        err_set(__CR_TGA_C__, CR_NULL,
                "load_cr_tga()", "image_new() failure");
        return (NULL);
    }

    /* 读取图片数据 */
    by = head.bpp / 8;
    bpl = ww;
    bpl *= by;
    line = temp.pic->data;
    if (head.img_type == 2) {
        for (; hh != 0; hh--) {
            read = CR_VCALL(datin)->read(datin, line, bpl);
            if (read != bpl) {
                err_set(__CR_TGA_C__, read,
                        "load_cr_tga()", "iDATIN::read() failure");
                goto _failure;
            }
            line += temp.pic->bpl;
        }
    }
    else {
        for (; hh != 0; hh--) {
            ptr = line;
            for (ii = 0; ii < ww; ii++) {
                if (!CR_VCALL(datin)->getb_no(datin, pixel)) {
                    err_set(__CR_TGA_C__, FALSE,
                            "load_cr_tga()", "iDATIN::getb_no() failure");
                    goto _failure;
                }
                jj = (pixel[0] & 0x7F) + 1;
                if (jj > ww - ii) {
                    err_set(__CR_TGA_C__, pixel[0],
                            "load_cr_tga()", "invalid TGA format");
                    goto _failure;
                }
                repeat = jj;
                repeat *= by;
                if (!(pixel[0] & 0x80)) {
                    read = CR_VCALL(datin)->read(datin, ptr, repeat);
                    if (read != repeat) {
                        err_set(__CR_TGA_C__, read,
                                "load_cr_tga()", "iDATIN::read() failure");
                        goto _failure;
                    }
                    ii  += jj - 1;
                    ptr += repeat;
                    continue;
                }

                /* 读取需要重复的像素 */
                read = CR_VCALL(datin)->read(datin, pixel, by);
                if (read != by) {
                    err_set(__CR_TGA_C__, read,
                            "load_cr_tga()", "iDATIN::read() failure");
                    goto _failure;
                }
                for (; jj != 0; jj--, ii++) {
                    mem_cpy(ptr, pixel, by);
                    ptr += by;
                }
                ii -= 1;
            }
            line += temp.pic->bpl;
        }
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL) {
        err_set(__CR_TGA_C__, CR_NULL,
                "load_cr_tga()", "struct_new() failure");
        goto _failure;
    }
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        err_set(__CR_TGA_C__, CR_NULL,
                "load_cr_tga()", "struct_dup() failure");
        mem_free(rett);
        goto _failure;
    }
    CR_NOUSE(param);
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = "Truevision Targa (TGA)";
    return (rett);

_failure:
    image_del(temp.pic);
    return (NULL);
}

#endif  /* !__CR_TGA_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
