/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-20  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack PNG 图片读取函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "fmtz.h"
#include "safe.h"
#include "enclib.h"
#include "pixels.h"

/*
=======================================
    PNG 基本行过滤器
=======================================
*/
CR_API bool_t
png_filter (
  __CR_IO__ byte_t* data,
  __CR_IN__ leng_t  bpl,
  __CR_IN__ leng_t  bpp,
  __CR_IN__ uint_t  height,
  __CR_IN__ leng_t  size
    )
{
    leng_t  xx, line;
    sint_t  pa, pb, pc;
    uint_t  aa, bb, cc, pp, yy;
    byte_t  type, *dst = data;

    if (size < (bpl + 1) * height)
        return (FALSE);

    /* 第一行超出边界的点都按0来算 */
    type = *data++;
    for (xx = 0; xx < bpl; xx++)
        dst[xx] = *data++;

    switch (type)
    {
        case 0:     /* none */
        case 2:     /* up */
            break;

        case 1:     /* sub */
        case 4:     /* peath */
            for (xx = bpp; xx < bpl; xx++)
                dst[xx] = dst[xx] + dst[xx - bpp];
            break;

        case 3:     /* average */
            for (xx = bpp; xx < bpl; xx++)
                dst[xx] = dst[xx] + dst[xx - bpp] / 2;
            break;

        default:
            return (FALSE);
    }

    /* 后续行按正常的方式计算 */
    for (yy = 1, line = bpl; yy < height; yy++, line += bpl)
    {
        type = *data++;
        for (xx = 0; xx < bpl; xx++)
            dst[line + xx] = *data++;

        switch (type)
        {
            case 0:     /* none */
                break;

            case 1:     /* sub */
                for (xx = bpp; xx < bpl; xx++)
                    dst[line + xx] = dst[line + xx] + dst[line + xx - bpp];
                break;

            case 2:     /* up */
                for (xx = 0; xx < bpl; xx++)
                    dst[line + xx] = dst[line + xx] + dst[line - bpl + xx];
                break;

            case 3:     /* average */
                for (xx = 0; xx < bpp; xx++)
                    dst[line + xx] = dst[line + xx] + dst[line - bpl + xx] / 2;

                for (xx = bpp; xx < bpl; xx++) {
                    pp = dst[line + xx - bpp] + dst[line - bpl + xx];
                    dst[line + xx] = dst[line + xx] + (byte_t)(pp / 2);
                }
                break;

            case 4:     /* peath */
                for (xx = 0; xx < bpp; xx++)
                    dst[line + xx] = dst[line + xx] + dst[line - bpl + xx];

                for (xx = bpp; xx < bpl; xx++)
                {
                    aa = dst[line + xx - bpp];
                    bb = dst[line - bpl + xx];
                    cc = dst[line - bpl + xx - bpp];
                    pp = aa + bb - cc;

                    pa = pp - aa;
                    pb = pp - bb;
                    pc = pp - cc;
                    if (pa < 0) pa = -pa;
                    if (pb < 0) pb = -pb;
                    if (pc < 0) pc = -pc;

                    if (pa <= pb && pa <= pc)
                        dst[line + xx] = dst[line + xx] + (byte_t)aa;
                    else if (pb <= pc)
                        dst[line + xx] = dst[line + xx] + (byte_t)bb;
                    else
                        dst[line + xx] = dst[line + xx] + (byte_t)cc;
                }
                break;

            default:
                return (FALSE);
        }
    }
    return (TRUE);
}

/* PNG 内部结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
        int32u  size;   /* BE */
        int32u  name;   /* NO */
#if 0                   /* 这里跟具体的块有关, 这里省略 */
        byte_t  data[size];
        int32u  crc32;  /* BE (从 name 算到 data [CRC32/IEEE]) */
#endif
} CR_PACKED sCHUNK;

