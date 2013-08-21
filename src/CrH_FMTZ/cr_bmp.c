/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-05-03  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack BMP 图片读取函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_BMP_C__
#define __CR_BMP_C__ 0x04578D73UL

#include "fmtz.h"
#include "safe.h"

/* BMP 压缩格式 */
#define CR_BI_RGB       0UL
#define CR_BI_RLE8      1UL
#define CR_BI_RLE4      2UL
#define CR_BI_BITFIELDS 3UL
#define CR_BI_JPEG      4UL
#define CR_BI_PNG       5UL

/* BMP 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* BMP 文件头结构 */
CR_TYPEDEF struct
{
        int16u  biMagic;            /* "BM" 标志 */
        int32u  biFileSize;         /* 整个文件大小 */
        int32u  biReserved;         /* 保留为 0x00 */
        int32u  biOffset;           /* 图片数据开始偏移 */
        int32u  biInfoSize;         /* 图片信息大小=40 */
        int32u  biWidth;            /* 图片宽度 */
        int32u  biHeight;           /* 图片高度 */
        int16u  biPlanes;           /* 颜色平面数=1 */
        int16u  biBitCount;         /* 颜色位数: 0/1/4/8/16/24/32
                                       0表示 PNG 或 JPEG 格式 */
        int32u  biCompression;      /* 图片压缩格式 */
        int32u  biSizeImage;        /* 图片数据大小 */
        int32u  biXPelsPerMeter;    /* X 轴每米像素 */
        int32u  biYPelsPerMeter;    /* Y 轴每米像素 */
        int32u  biClrUsed;          /* 0或调色板颜色数 */
        int32u  biClrImportant;     /* 用到的调色板颜色数 */

} CR_PACKED sBMP_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
---------------------------------------
    BMP RLE 解压 (4位色)
---------------------------------------
*/
static bool_t
bmp_unrle4 (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sIMAGE*   image
    )
{
    leng_t  idx, count, offset = 0;
    byte_t  pair[2], *data = image->data;
    leng_t  read, end_of_line = image->bpl;

    mem_zero(image->data, image->size);
    while (offset < image->size)
    {
        /* 两个字节一对 */
        read = CR_VCALL(datin)->read(datin, pair, 2);
        if (read != 2) {
            err_set(__CR_BMP_C__, read,
                    "bmp_unrle4()", "iDATIN::read() failure");
            return (FALSE);
        }

        /* RLE 输出 */
        if (pair[0] != 0x00) {
            count = (leng_t)pair[0];
            if (count > image->size - offset)
                count = image->size - offset;
            pair[0] = pair[1] >> 4;
            pair[1] = pair[1] & 0x0F;
            for (idx = 0; idx < count; idx++)
                data[offset++] = pair[idx & 1];
            continue;
        }

        /* 特殊标记 */
        switch (pair[1])
        {
            case 0x00:  /* 一行结束 */
                offset = end_of_line;
                end_of_line += image->bpl;
                break;

            case 0x01:  /* 位图结束 */
                offset = image->size;
                break;

            case 0x02:  /* 移动坐标 */
                read = CR_VCALL(datin)->read(datin, pair, 2);
                if (read != 2) {
                    err_set(__CR_BMP_C__, read,
                            "bmp_unrle4()", "iDATIN::read() failure");
                    return (FALSE);
                }
                offset += pair[0] + pair[1] * image->bpl;
                end_of_line += pair[1] * image->bpl;
                break;

            default:    /* 直接复制 */
                count = (leng_t)pair[1];
                if (count > image->size - offset)
                    count = image->size - offset;
                for (idx = 0; idx < count; idx++) {
                    if (idx & 1) {
                        data[offset++] = pair[0] & 0x0F;
                        continue;
                    }
                    read = CR_VCALL(datin)->read(datin, pair, 1);
                    if (read != 1) {
                        err_set(__CR_BMP_C__, read,
                                "bmp_unrle4()", "iDATIN::read() failure");
                        return (FALSE);
                    }
                    data[offset++] = pair[0] >> 4;
                }
                count %= 4;
                if (count == 1 || count == 2) {
                    read = CR_VCALL(datin)->read(datin, pair, 1);
                    if (read != 1) {
                        err_set(__CR_BMP_C__, read,
                                "bmp_unrle4()", "iDATIN::read() failure");
                        return (FALSE);
                    }
                }
                break;
        }
    }
    return (TRUE);
}

