/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-29  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 文件格式函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "safe.h"
#include "fmtint.h"
#include "strlib.h"

/*****************************************************************************/
/*                               插件功能封装                                */
/*****************************************************************************/

/*
---------------------------------------
    使用文件名的扩展名作为键A
---------------------------------------
*/
static uint_t
match_findA (
  __CR_IN__ const void_t*   key
    )
{
    leng_t  idx, len;

    /* 注意: 文件名必须都小写 */
    len = str_lenA((ansi_t*)key);
    for (idx = len; idx != 0; idx--) {
        if (((ansi_t*)key)[idx - 1] == CR_AC('.')) {
            key = (void_t*)(&(((ansi_t*)key)[idx - 1]));
            return (hash_str_djb2(key, (len - idx + 1) * sizeof(ansi_t)));
        }
    }
    return (hash_str_djb2(key, 0));     /* 无扩展名或不确定 */
}

/*
---------------------------------------
    使用文件名的扩展名作为键W
---------------------------------------
*/
static uint_t
match_findW (
  __CR_IN__ const void_t*   key
    )
{
    leng_t  idx, len;

    /* 注意: 文件名必须都小写 */
    len = str_lenW((wide_t*)key);
    for (idx = len; idx != 0; idx--) {
        if (((wide_t*)key)[idx - 1] == CR_WC('.')) {
            key = (void_t*)(&(((wide_t*)key)[idx - 1]));
            return (hash_str_djb2(key, (len - idx + 1) * sizeof(wide_t)));
        }
    }
    return (hash_str_djb2(key, 0));     /* 无扩展名或不确定 */
}

/*
=======================================
    释放引擎插件
=======================================
*/
CR_API void_t
engine_free (
  __CR_IN__ sENGINE*    engine
    )
{
    sENGINE_INT*    real;

    real = (sENGINE_INT*)engine;
    if (engine->engine_free != NULL)
        engine->engine_free(engine);
    curtain_freeT(&real->m_findw, sMATCHw);
    curtain_freeT(&real->m_finda, sMATCHa);
    if (engine->sbin != NULL)
        sbin_unload(engine->sbin);
    mem_free(engine);
}

/*
=======================================
    初始化引擎插件
=======================================
*/
CR_API sENGINE*
engine_init (
  __CR_IN__ const sMATCHa*      finda,
  __CR_IN__ const sMATCHw*      findw,
  __CR_IN__ const sTRY_LDRa*    loada,
  __CR_IN__ const sTRY_LDRw*    loadw
    )
{
    leng_t          count;
    sENGINE_INT*    engine;

    engine = struct_new(sENGINE_INT);
    if (engine == NULL)
        return (NULL);
    struct_zero(engine, sENGINE_INT);

    /* 生成两个哈希表并注册文件类型匹配 */
    if (finda != NULL) {
        for (count = 0; ; count++) {
            if (finda[count].loader == NULL)
                break;
        }
        if (count != 0) {
            if (!curtain_initT(&engine->m_finda, sMATCHa, count, 0))
                goto _failure1;
            engine->m_finda.find = match_findA;

            while (finda->loader != NULL) {
                if (curtain_insertT(&engine->m_finda, sMATCHa,
                                    finda->filext, finda) == NULL)
                    goto _failure2;
                finda++;
            }
        }
    }
    if (findw != NULL) {
        for (count = 0; ; count++) {
            if (findw[count].loader == NULL)
                break;
        }
        if (count != 0) {
            if (!curtain_initT(&engine->m_findw, sMATCHw, count, 0))
                goto _failure2;
            engine->m_findw.find = match_findW;

            while (findw->loader != NULL) {
                if (curtain_insertT(&engine->m_findw, sMATCHw,
                                    findw->filext, findw) == NULL)
                    goto _failure3;
                findw++;
            }
        }
    }
    engine->m_loada = loada;
    engine->m_loadw = loadw;
    engine->engine.mask = CR_FMTZ_MASK_ALL;
    return ((sENGINE*)engine);

_failure3:
    curtain_freeT(&engine->m_findw, sMATCHw);
_failure2:
    curtain_freeT(&engine->m_finda, sMATCHa);
_failure1:
    mem_free(engine);
    return (NULL);
}

