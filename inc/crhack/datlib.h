/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-18  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 数据结构头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DATLIB_H__
#define __CR_DATLIB_H__

#include "memlib.h"

/*****************************************************************************/
/*                                可生长数组                                 */
/*****************************************************************************/

/***** 数据结构 *****/
typedef struct
{
        uchar*  __buff__;   /* 数据指针 */
        leng_t  __cnts__;   /* 成员个数 */
        leng_t  __size__;   /* 实际个数 */

        /* 成员释放回调 */
        void_t  (*free) (void_t *obj);

} sARRAY;

/***** 泛型映射 *****/
#define array_initT(that, type) \
         array_init(that)

#define array_freeT(that, type) \
         array_free(that, sizeof(type))

#define array_clearT(that, type) \
         array_clear(that, sizeof(type))

#define array_no_growT(that, type) \
         array_no_grow(that, sizeof(type))

#define array_reserveT(that, type, size) \
         array_reserve(that, sizeof(type), size)

#define array_pushT(that, type, data) \
 ((type*)array_push(that, sizeof(type), data))

#define array_push_growT(that, type, data) \
 ((type*)array_push_grow(that, sizeof(type), data))

#define array_topT(that, type, data) \
 ((type*)array_top(that, sizeof(type), data))

#define array_popT(that, type, data) \
         array_pop(that, sizeof(type), data)

#define array_deleteT(that, type, index) \
         array_delete(that, sizeof(type), index)

#define array_insertT(that, type, index, data) \
 ((type*)array_insert(that, sizeof(type), index, data))

#define array_insert_growT(that, type, index, data) \
 ((type*)array_insert_grow(that, sizeof(type), index, data))

#define array_get_dataT(that, type) \
 ((type*)array_get_data(that))

#define array_get_sizeT(that, type) \
         array_get_size(that)

#define array_get_unitT(that, type, index) \
 ((type*)array_get_unit(that, sizeof(type), index))

#define array_get_unit_safeT(that, type, index) \
 ((type*)array_get_unit_safe(that, sizeof(type), index))

/***** 原生函数 *****/
CR_API void_t   array_init (sARRAY *that);
CR_API void_t   array_free (sARRAY *that, leng_t unit);
CR_API void_t   array_clear (sARRAY *that, leng_t unit);
CR_API bool_t   array_no_grow (sARRAY *that, leng_t unit);
CR_API bool_t   array_reserve (sARRAY *that, leng_t unit, leng_t size);
CR_API void_t*  array_push (sARRAY *that, leng_t unit, const void_t *data);
CR_API void_t*  array_push_grow (sARRAY *that, leng_t unit,const void_t *data);
CR_API void_t*  array_top (const sARRAY *that, leng_t unit, void_t *data);
CR_API bool_t   array_pop (sARRAY *that, leng_t unit, void_t *data);
CR_API void_t   array_delete (sARRAY *that, leng_t unit, leng_t index);
CR_API void_t*  array_insert (sARRAY *that, leng_t unit, leng_t index,
                              const void_t *data);
CR_API void_t*  array_insert_grow (sARRAY *that, leng_t unit, leng_t index,
                                   const void_t *data);
#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    获取成员总指针
=======================================
*/
cr_inline void_t*
array_get_data (
  __CR_IN__ const sARRAY*   that
    )
{
    return (that->__buff__);
}

/*
=======================================
    获取成员总个数
=======================================
*/
cr_inline leng_t
array_get_size (
  __CR_IN__ const sARRAY*   that
    )
{
    return (that->__cnts__);
}

/*
=======================================
    获取指定成员指针
=======================================
*/
cr_inline void_t*
array_get_unit (
  __CR_IN__ const sARRAY*   that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ leng_t          index
    )
{
    return (that->__buff__ + index * unit);
}

/*
=======================================
    获取指定成员指针 (带安全检查)
=======================================
*/
cr_inline void_t*
array_get_unit_safe (
  __CR_IN__ const sARRAY*   that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ leng_t          index
    )
{
    if (index >= that->__cnts__)
        return (NULL);
    return (that->__buff__ + index * unit);
}

#endif  /* !_CR_SICK_INLINE_ */

/*****************************************************************************/
/*                                窗帘哈希表                                 */
/*****************************************************************************/

