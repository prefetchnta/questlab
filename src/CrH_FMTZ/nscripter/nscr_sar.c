/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-04-05  */
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
/*  >>>>>>>>>>>>>>>>> CrHack NScripter SAR 封包读取函数库 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "strlib.h"
#include "../fmtint.h"
#include "fmtz/nscripter.h"

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPACKAGE    pack;

        /* 个性部分 */
        uint_t  m_cnt;  /* 文件个数 */
        iDATIN* m_file; /* 文件对象 */

} iPAK_SAR;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPAK_SAR_release (
  __CR_IN__ iPACKAGE*   that
    )
{
    uint_t      idx;
    iPAK_SAR*   real;
    sPAK_FILE*  list;

    pack_free_list(that);
    real = (iPAK_SAR*)that;
    list = real->pack.__filelst__;
    for (idx = 0; idx < real->m_cnt; idx++) {
        mem_free(list[idx].find);
        mem_free(list[idx].name);
    }
    mem_free(list);
    CR_VCALL(real->m_file)->release(real->m_file);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iPAK_SAR_getMore (
  __CR_IN__ iPACKAGE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPACKAGE::NS_SAR") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    读取文件个数
---------------------------------------
*/
static int64u
iPAK_SAR_getFileNum (
  __CR_IN__ iPACKAGE*   that
    )
{
    return (((iPAK_SAR*)that)->m_cnt);
}

/*
---------------------------------------
    读取文件数据
---------------------------------------
*/
static bool_t
iPAK_SAR_getFileData (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sBUFFER*    buff,
  __CR_IN__ int64u      index,
  __CR_IN__ bool_t      hash
    )
{
    int64u      size;
    void_t*     data;
    iDATIN*     file;
    iPAK_SAR*   real;
    sPAK_FILE*  item;

    /* 定位文件索引 */
    CR_NOUSE(hash);
    real = (iPAK_SAR*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    item = real->pack.__filelst__;
    item += (uint_t)index;

    /* 获取文件数据 (0大小文件分配1个字节) */
    size = item->size;
    if (size == 0) {
        data = mem_malloc(1);
        if (data == NULL)
            return (FALSE);
        size = 1;
        *(byte_t*)data = 0x00;
    }
    else {
        data = mem_malloc64(size);
        if (data == NULL)
            return (FALSE);
        file = real->m_file;

        /* 定位到文件并读起数据 */
        if (!CR_VCALL(file)->seek64(file, item->offs, SEEK_SET))
            goto _failure;
        if (CR_VCALL(file)->read(file, data, (leng_t)size) != (leng_t)size)
            goto _failure;
    }

    /* 返回文件数据 */
    return (buffer_init(buff, data, (leng_t)size, TRUE));

_failure:
    mem_free(data);
    return (FALSE);
}

/*
---------------------------------------
    读取文件信息
---------------------------------------
*/
static bool_t
iPAK_SAR_getFileInfo (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sPAK_FILE** info,
  __CR_IN__ int64u      index
    )
{
    uint_t      idx;
    iPAK_SAR*   real;

    /* 定位文件索引 */
    real = (iPAK_SAR*)that;
    if (index >= real->m_cnt)
        return (FALSE);

    /* 返回文件信息 */
    idx = (uint_t)index;
    *info = &real->pack.__filelst__[idx];
    return (TRUE);
}

/* 接口虚函数表 */
static const iPACKAGE_vtbl _rom_ s_pack_vtbl =
{
    iPAK_SAR_release, iPAK_SAR_getMore,
    iPAK_SAR_getFileNum, iPAK_SAR_getFileData,
    iPAK_SAR_getFileInfo,
};

/*
=======================================
    SAR 文件读取
=======================================
*/
CR_API sFMT_PRT*
load_nscr_sar (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    int32u      beg;
    int16u      idx;
    int16u      cnt;
    byte_t      cha;
    uint_t      tmp;
    int32u      offs;
    int32u      size;
    iPAK_SAR*   port;
    sFMT_PRT*   rett;
    sPAK_FILE*  list;
    /* ----------- */
    ansi_t  str[MAX_PATHA];

    /* 必须使用自己私有的读取接口 */
    datin = create_file_inX(param);
    if (datin == NULL)
        return (NULL);

    /* 读取文件头信息 */
    if (!CR_VCALL(datin)->getw_be(datin, &cnt))
        goto _failure1;
    if (cnt == 0)
        goto _failure1;
    if (!CR_VCALL(datin)->getd_be(datin, &beg))
        goto _failure1;
    if (beg > dati_get_size(datin))
        goto _failure1;

    /* 分配子文件属性表 */
    list = mem_talloc(cnt, sPAK_FILE);
    if (list == NULL)
        goto _failure1;
    mem_tzero(list, cnt, sPAK_FILE);
    for (idx = 0; idx < cnt; idx++)
    {
        /* 读取文件名\0结尾 */
        for (cha = 1, tmp = 0; tmp < sizeof(str); tmp++) {
            if (!CR_VCALL(datin)->getb_no(datin, &cha))
                goto _failure2;
            str[tmp] = (ansi_t)cha;
            if (cha == 0x00)
                break;
        }
        if (cha != 0x00 || tmp == 0)
            goto _failure2;

        /* 文件偏移和大小 */
        if (!CR_VCALL(datin)->getd_be(datin, &offs))
            goto _failure2;
        if (offs > dati_get_size(datin) - beg)
            goto _failure2;
        if (!CR_VCALL(datin)->getd_be(datin, &size))
            goto _failure2;
        if (size > dati_get_size(datin) - beg - offs)
            goto _failure2;

        /* 文件名统一使用 UTF-8 编码 */
        list[idx].name = local_to_utf8(param->page, str);
        if (list[idx].name == NULL)
            goto _failure2;

        /* 设置文件属性 */
        list[idx].skip = sizeof(sPAK_FILE);
        list[idx].attr = 0;
        list[idx].offs = beg;
        list[idx].offs += offs;
        list[idx].pack = size;
        list[idx].size = size;
        list[idx].memo = "Store";
    }

    /* 生成读包接口对象 */
    port = struct_new(iPAK_SAR);
    if (port == NULL)
        goto _failure2;
    port->m_cnt = cnt;
    port->m_file = datin;
    port->pack.__filelst__ = list;
    port->pack.__vptr__ = &s_pack_vtbl;
    if (!pack_init_list((iPACKAGE*)port, TRUE)) {
        mem_free(port);
        goto _failure2;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iPAK_SAR_release((iPACKAGE*)port);
        return (NULL);
    }
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iPACKAGE";
    rett->infor = "Scripter3 ARchive (SAR)";
    return (rett);

_failure2:
    for (cnt = 0; cnt < idx; cnt++) {
        TRY_FREE(list[cnt].find);
        mem_free(list[cnt].name);
    }
    mem_free(list);
_failure1:
    CR_VCALL(datin)->release(datin);
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
