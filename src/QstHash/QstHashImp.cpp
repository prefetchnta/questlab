/*
 ================================================
 *      ######
 *    #########                            ##
 *   ####    ###                           ##
 *   ##       ##                           ##
 *  ##         ## ##   ##   ####     ### ######
 *  ##         ## ##   ##  ######  ##### ######
 *  ##         ## ##   ## ###  ## ###      ##
 *  ##    #    ## ##   ## ##  ##  #####    ##
 *   ##   ##   ## ##   ## ## ##    #####   ##
 *   ###   ## ##  ##   ## ###   #     ##   ##
 *    ##########  ####### ####### ######   ##
 *      #### ##    ######   ####   ####    ##
 *            ##
 *             ##       CREATE: 2013-11-14
 *              #
 ================================================
        QuestLAB 哈希计算实现
 ================================================
 */

#include "QstHash.h"

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    插件 DLL 入口点
=======================================
*/
BOOL WINAPI
DllMain (
  __CR_IN__ HANDLE  hinst,
  __CR_IN__ DWORD   reason,
  __CR_UU__ LPVOID  reserved
    )
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}

#endif  /* _CR_BUILD_DLL_ */

/*
---------------------------------------
    数组转16进制串
---------------------------------------
*/
static ansi_t*
hex_to_str (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    ansi_t* str;

    str = str_allocA((size + 1) * 2 + 1);
    if (str != NULL) {
        str[0] = ':';
        str[1] = ' ';
        hex2strA(&str[2], data, size);
    }
    return (str);
}

