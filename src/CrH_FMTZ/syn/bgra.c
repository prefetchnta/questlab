/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-09-12  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-02-28  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack ARGB 图片读取函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_BGRA_C__
#define __CR_BGRA_C__ 0xD945B176UL

#include "safe.h"
#include "memlib.h"
#include "fmtz/syn.h"

/* ARGB 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* ARGB 文件头结构 */
CR_TYPEDEF struct
{
        int32u  dwIdentifier;   /* "BGRA" */
        int32u  dwPixelFormat;  /* 像素格式 */
        int32u  dwWidth;        /* 图片宽度 */
        int32u  dwHeight;       /* 图片高度 */

} CR_PACKED sARGB_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    ARGB 文件读取
=======================================
*/
CR_API sFMT_PIC*
load_syn_argb (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    uchar*  pntr;
    uchar*  line;
    leng_t  bpl2;
    leng_t  read;
    uint_t  ww, hh;
    /* ----------- */
    sFMT_PIC*   rett;
    sARGB_HDR   head;
    sFMT_FRAME  temp;

    /* 这个参数可能为空 */
    if (datin == NULL) {
        err_set(__CR_BGRA_C__, CR_NULL,
                "load_syn_argb()", "invalid param: datin");
        return (NULL);
    }

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->getT(datin, &head, sARGB_HDR))) {
        err_set(__CR_BGRA_C__, FALSE,
                "load_syn_argb()", "iDATIN::getT() failure");
        return (NULL);
    }
    if (head.dwIdentifier != mk_tag4("BGRA")) {
        err_set(__CR_BGRA_C__, head.dwIdentifier,
                "load_syn_argb()", "invalid ARGB format");
        return (NULL);
    }

    /* 对宽高的截断检查 */
    if (cut_int32_u(&ww, DWORD_LE(head.dwWidth))) {
        err_set(__CR_BGRA_C__, head.dwWidth,
                "load_syn_argb()", "image width truncated");
        return (NULL);
    }
    if (cut_int32_u(&hh, DWORD_LE(head.dwHeight))) {
        err_set(__CR_BGRA_C__, head.dwHeight,
                "load_syn_argb()", "image height truncated");
        return (NULL);
    }

    /* 生成图片对象 */
    temp.clr = "ARGB";
    temp.fmt = CR_PIC_ARGB;
    mem_zero(&temp.wh, sizeof(temp.wh));
    if (head.dwPixelFormat == 0x08080808UL) {
        temp.bpp = 32;
        temp.wh[0] = 8;
        temp.wh[1] = 8;
        temp.wh[2] = 8;
        temp.wh[3] = 8;
    }
    else
    if (head.dwPixelFormat == 0x10101010UL) {
        temp.bpp = 64;
        temp.wh[0] = 16;
        temp.wh[1] = 16;
        temp.wh[2] = 16;
        temp.wh[3] = 16;
    }
    else {
        err_set(__CR_BGRA_C__, head.dwPixelFormat,
                "load_syn_argb()", "invalid ARGB format");
        return (NULL);
    }
    temp.pic = image_new(0, 0, ww, hh, CR_ARGB8888, FALSE, 4);
    if (temp.pic == NULL) {
        err_set(__CR_BGRA_C__, CR_NULL,
                "load_syn_argb()", "image_new() failure");
        return (NULL);
    }

    /* 读取图片数据 */
    if (head.dwPixelFormat == 0x08080808UL) {
        read = CR_VCALL(datin)->read(datin, temp.pic->data, temp.pic->size);
        if (read != temp.pic->size) {
            err_set(__CR_BGRA_C__, read,
                    "load_syn_argb()", "iDATIN::read() failure");
            goto _failure;
        }
    }
    else {
        bpl2 = temp.pic->bpl * 2;
        line = (byte_t*)mem_malloc(bpl2);
        if (line == NULL) {
            err_set(__CR_BGRA_C__, CR_NULL,
                    "load_syn_argb()", "mem_malloc() failure");
            goto _failure;
        }
        /* 隔一个字节复制像素 */
        for (pntr = temp.pic->data; hh != 0; hh--) {
            read = CR_VCALL(datin)->read(datin, line, bpl2);
            if (read != bpl2) {
                err_set(__CR_BGRA_C__, read,
                        "load_syn_argb()", "iDATIN::read() failure");
                mem_free(line);
                goto _failure;
            }
            for (read = 0; read < bpl2; read += 2)
                *pntr++ = line[read + 1];
        }
        mem_free(line);
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL) {
        err_set(__CR_BGRA_C__, CR_NULL,
                "load_syn_argb()", "struct_new() failure");
        goto _failure;
    }
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        err_set(__CR_BGRA_C__, CR_NULL,
                "load_syn_argb()", "struct_dup() failure");
        mem_free(rett);
        goto _failure;
    }
    CR_NOUSE(param);
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = "ARGB Image File (by Shin-ichi TSURUTA)";
    return (rett);

_failure:
    image_del(temp.pic);
    return (NULL);
}

#endif  /* !__CR_BGRA_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
