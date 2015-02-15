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
        int32u  unknown1;       /* 未知数据值 */
        int32u  width;          /* 图片的宽度 */
        int32u  height;         /* 图片的高度 */
        int32u  dat_size;       /* 压缩数据大小 */
        byte_t  pal_count;      /* 调色板颜色数 */
        byte_t  unknown2[3];    /* 未知数据值 */

} CR_PACKED sBMR_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

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
    leng_t      cnts;
    fsize_t     size;
    sBMR_HDR    head;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;
    /***************/
    byte_t  pair[2], *ptr;
    uint_t  rle, ww, hh, ii;

    /* 这个参数可能为空 */
    if (datin == NULL)
        return (NULL);

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->geType(datin, &head, sBMR_HDR)))
        return (NULL);
    if (head.pal_count == 0)
        return (NULL);
    head.dat_size = DWORD_LE(head.dat_size);
    size = head.pal_count;
    size *= sizeof(int32u);
    size += head.dat_size;
    size += sizeof(sBMR_HDR);
    if (size != dati_get_size(datin))
        return (NULL);

    /* 对宽高的截断检查 */
    if (cut_int32_u(&ww, DWORD_LE(head.width)))
        return (NULL);
    if (cut_int32_u(&hh, DWORD_LE(head.height)))
        return (NULL);

    /* 生成图片对象 */
    mem_zero(temp.wh, sizeof(temp.wh));
    temp.fmt = CR_PIC_PALS;
    temp.bpp = 8;
    temp.clr = "P";
    temp.wh[0] = 8;
    temp.pic = image_new(0, 0, ww, hh, CR_INDEX8, FALSE, 4);
    if (temp.pic == NULL)
        return (NULL);

    /* 读取调色板数据 */
    cnts  = head.pal_count;
    cnts *= sizeof(int32u);
    if (CR_VCALL(datin)->read(datin, temp.pic->pal, cnts) != cnts)
        goto _failure;
    ii = 0;
    ptr = temp.pic->data;
    pal_4b_alp_sw(temp.pic->pal, TRUE, 0xFF, head.pal_count);

    /* 读取图片数据 */
    while (hh != 0) {
        if (!CR_VCALL(datin)->getb_no(datin, pair))
            goto _failure;

        /* 注意：RLE 是跨行压缩的 */
        if (pair[0] != 0xFF) {
            rle = 1;
        }
        else {
            if (CR_VCALL(datin)->read(datin, pair, 2) != 2)
                goto _failure;
            if (pair[1] == 0)
                rle = 256;
            else
                rle = pair[1];
        }

        /* 填充到目标图片 */
        for (; rle != 0; rle--) {
            ptr[ii++] = pair[0];
            if (ii == ww) {
                hh -= 1;
                if (hh == 0)
                    break;
                ptr += temp.pic->bpl;
                ii = 0;
            }
        }
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
    rett->infor = "TGL BMR Image File (BMR)";
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