/*
---------------------------------------
    BMP RLE 解压 (8位色)
---------------------------------------
*/
static bool_t
bmp_unrle8 (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sIMAGE*   image
    )
{
    leng_t  count, offset = 0;
    byte_t  pair[2], *data = image->data;
    leng_t  read, end_of_line = image->bpl;

    mem_zero(image->data, image->size);
    while (offset < image->size)
    {
        /* 两个字节一对 */
        read = CR_VCALL(datin)->read(datin, pair, 2);
        if (read != 2) {
            err_set(__CR_BMP_C__, read,
                    "bmp_unrle8()", "iDATIN::read() failure");
            return (FALSE);
        }

        /* RLE 输出 */
        if (pair[0] != 0x00) {
            count = (leng_t)pair[0];
            if (count > image->size - offset)
                count = image->size - offset;
            mem_set(data + offset, pair[1], count);
            offset += count;
            continue;
        }

        /* 特殊标记 */
        switch (pair[1])
        {
            case 0x00:  /* 一行结束 */
                offset = end_of_line;
                end_of_line += image->bpl;
                break;

            case 0x01:  /* 位图结束 */
                offset = image->size;
                break;

            case 0x02:  /* 移动坐标 */
                read = CR_VCALL(datin)->read(datin, pair, 2);
                if (read != 2) {
                    err_set(__CR_BMP_C__, read,
                            "bmp_unrle8()", "iDATIN::read() failure");
                    return (FALSE);
                }
                offset += pair[0] + pair[1] * image->bpl;
                end_of_line += pair[1] * image->bpl;
                break;

            default:    /* 直接复制 */
                count = (leng_t)pair[1];
                if (count > image->size - offset)
                    count = image->size - offset;
                read = CR_VCALL(datin)->read(datin, data + offset, count);
                if (read != count) {
                    err_set(__CR_BMP_C__, read,
                            "bmp_unrle8()", "iDATIN::read() failure");
                    return (FALSE);
                }
                if (count & 1) {
                    read = CR_VCALL(datin)->read(datin, pair, 1);
                    if (read != 1) {
                        err_set(__CR_BMP_C__, read,
                                "bmp_unrle8()", "iDATIN::read() failure");
                        return (FALSE);
                    }
                }
                offset += count;
                break;
        }
    }
    return (TRUE);
}

