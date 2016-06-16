/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-06-13  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 哈希表函数库模板 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "strlib.h"

/* 默认数据类型 */
#ifndef _CR_H_
    #include "hash.h"
    #define _CR_H_          int32u
    #define HASHTAB_HASH    hash_crc32i_total
#endif

#if defined(CR_NO_STRUCT)
    #define static  CR_API
#else

/* 哈希表结构 */
typedef struct
{
    _CR_T_  list[_CR_N_];
    _CR_H_  hash[_CR_N_];

} sHASHTAB;

#endif  /* CR_NO_STRUCT */

/*
---------------------------------------
    初始化哈希表
---------------------------------------
*/
static void_t
hashtab_init (
  __CR_IO__ sHASHTAB*   htab
    )
{
    struct_zero(htab, sHASHTAB);
}

/*
---------------------------------------
    释放哈希表
---------------------------------------
*/
static void_t
hashtab_free (
  __CR_IO__ sHASHTAB*   htab
    )
{
#if defined(HASHTAB_FREE)
    leng_t  idx;

    for (idx = 0; idx < _CR_N_; idx++) {
        if (htab->list[idx].name != NULL)
            HASHTAB_FREE(&htab->list[idx]);
    }
#endif
    hashtab_init(htab);
}

/*
---------------------------------------
    哈希表查找
---------------------------------------
*/
static leng_t
hashtab_find (
  __CR_IN__ const sHASHTAB* htab,
  __CR_IN__ const ansi_t*   key
    )
{
    _CR_H_  hash;
    leng_t  idx, beg;

    hash = HASHTAB_HASH(key, str_lenA(key));
    beg = (leng_t)(hash % _CR_N_);
    for (idx = 0; idx < _CR_N_; idx++) {
        if (htab->list[beg].name == NULL)
            break;
        if (hash == htab->hash[beg] &&
            str_cmpA(key, htab->list[beg].name) == 0)
            return (beg);
        if (++beg >= _CR_N_)
            beg = 0;
    }
    return (_CR_N_);
}

/*
---------------------------------------
    哈希表查找
---------------------------------------
*/
static _CR_T_*
hashtab_get (
  __CR_IN__ sHASHTAB*       htab,
  __CR_IN__ const ansi_t*   key
    )
{
    leng_t  idx = hashtab_find(htab, key);

    return ((idx == _CR_N_) ? NULL : &htab->list[idx]);
}

/*
---------------------------------------
    哈希表插入
---------------------------------------
*/
static _CR_T_*
hashtab_put (
  __CR_IO__ sHASHTAB*       htab,
  __CR_IN__ const _CR_T_*   unit,
  __CR_IN__ bool_t          replace
    )
{
    _CR_H_  hash;
    leng_t  idx, beg;

    hash = HASHTAB_HASH(unit->name, str_lenA(unit->name));
    beg = (leng_t)(hash % _CR_N_);
    for (idx = 0; idx < _CR_N_; idx++) {
        if (htab->list[beg].name == NULL) {
            htab->hash[beg] = hash;
            struct_cpy(&htab->list[beg], unit, _CR_T_);
            return (&htab->list[beg]);
        }
        if (hash == htab->hash[beg] &&
            str_cmpA(unit->name, htab->list[beg].name) == 0) {
            if (!replace)
                break;
        #if defined(HASHTAB_FREE)
            HASHTAB_FREE(&htab->list[beg]);
        #endif
            htab->hash[beg] = hash;
            struct_cpy(&htab->list[beg], unit, _CR_T_);
            return (&htab->list[beg]);
        }
        if (++beg >= _CR_N_)
            beg = 0;
    }
    return (NULL);
}

/*
---------------------------------------
    哈希表删除
---------------------------------------
*/
static void_t
hashtab_del (
  __CR_IO__ sHASHTAB*       htab,
  __CR_IN__ const ansi_t*   key
    )
{
    leng_t  idx = hashtab_find(htab, key);

    if (idx == _CR_N_)
        return;
#if defined(HASHTAB_FREE)
    HASHTAB_FREE(&htab->list[idx]);
#endif
    htab->hash[idx] = 0;
    struct_zero(&htab->list[idx], _CR_T_);
}

#if defined(CR_NO_STRUCT)
    #undef  static
#endif

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
