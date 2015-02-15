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
/*  >>>>>>>>>>>>>>>>> CrHack NScripter NSA 封包读取函数库 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "strlib.h"
#include "../fmtint.h"
#include "fmtz/nscripter.h"

/*****************************************************************************/
/*                                 数据解码                                  */
/*****************************************************************************/

/* LZSS-8 参数 */
#define LZSS_EI     8
#define LZSS_EJ     4
#define LZSS_P      1
#define LZSS_N  ((1 << LZSS_EI))
#define LZSS_F  ((1 << LZSS_EJ) + LZSS_P)

/*
=======================================
    LZSS-8 解压缩
=======================================
*/
CR_API leng_t
uncompr_lzss8 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    leng_t  o_ptr;
    byte_t* o_buf;
    sBITIN  bitin;
    iDATIN* datin;
    uint_t  cc, rr;
    uint_t  ii, jj, kk;
    byte_t  win[LZSS_N];

    /* 建立位流输入对象 */
    datin = create_buff_in(src, srclen, FALSE);
    if (datin == NULL)
        return (0);
    bitin_init(&bitin, datin, sizeof(byte_t));

    o_ptr = 0;
    o_buf = (byte_t*)dst;
    rr = LZSS_N - LZSS_F;
    for (cc = 0; cc < LZSS_N; cc++)
        win[cc] = 0;
    while (o_ptr < dstlen) {
        if (!bitin_hi_push(&bitin, &cc, 1))
            break;
        if (cc) {
            if (!bitin_hi_push(&bitin, &cc, 8))
                goto _failure;
            if (o_ptr >= dstlen)
                goto _failure;
            win[rr++] = (byte_t)cc;
            o_buf[o_ptr++] = (byte_t)cc;
            rr &= (LZSS_N - 1);
        }
        else {
            if (!bitin_hi_push(&bitin, &ii, LZSS_EI))
                goto _failure;
            if (!bitin_hi_push(&bitin, &jj, LZSS_EJ))
                goto _failure;
            for (kk = 0; kk <= jj + 1; kk++) {
                if (o_ptr >= dstlen)
                    goto _failure;
                cc = win[(ii + kk) & (LZSS_N - 1)];
                win[rr++] = (byte_t)cc;
                o_buf[o_ptr++] = (byte_t)cc;
                rr &= (LZSS_N - 1);
            }
        }
    }
    CR_VCALL(datin)->release(datin);
    return (o_ptr);

_failure:
    CR_VCALL(datin)->release(datin);
    return (0);
}

/* 24位色 BMP 文件头 */
static const byte_t _rom_ s_bmp[54] =
{
/*000*/ 0x42, 0x4D,
/*002*/ 0x00, 0x00, 0x00, 0x00, /* 文件大小 */
/*006*/ 0x00, 0x00, 0x00, 0x00,
/*010*/ 0x36, 0x00, 0x00, 0x00,
/*014*/ 0x28, 0x00, 0x00, 0x00,
/*018*/ 0x00, 0x00, 0x00, 0x00, /* 图片宽度 */
/*022*/ 0x00, 0x00, 0x00, 0x00, /* 图片高度 */
/*026*/ 0x01, 0x00,
/*028*/ 0x18, 0x00,
/*030*/ 0x00, 0x00, 0x00, 0x00,
/*034*/ 0x00, 0x00, 0x00, 0x00, /* 数据大小 */
/*038*/ 0x00, 0x00, 0x00, 0x00,
/*042*/ 0x00, 0x00, 0x00, 0x00,
/*046*/ 0x00, 0x00, 0x00, 0x00,
/*050*/ 0x00, 0x00, 0x00, 0x00,
/*054*/
};

