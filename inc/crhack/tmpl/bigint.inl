/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-03-14  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 大整数运算函数库模板 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "msclib.h"

/* 定义整数的长度 */
#ifndef BI_MAXLEN
    #define BI_MAXLEN   (1024 / 32)
#else
    #if (BI_MAXLEN < 4)
        #undef  BI_MAXLEN
        #define BI_MAXLEN   4
    #endif
#endif

#if defined(CR_NO_STRUCT)
    #define static  CR_API
#else

/* 大整数数据结构 */
typedef struct
{
        sint_t  len;
        int32u  val[BI_MAXLEN];

} sBIGINT;

#endif  /* CR_NO_STRUCT */

/*
---------------------------------------
    大整数调整
---------------------------------------
*/
static void_t
bigint_adj (
  __CR_IO__ sBIGINT*    bi
    )
{
    sint_t  idx;

    for (idx = bi->len - 1; idx > 0; idx--) {
        if (bi->val[idx] != 0UL)
            break;
    }
    bi->len = idx + 1;
}

/*
---------------------------------------
    大整数清零
---------------------------------------
*/
static void_t
bigint_clr (
  __CR_OT__ sBIGINT*    bi
    )
{
    bi->len = 1;
    mem_zero(bi->val, BI_MAXLEN * sizeof(int32u));
}

/*
---------------------------------------
    大整数复制
---------------------------------------
*/
static void_t
bigint_cpy (
  __CR_OT__ sBIGINT*        biA,
  __CR_IN__ const sBIGINT*  biB
    )
{
    struct_cpy(biA, biB, sBIGINT);
}

/*
---------------------------------------
    大整数赋值
---------------------------------------
*/
static void_t
bigint_set (
  __CR_OT__ sBIGINT*    bi,
  __CR_IN__ int64u      val
    )
{
    sint_t  idx;

    if (val > 0xFFFFFFFFUL) {
        bi->len = idx = 2;
        bi->val[0] = (int32u)(val);
        bi->val[1] = (int32u)(val >> 32);
    }
    else {
        bi->len = idx = 1;
        bi->val[0] = (int32u)(val);
    }
    for (; idx < BI_MAXLEN; idx++)
        bi->val[idx] = 0UL;
}

/*
---------------------------------------
    大整数比较
---------------------------------------
*/
static sint_t
bigint_cmp (
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ const sBIGINT*  biB
    )
{
    sint_t  idx;

    if (biA->len > biB->len)
        return ( 1);
    if (biA->len < biB->len)
        return (-1);
    for (idx = biA->len - 1; idx >= 0; idx--) {
        if (biA->val[idx] > biB->val[idx])
            return ( 1);
        if (biA->val[idx] < biB->val[idx])
            return (-1);
    }
    return (0);
}

/*
---------------------------------------
    大整数加法I
---------------------------------------
*/
static void_t
bigint_addI (
  __CR_OT__ sBIGINT*        biX,
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ int32u          val
    )
{
    sBIGINT X;
    /* ---- */
    int64u  sum;
    sint_t  idx;

    bigint_cpy(&X, biA);
    sum = X.val[0];
    sum += val;
    X.val[0] = (int32u)sum;
    if (sum > 0xFFFFFFFFUL) {
        idx = 1;
        while (X.val[idx] == 0xFFFFFFFFUL) {
            X.val[idx++] = 0UL;
            if (idx >= BI_MAXLEN)
                break;
        }
        if (idx < BI_MAXLEN) {
            X.val[idx]++;
            if (X.len == idx)
                X.len++;
        }
    }
    bigint_cpy(biX, &X);
}

/*
---------------------------------------
    大整数加法
---------------------------------------
*/
static void_t
bigint_add (
  __CR_OT__ sBIGINT*        biX,
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ const sBIGINT*  biB
    )
{
    sBIGINT X;
    /* ---- */
    int64u  sum;
    sint_t  idx;
    uint_t  carry = 0;

    bigint_cpy(&X, biA);
    if (X.len < biB->len)
        X.len = biB->len;
    for (idx = 0; idx < X.len; idx++) {
        sum = biB->val[idx];
        sum = sum + X.val[idx] + carry;
        X.val[idx] = (int32u)sum;
        carry = (uint_t)(sum >> 32);
    }
    if (carry) {
        if (X.len < BI_MAXLEN)
            X.val[X.len++] = carry;
    }
    bigint_cpy(biX, &X);
}

