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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 数据哈希头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_HASH_H__
#define __CR_HASH_H__

#include "defs.h"
#if defined(_CR_OS_MACOSX_)
    #include <CommonCrypto/CommonDigest.h>
#endif

/*****************************************************************************/
/*                                数据块哈希                                 */
/*****************************************************************************/

/* Adler32 */
CR_API int32u   hash_adler32_init (void_t);
CR_API int32u   hash_adler32_update (int32u hash, const void_t *data,
                                     leng_t size);
CR_API int32u   hash_adler32_finish (int32u hash);
CR_API int32u   hash_adler32_total (const void_t *data, leng_t size);

/* XOR-8 */
CR_API byte_t   hash_xor8_init (void_t);
CR_API byte_t   hash_xor8_update (byte_t hash, const void_t *data,
                                  leng_t size);
CR_API byte_t   hash_xor8_finish (byte_t hash);
CR_API byte_t   hash_xor8_total (const void_t *data, leng_t size);

/* CheckSum32 */
CR_API int32u   hash_sum32_init (void_t);
CR_API int32u   hash_sum32_update (int32u hash, const void_t *data,
                                   leng_t size);
CR_API int32u   hash_sum32_finish (int32u hash);
CR_API int32u   hash_sum32_total (const void_t *data, leng_t size);

/* CheckSum64 */
CR_API int64u   hash_sum64_init (void_t);
CR_API int64u   hash_sum64_update (int64u hash, const void_t *data,
                                   leng_t size);
CR_API int64u   hash_sum64_finish (int64u hash);
CR_API int64u   hash_sum64_total (const void_t *data, leng_t size);

/* CRC-7/MMC */
CR_API byte_t   hash_crc7_init (void_t);
CR_API byte_t   hash_crc7_update (byte_t hash, const void_t *data,
                                  leng_t size);
CR_API byte_t   hash_crc7_finish (byte_t hash);
CR_API byte_t   hash_crc7_total (const void_t *data, leng_t size);

/* CRC-8/ATM */
CR_API byte_t   hash_crc8a_init (void_t);
CR_API byte_t   hash_crc8a_update (byte_t hash, const void_t *data,
                                   leng_t size);
CR_API byte_t   hash_crc8a_finish (byte_t hash);
CR_API byte_t   hash_crc8a_total (const void_t *data, leng_t size);

/* CRC-8/DALLAS */
CR_API byte_t   hash_crc8d_init (void_t);
CR_API byte_t   hash_crc8d_update (byte_t hash, const void_t *data,
                                   leng_t size);
CR_API byte_t   hash_crc8d_finish (byte_t hash);
CR_API byte_t   hash_crc8d_total (const void_t *data, leng_t size);

/* CRC-16/CCITT */
CR_API int16u   hash_crc16c_init (void_t);
CR_API int16u   hash_crc16c_update (int16u hash, const void_t *data,
                                    leng_t size);
CR_API int16u   hash_crc16c_finish (int16u hash);
CR_API int16u   hash_crc16c_total (const void_t *data, leng_t size);

/* CRC-16/IBM */
CR_API int16u   hash_crc16i_init (void_t);
CR_API int16u   hash_crc16i_update (int16u hash, const void_t *data,
                                    leng_t size);
CR_API int16u   hash_crc16i_finish (int16u hash);
CR_API int16u   hash_crc16i_total (const void_t *data, leng_t size);

/* CRC-32/AAL5 */
CR_API int32u   hash_crc32a_init (void_t);
CR_API int32u   hash_crc32a_update (int32u hash, const void_t *data,
                                    leng_t size);
CR_API int32u   hash_crc32a_finish (int32u hash);
CR_API int32u   hash_crc32a_total (const void_t *data, leng_t size);

/* CRC-32/CASTAGNOLI */
CR_API int32u   hash_crc32c_init (void_t);
CR_API int32u   hash_crc32c_update (int32u hash, const void_t *data,
                                    leng_t size);
CR_API int32u   hash_crc32c_finish (int32u hash);
CR_API int32u   hash_crc32c_total (const void_t *data, leng_t size);

/* CRC-32/HONEYMAN */
CR_API int32u   hash_crc32h_init (void_t);
CR_API int32u   hash_crc32h_update (int32u hash, const void_t *data,
                                    leng_t size);