/*
=======================================
    文件格式模式匹配
=======================================
*/
CR_API sFMTZ*
fmtz_find (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    leng_t          idx;
    sFMTZ*          fmtz;
    uint_t          hash;
    sARRAY*         node;
    iDATIN*         datin;
    ansi_t*         name_a;
    wide_t*         name_w;
    sMATCHa*        match_a;
    sMATCHw*        match_w;
    sTRY_LDRa*      try_ldra;
    sTRY_LDRw*      try_ldrw;
    load_fmtz_t     fmtz_loader;
    sENGINE_INT*    real = (sENGINE_INT*)engine;

    /* 尝试打开文件 */
    datin = create_file_inX(loader);
    if (datin == NULL)
        return (NULL);
    fmtz = NULL;

    /* 单字节匹配优先 */
    if (loader->name.ansi != NULL) {
        if (real->m_finda.__list__ == NULL)
            goto _func_out;
        name_a = str_dupA(loader->name.ansi);
        if (name_a == NULL)
            goto _func_out;

        /* 文件名规则匹配 */
        flname_extractA(name_a, loader->name.ansi);
        hash = match_findA(str_lwrA(name_a));
        hash %= real->m_finda.__size__;
        node = &real->m_finda.__list__[hash];
        match_a = (sMATCHa*)node->__buff__;
        for (idx = 0; idx < node->__cnts__; idx++, match_a++)
        {
            /* 必须是期望接口类型 */
            if (!(match_a->maskz & engine->mask))
                continue;

            /* 扩展名与文件名必须符合规则 */
            if (!filext_checkA(name_a, match_a->filext))
                continue;
            if (match_a->match != NULL) {
                if (!wildcard_matchIA(name_a, match_a->match))
                    continue;
            }

            /* 尝试加载目标文件 */
            fmtz_loader = (load_fmtz_t)match_a->loader;
            fmtz = fmtz_loader(datin, loader);
            if (fmtz != NULL)
                break;

            /* 复位文件读取指针 */
            if (!CR_VCALL(datin)->rewind(datin)) {
                mem_free(name_a);
                goto _func_out;
            }
        }

        /* 逐个尝试加载匹配 */
        if (fmtz == NULL) {
            try_ldra = (sTRY_LDRa*)real->m_loada;
            for (; try_ldra->func != NULL; try_ldra++)
            {
                /* 必须是期望接口类型 */
                if (!(try_ldra->mask & engine->mask))
                    continue;

                /* 文件名必须符合规则 */
                if (try_ldra->match != NULL) {
                    if (!wildcard_matchIA(name_a, try_ldra->match))
                        continue;
                }

                /* 尝试加载目标文件 */
                fmtz_loader = (load_fmtz_t)try_ldra->func;
                fmtz = fmtz_loader(datin, loader);
                if (fmtz != NULL)
                    break;

                /* 复位文件读取指针 */
                if (!CR_VCALL(datin)->rewind(datin))
                    break;
            }
        }
        mem_free(name_a);
    }
    else
    if (loader->name.wide != NULL) {
        if (real->m_findw.__list__ == NULL)
            goto _func_out;
        name_w = str_dupW(loader->name.wide);
        if (name_w == NULL)
            goto _func_out;

        /* 文件名规则匹配 */
        flname_extractW(name_w, loader->name.wide);
        hash = match_findW(str_lwrW(name_w));
        hash %= real->m_findw.__size__;
        node = &real->m_findw.__list__[hash];
        match_w = (sMATCHw*)node->__buff__;
        for (idx = 0; idx < node->__cnts__; idx++, match_w++)
        {
            /* 必须是期望接口类型 */
            if (!(match_w->maskz & engine->mask))
                continue;

            /* 扩展名与文件名必须符合规则 */
            if (!filext_checkW(name_w, match_w->filext))
                continue;
            if (match_w->match != NULL) {
                if (!wildcard_matchIW(name_w, match_w->match))
                    continue;
            }

            /* 尝试加载目标文件 */
            fmtz_loader = (load_fmtz_t)match_w->loader;
            fmtz = fmtz_loader(datin, loader);
            if (fmtz != NULL)
                break;

            /* 复位文件读取指针 */
            if (!CR_VCALL(datin)->rewind(datin)) {
                mem_free(name_w);
                goto _func_out;
            }
        }

        /* 逐个尝试加载匹配 */
        if (fmtz == NULL) {
            try_ldrw = (sTRY_LDRw*)real->m_loadw;
            for (; try_ldrw->func != NULL; try_ldrw++)
            {
                /* 必须是期望接口类型 */
                if (!(try_ldrw->mask & engine->mask))
                    continue;

                /* 文件名必须符合规则 */
                if (try_ldrw->match != NULL) {
                    if (!wildcard_matchIW(name_w, try_ldrw->match))
                        continue;
                }

                /* 尝试加载目标文件 */
                fmtz_loader = (load_fmtz_t)try_ldrw->func;
                fmtz = fmtz_loader(datin, loader);
                if (fmtz != NULL)
                    break;

                /* 复位文件读取指针 */
                if (!CR_VCALL(datin)->rewind(datin))
                    break;
            }
        }
        mem_free(name_w);
    }

_func_out:
    CR_VCALL(datin)->release(datin);
    return (fmtz);
}

