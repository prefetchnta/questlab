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
/*  >>>>>>>>>>>>>>>>>>> CrHack FALCOM NNK 封包读取函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "strlib.h"
#include "../fmtint.h"
#include "fmtz/falcom.h"

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPACKAGE    pack;

        /* 个性部分 */
        leng_t  m_cnt;  /* 文件个数 */
        iDATIN* m_file; /* 文件对象 */

} iPAK_NNK;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPAK_NNK_release (
  __CR_IN__ iPACKAGE*   that
    )
{
    leng_t          idx;
    iPAK_NNK*       real;
    sPAK_NNK_FILE*  list;

    pack_free_list(that);
    real = (iPAK_NNK*)that;
    list = (sPAK_NNK_FILE*)real->pack.__filelst__;
    if (list != NULL) {
        for (idx = 0; idx < real->m_cnt; idx++) {
            mem_free(list[idx].base.find);
            mem_free(list[idx].base.name);
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
iPAK_NNK_getMore (
  __CR_IN__ iPACKAGE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPACKAGE::YS_NNK") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    读取文件个数
---------------------------------------
*/
static int64u
iPAK_NNK_getFileNum (
  __CR_IN__ iPACKAGE*   that
    )
{
    return (((iPAK_NNK*)that)->m_cnt);
}

/*
---------------------------------------
    读取文件数据
---------------------------------------
*/
static bool_t
iPAK_NNK_getFileData (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sBUFFER*    buff,
  __CR_IN__ int64u      index,
  __CR_IN__ bool_t      hash
    )
{
    int64u          size;
    void_t*         data;
    iDATIN*         file;
    iPAK_NNK*       real;
    sPAK_NNK_FILE*  item;

    /* 定位文件索引 */
    CR_NOUSE(hash);
    real = (iPAK_NNK*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    item = (sPAK_NNK_FILE*)real->pack.__filelst__;
    item += (leng_t)index;

    /* 获取文件数据 (0大小文件分配1个字节) */
    size = item->base.size;
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
        if (!CR_VCALL(file)->seek64(file, item->base.offs, SEEK_SET))
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
iPAK_NNK_getFileInfo (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sPAK_FILE** info,
  __CR_IN__ int64u      index
    )
{
    leng_t          idx;
    iPAK_NNK*       real;
    sPAK_NNK_FILE*  list;

    /* 定位文件索引 */
    real = (iPAK_NNK*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    idx = (leng_t)index;
    list = (sPAK_NNK_FILE*)real->pack.__filelst__;

    /* 返回文件信息 */
    *info = (sPAK_FILE*)(&list[idx]);
    return (TRUE);
}

/* 接口虚函数表 */
static const iPACKAGE_vtbl _rom_ s_pack_vtbl =
{
    iPAK_NNK_release, iPAK_NNK_getMore,
    iPAK_NNK_getFileNum, iPAK_NNK_getFileData,
    iPAK_NNK_getFileInfo,
};

/*
---------------------------------------
    NNK 字节流解密
---------------------------------------
*/
static void_t
nnk_decode (
  __CR_IN__ int32u  key,
  __CR_IO__ byte_t* data,
  __CR_IN__ leng_t  size
    )
{
    for (; size != 0; size--, data++) {
        key *= 0x3D09UL;
        *data = *data - (byte_t)(key >> 16);
    }
}

/* NNK 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* NNK 文件记录结构 */
CR_TYPEDEF struct
{
        int32u  hash;       /* 文件名哈希 */
        int32u  size;       /* 文件的大小 */
        int32u  offset;     /* 文件的偏移 */
        int32u  name;       /* 文件名偏移 */

} CR_PACKED sNNK_FILE;

/* NNK 文件头结构 */
CR_TYPEDEF struct
{
        int32u  magic;      /* 文件标志 "NNI" */
        int32u  count;      /* 包内文件数量 */
        int32u  name_size;  /* 文件名表大小 */
        int32u  flags;      /* 文件标志？ */

} CR_PACKED sNNK_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    FALCOM NNK 文件读取
=======================================
*/
CR_API sFMT_PRT*
load_flc_nnk (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    int32u          idx;
    int32u          cnt;
    leng_t          back;
    leng_t          size;
    ansi_t*         name;
    iDATIN*         flna;
    sLOADER         ldrs;
    sNNK_HDR        head;
    sFMT_PRT*       rett;
    iPAK_NNK*       port;
    sNNK_FILE*      info;
    sPAK_NNK_FILE*  list;

    /* 指定的文件只是数据文件
       索引位于另外一个文件, 需要替换扩展名 */
    struct_cpy(&ldrs, param, sLOADER);
    if (param->type == CR_LDR_ANSI) {
        size = str_lenA(param->name.ansi) + 4;
        ldrs.name.ansi = str_allocA(size);
        if (ldrs.name.ansi == NULL)
            return (NULL);
        filext_changeA((ansi_t*)ldrs.name.ansi,
            param->name.ansi, CR_AS(".ni"));
    }
    else
    if (param->type == CR_LDR_WIDE) {
        size = str_lenW(param->name.wide) + 4;
        ldrs.name.wide = str_allocW(size);
        if (ldrs.name.wide == NULL)
            return (NULL);
        filext_changeW((wide_t*)ldrs.name.wide,
            param->name.wide, CR_WS(".ni"));
    }
    else {
        return (NULL);
    }

    /* 打开索引文件 */
    datin = create_file_inX(&ldrs);
    if (param->type == CR_LDR_ANSI)
        mem_free(ldrs.name.ansi);
    else
        mem_free(ldrs.name.wide);
    if (datin == NULL)
        return (NULL);

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->geType(datin, &head, sNNK_HDR)))
        goto _failure1;
    if (head.magic != mk_tag4("NNI"))
        goto _failure1;

    /* 读取文件索引表 */
    head.count = DWORD_LE(head.count);
    info = mem_talloc32(head.count, sNNK_FILE);
    if (info == NULL)
        goto _failure1;
    size = (leng_t)head.count;
    size *= sizeof(sNNK_FILE);
    back = CR_VCALL(datin)->read(datin, info, size);
    if (back != size)
        goto _failure2;
    cnt = head.count;
    nnk_decode(0x7C53F961UL, (byte_t*)info, back);

    /* 读取文件名表 */
    head.name_size = DWORD_LE(head.name_size);
    name = mem_malloc32(head.name_size);
    if (name == NULL)
        goto _failure2;
    back = CR_VCALL(datin)->read(datin, name, (leng_t)head.name_size);
    if (back != (leng_t)head.name_size)
        goto _failure3;
    nnk_decode(0x7C53F961UL, (byte_t*)name, back);

    /* 必须使用自己私有的读取接口 */
    flna = create_file_inX(param);
    if (flna == NULL)
        goto _failure3;
    CR_VCALL(datin)->release(datin);
    datin = flna;

    /* 分配子文件属性表 */
    if (cnt != 0) {
        list = mem_talloc32(cnt, sPAK_NNK_FILE);
        if (list == NULL)
            goto _failure3;
        mem_tzero(list, cnt, sPAK_NNK_FILE);
    }
    else {
        list = NULL;    /* 支持空的包文件 */
    }

    /* 加载文件信息表 */
    for (idx = 0; idx < cnt; idx++)
    {
        /* 文件名偏移过滤 */
        info[idx].name = DWORD_LE(info[idx].name);
        if (info[idx].name >= head.name_size)
            goto _failure4;

        /* 文件名统一使用 UTF-8 编码 */
        list[idx].base.name = local_to_utf8(param->page,
                                &name[info[idx].name]);
        if (list[idx].base.name == NULL)
            goto _failure4;

        /* 设置公用文件属性 */
        list[idx].base.skip = sizeof(sPAK_NNK_FILE);
        list[idx].base.attr = 0;
        list[idx].base.offs = DWORD_LE(info[idx].offset);
        list[idx].base.pack = DWORD_LE(info[idx].size);
        list[idx].base.size = list[idx].base.pack;
        list[idx].base.memo = "Store";

        /* 设置私有文件属性 */
        list[idx].hash = DWORD_LE(info[idx].hash);
    }

    /* 生成读包接口对象 */
    port = struct_new(iPAK_NNK);
    if (port == NULL)
        goto _failure4;
    port->m_file = datin;
    port->m_cnt = (leng_t)cnt;
    port->pack.__filelst__ = (sPAK_FILE*)list;
    port->pack.__vptr__ = &s_pack_vtbl;
    if (!pack_init_list((iPACKAGE*)port, TRUE)) {
        mem_free(port);
        goto _failure4;
    }
    mem_free(name);
    mem_free(info);

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iPAK_NNK_release((iPACKAGE*)port);
        return (NULL);
    }
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iPACKAGE";
    rett->infor = "FALCOM YS NNK Archive (*.NI + *.NA)";
    return (rett);

_failure4:
    if (list != NULL) {
        for (cnt = 0; cnt < idx; cnt++) {
            TRY_FREE(list[cnt].base.find);
            mem_free(list[cnt].base.name);
        }
        mem_free(list);
    }
_failure3:
    mem_free(name);
_failure2:
    mem_free(info);
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