/*
---------------------------------------
    大整数减法I
---------------------------------------
*/
static void_t
bigint_subI (
  __CR_OT__ sBIGINT*        biX,
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ int32u          val
    )
{
    sBIGINT X;
    /* ---- */
    int64u  num;
    sint_t  idx;

    if (biA->val[0] >= val) {
        if (biX != (sBIGINT*)biA)
            bigint_cpy(biX, biA);
        biX->val[0] -= val;
        return;
    }
    if (biA->len == 1) {
        bigint_clr(biX);
        return;
    }
    bigint_cpy(&X, biA);
    num = CR_ULL(0x100000000) + X.val[0];
    X.val[0] = (int32u)(num - val);
    idx = 1;
    while (X.val[idx] == 0UL) {
        X.val[idx++] = 0xFFFFFFFFUL;
        if (idx >= BI_MAXLEN)
            break;
    }
    if (idx < BI_MAXLEN) {
        X.val[idx]--;
        if (X.val[idx] == 0UL)
            X.len--;
    }
    bigint_cpy(biX, &X);
}

/*
---------------------------------------
    大整数减法
---------------------------------------
*/
static void_t
bigint_sub (
  __CR_OT__ sBIGINT*        biX,
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ const sBIGINT*  biB
    )
{
    sBIGINT X;
    /* ---- */
    int64u  num;
    sint_t  idx;
    uint_t  carry;

    if (bigint_cmp(biA, biB) <= 0) {
        bigint_clr(biX);
        return;
    }
    carry = 0;
    bigint_cpy(&X, biA);
    for (idx = 0; idx < biA->len; idx++) {
        if ((biA->val[idx] > biB->val[idx]) ||
            ((biA->val[idx] == biB->val[idx]) && (carry == 0))) {
            X.val[idx] = biA->val[idx] - carry - biB->val[idx];
            carry = 0;
        }
        else {
            num = CR_ULL(0x100000000) + biA->val[idx];
            X.val[idx] = (int32u)(num - carry - biB->val[idx]);
            carry = 1;
        }
    }
    while (X.val[X.len - 1] == 0UL)
        X.len--;
    bigint_cpy(biX, &X);
}

/*
---------------------------------------
    大整数乘法I
---------------------------------------
*/
static void_t
bigint_mulI (
  __CR_OT__ sBIGINT*        biX,
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ int32u          val
    )
{
    sBIGINT X;
    /* ---- */
    int64u  mul;
    sint_t  idx;
    int32u  carry = 0UL;

    bigint_cpy(&X, biA);
    for (idx = 0; idx < biA->len; idx++) {
        mul = biA->val[idx];
        mul = mul * val + carry;
        X.val[idx] = (int32u)mul;
        carry = (int32u)(mul >> 32);
    }
    if (carry) {
        if (X.len < BI_MAXLEN)
            X.val[X.len++] = carry;
    }
    bigint_cpy(biX, &X);
}

/*
---------------------------------------
    大整数乘法
---------------------------------------
*/
static void_t
bigint_mul (
  __CR_OT__ sBIGINT*        biX,
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ const sBIGINT*  biB
    )
{
    sBIGINT X;
    /* ---- */
    int64u  carry;
    sint_t  ii, jj;
    int64u  sum, mul;

    if (biB->len == 1) {
        bigint_mulI(biX, biA, biB->val[0]);
        return;
    }
    carry = 0;
    bigint_clr(&X);
    X.len = biA->len + biB->len - 1;
    for (ii = 0; ii < X.len; ii++) {
        sum = carry;
        carry = 0;
        for (jj = 0; jj < biB->len; jj++) {
            if (((ii - jj) >= 0) && ((ii - jj) < biA->len)) {
                mul = biA->val[ii - jj];
                mul *= biB->val[jj];
                carry += mul >> 32;
                mul &= 0xFFFFFFFFUL;
                sum += mul;
            }
        }
        carry += sum >> 32;
        X.val[ii] = (int32u)sum;
    }
    if (carry) {
        if (X.len < BI_MAXLEN)
            X.val[X.len++] = (int32u)carry;
    }
    bigint_cpy(biX, &X);
}

