/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-03-04  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack RAR FMTZ 插件接口实现 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "fmtint.h"
#include "strlib.h"
#include "fmtz/rar.h"

/* 补上类型缺失 */
#ifndef _UNIX
    #define CALLBACK STDCALL
    #define PASCAL STDCALL
    #define HANDLE void_t*
    #define LPARAM size_t
    #define UINT unsigned int
#endif
#include "unrar/unrar.h"
typedef struct RARHeaderDataEx RARHeaderDataEx;
typedef struct RAROpenArchiveDataEx RAROpenArchiveDataEx;

/* UnRAR 导入库 */
#ifndef _CR_NO_PRAGMA_LIB_
    #if     defined(_CR_SYS32_)
        #pragma comment (lib, "unrar.lib")
    #elif   defined(_CR_SYS64_)
        #pragma comment (lib, "unrar64.lib")
    #endif
#endif

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPACKAGE    pack;

        /* 个性部分 */
        HANDLE  m_rar;      /* RAR 对象 */
        leng_t  m_cnt;      /* 文件个数 */
        leng_t  m_cur;      /* 当前序号 */
        ansi_t* m_pass;     /* 保存的密码 */
        ansi_t* m_ansi;     /* 保存的文件名A */
        wide_t* m_wide;     /* 保存的文件名W */
        void_t* m_temp;     /* 临时用的指针 */

} iPAK_RAR;

