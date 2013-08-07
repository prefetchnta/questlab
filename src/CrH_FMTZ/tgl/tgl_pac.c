/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-08-07  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-08-07  */
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

#ifndef __CR_TGL_PAC_C__
#define __CR_TGL_PAC_C__ 0xB59EC6BBUL

#include "enclib.h"
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
    leng_t          idx;
    iPAK_PACF*      real;
    sPAK_TGL_FILE*  list;

    pack_free_list(that);
    real = (iPAK_PACF*)that;
    list = (sPAK_TGL_FILE*)real->pack.__filelst__;
    for (idx = 0; idx < real->m_cnt; idx++) {
        mem_free(list[idx].base.find);
        mem_free(list[idx].base.name);
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
    leng_t          idx;
    leng_t          read;
    int64u          pack;
    int64u          size;
    int32u          type;
    void_t*         temp;
    void_t*         data;
    byte_t*         pntr;
    iDATIN*         file;
    iPAK_PACF*      real;
    sPAK_TGL_FILE*  list;

    /* 定位文件索引 */
    CR_NOUSE(hash);
    real = (iPAK_PACF*)that;
    if (index >= real->m_cnt) {
        err_set(__CR_TGL_PAC_C__, index,
                "iPACKAGE::getFileData()", "index: out of bounds");
        return (FALSE);
    }
    idx = (leng_t)index;
    list = (sPAK_TGL_FILE*)real->pack.__filelst__;

    /* 获取文件数据 (0大小文件分配1个字节) */
    size = list[idx].base.size;
    if (size == 0) {
        data = mem_malloc(1);
        if (data == NULL) {
            err_set(__CR_TGL_PAC_C__, CR_NULL,
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
            err_set(__CR_TGL_PAC_C__, CR_NULL,
                    "iPACKAGE::getFileData()", "mem_malloc64() failure");
            return (FALSE);
        }

        /* 定位到文件并读起数据 */
        file = real->m_file;
        if (!CR_VCALL(file)->seek64(file, list[idx].base.offs, SEEK_SET)) {
            err_set(__CR_TGL_PAC_C__, FALSE,
                    "iPACKAGE::getFileData()", "iDATIN::seek64() failure");
            goto _failure1;
        }
        read = CR_VCALL(file)->read(file, temp, (leng_t)pack);
        if (read != (leng_t)pack) {
            err_set(__CR_TGL_PAC_C__, read,
                    "iPACKAGE::getFileData()", "iDATIN::read() failure");
            goto _failure1;
        }

        /* 根据压缩类型解码数据 */
        type = list[idx].ftype;
        if (type != mk_tag4("IEL1"))
        {
            /* Store */
            size = pack;
            data = temp;
        }
        else
        {
            data = mem_malloc64(size);
            if (data == NULL) {
                err_set(__CR_TGL_PAC_C__, CR_NULL,
                        "iPACKAGE::getFileData()", "mem_malloc64() failure");
                goto _failure1;
            }

            /* LZSS (IEL1) */
            pack -= sizeof(int64u);
            pntr = (byte_t*)temp + sizeof(int64u);
            read = uncompr_lzss(data, (leng_t)size, pntr, (leng_t)pack, 0);
            if (read != (leng_t)size) {
                err_set(__CR_TGL_PAC_C__, read,
                        "iPACKAGE::getFileData()", "uncompr_lzss8() failure");
                goto _failure2;
            }
            mem_free(temp);
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
iPAK_PACF_getFileInfo (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sPAK_FILE** info,
  __CR_IN__ int64u      index
    )
{
    leng_t          idx;
    iPAK_PACF*      real;
    sPAK_TGL_FILE*  list;

    /* 定位文件索引 */
    real = (iPAK_PACF*)that;
    if (index >= real->m_cnt) {
        err_set(__CR_TGL_PAC_C__, index,
                "iPACKAGE::getFileInfo()", "index: out of bounds");
        return (FALSE);
    }
    idx = (uint_t)index;
    list = (sPAK_TGL_FILE*)real->pack.__filelst__;

    /* 返回文件信息 */
    *info = (sPAK_FILE*)(&list[idx]);
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
    leng_t      read;
    sFMT_PRT*   rett;
    iPAK_PACF*  port;
    ansi_t      str[17];
    /* -------------- */
    sPAK_TGL_FILE*  list;

    /* 必须使用自己私有的读取接口 */
    datin = create_file_inX(param);
    if (datin == NULL) {
        err_set(__CR_TGL_PAC_C__, CR_NULL,
                "load_tgl_pac()", "create_file_inX() failure");
        return (NULL);
    }

    /* 读取文件头信息 */
    read = CR_VCALL(datin)->read(datin, str, 9);
    if (read != 9) {
        err_set(__CR_TGL_PAC_C__, read,
                "load_tgl_pac()", "iDATIN::read() failure");
        goto _failure1;
    }
    if (mem_cmp(str, "PAC_FILE", 9) != 0) {
        err_set(__CR_TGL_PAC_C__, CR_ERROR,
                "load_tgl_pac()", "invalid TGL PAC format");
        goto _failure1;
    }
    /* 子文件的个数 */
    if (!CR_VCALL(datin)->getd_le(datin, &cnt)) {
        err_set(__CR_TGL_PAC_C__, FALSE,
                "load_tgl_pac()", "iDATIN::getd_le() failure");
        goto _failure1;
    }
    if (cnt == 0) {
        err_set(__CR_TGL_PAC_C__, cnt,
                "load_tgl_pac()", "invalid TGL PAC format");
        goto _failure1;
    }

    /* 分配子文件属性表 */
    list = mem_talloc32(cnt, sPAK_TGL_FILE);
    if (list == NULL) {
        err_set(__CR_TGL_PAC_C__, CR_NULL,
                "load_tgl_pac()", "mem_talloc32() failure");
        goto _failure1;
    }
    mem_tzero(list, cnt, sPAK_TGL_FILE);
    for (str[16] = 0, idx = 0; idx < cnt; idx++)
    {
        /* 读取文件名不保证\0结尾 */
        read = CR_VCALL(datin)->read(datin, str, 16);
        if (read != 16) {
            err_set(__CR_TGL_PAC_C__, read,
                    "load_tgl_pac()", "iDATIN::read() failure");
            goto _failure2;
        }

        /* 文件的包内偏移 */
        if (!CR_VCALL(datin)->getd_le(datin, &offs)) {
            err_set(__CR_TGL_PAC_C__, FALSE,
                    "load_tgl_pac()", "iDATIN::getd_le() failure");
            goto _failure2;
        }

        /* 文件名统一使用 UTF-8 编码 */
        list[idx].base.name = local_to_utf8(param->page, str);
        if (list[idx].base.name == NULL) {
            err_set(__CR_TGL_PAC_C__, CR_NULL,
                    "load_tgl_pac()", "local_to_utf8() failure");
            goto _failure2;
        }

        /* 设置公用文件属性 */
        list[idx].base.skip = sizeof(sPAK_TGL_FILE);
        list[idx].base.attr = 0;
        list[idx].base.offs = offs;
        list[idx].base.offs += 4;
    }

    /* 计算文件的包内大小 */
    for (idx = 0; idx < cnt - 1; idx++) {
        if (list[idx].base.offs > list[idx + 1].base.offs) {
            list[idx].base.pack = CR_ULL(0);
        }
        else {
            list[idx].base.pack = list[idx + 1].base.offs -
                                  list[idx + 0].base.offs;
        }
    }
    list[idx].base.pack = dati_get_size(datin) - list[idx].base.offs;

    /* 获取文件的实际大小和类型 */
    for (idx = 0; idx < cnt; idx++) {
        if (!CR_VCALL(datin)->seek64(datin, list[idx].base.offs, SEEK_SET)) {
            err_set(__CR_TGL_PAC_C__, FALSE,
                    "load_tgl_pac()", "iDATIN::seek64() failure");
            goto _failure2;
        }

        /* 压缩文件的标志 */
        read = CR_VCALL(datin)->read(datin, &list[idx].ftype, 4);
        if (read != 4) {
            err_set(__CR_TGL_PAC_C__, read,
                    "load_tgl_pac()", "iDATIN::read() failure");
            goto _failure2;
        }
        if (list[idx].ftype != mk_tag4("IEL1")) {
            list[idx].base.size = list[idx].base.pack;
            list[idx].base.memo = "Storing";
        }
        else {
            if (!CR_VCALL(datin)->getd_le(datin, &offs)) {
                err_set(__CR_TGL_PAC_C__, FALSE,
                        "load_tgl_pac()", "iDATIN::getd_le() failure");
                goto _failure2;
            }
            list[idx].base.size = offs;
            list[idx].base.attr |= PAK_FILE_CMP;
            list[idx].base.memo = "LZSS (IEL1)";
        }
    }

    /* 生成读包接口对象 */
    port = struct_new(iPAK_PACF);
    if (port == NULL) {
        err_set(__CR_TGL_PAC_C__, CR_NULL,
                "load_tgl_pac()", "struct_new() failure");
        goto _failure2;
    }
    port->m_file = datin;
    port->m_cnt = (leng_t)cnt;
    port->pack.__filelst__ = (sPAK_FILE*)list;
    port->pack.__vptr__ = &s_pack_vtbl;
    if (!pack_init_list((iPACKAGE*)port, TRUE)) {
        err_set(__CR_TGL_PAC_C__, FALSE,
                "load_tgl_pac()", "pack_init_list() failure");
        mem_free(port);
        goto _failure2;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        err_set(__CR_TGL_PAC_C__, CR_NULL,
                "load_tgl_pac()", "struct_new() failure");
        iPAK_PACF_release((iPACKAGE*)port);
        return (NULL);
    }
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iPACKAGE";
    rett->infor = "TGL PAC_FILE Archive (PACF)";
    return (rett);

_failure2:
    for (cnt = 0; cnt < idx; cnt++) {
        TRY_FREE(list[cnt].base.find)
        mem_free(list[cnt].base.name);
    }
    mem_free(list);
_failure1:
    CR_VCALL(datin)->release(datin);
    return (NULL);
}

#endif  /* !__CR_TGL_PAC_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
