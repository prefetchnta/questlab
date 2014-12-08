/*
***************************************
*   CPU STUFF @ 2014-03-21
***************************************
*/

#ifndef __ATOMIC_HPP__
#define __ATOMIC_HPP__

/* Asylum Namespace */
namespace asy {

/*
***************************************
*       FULL BARRIER
***************************************
*/

/* =============================== */
cr_inline void atom_rw_barrier (void)
{
#if defined(_CR_CC_MSC_)
    _ReadWriteBarrier();
#else
    __sync_synchronize();
#endif
}

/*
***************************************
*       ATOMIC CMP & SWAP (VAL)
***************************************
*/

/* ========================================================================== */
cr_inline short atom_cmp_and_swap16v (short volatile* dst, short set, short cmp)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedCompareExchange16(dst, set, cmp));
#else
    return (__sync_val_compare_and_swap(dst, cmp, set));
#endif
}

/* ================================================================== */
cr_inline int atom_cmp_and_swap32v (int volatile* dst, int set, int cmp)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedCompareExchange((long*)dst, set, cmp));
#else
    return (__sync_val_compare_and_swap(dst, cmp, set));
#endif
}

/* ================================================================================= */
cr_inline ssize_t atom_cmp_and_swap_v (ssize_t volatile* dst, ssize_t set, ssize_t cmp)
{
#if defined(_CR_CC_MSC_)
    #if defined(_CR_SYS32_)
        return (_InterlockedCompareExchange((long*)dst, set, cmp));
    #else
        return (_InterlockedCompareExchange64((__int64*)dst, set, cmp));
    #endif
#else
    return (__sync_val_compare_and_swap(dst, cmp, set));
#endif
}

/*
***************************************
*       ATOMIC CMP & SWAP (BOOL)
***************************************
*/

/* ========================================================================= */
cr_inline bool atom_cmp_and_swap16b (short volatile* dst, short set, short cmp)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedCompareExchange16(dst, set, cmp) == cmp);
#else
    return (__sync_bool_compare_and_swap(dst, cmp, set));
#endif
}

/* =================================================================== */
cr_inline bool atom_cmp_and_swap32b (int volatile* dst, int set, int cmp)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedCompareExchange((long*)dst, set, cmp) == cmp);
#else
    return (__sync_bool_compare_and_swap(dst, cmp, set));
#endif
}

/* ============================================================================== */
cr_inline bool atom_cmp_and_swap_b (ssize_t volatile* dst, ssize_t set, ssize_t cmp)
{
#if defined(_CR_CC_MSC_)
    #if defined(_CR_SYS32_)
        return (_InterlockedCompareExchange((long*)dst, set, cmp) == cmp);
    #else
        return (_InterlockedCompareExchange64((__int64*)dst, set, cmp) == cmp);
    #endif
#else
    return (__sync_bool_compare_and_swap(dst, cmp, set));
#endif
}

/*
***************************************
*       ATOMIC ZERO
***************************************
*/

/* ============================================ */
cr_inline void atom_set_zero32 (int volatile* dst)
{
#if defined(_CR_CC_MSC_)
    _InterlockedExchange((long*)dst, 0);
#else
    __sync_lock_release(dst);
#endif
}

/* ============================================== */
cr_inline void atom_set_zero (ssize_t volatile* dst)
{
#if defined(_CR_CC_MSC_)
    #if defined(_CR_SYS32_)
        _InterlockedExchange((long*)dst, 0);
    #else
        _InterlockedExchange64((__int64*)dst, 0);
    #endif
#else
    __sync_lock_release(dst);
#endif
}

/*
***************************************
*       ATOMIC ADD
***************************************
*/

/* ===================================================== */
cr_inline int atom_fetch_add32 (int volatile* dst, int src)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedExchangeAdd((long*)dst, src));
#else
    return (__sync_fetch_and_add(dst, src));
#endif
}

/* =============================================================== */
cr_inline ssize_t atom_fetch_add (ssize_t volatile* dst, ssize_t src)
{
#if defined(_CR_CC_MSC_)
    #if defined(_CR_SYS32_)
        return (_InterlockedExchangeAdd((long*)dst, src));
    #else
        return (_InterlockedExchangeAdd64((__int64*)dst, src));
    #endif
#else
    return (__sync_fetch_and_add(dst, src));
#endif
}

/*
***************************************
*       ATOMIC SUB
***************************************
*/

/* ===================================================== */
cr_inline int atom_fetch_sub32 (int volatile* dst, int src)
{
#if defined(_CR_CC_MSC_)
    return (atom_fetch_add32(dst, -src));
#else
    return (__sync_fetch_and_sub(dst, src));
#endif
}

/* =============================================================== */
cr_inline ssize_t atom_fetch_sub (ssize_t volatile* dst, ssize_t src)
{
#if defined(_CR_CC_MSC_)
    return (atom_fetch_add(dst, -src));
#else
    return (__sync_fetch_and_sub(dst, src));
#endif
}

/*
***************************************
*       ATOMIC AND
***************************************
*/

/* ======================================================= */
cr_inline char atom_fetch_and8 (char volatile* dst, char src)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedAnd8(dst, src));
#else
    return (__sync_fetch_and_and(dst, src));
#endif
}

/* =========================================================== */
cr_inline short atom_fetch_and16 (short volatile* dst, short src)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedAnd16(dst, src));
#else
    return (__sync_fetch_and_and(dst, src));
#endif
}