/*
=======================================
    BMP 文件读取
=======================================
*/
CR_API sFMT_PIC*
load_cr_bmp (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    uint_t  ww;
    sint_t  hh;
    leng_t  read;
    bool_t  flip;
    uint_t  fcrh;
    byte_t  pal[1024];
    /* ----------- */
    sBMP_HDR    head;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 这个参数可能为空 */
    if (datin == NULL) {
        err_set(__CR_BMP_C__, CR_NULL,
                "load_cr_bmp()", "invalid param: datin");
        return (NULL);
    }

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->getT(datin, &head, sBMP_HDR))) {
        err_set(__CR_BMP_C__, FALSE,
                "load_cr_bmp()", "iDATIN::getT() failure");
        return (NULL);
    }
    if (head.biMagic != mk_tag2("BM")) {
        err_set(__CR_BMP_C__, head.biMagic,
                "load_cr_bmp()", "invalid BMP format");
        return (NULL);
    }
    if (head.biReserved != 0UL) {
        err_set(__CR_BMP_C__, head.biReserved,
                "load_cr_bmp()", "invalid BMP format");
        return (NULL);
    }
    if (head.biInfoSize != CDWORD_LE(40)) {
        err_set(__CR_BMP_C__, head.biInfoSize,
                "load_cr_bmp()", "invalid BMP format");
        return (NULL);
    }
    if (head.biPlanes != CWORD_LE(1)) {
        err_set(__CR_BMP_C__, head.biPlanes,
                "load_cr_bmp()", "invalid BMP format");
        return (NULL);
    }
    head.biOffset = DWORD_LE(head.biOffset);
    if (head.biOffset < sizeof(sBMP_HDR) ||
        head.biOffset >= dati_get_size(datin)) {
        err_set(__CR_BMP_C__, head.biOffset,
                "load_cr_bmp()", "invalid BMP format");
        return (NULL);
    }
    head.biClrUsed = DWORD_LE(head.biClrUsed);
    if (head.biClrUsed > 256) {
        err_set(__CR_BMP_C__, head.biClrUsed,
                "load_cr_bmp()", "invalid BMP format");
        return (NULL);
    }

    /* 对宽高的截断检查 */
    if (cut_int32_u(&ww, DWORD_LE(head.biWidth))) {
        err_set(__CR_BMP_C__, head.biWidth,
                "load_cr_bmp()", "image width truncated");
        return (NULL);
    }
    if (cut_int32_s(&hh, DWORD_LE(head.biHeight))) {
        err_set(__CR_BMP_C__, head.biHeight,
                "load_cr_bmp()", "image height truncated");
        return (NULL);
    }

    /* 图像是否颠倒 */
    if (hh < 0) {
        hh = -hh;
        flip = FALSE;
    }
    else {
        flip = TRUE;
    }

    /* 读取调色板区 */
    mem_zero(pal, sizeof(pal));
    head.biBitCount = WORD_LE(head.biBitCount);
    head.biCompression = DWORD_LE(head.biCompression);
    if (head.biCompression == CR_BI_BITFIELDS) {
        head.biClrUsed = 3 * sizeof(int32u);
    }
    else if (head.biBitCount <= 8) {
        if (head.biClrUsed == 0)
            head.biClrUsed = 1UL << head.biBitCount;
        head.biClrUsed *= sizeof(int32u);
    }
    if (head.biClrUsed != 0) {
        read = CR_VCALL(datin)->read(datin, pal, (leng_t)head.biClrUsed);
        if (read != (leng_t)head.biClrUsed) {
            err_set(__CR_BMP_C__, read,
                    "load_cr_bmp()", "iDATIN::read() failure");
            return (NULL);
        }
    }

    /* 生成图片对象 */
    mem_zero(temp.wh, sizeof(temp.wh));
    switch (head.biBitCount)
    {
        case 1:
            if (head.biCompression != CR_BI_RGB) {
                err_set(__CR_BMP_C__, head.biCompression,
                        "load_cr_bmp()", "invalid BMP format");
                return (NULL);
            }
            fcrh = CR_INDEX1;
            temp.fmt = CR_PIC_PALS;
            temp.bpp = 1;
            temp.clr = "P";
            temp.wh[0] = 1;
            break;

        case 4:
            if (head.biCompression != CR_BI_RGB &&
                head.biCompression != CR_BI_RLE4) {
                err_set(__CR_BMP_C__, head.biCompression,
                        "load_cr_bmp()", "invalid BMP format");
                return (NULL);
            }
            if (head.biCompression != CR_BI_RLE4)
                fcrh = CR_INDEX4;
            else
                fcrh = CR_INDEX8;
            temp.fmt = CR_PIC_PALS;
            temp.bpp = 4;
            temp.clr = "P";
            temp.wh[0] = 4;
            break;

        case 8:
            if (head.biCompression != CR_BI_RGB &&
                head.biCompression != CR_BI_RLE8) {
                err_set(__CR_BMP_C__, head.biCompression,
                        "load_cr_bmp()", "invalid BMP format");
                return (NULL);
            }
            fcrh = CR_INDEX8;
            temp.fmt = CR_PIC_PALS;
            temp.bpp = 8;
            temp.clr = "P";
            temp.wh[0] = 8;
            break;

        case 16:
            if (head.biCompression != CR_BI_RGB &&
                head.biCompression != CR_BI_BITFIELDS) {
                err_set(__CR_BMP_C__, head.biCompression,
                        "load_cr_bmp()", "invalid BMP format");
                return (NULL);
            }
            temp.fmt = CR_PIC_ARGB;
            temp.bpp = 16;
            if (head.biCompression == CR_BI_RGB ||
                mem_cmp(pal, "\0\x7C\0\0\xE0\x03\0\0\x1F\0\0\0", 12) == 0)
            {
                /* 格式 X555 */
                fcrh = CR_ARGBX555;
                temp.clr = "RGB";
                temp.wh[0] = 5;
                temp.wh[1] = 5;
                temp.wh[2] = 5;
            }
            else
            if (mem_cmp(pal, "\0\xF8\0\0\xE0\x07\0\0\x1F\0\0\0", 12) == 0)
            {
                /* 格式 565 */
                fcrh = CR_ARGB565;
                temp.clr = "RGB";
                temp.wh[0] = 5;
                temp.wh[1] = 6;
                temp.wh[2] = 5;
            }
            else
            {
                /* 无法识别的颜色码值 */
                err_set(__CR_BMP_C__, head.biCompression,
                        "load_cr_bmp()", "invalid BMP format");
                return (NULL);
            }
            break;

        case 24:
            if (head.biCompression != CR_BI_RGB) {
                err_set(__CR_BMP_C__, head.biCompression,
                        "load_cr_bmp()", "invalid BMP format");
                return (NULL);
            }
            fcrh = CR_ARGB888;
            temp.fmt = CR_PIC_ARGB;
            temp.bpp = 24;
            temp.clr = "RGB";
            temp.wh[0] = 8;
            temp.wh[1] = 8;
            temp.wh[2] = 8;
            break;

        case 32:
            if (head.biCompression != CR_BI_RGB &&
                head.biCompression != CR_BI_BITFIELDS) {
                err_set(__CR_BMP_C__, head.biCompression,
                        "load_cr_bmp()", "invalid BMP format");
                return (NULL);
            }
            fcrh = CR_ARGB8888;
            temp.fmt = CR_PIC_ARGB;
            temp.bpp = 32;
            temp.clr = "ARGB";
            temp.wh[0] = 8;
            temp.wh[1] = 8;
            temp.wh[2] = 8;
            temp.wh[3] = 8;
            break;

        default:
        case 0: /* PNG/JPEG 不支持 */
            err_set(__CR_BMP_C__, head.biBitCount,
                    "load_cr_bmp()", "invalid BMP format");
            return (NULL);
    }
    /* BMP 一定是使用行4字节对齐的 */
    temp.pic = image_new(0, 0, ww, hh, fcrh, flip, 4);
    if (temp.pic == NULL) {
        err_set(__CR_BMP_C__, CR_NULL,
                "load_cr_bmp()", "image_new() failure");
        return (NULL);
    }

    /* 读取图片数据 */
    if (!CR_VCALL(datin)->seek(datin, head.biOffset, SEEK_SET)) {
        err_set(__CR_BMP_C__, FALSE,
                "load_cr_bmp()", "iDATIN::seek() failure");
        goto _failure;
    }
    switch (head.biCompression)
    {
        case CR_BI_RGB:
        case CR_BI_BITFIELDS:
            read = CR_VCALL(datin)->read(datin, temp.pic->data,
                                         temp.pic->size);
            if (read != temp.pic->size) {
                err_set(__CR_BMP_C__, read,
                        "load_cr_bmp()", "iDATIN::read() failure");
                goto _failure;
            }
            break;

        case CR_BI_RLE4:
            if (!bmp_unrle4(datin, temp.pic)) {
                err_set(__CR_BMP_C__, FALSE,
                        "load_cr_bmp()", "bmp_unrle4() failure");
                goto _failure;
            }
            break;

        case CR_BI_RLE8:
            if (!bmp_unrle8(datin, temp.pic)) {
                err_set(__CR_BMP_C__, FALSE,
                        "load_cr_bmp()", "bmp_unrle8() failure");
                goto _failure;
            }
            break;

        default:
            err_set(__CR_BMP_C__, head.biCompression,
                    "load_cr_bmp()", "invalid BMP format");
            goto _failure;
    }

    /* 复制调色板数据 */
    if (head.biBitCount <= 8) {
        for (fcrh = 0; fcrh < 256; fcrh++)
            pal[fcrh * 4 + 3] = 0xFF;
        mem_cpy(temp.pic->pal, pal, 1024);
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL) {
        err_set(__CR_BMP_C__, CR_NULL,
                "load_cr_bmp()", "struct_new() failure");
        goto _failure;
    }
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        err_set(__CR_BMP_C__, CR_NULL,
                "load_cr_bmp()", "struct_dup() failure");
        mem_free(rett);
        goto _failure;
    }
    CR_NOUSE(param);
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = "Windows Bitmap File (BMP)";
    return (rett);

_failure:
    image_del(temp.pic);
    return (NULL);
}

#endif  /* !__CR_BMP_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