/*
---------------------------------------
    复制数据的回调
---------------------------------------
*/
static int PASCAL
rar_mem_copy (
  __CR_IN__ UINT    msg,
  __CR_IN__ LPARAM  UserData,
  __CR_IN__ LPARAM  P1,
  __CR_IN__ LPARAM  P2
    )
{
    byte_t*     recv;
    void_t**    addr;

    if (msg == UCM_PROCESSDATA && UserData != 0)
    {
        /* 测试时复制数据 */
        addr = (void_t**)UserData;
        recv = (byte_t*)(addr[0]);
        if (recv != NULL) {
            mem_cpy(recv, (void_t*)P1, (leng_t)P2);
            recv += P2;
            addr[0] = recv;
        }
    }
    return (TRUE);
}

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPAK_RAR_release (
  __CR_IN__ iPACKAGE*   that
    )
{
    leng_t          idx;
    iPAK_RAR*       real;
    sPAK_RAR_FILE*  list;

    pack_free_list(that);
    real = (iPAK_RAR*)that;
    list = (sPAK_RAR_FILE*)real->pack.__filelst__;
    if (list != NULL) {
        for (idx = 0; idx < real->m_cnt; idx++) {
            mem_free(list[idx].base.find);
            mem_free(list[idx].base.name);
        }
        mem_free(list);
    }
    if (real->m_rar != NULL)
        RARCloseArchive(real->m_rar);
    TRY_FREE(real->m_ansi);
    TRY_FREE(real->m_wide);
    TRY_FREE(real->m_pass);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iPAK_RAR_getMore (
  __CR_IN__ iPACKAGE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPACKAGE::RAR") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    读取文件个数
---------------------------------------
*/
static int64u
iPAK_RAR_getFileNum (
  __CR_IN__ iPACKAGE*   that
    )
{
    return (((iPAK_RAR*)that)->m_cnt);
}

/*
---------------------------------------
    读取文件数据
---------------------------------------
*/
static bool_t
iPAK_RAR_getFileData (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sBUFFER*    buff,
  __CR_IN__ int64u      index,
  __CR_IN__ bool_t      hash
    )
{
    sint_t                  rett;
    int64u                  size;
    void_t*                 data;
    iPAK_RAR*               real;
    sPAK_RAR_FILE*          item;
    RARHeaderDataEx         info;
    RAROpenArchiveDataEx    open;

    /* 定位文件索引 */
    CR_NOUSE(hash);
    real = (iPAK_RAR*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    item = (sPAK_RAR_FILE*)real->pack.__filelst__;
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
        real->m_temp = data = mem_malloc64(size);
        if (data == NULL)
            return (FALSE);

        /* RAR 只能顺序读取文件 */
        if (real->m_rar == NULL || item->id < real->m_cur)
        {
            /* 需要重新打开封包 */
            if (real->m_rar != NULL) {
                RARCloseArchive(real->m_rar);
                real->m_rar = NULL;
            }
            struct_zero(&open, RAROpenArchiveDataEx);
            if (real->m_ansi != NULL)
                open.ArcName = real->m_ansi;
            else
                open.ArcNameW = real->m_wide;
            open.OpenMode = RAR_OM_EXTRACT;
            open.Callback = rar_mem_copy;
            open.UserData = (LPARAM)(&real->m_temp);
            real->m_rar = RAROpenArchiveEx(&open);
            if (real->m_rar == NULL)
                goto _failure1;
            if (real->m_pass != NULL)
                RARSetPassword(real->m_rar, real->m_pass);
            real->m_cur = 0;
        }

        /* 定位到指定文件 */
        struct_zero(&info, RARHeaderDataEx);
        while (real->m_cur != item->id) {
            rett = RARReadHeaderEx(real->m_rar, &info);
            if (rett != ERAR_SUCCESS)
                goto _failure2;
            rett = RARProcessFile(real->m_rar, RAR_SKIP, NULL, NULL);
            if (rett != ERAR_SUCCESS && rett != ERAR_BAD_DATA)
                goto _failure2;
            real->m_cur += 1;
        }

        /* 测试目标文件就不会有磁盘操作了 */
        rett = RARReadHeaderEx(real->m_rar, &info);
        if (rett != ERAR_SUCCESS)
            goto _failure2;
        rett = RARProcessFile(real->m_rar, RAR_TEST, NULL, NULL);
        if (rett != ERAR_SUCCESS)
            goto _failure2;
        real->m_cur += 1;
    }

    /* 返回文件数据 */
    return (buffer_init(buff, data, (leng_t)size, TRUE));

_failure2:
    RARCloseArchive(real->m_rar);
    real->m_rar = NULL;
_failure1:
    real->m_cur = (leng_t)-1;
    mem_free(data);
    return (FALSE);
}

/*
---------------------------------------
    读取文件信息
---------------------------------------
*/
static bool_t
iPAK_RAR_getFileInfo (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sPAK_FILE** info,
  __CR_IN__ int64u      index
    )
{
    leng_t          idx;
    iPAK_RAR*       real;
    sPAK_RAR_FILE*  list;

    /* 定位文件索引 */
    real = (iPAK_RAR*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    idx = (leng_t)index;
    list = (sPAK_RAR_FILE*)real->pack.__filelst__;

    /* 返回文件信息 */
    *info = (sPAK_FILE*)(&list[idx]);
    return (TRUE);
}

/* 接口虚函数表 */
static const iPACKAGE_vtbl _rom_ s_pack_vtbl =
{
    iPAK_RAR_release, iPAK_RAR_getMore,
    iPAK_RAR_getFileNum, iPAK_RAR_getFileData,
    iPAK_RAR_getFileInfo,
};

/*
---------------------------------------
    文件单元释放回调
---------------------------------------
*/
static void_t
rar_free (
  __CR_IN__ void_t* obj
    )
{
    sPAK_RAR_FILE*  unit;

    unit = (sPAK_RAR_FILE*)obj;
    TRY_FREE(unit->base.find);
    mem_free(unit->base.name);
}

/*
=======================================
    RAR 文件读取
=======================================
*/
CR_API sFMT_PRT*
load_rar (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    HANDLE                  rar;
    sARRAY                  list;
    int32u                  attr;
    sint_t                  retc;
    leng_t                  fpos;
    iPAK_RAR*               port;
    sFMT_PRT*               rett;
    sPAK_RAR_FILE           temp;
    RARHeaderDataEx         info;
    RAROpenArchiveDataEx    open;

    /* 只支持磁盘文件 */
    if (param->type != CR_LDR_ANSI &&
        param->type != CR_LDR_WIDE)
        return (NULL);

    /* 列表模式打开 RAR 文件 */
    struct_zero(&open, RAROpenArchiveDataEx);
    if (param->type == CR_LDR_ANSI)
        open.ArcName = (ansi_t*)param->name.ansi;
    else
        open.ArcNameW = (wide_t*)param->name.wide;
    open.OpenMode = RAR_OM_LIST;
    rar = RAROpenArchiveEx(&open);
    if (rar == NULL)
        return (NULL);
    if (param->aprm != NULL &&
        *(byte_t*)param->aprm != 0x00) {
        RARSetPassword(rar, (ansi_t*)param->aprm);
        attr = PAK_FILE_ENC;
    }
    else {
        attr = 0;
    }

    /* 开始逐个文件读取信息并定位 */
    array_initT(&list, sPAK_RAR_FILE);
    list.free = rar_free;
    struct_zero(&info, RARHeaderDataEx);
    for (fpos = 0; ; fpos++)
    {
        /* 读取一个文件记录头 */
        retc = RARReadHeaderEx(rar, &info);
        if (retc == ERAR_END_ARCHIVE)
            break;
        if (retc != ERAR_SUCCESS)
            goto _failure1;

        /* 目录文件不加入列表 */
        if (info.Flags & RHDF_DIRECTORY) {
            retc = RARProcessFile(rar, RAR_SKIP, NULL, NULL);
            if (retc != ERAR_SUCCESS)
                goto _failure1;
            continue;
        }

        /* 文件名统一使用 UTF-8 编码 */
        struct_zero(&temp, sPAK_RAR_FILE);
        temp.base.name = local_to_utf8(param->page, info.FileName);
        if (temp.base.name == NULL)
            goto _failure1;

        /* 设置公用文件属性 (偏移没有实际用处) */
        temp.base.skip = sizeof(sPAK_RAR_FILE);
        temp.base.attr = attr;
        temp.base.offs = 0;
        temp.base.pack = mk_size(info.PackSizeHigh, info.PackSize);
        temp.base.size = mk_size(info.UnpSizeHigh, info.UnpSize);
        if (info.Method != 0x30)
            temp.base.attr |= PAK_FILE_CMP;
        if (info.Flags & RHDF_ENCRYPTED)
            temp.base.attr |=  PAK_FILE_ENC;
        else
            temp.base.attr &= ~PAK_FILE_ENC;
        switch (info.Method)
        {
            case 0x30: temp.base.memo = "Storing";             break;
            case 0x31: temp.base.memo = "Fastest compression"; break;
            case 0x32: temp.base.memo = "Fast compression";    break;
            case 0x33: temp.base.memo = "Normal compression";  break;
            case 0x34: temp.base.memo = "Good compression";    break;
            case 0x35: temp.base.memo = "Best compression";    break;
            default:   temp.base.memo = "Unknown compression"; break;
        }

        /* 设置私有文件属性 */
        temp.id = fpos;
        temp.crc32 = (int32u)(info.FileCRC);
        temp.fattr = (int32u)(info.FileAttr);
        temp.ftime = (int16u)(info.FileTime & 0xFFFF);
        temp.fdate = (int16u)(info.FileTime >> 16);
        temp.htype = (int32u)(info.HashType);
        mem_cpy(temp.hash, info.Hash, sizeof(temp.hash));

        /* 文件信息压入列表 */
        if (array_push_growT(&list, sPAK_RAR_FILE, &temp) == NULL) {
            mem_free(temp.base.name);
            goto _failure1;
        }

        /* 跳过当前已读文件 */
        retc = RARProcessFile(rar, RAR_SKIP, NULL, NULL);
        if (retc != ERAR_SUCCESS && retc != ERAR_BAD_DATA)
            goto _failure1;
    }

    /* 固定一下列表大小 */
    if (!array_no_growT(&list, sPAK_RAR_FILE))
        goto _failure1;

    /* 生成读包接口对象 */
    port = struct_new(iPAK_RAR);
    if (port == NULL)
        goto _failure1;

    /* 保存需要用到的参数 */
    port->m_temp = NULL;
    if (attr == 0) {
        port->m_pass = NULL;
    }
    else {
        port->m_pass = str_dupA((ansi_t*)param->aprm);
        if (port->m_pass == NULL)
            goto _failure2;
    }
    if (param->type == CR_LDR_ANSI) {
        port->m_wide = NULL;
        port->m_ansi = str_dupA(param->name.ansi);
        if (port->m_ansi == NULL)
            goto _failure3;
    }
    else {
        port->m_ansi = NULL;
        port->m_wide = str_dupW(param->name.wide);
        if (port->m_wide == NULL)
            goto _failure3;
    }
    port->m_rar = NULL;
    port->m_cur = (leng_t)-1;
    port->m_cnt = array_get_sizeT(&list, sPAK_RAR_FILE);
    port->pack.__filelst__ = array_get_dataT(&list, sPAK_FILE);
    port->pack.__vptr__ = &s_pack_vtbl;
    if (!pack_init_list((iPACKAGE*)port, TRUE))
        goto _failure4;
    RARCloseArchive(rar);

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iPAK_RAR_release((iPACKAGE*)port);
        return (NULL);
    }
    CR_NOUSE(datin);
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iPACKAGE";
    rett->infor = "Roshal ARchive (RAR)";
    return (rett);

_failure4:
    TRY_FREE(port->m_ansi);
    TRY_FREE(port->m_wide);
_failure3:
    TRY_FREE(port->m_pass);
_failure2:
    mem_free(port);
_failure1:
    array_freeT(&list, sPAK_RAR_FILE);
    RARCloseArchive(rar);
    return (NULL);
}

/*
---------------------------------------
    文件格式加载接口
---------------------------------------
*/
static sFMTZ*
engine_rar_load (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    if (!(engine->mask & CR_FMTZ_MASK_PAK))
        return (NULL);
    return ((sFMTZ*)load_rar(NULL, loader));
}

/*
=======================================
    获取引擎插件接口
=======================================
*/
CR_API sENGINE*
engine_rar (void_t)
{
    sENGINE*    engine;

    engine = engine_init(NULL, NULL, NULL, NULL);
    if (engine == NULL)
        return (NULL);
    engine->fmtz_load = engine_rar_load;
    engine->info = "RAR FMTz Engine (Done by CrHackOS)";
    return (engine);
}

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    获取引擎插件接口 (同名)
=======================================
*/
CR_API sENGINE*
engine_get (void_t)
{
    return (engine_rar());
}
#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
