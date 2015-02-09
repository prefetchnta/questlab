/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-03-13  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 字节环形队列函数库模板 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"

/* 默认数据类型 */
#ifndef BRING_TYPE
    #define BRING_NO_COPY
    #define BRING_TYPE      byte_t
#endif
#define BRING_UNIT  sizeof(BRING_TYPE)

#if !defined(BRING_SIZE)

#if defined(CR_NO_STRUCT)
    #define static  CR_API
#else

/* 环形队列数据结构 */
typedef struct
{
        bool_t      buf_full;       /* 缓冲区是否满 */
        leng_t      head, tail;     /* 队列的头和尾 */
        leng_t          size;       /* 环形队列大小 */
        BRING_TYPE*     data;       /* 环形队列数据 */

} sBRING;

#endif  /* CR_NO_STRUCT */

/* 映射到结构里的成员 */
#define BRING_SIZE  (that->size)

/*
---------------------------------------
    构造函数
---------------------------------------
*/
static bool_t
bring_init (
  __CR_OT__ sBRING* that,
  __CR_IN__ leng_t  size
    )
{
    struct_zero(that, sBRING);
    that->data = mem_talloc(size, BRING_TYPE);
    if (that->data == NULL)
        return (FALSE);
    that->size = size;
    return (TRUE);
}

/*
---------------------------------------
    析构函数
---------------------------------------
*/
static void_t
bring_free (
  __CR_IO__ sBRING* that
    )
{
    TRY_FREE(that->data);
    struct_zero(that, sBRING);
}

#else

#if defined(CR_NO_STRUCT)
    #define static  CR_API
#else

/* 环形队列数据结构 */
typedef struct
{
        bool_t      buf_full;           /* 缓冲区是否满 */
        leng_t      head, tail;         /* 队列的头和尾 */
        BRING_TYPE  data[BRING_SIZE];   /* 环形队列数据 */

} sBRING;

#endif  /* CR_NO_STRUCT */

/*
---------------------------------------
    构造函数
---------------------------------------
*/
static void_t
bring_init (
  __CR_OT__ sBRING* that
    )
{
    struct_zero(that, sBRING);
}

/*
---------------------------------------
    析构函数
---------------------------------------
*/
static void_t
bring_free (
  __CR_IO__ sBRING* that
    )
{
    struct_zero(that, sBRING);
}

#endif  /* !BRING_SIZE */

/*
---------------------------------------
    返回环形队列大小
---------------------------------------
*/
static leng_t
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
---------------------------------------
    环形队列读取数据
---------------------------------------
*/
static leng_t
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
---------------------------------------
    环形队列偷窥数据
---------------------------------------
*/
static leng_t
bring_peek (
  __CR_IN__ const sBRING*   that,
  __CR_OT__ void_t*         data,
  __CR_IN__ leng_t          size
    )
{
    leng_t  head, total;

    if (size == 0)
        return (0);
    total = bring_get_size(that);
    if (size > total)
        size = total;
    head = that->head;
    for (total = size; size != 0; size--)
    {
#if defined(BRING_NO_COPY)
        *(BRING_TYPE*)data = that->data[head];
#else
        mem_cpy(data, &that->data[head], BRING_UNIT);
#endif
        head += 1;
        if (head >= BRING_SIZE)
            head = 0;
        data = (byte_t*)data + BRING_UNIT;
    }
    return (total);
}

/*
---------------------------------------
    环形队列写入数据
---------------------------------------
*/
static leng_t
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
        if (that->tail == that->head)
            that->buf_full = TRUE;
        data = (byte_t*)data + BRING_UNIT;
    }
    if (that->buf_full)
        that->head = that->tail;
    return (total);
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