/* IHDR 结构 */
CR_TYPEDEF struct
{
        sCHUNK  head;
        int32u  w, h;       /* BE */
        byte_t  depth;      /* 像素位数: 1/2/4/8/16 */
        byte_t  color;      /* 颜色类型: 0 - 灰度图像 1/2/4/8/16
                                         2 - 真彩图像 8/16
                                         3 - 索引图像 1/2/4/8
                                         4 - 带α通道数据的灰度图像 8/16
                                         6 - 带α通道数据的真彩图像 8/16
                            */
        byte_t  compr;      /* 压缩类型: 0 - deflate */
        byte_t  filter;     /* 过滤类型: 0 - 基本过滤器 */
        byte_t  interlace;  /* 隔行扫描方法: 0 - 非隔行扫描
                                             1 - Adam7 (7遍隔行扫描方法)
                            */
        int32u  crc32;

} CR_PACKED sIHDR;

/* PLTE 结构 */
CR_TYPEDEF struct
{
        sCHUNK  head;
#if 0
        byte_t  pal[clr_num * 3];   /* 调色板: RGB * clr_num (R 在前) */
        int32u  crc32;
#endif
} CR_PACKED sPLTE;

/* tRNS 结构 */
CR_TYPEDEF struct
{
        sCHUNK  head;
#if 0
    #if   (color == 0)
        int16u  grey_trans;     /* 灰度的透明色 (BE) */
    #elif (color == 2)
        int16u  r, g, b;        /* 真彩的透明色 (BE) */
    #elif (color == 3)
        byte_t  alp[alp_num];   /* 调色板的α通道数据, 可以小于 clr_num */
    #endif
        int32u  crc32;
#endif
} CR_PACKED stRNS;

/* IDAT 结构 */
CR_TYPEDEF struct
{
        sCHUNK  head;
#if 0
        byte_t  data[head.size];    /* 压缩的数据 (可以有多个 IDAT 块) */
        int32u  crc32;
#endif
} CR_PACKED sIDAT;

/* IEND 结构 (文件结束块) */
CR_TYPEDEF struct
{
        sCHUNK  head;   /* head.size = 0x0 */
        int32u  crc32;  /* = (AE 42 60 82) */

} CR_PACKED sIEND;

