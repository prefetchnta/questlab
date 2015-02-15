/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-05-08  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack EGO DAT 封包读取函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "strlib.h"
#include "fmtz/ego.h"
#include "../fmtint.h"

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPACKAGE    pack;

        /* 个性部分 */
        leng_t  m_cnt;  /* 文件个数 */
        iDATIN* m_file; /* 文件对象 */

} iPAK_GDAT;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPAK_GDAT_release (
  __CR_IN__ iPACKAGE*   that
    )
{
    leng_t      idx;
    iPAK_GDAT*  real;
    sPAK_FILE*  list;

    pack_free_list(that);
    real = (iPAK_GDAT*)that;
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
iPAK_GDAT_getMore (
  __CR_IN__ iPACKAGE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPACKAGE::EGO_GDAT") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    读取文件个数
---------------------------------------
*/
static int64u
iPAK_GDAT_getFileNum (
  __CR_IN__ iPACKAGE*   that
    )
{
    return (((iPAK_GDAT*)that)->m_cnt);
}

/*
---------------------------------------
    读取文件数据
---------------------------------------
*/
static bool_t
iPAK_GDAT_getFileData (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sBUFFER*    buff,
  __CR_IN__ int64u      index,
  __CR_IN__ bool_t      hash
    )
{
    int64u      size;
    void_t*     data;
    iDATIN*     file;
    iPAK_GDAT*  real;
    sPAK_FILE*  item;

    /* 定位文件索引 */
    CR_NOUSE(hash);
    real = (iPAK_GDAT*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    item = real->pack.__filelst__;
    item += (leng_t)index;

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
iPAK_GDAT_getFileInfo (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sPAK_FILE** info,
  __CR_IN__ int64u      index
    )
{
    leng_t      idx;
    iPAK_GDAT*  real;

    /* 定位文件索引 */
    real = (iPAK_GDAT*)that;
    if (index >= real->m_cnt)
        return (FALSE);

    /* 返回文件信息 */
    idx = (leng_t)index;
    *info = &real->pack.__filelst__[idx];
    return (TRUE);
}

/* 接口虚函数表 */
static const iPACKAGE_vtbl _rom_ s_pack_vtbl =
{
    iPAK_GDAT_release, iPAK_GDAT_getMore,
    iPAK_GDAT_getFileNum, iPAK_GDAT_getFileData,
    iPAK_GDAT_getFileInfo,
};

/*
---------------------------------------
    文件单元释放回调
---------------------------------------
*/
static void_t
gdat_free (
  __CR_IN__ void_t* obj
    )
{
    sPAK_FILE*  unit;

    unit = (sPAK_FILE*)obj;
    TRY_FREE(unit->find);
    mem_free(unit->name);
}

/*
=======================================
    EGO DAT 文件读取
=======================================
*/
CR_API sFMT_PRT*
load_ego_gdat (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    leng_t      back;
    int32u      unit;
    int32u      high;
    int32u      offs;
    int32u      size;
    int32u      sdat;
    int32u      fbeg;
    sARRAY      list;
    sFMT_PRT*   rett;
    sPAK_FILE   temp;
    iPAK_GDAT*  port;
    /* ----------- */
    ansi_t  str[MAX_PATHA];

    /* 必须使用自己私有的读取接口 */
    datin = create_file_inX(param);
    if (datin == NULL)
        return (NULL);
    high = 0;
    array_initT(&list, sPAK_FILE);
    list.free = gdat_free;

    /* 文件索引表大小 */
    if (!CR_VCALL(datin)->getd_le(datin, &sdat))
        goto _failure;
    fbeg = sdat + 4;
    if (sdat <= 12 + 1 ||
        sdat > dati_get_size(datin) ||
        fbeg > dati_get_size(datin))
        goto _failure;

    /* 开始读取文件索引 */
    while (sdat != 0)
    {
        /* 读取一个文件的索引大小 (这个值可能是64位的) */
        if (!CR_VCALL(datin)->getd_le(datin, &unit))
            goto _failure;
        if (!CR_VCALL(datin)->getd_le(datin, &high))
            goto _failure;
        if (unit > sdat)
            goto _failure;
        sdat -= unit;

        /* 如果不是64位的这个值就是文件偏移 */
        if (high != 0) {
            if (unit <= 12 + 1)
                goto _failure;
            unit -= 12;
            offs = high;
        }
        else {
            if (unit <= 16 + 1)
                goto _failure;
            if (!CR_VCALL(datin)->getd_le(datin, &offs))
                goto _failure;
            unit -= 16;
        }
        if (offs < fbeg || offs > dati_get_size(datin))
            goto _failure;

        /* 接下来是文件的大小 */
        if (!CR_VCALL(datin)->getd_le(datin, &size))
            goto _failure;
        if (size > dati_get_size(datin) - offs)
            goto _failure;

        /* 剩下的全是文件名 (有\0结尾) */
        if (unit > sizeof(str))
            goto _failure;
        back = CR_VCALL(datin)->read(datin, str, (leng_t)unit);
        if (back != (leng_t)unit)
            goto _failure;
        back -= 1;
        if (str[back] != 0x00 || str_lenA(str) != back)
            goto _failure;

        /* 填充文件信息结构 */
        struct_zero(&temp, sPAK_FILE);

        /* 文件名统一使用 UTF-8 编码 */
        temp.name = local_to_utf8(param->page, str);
        if (temp.name == NULL)
            goto _failure;

        /* 设置公用文件属性 */
        temp.skip = sizeof(sPAK_FILE);
        temp.attr = 0;
        temp.offs = offs;
        temp.pack = size;
        temp.size = size;
        temp.memo = "Store";

        /* 文件信息压入列表 */
        if (array_push_growT(&list, sPAK_FILE, &temp) == NULL) {
            mem_free(temp.name);
            goto _failure;
        }
    }

    /* 不包含文件视为非法 */
    if (array_get_dataT(&list, sPAK_FILE) == NULL)
        goto _failure;

    /* 固定一下列表大小 */
    if (!array_no_growT(&list, sPAK_FILE))
        goto _failure;

    /* 生成读包接口对象 */
    port = struct_new(iPAK_GDAT);
    if (port == NULL)
        goto _failure;
    port->m_file = datin;
    port->m_cnt = array_get_sizeT(&list, sPAK_FILE);
    port->pack.__filelst__ = array_get_dataT(&list, sPAK_FILE);
    port->pack.__vptr__ = &s_pack_vtbl;
    if (!pack_init_list((iPACKAGE*)port, TRUE)) {
        mem_free(port);
        goto _failure;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iPAK_GDAT_release((iPACKAGE*)port);
        return (NULL);
    }
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iPACKAGE";
    if (high == 0)
        rett->infor = "Studio e.go! Game Data Archive (64)";
    else
        rett->infor = "Studio e.go! Game Data Archive (32)";
    return (rett);

_failure:
    array_freeT(&list, sPAK_FILE);
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
