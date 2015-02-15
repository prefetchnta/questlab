/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-10-09  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack TGL CGR 图片读取函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "pixels.h"
#include "fmtz/tgl.h"

/*
=======================================
    TGL CGR 文件读取
=======================================
*/
CR_API sFMT_PIC*
load_tgl_cgr (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    int32u  wnh[2];
    uint_t  ww, hh;
    /*************/
    byte_t*     line;
    fsize_t     size;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 这个参数可能为空 */
    if (datin == NULL)
        return (NULL);

    /* 只能根据文件大小来判断 */
    size = dati_get_size(datin);
    if (size <= 0x600 + 12)
        return (NULL);
    if (!CR_VCALL(datin)->seek(datin, 0x600, SEEK_SET))
        return (NULL);
    if (CR_VCALL(datin)->read(datin, wnh, 8) != 8)
        return (NULL);
    wnh[0] = DWORD_LE(wnh[0]);
    wnh[1] = DWORD_LE(wnh[1]);
    size  = wnh[0];
    size *= wnh[1];
    size += 0x600 + 12;
    if (size != dati_get_size(datin))
        return (NULL);

    /* 对宽高的截断检查 */
    if (cut_int32_u(&ww, wnh[0]))
        return (NULL);
    if (cut_int32_u(&hh, wnh[1]))
        return (NULL);

    /* 定位到调色板 */
    if (!CR_VCALL(datin)->rewind(datin))
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
    if (CR_VCALL(datin)->read(datin, temp.pic->pal, 1024) != 1024)
        goto _failure;
    line = temp.pic->data;
    pal_4b_alp_sw(temp.pic->pal, TRUE, 0xFF, 256);

    /* 定位到图片数据 */
    if (!CR_VCALL(datin)->seek(datin, 0x600 + 12, SEEK_SET))
        goto _failure;

    /* 读取图片数据 */
    for (; hh != 0; hh--) {
        if (CR_VCALL(datin)->read(datin, line, ww) != ww)
            goto _failure;
        line += temp.pic->bpl;
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
    rett->infor = "TGL CGR Image File (CGR)";
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
