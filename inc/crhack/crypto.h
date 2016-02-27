/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-06  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 加密解密头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_CRYPTO_H__
#define __CR_CRYPTO_H__

#include "defs.h"

/*****************************************************************************/
/*                                密码学相关                                 */
/*****************************************************************************/

/* 获取一张小素数表 */
CR_API sint_t           prime_tbl_num (void_t);
CR_API const sint_t*    prime_tbl_ptr (void_t);

/* 4096位正整数结构 */
typedef struct
{
        sint_t  len;
        int32u  val[128];

} sBIGINT;

CR_API void_t   bigint_adj (sBIGINT *bi);
CR_API void_t   bigint_clr (sBIGINT *bi);
CR_API void_t   bigint_cpy (sBIGINT *biA, const sBIGINT *biB);
CR_API void_t   bigint_set (sBIGINT *bi, int64u val);
CR_API sint_t   bigint_cmp (const sBIGINT *biA, const sBIGINT *biB);
CR_API void_t   bigint_add (sBIGINT *biX, const sBIGINT *biA,
                            const sBIGINT *biB);
CR_API void_t   bigint_sub (sBIGINT *biX, const sBIGINT *biA,
                            const sBIGINT *biB);
CR_API void_t   bigint_mul (sBIGINT *biX, const sBIGINT *biA,
                            const sBIGINT *biB);
CR_API void_t   bigint_div (sBIGINT *biX, const sBIGINT *biA,
                            const sBIGINT *biB);
CR_API void_t   bigint_mod (sBIGINT *biX, const sBIGINT *biA,
                            const sBIGINT *biB);
CR_API void_t   bigint_addI (sBIGINT *biX, const sBIGINT *biA, int32u val);
CR_API void_t   bigint_subI (sBIGINT *biX, const sBIGINT *biA, int32u val);
CR_API void_t   bigint_mulI (sBIGINT *biX, const sBIGINT *biA, int32u val);
CR_API void_t   bigint_divI (sBIGINT *biX, const sBIGINT *biA, int32u val);
CR_API int32u   bigint_modI (const sBIGINT *bi, int32u val);
CR_API void_t   bigint_euc (sBIGINT *biX, const sBIGINT *biA,
                            const sBIGINT *biB);
CR_API void_t   bigint_rsa (sBIGINT *biX, const sBIGINT *biA,
                            const sBIGINT *biB, const sBIGINT *biC);
CR_API bool_t   bigint_rab (const sBIGINT *bi);
CR_API void_t   bigint_prime (sBIGINT *bi, sint_t bits);
CR_API uint_t   bigint_bits (const sBIGINT *bi);
CR_API leng_t   bigint_to (const sBIGINT *bi, void_t *data,
                           leng_t size, uint_t type);
CR_API void_t   bigint_from (sBIGINT *bi, const void_t *data,
                             leng_t size, uint_t type);
CR_API void_t   bigint_private (sBIGINT *biD, sBIGINT *biN,
                                const sBIGINT *biP, const sBIGINT *biQ,
                                const sBIGINT *biE);
CR_API void_t   bigint_fill2top (sBIGINT *bi);
CR_API uint_t   bigint_count_bits (const sBIGINT *bi);

/*****************************************************************************/
/*                               加密解密算法                                */
/*****************************************************************************/

/* AES */
typedef struct
{
        uint_t  key_len;
        int32u  key_enc[60];
        int32u  key_dec[60];
} sAES;

CR_API void_t   crypto_aes_key (sAES *ctx, const void_t *key,
                                leng_t keylen);
CR_API void_t   crypto_aes_enc (const sAES *ctx, int32u dst[4],
                                const int32u src[4]);
CR_API void_t   crypto_aes_dec (const sAES *ctx, int32u dst[4],
                                const int32u src[4]);
/* ANUBIS */
typedef struct
{
        uint_t  round;
        int32u  e[19][4];
        int32u  d[19][4];

} sANUBIS;

CR_API void_t   crypto_anubis_enc (sANUBIS *ctx, int32u dst[4],
                                   const int32u src[4]);
CR_API void_t   crypto_anubis_dec (sANUBIS *ctx, int32u dst[4],
                                   const int32u src[4]);
CR_API void_t   crypto_anubis_key (sANUBIS *ctx, const void_t *key,
                                   leng_t keylen);
/* BLOWFISH */
typedef struct
{
        int32u  p[18];
        int32u  s[1024];
} sBF;

CR_API void_t   crypto_bfish_key (sBF *ctx, const void_t *key,
                                  leng_t keylen);
CR_API void_t   crypto_bfish_enc (const sBF *ctx, int32u dst[2],
                                  const int32u src[2]);
CR_API void_t   crypto_bfish_dec (const sBF *ctx, int32u dst[2],
                                  const int32u src[2]);
/* CAST5 */
typedef struct
{
        uint_t  round;
        int32u  km[16];
        byte_t  kr[16];

} sCAST5;

CR_API void_t   crypto_cast5_key (sCAST5 *ctx, const void_t *key,
                                  leng_t keylen);
CR_API void_t   crypto_cast5_enc (const sCAST5 *ctx, int32u dst[2],
                                  const int32u src[2]);
CR_API void_t   crypto_cast5_dec (const sCAST5 *ctx, int32u dst[2],
                                  const int32u src[2]);
/* CAST6 */
typedef struct
{
        int32u  km[12][4];
        byte_t  kr[12][4];

} sCAST6;

CR_API void_t   crypto_cast6_key (sCAST6 *ctx, const void_t *key,
                                  leng_t keylen);
CR_API void_t   crypto_cast6_enc (const sCAST6 *ctx, int32u dst[4],
                                  const int32u src[4]);