/***** 数据结构 *****/
typedef struct
{
        uint_t  __size__;   /* 成员个数 */
        sARRAY* __list__;   /* 表格指针 */

        /* 成员释放回调 */
        void_t  (*free) (void_t *obj);

        /* 成员索引回调 */
        uint_t  (*find) (const void_t *key);

        /* 成员比较回调 */
        bool_t  (*comp) (const void_t *key, const void_t *obj);

} sCURTAIN;

/***** 泛型映射 *****/
#define curtain_initT(that, type, count, leng) \
         curtain_init(that, sizeof(type), count, leng)

#define curtain_freeT(that, type) \
         curtain_free(that, sizeof(type))

#define curtain_findT(that, type, key) \
 ((type*)curtain_find(that, sizeof(type), key))

#define curtain_deleteT(that, type, key) \
         curtain_delete(that, sizeof(type), key)

#define curtain_insertT(that, type, key, data) \
 ((type*)curtain_insert(that, sizeof(type), key, data))

#define curtain_checkT(that, type, size) \
         curtain_check(that, size)

/***** 原生函数 *****/
CR_API bool_t   curtain_init (sCURTAIN *that, leng_t unit,
                              leng_t count, leng_t leng CR_DEFAULT(4));
CR_API void_t   curtain_free (sCURTAIN *that, leng_t unit);
CR_API void_t*  curtain_find (const sCURTAIN *that, leng_t unit,
                              const void_t *key);
CR_API bool_t   curtain_delete (const sCURTAIN *that, leng_t unit,
                                const void_t *key);
CR_API void_t*  curtain_insert (const sCURTAIN *that, leng_t unit,
                                const void_t *key, const void_t *data);
CR_API leng_t*  curtain_check (const sCURTAIN *that, uint_t *size);

/*****************************************************************************/
/*                                 双向链表                                  */
/*****************************************************************************/

/***** 数据结构 *****/
typedef struct  _sLST_UNIT
{
        struct _sLST_UNIT*  prev;   /* 上一个节点 */
        struct _sLST_UNIT*  next;   /* 下一个节点 */
#if 0
        byte_t  udata[unit_size];   /* 用户的数据 */
#endif
} sLST_UNIT;

typedef struct
{
        leng_t      __size__;   /* 节点的个数 */
        sLST_UNIT*  __head__;   /* 首节点指针 */
        sLST_UNIT*  __tail__;   /* 尾节点指针 */

        /* 成员释放回调 */
        void_t  (*free) (void_t *obj);
} sLIST;

/***** 泛型映射 *****/
#define slist_initT(that, type) \
         slist_init(that)

#define slist_freeT(that, type) \
         slist_free(that)

#define slist_swapT(that, type, node) \
         slist_swap(that, node)

#define slist_deleteT(that, type, node) \
         slist_delete(that, node)

#define slist_appendT(that, type, data) \
         slist_append(that, sizeof(type), data)

#define slist_stheadT(that, type, data) \
         slist_sthead(that, sizeof(type), data)

#define slist_insertT(that, type, node, data) \
         slist_insert(that, sizeof(type), node, data)

#define slist_get_dataT(node, type) \
 ((type*)slist_get_data(node))

#define slist_get_sizeT(that, type) \
         slist_get_size(that)

#define slist_get_headT(that, type) \
         slist_get_head(that)

#define slist_get_tailT(that, type) \
         slist_get_tail(that)

#define slist_go_nextT(node, type) \
         slist_go_next(node)

#define slist_go_prevT(node, type) \
         slist_go_prev(node)

#define slist_get_unitT(that, type, index) \
         slist_get_unit(that, index)

#define slist_get_unit_safeT(that, type, index) \
         slist_get_unit_safe(that, index)

/***** 原生函数 *****/
CR_API void_t       slist_init (sLIST *that);
CR_API void_t       slist_free (sLIST *that);
CR_API sLST_UNIT*   slist_swap (sLIST *that, sLST_UNIT *node);
CR_API void_t       slist_delete (sLIST *that, sLST_UNIT *node);
CR_API sLST_UNIT*   slist_append (sLIST *that, leng_t unit,const void_t *data);
CR_API sLST_UNIT*   slist_sthead (sLIST *that, leng_t unit,const void_t *data);
CR_API sLST_UNIT*   slist_insert (sLIST *that, leng_t unit, sLST_UNIT *node,
                                        const void_t *data);