/*
---------------------------------------
    大整数除法I
---------------------------------------
*/
static void_t
bigint_divI (
  __CR_OT__ sBIGINT*        biX,
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ int32u          val
    )
{
    sBIGINT X;
    /* ---- */
    int64u  div;
    int64u  mul;
    sint_t  idx;
    int32u  carry;

    if (val == 0UL)
        return;
    if (biA->len == 1) {
        if (biX != (sBIGINT*)biA)
            bigint_cpy(biX, biA);
        biX->val[0] /= val;
        return;
    }
    carry = 0UL;
    bigint_cpy(&X, biA);
    for (idx = X.len - 1; idx >= 0; idx--) {
        div = carry;
        div = (div << 32) + X.val[idx];
        X.val[idx] = (int32u)(div / val);
        mul = (div / val) * val;
        carry = (int32u)(div - mul);
    }
    if (X.val[X.len - 1] == 0UL)
        X.len--;
    bigint_cpy(biX, &X);
}

/*
---------------------------------------
    大整数除法
---------------------------------------
*/
static void_t
bigint_div (
  __CR_OT__ sBIGINT*        biX,
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ const sBIGINT*  biB
    )
{
    sint_t  idx, len;
    int64u  num, div;
    sBIGINT X, Y, Z, T;

    if (biB->len == 1) {
        bigint_divI(biX, biA, biB->val[0]);
        return;
    }
    bigint_clr(&X);
    bigint_cpy(&Y, biA);
    while (bigint_cmp(&Y, biB) >= 0) {
        div = Y.val[Y.len - 1];
        num = biB->val[biB->len - 1];
        len = Y.len - biB->len;
        if ((div == num) && (len == 0)) {
            bigint_addI(&X, &X, 1);
            break;
        }
        if ((div <= num) && (len != 0)) {
            len--;
            div = (div << 32) + Y.val[Y.len - 2];
        }
        div = div / (num + 1);
        bigint_set(&Z, div);
        if (len != 0) {
            Z.len += len;
            for (idx = Z.len - 1; idx >= len; idx--)
                Z.val[idx] = Z.val[idx - len];
            for (idx = 0; idx < len; idx++)
                Z.val[idx] = 0UL;
        }
        bigint_add(&X, &X, &Z);
        bigint_mul(&T, biB, &Z);
        bigint_sub(&Y, &Y, &T);
    }
    bigint_cpy(biX, &X);
}

/*
---------------------------------------
    大整数取模I
---------------------------------------
*/
static int32u
bigint_modI (
  __CR_IN__ const sBIGINT*  bi,
  __CR_IN__ int32u          val
    )
{
    int64u  div;
    sint_t  idx;
    int32u  carry = 0UL;

    if (val == 0UL)
        return (val);
    if (bi->len == 1)
        return (bi->val[0] % val);
    for (idx = bi->len - 1; idx >= 0; idx--) {
        div = bi->val[idx];
        div += carry * CR_ULL(0x100000000);
        carry = (int32u)(div % val);
    }
    return (carry);
}

/*
---------------------------------------
    大整数取模
---------------------------------------
*/
static void_t
bigint_mod (
  __CR_OT__ sBIGINT*        biX,
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ const sBIGINT*  biB
    )
{
    sBIGINT X, Y;
    /* ------- */
    int64u  div, num;
    sint_t  idx, len;

    bigint_cpy(&X, biA);
    while (bigint_cmp(&X, biB) >= 0) {
        div = X.val[X.len - 1];
        num = biB->val[biB->len - 1];
        len = X.len - biB->len;
        if ((div == num) && (len == 0)) {
            bigint_sub(&X, &X, biB);
            break;
        }
        if ((div <= num) && (len != 0)) {
            len--;
            div = (div << 32) + X.val[X.len - 2];
        }
        div = div / (num + 1);
        bigint_set(&Y, div);
        bigint_mul(&Y, biB, &Y);
        if (len != 0) {
            Y.len += len;
            for (idx = Y.len - 1; idx >= len; idx--)
                Y.val[idx] = Y.val[idx - len];
            for (idx = 0; idx < len; idx++)
                Y.val[idx] = 0UL;
        }
        bigint_sub(&X, &X, &Y);
    }
    bigint_cpy(biX, &X);
}