/* PNG 文件头结构 */
CR_TYPEDEF struct
{
        byte_t  m[8];
        sIHDR   info;

} CR_PACKED sPNG_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    PNG 文件读取
=======================================
*/
CR_API sFMT_PIC*
load_cr_png (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    leng_t  nbpl;
    leng_t  sbpp;
    leng_t  dptr;
    uint_t  fcrh;
    uint_t  index;
    uchar*  image;
    uchar*  sdata;
    uchar*  ddata;
    leng_t  dsize;
    int32u  ssize;
    uint_t  ww, hh;
    byte_t  pal[768];
    /* ----------- */
    fsize_t     fsze;
    sPNG_HDR    head;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 这个参数可能为空 */
    if (datin == NULL)
        return (NULL);

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->geType(datin, &head, sPNG_HDR)))
        return (NULL);
    if (mem_cmp(&head, "\x89PNG\r\n\x1A\n\0\0\0\x0DIHDR", 16) != 0)
        return (NULL);
    if (head.info.compr != 0 || head.info.filter != 0 ||
        head.info.interlace != 0)
        return (NULL);

    /* 对宽高的截断检查 */
    if (cut_int32_u(&ww, DWORD_BE(head.info.w)))
        return (NULL);
    if (cut_int32_u(&hh, DWORD_BE(head.info.h)))
        return (NULL);

    /* 生成图片对象 */
    mem_zero(temp.wh, sizeof(temp.wh));
    switch (head.info.color)
    {
        case 0: /* 灰度图像 */
            if (head.info.depth != 1 &&
                head.info.depth != 2 &&
                head.info.depth != 4 &&
                head.info.depth != 8 &&
                head.info.depth != 16)
                return (NULL);
            fcrh = CR_INDEX8;
            temp.fmt = CR_PIC_GREY;
            temp.bpp = head.info.depth;
            temp.clr = "I";
            temp.wh[0] = head.info.depth;
            break;

        case 2: /* 真彩图像 */
            if (head.info.depth != 8 &&
                head.info.depth != 16)
                return (NULL);
            fcrh = CR_ARGB888;
            temp.fmt = CR_PIC_ARGB;
            temp.bpp = head.info.depth * 3;
            temp.clr = "BGR";
            temp.wh[0] = head.info.depth;
            temp.wh[1] = head.info.depth;
            temp.wh[2] = head.info.depth;
            break;

        case 3: /* 索引图像 */
            if (head.info.depth != 1 &&
                head.info.depth != 2 &&
                head.info.depth != 4 &&
                head.info.depth != 8)
                return (NULL);
            fcrh = CR_INDEX8;
            temp.fmt = CR_PIC_PALS;
            temp.bpp = head.info.depth;
            temp.clr = "P";
            temp.wh[0] = head.info.depth;
            break;

        case 4: /* α灰度图像 */
            if (head.info.depth != 8 &&
                head.info.depth != 16)
                return (NULL);
            fcrh = CR_ARGB8888;
            temp.fmt = CR_PIC_GREY;
            temp.bpp = head.info.depth * 2;
            temp.clr = "AI";
            temp.wh[0] = head.info.depth;
            temp.wh[1] = head.info.depth;
            break;

        case 6: /* α真彩图像 */
            if (head.info.depth != 8 &&
                head.info.depth != 16)
                return (NULL);
            fcrh = CR_ARGB8888;
            temp.fmt = CR_PIC_ARGB;
            temp.bpp = head.info.depth * 4;
            temp.clr = "ABGR";
            temp.wh[0] = head.info.depth;
            temp.wh[1] = head.info.depth;
            temp.wh[2] = head.info.depth;
            temp.wh[3] = head.info.depth;
            break;

        default:
            return (NULL);
    }
    sbpp = (temp.bpp - 1) / 8 + 1;
    temp.pic = image_new(0, 0, ww, hh, fcrh, FALSE, 4);
    if (temp.pic == NULL)
        return (NULL);

    /* 生成灰度调色板 */
    if (temp.fmt == CR_PIC_GREY)
        pal_set_gray8(temp.pic->pal, 256);

    /* 分配 IDAT 的内存 */
    fsze = dati_get_size(datin);
    if (fsze <= sizeof(sPNG_HDR) + sizeof(sIEND) * 2)
        goto _failure1;
    fsze -= sizeof(sPNG_HDR) + sizeof(sIEND) * 2;
    ddata = (byte_t*)mem_malloc64(fsze);
    if (ddata == NULL)
        goto _failure1;
    dsize = (leng_t)fsze;

    /* 读取数据块 */
    dptr = 0;
    fcrh = 256;         /* 这个保存调色板颜色数 */
    do
    {
        /* 数据块大小 */
        if (!(CR_VCALL(datin)->geType(datin, &head.info, sCHUNK)))
            goto _failure2;
        ssize = DWORD_BE(head.info.head.size);
        if (ssize > fsze)
            goto _failure2;

        if (head.info.head.name == mk_tag4("PLTE"))
        {
            /* 调色板, 安全检查 */
            if (ssize > 768 || ssize % 3 != 0)
                goto _failure2;
            if (CR_VCALL(datin)->read(datin, pal, ssize) != ssize)
                goto _failure2;

            /* 转换到 4B 格式 */
            fcrh = (uint_t)ssize / 3;
            pal_3b_to_4b_sw(temp.pic->pal, pal, fcrh);
        }
        else
        if (head.info.head.name == mk_tag4("IDAT"))
        {
            /* 检查缓冲溢出 */
            if (dsize < ssize)
                goto _failure2;
            if (CR_VCALL(datin)->read(datin, ddata + dptr, ssize) != ssize)
                goto _failure2;
            dptr  += ssize;
            dsize -= ssize;
        }
        else
        if (head.info.head.name == mk_tag4("tRNS"))
        {
            /* 透明数据 */
            if (head.info.color == 0) {
                if (ssize != 2)
                    goto _failure2;
                if (CR_VCALL(datin)->read(datin, pal, 2) != 2)
                    goto _failure2;

                /* 调色板的这个颜色为透明色 */
                if (head.info.depth != 16)
                    temp.pic->pal[pal[1]] &= CDWORD_LE(0x00FFFFFFUL);
            }
            else
            if (head.info.color == 2) {
                if (ssize != 6)
                    goto _failure2;
                if (CR_VCALL(datin)->read(datin, pal, 6) != 6)
                    goto _failure2;

                /* 这个颜色为透明色, 这里只能展开来写
                   否则 C++Builder 2010 编译器编译时会崩溃 */
                if (head.info.depth != 16) {
                    temp.pic->keycolor  = pal[1];
                    temp.pic->keycolor <<= 8;
                    temp.pic->keycolor |= pal[3];
                    temp.pic->keycolor <<= 8;
                    temp.pic->keycolor |= pal[5];
                    temp.pic->keycolor |= 0xFF000000UL;
                    temp.pic->keycolor = DWORD_LE(temp.pic->keycolor);
                }
            }
            else
            if (head.info.color == 3) {
                if (ssize > fcrh)
                    goto _failure2;
                if (CR_VCALL(datin)->read(datin, pal, ssize) != ssize)
                    goto _failure2;

                /* 设置调色板的 Alpha 通道 */
                for (fcrh = (uint_t)ssize, index = 0; index < fcrh; index++)
                    ((uchar*)temp.pic->pal)[index * 4 + 3] = pal[index];
            }
            else {
                goto _failure2;
            }
        }
        else
        {
            /* 跳过其他数据块 */
            if (!CR_VCALL(datin)->seek(datin, ssize, SEEK_CUR))
                goto _failure2;
        }

        /* 跳过 CRC-32 */
        if (!CR_VCALL(datin)->seek(datin, 4, SEEK_CUR))
            goto _failure2;
    } while (head.info.head.name != mk_tag4("IEND"));

    /* 无 IDAT 块 */
    if (dptr == 0)
        goto _failure2;

    /* 分配带 filter 的图形内存 */
    if (cut_mad(&dsize, ww, sbpp * hh, hh))
        goto _failure2;
    sdata = (byte_t*)mem_malloc(dsize);
    if (sdata == NULL)
        goto _failure2;

    /* 解压图形数据 */
    dptr = uncompr_zlib(sdata, dsize, ddata, dptr);
    mem_free(ddata);
    ddata = sdata;
    if (dptr <= hh)
        goto _failure2;
    image = temp.pic->data;

    /* 文件解码完毕, 解析图片的像素数据 */
    if (temp.pic->fmt == CR_INDEX8)
    {
        switch (head.info.depth)
        {
            case 1:
                if (ww % 8 == 0)
                    nbpl = ww / 8;
                else
                    nbpl = ww / 8 + 1;
                if (!png_filter(ddata, nbpl, 1, hh, dptr))
                    goto _failure2;
                for (index = hh; index != 0; index--) {
                    sdata = (uchar*)font1_h2l(image, sdata, ww);
                    image += temp.pic->bpl;
                }
                break;

            case 2:
                if (ww % 4 == 0)
                    nbpl = ww / 4;
                else
                    nbpl = ww / 4 + 1;
                if (!png_filter(ddata, nbpl, 1, hh, dptr))
                    goto _failure2;
                for (index = hh; index != 0; index--) {
                    sdata = (uchar*)font2_h2l(image, sdata, ww);
                    image += temp.pic->bpl;
                }
                break;

            case 4:
                if (ww % 2 == 0)
                    nbpl = ww / 2;
                else
                    nbpl = ww / 2 + 1;
                if (!png_filter(ddata, nbpl, 1, hh, dptr))
                    goto _failure2;
                for (index = hh; index != 0; index--) {
                    sdata = (uchar*)font4_h2l(image, sdata, ww);
                    image += temp.pic->bpl;
                }
                break;

            case 8:
                nbpl = ww;
                if (!png_filter(ddata, nbpl, 1, hh, dptr))
                    goto _failure2;
                for (index = hh; index != 0; index--) {
                    mem_cpy(image, sdata, nbpl);
                    sdata += nbpl;
                    image += temp.pic->bpl;
                }
                break;

            default:
            case 16:
                nbpl = ww;
                nbpl *= 2;
                if (!png_filter(ddata, nbpl, 2, hh, dptr))
                    goto _failure2;
                for (index = hh; index != 0; index--) {
                    for (fcrh = 0; fcrh < ww; fcrh++, sdata += 2)
                        image[fcrh] = sdata[0];
                    image += temp.pic->bpl;
                }
                break;
        }
    }
    else
    {
        nbpl = ww * sbpp;
        if (!png_filter(ddata, nbpl, sbpp, hh, dptr))
            goto _failure2;
        nbpl = ww * temp.pic->bpc;

        switch (head.info.color)
        {
            case 2:
                if (head.info.depth == 8)
                {
                    /* 直接逐行复制 */
                    for (index = hh; index != 0; index--) {
                        for (dsize = 0; dsize < nbpl; dsize += 3) {
                            image[dsize + 0] = sdata[2];
                            image[dsize + 1] = sdata[1];
                            image[dsize + 2] = sdata[0];
                            sdata += 3;
                        }
                        image += temp.pic->bpl;
                    }
                }
                else
                {
                    /* 跳开一个像素复制 */
                    for (index = hh; index != 0; index--) {
                        for (dsize = 0; dsize < nbpl; dsize += 3) {
                            image[dsize + 0] = sdata[4];
                            image[dsize + 1] = sdata[2];
                            image[dsize + 2] = sdata[0];
                            sdata += 6;
                        }
                        image += temp.pic->bpl;
                    }
                }
                break;

            case 4:
                if (head.info.depth == 8)
                {
                    /* 直接逐行复制 */
                    for (index = hh; index != 0; index--) {
                        for (dsize = 0; dsize < nbpl; dsize += 4) {
                            image[dsize + 0] = sdata[0];
                            image[dsize + 1] = sdata[0];
                            image[dsize + 2] = sdata[0];
                            image[dsize + 3] = sdata[1];
                            sdata += 2;
                        }
                        image += temp.pic->bpl;
                    }
                }
                else
                {
                    /* 跳开一个像素复制 */
                    for (index = hh; index != 0; index--) {
                        for (dsize = 0; dsize < nbpl; dsize += 4) {
                            image[dsize + 0] = sdata[0];
                            image[dsize + 1] = sdata[0];
                            image[dsize + 2] = sdata[0];
                            image[dsize + 3] = sdata[2];
                            sdata += 4;
                        }
                        image += temp.pic->bpl;
                    }
                }
                break;

            default:
            case 6:
                if (head.info.depth == 8)
                {
                    /* 直接逐行复制 */
                    for (index = hh; index != 0; index--) {
                        for (dsize = 0; dsize < nbpl; dsize += 4) {
                            image[dsize + 0] = sdata[2];
                            image[dsize + 1] = sdata[1];
                            image[dsize + 2] = sdata[0];
                            image[dsize + 3] = sdata[3];
                            sdata += 4;
                        }
                        image += temp.pic->bpl;
                    }
                }
                else
                {
                    /* 跳开一个像素复制 */
                    for (index = hh; index != 0; index--) {
                        for (dsize = 0; dsize < nbpl; dsize += 4) {
                            image[dsize + 0] = sdata[4];
                            image[dsize + 1] = sdata[2];
                            image[dsize + 2] = sdata[0];
                            image[dsize + 3] = sdata[6];
                            sdata += 8;
                        }
                        image += temp.pic->bpl;
                    }
                }
                break;
        }
    }
    mem_free(ddata);

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL)
        goto _failure1;
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        mem_free(rett);
        goto _failure1;
    }
    CR_NOUSE(param);
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = "Portable Network Graphics (PNG)";
    return (rett);

_failure2:
    mem_free(ddata);
_failure1:
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
