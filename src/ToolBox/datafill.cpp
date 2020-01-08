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
 *             ##       CREATE: 2020-01-08
 *              #
 ================================================
        数据填充校验小工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 系统内存参数 */
static void_t*  s_rdata;
static msize_t  s_total;
static msize_t  s_avail;

/* 文件分块大小 */
#define FREAD_BLOCK CR_M2B(128)

/* 性能测试对象 */
static xtime_t  s_profile;

/*
---------------------------------------
    数据校验回调
---------------------------------------
*/
static bool_t checker (void_t *param, sSEARCHa *info)
{
    sMD5    ctx;
    leng_t  idx, len;
    byte_t  md5src[16];
    byte_t  md5dst[16];

    CR_NOUSE(param);

    /* 根据文件名过滤 */
    len = str_lenA(info->name);
    if (len < 32)
        return (TRUE);
    str_uprA(info->name);
    for (idx = 0; idx < 32; idx++) {
        if (!is_xnumbA(info->name[len - 32 + idx]))
            return (TRUE);
    }

    /* 提取哈希值 */
    idx = len - 32;
    len = sizeof(md5src);
    str2datA(md5src, &len, &info->name[idx]);
    if (len != sizeof(md5src))
        return (TRUE);

    /* 显示文件名 */
    printf("%s\t", &info->name[idx]);

    /* 根据内存大小选择读取方式 */
    timer_set_base(s_profile);
    if (info->size == 0)
    {
        /* 空文件 */
        hash_md5_total(md5dst, NULL, 0);
    }
    else
    if (info->size <= s_total)
    {
        sVFILE file;
        void_t *data;

        /* 内存映射 */
        data = file_mappingA(info->name, &file);
        if (data == NULL)
            goto _read_it;
        hash_md5_total(md5dst, data, (leng_t)info->size);
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
        for (hash_md5_init(&ctx); blks != 0; blks--) {
            if (file_raw_read(s_rdata, FREAD_BLOCK, file) != FREAD_BLOCK) {
                file_raw_close(file);
                goto _failure;
            }
            hash_md5_update(&ctx, s_rdata, FREAD_BLOCK);
        }
        if (rest != 0) {
            if (file_raw_read(s_rdata, rest, file) != rest) {
                file_raw_close(file);
                goto _failure;
            }
            hash_md5_update(&ctx, s_rdata, rest);
        }
        hash_md5_finish(md5dst, &ctx);
        file_raw_close(file);
    }

    fp32_t  time;

    /* 计算哈希耗时 */
    time = timer_get_delta(s_profile);
    time *= 1.024f;
    time = info->size / time;

    /* 判断是否一致 */
    if (mem_cmp(md5dst, md5src, sizeof(md5src)) != 0) {
        printf("[FAIL] %.2f KB/S\n", time);
        return (FALSE);
    }
    printf("[OKAY] %.2f KB/S\n", time);
    return (TRUE);

_failure:
    printf("[FAIL]\n");
    return (FALSE);
}

/*
---------------------------------------
    字符串转字节大小
---------------------------------------
*/
static int64u str2size (const ansi_t *str)
{
    int64u  size;
    leng_t  skip;

    size = str2intx64A(str, &skip);
    str += skip;
    if      (*str == 'K' || *str == 'k') size *= 1024;
    else if (*str == 'M' || *str == 'm') size *= 1024 * 1024;
    else if (*str == 'G' || *str == 'g') size *= 1024 * 1024 * 1024;
    return (size);
}

/*
---------------------------------------
    写入随机数据
---------------------------------------
*/
static bool_t write_data (int64u size)
{
    sMD5    ctx;
    FILE*   fpp;
    ansi_t  name[33];
    byte_t  cha, md5[16];

    /* 写入临时文件 */
    fpp = fopen("DATAFILL.TMP", "wb");
    if (fpp == NULL)
        return (FALSE);
    if (s_rdata == NULL)
        s_rdata = mem_malloc(FREAD_BLOCK);
    if (setvbuf(fpp, (char*)s_rdata, _IOFBF, FREAD_BLOCK) != 0) {
        fclose(fpp);
        return (FALSE);
    }
    hash_md5_init(&ctx);
    timer_set_base(s_profile);
    for (int64u idx = 0; idx < size; idx++) {
        cha = (byte_t)rand_get();
        hash_md5_update(&ctx, &cha, sizeof(cha));
        if (fwrite(&cha, 1, 1, fpp) != 1) {
            fclose(fpp);
            return (FALSE);
        }
    }
    hash_md5_finish(md5, &ctx);
    fclose(fpp);

    fp32_t  time;

    /* 计算写入耗时 */
    time = timer_get_delta(s_profile);
    time *= 1.024f;
    time = size / time;

    /* 更名到哈希值 */
    hex2strA(name, md5, sizeof(md5));
    printf("%s\t%.2f KB/S\n", name, time);
    return (file_renameA(name, "DATAFILL.TMP"));
}

/*
---------------------------------------
    填充数据的生成
---------------------------------------
*/
static bool_t fill_data (int64u *total, int64u min_size, int64u max_size)
{
    int64u  size;

    /* 确定大小 */
    if (min_size == max_size)
    {
        /* 固定大小 */
        size = min_size;
    }
    else
    {
        /* 随机大小 */
        size = rand_get() & 0x0F;
        for (uint_t idx = 0; idx < 64 / 16; idx++) {
            size <<= 15;
            size |= rand_get() & 0x7FFF;
        }
        size %= max_size - min_size;
        size += min_size;
    }
    if (size > *total)
        size = *total;

    /* 写入数据 */
    *total -= size;
    rand_seed(timer_get32());
    return (write_data(size));
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    const ansi_t *root;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 [根目录] [总大小] [最小大小] [最大大小] */
    if (argc < 1) {
        printf("datafill [directory] [total size] [min size] [max size]\n");
        return (QST_ERROR);
    }

    /* 生成性能测试对象 */
    rand_seed(timer_get32());
    s_profile = timer_new();

    /* 枚举所有文件 */
    if (argc == 1)
        root = "";
    else
        root = argv[1];
    s_rdata = NULL;
    mem_info(&s_total, &s_avail);
    s_total = (s_total * 618) / 1000;
    if (argc <= 2)
    {
        /* 数据校验模式 */
        file_searchA(root, FALSE, TRUE, FALSE, "*.*", checker, NULL);
    }
    else
    {
        int64u  tot_size = 0, min_size = 0, max_size = 0;

        /* 数据填充模式 */
        if (argc > 2) {
            tot_size = str2size(argv[2]);
            if (argc > 3) {
                min_size = str2size(argv[3]);
                if (argc > 4)
                    max_size = str2size(argv[4]);
                else
                    max_size = min_size;
            }
        }

        /* 参数过滤 */
        if (min_size > max_size)
            min_size = max_size;
        if (tot_size == 0) tot_size = FREAD_BLOCK;
        if (min_size == 0) min_size = FREAD_BLOCK;
        if (max_size == 0) max_size = FREAD_BLOCK;

        /* 跳转到目标目录 */
        if (!SetCurrentDirectoryA(root)) {
            printf("invalid directory!\n");
            return (QST_ERROR);
        }

        int64u  save = tot_size;

        /* 填充指定大小的数据 */
        while (tot_size != 0) {
            if (!fill_data(&tot_size, min_size, max_size)) {
                printf("fill data error: %" CR_I64 "u!\n", save - tot_size);
                return (QST_ERROR);
            }
        }
    }

    /* 释放内存 */
    timer_del(s_profile);
    TRY_FREE(s_rdata);
    return (QST_OKAY);
}
