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
 *             ##       CREATE: 2015-03-23
 *              #
 ================================================
        文件哈希校验小工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 系统内存参数 */
static void_t*  s_rdata;
static msize_t  s_total;
static msize_t  s_avail;

/* 文件分块大小 */
#define CACHE_BLOCK CR_M2B(2)
#define FREAD_BLOCK CR_M2B(128)

/* 哈希的上下文 */
typedef struct
{
        sMD5    md5c;
        sSHA1   sha1;
        sED2K   ed2k;
        int32u  crc4;
} sHASH;

/* 性能测试对象 */
static xtime_t  s_profile;

/*
---------------------------------------
    初始化哈希
---------------------------------------
*/
static void_t hash_init (sHASH *hash)
{
    hash_md5_init (&hash->md5c);
    hash_sha1_init(&hash->sha1);
    hash_ed2k_init(&hash->ed2k);
    hash->crc4 = hash_crc32i_init();
}

/*
---------------------------------------
    哈希数据块
---------------------------------------
*/
static void_t hash_update (sHASH *hash, const void_t *data, leng_t size)
{
    leng_t blk = size / CACHE_BLOCK;
    leng_t rst = size % CACHE_BLOCK;

    /* 逐块哈希数据 */
    for (; blk != 0; blk--) {
        hash_md5_update (&hash->md5c, data, CACHE_BLOCK);
        hash_sha1_update(&hash->sha1, data, CACHE_BLOCK);
        hash_ed2k_update(&hash->ed2k, data, CACHE_BLOCK);
        hash->crc4 = hash_crc32i_update(hash->crc4, data, CACHE_BLOCK);
        data = (const byte_t*)data + CACHE_BLOCK;
        printf("..");
    }

    /* 哈希剩余数据 */
    if (rst != 0) {
        hash_md5_update (&hash->md5c, data, (int32u)rst);
        hash_sha1_update(&hash->sha1, data, (leng_t)rst);
        hash_ed2k_update(&hash->ed2k, data, (leng_t)rst);
        hash->crc4 = hash_crc32i_update(hash->crc4, data, rst);
        printf(".");
    }
}

/*
---------------------------------------
    获取哈希值
---------------------------------------
*/
static void_t hash_finish (fbuf_t file, fsize_t size,
                const ansi_t *name, sHASH *hash)
{
    leng_t ssize;
    int32u crc32;
    void_t *result;
    byte_t md5c[16], ed2k[16], sha1[20];
    ansi_t md5cs[33], ed2ks[33], sha1s[41];

    hash_md5_finish  (md5c, &hash->md5c);
    hash_sha1_finish (sha1, &hash->sha1);
    hash_ed2k_finish1(ed2k, &hash->ed2k);
    crc32 = hash_crc32i_finish(hash->crc4);

    hex2strA(md5cs, md5c, 16);
    hex2strA(sha1s, sha1, 20);
    hex2strA(ed2ks, ed2k, 16);
    result = str_fmtX(CR_UTF8, &ssize, "%s\r\n%s\r\n"\
                "%s + %08" CR_I32 "X\r\n%s | %" CR_FSZ "u\r\n\r\n",
                       name, sha1s, md5cs, crc32, ed2ks, size);
    file_buf_write(result, ssize - 1, file);
    file_buf_flush(file);
    mem_free(result);
    printf(" [OK]");
}

/*
---------------------------------------
    文件处理回调
---------------------------------------
*/
static bool_t hasher (void_t *param, sSEARCHa *info)
{
    sHASH hash;

    /* 过滤掉两个生成的文件 */
    if (str_cmpA(info->name, "__hash__.old") == 0 ||
        str_cmpA(info->name, "__hash__.txt") == 0)
        return (TRUE);

    timer_set_base(s_profile);

    /* 显示文件名和大小字节数 */
    printf("%s (%" CR_FSZ "u Bytes) ", info->name, info->size);

    /* 根据内存大小选择读取方式 */
    if (info->size == 0)
    {
        /* 空文件 */
        hash_init(&hash);
        hash_update(&hash, NULL, 0);
        hash_finish(param, info->size, info->name, &hash);
    }
    else
    if (info->size <= s_total)
    {
        sVFILE file;
        void_t* data;

        /* 内存映射 */
        data = file_mappingA(info->name, &file);
        if (data == NULL)
            goto _read_it;
        hash_init(&hash);
        hash_update(&hash, data, (leng_t)info->size);
        hash_finish(param, info->size, info->name, &hash);
        file_release(&file);
    }
    else
    {
        fraw_t file;
        leng_t rest;
        fsize_t blks;

_read_it:   /* 分块读取 */
        file = file_raw_openA(info->name, CR_FO_RO | CR_FO_SEQ);
        if (file == NULL)
            goto _failure;

        /* 文件很大, 只能分块读取 */
        if (s_rdata == NULL)
            s_rdata = mem_malloc(FREAD_BLOCK);
        rest = ( leng_t)(info->size % FREAD_BLOCK);
        blks = (fsize_t)(info->size / FREAD_BLOCK);
        for (hash_init(&hash); blks != 0; blks--) {
            if (file_raw_read(s_rdata, FREAD_BLOCK, file) != FREAD_BLOCK) {
                file_raw_close(file);
                goto _failure;
            }
            hash_update(&hash, s_rdata, FREAD_BLOCK);
        }
        if (rest != 0) {
            if (file_raw_read(s_rdata, rest, file) != rest) {
                file_raw_close(file);
                goto _failure;
            }
            hash_update(&hash, s_rdata, rest);
        }
        hash_finish(param, info->size, info->name, &hash);
        file_raw_close(file);
    }

    fp32_t  time;

    time = timer_get_delta(s_profile);
    time *= 1.024f;
    printf(" %.2f KB/S\n", info->size / time);
    return (TRUE);

_failure:
    printf(" [FAILED]\n");
    return (TRUE);
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    fbuf_t file;
    const ansi_t *root;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 [搜索根目录] */
    if (argc < 1) {
        printf("filehash [directory]\n");
        return (QST_ERROR);
    }

    /* 生成性能测试对象 */
    s_profile = timer_new();

    /* 生成输出文件 */
    file_deleteA("__hash__.old");
    file_renameA("__hash__.old", "__hash__.txt");
    file = file_buf_openA("__hash__.txt", CR_FO_WO);
    if (file == NULL) {
        printf("can't create output file\n");
        return (QST_ERROR);
    }

    /* 枚举所有文件 */
    if (argc == 1)
        root = "";
    else
        root = argv[1];
    s_rdata = NULL;
    mem_info(&s_total, &s_avail);
    s_total = (s_total * 618) / 1000;
    file_searchA(root, TRUE, FALSE, FALSE, "*.*", hasher, file);

    /* 释放内存 */
    file_buf_close(file);
    timer_del(s_profile);
    TRY_FREE(s_rdata);
    return (QST_OKAY);
}