/*
---------------------------------------
    求方程 ax - by = 1 最小整数解
---------------------------------------
*/
static void_t
bigint_euc (
  __CR_OT__ sBIGINT*        biX,
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ const sBIGINT*  biB
    )
{
    sint_t  xx = 1, yy = 1;
    sBIGINT M, E, X, Y, I, J;

    bigint_cpy(&M, biB);
    bigint_cpy(&E, biA);
    bigint_set(&X, 0);
    bigint_set(&Y, 1);
    while ((E.len != 1) || (E.val[0] != 0UL)) {
        bigint_div(&I, &M, &E);
        bigint_mod(&J, &M, &E);
        bigint_cpy(&M, &E);
        bigint_cpy(&E, &J);
        bigint_cpy(&J, &Y);
        bigint_mul(&Y, &Y, &I);
        if (xx == yy) {
            if (bigint_cmp(&X, &Y) >= 0) {
                bigint_sub(&Y, &X, &Y);
            }
            else {
                bigint_sub(&Y, &Y, &X);
                yy = 0;
            }
        }
        else {
            bigint_add(&Y, &X, &Y);
            xx = 1 - xx;
            yy = 1 - yy;
        }
        bigint_cpy(&X, &J);
    }
    if (xx == 0)
        bigint_sub(&X, biB, &X);
    bigint_cpy(biX, &X);
}

/*
---------------------------------------
    大整数乘方的模
---------------------------------------
*/
static void_t
bigint_rsa (
  __CR_OT__ sBIGINT*        biX,
  __CR_IN__ const sBIGINT*  biA,
  __CR_IN__ const sBIGINT*  biB,
  __CR_IN__ const sBIGINT*  biC
    )
{
    int32u  num;
    sint_t  ii, jj, kk, nn;
    /* ----------------- */
    sBIGINT     X, Y, T;

    kk = biB->len * 32 - 32;
    num = biB->val[biB->len - 1];
    while (num != 0UL) {
        num >>= 1;
        kk++;
    }
    bigint_cpy(&X, biA);
    for (ii = kk - 2; ii >= 0; ii--) {
        bigint_mulI(&Y, &X, X.val[X.len - 1]);
        bigint_mod(&Y, &Y, biC);
        for (nn = 1; nn < X.len; nn++) {
            for (jj = Y.len; jj > 0; jj--)
                Y.val[jj] = Y.val[jj - 1];
            Y.val[0] = 0UL;
            Y.len++;
            bigint_mulI(&T, &X, X.val[X.len - nn - 1]);
            bigint_add(&Y, &Y, &T);
            bigint_mod(&Y, &Y, biC);
        }
        bigint_cpy(&X, &Y);
        if ((biB->val[ii >> 5] >> (ii & 31)) & 1) {
            bigint_mulI(&Y, biA, X.val[X.len - 1]);
            bigint_mod(&Y, &Y, biC);
            for (nn = 1; nn < X.len; nn++) {
                for (jj = Y.len; jj > 0; jj--)
                    Y.val[jj] = Y.val[jj - 1];
                Y.val[0] = 0UL;
                Y.len++;
                bigint_mulI(&T, biA, X.val[X.len - nn - 1]);
                bigint_add(&Y, &Y, &T);
                bigint_mod(&Y, &Y, biC);
            }
            bigint_cpy(&X, &Y);
        }
    }
    bigint_cpy(biX, &X);
}

/*
---------------------------------------
    拉宾米勒测试素数
---------------------------------------
*/
static bool_t
bigint_rab (
  __CR_IN__ const sBIGINT*  bi
    )
{
    int64u  rnd;
    sBIGINT M, A, I, K;
    /* ------------- */
    sint_t  ii, jj, pass, *tbl;

    pass = prime_tbl_num();
    tbl = (sint_t*)prime_tbl_ptr();
    for (ii = 0; ii < pass; ii++) {
        if (bigint_modI(bi, tbl[ii]) == 0)
            return (FALSE);
    }
    bigint_clr(&I);
    bigint_cpy(&K, bi);
    K.val[0]--;
    for (ii = 0; ii < 5; ii++) {
        pass = 0;
        rnd  = (int64u)rand_get();
        rnd *= (int64u)rand_get();
        bigint_set(&A, rnd);
        bigint_cpy(&M, &K);
        while ((M.val[0] & 1) == 0) {
            for (jj = 0; jj < M.len; jj++) {
                M.val[jj] >>= 1;
                if (M.val[jj + 1] & 1)
                    M.val[jj] |= 0x80000000UL;
            }
            if (M.val[M.len - 1] == 0UL)
                M.len--;
            bigint_rsa(&I, &A, &M, bi);
            if (bigint_cmp(&I, &K) == 0) {
                pass = 1;
                break;
            }
        }
        if ((I.len == 1) && (I.val[0] == 1UL))
            pass = 1;
        if (pass == 0)
            return (FALSE);
    }
    return (TRUE);
}