CR_API void_t   crypto_cast6_dec (const sCAST6 *ctx, int32u dst[4],
                                  const int32u src[4]);
/* DES */
typedef struct
{
        int32u  knl[32];
} sDES;

CR_API void_t   crypto_des_key (sDES *ctx, const byte_t key[8],
                                bool_t is_dec);
CR_API void_t   crypto_des_ops (const sDES *ctx, byte_t dst[8],
                                const byte_t src[8]);
/* 3DES */
typedef struct
{
        sDES    ek1, dk1;
        sDES    ek2, dk2;
        sDES    ek3, dk3;
} s3DES;

CR_API void_t   crypto_3des_enc1 (const s3DES *ctx, byte_t dst[8],
                                  const byte_t src[8]);
CR_API void_t   crypto_3des_dec1 (const s3DES *ctx, byte_t dst[8],
                                  const byte_t src[8]);
CR_API void_t   crypto_3des_enc2 (const s3DES *ctx, byte_t dst[8],
                                  const byte_t src[8]);
CR_API void_t   crypto_3des_dec2 (const s3DES *ctx, byte_t dst[8],
                                  const byte_t src[8]);
CR_API void_t   crypto_3des_key2 (s3DES *ctx, const byte_t key[16]);
CR_API void_t   crypto_3des_key3 (s3DES *ctx, const byte_t key[24]);

/* IDEA */
CR_API void_t   crypto_idea_enc (int16u dst[4], const int16u src[4],
                                 const int16u key[8]);
CR_API void_t   crypto_idea_dec (int16u dst[4], const int16u src[4],
                                 const int16u key[8]);
/* RC2 */
typedef struct
{
        int16u  xkey[64];
} sRC2;

CR_API void_t   crypto_rc2_enc (const sRC2 *ctx, byte_t dst[8],
                                const byte_t src[8]);
CR_API void_t   crypto_rc2_dec (const sRC2 *ctx, byte_t dst[8],
                                const byte_t src[8]);
CR_API void_t   crypto_rc2_key (sRC2 *ctx, const void_t *key, leng_t keylen,
                                uint_t bits CR_DEFAULT(0));
/* RC4 */
typedef struct
{
        byte_t  x, y;
        byte_t  s[256];
} sRC4;

CR_API void_t   crypto_rc4_key (sRC4 *ctx, const void_t *key, leng_t keylen);
CR_API void_t   crypto_rc4_ops (sRC4 *ctx, void_t *data, leng_t size);

/* RSA */
typedef struct
{
        uint_t      block;
        uint_t      split;
        sBIGINT     N, E;

} sRSA;

CR_API leng_t   crypto_rsa_enc (const sRSA *pub, void_t *dst,
                                const void_t *src, leng_t size);
CR_API leng_t   crypto_rsa_dec (const sRSA *prv, void_t *dst,
                                const void_t *src, leng_t size);
CR_API void_t   crypto_rsa_key (sRSA *pub, sRSA *prv, const ansi_t *e10,
                                uint_t bits, uint_t split CR_DEFAULT(0));
/* TEA */
CR_API void_t   crypto_tea_enc (int32u data[2], const int32u key[4],
                                uint_t round CR_DEFAULT(32));
CR_API void_t   crypto_tea_dec (int32u data[2], const int32u key[4],
                                uint_t round CR_DEFAULT(32));
/* TWOFISH */
typedef struct
{
        int32u  s[4][256];
        int32u  w[8], k[32];
} sTF;

CR_API void_t   crypto_tfish_key (sTF *ctx, const void_t *key,
                                  leng_t keylen);
CR_API void_t   crypto_tfish_enc (const sTF *ctx, int32u dst[4],
                                  const int32u src[4]);
CR_API void_t   crypto_tfish_dec (const sTF *ctx, int32u dst[4],
                                  const int32u src[4]);
/* XTEA */
CR_API void_t   crypto_xtea_enc (int32u data[2], const int32u key[4],
                                 uint_t round CR_DEFAULT(32));
CR_API void_t   crypto_xtea_dec (int32u data[2], const int32u key[4],
                                 uint_t round CR_DEFAULT(32));
/* XXTEA */
CR_API void_t   crypto_xxtea_enc (int32u *data, leng_t num,
                                  const int32u key[4]);
CR_API void_t   crypto_xxtea_dec (int32u *data, leng_t num,
                                  const int32u key[4]);
/* PADDING MODE */
#define CR_PADDING_PKCS5        0x100
#define CR_PADDING_PKCS7        0x200
#define CR_PADDING_ANSI_X923    0x300
#define CR_PADDING_ISO10126     0x400

/* ALL-EBC */
CR_API leng_t   crypto_all_ebc_enc (void_t *ctx, void_t *dst,
                                    leng_t dstlen, leng_t dstblk,
                                    const void_t *src, leng_t srclen,
                                    leng_t srcblk, uint_t fill,
                                    enc_parm_t func);

CR_API leng_t   crypto_all_ebc_dec (void_t *ctx, void_t *dst,
                                    leng_t dstlen, leng_t dstblk,
                                    const void_t *src, leng_t srclen,
                                    leng_t srcblk, enc_parm_t func);
/* ALL-CBC */
CR_API leng_t   crypto_all_cbc_enc (void_t *ctx, void_t *dst,
                                    leng_t dstlen, const void_t *src,
                                    leng_t srclen, const void_t *ivec,
                                    leng_t block, uint_t fill,
                                    enc_parm_t func);

CR_API leng_t   crypto_all_cbc_dec (void_t *ctx, void_t *dst,
                                    leng_t dstlen, const void_t *src,
                                    leng_t srclen, const void_t *ivec,
                                    leng_t block, enc_parm_t func);

#endif  /* !__CR_CRYPTO_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
