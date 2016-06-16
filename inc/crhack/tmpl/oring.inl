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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 对象环形队列函数库模板 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"

#if defined(CR_NO_STRUCT)
    #define static  CR_API
#else

/* 环形队列数据结构 */
typedef struct
{
        leng_t      head;   /* 环形队列的头 */
        leng_t      tail;   /* 环形队列的尾 */
        leng_t      size;   /* 环形队列大小 */
        ORING_TYPE* data;   /* 环形队列数据 */

} sORING;

#endif  /* CR_NO_STRUCT */

/*
---------------------------------------
    构造函数
---------------------------------------
*/
static bool_t
oring_init (
  __CR_OT__ sORING* that,
  __CR_IN__ leng_t  size
    )
{
    struct_zero(that, sORING);
    that->data = mem_talloc(size + 1, ORING_TYPE);
    if (that->data == NULL)
        return (FALSE);
    that->size = size + 1;
    return (TRUE);
}

/*
---------------------------------------
    清除环形队列
---------------------------------------
*/
static void_t
oring_clear (
  __CR_IO__ sORING* that
    )
{
#if defined(ORING_FREE)
    leng_t  hd, tl;

    hd = that->head;
    tl = that->tail;
    while (hd != tl) {
        ORING_FREE(&that->data[hd++]);
        if (hd >= that->size)
            hd = 0;
    }
#endif
    that->head = 0;
    that->tail = 0;
}

/*
---------------------------------------
    析构函数
---------------------------------------
*/
static void_t
oring_free (
  __CR_IO__ sORING* that
    )
{
    oring_clear(that);
    TRY_FREE(that->data);
    struct_zero(that, sORING);
}

/*
---------------------------------------
    返回环形队列大小
---------------------------------------
*/
static leng_t
oring_get_size (
  __CR_IN__ const sORING*   that
    )
{
    if (that->tail < that->head)
        return (that->size - that->head + that->tail);
    return (that->tail - that->head);
}

/*
---------------------------------------
    环形队列读取数据
---------------------------------------
*/
static bool_t
oring_read (
  __CR_IO__ sORING* that,
  __CR_OT__ void_t* data
    )
{
    if (that->head == that->tail)
        return (FALSE);
    struct_cpy(data, &that->data[that->head++], ORING_TYPE);
    if (that->head >= that->size)
        that->head = 0;
    return (TRUE);
}

/*
---------------------------------------
    环形队列偷窥数据
---------------------------------------
*/
static bool_t
oring_peek (
  __CR_IN__ const sORING*   that,
  __CR_OT__ void_t*         data
    )
{
    if (that->head == that->tail)
        return (FALSE);
    struct_cpy(data, &that->data[that->head], ORING_TYPE);
    return (TRUE);
}

/*
---------------------------------------
    环形队列写入数据
---------------------------------------
*/
static bool_t
oring_write (
  __CR_IO__ sORING*         that,
  __CR_IN__ const void_t*   data,
  __CR_IN__ bool_t          grow
    )
{
    ORING_TYPE* temp;
    /* ----------- */
    leng_t  ii, tl = that->tail + 1;

    if (tl >= that->size)
        tl = 0;
    if (tl == that->head) {
        if (grow) {
            temp = mem_talloc(that->size * 2, ORING_TYPE);
            if (temp == NULL)
                return (FALSE);
            for (tl = 0, ii = that->head; tl < that->size - 1; tl++) {
                struct_cpy(&temp[tl], &that->data[ii++], ORING_TYPE);
                if (ii >= that->size)
                    ii = 0;
            }
            mem_free(that->data);
            that->data = temp;
            that->head = 0;
            that->tail = that->size - 1;
            that->size *= 2;
        }
        else {
            return (FALSE);
        }
    }
    struct_cpy(&that->data[that->tail++], data, ORING_TYPE);
    if (that->tail >= that->size)
        that->tail = 0;
    return (TRUE);
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
