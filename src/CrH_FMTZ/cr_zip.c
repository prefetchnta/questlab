/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-02-28  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack ZIP 封包读取函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_ZIP_C__
#define __CR_ZIP_C__ 0xCBF71DCCUL

#include "hash.h"
#include "safe.h"
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
    leng_t          idx;
    leng_t          read;
    int64u          pack;
    int64u          size;
    int32u          cksm;
    uint_t          type;
    uint_t          unsz;
    uint_t          pksz;
    void_t*         temp;
    void_t*         data;
    iDATIN*         file;
    iPAK_ZIP*       real;
    sPAK_ZIP_FILE*  list;

    /* 定位文件索引 */
    real = (iPAK_ZIP*)that;
    if (index >= real->m_cnt) {
        err_set(__CR_ZIP_C__, index,
                "iPACKAGE::getFileData()", "index: out of bounds");
        return (FALSE);
    }
    idx = (leng_t)index;
    list = (sPAK_ZIP_FILE*)real->pack.__filelst__;

    /* 提前过滤压缩类型 */
    type = list[idx].ftype;
    if (type != 0 && type != 8 && type != 12) {
        err_set(__CR_ZIP_C__, type,
                "iPACKAGE::getFileData()", "invalid compression type");
        return (FALSE);
    }

    /* 获取文件数据 (0大小文件分配1个字节) */
    size = list[idx].base.size;
    if (size == 0) {
        data = mem_malloc(1);
        if (data == NULL) {
            err_set(__CR_ZIP_C__, CR_NULL,
                    "iPACKAGE::getFileData()", "mem_malloc() failure");
            return (FALSE);
        }
        size = 1;
        *(byte_t*)data = 0x00;
    }
    else {
        pack = list[idx].base.pack;
        temp = mem_malloc64(pack);
        if (temp == NULL) {
            err_set(__CR_ZIP_C__, CR_NULL,
                    "iPACKAGE::getFileData()", "mem_malloc64() failure");
            return (FALSE);
        }

        /* 定位到文件并读起数据 */
        file = real->m_file;
        if (!CR_VCALL(file)->seek64(file, list[idx].base.offs, SEEK_SET)) {
            err_set(__CR_ZIP_C__, FALSE,
                    "iPACKAGE::getFileData()", "iDATIN::seek64() failure");
            goto _failure1;
        }
        read = CR_VCALL(file)->read(file, temp, (leng_t)pack);
        if (read != (leng_t)pack) {
            err_set(__CR_ZIP_C__, read,
                    "iPACKAGE::getFileData()", "iDATIN::read() failure");
            goto _failure1;
        }

        /* 根据压缩类型解码数据 */
        if (type == 0)
        {
            /* Store */
            size = pack;
            data = temp;
        }
        else
        {
            data = mem_malloc64(size);
            if (data == NULL) {
                err_set(__CR_ZIP_C__, CR_NULL,
                        "iPACKAGE::getFileData()", "mem_malloc64() failure");
                goto _failure1;
            }

            if (type == 8)
            {
                /* Deflate (32K) */
                if (cut_size(&pksz, (leng_t)pack)) {
                    err_set(__CR_ZIP_C__, pack,
                            "iPACKAGE::getFileData()", "<pack> truncated");
                    goto _failure2;
                }
                if (cut_size(&unsz, (leng_t)size)) {
                    err_set(__CR_ZIP_C__, size,
                            "iPACKAGE::getFileData()", "<size> truncated");
                    goto _failure2;
                }
                pksz = uncompr_pkzip(data, unsz, temp, pksz);
                if (pksz != unsz) {
                    err_set(__CR_ZIP_C__, pksz,
                        "iPACKAGE::getFileData()", "uncompr_pkzip() failure");
                    goto _failure2;
                }
            }
            else
            if (type == 12)
            {
                /* BZip2 */
                if (cut_size(&pksz, (leng_t)pack)) {
                    err_set(__CR_ZIP_C__, pack,
                            "iPACKAGE::getFileData()", "<pack> truncated");
                    goto _failure2;
                }
                if (cut_size(&unsz, (leng_t)size)) {
                    err_set(__CR_ZIP_C__, size,
                            "iPACKAGE::getFileData()", "<size> truncated");
                    goto _failure2;
                }
                pksz = uncompr_bzip2(data, unsz, temp, pksz);
                if (pksz != unsz) {
                    err_set(__CR_ZIP_C__, pksz,
                        "iPACKAGE::getFileData()", "uncompr_bzip2() failure");
                    goto _failure2;
                }
            }
            mem_free(temp);
        }
    }

    /* 校验文件数据 */
    if (hash) {
        cksm = hash_crc32i_total(data, (leng_t)list[idx].base.size);
        if (cksm != list[idx].crc32) {
            err_set(__CR_ZIP_C__, cksm,
                    "iPACKAGE::getFileData()", "file checksum error");
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
    if (index >= real->m_cnt) {
        err_set(__CR_ZIP_C__, index,
                "iPACKAGE::getFileInfo()", "index: out of bounds");
        return (FALSE);
    }
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
    TRY_FREE(unit->base.find)
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
    leng_t          read;
    sARRAY          list;
    ansi_t*         name;
    ansi_t*         utf8;
    fdist_t         offs;
    iPAK_ZIP*       port;
    sFMT_PRT*       rett;
    sZIP_FILE       unit;
    sPAK_ZIP_FILE   temp;

    /* 必须使用自己私有的读取接口 */
    datin = create_file_inX(param);
    if (datin == NULL) {
        err_set(__CR_ZIP_C__, CR_NULL,
                "load_cr_zip()", "create_file_inX() failure");
        return (NULL);
    }

    /* 开始逐个文件读取信息并定位 */
    array_initT(&list, sPAK_ZIP_FILE);
    list.free = zip_free;
    for (;;)
    {
        /* 读取一个文件记录头 */
        if (!(CR_VCALL(datin)->getT(datin, &unit, sZIP_FILE))) {
            err_set(__CR_ZIP_C__, FALSE,
                    "load_cr_zip()", "iDATIN::getT() failure");
            goto _failure;
        }

        /* 文件数据区结束 */
        if (unit.magic == mk_tag4("PK\x01\x02"))
            break;

        /* 非法或损坏的 ZIP 文件 */
        if (unit.magic != mk_tag4("PK\x03\x04")) {
            err_set(__CR_ZIP_C__, unit.magic,
                    "load_cr_zip()", "invalid ZIP format");
            goto _failure;
        }

        /* 填充文件信息结构 */
        struct_zero(&temp, sPAK_ZIP_FILE);
        len = WORD_LE(unit.name_len);
        name = str_allocA(len + 1);
        if (name == NULL) {
            err_set(__CR_ZIP_C__, CR_NULL,
                    "load_cr_zip()", "str_allocA() failure");
            goto _failure;
        }
        read = CR_VCALL(datin)->read(datin, name, len);
        if (read != len) {
            err_set(__CR_ZIP_C__, read,
                    "load_cr_zip()", "iDATIN::read() failure");
            mem_free(name);
            goto _failure;
        }
        name[len] = NIL;

        /* 跳过附加数据 */
        offs = WORD_LE(unit.ext_len);
        if (!CR_VCALL(datin)->seek(datin, offs, SEEK_CUR)) {
            err_set(__CR_ZIP_C__, FALSE,
                    "load_cr_zip()", "iDATIN::seek() failure");
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
        if (offs == (fdist_t)-1) {
            err_set(__CR_ZIP_C__, -1L,
                    "load_cr_zip()", "iDATIN::tell() failure");
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
            if (utf8 == NULL) {
                err_set(__CR_ZIP_C__, CR_NULL,
                        "load_cr_zip()", "local_to_utf8() failure");
                goto _failure;
            }
        }

        /* 设置公用文件属性 */
        temp.base.skip = sizeof(sPAK_ZIP_FILE);
        temp.base.attr = 0;
        temp.base.offs = (int64s)offs;
        temp.base.pack = DWORD_LE(unit.pksize);
        temp.base.size = DWORD_LE(unit.unsize);
        temp.base.name = utf8;
        if (temp.base.pack != temp.base.size)
            temp.base.attr |= PAK_FILE_CMP;

        /* 设置私有文件属性 */
        temp.crc32 = DWORD_LE(unit.crc32);
        temp.ftime =  WORD_LE(unit.ftime);
        temp.fdate =  WORD_LE(unit.fdate);
        temp.ftype =  WORD_LE(unit.compr);
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
            err_set(__CR_ZIP_C__, FALSE,
                    "load_cr_zip()", "iDATIN::seek() failure");
            mem_free(utf8);
            goto _failure;
        }

        /* 文件信息压入列表 */
        if (array_push_growT(&list, sPAK_ZIP_FILE, &temp) == NULL) {
            err_set(__CR_ZIP_C__, CR_NULL,
                    "load_cr_zip()", "array_push_growT() failure");
            mem_free(utf8);
            goto _failure;
        }
    }

    /* 固定一下列表大小 */
    if (!array_no_growT(&list, sPAK_ZIP_FILE)) {
        err_set(__CR_ZIP_C__, FALSE,
                "load_cr_zip()", "array_no_growT() failure");
        goto _failure;
    }

    /* 生成读包接口对象 */
    port = struct_new(iPAK_ZIP);
    if (port == NULL) {
        err_set(__CR_ZIP_C__, CR_NULL,
                "load_cr_zip()", "struct_new() failure");
        goto _failure;
    }
    port->m_file = datin;
    port->m_cnt = array_get_sizeT(&list, sPAK_ZIP_FILE);
    port->pack.__filelst__ = array_get_dataT(&list, sPAK_FILE);
    port->pack.__vptr__ = &s_pack_vtbl;
    if (!pack_init_list((iPACKAGE*)port, TRUE)) {
        err_set(__CR_ZIP_C__, FALSE,
                "load_cr_zip()", "pack_init_list() failure");
        mem_free(port);
        goto _failure;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        err_set(__CR_ZIP_C__, CR_NULL,
                "load_cr_zip()", "struct_new() failure");
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

#endif  /* !__CR_ZIP_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