CR_API int32u   hash_crc32h_finish (int32u hash);
CR_API int32u   hash_crc32h_total (const void_t *data, leng_t size);

/* CRC-32/IEEE */
CR_API int32u   hash_crc32i_init (void_t);
CR_API int32u   hash_crc32i_update (int32u hash, const void_t *data,
                                    leng_t size);
CR_API int32u   hash_crc32i_finish (int32u hash);
CR_API int32u   hash_crc32i_total (const void_t *data, leng_t size);

/* CRC-64/ECMA-182 */
CR_API int64u   hash_crc64e_init (void_t);
CR_API int64u   hash_crc64e_update (int64u hash, const void_t *data,
                                    leng_t size);
CR_API int64u   hash_crc64e_finish (int64u hash);
CR_API int64u   hash_crc64e_total (const void_t *data, leng_t size);

/* CRC-ALL */
CR_API byte_t   hash_crc_x (byte_t seed, byte_t poly, bool_t lsb,
                        ufast_t bits, const void_t *data, leng_t size);
CR_API byte_t   hash_crc8x (byte_t seed, byte_t poly, bool_t lsb,
                            const void_t *data, leng_t size);
CR_API int16u   hash_crc16x (int16u seed, int16u poly, bool_t lsb,
                             const void_t *data, leng_t size);
CR_API int32u   hash_crc32x (int32u seed, int32u poly, bool_t lsb,
                             const void_t *data, leng_t size);
CR_API int64u   hash_crc64x (int64u seed, int64u poly, bool_t lsb,
                             const void_t *data, leng_t size);
/* BLIZZARD5 */
typedef struct
{
        int32u  seed1, seed2;
        int32u  seed3, seed4;
        int32u  seed5, seed6;
        int32u  seed7, seed8;
        int32u  seed9, seeda;
} sBZZ5;

CR_API void_t   hash_bzz5_init (sBZZ5 *ctx);
CR_API void_t   hash_bzz5_update (sBZZ5 *ctx, const void_t *data,
                                  leng_t size);
CR_API void_t   hash_bzz5_finish (int32u hash[5], sBZZ5 *ctx);
CR_API void_t   hash_bzz5_total (int32u hash[5], const void_t *data,
                                 leng_t size);
/* MD2 */
#if defined(_CR_OS_MACOSX_)
    typedef CC_MD2_CTX  sMD2;
#else
typedef struct
{
        leng_t  left;
        byte_t  cksum[16];
        byte_t  state[48];
        byte_t  buffer[16];
} sMD2;
#endif
CR_API void_t   hash_md2_init (sMD2 *ctx);
CR_API void_t   hash_md2_update (sMD2 *ctx, const void_t *data,
                                 leng_t size);
CR_API void_t   hash_md2_finish (byte_t hash[16], sMD2 *ctx);
CR_API void_t   hash_md2_total (byte_t hash[16], const void_t *data,
                                leng_t size);
/* MD4, MD5 */
#if defined(_CR_OS_MACOSX_)
    typedef CC_MD4_CTX  sMD4;
    typedef CC_MD5_CTX  sMD5;
#else
typedef struct
{
        int32u  count[2];
        int32u  state[4];
        byte_t  buffer[64];

} sMD4, sMD5;
#endif
/* MD4 */
CR_API void_t   hash_md4_init (sMD4 *ctx);
CR_API void_t   hash_md4_update (sMD4 *ctx, const void_t *data,
                                 leng_t size);
CR_API void_t   hash_md4_finish (byte_t hash[16], sMD4 *ctx);
CR_API void_t   hash_md4_total (byte_t hash[16], const void_t *data,
                                leng_t size);
/* MD5 */
CR_API void_t   hash_md5_init (sMD5 *ctx);
CR_API void_t   hash_md5_update (sMD5 *ctx, const void_t *data,
                                 leng_t size);
CR_API void_t   hash_md5_finish (byte_t hash[16], sMD5 *ctx);
CR_API void_t   hash_md5_total (byte_t hash[16], const void_t *data,
                                leng_t size);
/* SHA-1 */
#if defined(_CR_OS_MACOSX_)
    typedef CC_SHA1_CTX     sSHA1;
