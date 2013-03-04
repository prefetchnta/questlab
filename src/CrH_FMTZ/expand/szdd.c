/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-03-04  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-03-04  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 微软 SZDD 文件解码函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_SZDD_C__
#define __CR_SZDD_C__ 0x7232133BUL

#include "enclib.h"
#include "fmtz/expand.h"

/* SZDD 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* SZDD 文件头结构 */
CR_TYPEDEF struct
{
        byte_t  tag[8];     /* "SZDD\x88\xF0\x27\x33" */
        byte_t  ftype;      /* 文件压缩类型 =0x41 */
        byte_t  ext_chr;    /* 下划线替换掉的那个字符 */
        int32u  unsize;     /* 文件解压缩后的大小 */

} CR_PACKED sSZDD_HDR;

/* SZ 文件头结构 */
CR_TYPEDEF struct
{
        byte_t  tag[8];     /* "SZ\x20\x88\xF0\x27\x33\xD1" */
        int32u  unsize;     /* 文件解压缩后的大小 */

} CR_PACKED sSZ_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    SZDD 文件读取
=======================================
*/
CR_API sFMT_DAT*
load_ms_szdd (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    leng_t      pksz;
    leng_t      unsz;
    void_t*     data;
    void_t*     temp;
    sFMT_DAT*   rett;
    sSZDD_HDR   head;

    /* 这个参数可能为空 */
    if (datin == NULL) {
        err_set(__CR_SZDD_C__, CR_NULL,
                "load_ms_szdd()", "invalid param: datin");
        return (NULL);
    }

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->getT(datin, &head, sSZDD_HDR))) {
        err_set(__CR_SZDD_C__, FALSE,
                "load_ms_szdd()", "iDATIN::getT() failure");
        return (NULL);
    }
    if (mem_cmp(head.tag, "SZDD\x88\xF0\x27\x33", 8) != 0) {
        err_set(__CR_SZDD_C__, CR_ERROR,
                "load_ms_szdd()", "invalid SZDD format");
        return (NULL);
    }
    if (head.ftype != 0x41) {
        err_set(__CR_SZDD_C__, head.ftype,
                "load_ms_szdd()", "invalid SZDD format");
        return (NULL);
    }

    /* 读取所有后续数据 */
    temp = CR_VCALL(datin)->get(datin, &pksz, FALSE);
    if (temp == NULL) {
        err_set(__CR_SZDD_C__, CR_NULL,
                "load_ms_szdd()", "iDATIN::get() failure");
        return (NULL);
    }

    /* 分配目标数据缓冲 */
    head.unsize = DWORD_LE(head.unsize);
    data = mem_malloc32(head.unsize);
    if (data == NULL) {
        err_set(__CR_SZDD_C__, CR_NULL,
                "load_ms_szdd()", "mem_malloc32() failure");
        mem_free(temp);
        return (NULL);
    }
    unsz = (leng_t)(head.unsize);
    pksz = uncompr_lz32(data, unsz, temp, pksz);
    mem_free(temp);
    if (pksz != unsz) {
        err_set(__CR_SZDD_C__, pksz,
                "load_ms_szdd()", "uncompr_lz32() failure");
        goto _failure;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_DAT);
    if (rett == NULL) {
        err_set(__CR_SZDD_C__, CR_NULL,
                "load_ms_szdd()", "struct_new() failure");
        goto _failure;
    }
    CR_NOUSE(param);
    rett->type = CR_FMTZ_DEC;
    rett->unsz = unsz;
    rett->pksz = dati_get_size(datin);
    rett->data = data;
    rett->infor = "Microsoft Compressed (SZDD) file";
    return (rett);

_failure:
    mem_free(data);
    return (NULL);
}

/*
=======================================
    SZ 文件读取
=======================================
*/
CR_API sFMT_DAT*
load_ms_sz (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    leng_t      pksz;
    leng_t      unsz;
    void_t*     data;
    void_t*     temp;
    sSZ_HDR     head;
    sFMT_DAT*   rett;

    /* 这个参数可能为空 */
    if (datin == NULL) {
        err_set(__CR_SZDD_C__, CR_NULL,
                "load_ms_sz()", "invalid param: datin");
        return (NULL);
    }

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->getT(datin, &head, sSZ_HDR))) {
        err_set(__CR_SZDD_C__, FALSE,
                "load_ms_sz()", "iDATIN::getT() failure");
        return (NULL);
    }
    if (mem_cmp(head.tag, "SZ\x20\x88\xF0\x27\x33\xD1", 8) != 0) {
        err_set(__CR_SZDD_C__, CR_ERROR,
                "load_ms_sz()", "invalid SZ format");
        return (NULL);
    }

    /* 读取所有后续数据 */
    temp = CR_VCALL(datin)->get(datin, &pksz, FALSE);
    if (temp == NULL) {
        err_set(__CR_SZDD_C__, CR_NULL,
                "load_ms_sz()", "iDATIN::get() failure");
        return (NULL);
    }

    /* 分配目标数据缓冲 */
    head.unsize = DWORD_LE(head.unsize);
    data = mem_malloc32(head.unsize);
    if (data == NULL) {
        err_set(__CR_SZDD_C__, CR_NULL,
                "load_ms_sz()", "mem_malloc32() failure");
        mem_free(temp);
        return (NULL);
    }
    unsz = (leng_t)(head.unsize);
    pksz = uncompr_lzss(data, unsz, temp, pksz, 0x20);
    mem_free(temp);
    if (pksz != unsz) {
        err_set(__CR_SZDD_C__, pksz,
                "load_ms_sz()", "uncompr_lzss() failure");
        goto _failure;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_DAT);
    if (rett == NULL) {
        err_set(__CR_SZDD_C__, CR_NULL,
                "load_ms_sz()", "struct_new() failure");
        goto _failure;
    }
    CR_NOUSE(param);
    rett->type = CR_FMTZ_DEC;
    rett->unsz = unsz;
    rett->pksz = dati_get_size(datin);
    rett->data = data;
    rett->infor = "Microsoft Compressed (SZ) file";
    return (rett);

_failure:
    mem_free(data);
    return (NULL);
}

#endif  /* !__CR_SZDD_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