/*
=======================================
    SPB (BMP) 解压缩
=======================================
*/
CR_API leng_t
decode_spb (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    int32u  vals;
    byte_t* copy;
    byte_t* line;
    byte_t* image;
    sBITIN  bitin;
    iDATIN* datin;
    leng_t  count;
    uint_t  ii, jj;
    uint_t  mm, nn;
    uint_t  cc, kk;
    int16u  ww, hh;
    byte_t* channel;
    leng_t  img_size;
    leng_t  bmp_size;
    leng_t  bpl, size;

    /* 建立位流输入对象 */
    datin = create_buff_in(src, srclen, FALSE);
    if (datin == NULL)
        return (0);
    bitin_init(&bitin, datin, sizeof(byte_t));

    /* 读取图片宽高 */
    if (!CR_VCALL(datin)->getw_be(datin, &ww))
        goto _failure1;
    if (!CR_VCALL(datin)->getw_be(datin, &hh))
        goto _failure1;
    if (cut_mul(&bpl, ww, 3))
        goto _failure1;
    size = (leng_t)CR_PADDED(bpl, 4);
    if (cut_addu(&bpl, bpl, size))
        goto _failure1;
    if (cut_mul(&img_size, bpl, hh))
        goto _failure1;
    if (cut_addu(&bmp_size, img_size, sizeof(s_bmp)))
        goto _failure1;

    /* 安全检查 */
    if (dstlen < bmp_size)
        goto _failure1;
    mem_cpy(dst, s_bmp, sizeof(s_bmp));
    image = (byte_t*)dst + sizeof(s_bmp);

    /* 生成一个颜色通道的缓存 */
    size  = ww;
    size *= hh;
    channel = (byte_t*)mem_malloc(size + 4);
    if (channel == NULL)
        goto _failure1;

    /* 是三个通道分开压缩的 */
    for (ii = 0; ii < 3; ii++)
    {
        count = 0;
        if (!bitin_hi_push(&bitin, &cc, 8))
            goto _failure2;

        /* 解码一个颜色通道 */
        channel[count++] = (byte_t)cc;
        while (count < size) {
            if (!bitin_hi_push(&bitin, &nn, 3))
                goto _failure2;
            if (nn == 0) {
                channel[count++] = (byte_t)cc;
                channel[count++] = (byte_t)cc;
                channel[count++] = (byte_t)cc;
                channel[count++] = (byte_t)cc;
                continue;
            }
            if (nn == 7) {
                if (!bitin_hi_push(&bitin, &mm, 1))
                    goto _failure2;
                mm += 1;
            }
            else {
                mm = nn + 2;
            }

            for (jj = 0; jj < 4; jj++) {
                if (mm == 8) {
                    if (!bitin_hi_push(&bitin, &cc, 8))
                        goto _failure2;
                }
                else {
                    if (!bitin_hi_push(&bitin, &kk, mm))
                        goto _failure2;
                    if (kk & 1)
                        cc += (kk >> 1) + 1;
                    else
                        cc -= (kk >> 1);
                }
                channel[count++] = (byte_t)cc;
            }
        }

        /* 颜色通道填充到图片数据 */
        line = image + bpl * (hh - 1) + ii;
        copy = channel;
        for (jj = 0; jj < (uint_t)hh; jj++) {
            if (jj & 1) {
                for (kk = 0; kk < (uint_t)ww; kk++, line -= 3)
                    *line = *copy++;
                line -= bpl - 3;
            }
            else {
                for (kk = 0; kk < (uint_t)ww; kk++, line += 3)
                    *line = *copy++;
                line -= bpl + 3;
            }
        }
    }

    /* 填写 BMP 图片信息 */
    vals = DWORD_LE((int32u)bmp_size);
    mem_cpy((byte_t*)dst + 2, &vals, 4);
    ww = WORD_LE(ww);
    mem_cpy((byte_t*)dst + 18, &ww, 2);
    hh = WORD_LE(hh);
    mem_cpy((byte_t*)dst + 22, &hh, 2);
    vals = DWORD_LE((int32u)img_size);
    mem_cpy((byte_t*)dst + 34, &vals, 4);
    mem_free(channel);
    CR_VCALL(datin)->release(datin);
    return (bmp_size);

_failure2:
    mem_free(channel);
_failure1:
    CR_VCALL(datin)->release(datin);
    return (0);
}

