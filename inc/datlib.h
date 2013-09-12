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
#define __CR_DATLIB_H__ 0x6A178767UL

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

#define array_get_dataT(that, type) \
 ((type*)array_get_data(that))

#define array_get_sizeT(that, type) \
         array_get_size(that)

#define array_get_unitT(that, type, index) \
 ((type*)array_get_unit(that, sizeof(type), index))

#define array_get_unit_safeT(that, type, index) \
 ((type*)array_get_unit_safe(that, sizeof(type), index))

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
 ((type*)array_pop(that, sizeof(type), data))

#define array_deleteT(that, type, index) \
         array_delete(that, sizeof(type), index)

#define array_insertT(that, type, index, data) \
 ((type*)array_insert(that, sizeof(type), index, data))

#define array_insert_growT(that, type, index, data) \
 ((type*)array_insert_grow(that, sizeof(type), index, data))

/***** 原生函数 *****/
CR_API void_t   array_init (sARRAY *that);
CR_API void_t   array_free (sARRAY *that, leng_t unit);
CR_API void_t   array_clear (sARRAY *that, leng_t unit);
CR_API bool_t   array_no_grow (sARRAY *that, leng_t unit);
CR_API bool_t   array_reserve (sARRAY *that, leng_t unit, leng_t size);
CR_API void_t*  array_push (sARRAY *that, leng_t unit, const void_t *data);
CR_API void_t*  array_push_grow (sARRAY *that, leng_t unit,const void_t *data);
CR_API void_t*  array_top (const sARRAY *that, leng_t unit, void_t *data);
CR_API void_t*  array_pop (sARRAY *that, leng_t unit, void_t *data);
CR_API bool_t   array_delete (sARRAY *that, leng_t unit, leng_t index);
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
inline void_t*
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
inline leng_t
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
inline void_t*
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
inline void_t*
array_get_unit_safe (
  __CR_IN__ const sARRAY*   that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ leng_t          index
    )
{
    if (index >= that->__cnts__) {
        err_set(__CR_DATLIB_H__, index,
                "array_get_unit_safe()", "index: out of bounds");
        return (NULL);
    }
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
        sLST_UNIT*  __node__;   /* 当前的节点 */

        /* 成员释放回调 */
        void_t  (*free) (void_t *obj);
} sLIST;

/***** 泛型映射 *****/
#define list_initT(that, type) \
         list_init(that)

#define list_freeT(that, type) \
         list_free(that)

#define list_swapT(that, type) \
 ((type*)list_swap(that))

#define list_go_headT(that, type) \
 ((type*)list_go_head(that))

#define list_go_tailT(that, type) \
 ((type*)list_go_tail(that))

#define list_go_nextT(that, type) \
 ((type*)list_go_next(that))

#define list_go_prevT(that, type) \
 ((type*)list_go_prev(that))

#define list_get_dataT(node, type) \
 ((type*)list_get_data(node))

#define list_get_sizeT(that, type) \
         list_get_size(that)

#define list_get_nodeT(that, type) \
 ((type*)list_get_node(that))

#define list_get_posT(that, type) \
         list_get_pos(that)

#define list_set_posT(that, type, node) \
         list_set_pos(that, node)

#define list_deleteT(that, type) \
         list_delete(that)

#define list_appendT(that, type, data) \
 ((type*)list_append(that, sizeof(type), data))

#define list_stheadT(that, type, data) \
 ((type*)list_sthead(that, sizeof(type), data))

#define list_insertT(that, type, data, front) \
 ((type*)list_insert(that, sizeof(type), data, front))

/***** 原生函数 *****/
CR_API void_t   list_init (sLIST *that);
CR_API void_t   list_free (sLIST *that);
CR_API void_t*  list_swap (sLIST *that);
CR_API void_t   list_delete (sLIST *that);
CR_API void_t*  list_append (sLIST *that, leng_t unit, const void_t *data);
CR_API void_t*  list_sthead (sLIST *that, leng_t unit, const void_t *data);
CR_API void_t*  list_insert (sLIST *that, leng_t unit, const void_t *data,
                             bool_t front CR_DEFAULT(FALSE));