/*
=======================================
    释放读取的文件数据
=======================================
*/
CR_API bool_t
fmtz_free (
  __CR_IN__ const sFMTZ*    fmtz
    )
{
    int32u          idx;
    const sFMT_DAT* fmt_dat;
    const sFMT_PIC* fmt_pic;
    const sFMT_RCT* fmt_rct;
    const sFMT_TXT* fmt_txt;
    const sFMT_PRT* fmt_prt;

    switch (fmtz->type)
    {
        /* 释放数据文件 */
        case CR_FMTZ_NOP:
        case CR_FMTZ_DEC:
            fmt_dat = (const sFMT_DAT*)fmtz;
            TRY_FREE(fmt_dat->data);
            break;

        /* 释放图片文件 */
        case CR_FMTZ_PIC:
            fmt_pic = (const sFMT_PIC*)fmtz;
            for (idx = 0; idx < fmt_pic->count; idx++)
                image_del(fmt_pic->frame[idx].pic);
            mem_free(fmt_pic->frame);
            break;

        /* 释放区域文件 */
        case CR_FMTZ_RCT:
            fmt_rct = (const sFMT_RCT*)fmtz;
            TRY_FREE(fmt_rct->pict);
            break;

        /* 释放文本文件 */
        case CR_FMTZ_TXT:
            fmt_txt = (const sFMT_TXT*)fmtz;
            TRY_FREE(fmt_txt->text);
            break;

        /* 释放对象文件 */
        case CR_FMTZ_PRT:
            fmt_prt = (const sFMT_PRT*)fmtz;
            CR_VCALL(fmt_prt->port)->release(fmt_prt->port);
            break;

        default:
            return (FALSE);
    }
    mem_free(fmtz);
    return (TRUE);
}

/*
=======================================
    获取指定图片帧对象
=======================================
*/
CR_API sIMAGE*
fmtz_get_imgo (
  __CR_IN__ const sFMTZ*    fmtz,
  __CR_IN__ int32u          index
    )
{
    const sFMT_PIC* fmt_pic;

    if (fmtz->type != CR_FMTZ_PIC)
        return (NULL);
    fmt_pic = (const sFMT_PIC*)fmtz;
    if (index >= fmt_pic->count)
        return (NULL);
    return (fmt_pic->frame[index].pic);
}

/*
=======================================
    获取读包系统接口
=======================================
*/
CR_API iPACKAGE*
fmtz_get_pack (
  __CR_IN__ const sFMTZ*    fmtz
    )
{
    const sFMT_PRT* fmt_prt;

    if (fmtz->type != CR_FMTZ_PRT)
        return (NULL);
    fmt_prt = (const sFMT_PRT*)fmtz;
    if (str_cmpA(fmt_prt->more, "iPACKAGE") != 0)
        return (NULL);
    return ((iPACKAGE*)fmt_prt->port);
}

/*
=======================================
    获取媒体播放接口
=======================================
*/
CR_API iXMMEDIA*
fmtz_get_xmms (
  __CR_IN__ const sFMTZ*    fmtz
    )
{
    const sFMT_PRT* fmt_prt;

    if (fmtz->type != CR_FMTZ_PRT)
        return (NULL);
    fmt_prt = (const sFMT_PRT*)fmtz;
    if (str_cmpA(fmt_prt->more, "iXMMEDIA") != 0)
        return (NULL);
    return ((iXMMEDIA*)fmt_prt->port);
}

/*
=======================================
    获取多帧图片接口
=======================================
*/
CR_API iPICTURE*
fmtz_get_pict (
  __CR_IN__ const sFMTZ*    fmtz
    )
{
    const sFMT_PRT* fmt_prt;

    if (fmtz->type != CR_FMTZ_PRT)
        return (NULL);
    fmt_prt = (const sFMT_PRT*)fmtz;
    if (str_cmpA(fmt_prt->more, "iPICTURE") != 0)
        return (NULL);
    return ((iPICTURE*)fmt_prt->port);
}

/*****************************************************************************/
/*                               读包功能封装                                */
/*****************************************************************************/

