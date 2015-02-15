/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-08-07  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack TGL PAC 封包读取函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"
#include "strlib.h"
#include "fmtz/tgl.h"
#include "../fmtint.h"

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPACKAGE    pack;

        /* 个性部分 */
        leng_t  m_cnt;  /* 文件个数 */
        iDATIN* m_file; /* 文件对象 */

} iPAK_PACF;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPAK_PACF_release (
  __CR_IN__ iPACKAGE*   that
    )
{
    leng_t      idx;
    iPAK_PACF*  real;
    sPAK_FILE*  list;

    pack_free_list(that);
    real = (iPAK_PACF*)that;
    list = real->pack.__filelst__;
    if (list != NULL) {
        for (idx = 0; idx < real->m_cnt; idx++) {
            mem_free(list[idx].find);
            mem_free(list[idx].name);
        }
        mem_free(list);
    }
    CR_VCALL(real->m_file)->release(real->m_file);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iPAK_PACF_getMore (
  __CR_IN__ iPACKAGE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPACKAGE::TGL_PAC") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    读取文件个数
---------------------------------------
*/
static int64u
iPAK_PACF_getFileNum (
  __CR_IN__ iPACKAGE*   that
    )
{
    return (((iPAK_PACF*)that)->m_cnt);
}

/*
---------------------------------------
    读取文件数据
---------------------------------------
*/
static bool_t
iPAK_PACF_getFileData (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sBUFFER*    buff,
  __CR_IN__ int64u      index,
  __CR_IN__ bool_t      hash
    )
{
    int64u      size;
    void_t*     data;
    iDATIN*     file;
    iPAK_PACF*  real;
    sPAK_FILE*  item;

    /* 定位文件索引 */
    CR_NOUSE(hash);
    real = (iPAK_PACF*)that;
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
iPAK_PACF_getFileInfo (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sPAK_FILE** info,
  __CR_IN__ int64u      index
    )
{
    leng_t      idx;
    iPAK_PACF*  real;

    /* 定位文件索引 */
    real = (iPAK_PACF*)that;
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
    iPAK_PACF_release, iPAK_PACF_getMore,
    iPAK_PACF_getFileNum, iPAK_PACF_getFileData,
    iPAK_PACF_getFileInfo,
};

/*
---------------------------------------
    根据文件偏移排序
---------------------------------------
*/
static sint_t
offset_comp (
  __CR_IN__ const void_t*   elem1,
  __CR_IN__ const void_t*   elem2
    )
{
    int32u  off1, off2;

    off1 = (int32u)(((sPAK_FILE*)elem1)->offs);
    off2 = (int32u)(((sPAK_FILE*)elem2)->offs);
    if (off1 < off2)
        return (-1);
    if (off1 > off2)
        return (1);
    return (0);
}

/*
=======================================
    TGL PAC_FILE 文件读取
=======================================
*/
CR_API sFMT_PRT*
load_tgl_pac (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    int32u      idx;
    int32u      cnt;
    int32u      offs;
    sFMT_PRT*   rett;
    iPAK_PACF*  port;
    sPAK_FILE*  list;
    ansi_t      str[17];

    /* 必须使用自己私有的读取接口 */
    datin = create_file_inX(param);
    if (datin == NULL)
        return (NULL);

    /* 读取文件头信息 */
    if (CR_VCALL(datin)->read(datin, str, 9) != 9)
        goto _failure1;
    if (mem_cmp(str, "PAC_FILE", 9) != 0)
        goto _failure1;

    /* 子文件的个数 */
    if (!CR_VCALL(datin)->getd_le(datin, &cnt))
        goto _failure1;

    /* 分配子文件属性表 */
    if (cnt != 0) {
        list = mem_talloc32(cnt, sPAK_FILE);
        if (list == NULL)
            goto _failure1;
        str[16] = CR_AC(NIL);
        mem_tzero(list, cnt, sPAK_FILE);
    }
    else {
        list = NULL;    /* 支持空的包文件 */
    }

    /* 加载文件信息表 */
    for (idx = 0; idx < cnt; idx++)
    {
        /* 读取文件名不保证\0结尾 */
        if (CR_VCALL(datin)->read(datin, str, 16) != 16)
            goto _failure2;

        /* 文件的包内偏移 */
        if (!CR_VCALL(datin)->getd_le(datin, &offs))
            goto _failure2;
        if (offs > dati_get_size(datin))
            goto _failure2;

        /* 文件名统一使用 UTF-8 编码 */
        list[idx].name = local_to_utf8(param->page, str);
        if (list[idx].name == NULL)
            goto _failure2;

        /* 设置公用文件属性 */
        list[idx].skip = sizeof(sPAK_FILE);
        list[idx].attr = 0;
        list[idx].offs = offs;
        list[idx].offs += 4;
        list[idx].memo = "Store";
    }

    /* 计算文件的包内大小 */
    if (cnt != 0)
    {
        /* 先根据偏移值排序一下以免出意外状况 */
        quick_sort(list, (leng_t)cnt, sizeof(sPAK_FILE), offset_comp);
        for (idx = 0; idx < cnt - 1; idx++) {
            list[idx].pack = list[idx + 1].offs - list[idx].offs;
            list[idx].size = list[idx].pack;
        }
        list[idx].pack = dati_get_size(datin) - list[idx].offs;
        list[idx].size = list[idx].pack;
        idx = cnt;
    }

    /* 生成读包接口对象 */
    port = struct_new(iPAK_PACF);
    if (port == NULL)
        goto _failure2;
    port->m_file = datin;
    port->m_cnt = (leng_t)cnt;
    port->pack.__filelst__ = list;
    port->pack.__vptr__ = &s_pack_vtbl;
    if (!pack_init_list((iPACKAGE*)port, TRUE)) {
        mem_free(port);
        goto _failure2;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iPAK_PACF_release((iPACKAGE*)port);
        return (NULL);
    }
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iPACKAGE";
    rett->infor = "TGL PAC_FILE Archive (PACF)";
    return (rett);

_failure2:
    if (list != NULL) {
        for (cnt = 0; cnt < idx; cnt++) {
            TRY_FREE(list[cnt].find);
            mem_free(list[cnt].name);
        }
        mem_free(list);
    }
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