#else
typedef struct
{
        int32u  count[2];
        int32u  state[5];
        byte_t  buffer[64];
} sSHA1;
#endif
CR_API void_t   hash_sha1_init (sSHA1 *ctx);
CR_API void_t   hash_sha1_update (sSHA1 *ctx, const void_t *data,
                                  leng_t size);
CR_API void_t   hash_sha1_finish (byte_t hash[20], sSHA1 *ctx);
CR_API void_t   hash_sha1_total (byte_t hash[20], const void_t *data,
                                 leng_t size);
/* SHA-224, SHA-256 */
#if defined(_CR_OS_MACOSX_)
    typedef CC_SHA256_CTX   sSHA224;
    typedef CC_SHA256_CTX   sSHA256;
#else
typedef struct
{
        int32u  count[2];
        int32u  state[8];
        byte_t  buffer[64];

} sSHA224, sSHA256;
#endif
/* SHA-224 */
CR_API void_t   hash_sha224_init (sSHA224 *ctx);
CR_API void_t   hash_sha224_update (sSHA224 *ctx, const void_t *data,
                                    leng_t size);
CR_API void_t   hash_sha224_finish (byte_t hash[28], sSHA224 *ctx);
CR_API void_t   hash_sha224_total (byte_t hash[28], const void_t *data,
                                   leng_t size);
/* SHA-256 */
CR_API void_t   hash_sha256_init (sSHA256 *ctx);
CR_API void_t   hash_sha256_update (sSHA256 *ctx, const void_t *data,
                                    leng_t size);
CR_API void_t   hash_sha256_finish (byte_t hash[32], sSHA256 *ctx);
CR_API void_t   hash_sha256_total (byte_t hash[32], const void_t *data,
                                   leng_t size);
/* SHA-384, SHA-512 */
#if defined(_CR_OS_MACOSX_)
    typedef CC_SHA512_CTX   sSHA384;
    typedef CC_SHA512_CTX   sSHA512;
#else
typedef struct
{
        int64u  count[2];
        int64u  state[8];
        byte_t  buffer[128];

} sSHA384, sSHA512;
#endif
/* SHA-384 */
CR_API void_t   hash_sha384_init (sSHA384 *ctx);
CR_API void_t   hash_sha384_update (sSHA384 *ctx, const void_t *data,
                                    leng_t size);
CR_API void_t   hash_sha384_finish (byte_t hash[48], sSHA384 *ctx);
CR_API void_t   hash_sha384_total (byte_t hash[48], const void_t *data,
                                   leng_t size);
/* SHA-512 */
CR_API void_t   hash_sha512_init (sSHA512 *ctx);
CR_API void_t   hash_sha512_update (sSHA512 *ctx, const void_t *data,
                                    leng_t size);
CR_API void_t   hash_sha512_finish (byte_t hash[64], sSHA512 *ctx);
CR_API void_t   hash_sha512_total (byte_t hash[64], const void_t *data,
                                   leng_t size);
/* ED2K */
typedef struct
{
        sMD4    md4dat;
        sMD4    md4lst;
        sMD4    md4ctx;
        leng_t  counts;
        int32u  blocks;
} sED2K;

CR_API void_t   hash_ed2k_init (sED2K *ctx);
CR_API void_t   hash_ed2k_update (sED2K *ctx, const void_t *data,
                                  leng_t size);
CR_API void_t   hash_ed2k_finish1 (byte_t hash[16], sED2K *ctx);
CR_API void_t   hash_ed2k_finish2 (byte_t hash[16], sED2K *ctx);
CR_API void_t   hash_ed2k_total1 (byte_t hash[16], const void_t *data,
                                  leng_t size);
CR_API void_t   hash_ed2k_total2 (byte_t hash[16], const void_t *data,
                                  leng_t size);
/* RMD-128 */
typedef struct
{
        int32u  count[2];
        int32u  state[4];
        byte_t  buffer[64];

} sRMD128;

CR_API void_t   hash_rmd128_init (sRMD128 *ctx);
CR_API void_t   hash_rmd128_update (sRMD128 *ctx, const void_t *data,
                                    leng_t size);
CR_API void_t   hash_rmd128_finish (byte_t hash[16], sRMD128 *ctx);
CR_API void_t   hash_rmd128_total (byte_t hash[16], const void_t *data,
                                   leng_t size);
/* RMD-160 */
typedef struct
{
        int32u  count[2];
        int32u  state[5];
        byte_t  buffer[64];

} sRMD160;