#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    获取节点数据
=======================================
*/
cr_inline void_t*
slist_get_data (
  __CR_IN__ sLST_UNIT*  node
    )
{
    return (&node[1]);
}

/*
=======================================
    获取节点总个数
=======================================
*/
cr_inline leng_t
slist_get_size (
  __CR_IN__ const sLIST*    that
    )
{
    return (that->__size__);
}

/*
=======================================
    获取头节点
=======================================
*/
cr_inline sLST_UNIT*
slist_get_head (
  __CR_IN__ const sLIST*    that
    )
{
    return (that->__head__);
}

/*
=======================================
    获取尾节点
=======================================
*/
cr_inline sLST_UNIT*
slist_get_tail (
  __CR_IN__ const sLIST*    that
    )
{
    return (that->__tail__);
}

/*
=======================================
    返回下一个节点
=======================================
*/
cr_inline sLST_UNIT*
slist_go_next (
  __CR_IN__ const sLST_UNIT*    node
    )
{
    return ((sLST_UNIT*)node->next);
}

/*
=======================================
    返回上一个节点
=======================================
*/
cr_inline sLST_UNIT*
slist_go_prev (
  __CR_IN__ const sLST_UNIT*    node
    )
{
    return ((sLST_UNIT*)node->prev);
}

/*
=======================================
    获取指定的节点
=======================================
*/
cr_inline sLST_UNIT*
slist_get_unit (
  __CR_IN__ const sLIST*    that,
  __CR_IN__ leng_t          index
    )
{
    sLST_UNIT*  node;

    node = that->__head__;
    for (; index != 0; index--)
        node = node->next;
    return (node);
}

/*
=======================================
    获取指定的节点 (带安全检查)
=======================================
*/
cr_inline sLST_UNIT*
slist_get_unit_safe (
  __CR_IN__ const sLIST*    that,
  __CR_IN__ leng_t          index
    )
{
    sLST_UNIT*  node;

    if (index >= that->__size__)
        return (NULL);
    node = that->__head__;
    for (; index != 0; index--)
        node = node->next;
    return (node);
}

#endif  /* !_CR_SICK_INLINE_ */

/*****************************************************************************/
/*                                珠帘哈希表                                 */
/*****************************************************************************/

/***** 数据结构 *****/
typedef struct
{
        uint_t  __size__;   /* 成员个数 */
        sLIST*  __list__;   /* 表格指针 */

        /* 成员释放回调 */
        void_t  (*free) (void_t *obj);

        /* 成员索引回调 */
        uint_t  (*find) (const void_t *key);

        /* 成员比较回调 */
        bool_t  (*comp) (const void_t *key, const void_t *obj);

} sCURBEAD;

/***** 泛型映射 *****/
#define curbead_initT(that, type, count) \
         curbead_init(that, count)

#define curbead_freeT(that, type) \
         curbead_free(that)

#define curbead_findT(that, type, key) \
 ((type*)curbead_find(that, key))

#define curbead_deleteT(that, type, key) \
         curbead_delete(that, key)

#define curbead_insertT(that, type, key, data) \
 ((type*)curbead_insert(that, sizeof(type), key, data))

#define curbead_checkT(that, type, size) \
         curbead_check(that, size)

/***** 原生函数 *****/
CR_API bool_t   curbead_init (sCURBEAD *that, leng_t count);
CR_API void_t   curbead_free (sCURBEAD *that);
CR_API void_t*  curbead_find (const sCURBEAD *that, const void_t *key);
CR_API bool_t   curbead_delete (const sCURBEAD *that, const void_t *key);
CR_API void_t*  curbead_insert (const sCURBEAD *that, leng_t unit,
                                const void_t *key, const void_t *data);
CR_API leng_t*  curbead_check (const sCURBEAD *that, uint_t *size);

/*****************************************************************************/
/*                                树形数据表                                 */
/*****************************************************************************/

/***** 数据结构 *****/
typedef struct  _sATR_UNIT
{
        sARRAY              next;   /* 下一层节点 */
        struct _sATR_UNIT*  prev;   /* 上一个节点 */
        leng_t              npos;   /* 上层索引号 */
#if 0
        byte_t  udata[unit_size];   /* 用户的数据 */
#endif
} sATR_UNIT;