/*
---------------------------------------
    产生随机素数 (bits 位)
---------------------------------------
*/
static void_t
bigint_prime (
  __CR_OT__ sBIGINT*    bi,
  __CR_IN__ sint_t      bits
    )
{
    int64u  rnd;
    sint_t  idx, num, *tbl;
    /* ----------------- */
    sBIGINT     S, A, I, K;

    bits /= 32;
    if (bits > BI_MAXLEN)
        return;
    bigint_clr(bi);
    bi->len = bits;
    num = prime_tbl_num();
    tbl = (sint_t*)prime_tbl_ptr();
_begin:
    for (idx = 0; idx < bi->len; idx++)
        bi->val[idx] = rand_get() * 0x10000UL + rand_get();
    bi->val[0] |= 1;
    for (idx = bi->len - 1; idx > 0; idx--) {
        bi->val[idx] <<= 1;
        if (bi->val[idx - 1] & 0x80000000UL)
            bi->val[idx]++;
    }
    bi->val[0] <<= 1;
    bi->val[0]++;
    for (idx = 0; idx < num; idx++) {
        if (bigint_modI(bi, tbl[idx]) == 0)
            goto _begin;
    }
    bigint_cpy(&K, bi);
    K.val[0]--;
    for (idx = 0; idx < 5; idx++) {
        rnd  = (int64u)rand_get();
        rnd *= (int64u)rand_get();
        bigint_set(&A, rnd);
        bigint_divI(&S, &K, 2);
        bigint_rsa(&I, &A, &S, bi);
        if (((I.len != 1) || (I.val[0] != 1)) &&
            (bigint_cmp(&I, &K) != 0))
            goto _begin;
    }
}

/*
---------------------------------------
    大整数位数
---------------------------------------
*/
static uint_t
bigint_bits (
  __CR_IN__ const sBIGINT*  bi
    )
{
    int32u  last;
    uint_t  bits;

    last = bi->val[bi->len - 1];
    bits = (bi->len - 1) * bitsof(int32u);
    last = fill2top32(last);
    return (bits + count_bits32(last));
}

/*
---------------------------------------
    大整数转换到
---------------------------------------
*/
static leng_t
bigint_to (
  __CR_IN__ const sBIGINT*  bi,
  __CR_OT__ void_t*         data,
  __CR_IN__ leng_t          size,
  __CR_IN__ uint_t          type
    )
{
    sBIGINT         X;
    /* ------------ */
    ansi_t          tmp;
    int32u          cha;
    leng_t          num;
    leng_t          idx;
    const ansi_t*   str;

    mem_zero(data, size);

    /* 二进制型 */
    if (type != 2 && type != 8 && type != 16 && type != 10) {
        if (size < bi->len * sizeof(int32u))
            return (0);
        if (size > bi->len * sizeof(int32u))
            size = bi->len * sizeof(int32u);
        for (idx = 0; idx < size; idx++) {
            *(byte_t*)data = (byte_t)((bi->val[idx >> 2]) >> ((idx & 3) * 8));
            data = (byte_t*)data + 1;
        }
        return (size);
    }

    /* 字符串型 */
    if ((bi->len == 1) && (bi->val[0] == 0UL)) {
        if (size <= 1)
            return (0);
        ((ansi_t*)data)[0] = '0';
        return (2);
    }
    idx = 0;
    bigint_cpy(&X, bi);
    str = "0123456789ABCDEF";
    while (X.val[X.len - 1] != 0UL) {
        if (idx >= size)
            return (0);
        cha = bigint_modI(&X, type) & 15;
        ((ansi_t*)data)[idx++] = str[cha];
        bigint_divI(&X, &X, type);
    }

    /* 字符颠倒 */
    if (idx >= size)
        return (0);
    for (num = idx, idx = 0; idx < num / 2; idx++) {
        tmp = ((ansi_t*)data)[idx];
        ((ansi_t*)data)[idx] = ((ansi_t*)data)[num - idx - 1];
        ((ansi_t*)data)[num - idx - 1] = tmp;
    }
    return (num + 1);
}