#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    获取节点数据
=======================================
*/
inline void_t*
list_get_data (
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
inline leng_t
list_get_size (
  __CR_IN__ const sLIST*    that
    )
{
    return (that->__size__);
}

/*
=======================================
    获取当前节点数据
=======================================
*/
inline void_t*
list_get_node (
  __CR_IN__ const sLIST*    that
    )
{
    if (that->__node__ == NULL)
        return (NULL);
    return (&that->__node__[1]);
}

/*
=======================================
    获取当前节点
=======================================
*/
inline sLST_UNIT*
list_get_pos (
  __CR_IN__ const sLIST*    that
    )
{
    return (that->__node__);
}

/*
=======================================
    设置当前节点
=======================================
*/
inline void_t
list_set_pos (
  __CR_IN__ sLIST*      that,
  __CR_IN__ sLST_UNIT*  node
    )
{
    that->__node__ = node;
}

/*
=======================================
    定位到链表头
=======================================
*/
inline void_t*
list_go_head (
  __CR_IO__ sLIST*  that
    )
{
    that->__node__ = that->__head__;
    if (that->__node__ == NULL)
        return (NULL);
    return (&that->__node__[1]);
}

/*
=======================================
    定位到链表尾
=======================================
*/
inline void_t*
list_go_tail (
  __CR_IO__ sLIST*  that
    )
{
    that->__node__ = that->__tail__;
    if (that->__node__ == NULL)
        return (NULL);
    return (&that->__node__[1]);
}

/*
=======================================
    定位到下一个节点
=======================================
*/
inline void_t*
list_go_next (
  __CR_IO__ sLIST*  that
    )
{
    sLST_UNIT*  next;

    next = that->__node__->next;
    if (next == NULL)
        return (NULL);
    that->__node__ = next;
    return ((void_t*)(&next[1]));
}

/*
=======================================
    定位到上一个节点
=======================================
*/
inline void_t*
list_go_prev (
  __CR_IO__ sLIST*  that
    )
{
    sLST_UNIT*  prev;

    prev = that->__node__->prev;
    if (prev == NULL)
        return (NULL);
    that->__node__ = prev;
    return ((void_t*)(&prev[1]));
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

/***** 原生函数 *****/
CR_API bool_t   curbead_init (sCURBEAD *that, leng_t count);
CR_API void_t   curbead_free (sCURBEAD *that);
CR_API void_t*  curbead_find (const sCURBEAD *that, const void_t *key);
CR_API bool_t   curbead_delete (const sCURBEAD *that, const void_t *key);
CR_API void_t*  curbead_insert (const sCURBEAD *that, leng_t unit,
                                const void_t *key, const void_t *data);

/*****************************************************************************/
/*                                树形数据表                                 */
/*****************************************************************************/

/***** 数据结构 *****/
typedef struct  _sATR_UNIT
{
        sARRAY              next;   /* 下一层节点 */
        struct _sATR_UNIT*  prev;   /* 上一个节点 */
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
#define atree_get_rootT(that, type) \
         atree_get_root(that)

#define atree_get_subnT(that, type) \
         atree_get_subn(that)

#define atree_go_nextT(node, type, index) \
         atree_go_next(node, index)

#define atree_go_prevT(node, type) \
         atree_go_prev(node)

#define atree_get_dataT(node, type) \
 ((type*)atree_get_data(node))

#define atree_initT(that, type, count) \
         atree_init(that, sizeof(type), count)

#define atree_freeT(that, type) \
         atree_free(that)

#define atree_deleteT(that, type, node) \
         atree_delete(that, node)

#define atree_appendT(that, type, node, data, index) \
         atree_append(that, sizeof(type), node, data, index)

#define atree_insertT(that, type, node, data, index) \
         atree_insert(that, sizeof(type), node, data, index)

/***** 原生函数 *****/
CR_API bool_t       atree_init (sATREE *that, leng_t unit,
                                leng_t count CR_DEFAULT(0));
CR_API void_t       atree_free (sATREE *that);
CR_API void_t       atree_delete (const sATREE *that, sATR_UNIT *node);
CR_API sATR_UNIT*   atree_append (const sATREE *that, leng_t unit,
                                  sATR_UNIT *node, const void_t *data,
                                  leng_t index);
CR_API sATR_UNIT*   atree_insert (const sATREE *that, leng_t unit,
                                  sATR_UNIT *node, const void_t *data,
                                  leng_t index);
#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    获取节点数据
=======================================
*/
inline void_t*
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
inline sATR_UNIT*
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
inline leng_t
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
inline sATR_UNIT*
atree_go_next (
  __CR_IN__ const sATR_UNIT*    node,
  __CR_IN__ leng_t              index
    )
{
    sATR_UNIT** next;

    if (index >= node->next.__cnts__) {
        err_set(__CR_DATLIB_H__, index,
                "atree_go_next()", "index: out of bounds");
        return (NULL);
    }
    next = array_get_unitT(&node->next, sATR_UNIT*, index);
    return ((sATR_UNIT*)(next[0]));
}

/*
=======================================
    返回上一级节点
=======================================
*/
inline sATR_UNIT*
atree_go_prev (
  __CR_IN__ const sATR_UNIT*    node
    )
{
    return ((sATR_UNIT*)node->prev);
}

#endif  /* !_CR_SICK_INLINE_ */

/*****************************************************************************/
/*                                 环形队列                                  */
/*****************************************************************************/

/* 开启环形队列才能使用 */
#if defined(_CR_USE_BRING_INL_)

/* 默认数据类型 */
#ifndef BRING_TYPE
    #define BRING_NO_COPY
    #define BRING_TYPE      byte_t
#endif
#define BRING_UNIT  sizeof(BRING_TYPE)

#if !defined(BRING_SIZE)

/***** 数据结构 *****/
typedef struct
{
        bool_t      buf_full;       /* 缓冲区是否满 */
        leng_t      head, tail;     /* 队列的头和尾 */
        leng_t          size;       /* 环形队列大小 */
        BRING_TYPE*     data;       /* 环形队列数据 */

} sBRING;

/* 映射到结构里的成员 */
#define BRING_SIZE  (that->size)

/*
=======================================
    构造函数
=======================================
*/
inline bool_t
bring_init (
  __CR_OT__ sBRING* that,
  __CR_IN__ leng_t  size
    )
{
    struct_zero(that, sBRING);
    that->data = mem_talloc(size, BRING_TYPE);
    if (that->data == NULL) {
        err_set(__CR_DATLIB_H__, CR_NULL,
                "bring_init()", "mem_talloc() failure");
        return (FALSE);
    }
    that->size = size;
    return (TRUE);
}

#else

/***** 数据结构 *****/
typedef struct
{
        bool_t      buf_full;           /* 缓冲区是否满 */
        leng_t      head, tail;         /* 队列的头和尾 */
        BRING_TYPE  data[BRING_SIZE];   /* 环形队列数据 */

} sBRING;

/*
=======================================
    构造函数
=======================================
*/
inline void_t
bring_init (
  __CR_OT__ sBRING* that
    )
{
    struct_zero(that, sBRING);
}

#endif  /* !BRING_SIZE */

/*
=======================================
    析构函数
=======================================
*/
inline void_t
bring_free (
  __CR_IO__ sBRING* that
    )
{
#ifndef BRING_SIZE
    TRY_FREE(that->data)
#endif
    struct_zero(that, sBRING);
}

/*
=======================================
    返回环形队列大小
=======================================
*/
inline leng_t
bring_get_size (
  __CR_IN__ const sBRING*   that
    )
{
    if (that->tail == that->head)
        return (that->buf_full ? BRING_SIZE : 0);
    if (that->tail  < that->head)
        return (BRING_SIZE - that->head + that->tail);
    return (that->tail - that->head);
}

/*
=======================================
    环形队列读取数据
=======================================
*/
inline leng_t
bring_read (
  __CR_IO__ sBRING* that,
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    leng_t  total;

    if (size == 0)
        return (0);
    total = bring_get_size(that);
    if (size > total)
        size = total;
    for (total = size; size != 0; size--)
    {
#if defined(BRING_NO_COPY)
        *(BRING_TYPE*)data = that->data[that->head];
#else
        mem_cpy(data, &that->data[that->head], BRING_UNIT);
#endif
        that->head += 1;
        if (that->head >= BRING_SIZE)
            that->head = 0;
        data = (byte_t*)data + BRING_UNIT;
    }
    that->buf_full = FALSE;
    return (total);
}

/*
=======================================
    环形队列写入数据
=======================================
*/
inline leng_t
bring_write (
  __CR_IO__ sBRING*         that,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    leng_t  total;

    if (size == 0)
        return (0);
    for (total = size; size != 0; size--)
    {
#if defined(BRING_NO_COPY)
        that->data[that->tail] = *(BRING_TYPE*)data;
#else
        mem_cpy(&that->data[that->tail], data, BRING_UNIT);
#endif
        that->tail += 1;
        if (that->tail >= BRING_SIZE)
            that->tail = 0;
        if (that->buf_full)
            that->head = that->tail;
        else
        if (that->tail == that->head)
            that->buf_full = TRUE;
        data = (byte_t*)data + BRING_UNIT;
    }
    return (total);
}

#endif  /* _CR_USE_BRING_INL_ */

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

#define pltable_clearT(that, type) \
         pltable_clear(that, sizeof(type))

#define pltable_findT(that, type, key) \
 ((type*)pltable_find(that, sizeof(type), key))

#define pltable_deleteT(that, type, key) \
         pltable_delete(that, sizeof(type), key)

#define pltable_insertT(that, type, key, data) \
 ((type*)pltable_insert(that, sizeof(type), key, data))

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
inline void_t*
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
inline byte_t*
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
inline leng_t
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
inline void_t*
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
inline void_t*
pltable_get_unit_safe (
  __CR_IN__ const sPLTABLE* that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ leng_t          index
    )
{
    if (index >= that->__size__) {
        err_set(__CR_DATLIB_H__, index,
                "pltable_get_unit_safe()", "index: out of bounds");
        return (NULL);
    }
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