/* ===================================================== */
cr_inline int atom_fetch_and32 (int volatile* dst, int src)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedAnd((long*)dst, src));
#else
    return (__sync_fetch_and_and(dst, src));
#endif
}

/* =============================================================== */
cr_inline ssize_t atom_fetch_and (ssize_t volatile* dst, ssize_t src)
{
#if defined(_CR_CC_MSC_)
    #if defined(_CR_SYS32_)
        return (_InterlockedAnd((long*)dst, src));
    #else
        return (_InterlockedAnd64((__int64*)dst, src));
    #endif
#else
    return (__sync_fetch_and_and(dst, src));
#endif
}

/*
***************************************
*       ATOMIC OR
***************************************
*/

/* ====================================================== */
cr_inline char atom_fetch_or8 (char volatile* dst, char src)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedOr8(dst, src));
#else
    return (__sync_fetch_and_or(dst, src));
#endif
}

/* ========================================================== */
cr_inline short atom_fetch_or16 (short volatile* dst, short src)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedOr16(dst, src));
#else
    return (__sync_fetch_and_or(dst, src));
#endif
}

/* ==================================================== */
cr_inline int atom_fetch_or32 (int volatile* dst, int src)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedOr((long*)dst, src));
#else
    return (__sync_fetch_and_or(dst, src));
#endif
}

/* ============================================================== */
cr_inline ssize_t atom_fetch_or (ssize_t volatile* dst, ssize_t src)
{
#if defined(_CR_CC_MSC_)
    #if defined(_CR_SYS32_)
        return (_InterlockedOr((long*)dst, src));
    #else
        return (_InterlockedOr64((__int64*)dst, src));
    #endif
#else
    return (__sync_fetch_and_or(dst, src));
#endif
}

/*
***************************************
*       ATOMIC XOR
***************************************
*/

/* ======================================================= */
cr_inline char atom_fetch_xor8 (char volatile* dst, char src)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedXor8(dst, src));
#else
    return (__sync_fetch_and_xor(dst, src));
#endif
}

/* =========================================================== */
cr_inline short atom_fetch_xor16 (short volatile* dst, short src)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedXor16(dst, src));
#else
    return (__sync_fetch_and_xor(dst, src));
#endif
}

/* ===================================================== */
cr_inline int atom_fetch_xor32 (int volatile* dst, int src)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedXor((long*)dst, src));
#else
    return (__sync_fetch_and_xor(dst, src));
#endif
}

/* =============================================================== */
cr_inline ssize_t atom_fetch_xor (ssize_t volatile* dst, ssize_t src)
{
#if defined(_CR_CC_MSC_)
    #if defined(_CR_SYS32_)
        return (_InterlockedXor((long*)dst, src));
    #else
        return (_InterlockedXor64((__int64*)dst, src));
    #endif
#else
    return (__sync_fetch_and_xor(dst, src));
#endif
}

/*
***************************************
*       ATOMIC DEC
***************************************
*/

/* ================================================ */
cr_inline short atom_dec_fetch16 (short volatile* dst)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedDecrement16(dst));
#else
    return (__sync_sub_and_fetch(dst, 1));
#endif
}

/* ============================================ */
cr_inline int atom_dec_fetch32 (int volatile* dst)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedDecrement((long*)dst));
#else
    return (__sync_sub_and_fetch(dst, 1));
#endif
}

/* ================================================== */
cr_inline ssize_t atom_dec_fetch (ssize_t volatile* dst)
{
#if defined(_CR_CC_MSC_)
    #if defined(_CR_SYS32_)
        return (_InterlockedDecrement((long*)dst));
    #else
        return (_InterlockedDecrement64((__int64*)dst));
    #endif
#else
    return (__sync_sub_and_fetch(dst, 1));
#endif
}

/*
***************************************
*       ATOMIC INC
***************************************
*/

/* ================================================ */
cr_inline short atom_inc_fetch16 (short volatile* dst)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedIncrement16(dst));
#else
    return (__sync_add_and_fetch(dst, 1));
#endif
}

/* ============================================ */
cr_inline int atom_inc_fetch32 (int volatile* dst)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedIncrement((long*)dst));
#else
    return (__sync_add_and_fetch(dst, 1));
#endif
}

/* ================================================== */
cr_inline ssize_t atom_inc_fetch (ssize_t volatile* dst)
{
#if defined(_CR_CC_MSC_)
    #if defined(_CR_SYS32_)
        return (_InterlockedIncrement((long*)dst));
    #else
        return (_InterlockedIncrement64((__int64*)dst));
    #endif
#else
    return (__sync_add_and_fetch(dst, 1));
#endif
}

/*
***************************************
*       ATOMIC EXCHANGE
***************************************
*/

/* ==================================================== */
cr_inline int atom_exchange32 (int volatile* dst, int src)
{
#if defined(_CR_CC_MSC_)
    return (_InterlockedExchange((long*)dst, src));
#else
    return (__sync_lock_test_and_set(dst, src));
#endif
}

/* ============================================================== */
cr_inline ssize_t atom_exchange (ssize_t volatile* dst, ssize_t src)
{
#if defined(_CR_CC_MSC_)
    #if defined(_CR_SYS32_)
        return (_InterlockedExchange((long*)dst, src));
    #else
        return (_InterlockedExchange64((__int64*)dst, src));
    #endif
#else
    return (__sync_lock_test_and_set(dst, src));
#endif
}

}   /* namespace */

#endif  /* __ATOMIC_HPP__ */