typedef struct
{
        leng_t      __chld__;   /* 子节点个数 */
        sATR_UNIT*  __root__;   /* 根节点指针 */

        /* 成员释放回调 */
        void_t  (*free) (void_t *obj);

} sATREE;

/***** 泛型映射 *****/
#define atree_initT(that, type, count) \
         atree_init(that, sizeof(type), count)

#define atree_freeT(that, type) \
         atree_free(that)

#define atree_deleteT(that, type, node) \
         atree_delete(that, node)

#define atree_appendT(that, type, node, data) \
         atree_append(that, sizeof(type), node, data)

#define atree_insertT(that, type, node, data, index) \
         atree_insert(that, sizeof(type), node, data, index)

#define atree_get_dataT(node, type) \
 ((type*)atree_get_data(node))

#define atree_get_rootT(that, type) \
         atree_get_root(that)

#define atree_get_subnT(node, type) \
         atree_get_subn(node)

#define atree_go_nextT(node, type, index) \
         atree_go_next(node, index)

#define atree_go_prevT(node, type) \
         atree_go_prev(node)

#define atree_sibling_prevT(node, type) \
         atree_sibling_prev(node)

#define atree_sibling_nextT(node, type) \
         atree_sibling_next(node)

/***** 原生函数 *****/
CR_API bool_t       atree_init (sATREE *that, leng_t unit,
                                leng_t count CR_DEFAULT(0));
CR_API void_t       atree_free (sATREE *that);
CR_API void_t       atree_delete (const sATREE *that, sATR_UNIT *node);
CR_API sATR_UNIT*   atree_append (const sATREE *that, leng_t unit,
                                  sATR_UNIT *node, const void_t *data);
CR_API sATR_UNIT*   atree_insert (const sATREE *that, leng_t unit,
                                  sATR_UNIT *node, const void_t *data,
                                  leng_t index);
#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    获取节点数据
=======================================
*/
cr_inline void_t*
atree_get_data (
  __CR_IN__ sATR_UNIT*  node
    )
{
    return (&node[1]);
}

/*
=======================================
    获取根节点指针
=======================================
*/
cr_inline sATR_UNIT*
atree_get_root (
  __CR_IN__ const sATREE*   that
    )
{
    return (that->__root__);
}

/*
=======================================
    获取子节点数量
=======================================
*/
cr_inline leng_t
atree_get_subn (
  __CR_IN__ const sATR_UNIT*    node
    )
{
    return (node->next.__cnts__);
}

/*
=======================================
    返回下一级节点
=======================================
*/
cr_inline sATR_UNIT*
atree_go_next (
  __CR_IN__ const sATR_UNIT*    node,
  __CR_IN__ leng_t              index
    )
{
    sATR_UNIT** next;

    if (index >= node->next.__cnts__)
        return (NULL);
    next = array_get_unitT(&node->next, sATR_UNIT*, index);
    return ((sATR_UNIT*)(next[0]));
}

/*
=======================================
    返回上一级节点
=======================================
*/
cr_inline sATR_UNIT*
atree_go_prev (
  __CR_IN__ const sATR_UNIT*    node
    )
{
    return ((sATR_UNIT*)node->prev);
}

/*
=======================================
    返回同级上一个节点
=======================================
*/
cr_inline sATR_UNIT*
atree_sibling_prev (
  __CR_IN__ const sATR_UNIT*    node
    )
{
    sATR_UNIT** ptr;

    if (node->prev == NULL || node->npos == 0)
        return (NULL);
    ptr = array_get_unitT(&node->prev->next, sATR_UNIT*, node->npos - 1);
    return (ptr[0]);
}

/*
=======================================
    返回同级下一个节点
=======================================
*/
cr_inline sATR_UNIT*
atree_sibling_next (
  __CR_IN__ const sATR_UNIT*    node
    )
{
    sATR_UNIT** ptr;

    if (node->prev == NULL || node->npos + 1 >= node->prev->next.__cnts__)
        return (NULL);
    ptr = array_get_unitT(&node->prev->next, sATR_UNIT*, node->npos + 1);
    return (ptr[0]);
}

#endif  /* !_CR_SICK_INLINE_ */

/*****************************************************************************/
/*                                 环形队列                                  */
/*****************************************************************************/

/***** 数据结构 *****/
typedef struct
{
        bool_t  buf_full;       /* 缓冲区是否满 */
        leng_t  head, tail;     /* 队列的头和尾 */
        leng_t      size;       /* 环形队列大小 */
        byte_t*     data;       /* 环形队列数据 */

} sBRING;