/*
---------------------------------------
    转换到大整数
---------------------------------------
*/
static void_t
bigint_from (
  __CR_OT__ sBIGINT*        bi,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ uint_t          type
    )
{
    int32u  cha;
    leng_t  idx;

    bigint_clr(bi);
    switch (type)
    {
        case 16:
            size = str_lenA((ansi_t*)data);
            for (idx = 0; idx < size; idx++) {
                bigint_mulI(bi, bi, 16);
                cha = *(byte_t*)data;
                if ((cha >= '0') && (cha <= '9'))
                    cha -= 48;
                else
                if ((cha >= 'A') && (cha <= 'F'))
                    cha -= 55;
                else
                if ((cha >= 'a') && (cha <= 'f'))
                    cha -= 87;
                else
                    break;
                data = (byte_t*)data + 1;
                bigint_addI(bi, bi, cha);
            }
            break;

        case 10:
            size = str_lenA((ansi_t*)data);
            for (idx = 0; idx < size; idx++) {
                bigint_mulI(bi, bi, 10);
                cha = *(byte_t*)data;
                if ((cha >= '0') && (cha <= '9'))
                    cha -= 48;
                else
                    break;
                data = (byte_t*)data + 1;
                bigint_addI(bi, bi, cha);
            }
            break;

        case 8:
            size = str_lenA((ansi_t*)data);
            for (idx = 0; idx < size; idx++) {
                bigint_mulI(bi, bi, 8);
                cha = *(byte_t*)data;
                if ((cha >= '0') && (cha <= '7'))
                    cha -= 48;
                else
                    break;
                data = (byte_t*)data + 1;
                bigint_addI(bi, bi, cha);
            }
            break;

        case 2:
            size = str_lenA((ansi_t*)data);
            for (idx = 0; idx < size; idx++) {
                bigint_mulI(bi, bi, 2);
                cha = *(byte_t*)data;
                if ((cha >= '0') && (cha <= '1'))
                    cha -= 48;
                else
                    break;
                data = (byte_t*)data + 1;
                bigint_addI(bi, bi, cha);
            }
            break;

        default:
            if (size > BI_MAXLEN * sizeof(int32u))
                size = BI_MAXLEN * sizeof(int32u);
            for (idx = 0; idx < size; idx++) {
                cha = *(byte_t*)data;
                data = (byte_t*)data + 1;
                bi->val[idx >> 2] |= (cha << ((idx & 3) * 8));
            }
            bi->len = (sint_t)((size + 3) >> 2);
            bigint_adj(bi);
            break;
    }
}

/*
---------------------------------------
    求 RSA 私钥
---------------------------------------
*/
static void_t
bigint_private (
  __CR_OT__ sBIGINT*        biD,
  __CR_OT__ sBIGINT*        biN,
  __CR_IN__ const sBIGINT*  biP,
  __CR_IN__ const sBIGINT*  biQ,
  __CR_IN__ const sBIGINT*  biE
    )
{
    sBIGINT F, P, Q;

    bigint_subI(&P, biP, 1);
    bigint_subI(&Q, biQ, 1);
    bigint_mul(&F, &P, &Q);
    bigint_euc(biD, biE, &F);
    bigint_mul(biN, biP, biQ);
}

/*
---------------------------------------
    大整数填充1到最高位
---------------------------------------
*/
static void_t
bigint_fill2top (
  __CR_IO__ sBIGINT*    bi
    )
{
    sint_t  idx;

    for (idx = 0; idx < bi->len - 1; idx++)
        bi->val[idx] = 0xFFFFFFFFUL;
    bi->val[idx] = fill2top32(bi->val[idx]);
}

/*
---------------------------------------
    大整数统计字节1的个数
---------------------------------------
*/
static uint_t
bigint_count_bits (
  __CR_IN__ const sBIGINT*  bi
    )
{
    sint_t  idx;
    uint_t  cnt = 0;

    for (idx = 0; idx < bi->len; idx++)
        cnt += count_bits32(bi->val[idx]);
    return (cnt);
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