/*****************************************************************************/
/*                                 Adler32                                   */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
adler32_init (void_t)
{
    int32u* ctx;

    ctx = struct_new(int32u);
    if (ctx != NULL)
        *ctx = hash_adler32_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
adler32_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u* cvt = (int32u*)ctx;

    *cvt = hash_adler32_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
adler32_finish (
  __CR_IO__ void_t* ctx
    )
{
    int32u* cvt = (int32u*)ctx;
    int32u  result = hash_adler32_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %08X", result));
}

/*****************************************************************************/
/*                                BLIZZARD5                                  */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
bzz5_init (void_t)
{
    sBZZ5*  ctx;

    ctx = struct_new(sBZZ5);
    if (ctx != NULL)
        hash_bzz5_init(ctx);
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
bzz5_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    hash_bzz5_update((sBZZ5*)ctx, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
bzz5_finish (
  __CR_IO__ void_t* ctx
    )
{
    int32u  result[5];

    hash_bzz5_finish(result, (sBZZ5*)ctx);
    mem_free(ctx);
    return (hex_to_str(result, sizeof(result)));
}

/*****************************************************************************/
/*                                CheckSum8                                  */
/*****************************************************************************/

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
sum08_finish (
  __CR_IO__ void_t* ctx
    )
{
    int32u* cvt = (int32u*)ctx;
    int32u  result = hash_sum32_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %02X", result & 0xFF));
}

/*****************************************************************************/
/*                                CheckSum16                                 */
/*****************************************************************************/

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
sum16_finish (
  __CR_IO__ void_t* ctx
    )
{
    int32u* cvt = (int32u*)ctx;
    int32u  result = hash_sum32_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %04X", result & 0xFFFF));
}

/*****************************************************************************/
/*                                CheckSum32                                 */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
sum32_init (void_t)
{
    int32u* ctx;

    ctx = struct_new(int32u);
    if (ctx != NULL)
        *ctx = hash_sum32_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
sum32_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u* cvt = (int32u*)ctx;

    *cvt = hash_sum32_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
sum32_finish (
  __CR_IO__ void_t* ctx
    )
{
    int32u* cvt = (int32u*)ctx;
    int32u  result = hash_sum32_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %08X", result));
}

/*****************************************************************************/
/*                                CheckSum64                                 */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
sum64_init (void_t)
{
    int64u* ctx;

    ctx = struct_new(int64u);
    if (ctx != NULL)
        *ctx = hash_sum64_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
sum64_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int64u* cvt = (int64u*)ctx;

    *cvt = hash_sum64_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
sum64_finish (
  __CR_IO__ void_t* ctx
    )
{
    int64u* cvt = (int64u*)ctx;
    int64u  result = hash_sum64_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %016I64X", result));
}

/*****************************************************************************/
/*                                  CRC-7                                    */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc7_init (void_t)
{
    byte_t* ctx;

    ctx = struct_new(byte_t);
    if (ctx != NULL)
        *ctx = hash_crc7_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
crc7_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    byte_t* cvt = (byte_t*)ctx;

    *cvt = hash_crc7_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
crc7_finish (
  __CR_IO__ void_t* ctx
    )
{
    byte_t* cvt = (byte_t*)ctx;
    byte_t  result = hash_crc7_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %02X", result));
}

/*****************************************************************************/
/*                                CRC-8/ATM                                  */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc8a_init (void_t)
{
    byte_t* ctx;

    ctx = struct_new(byte_t);
    if (ctx != NULL)
        *ctx = hash_crc8a_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
crc8a_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    byte_t* cvt = (byte_t*)ctx;

    *cvt = hash_crc8a_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
crc8a_finish (
  __CR_IO__ void_t* ctx
    )
{
    byte_t* cvt = (byte_t*)ctx;
    byte_t  result = hash_crc8a_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %02X", result));
}

/*****************************************************************************/
/*                               CRC-8/DALLAS                                */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc8d_init (void_t)
{
    byte_t* ctx;

    ctx = struct_new(byte_t);
    if (ctx != NULL)
        *ctx = hash_crc8d_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
crc8d_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    byte_t* cvt = (byte_t*)ctx;

    *cvt = hash_crc8d_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
crc8d_finish (
  __CR_IO__ void_t* ctx
    )
{
    byte_t* cvt = (byte_t*)ctx;
    byte_t  result = hash_crc8d_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %02X", result));
}

/*****************************************************************************/
/*                           CRC-16/CCITT (0000)                             */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc16c_init_not (void_t)
{
    int16u* ctx;

    ctx = struct_new(int16u);
    if (ctx != NULL)
        *ctx = ~hash_crc16c_init();
    return (ctx);
}

/*****************************************************************************/
/*                           CRC-16/CCITT (FFFF)                             */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc16c_init (void_t)
{
    int16u* ctx;

    ctx = struct_new(int16u);
    if (ctx != NULL)
        *ctx = hash_crc16c_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
crc16c_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int16u* cvt = (int16u*)ctx;

    *cvt = hash_crc16c_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
crc16c_finish (
  __CR_IO__ void_t* ctx
    )
{
    int16u* cvt = (int16u*)ctx;
    int16u  result = hash_crc16c_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %04X", result));
}

/*****************************************************************************/
/*                            CRC-16/IBM (0000)                              */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc16i_init (void_t)
{
    int16u* ctx;

    ctx = struct_new(int16u);
    if (ctx != NULL)
        *ctx = hash_crc16i_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
crc16i_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int16u* cvt = (int16u*)ctx;

    *cvt = hash_crc16i_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
crc16i_finish (
  __CR_IO__ void_t* ctx
    )
{
    int16u* cvt = (int16u*)ctx;
    int16u  result = hash_crc16i_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %04X", result));
}

/*****************************************************************************/
/*                            CRC-16/IBM (FFFF)                              */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc16i_init_not (void_t)
{
    int16u* ctx;

    ctx = struct_new(int16u);
    if (ctx != NULL)
        *ctx = ~hash_crc16i_init();
    return (ctx);
}

/*****************************************************************************/
/*                               CRC-32/AAL5                                 */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc32a_init (void_t)
{
    int32u* ctx;

    ctx = struct_new(int32u);
    if (ctx != NULL)
        *ctx = hash_crc32a_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
crc32a_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u* cvt = (int32u*)ctx;

    *cvt = hash_crc32a_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
crc32a_finish (
  __CR_IO__ void_t* ctx
    )
{
    int32u* cvt = (int32u*)ctx;
    int32u  result = hash_crc32a_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %08X", result));
}

/*****************************************************************************/
/*                            CRC-32/CASTAGNOLI                              */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc32c_init (void_t)
{
    int32u* ctx;

    ctx = struct_new(int32u);
    if (ctx != NULL)
        *ctx = hash_crc32c_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
crc32c_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u* cvt = (int32u*)ctx;

    *cvt = hash_crc32c_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
crc32c_finish (
  __CR_IO__ void_t* ctx
    )
{
    int32u* cvt = (int32u*)ctx;
    int32u  result = hash_crc32c_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %08X", result));
}

/*****************************************************************************/
/*                             CRC-32/HONEYMAN                               */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc32h_init (void_t)
{
    int32u* ctx;

    ctx = struct_new(int32u);
    if (ctx != NULL)
        *ctx = hash_crc32h_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
crc32h_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u* cvt = (int32u*)ctx;

    *cvt = hash_crc32h_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
crc32h_finish (
  __CR_IO__ void_t* ctx
    )
{
    int32u* cvt = (int32u*)ctx;
    int32u  result = hash_crc32h_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %08X", result));
}

/*****************************************************************************/
/*                               CRC-32/IEEE                                 */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc32i_init (void_t)
{
    int32u* ctx;

    ctx = struct_new(int32u);
    if (ctx != NULL)
        *ctx = hash_crc32i_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
crc32i_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u* cvt = (int32u*)ctx;

    *cvt = hash_crc32i_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
crc32i_finish (
  __CR_IO__ void_t* ctx
    )
{
    int32u* cvt = (int32u*)ctx;
    int32u  result = hash_crc32i_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %08X", result));
}

/*****************************************************************************/
/*                             CRC-64/ECMA-182                               */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
crc64e_init (void_t)
{
    int64u* ctx;

    ctx = struct_new(int64u);
    if (ctx != NULL)
        *ctx = hash_crc64e_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
crc64e_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int64u* cvt = (int64u*)ctx;

    *cvt = hash_crc64e_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
crc64e_finish (
  __CR_IO__ void_t* ctx
    )
{
    int64u* cvt = (int64u*)ctx;
    int64u  result = hash_crc64e_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %016I64X", result));
}

/*****************************************************************************/
/*                                  ED2K-A                                   */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
ed2k_init (void_t)
{
    sED2K*  ctx;

    ctx = struct_new(sED2K);
    if (ctx != NULL)
        hash_ed2k_init(ctx);
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
ed2k_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    hash_ed2k_update((sED2K*)ctx, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
ed2k_finish1 (
  __CR_IO__ void_t* ctx
    )
{
    byte_t  result[16];

    hash_ed2k_finish1(result, (sED2K*)ctx);
    mem_free(ctx);
    return (hex_to_str(result, sizeof(result)));
}

/*****************************************************************************/
/*                                  ED2K-B                                   */
/*****************************************************************************/

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
ed2k_finish2 (
  __CR_IO__ void_t* ctx
    )
{
    byte_t  result[16];

    hash_ed2k_finish2(result, (sED2K*)ctx);
    mem_free(ctx);
    return (hex_to_str(result, sizeof(result)));
}

/*****************************************************************************/
/*                                   MD4                                     */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
md4_init (void_t)
{
    sMD4*   ctx;

    ctx = struct_new(sMD4);
    if (ctx != NULL)
        hash_md4_init(ctx);
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
md4_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    hash_md4_update((sMD4*)ctx, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
md4_finish (
  __CR_IO__ void_t* ctx
    )
{
    byte_t  result[16];

    hash_md4_finish(result, (sMD4*)ctx);
    mem_free(ctx);
    return (hex_to_str(result, sizeof(result)));
}

/*****************************************************************************/
/*                                   MD5                                     */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
md5_init (void_t)
{
    sMD5*   ctx;

    ctx = struct_new(sMD5);
    if (ctx != NULL)
        hash_md5_init(ctx);
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
md5_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    hash_md5_update((sMD5*)ctx, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
md5_finish (
  __CR_IO__ void_t* ctx
    )
{
    byte_t  result[16];

    hash_md5_finish(result, (sMD5*)ctx);
    mem_free(ctx);
    return (hex_to_str(result, sizeof(result)));
}

/*****************************************************************************/
/*                                   SHA1                                    */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
sha1_init (void_t)
{
    sSHA1*  ctx;

    ctx = struct_new(sSHA1);
    if (ctx != NULL)
        hash_sha1_init(ctx);
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
sha1_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    hash_sha1_update((sSHA1*)ctx, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
sha1_finish (
  __CR_IO__ void_t* ctx
    )
{
    byte_t  result[20];

    hash_sha1_finish(result, (sSHA1*)ctx);
    mem_free(ctx);
    return (hex_to_str(result, sizeof(result)));
}

/*****************************************************************************/
/*                                  STR-AP                                   */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
string_init (void_t)
{
    return (struct_new(uint_t));
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
str_ap_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    *(uint_t*)ctx = hash_str_ap(data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
string_finish (
  __CR_IO__ void_t* ctx
    )
{
    uint_t  result = *(uint_t*)ctx;

    mem_free(ctx);
    return (str_fmtA(": %u", result));
}

/*****************************************************************************/
/*                                 STR-BKDR                                  */
/*****************************************************************************/

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
str_bkdr_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    *(uint_t*)ctx = hash_str_bkdr(data, size);
}

/*****************************************************************************/
/*                                  STR-BP                                   */
/*****************************************************************************/

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
str_bp_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    *(uint_t*)ctx = hash_str_bp(data, size);
}

/*****************************************************************************/
/*                                 STR-DEK                                   */
/*****************************************************************************/

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
str_dek_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    *(uint_t*)ctx = hash_str_dek(data, size);
}

/*****************************************************************************/
/*                                 STR-DJB2                                  */
/*****************************************************************************/

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
str_djb2_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    *(uint_t*)ctx = hash_str_djb2(data, size);
}

/*****************************************************************************/
/*                                 STR-ELF                                   */
/*****************************************************************************/

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
str_elf_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    *(uint_t*)ctx = hash_str_elf(data, size);
}

/*****************************************************************************/
/*                                 STR-FNV                                   */
/*****************************************************************************/

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
str_fnv_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    *(uint_t*)ctx = hash_str_fnv(data, size);
}

/*****************************************************************************/
/*                                  STR-JS                                   */
/*****************************************************************************/

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
str_js_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    *(uint_t*)ctx = hash_str_js(data, size);
}

/*****************************************************************************/
/*                                  STR-RS                                   */
/*****************************************************************************/

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
str_rs_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    *(uint_t*)ctx = hash_str_rs(data, size);
}

/*****************************************************************************/
/*                                 STR-SDBM                                  */
/*****************************************************************************/

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
str_sdbm_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    *(uint_t*)ctx = hash_str_sdbm(data, size);
}

/*****************************************************************************/
/*                                  XOR-8                                    */
/*****************************************************************************/

/*
---------------------------------------
    获取哈希对象
---------------------------------------
*/
static void_t*
xor8_init (void_t)
{
    byte_t* ctx;

    ctx = struct_new(byte_t);
    if (ctx != NULL)
        *ctx = hash_xor8_init();
    return (ctx);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
xor8_update (
  __CR_IO__ void_t*         ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    byte_t* cvt = (byte_t*)ctx;

    *cvt = hash_xor8_update(*cvt, data, size);
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static ansi_t*
xor8_finish (
  __CR_IO__ void_t* ctx
    )
{
    byte_t* cvt = (byte_t*)ctx;
    byte_t  result = hash_xor8_finish(*cvt);

    mem_free(ctx);
    return (str_fmtA(": %02X", result));
}

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

/* 导出所有哈希计算单元 */
CR_API const sQHSH_UNIT hasher[] = {
{ "Adler32", TRUE, adler32_init, adler32_update, adler32_finish },
{ "BLIZZARD5", TRUE, bzz5_init, bzz5_update, bzz5_finish },
{ "CheckSum8", TRUE, sum32_init, sum32_update, sum08_finish },
{ "CheckSum16", TRUE, sum32_init, sum32_update, sum16_finish },
{ "CheckSum32", TRUE, sum32_init, sum32_update, sum32_finish },
{ "CheckSum64", TRUE, sum64_init, sum64_update, sum64_finish },
{ "CRC-7", TRUE, crc7_init, crc7_update, crc7_finish },
{ "CRC-8/ATM", TRUE, crc8a_init, crc8a_update, crc8a_finish },
{ "CRC-8/DALLAS", TRUE, crc8d_init, crc8d_update, crc8d_finish },
{ "CRC-16/CCITT (0000)", TRUE, crc16c_init_not, crc16c_update, crc16c_finish },
{ "CRC-16/CCITT (FFFF)", TRUE, crc16c_init, crc16c_update, crc16c_finish },
{ "CRC-16/IBM (0000)", TRUE, crc16i_init, crc16i_update, crc16i_finish },
{ "CRC-16/IBM (FFFF)", TRUE, crc16i_init_not, crc16i_update, crc16i_finish },
{ "CRC-32/AAL5", TRUE, crc32a_init, crc32a_update, crc32a_finish },
{ "CRC-32/CASTAGNOLI", TRUE, crc32c_init, crc32c_update, crc32c_finish },
{ "CRC-32/HONEYMAN", TRUE, crc32h_init, crc32h_update, crc32h_finish },
{ "CRC-32/IEEE", TRUE, crc32i_init, crc32i_update, crc32i_finish },
{ "CRC-64/ECMA-182", TRUE, crc64e_init, crc64e_update, crc64e_finish },
{ "ED2K-A", TRUE, ed2k_init, ed2k_update, ed2k_finish1 },
{ "ED2K-B", TRUE, ed2k_init, ed2k_update, ed2k_finish2 },
{ "MD4", TRUE, md4_init, md4_update, md4_finish },
{ "MD5", TRUE, md5_init, md5_update, md5_finish },
{ "SHA1", TRUE, sha1_init, sha1_update, sha1_finish },
{ "STR-AP", FALSE, string_init, str_ap_update, string_finish },
{ "STR-BKDR", FALSE, string_init, str_bkdr_update, string_finish },
{ "STR-BP", FALSE, string_init, str_bp_update, string_finish },
{ "STR-DEK", FALSE, string_init, str_dek_update, string_finish },
{ "STR-DJB2", FALSE, string_init, str_djb2_update, string_finish },
{ "STR-ELF", FALSE, string_init, str_elf_update, string_finish },
{ "STR-FNV", FALSE, string_init, str_fnv_update, string_finish },
{ "STR-JS", FALSE, string_init, str_js_update, string_finish },
{ "STR-RS", FALSE, string_init, str_rs_update, string_finish },
{ "STR-SDBM", FALSE, string_init, str_sdbm_update, string_finish },
{ "XOR-8", TRUE, xor8_init, xor8_update, xor8_finish },
{ NULL, FALSE, NULL, NULL, NULL }};
