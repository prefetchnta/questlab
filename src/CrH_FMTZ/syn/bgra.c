/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-09-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack ARGB 图片读取函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
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
    uint_t  ww, hh;
    leng_t  ii, bpl2;
    /* ----------- */
    sFMT_PIC*   rett;
    sARGB_HDR   head;
    sFMT_FRAME  temp;

    /* 这个参数可能为空 */
    if (datin == NULL)
        return (NULL);

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->geType(datin, &head, sARGB_HDR)))
        return (NULL);
    if (head.dwIdentifier != mk_tag4("BGRA"))
        return (NULL);

    /* 对宽高的截断检查 */
    if (cut_int32_u(&ww, DWORD_LE(head.dwWidth)))
        return (NULL);
    if (cut_int32_u(&hh, DWORD_LE(head.dwHeight)))
        return (NULL);

    /* 生成图片对象 */
    temp.clr = "ARGB";
    temp.fmt = CR_PIC_ARGB;
    mem_zero(temp.wh, sizeof(temp.wh));
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
        return (NULL);
    }
    temp.pic = image_new(0, 0, ww, hh, CR_ARGB8888, FALSE, 4);
    if (temp.pic == NULL)
        return (NULL);

    /* 读取图片数据 */
    if (head.dwPixelFormat == 0x08080808UL) {
        if (CR_VCALL(datin)->read(datin, temp.pic->data,
                temp.pic->size) != temp.pic->size)
            goto _failure;
    }
    else {
        bpl2 = temp.pic->bpl * 2;
        line = (byte_t*)mem_malloc(bpl2);
        if (line == NULL)
            goto _failure;

        /* 隔一个字节复制像素 */
        for (pntr = temp.pic->data; hh != 0; hh--) {
            if (CR_VCALL(datin)->read(datin, line, bpl2) != bpl2) {
                mem_free(line);
                goto _failure;
            }
            for (ii = 0; ii < bpl2; ii += 2)
                *pntr++ = line[ii + 1];
        }
        mem_free(line);
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
    rett->infor = "ARGB Image File (by Shin-ichi TSURUTA)";
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
