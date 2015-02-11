/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-02-28  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack ZIP 封包读取函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "enclib.h"
#include "fmtint.h"
#include "strlib.h"

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPACKAGE    pack;

        /* 个性部分 */
        leng_t  m_cnt;  /* 文件个数 */
        iDATIN* m_file; /* 文件对象 */

} iPAK_ZIP;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPAK_ZIP_release (
  __CR_IN__ iPACKAGE*   that
    )
{
    leng_t          idx;
    iPAK_ZIP*       real;
    sPAK_ZIP_FILE*  list;

    pack_free_list(that);
    real = (iPAK_ZIP*)that;
    list = (sPAK_ZIP_FILE*)real->pack.__filelst__;
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
iPAK_ZIP_getMore (
  __CR_IN__ iPACKAGE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPACKAGE::ZIP") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    读取文件个数
---------------------------------------
*/
static int64u
iPAK_ZIP_getFileNum (
  __CR_IN__ iPACKAGE*   that
    )
{
    return (((iPAK_ZIP*)that)->m_cnt);
}

/*
---------------------------------------
    读取文件数据
---------------------------------------
*/
static bool_t
iPAK_ZIP_getFileData (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sBUFFER*    buff,
  __CR_IN__ int64u      index,
  __CR_IN__ bool_t      hash
    )
{
    int64u          pack;
    int64u          size;
    void_t*         temp;
    void_t*         data;
    iDATIN*         file;
    iPAK_ZIP*       real;
    sPAK_ZIP_FILE*  item;

    /* 定位文件索引 */
    real = (iPAK_ZIP*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    item = (sPAK_ZIP_FILE*)real->pack.__filelst__;
    item += (leng_t)index;

    /* 提前过滤压缩类型 */
    if (item->ftype != 0 && item->ftype != 8 && item->ftype != 12)
        return (FALSE);

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
        pack = item->base.pack;
        temp = mem_malloc64(pack);
        if (temp == NULL)
            return (FALSE);
        file = real->m_file;

        /* 定位到文件并读起数据 */
        if (!CR_VCALL(file)->seek64(file, item->base.offs, SEEK_SET))
            goto _failure1;
        if (CR_VCALL(file)->read(file, temp, (leng_t)pack) != (leng_t)pack)
            goto _failure1;

        /* 根据压缩类型解码数据 */
        if (item->ftype == 0)
        {
            /* Store */
            size = pack;
            data = temp;
        }
        else
        {
            data = mem_malloc64(size);
            if (data == NULL)
                goto _failure1;
            if (item->ftype == 8)
            {
                /* Deflate (32K) */
                if (uncompr_flate(data, (leng_t)size, temp,
                       (leng_t)pack) != (leng_t)size)
                    goto _failure2;
            }
            else
            if (item->ftype == 12)
            {
                /* BZip2 */
                if (uncompr_bzip2(data, (leng_t)size, temp,
                       (leng_t)pack) != (leng_t)size)
                    goto _failure2;
            }
            mem_free(temp);
        }
    }

    /* 校验文件数据 */
    if (hash) {
        if (item->crc32 !=
            hash_crc32i_total(data, (leng_t)item->base.size)) {
            mem_free(data);
            return (FALSE);
        }
    }

    /* 返回文件数据 */
    return (buffer_init(buff, data, (leng_t)size, TRUE));

_failure2:
    mem_free(data);
_failure1:
    mem_free(temp);
    return (FALSE);
}

/*
---------------------------------------
    读取文件信息
---------------------------------------
*/
static bool_t
iPAK_ZIP_getFileInfo (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sPAK_FILE** info,
  __CR_IN__ int64u      index
    )
{
    leng_t          idx;
    iPAK_ZIP*       real;
    sPAK_ZIP_FILE*  list;

    /* 定位文件索引 */
    real = (iPAK_ZIP*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    idx = (leng_t)index;
    list = (sPAK_ZIP_FILE*)real->pack.__filelst__;

    /* 返回文件信息 */
    *info = (sPAK_FILE*)(&list[idx]);
    return (TRUE);
}

/* 接口虚函数表 */
static const iPACKAGE_vtbl _rom_ s_pack_vtbl =
{
    iPAK_ZIP_release, iPAK_ZIP_getMore,
    iPAK_ZIP_getFileNum, iPAK_ZIP_getFileData,
    iPAK_ZIP_getFileInfo,
};

/* ZIP 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* ZIP 文件记录结构 */
CR_TYPEDEF struct
{
        int32u  magic;      /* "PK\x03\x04" */
        int16u  version;    /* 打包版本号 */
        int16u  flags;      /* 文件的标志 */
        int16u  compr;      /* 压缩的类型 */
        int16u  ftime;      /* 文件时间 (DOS) */
        int16u  fdate;      /* 文件日期 (DOS) */
        int32u  crc32;      /* 文件的校验 */
        int32u  pksize;     /* 文件存储大小 */
        int32u  unsize;     /* 文件实际大小 */
        int16u  name_len;   /* 文件名长度 (不包括\0) */
        int16u  ext_len;    /* 文件附加数据长度 */
#if 0
        ansi_t  name[name_len];     /* 文件名 (不包括\0) */
        byte_t  exts[ext_len];      /* 文件附加数据 */
        byte_t  data[pksize];       /* 文件存储数据 */
#endif
} CR_PACKED sZIP_FILE;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
---------------------------------------
    文件单元释放回调
---------------------------------------
*/
static void_t
zip_free (
  __CR_IN__ void_t* obj
    )
{
    sPAK_ZIP_FILE*  unit;

    unit = (sPAK_ZIP_FILE*)obj;
    TRY_FREE(unit->base.find);
    mem_free(unit->base.name);
}

/*
=======================================
    ZIP 文件读取
=======================================
*/
CR_API sFMT_PRT*
load_cr_zip (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    leng_t          len;
    sARRAY          list;
    int32u          tags;
    ansi_t*         name;
    ansi_t*         utf8;
    fdist_t         offs;
    iPAK_ZIP*       port;
    sFMT_PRT*       rett;
    sZIP_FILE       unit;
    sPAK_ZIP_FILE   temp;

    /* 必须使用自己私有的读取接口 */
    datin = create_file_inX(param);
    if (datin == NULL)
        return (NULL);

    /* 开始逐个文件读取信息并定位 */
    array_initT(&list, sPAK_ZIP_FILE);
    list.free = zip_free;
    for (;;)
    {
        /* 读取一个文件记录头 */
        if (!(CR_VCALL(datin)->geType(datin, &unit, sZIP_FILE)))
            goto _failure;

        /* 文件数据区结束 */
        if (unit.magic == mk_tag4("PK\x01\x02"))
            break;

        /* 非法或损坏的 ZIP 文件 */
        if (unit.magic != mk_tag4("PK\x03\x04"))
            goto _failure;

        /* 填充文件信息结构 */
        struct_zero(&temp, sPAK_ZIP_FILE);
        len = WORD_LE(unit.name_len);
        name = str_allocA(len + 1);
        if (name == NULL)
            goto _failure;
        if (CR_VCALL(datin)->read(datin, name, len) != len) {
            mem_free(name);
            goto _failure;
        }
        name[len] = CR_AC(NIL);

        /* 跳过附加数据 */
        offs = WORD_LE(unit.ext_len);
        if (!CR_VCALL(datin)->seek(datin, offs, SEEK_CUR)) {
            mem_free(name);
            goto _failure;
        }

        /* 目录文件不加入列表 */
        if (name[len - 1] == CR_AC('/')) {
            mem_free(name);
            continue;
        }

        /* 记录当前的文件偏移位置 */
        offs = CR_VCALL(datin)->tell(datin);
        if (offs <= 0) {
            mem_free(name);
            goto _failure;
        }

        /* 文件名可能有 UTF8 编码 (7Z 生成的) */
        if (param->page == CR_UTF8) {
            utf8 = name;
        }
        else {
            utf8 = local_to_utf8(param->page, name);
            mem_free(name);
            if (utf8 == NULL)
                goto _failure;
        }

        /* 设置公用文件属性 */
        temp.base.skip = sizeof(sPAK_ZIP_FILE);
        temp.base.attr = 0;
        temp.base.offs = (int64s)offs;
        temp.base.pack = DWORD_LE(unit.pksize);
        temp.base.size = DWORD_LE(unit.unsize);
        temp.base.name = utf8;

        /* 设置私有文件属性 */
        temp.crc32 = DWORD_LE(unit.crc32);
        temp.ftime =  WORD_LE(unit.ftime);
        temp.fdate =  WORD_LE(unit.fdate);
        temp.ftype =  WORD_LE(unit.compr);
        if (temp.ftype != 0)
            temp.base.attr |= PAK_FILE_CMP;
        switch (temp.ftype)
        {
            case 0:  temp.base.memo = "Store";            break;
            case 1:  temp.base.memo = "Shrinking";        break;
            case 2:  temp.base.memo = "Reducing (2)";     break;
            case 3:  temp.base.memo = "Reducing (3)";     break;
            case 4:  temp.base.memo = "Reducing (4)";     break;
            case 5:  temp.base.memo = "Reducing (5)";     break;
            case 6:  temp.base.memo = "Imploding";        break;
            case 7:  temp.base.memo = "Tokenizing";       break;
            case 8:  temp.base.memo = "Deflate (32K)";    break;
            case 9:  temp.base.memo = "Deflate (64K)";    break;
            case 10: temp.base.memo = "PKWARE imploding"; break;
            case 11: temp.base.memo = "Reserved";         break;
            case 12: temp.base.memo = "BZip2";            break;
            case 14: temp.base.memo = "LZMA";             break;
            case 98: temp.base.memo = "PPMd";             break;
            default: temp.base.memo = "Unknown type";     break;
        }

        /* 跳过文件数据 */
        offs = DWORD_LE(unit.pksize);
        if (!CR_VCALL(datin)->seek(datin, offs, SEEK_CUR)) {
            mem_free(utf8);
            goto _failure;
        }

        /* 文件信息压入列表 */
        if (array_push_growT(&list, sPAK_ZIP_FILE, &temp) == NULL) {
            mem_free(utf8);
            goto _failure;
        }

        /* 可能会有数据描述区 */
        unit.flags = WORD_LE(unit.flags);
        if (unit.flags & 8) {
            if (!CR_VCALL(datin)->getd_no(datin, &tags))
                goto _failure;

            /* 可能会没有头部标志 */
            if (tags == mk_tag4("PK\x07\x08"))
                offs = 3 * sizeof(int32u);
            else
                offs = 2 * sizeof(int32u);
            if (!CR_VCALL(datin)->seek(datin, offs, SEEK_CUR))
                goto _failure;
        }
    }

    /* 固定一下列表大小 */
    if (!array_no_growT(&list, sPAK_ZIP_FILE))
        goto _failure;

    /* 生成读包接口对象 */
    port = struct_new(iPAK_ZIP);
    if (port == NULL)
        goto _failure;
    port->m_file = datin;
    port->m_cnt = array_get_sizeT(&list, sPAK_ZIP_FILE);
    port->pack.__filelst__ = array_get_dataT(&list, sPAK_FILE);
    port->pack.__vptr__ = &s_pack_vtbl;
    if (!pack_init_list((iPACKAGE*)port, TRUE)) {
        mem_free(port);
        goto _failure;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iPAK_ZIP_release((iPACKAGE*)port);
        return (NULL);
    }
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iPACKAGE";
    rett->infor = "Phil Katz's ZIP Archive (PKZIP)";
    return (rett);

_failure:
    array_freeT(&list, sPAK_ZIP_FILE);
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