/*
=======================================
    初始化文件查找表
=======================================
*/
CR_API bool_t
pack_init_list (
  __CR_IO__ iPACKAGE*   pack,
  __CR_IN__ bool_t      caseless
    )
{
    leng_t      ii;
    int64u      idx;
    int64u      num;
    leng_t      cnt;
    sARRAY*     nod;
    sFINDER     tmp;
    sFINDER*    fnd;
    sPAK_FILE*  ptr;

    num = pack_file_num(pack);
    if (cut_int64u(&cnt, num))
        return (FALSE);
    if (!curtain_initT(&pack->__search__, sFINDER, cnt, 0))
        return (FALSE);
    pack->__caseless__ = caseless;

    if (pack->__filelst__ == NULL)
        return (TRUE);
    ptr = pack->__filelst__;
    for (idx = 0; idx < num; idx++,
         ptr = (sPAK_FILE*)((byte_t*)ptr + ptr->skip))
    {
        if (ptr->name[0] == 0x00)
            continue;
        if (ptr->find == NULL) {
            ii = str_lenA(ptr->name);
            ptr->find = (ansi_t*)mem_dup(ptr->name, ii + 1);
            if (ptr->find == NULL) {
                curtain_freeT(&pack->__search__, sFINDER);
                return (FALSE);
            }
            if (caseless)
                flname_uniqueA((ansi_t*)ptr->find);
            else
                path_uniqueA((ansi_t*)ptr->find);
        }
        else {
            ii = str_lenA(ptr->find);
        }
        tmp.index = idx;
        tmp.finfo = ptr;
        tmp.crc32 = hash_crc32i_total(ptr->find, ii);
        ii = (leng_t)(tmp.crc32 % pack->__search__.__size__);
        nod = &pack->__search__.__list__[ii];
        fnd = array_get_dataT(nod, sFINDER);
        for (ii = 0; ii < nod->__cnts__; ii++, fnd++) {
            if (tmp.crc32 == fnd->crc32 &&
                str_cmpA(ptr->find, fnd->finfo->find) == 0) {
                struct_cpy(fnd, &tmp, sFINDER);
                break;
            }
        }
        if (ii >= nod->__cnts__) {
            if (array_push_growT(nod, sFINDER, &tmp) == NULL) {
                curtain_freeT(&pack->__search__, sFINDER);
                return (FALSE);
            }
        }
    }
    return (TRUE);
}

/*
=======================================
    释放文件查找表
=======================================
*/
CR_API void_t
pack_free_list (
  __CR_IN__ iPACKAGE*   pack
    )
{
    curtain_freeT(&pack->__search__, sFINDER);
}

/*
=======================================
    文件名到索引的查找U
=======================================
*/
CR_API bool_t
pack_find_fileU (
  __CR_IO__ iPACKAGE*       pack,
  __CR_OT__ int64u*         index,
  __CR_IN__ const ansi_t*   name
    )
{
    leng_t      idx;
    int32u      hash;
    ansi_t*     find;
    sARRAY*     node;
    sFINDER*    unit;

    /* 统一文件目录风格 */
    idx = str_lenA(name);
    find = (ansi_t*)mem_dup(name, idx + 1);
    if (find == NULL)
        return (FALSE);
    if (pack->__caseless__)
        flname_uniqueA(find);
    else
        path_uniqueA(find);

    hash = hash_crc32i_total(find, idx);
    idx = (leng_t)(hash % pack->__search__.__size__);
    node = &pack->__search__.__list__[idx];
    unit = array_get_dataT(node, sFINDER);
    for (idx = 0; idx < node->__cnts__; idx++, unit++) {
        if (hash == unit->crc32 &&
            str_cmpA(find, unit->finfo->find) == 0)
            break;
    }
    mem_free(find);
    if (idx >= node->__cnts__)
        return (FALSE);
    if (index != NULL)
       *index  = unit->index;
    return (TRUE);
}

/*
=======================================
    文件名到索引的查找W
=======================================
*/
CR_API bool_t
pack_find_fileW (
  __CR_IO__ iPACKAGE*       pack,
  __CR_OT__ int64u*         index,
  __CR_IN__ const wide_t*   name
    )
{
    bool_t  rett;
    ansi_t* utf8;

    utf8 = utf16_to_utf8(name);
    if (utf8 == NULL)
        return (FALSE);
    rett = pack_find_fileU(pack, index, utf8);
    mem_free(utf8);
    return (rett);
}

/*
=======================================
    文件名到索引的查找A
=======================================
*/
CR_API bool_t
pack_find_fileA (
  __CR_IO__ iPACKAGE*       pack,
  __CR_OT__ int64u*         index,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          cpage
    )
{
    bool_t  rett;
    ansi_t* utf8;

    utf8 = local_to_utf8(cpage, name);
    if (utf8 == NULL)
        return (FALSE);
    rett = pack_find_fileU(pack, index, utf8);
    mem_free(utf8);
    return (rett);
}

