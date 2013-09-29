/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-09-28  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack TGL BMR 图片读取函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_TGL_BMR_C__
#define __CR_TGL_BMR_C__ 0x6F9D3515UL

#include "safe.h"
#include "pixels.h"
#include "fmtz/tgl.h"

/* BMR 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* BMR 文件头结构 */
CR_TYPEDEF struct
{
        int32u  zero;           /* 0x00000000？ */
        int32u  width;          /* 图片的宽度 */
        int32u  height;         /* 图片的高度 */
        int32u  dat_size;       /* 压缩数据大小 */
        byte_t  pal_count;      /* 调色板颜色数 */
        byte_t  unknown[3];     /* 未知数据值 */

} CR_PACKED sBMR_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif
#include <windows.h>
/*
=======================================
    TGL BMR 文件读取
=======================================
*/
CR_API sFMT_PIC*
load_tgl_bmr (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    fsize_t     size;
    sBMR_HDR    head;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;
    /***************/
    leng_t  read, count;
    byte_t  pair[2], *ptr;
    uint_t  rle, ww, hh, ii;

    /* 这个参数可能为空 */
    if (datin == NULL) {
        err_set(__CR_TGL_BMR_C__, CR_NULL,
                "load_tgl_bmr()", "invalid param: datin");
        return (NULL);
    }

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->getT(datin, &head, sBMR_HDR))) {
        err_set(__CR_TGL_BMR_C__, FALSE,
                "load_tgl_bmr()", "iDATIN::getT() failure");
        return (NULL);
    }
    if (head.zero != 0x00000000UL) {
        err_set(__CR_TGL_BMR_C__, head.zero,
                "load_tgl_bmr()", "invalid BMR format");
        return (NULL);
    }
    if (head.pal_count == 0) {
        err_set(__CR_TGL_BMR_C__, head.pal_count,
                "load_tgl_bmr()", "invalid BMR format");
        return (NULL);
    }
    head.dat_size = DWORD_LE(head.dat_size);
    size = head.pal_count;
    size *= sizeof(int32u);
    size += head.dat_size;
    size += sizeof(sBMR_HDR);
    if (size != dati_get_size(datin)) {
        err_set(__CR_TGL_BMR_C__, size,
                "load_tgl_bmr()", "invalid BMR format");
        return (NULL);
    }

    /* 对宽高的截断检查 */
    if (cut_int32_u(&ww, DWORD_LE(head.width))) {
        err_set(__CR_TGL_BMR_C__, head.width,
                "load_tgl_bmr()", "image width truncated");
        return (NULL);
    }
    if (cut_int32_u(&hh, DWORD_LE(head.height))) {
        err_set(__CR_TGL_BMR_C__, head.height,
                "load_tgl_bmr()", "image height truncated");
        return (NULL);
    }

    /* 生成图片对象 */
    mem_zero(temp.wh, sizeof(temp.wh));
    temp.fmt = CR_PIC_PALS;
    temp.bpp = 8;
    temp.clr = "P";
    temp.wh[0] = 8;
    temp.pic = image_new(0, 0, ww, hh, CR_INDEX8, FALSE, 4);
    if (temp.pic == NULL) {
        err_set(__CR_TGL_BMR_C__, CR_NULL,
                "load_tgl_bmr()", "image_new() failure");
        return (NULL);
    }

    /* 读取调色板数据 */
    count  = head.pal_count;
    count *= sizeof(int32u);
    read = CR_VCALL(datin)->read(datin, temp.pic->pal, count);
    if (read != count) {
        err_set(__CR_TGL_BMR_C__, read,
                "load_tgl_bmr()", "iDATIN::read() failure");
        goto _failure;
    }
    ii = 0;
    ptr = temp.pic->data;
    pal_4b_alp_sw(temp.pic->pal, TRUE, 0xFF, head.pal_count);

    /* 读取图片数据 */
    while (hh != 0) {
        if (!CR_VCALL(datin)->getb_no(datin, pair)) {
            err_set(__CR_TGL_BMR_C__, FALSE,
                    "load_tgl_bmr()", "iDATIN::getb_no() failure");
            goto _failure;
        }

        /* 注意：RLE 是跨行压缩的 */
        if (pair[0] != 0xFF) {
            rle = 1;
        }
        else {
            read = CR_VCALL(datin)->read(datin, pair, 2);
            if (read != 2) {
                err_set(__CR_TGL_BMR_C__, read,
                        "load_tgl_bmr()", "iDATIN::read() failure");
                goto _failure;
            }
            if (pair[1] == 0)
                rle = 256;
            else
                rle = pair[1];
        }

        /* 填充到目标图片 */
        for (; rle != 0; rle--) {
            ptr[ii++] = pair[0];
            if (ii == ww) {
                if (hh == 0)
                    break;
                hh -= 1;
                ptr += temp.pic->bpl;
                ii = 0;
            }
        }
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL) {
        err_set(__CR_TGL_BMR_C__, CR_NULL,
                "load_tgl_bmr()", "struct_new() failure");
        goto _failure;
    }
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        err_set(__CR_TGL_BMR_C__, CR_NULL,
                "load_tgl_bmr()", "struct_dup() failure");
        mem_free(rett);
        goto _failure;
    }
    CR_NOUSE(param);
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = "TGL BMR Image File (BMR)";
    return (rett);

_failure:
    image_del(temp.pic);
    return (NULL);
}

#endif  /* !__CR_TGL_BMR_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
