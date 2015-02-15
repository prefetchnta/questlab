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
/*  >>>>>>>>>>>>>>>>>>>> CrHack FALCOM Z 文件解码函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "enclib.h"
#include "fmtz/falcom.h"

/* Z 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* Z 文件头结构 */
CR_TYPEDEF struct
{
        int32u  crc32;      /* 文件解压缩后的校验 */
        int32u  unsize;     /* 文件解压缩后的大小 */

} CR_PACKED sZED_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    FALCOM Z 文件读取
=======================================
*/
CR_API sFMT_DAT*
load_flc_zed (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    leng_t      pksz;
    leng_t      unsz;
    void_t*     data;
    void_t*     temp;
    sZED_HDR    head;
    sFMT_DAT*   rett;

    /* 这个参数可能为空 */
    if (datin == NULL)
        return (NULL);

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->geType(datin, &head, sZED_HDR)))
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
    pksz = uncompr_zlib(data, unsz, temp, pksz);
    mem_free(temp);
    if (pksz != unsz)
        goto _failure;

    /* 文件数据校验 (需要吗？) */
    head.unsize = hash_crc32i_total(data, unsz);
    if (head.unsize != DWORD_LE(head.crc32))
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
    rett->infor = "FALCOM ZLib Compressed file (*.Z)";
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