/*
=======================================
    读取文件个数
=======================================
*/
CR_API int64u
pack_file_num (
  __CR_IO__ iPACKAGE*   pack
    )
{
    return (CR_VCALL(pack)->getFileNum(pack));
}

/*
=======================================
    读取文件数据 (根据索引)
=======================================
*/
CR_API bool_t
pack_file_data (
  __CR_IO__ iPACKAGE*   pack,
  __CR_OT__ sBUFFER*    buff,
  __CR_IN__ int64u      index,
  __CR_IN__ bool_t      hash
    )
{
    return (CR_VCALL(pack)->getFileData(pack, buff, index, hash));
}

/*
=======================================
    读取文件信息 (根据索引)
=======================================
*/
CR_API bool_t
pack_file_info (
  __CR_IO__ iPACKAGE*   pack,
  __CR_OT__ sPAK_FILE** info,
  __CR_IN__ int64u      index
    )
{
    return (CR_VCALL(pack)->getFileInfo(pack, info, index));
}

/*
=======================================
    读取文件数据U (根据名称)
=======================================
*/
CR_API bool_t
pack_file_dataU (
  __CR_IO__ iPACKAGE*       pack,
  __CR_OT__ sBUFFER*        buff,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ bool_t          hash
    )
{
    int64u  index;

    if (!pack_find_fileU(pack, &index, name))
        return (FALSE);
    return (pack_file_data(pack, buff, index, hash));
}

/*
=======================================
    读取文件数据W (根据名称)
=======================================
*/
CR_API bool_t
pack_file_dataW (
  __CR_IO__ iPACKAGE*       pack,
  __CR_OT__ sBUFFER*        buff,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ bool_t          hash
    )
{
    int64u  index;

    if (!pack_find_fileW(pack, &index, name))
        return (FALSE);
    return (pack_file_data(pack, buff, index, hash));
}

/*
=======================================
    读取文件数据A (根据名称)
=======================================
*/
CR_API bool_t
pack_file_dataA (
  __CR_IO__ iPACKAGE*       pack,
  __CR_OT__ sBUFFER*        buff,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          cpage,
  __CR_IN__ bool_t          hash
    )
{
    int64u  index;

    if (!pack_find_fileA(pack, &index, name, cpage))
        return (FALSE);
    return (pack_file_data(pack, buff, index, hash));
}

/*
=======================================
    读取文件信息U (根据名称)
=======================================
*/
CR_API bool_t
pack_file_infoU (
  __CR_IO__ iPACKAGE*       pack,
  __CR_OT__ sPAK_FILE**     info,
  __CR_IN__ const ansi_t*   name
    )
{
    int64u  index;

    if (!pack_find_fileU(pack, &index, name))
        return (FALSE);
    return (pack_file_info(pack, info, index));
}

/*
=======================================
    读取文件信息W (根据名称)
=======================================
*/
CR_API bool_t
pack_file_infoW (
  __CR_IO__ iPACKAGE*       pack,
  __CR_OT__ sPAK_FILE**     info,
  __CR_IN__ const wide_t*   name
    )
{
    int64u  index;

    if (!pack_find_fileW(pack, &index, name))
        return (FALSE);
    return (pack_file_info(pack, info, index));
}

/*
=======================================
    读取文件信息A (根据名称)
=======================================
*/
CR_API bool_t
pack_file_infoA (
  __CR_IO__ iPACKAGE*       pack,
  __CR_OT__ sPAK_FILE**     info,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          cpage
    )
{
    int64u  index;

    if (!pack_find_fileA(pack, &index, name, cpage))
        return (FALSE);
    return (pack_file_info(pack, info, index));
}

/*****************************************************************************/
/*                               外部文件加载                                */
/*****************************************************************************/

/*
=======================================
    释放外部文件
=======================================
*/
CR_API void_t
filex_free (
  __CR_IN__ const sEX_FILE* filex
    )
{
    if (!filex->is_free)
        return;
    switch (filex->ex_file.type)
    {
        default:
            break;

        case CR_LDR_ANSI:
            TRY_FREE(filex->ex_file.name.ansi);
            break;

        case CR_LDR_WIDE:
            TRY_FREE(filex->ex_file.name.wide);
            break;

        case CR_LDR_BUFF:
            TRY_FREE(filex->ex_file.buff.data);
            break;
    }
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