CR_API void_t   hash_rmd160_init (sRMD160 *ctx);
CR_API void_t   hash_rmd160_update (sRMD160 *ctx, const void_t *data,
                                    leng_t size);
CR_API void_t   hash_rmd160_finish (byte_t hash[20], sRMD160 *ctx);
CR_API void_t   hash_rmd160_total (byte_t hash[20], const void_t *data,
                                   leng_t size);
/* RMD-256 */
typedef struct
{
        int32u  count[2];
        int32u  state[8];
        byte_t  buffer[64];

} sRMD256;

CR_API void_t   hash_rmd256_init (sRMD256 *ctx);
CR_API void_t   hash_rmd256_update (sRMD256 *ctx, const void_t *data,
                                    leng_t size);
CR_API void_t   hash_rmd256_finish (byte_t hash[32], sRMD256 *ctx);
CR_API void_t   hash_rmd256_total (byte_t hash[32], const void_t *data,
                                   leng_t size);
/* RMD-320 */
typedef struct
{
        int32u  count[2];
        int32u  state[10];
        byte_t  buffer[64];

} sRMD320;

CR_API void_t   hash_rmd320_init (sRMD320 *ctx);
CR_API void_t   hash_rmd320_update (sRMD320 *ctx, const void_t *data,
                                    leng_t size);
CR_API void_t   hash_rmd320_finish (byte_t hash[40], sRMD320 *ctx);
CR_API void_t   hash_rmd320_total (byte_t hash[40], const void_t *data,
                                   leng_t size);

/*****************************************************************************/
/*                                字符串哈希                                 */
/*****************************************************************************/

/* 获取哈希表大小 */
CR_API uint_t   hash_count (leng_t count);

/* 一组常用字符串哈希函数 (注意: 16bit 下结果是不同的) */
CR_API uint_t   hash_str_ap  (const void_t *data, leng_t size);
CR_API uint_t   hash_str_apx (const void_t *data, leng_t size);
CR_API uint_t   hash_str_bp  (const void_t *data, leng_t size);
CR_API uint_t   hash_str_bpx (const void_t *data, leng_t size);
CR_API uint_t   hash_str_js  (const void_t *data, leng_t size);
CR_API uint_t   hash_str_jsx (const void_t *data, leng_t size);
CR_API uint_t   hash_str_rs  (const void_t *data, leng_t size);
CR_API uint_t   hash_str_rsx (const void_t *data, leng_t size);

CR_API uint_t   hash_str_elf  (const void_t *data, leng_t size);
CR_API uint_t   hash_str_elfx (const void_t *data, leng_t size);
CR_API uint_t   hash_str_dek  (const void_t *data, leng_t size);
CR_API uint_t   hash_str_dekx (const void_t *data, leng_t size);
CR_API uint_t   hash_str_fnv  (const void_t *data, leng_t size);
CR_API uint_t   hash_str_fnvx (const void_t *data, leng_t size);

CR_API uint_t   hash_str_djb2  (const void_t *data, leng_t size);
CR_API uint_t   hash_str_djb2x (const void_t *data, leng_t size);
CR_API uint_t   hash_str_sdbm  (const void_t *data, leng_t size);
CR_API uint_t   hash_str_sdbmx (const void_t *data, leng_t size);
CR_API uint_t   hash_str_bkdr  (const void_t *data, leng_t size);
CR_API uint_t   hash_str_bkdrx (const void_t *data, leng_t size);

/*****************************************************************************/
/*                                数据块纠错                                 */
/*****************************************************************************/

/* ECC-512 */
CR_API void_t   par512_ecc1_make (byte_t parity[3], const byte_t data[512]);
CR_API sint_t   par512_ecc1_comp (byte_t data[512], const byte_t parity1[3],
                                  const byte_t parity2[3]);
/* ECC-256/512 */
CR_API bool_t   parity_ecc1_make (byte_t parity[3], const void_t *data,
                                  uint_t size);
CR_API sint_t   parity_ecc1_comp (void_t *data, const byte_t parity1[3],
                                  const byte_t parity2[3], uint_t size);
/* ECC-STM32 */
CR_API sint_t   parity_ecc1_comp_st (void_t *data, int32u parity1,
                                     int32u parity2, uint_t size);

#endif  /* !__CR_HASH_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
