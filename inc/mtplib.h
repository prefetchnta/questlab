/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-16  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 线程进程头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_MTPLIB_H__
#define __CR_MTPLIB_H__

#include "defs.h"

/* 用到的数据类型 */
typedef uint_t      lock_t;     /* 多线程锁类型 */
typedef void_t*     thrd_t;     /* 线程句柄类型 */
typedef void_t*     crsc_t;     /* 临界区的类型 */
typedef void_t*     evts_t;     /* 事件信号类型 */

/*****************************************************************************/
/*                                 线程控制                                  */
/*****************************************************************************/

/* 忽略优先级值 */
#define CR_PRRT_NRM     ((sint_t)-1)

/* 无限等待时间 */
#define CR_INFINITE     ((uint_t)-1)

/* 线程回调函数类型 */
typedef uint_t  (STDCALL *mt_main_t) (void_t*);

/* 线程生成释放 */
CR_API sint_t   thread_prrt_nrm (void_t);
CR_API sint_t   thread_prrt_min (void_t);
CR_API sint_t   thread_prrt_max (void_t);
CR_API thrd_t   thread_new (uint_t stack_size, mt_main_t main,
                            void_t *param, bool_t suspended,
                            sint_t priority CR_DEFAULT(CR_PRRT_NRM),
                            size_t *addition CR_DEFAULT(NULL));
CR_API void_t   thread_del (thrd_t thread);

/* 线程控制操作 */
CR_API bool_t   thread_stop (thrd_t thread);
CR_API bool_t   thread_goon (thrd_t thread);
CR_API void_t   thread_wake (thrd_t thread);
CR_API bool_t   thread_wait (thrd_t thread, uint_t time_ms
                             CR_DEFAULT(CR_INFINITE));
CR_API void_t   thread_sleep (uint_t time_ms CR_DEFAULT(0));

/*****************************************************************************/
/*                                 线程同步                                  */
/*****************************************************************************/

#if !defined(_CR_NO_MT_)

/* 原子算术运算 */
CR_API void_t   atom_inc (sint_t volatile *dst);
CR_API void_t   atom_dec (sint_t volatile *dst);
CR_API void_t   atom_add (dist_t volatile *dst, dist_t src);
CR_API void_t   atom_sub (dist_t volatile *dst, dist_t src);
CR_API void_t   atom_set (sint_t volatile *dst, sint_t val);

/* 临界区相关操作 */
CR_API crsc_t   crisec_new (void_t);
CR_API void_t   crisec_del (crsc_t crisec);
CR_API void_t   crisec_enter (crsc_t crisec);
CR_API void_t   crisec_leave (crsc_t crisec);

/* 多线程锁相关操作 */
CR_API void_t   mtlock_init (lock_t volatile *lock);
CR_API void_t   mtlock_free (lock_t volatile *lock);
CR_API void_t   mtlock_acquire (lock_t volatile *lock);
CR_API void_t   mtlock_release (lock_t volatile *lock);

/* 自旋转锁相关操作 */
CR_API void_t   splock_init (lock_t volatile *lock);
CR_API void_t   splock_free (lock_t volatile *lock);
CR_API void_t   splock_acquire (lock_t volatile *lock);
CR_API void_t   splock_release (lock_t volatile *lock);

/* 事件信号相关操作 */
CR_API evts_t   event_new (void_t);
CR_API void_t   event_del (evts_t event);
CR_API bool_t   event_wait (evts_t event);
CR_API bool_t   event_fire (evts_t event, bool_t broadcast);

#else
#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    普通加一
=======================================
*/
cr_inline void_t
atom_inc (
  __CR_IO__ sint_t* dst
    )
{
    (*dst) += 1;
}

/*
=======================================
    普通减一
=======================================
*/
cr_inline void_t
atom_dec (
  __CR_IO__ sint_t* dst
    )
{
    (*dst) -= 1;
}

/*
=======================================
    普通加法
=======================================
*/
cr_inline void_t
atom_add (
  __CR_IO__ dist_t* dst,
  __CR_IN__ dist_t  src
    )
{
    (*dst) += src;
}

/*
=======================================
    普通减法
=======================================
*/
cr_inline void_t
atom_sub (
  __CR_IO__ dist_t* dst,
  __CR_IN__ dist_t  src
    )
{
    (*dst) -= src;
}

/*
=======================================
    普通设值
=======================================
*/
cr_inline void_t
atom_set (
  __CR_IO__ sint_t* dst,
  __CR_IN__ sint_t  val
    )
{
    (*dst) = val;
}

#endif  /* !_CR_SICK_INLINE_ */

/* 无多线程下的映射宏 */
#define crisec_new() NULL
#define crisec_del(crisec)
#define crisec_enter(crisec)
#define crisec_leave(crisec)
#define mtlock_init(lock)
#define mtlock_free(lock)
#define mtlock_acquire(lock)
#define mtlock_release(lock)
#define splock_init(lock)
#define splock_free(lock)
#define splock_acquire(lock)
#define splock_release(lock)

#endif  /* !_CR_NO_MT_ */

#endif  /* !__CR_MTPLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
