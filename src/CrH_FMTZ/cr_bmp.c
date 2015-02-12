/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-05-03  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack BMP 图片读取函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "fmtz.h"
#include "safe.h"
#include "pixels.h"

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
    leng_t  idx, cnt, offset = 0;
    leng_t  end_of_line = image->bpl;
    byte_t  pair[2], *data = image->data;

    mem_zero(image->data, image->size);
    while (offset < image->size)
    {
        /* 两个字节一对 */
        if (CR_VCALL(datin)->read(datin, pair, 2) != 2)
            return (FALSE);

        /* RLE 输出 */
        if (pair[0] != 0x00) {
            cnt = (leng_t)pair[0];
            if (cnt > image->size - offset)
                cnt = image->size - offset;
            pair[0] = pair[1] >> 4;
            pair[1] = pair[1] & 0x0F;
            for (idx = 0; idx < cnt; idx++)
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
                if (CR_VCALL(datin)->read(datin, pair, 2) != 2)
                    return (FALSE);
                offset += pair[0] + pair[1] * image->bpl;
                end_of_line += pair[1] * image->bpl;
                break;

            default:    /* 直接复制 */
                cnt = (leng_t)pair[1];
                if (cnt > image->size - offset)
                    cnt = image->size - offset;
                for (idx = 0; idx < cnt; idx++) {
                    if (idx & 1) {
                        data[offset++] = pair[0] & 0x0F;
                        continue;
                    }
                    if (CR_VCALL(datin)->read(datin, pair, 1) != 1)
                        return (FALSE);
                    data[offset++] = pair[0] >> 4;
                }
                cnt %= 4;
                if (cnt == 1 || cnt == 2) {
                    if (CR_VCALL(datin)->read(datin, pair, 1) != 1)
                        return (FALSE);
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
    leng_t  cnt, offset = 0;
    leng_t  end_of_line = image->bpl;
    byte_t  pair[2], *data = image->data;

    mem_zero(image->data, image->size);
    while (offset < image->size)
    {
        /* 两个字节一对 */
        if (CR_VCALL(datin)->read(datin, pair, 2) != 2)
            return (FALSE);

        /* RLE 输出 */
        if (pair[0] != 0x00) {
            cnt = (leng_t)pair[0];
            if (cnt > image->size - offset)
                cnt = image->size - offset;
            mem_set(data + offset, pair[1], cnt);
            offset += cnt;
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
                if (CR_VCALL(datin)->read(datin, pair, 2) != 2)
                    return (FALSE);
                offset += pair[0] + pair[1] * image->bpl;
                end_of_line += pair[1] * image->bpl;
                break;

            default:    /* 直接复制 */
                cnt = (leng_t)pair[1];
                if (cnt > image->size - offset)
                    cnt = image->size - offset;
                if (CR_VCALL(datin)->read(datin, data + offset, cnt) != cnt)
                    return (FALSE);
                if (cnt & 1) {
                    if (CR_VCALL(datin)->read(datin, pair, 1) != 1)
                        return (FALSE);
                }
                offset += cnt;
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
    sint_t  hh;
    bool_t  flip;
    uint_t  fcrh, ww;
    byte_t  mask[12];
    /* ----------- */
    sBMP_HDR    head;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 这个参数可能为空 */
    if (datin == NULL)
        return (NULL);

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->geType(datin, &head, sBMP_HDR)))
        return (NULL);
    if (head.biMagic != mk_tag2("BM") || head.biReserved != 0UL ||
        head.biInfoSize != CDWORD_LE(40) || head.biPlanes != CWORD_LE(1))
        return (NULL);
    head.biOffset = DWORD_LE(head.biOffset);
    if (head.biOffset < sizeof(sBMP_HDR) ||
        head.biOffset >= dati_get_size(datin))
        return (NULL);

    /* 对宽高的截断检查 */
    if (cut_int32_u(&ww, DWORD_LE(head.biWidth)))
        return (NULL);
    if (cut_int32_s(&hh, DWORD_LE(head.biHeight)))
        return (NULL);

    /* 图像是否颠倒 */
    if (hh < 0) {
        hh = -hh;
        flip = FALSE;
    }
    else {
        flip = TRUE;
    }

    /* 读取调色板区 */
    head.biBitCount = WORD_LE(head.biBitCount);
    head.biCompression = DWORD_LE(head.biCompression);
    if (head.biCompression == CR_BI_BITFIELDS) {
        if (CR_VCALL(datin)->read(datin, mask, 12) != 12)
            return (NULL);
    }

    /* 生成图片对象 */
    mem_zero(temp.wh, sizeof(temp.wh));
    switch (head.biBitCount)
    {
        case 1:
            if (head.biCompression != CR_BI_RGB)
                return (NULL);
            fcrh = CR_INDEX1;
            temp.fmt = CR_PIC_PALS;
            temp.bpp = 1;
            temp.clr = "P";
            temp.wh[0] = 1;
            break;

        case 4:
            if (head.biCompression != CR_BI_RGB &&
                head.biCompression != CR_BI_RLE4)
                return (NULL);
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
                head.biCompression != CR_BI_RLE8)
                return (NULL);
            fcrh = CR_INDEX8;
            temp.fmt = CR_PIC_PALS;
            temp.bpp = 8;
            temp.clr = "P";
            temp.wh[0] = 8;
            break;

        case 16:
            if (head.biCompression != CR_BI_RGB &&
                head.biCompression != CR_BI_BITFIELDS)
                return (NULL);
            temp.fmt = CR_PIC_ARGB;
            temp.bpp = 16;
            if (head.biCompression == CR_BI_RGB ||
                mem_cmp(mask, "\0\x7C\0\0\xE0\x03\0\0\x1F\0\0\0", 12) == 0)
            {
                /* 格式 X555 */
                fcrh = CR_ARGBX555;
                temp.clr = "RGB";
                temp.wh[0] = 5;
                temp.wh[1] = 5;
                temp.wh[2] = 5;
            }
            else
            if (mem_cmp(mask, "\0\xF8\0\0\xE0\x07\0\0\x1F\0\0\0", 12) == 0)
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
                return (NULL);
            }
            break;

        case 24:
            if (head.biCompression != CR_BI_RGB)
                return (NULL);
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
                head.biCompression != CR_BI_BITFIELDS)
                return (NULL);
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
            return (NULL);
    }
    /* BMP 一定是使用行4字节对齐的 */
    temp.pic = image_new(0, 0, ww, hh, fcrh, flip, 4);
    if (temp.pic == NULL)
        return (NULL);

    /* 读取调色板数据 */
    if (temp.bpp <= 8) {
        head.biClrUsed = DWORD_LE(head.biClrUsed);
        if (head.biClrUsed > 256)
            goto _failure;
        fcrh = (uint_t)head.biClrUsed;
        if (fcrh == 0)
            fcrh = ((uint_t)1) << temp.bpp;
        fcrh *= sizeof(int32u);
        if (CR_VCALL(datin)->read(datin, temp.pic->pal, fcrh) != fcrh)
            goto _failure;
        fcrh /= sizeof(int32u);
        pal_4b_alp_sw(temp.pic->pal, FALSE, 0xFF, fcrh);
    }

    /* 读取图片数据 */
    if (!CR_VCALL(datin)->seek(datin, head.biOffset, SEEK_SET))
        goto _failure;
    switch (head.biCompression)
    {
        case CR_BI_RGB:
        case CR_BI_BITFIELDS:
            if (CR_VCALL(datin)->read(datin, temp.pic->data,
                    temp.pic->size) != temp.pic->size)
                goto _failure;
            break;

        case CR_BI_RLE4:
            if (!bmp_unrle4(datin, temp.pic))
                goto _failure;
            break;

        case CR_BI_RLE8:
            if (!bmp_unrle8(datin, temp.pic))
                goto _failure;
            break;

        default:
            goto _failure;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL)
        goto _failure;
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
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

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
