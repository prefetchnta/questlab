/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-09-09  */
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
/*  >>>>>>>>>>>>>>>>> CrHack KiriKiri2 XP3 封包读取函数库 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "enclib.h"
#include "strlib.h"
#include "../fmtint.h"
#include "fmtz/kirikiri.h"

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPACKAGE    pack;

        /* 个性部分 */
        leng_t  m_cnt;  /* 文件个数 */
        iDATIN* m_file; /* 文件对象 */

        /* 文件解密回调 */
        void_t  (*decode_file) (void_t *data, leng_t size,
                                const sPAK_XP3_FILE *info);
        /* 数据块解密回调 */
        void_t  (*decode_block) (void_t *data, leng_t size,
                        leng_t offset, const sPAK_XP3_FILE *info);
} iPAK_XP3;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPAK_XP3_release (
  __CR_IN__ iPACKAGE*   that
    )
{
    leng_t          idx;
    iPAK_XP3*       real;
    sPAK_XP3_FILE*  list;

    pack_free_list(that);
    real = (iPAK_XP3*)that;
    list = (sPAK_XP3_FILE*)real->pack.__filelst__;
    if (list != NULL) {
        for (idx = 0; idx < real->m_cnt; idx++) {
            mem_free(list[idx].segm_lst);
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
iPAK_XP3_getMore (
  __CR_IN__ iPACKAGE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPACKAGE::KRKR_XP3") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    读取文件个数
---------------------------------------
*/
static int64u
iPAK_XP3_getFileNum (
  __CR_IN__ iPACKAGE*   that
    )
{
    return (((iPAK_XP3*)that)->m_cnt);
}

/*
---------------------------------------
    读取文件数据
---------------------------------------
*/
static bool_t
iPAK_XP3_getFileData (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sBUFFER*    buff,
  __CR_IN__ int64u      index,
  __CR_IN__ bool_t      hash
    )
{
    leng_t          pntr;
    leng_t          back;
    leng_t          segm;
    int64u          pack;
    int64u          size;
    void_t*         temp;
    void_t*         data;
    iDATIN*         file;
    iPAK_XP3*       real;
    sPAK_XP3_FILE*  item;

    /* 定位文件索引 */
    CR_NOUSE(hash);
    real = (iPAK_XP3*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    item = (sPAK_XP3_FILE*)real->pack.__filelst__;
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
        pack = item->base.pack;
        temp = mem_malloc64(pack);
        if (temp == NULL)
            return (FALSE);
        data = mem_malloc64(size);
        if (data == NULL)
            goto _failure1;
        file = real->m_file;

        /* 定位到文件并读起数据 */
        for (pntr = 0, segm = 0; segm < item->segm_cnt; segm++)
        {
            /* 逐块读取文件 */
            if (!CR_VCALL(file)->seek64(file,
                    item->segm_lst[segm].offset, SEEK_SET))
                goto _failure2;

            /* 是否为 ZLib 压缩 */
            if (item->segm_lst[segm].zlib) {
                back = CR_VCALL(file)->read(file, temp,
                            (leng_t)item->segm_lst[segm].pksize);
                if (back != (leng_t)item->segm_lst[segm].pksize)
                    goto _failure2;
                back = uncompr_zlib((byte_t*)data + pntr,
                            (leng_t)size - pntr, temp, back);
                if (back != (leng_t)item->segm_lst[segm].unsize)
                    goto _failure2;
            }
            else {
                back = CR_VCALL(file)->read(file, (byte_t*)data + pntr,
                            (leng_t)item->segm_lst[segm].unsize);
                if (back != (leng_t)item->segm_lst[segm].unsize)
                    goto _failure2;
            }

            /* 数据块解密 */
            if (real->decode_block != NULL)
                real->decode_block((byte_t*)data + pntr, back, pntr, item);
            pntr += back;
        }

        /* 整个文件解密 */
        if (real->decode_file != NULL)
            real->decode_file(data, (leng_t)size, item);
        mem_free(temp);
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
iPAK_XP3_getFileInfo (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sPAK_FILE** info,
  __CR_IN__ int64u      index
    )
{
    leng_t          idx;
    iPAK_XP3*       real;
    sPAK_XP3_FILE*  list;

    /* 定位文件索引 */
    real = (iPAK_XP3*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    idx = (leng_t)index;
    list = (sPAK_XP3_FILE*)real->pack.__filelst__;

    /* 返回文件信息 */
    *info = (sPAK_FILE*)(&list[idx]);
    return (TRUE);
}

/* 接口虚函数表 */
static const iPACKAGE_vtbl _rom_ s_pack_vtbl =
{
    iPAK_XP3_release, iPAK_XP3_getMore,
    iPAK_XP3_getFileNum, iPAK_XP3_getFileData,
    iPAK_XP3_getFileInfo,
};

/* 文件数据解密库 */
#include "krkr_dec_cmm.inl"

/*
---------------------------------------
    解密回调设置
---------------------------------------
*/
static void_t
decode_setup (
  __CR_OT__ iPAK_XP3*       that,
  __CR_IN__ const ansi_t*   type
    )
{
    int32u  crc32;
    leng_t  len = str_lenA(type);

    crc32 = hash_crc32i_total(type, len);
    switch (crc32)
    {
        default:
            that->decode_file = NULL;
            that->decode_block = NULL;
            break;

        case KRKR_AUTO_XOR8:
            that->decode_file = dec_auto_xor8;
            that->decode_block = NULL;
            break;
    }
}

/* XP3 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* XP3 文件头结构 */
CR_TYPEDEF struct
{
        byte_t  tag[11];    /* 文件头标志 */
        int64u  idx_pos;    /* 索引表位置 */

} CR_PACKED sXP3_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
---------------------------------------
    文件单元释放回调
---------------------------------------
*/
static void_t
xp3_free (
  __CR_IN__ void_t* obj
    )
{
    sPAK_XP3_FILE*  unit;

    unit = (sPAK_XP3_FILE*)obj;
    mem_free(unit->segm_lst);
    TRY_FREE(unit->base.find);
    mem_free(unit->base.name);
}

/*
=======================================
    KrKr XP3 文件读取
=======================================
*/
CR_API sFMT_PRT*
load_krkr_xp3 (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    int16u          idx;
    int16u          len;
    int32u          attr;
    int64u          tots;
    int64u          tsz1;
    int64u          tsz2;
    int64u          pksz;
    int64u          unsz;
    leng_t          back;
    sARRAY          list;
    byte_t*         info;
    byte_t*         pntr;
    wide_t*         name;
    sXP3_HDR        head;
    iPAK_XP3*       port;
    sFMT_PRT*       rett;
    sPAK_XP3_FILE   temp;

    /* 必须使用自己私有的读取接口 */
    datin = create_file_inX(param);
    if (datin == NULL)
        return (NULL);
    array_initT(&list, sPAK_XP3_FILE);
    list.free = xp3_free;

    /* 读取文件头信息 */
    if (!(CR_VCALL(datin)->geType(datin, &head, sXP3_HDR)))
        goto _failure1;
    if (mem_cmp(head.tag, "XP3\r\n \n\x1A\x8B\x67\x01", 11) != 0)
        goto _failure1;
    head.idx_pos = QWORD_LE(head.idx_pos);

    /* 读取文件索引表 */
    if (!CR_VCALL(datin)->seek64(datin, head.idx_pos, SEEK_SET))
        goto _failure1;
    if (!CR_VCALL(datin)->getb_no(datin, head.tag))
        goto _failure1;
    if (head.tag[0])
    {
        /* ZLib 压缩的索引表 */
        if (!CR_VCALL(datin)->getq_le(datin, &pksz))
            goto _failure1;
        if (!CR_VCALL(datin)->getq_le(datin, &unsz))
            goto _failure1;
        pntr = (byte_t*)mem_malloc64(pksz);
        if (pntr == NULL)
            goto _failure1;
        back = CR_VCALL(datin)->read(datin, pntr, (leng_t)pksz);
        if (back != (leng_t)pksz) {
            mem_free(pntr);
            goto _failure1;
        }
        info = (byte_t*)mem_malloc64(unsz);
        if (info == NULL) {
            mem_free(pntr);
            goto _failure1;
        }
        back = uncompr_zlib(info, (leng_t)unsz, pntr, (leng_t)pksz);
        mem_free(pntr);
        if (back != (leng_t)unsz)
            goto _failure2;
    }
    else
    {
        /* 直接读起索引表 */
        if (!CR_VCALL(datin)->getq_le(datin, &unsz))
            goto _failure1;
        info = (byte_t*)mem_malloc64(unsz);
        if (info == NULL)
            goto _failure1;
        back = CR_VCALL(datin)->read(datin, info, (leng_t)unsz);
        if (back != (leng_t)unsz)
            goto _failure2;
    }

    /* 设置加密类型 (如果有的话) */
    if (param->aprm != NULL &&
        *(byte_t*)param->aprm != 0x00)
        attr = PAK_FILE_ENC;
    else
        attr = 0;

    /* 加载文件信息表 */
    for (pntr = info, pksz = 0; pksz < unsz;)
    {
        /* "File" 数据块 */
        if (mem_cmp(pntr, "File", 4) != 0)
            break;
        mem_cpy(&tots, pntr + 4, sizeof(int64u));
        tots = QWORD_LE(tots);
        pntr += 12; pksz += 12;
        if (tots <= 12 + 22 + 12 + 28 || tots > unsz - pksz)
            goto _failure2;

        /* "info" 文件总信息 */
        if (mem_cmp(pntr, "info", 4) != 0)
            goto _failure2;
        mem_cpy(&tsz1, pntr + 4, sizeof(int64u));
        tsz1 = QWORD_LE(tsz1);
        pntr += 12; pksz += 12;
        if (tsz1 <= 22 || tsz1 > tots - 12 - 12 - 28)
            goto _failure2;

        /* 填充一些文件信息结构成员 */
        struct_zero(&temp, sPAK_XP3_FILE);
        mem_cpy(&temp.protect,   pntr +  0, sizeof(int32u));
        mem_cpy(&temp.base.size, pntr +  4, sizeof(int64u));
        mem_cpy(&temp.base.pack, pntr + 12, sizeof(int64u));
        temp.base.skip = sizeof(sPAK_XP3_FILE);
        temp.base.attr = attr;
        temp.protect   = DWORD_LE(temp.protect);
        temp.base.size = QWORD_LE(temp.base.size);
        temp.base.pack = QWORD_LE(temp.base.pack);

        /* 读取文件名 UTF-16 无\0结尾 */
        mem_cpy(&len, pntr + 20, sizeof(int16u));
        len = WORD_LE(len);
        if ((int64u)len > tsz1 - 22)
            goto _failure2;
        name = str_allocW(len + 1);
        if (name == NULL)
            goto _failure2;
        for (idx = 0; idx < len; idx++) {
            mem_cpy(&name[idx], pntr + 22 + ((leng_t)idx) * 2, 2);
            name[idx] = WORD_LE(name[idx]);
        }
        name[idx] = 0x0000;
        temp.base.name = utf16_to_utf8(name);
        mem_free(name);
        if (temp.base.name == NULL)
            goto _failure2;
        pntr += (leng_t)tsz1;
        pksz += (leng_t)tsz1;

        /* "segm" 文件分段信息 */
        if (mem_cmp(pntr, "segm", 4) != 0)
            goto _failure3;
        mem_cpy(&tsz2, pntr + 4, sizeof(int64u));
        tsz2 = QWORD_LE(tsz2);
        pntr += 12; pksz += 12;
        if (tsz2 < 28 || tsz2 % 28 != 0 ||
            tsz2 > tots - 12 - tsz1 - 12)
            goto _failure3;

        /* 读取所有分段信息 */
        temp.segm_lst = mem_talloc64(tsz2 / 28, sPAK_XP3_SEGM);
        if (temp.segm_lst == NULL)
            goto _failure3;
        temp.segm_cnt = (leng_t)(tsz2 / 28);
        mem_cpy(temp.segm_lst, pntr, (leng_t)tsz2);
        for (back = 0; back < temp.segm_cnt; back++) {
            temp.segm_lst[back].zlib   = DWORD_LE(temp.segm_lst[back].zlib);
            temp.segm_lst[back].offset = QWORD_LE(temp.segm_lst[back].offset);
            temp.segm_lst[back].unsize = QWORD_LE(temp.segm_lst[back].unsize);
            temp.segm_lst[back].pksize = QWORD_LE(temp.segm_lst[back].pksize);
            if (temp.segm_lst[back].unsize > temp.base.size)
                goto _failure4;
            if (temp.segm_lst[back].pksize > temp.base.pack)
                goto _failure4;
        }
        pntr += (leng_t)tsz2;
        pksz += (leng_t)tsz2;

        /* 其他非重要数据段 */
        if (tots > 12 + tsz1 + 12 + tsz2)
        {
            tots -= 12 + tsz1 + 12 + tsz2;

            /* "adlr" 附加数据 */
            if (mem_cmp(pntr, "adlr", 4) == 0) {
                if (tots < 16)
                    goto _failure4;
                mem_cpy(&temp.adlr_key, pntr + 4, sizeof(int32u));
                temp.adlr_key = DWORD_LE(temp.adlr_key);
            }
            pntr += (leng_t)tots;
            pksz += (leng_t)tots;
        }

        /* 用第一段设置剩下的成员 */
        temp.base.offs = temp.segm_lst[0].offset;
        if (temp.segm_lst[0].zlib) {
            if (attr == 0)
                temp.base.memo = "ZLib";
            else
                temp.base.memo = "ZLib + Encrypt";
            temp.base.attr |= PAK_FILE_CMP;
        }
        else {
            if (attr == 0)
                temp.base.memo = "Store";
            else
                temp.base.memo = "Encrypt";
        }

        /* 文件信息压入列表 */
        if (array_push_growT(&list, sPAK_XP3_FILE, &temp) == NULL)
            goto _failure4;
    }
    mem_free(info);

    /* 固定一下列表大小 */
    if (!array_no_growT(&list, sPAK_XP3_FILE))
        goto _failure1;

    /* 生成读包接口对象 */
    port = struct_new(iPAK_XP3);
    if (port == NULL)
        goto _failure1;
    port->m_file = datin;
    port->m_cnt = array_get_sizeT(&list, sPAK_XP3_FILE);
    port->pack.__filelst__ = array_get_dataT(&list, sPAK_FILE);
    port->pack.__vptr__ = &s_pack_vtbl;
    if (!pack_init_list((iPACKAGE*)port, TRUE)) {
        mem_free(port);
        goto _failure1;
    }

    /* 设置解密回调 (传入名称字符串) */
    if (attr != 0) {
        decode_setup(port, (ansi_t*)param->aprm);
    }
    else {
        port->decode_file = NULL;
        port->decode_block = NULL;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iPAK_XP3_release((iPACKAGE*)port);
        return (NULL);
    }
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iPACKAGE";
    rett->infor = "KiriKiri2 XP3 Archive (XP3)";
    return (rett);

_failure4:
    mem_free(temp.segm_lst);
_failure3:
    mem_free(temp.base.name);
_failure2:
    mem_free(info);
_failure1:
    array_freeT(&list, sPAK_XP3_FILE);
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