/*****************************************************************************/
/*                                 读包接口                                  */
/*****************************************************************************/

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPACKAGE    pack;

        /* 个性部分 */
        uint_t  m_cnt;  /* 文件个数 */
        iDATIN* m_file; /* 文件对象 */

} iPAK_NSA;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPAK_NSA_release (
  __CR_IN__ iPACKAGE*   that
    )
{
    uint_t          idx;
    iPAK_NSA*       real;
    sPAK_NSA_FILE*  list;

    pack_free_list(that);
    real = (iPAK_NSA*)that;
    list = (sPAK_NSA_FILE*)real->pack.__filelst__;
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
iPAK_NSA_getMore (
  __CR_IN__ iPACKAGE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPACKAGE::NS_NSA") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    读取文件个数
---------------------------------------
*/
static int64u
iPAK_NSA_getFileNum (
  __CR_IN__ iPACKAGE*   that
    )
{
    return (((iPAK_NSA*)that)->m_cnt);
}

/*
---------------------------------------
    读取文件数据
---------------------------------------
*/
static bool_t
iPAK_NSA_getFileData (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sBUFFER*    buff,
  __CR_IN__ int64u      index,
  __CR_IN__ bool_t      hash
    )
{
    leng_t          back;
    int64u          pack;
    int64u          size;
    void_t*         temp;
    void_t*         data;
    iDATIN*         file;
    iPAK_NSA*       real;
    sPAK_NSA_FILE*  item;

    /* 定位文件索引 */
    CR_NOUSE(hash);
    real = (iPAK_NSA*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    item = (sPAK_NSA_FILE*)real->pack.__filelst__;
    item += (uint_t)index;

    /* 提前过滤压缩类型 */
    if (item->ftype != 0 && item->ftype != 1 && item->ftype != 2)
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

            if (item->ftype == 1)
            {
                /* SPB (BMP) */
                back = decode_spb(data, (leng_t)size, temp, (leng_t)pack);
                if (back == 0)
                    goto _failure2;
                size = back;
            }
            else
            if (item->ftype == 2)
            {
                /* LZSS-8 */
                back = uncompr_lzss8(data, (leng_t)size, temp, (leng_t)pack);
                if (back != (leng_t)size)
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
iPAK_NSA_getFileInfo (
  __CR_IN__ iPACKAGE*   that,
  __CR_OT__ sPAK_FILE** info,
  __CR_IN__ int64u      index
    )
{
    uint_t          idx;
    iPAK_NSA*       real;
    sPAK_NSA_FILE*  list;

    /* 定位文件索引 */
    real = (iPAK_NSA*)that;
    if (index >= real->m_cnt)
        return (FALSE);
    idx = (uint_t)index;
    list = (sPAK_NSA_FILE*)real->pack.__filelst__;

    /* 返回文件信息 */
    *info = (sPAK_FILE*)(&list[idx]);
    return (TRUE);
}

/* 接口虚函数表 */
static const iPACKAGE_vtbl _rom_ s_pack_vtbl =
{
    iPAK_NSA_release, iPAK_NSA_getMore,
    iPAK_NSA_getFileNum, iPAK_NSA_getFileData,
    iPAK_NSA_getFileInfo,
};

/*
=======================================
    NSA 文件读取
=======================================
*/
CR_API sFMT_PRT*
load_nscr_nsa (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    int32u          beg;
    int16u          idx;
    int16u          cnt;
    byte_t          cha;
    uint_t          tmp;
    int32u          offs;
    int32u          pack;
    int32u          size;
    iPAK_NSA*       port;
    sFMT_PRT*       rett;
    sPAK_NSA_FILE*  list;
    /* --------------- */
    ansi_t  str[MAX_PATHA];

    /* 必须使用自己私有的读取接口 */
    datin = create_file_inX(param);
    if (datin == NULL)
        return (NULL);

    /* 读取文件头信息 (开头可能会填充两个0字节) */
    if (!CR_VCALL(datin)->getw_be(datin, &cnt))
        goto _failure1;
    if (cnt == 0) {
        if (!CR_VCALL(datin)->getw_be(datin, &cnt))
            goto _failure1;
        cha = TRUE;
    }
    else {
        cha = FALSE;
    }
    if (cnt == 0)
        goto _failure1;
    if (!CR_VCALL(datin)->getd_be(datin, &beg))
        goto _failure1;
    if (cha) {
        if (beg > dati_get_size(datin) - sizeof(int16u))
            goto _failure1;
        beg += sizeof(int16u);
    }
    else {
        if (beg > dati_get_size(datin))
            goto _failure1;
    }

    /* 分配子文件属性表 */
    list = mem_talloc(cnt, sPAK_NSA_FILE);
    if (list == NULL)
        goto _failure1;
    mem_tzero(list, cnt, sPAK_NSA_FILE);
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

        /* 文件压缩类型 */
        if (!CR_VCALL(datin)->getb_no(datin, &cha))
            goto _failure2;

        /* 文件偏移和大小 */
        if (!CR_VCALL(datin)->getd_be(datin, &offs))
            goto _failure2;
        if (offs > dati_get_size(datin) - beg)
            goto _failure2;
        if (!CR_VCALL(datin)->getd_be(datin, &pack))
            goto _failure2;
        if (pack > dati_get_size(datin) - beg - offs)
            goto _failure2;
        if (!CR_VCALL(datin)->getd_be(datin, &size))
            goto _failure2;

        /* 文件名统一使用 UTF-8 编码 */
        list[idx].base.name = local_to_utf8(param->page, str);
        if (list[idx].base.name == NULL)
            goto _failure2;

        /* 设置公用文件属性 */
        list[idx].base.skip = sizeof(sPAK_NSA_FILE);
        list[idx].base.attr = 0;
        list[idx].base.offs = beg;
        list[idx].base.offs += offs;
        list[idx].base.pack = pack;
        list[idx].base.size = size;

        /* 设置私有文件属性 */
        list[idx].ftype = cha;
        if (cha != 0)
            list[idx].base.attr |= PAK_FILE_CMP;
        switch (cha)
        {
            case 0:  list[idx].base.memo = "Store";        break;
            case 1:  list[idx].base.memo = "SPB (BMP)";    break;
            case 2:  list[idx].base.memo = "LZSS-8";       break;
            default: list[idx].base.memo = "Unknown type"; break;
        }
    }

    /* 生成读包接口对象 */
    port = struct_new(iPAK_NSA);
    if (port == NULL)
        goto _failure2;
    port->m_cnt = cnt;
    port->m_file = datin;
    port->pack.__filelst__ = (sPAK_FILE*)list;
    port->pack.__vptr__ = &s_pack_vtbl;
    if (!pack_init_list((iPACKAGE*)port, TRUE)) {
        mem_free(port);
        goto _failure2;
    }

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iPAK_NSA_release((iPACKAGE*)port);
        return (NULL);
    }
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iPACKAGE";
    rett->infor = "NScripter Archive (NSA)";
    return (rett);

_failure2:
    for (cnt = 0; cnt < idx; cnt++) {
        TRY_FREE(list[cnt].base.find);
        mem_free(list[cnt].base.name);
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