/***** 原生函数 *****/
CR_API bool_t   bring_init (sBRING *that, leng_t size);
CR_API void_t   bring_free (sBRING *that);
CR_API leng_t   bring_get_size (const sBRING *that);
CR_API leng_t   bring_read (sBRING *that, void_t *data, leng_t size);
CR_API leng_t   bring_peek (const sBRING *that, void_t *data, leng_t size);
CR_API leng_t   bring_write (sBRING *that, const void_t *data, leng_t size);

/*****************************************************************************/
/*                                平面哈希表                                 */
/*****************************************************************************/

/***** 数据结构 *****/
typedef struct
{
        uchar*  __buff__;   /* 数据指针 */
        uchar*  __info__;   /* 信息指针 */
        leng_t  __size__;   /* 成员个数 */

        /* 成员释放回调 */
        void_t  (*free) (void_t *obj);

        /* 成员索引回调 */
        uint_t  (*find) (const void_t *key);

        /* 成员比较回调 */
        bool_t  (*comp) (const void_t *key, const void_t *obj);

} sPLTABLE;

/***** 泛型映射 *****/
#define pltable_initT(that, type, count) \
         pltable_init(that, sizeof(type), count)

#define pltable_freeT(that, type) \
         pltable_free(that, sizeof(type))

#define pltable_clearT(that, type) \
         pltable_clear(that, sizeof(type))

#define pltable_findT(that, type, key) \
 ((type*)pltable_find(that, sizeof(type), key))

#define pltable_deleteT(that, type, key) \
         pltable_delete(that, sizeof(type), key)

#define pltable_insertT(that, type, key, data) \
 ((type*)pltable_insert(that, sizeof(type), key, data))

#define pltable_get_dataT(that, type) \
 ((type*)pltable_get_data(that))

#define pltable_get_infoT(that, type) \
         pltable_get_info(that)

#define pltable_get_sizeT(that, type) \
         pltable_get_size(that)

#define pltable_get_unitT(that, type, index) \
 ((type*)pltable_get_unit(that, sizeof(type), index))

#define pltable_get_unit_safeT(that, type, index) \
 ((type*)pltable_get_unit_safe(that, sizeof(type), index))

/***** 原生函数 *****/
CR_API bool_t   pltable_init (sPLTABLE *that, leng_t unit,
                              leng_t count);
CR_API void_t   pltable_free (sPLTABLE *that, leng_t unit);
CR_API void_t   pltable_clear (sPLTABLE *that, leng_t unit);
CR_API void_t*  pltable_find (const sPLTABLE *that, leng_t unit,
                              const void_t *key);
CR_API bool_t   pltable_delete (const sPLTABLE *that, leng_t unit,
                                const void_t *key);
CR_API void_t*  pltable_insert (const sPLTABLE *that, leng_t unit,
                                const void_t *key, const void_t *data);
#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    获取成员总指针
=======================================
*/
cr_inline void_t*
pltable_get_data (
  __CR_IN__ const sPLTABLE* that
    )
{
    return (that->__buff__);
}

/*
=======================================
    获取节点有效指针
=======================================
*/
cr_inline byte_t*
pltable_get_info (
  __CR_IN__ const sPLTABLE* that
    )
{
    return (that->__info__);
}

/*
=======================================
    获取成员总个数
=======================================
*/
cr_inline leng_t
pltable_get_size (
  __CR_IN__ const sPLTABLE* that
    )
{
    return (that->__size__);
}

/*
=======================================
    获取指定成员指针
=======================================
*/
cr_inline void_t*
pltable_get_unit (
  __CR_IN__ const sPLTABLE* that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ leng_t          index
    )
{
    if (!that->__info__[index])
        return (NULL);
    return (that->__buff__ + index * unit);
}

/*
=======================================
    获取指定成员指针 (带安全检查)
=======================================
*/
cr_inline void_t*
pltable_get_unit_safe (
  __CR_IN__ const sPLTABLE* that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ leng_t          index
    )
{
    if (index >= that->__size__)
        return (NULL);
    if (!that->__info__[index])
        return (NULL);
    return (that->__buff__ + index * unit);
}

#endif  /* !_CR_SICK_INLINE_ */

#endif  /* !__CR_DATLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
