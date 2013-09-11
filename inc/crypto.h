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
#define __CR_CRYPTO_H__ 0x6125D435UL

#include "defs.h"

/*****************************************************************************/
/*                                 常用算法                                  */
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
/* ARC4 */
typedef struct
{
        byte_t  x, y;
        byte_t  s[256];
} sARC4;

CR_API void_t   crypto_arc4_key (sARC4 *ctx, const void_t *key,
                                 leng_t keylen);
CR_API void_t   crypto_arc4_ops (sARC4 *ctx, void_t *data, leng_t size);

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
                                bool_t is_dec CR_DEFAULT(TRUE));
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
                                uint_t bits CR_DEFAULT(1024));
/* TEA */
CR_API void_t   crypto_tea_enc (int32u data[2], const int32u key[4],
                                uint_t round CR_DEFAULT(16));
CR_API void_t   crypto_tea_dec (int32u data[2], const int32u key[4],
                                uint_t round CR_DEFAULT(16));
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

/*****************************************************************************/
/*                                密码学算法                                 */
/*****************************************************************************/


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
