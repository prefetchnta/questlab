/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-08-13  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack TGL IEL1 文件解码函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "fmtz/tgl.h"

/* IEL1 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* IEL1 文件头结构 */
CR_TYPEDEF struct
{
        int32u  magic;      /* 文件标志 "IEL1" */
        int32u  unsize;     /* 文件解压缩后的大小 */

} CR_PACKED sIEL1_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    TGL IEL1 文件读取
=======================================
*/
CR_API sFMT_DAT*
load_tgl_iel1 (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    leng_t      pksz;
    leng_t      unsz;
    void_t*     data;
    void_t*     temp;
    sFMT_DAT*   rett;
    sIEL1_HDR   head;

    /* 这个参数可能为空 */
    if (datin == NULL)
        return (NULL);

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->geType(datin, &head, sIEL1_HDR)))
        return (NULL);
    if (head.magic != mk_tag4("IEL1"))
        return (NULL);

    /* 读取所有后续数据 */
    temp = CR_VCALL(datin)->get(datin, &pksz, FALSE);
    if (temp == NULL)
        return (NULL);

    /* 分配目标数据缓冲 */
    head.unsize = DWORD_LE(head.unsize);
    data = mem_malloc32(head.unsize);
    if (data == NULL) {
        mem_free(temp);
        return (NULL);
    }
    unsz = (leng_t)(head.unsize);
    pksz = uncompr_lzss(data, unsz, temp, pksz, 0);
    mem_free(temp);
    if (pksz != unsz)
        goto _failure;

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_DAT);
    if (rett == NULL)
        goto _failure;
    CR_NOUSE(param);
    rett->type = CR_FMTZ_DEC;
    rett->unsz = unsz;
    rett->pksz = dati_get_size(datin);
    rett->data = data;
    rett->infor = "TGL LZSS Compressed file (IEL1)";
    return (rett);

_failure:
    mem_free(data);
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
