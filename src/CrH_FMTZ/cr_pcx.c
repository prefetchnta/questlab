/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-07-08  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-07-09  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack PCX 图片读取函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_PCX_C__
#define __CR_PCX_C__ 0xFE6C2A25UL

#include "fmtz.h"
#include "pixels.h"

/* PCX 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* PCX 文件头结构 */
CR_TYPEDEF struct
{
        byte_t  man;            /* 标志 0x0A */
        byte_t  ver;            /* 版本号 0x05 */
        byte_t  enc;            /* 是否 RLE 压缩 */
        byte_t  bpp;            /* 颜色位数 */
        int16u  x1, y1;         /* 左上角坐标 */
        int16u  x2, y2;         /* 右下角坐标 */
        int16u  horz_res;       /* 水平分辨率 */
        int16u  vert_res;       /* 垂直分辨率 */
        byte_t  ega_pal[48];    /* EGA 调色板 */
        byte_t  reserved;       /* 保留字节 */
        byte_t  ncp;            /* 像素字节数 */
        int16u  bpl;            /* 一行字节数 */
        int16u  pal_type;       /* 调色板类型 */
        byte_t  padding[58];    /* 保留字节 */

} CR_PACKED sPCX_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    PCX 文件读取
=======================================
*/
CR_API sFMT_PIC*
load_cr_pcx (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    byte_t* ptr;
    byte_t* line;
    leng_t  ptsz;
    uint_t  fcrh;
    uint_t  x1, y1;
    uint_t  ww, hh;
    byte_t  pal[769];
    leng_t  bpl, read;
    /* ------------ */
    sPCX_HDR    head;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 这个参数可能为空 */
    if (datin == NULL) {
        err_set(__CR_PCX_C__, CR_NULL,
                "load_cr_pcx()", "invalid param: datin");
        return (NULL);
    }

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->getT(datin, &head, sPCX_HDR))) {
        err_set(__CR_PCX_C__, FALSE,
                "load_cr_pcx()", "iDATIN::getT() failure");
        return (NULL);
    }
    if (head.man != 0x0A) {
        err_set(__CR_PCX_C__, head.man,
                "load_cr_pcx()", "invalid PCX format");
        return (NULL);
    }
    if (head.ver != 0x05) {
        err_set(__CR_PCX_C__, head.ver,
                "load_cr_pcx()", "invalid PCX format");
        return (NULL);
    }
    if (head.enc != 0x00 && head.enc != 0x01) {
        err_set(__CR_PCX_C__, head.enc,
                "load_cr_pcx()", "invalid PCX format");
        return (NULL);
    }
    if (head.bpp != 0x08) {
        err_set(__CR_PCX_C__, head.bpp,
                "load_cr_pcx()", "invalid PCX format");
        return (NULL);
    }
    if (head.ncp != 1 && head.ncp != 3) {
        err_set(__CR_PCX_C__, head.ncp,
                "load_cr_pcx()", "invalid PCX format");
        return (NULL);
    }

    /* 获取图片宽高 */
    x1 = WORD_LE(head.x1);
    y1 = WORD_LE(head.y1);
    ww = WORD_LE(head.x2);
    hh = WORD_LE(head.y2);
    if (x1 > ww || y1 > hh) {
        err_set(__CR_PCX_C__, CR_ERROR,
                "load_cr_pcx()", "invalid PCX format");
        return (NULL);
    }
    ww = ww - x1 + 1;
    hh = hh - y1 + 1;

    /* 生成图片对象 */
    mem_zero(&temp.wh, sizeof(temp.wh));
    if (head.ncp == 1) {
        fcrh = CR_INDEX8;
        temp.fmt = CR_PIC_PALS;
        temp.bpp = 8;
        temp.clr = "P";
        temp.wh[0] = 8;
    }
    else {
        fcrh = CR_ARGB888;
        temp.fmt = CR_PIC_ARGB;
        temp.bpp = 24;
        temp.clr = "BGR";
        temp.wh[0] = 8;
        temp.wh[1] = 8;
        temp.wh[2] = 8;
    }
    /* PCX 没有行字节对齐 */
    temp.pic = image_new(x1, y1, ww, hh, fcrh, FALSE, 4);
    if (temp.pic == NULL) {
        err_set(__CR_PCX_C__, CR_NULL,
                "load_cr_pcx()", "image_new() failure");
        return (NULL);
    }

    /* 读取图片数据 */
    ptr = temp.pic->data;
    if (head.ncp == 1)
    {
        /* 读取调色板 */
        if (!CR_VCALL(datin)->seek(datin, -769L, SEEK_END)) {
            err_set(__CR_PCX_C__, FALSE,
                    "load_cr_pcx()", "iDATIN::seek() failure");
            goto _failure;
        }
        read = CR_VCALL(datin)->read(datin, pal, 769);
        if (read != 769) {
            err_set(__CR_PCX_C__, read,
                    "load_cr_pcx()", "iDATIN::read() failure");
            goto _failure;
        }
        if (pal[0] != 0x0C) {
            err_set(__CR_PCX_C__, pal[0],
                    "load_cr_pcx()", "invalid PCX format");
            goto _failure;
        }
        pal_3b_to_4b(temp.pic->pal, &pal[1], 256);

        /* 开始读取图形数据 */
        if (!CR_VCALL(datin)->seek(datin, 128, SEEK_SET)) {
            err_set(__CR_PCX_C__, FALSE,
                    "load_cr_pcx()", "iDATIN::seek() failure");
            goto _failure;
        }
        bpl = ww;

        /* 一般都有 RLE 压缩 */
        if (head.enc == 0x00) {
            for (; hh != 0; hh--) {
                read = CR_VCALL(datin)->read(datin, ptr, bpl);
                if (read != bpl) {
                    err_set(__CR_PCX_C__, read,
                            "load_cr_pcx()", "iDATIN::read() failure");
                    goto _failure;
                }
                ptr += temp.pic->bpl;
            }
        }
        else {
            for (; hh != 0; hh--) {
                for (x1 = 0; x1 < ww; x1++) {
                    if (!CR_VCALL(datin)->getb_no(datin, pal)) {
                        err_set(__CR_PCX_C__, FALSE,
                                "load_cr_pcx()", "iDATIN::getb_no() failure");
                        goto _failure;
                    }
                    if ((pal[0] & 0xC0) != 0xC0) {
                        ptr[x1] = pal[0];
                        continue;
                    }
                    y1 = pal[0] & 0x3F;
                    if (x1 + y1 > ww) {
                        err_set(__CR_PCX_C__, pal[0],
                                "load_cr_pcx()", "invalid PCX format");
                        goto _failure;
                    }
                    if (!CR_VCALL(datin)->getb_no(datin, pal)) {
                        err_set(__CR_PCX_C__, FALSE,
                                "load_cr_pcx()", "iDATIN::getb_no() failure");
                        goto _failure;
                    }
                    for (; y1 != 0; y1--)
                        ptr[x1++] = pal[0];
                    x1 -= 1;
                }
                ptr += temp.pic->bpl;
            }
        }
    }
    else
    {
        /* 24位色按 RRRRGGGGBBBB 格式存放 */
        bpl = ww;
        bpl *= 3;
        line = (byte_t*)mem_malloc(bpl);
        if (line == NULL) {
            err_set(__CR_PCX_C__, CR_NULL,
                    "load_cr_pcx()", "mem_malloc() failure");
            goto _failure;
        }

        /* 一般都有 RLE 压缩 */
        if (head.enc == 0x00) {
            for (; hh != 0; hh--) {
                read = CR_VCALL(datin)->read(datin, line, bpl);
                if (read != bpl) {
                    err_set(__CR_PCX_C__, read,
                            "load_cr_pcx()", "iDATIN::read() failure");
                    mem_free(line);
                    goto _failure;
                }
                for (y1 = 0; y1 < ww; y1++) {
                    ptsz = (leng_t)y1;
                    ptr[ptsz * 3 + 2] = line[ptsz];
                    ptr[ptsz * 3 + 1] = line[ptsz + ww];
                    ptr[ptsz * 3 + 0] = line[ptsz + ww + ww];
                }
                ptr += temp.pic->bpl;
            }
        }
        else {
            for (; hh != 0; hh--) {
                for (read = 0; read < bpl; read++) {
                    if (!CR_VCALL(datin)->getb_no(datin, pal)) {
                        err_set(__CR_PCX_C__, FALSE,
                                "load_cr_pcx()", "iDATIN::getb_no() failure");
                        mem_free(line);
                        goto _failure;
                    }
                    if ((pal[0] & 0xC0) != 0xC0) {
                        line[read] = pal[0];
                        continue;
                    }
                    y1 = pal[0] & 0x3F;
                    if (read + y1 > bpl) {
                        err_set(__CR_PCX_C__, pal[0],
                                "load_cr_pcx()", "invalid PCX format");
                        mem_free(line);
                        goto _failure;
                    }
                    if (!CR_VCALL(datin)->getb_no(datin, pal)) {
                        err_set(__CR_PCX_C__, FALSE,
                                "load_cr_pcx()", "iDATIN::getb_no() failure");
                        mem_free(line);
                        goto _failure;
                    }
                    for (; y1 != 0; y1--)
                        line[read++] = pal[0];
                    read -= 1;
                }
                for (y1 = 0; y1 < ww; y1++) {
                    ptsz = (leng_t)y1;
                    ptr[ptsz * 3 + 2] = line[ptsz];
                    ptr[ptsz * 3 + 1] = line[ptsz + ww];
                    ptr[ptsz * 3 + 0] = line[ptsz + ww + ww];
                }
                ptr += temp.pic->bpl;
            }
        }
        mem_free(line);
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL) {
        err_set(__CR_PCX_C__, CR_NULL,
                "load_cr_pcx()", "struct_new() failure");
        goto _failure;
    }
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        err_set(__CR_PCX_C__, CR_NULL,
                "load_cr_pcx()", "struct_dup() failure");
        mem_free(rett);
        goto _failure;
    }
    CR_NOUSE(param);
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = "Zsoft Publisher's Paintbrush (PCX)";
    return (rett);

_failure:
    image_del(temp.pic);
    return (NULL);
}

#endif  /* !__CR_PCX_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/