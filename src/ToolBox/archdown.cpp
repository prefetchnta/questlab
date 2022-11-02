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
 *             ##       CREATE: 2022-03-24
 *              #
 ================================================
        ARCHIVE.ORG 下载工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

#include <conio.h>

/* 临时缓冲 */
void_t* s_buffer = NULL;

/* 分块大小 */
#define BLOCK_SIZE  CR_M2B(128)

/* 文字颜色属性 */
static int16u   s_clr_back;
static int16u   s_clr_file;
static int16u   s_clr_info;
static int16u   s_clr_okay;
static int16u   s_clr_fail;

/*
---------------------------------------
    打印文件大小
---------------------------------------
*/
static void_t
print_file_size (
  __CR_IN__ int64u  size
    )
{
    if (size >= 1024 * 1024 * 1024)
        printf("%.2f GB", (double)size / 1024 / 1024 / 1024);
    else if (size >= 1024 * 1024)
        printf("%.2f MB", (double)size / 1024 / 1024);
    else
        printf("%.2f KB", (double)size / 1024);
}

/*
---------------------------------------
    文件 SHA1 哈希
---------------------------------------
*/
static void_t
do_file_hash_sha (
  __CR_IN__ const ansi_t*   dir,
  __CR_OT__ byte_t*         hash
    )
{
    sSHA1   ctx;
    fraw_t  fpp;
    leng_t  bck;
    fsize_t tot = file_sizeA(dir);
    fsize_t blk = tot / BLOCK_SIZE;
    fsize_t rst = tot % BLOCK_SIZE;

    cui_set_color(s_clr_info);
    printf("Hashing (SHA1): ");
    cui_set_color(s_clr_fail);
    printf("%s ", dir);
    cui_set_color(s_clr_back);

    mem_zero(hash, 20);
    fpp = file_raw_openA(dir, CR_FO_RO | CR_FO_SEQ);
    if (fpp == NULL)
        return;
    hash_sha1_init(&ctx);
    for (fsize_t idx = 0; idx < blk; idx++) {
        bck = file_raw_read(s_buffer, BLOCK_SIZE, fpp);
        if (bck != BLOCK_SIZE) {
            file_raw_close(fpp);
            return;
        }
        hash_sha1_update(&ctx, s_buffer, bck);
        printf(".");
    }
    if (rst != 0) {
        bck = file_raw_read(s_buffer, (leng_t)rst, fpp);
        if (bck != (leng_t)rst) {
            file_raw_close(fpp);
            return;
        }
        hash_sha1_update(&ctx, s_buffer, bck);
        printf(".");
    }
    printf("\n");
    file_raw_close(fpp);
    hash_sha1_finish(hash, &ctx);
}

/*
---------------------------------------
    文件 MD5 哈希
---------------------------------------
*/
static void_t
do_file_hash_md5 (
  __CR_IN__ const ansi_t*   dir,
  __CR_OT__ byte_t*         hash
    )
{
    sMD5    ctx;
    fraw_t  fpp;
    leng_t  bck;
    fsize_t tot = file_sizeA(dir);
    fsize_t blk = tot / BLOCK_SIZE;
    fsize_t rst = tot % BLOCK_SIZE;

    cui_set_color(s_clr_info);
    printf("Hashing (MD5): ");
    cui_set_color(s_clr_fail);
    printf("%s ", dir);
    cui_set_color(s_clr_back);

    mem_zero(hash, 16);
    fpp = file_raw_openA(dir, CR_FO_RO | CR_FO_SEQ);
    if (fpp == NULL)
        return;
    hash_md5_init(&ctx);
    for (fsize_t idx = 0; idx < blk; idx++) {
        bck = file_raw_read(s_buffer, BLOCK_SIZE, fpp);
        if (bck != BLOCK_SIZE) {
            file_raw_close(fpp);
            return;
        }
        hash_md5_update(&ctx, s_buffer, bck);
        printf(".");
    }
    if (rst != 0) {
        bck = file_raw_read(s_buffer, (leng_t)rst, fpp);
        if (bck != (leng_t)rst) {
            file_raw_close(fpp);
            return;
        }
        hash_md5_update(&ctx, s_buffer, bck);
        printf(".");
    }
    printf("\n");
    file_raw_close(fpp);
    hash_md5_finish(hash, &ctx);
}

/*
---------------------------------------
    文件 CRC32 哈希
---------------------------------------
*/
static int32u
do_file_hash_crc (
  __CR_IN__ const ansi_t*   dir
    )
{
    int32u  hsh;
    fraw_t  fpp;
    leng_t  bck;
    fsize_t tot = file_sizeA(dir);
    fsize_t blk = tot / BLOCK_SIZE;
    fsize_t rst = tot % BLOCK_SIZE;

    cui_set_color(s_clr_info);
    printf("Hashing (CRC32): ");
    cui_set_color(s_clr_fail);
    printf("%s ", dir);
    cui_set_color(s_clr_back);

    fpp = file_raw_openA(dir, CR_FO_RO | CR_FO_SEQ);
    if (fpp == NULL)
        return (0);
    hsh = hash_crc32i_init();
    for (fsize_t idx = 0; idx < blk; idx++) {
        bck = file_raw_read(s_buffer, BLOCK_SIZE, fpp);
        if (bck != BLOCK_SIZE) {
            file_raw_close(fpp);
            return (0);
        }
        hsh = hash_crc32i_update(hsh, s_buffer, bck);
        printf(".");
    }
    if (rst != 0) {
        bck = file_raw_read(s_buffer, (leng_t)rst, fpp);
        if (bck != (leng_t)rst) {
            file_raw_close(fpp);
            return (0);
        }
        hsh = hash_crc32i_update(hsh, s_buffer, bck);
        printf(".");
    }
    printf("\n");
    file_raw_close(fpp);
    return (hash_crc32i_finish(hsh));
}

/*
---------------------------------------
    执行文件下载
---------------------------------------
*/
static bool_t
do_file_download (
  __CR_IN__ const ansi_t*   url,
  __CR_IN__ const ansi_t*   root,
  __CR_IN__ const ansi_t*   path,
  __CR_IN__ const ansi_t*   proxy
    )
{
    leng_t  idx;
    leng_t  len;
    ansi_t* dir;
    ansi_t* pth;
    ansi_t* cmd;

    /* 拼装下载命令行 */
    dir = str_dupA(root);
    if (dir == NULL)
        return (FALSE);
    len = str_lenA(dir);
    for (idx = 0; idx < len; idx++) {
        if (dir[idx] == '\\')
            dir[idx] = '/';
    }
    pth = str_dupA(path);
    if (pth == NULL) {
        mem_free(dir);
        return (FALSE);
    }
    len = str_lenA(pth);
    for (idx = 0; idx < len; idx++) {
        if (pth[idx] == '\\')
            pth[idx] = '/';
    }
    if (proxy != NULL) {
        cmd = str_fmtA("aria2c \"--dir=%s\" \"--out=%s\" --all-proxy=%s"
                             " \"%s\"", dir, pth, proxy, url);
    }
    else {
        cmd = str_fmtA("aria2c \"--dir=%s\" \"--out=%s\" \"%s\"",
                                        dir, pth, url);
    }
    mem_free(dir);
    mem_free(pth);
    if (cmd == NULL)
        return (FALSE);

    FILE*   fp;
    bool_t  rett;

    /* 保存执行的命令到文件 */
    fp = fopen("__cmds__.txt", "ab+");
    if (fp != NULL) {
        fprintf(fp, "%s\r\n", cmd);
        fclose(fp);
    }

    /* 执行下载命令直到退出 */
    rett = misc_call_exe(cmd, TRUE, FALSE);
    mem_free(cmd);
    return (rett);
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    FILE*   fp;
    sXMLu*  xml;
    leng_t  len;
    leng_t  tot;
    int64u  dsk;
    ansi_t* str;
    ansi_t* proxy;
    ansi_t* web_root;
    ansi_t* dsk_root;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 颜色属性合成 */
    s_clr_back = cui_make_attr(0, CR_CUI_TEXT_GREEN | CR_CUI_TEXT_BLUE |
                                  CR_CUI_TEXT_RED);
    s_clr_file = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_GREEN |
                                  CR_CUI_TEXT_RED);
    s_clr_info = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_GREEN |
                                  CR_CUI_TEXT_BLUE  | CR_CUI_TEXT_RED);
    s_clr_okay = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_GREEN);
    s_clr_fail = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_RED);

    /* 参数解析 <XML 文件> <URL 根目录> [输出目录] [代理] */
    if (argc < 3) {
        printf("archdown <list.xml> <url_root> [dir] [proxy]\n");
        return (QST_ERROR);
    }

    /* 必须是有效的 URL 根 */
    len = str_lenA(argv[2]);
    if (len <= 7 || (argv[2])[len - 1] != '/') {
        printf("invalid URL root\n");
        return (QST_ERROR);
    }
    web_root = argv[2];

    /* 输出目录的根 */
    if (argc > 3) {
        len = str_lenA(argv[3]);
        if (len <= 1 || (argv[3])[len - 1] != '\\') {
            printf("invalid DIR root\n");
            return (QST_ERROR);
        }
        dsk_root = argv[3];
    }
    else {
        dsk_root = ".\\";
    }

    /* 代理服务器字符串 */
    if (argc > 4)
        proxy = argv[4];
    else
        proxy = NULL;

    /* 读入 XML 列表文件 */
    str = file_load_as_strA(argv[1]);
    if (str == NULL) {
        printf("file_load_as_strA() failure\n");
        return (QST_ERROR);
    }

    /* XML 标签解析 */
    xml = xml_parseU(str);
    mem_free(str);
    if (xml == NULL) {
        printf("xml_parseU() failure\n");
        return (QST_ERROR);
    }
    s_buffer = mem_malloc(BLOCK_SIZE);
    if (s_buffer == NULL) {
        printf("mem_malloc() failure\n");
        return (QST_ERROR);
    }

    int32u  crc = 0;
    int64u  fsize = 0;
    ansi_t  *pth = NULL;
    byte_t  md5[16], sha[20];
    bool_t  have_crc, have_md5, have_sha;

    /* 节点信息初始化 */
    mem_zero(md5, sizeof(md5));
    mem_zero(sha, sizeof(sha));
    have_crc = have_md5 = have_sha = FALSE;
    printf("======================================================\n");
    dsk = tot = 0;

    /* 逐个标签处理 */
    for (leng_t idx = 0; idx < xml->count; idx++)
    {
        sXNODEu*    node = &xml->nodes[idx];

        /* 文件路径 */
        if (pth == NULL) {
            if (str_cmpIA(node->name, "file") != 0)
                continue;
            pth = xml_attr_stringU("name", node);
            if (pth == NULL) {
                printf("xml_attr_stringU() failure\n");
                return (QST_ERROR);
            }
            /* 转义处理 */
            str = html_to_stringU(pth);
            mem_free(pth);
            if (str == NULL) {
                printf("html_to_stringU() failure\n");
                return (QST_ERROR);
            }
            pth = str;
            continue;
        }

        /* 子节点处理 */
        if (str_cmpIA(node->name, "size") == 0)
        {
            /* 文件大小 */
            fsize = str2int64A(node->node);
        }
        else
        if (str_cmpIA(node->name, "sha1") == 0)
        {
            /* 文件 SHA1 */
            len = sizeof(sha);
            str2datA(sha, &len, node->node);
            if (len != sizeof(sha)) {
                printf("invalid SHA1 node data\n");
                return (QST_ERROR);
            }
            have_sha = TRUE;
        }
        else
        if (str_cmpIA(node->name, "md5") == 0)
        {
            /* 文件 MD5 */
            len = sizeof(md5);
            str2datA(md5, &len, node->node);
            if (len != sizeof(md5)) {
                printf("invalid MD5 node data\n");
                return (QST_ERROR);
            }
            have_md5 = TRUE;
        }
        else
        if (str_cmpIA(node->name, "crc32") == 0)
        {
            /* 文件 CRC32 */
            len = sizeof(crc);
            str2datA(&crc, &len, node->node);
            if (len != sizeof(crc)) {
                printf("invalid CRC32 node data\n");
                return (QST_ERROR);
            }
            crc = xchg_int32u(crc);
            have_crc = TRUE;
        }
        else
        if (str_cmpIA(node->name, "/file") == 0)
        {
            bool_t  ex1;
            bool_t  ex2;
            ansi_t* dir;
            ansi_t* url;
            sint_t  upd;
            byte_t  tmp[20];
            uint_t  tim = 0;
            bool_t  dwn = FALSE;

            /* 没有文件大小直接跳过 */
            if (fsize == 0 || filext_checkA(pth, ".torrent"))
                goto _next;
            tot += 1;
            dsk += fsize;

            /* 拼装远程和本地路径 */
            dir = str_fmtA("%s%s", dsk_root, pth);
            if (dir == NULL) {
                printf("str_fmtA() failure\n");
                return (QST_ERROR);
            }
            path_uniqueA(dir);
            url = str_fmtA("%s%s", web_root, pth);
            if (url == NULL) {
                printf("str_fmtA() failure\n");
                return (QST_ERROR);
            }
_retry:
            /* 用户主动退出 */
            if (_kbhit()) {
                if (getch() == 27) {
                    cui_set_color(s_clr_okay);
                    printf("!!!!!!!! USER QUIT !!!!!!!!\n");
                    cui_set_color(s_clr_back);
                    break;
                }
            }

            /* 创建目标路径 */
            ex1 = file_existA(dir);
            if (!ex1)
                path_buildA(dir);

            /* 判断目标文件是否下载完 */
            str = str_fmtA("%s.aria2", dir);
            if (str == NULL) {
                printf("str_fmtA() failure\n");
                return (QST_ERROR);
            }
            ex2 = file_existA(str);
            cui_set_color(s_clr_file);
            if (!ex1 || ex2)
            {
                /* 文件未下载完继续下载 */
                printf("Downloading: ");
                cui_set_color(s_clr_okay);
                printf("%s\n", pth);
                if (!do_file_download(url, dsk_root, pth, proxy)) {
                    printf("do_file_download() failure\n");
                    return (QST_ERROR);
                }

                /* 判断是否下完, 否则需要重试 */
                if (!file_existA(dir) || file_existA(str)) {
                    mem_free(str);
                    thread_sleep(8888 + tim);
                    tim += 2222;
                    goto _retry;
                }
                dwn = TRUE;
            }
            else
            {
                /* 文件已经存在, 无需下载 */
                printf("Skiping: ");
                cui_set_color(s_clr_okay);
                printf("%s\n", pth);
            }
            mem_free(str);

            /* 文件已存在, 需要判断是否需要更新 */
            if (fsize != file_sizeA(dir)) {
                upd = 1;
            }
            else {
                mem_zero(tmp, sizeof(tmp));
                if (have_sha) {
                    do_file_hash_sha(dir, tmp);
                    upd = (mem_cmp(tmp, sha, 20) == 0) ? 0 : 1;
                }
                else
                if (have_md5) {
                    do_file_hash_md5(dir, tmp);
                    upd = (mem_cmp(tmp, md5, 16) == 0) ? 0 : 1;
                }
                else
                if (have_crc) {
                    upd = (crc == do_file_hash_crc(dir)) ? 0 : 1;
                }
                else {
                    upd = -1;
                }
            }

            /* 根据类型执行动作 */
            if (upd > 0)
            {
                /* 文件校验不同, 需要更新 */
                cui_set_color(s_clr_info);
                printf("[");
                cui_set_color(s_clr_fail);
                printf("FAIL");
                cui_set_color(s_clr_info);
                printf("]");
                cui_set_color(s_clr_file);
                printf(" %s | ", url);
            }
            else
            if (upd < 0)
            {
                /* 没有校验信息, 跳过 */
                cui_set_color(s_clr_info);
                printf("[");
                cui_set_color(s_clr_back);
                printf("SKIP");
                cui_set_color(s_clr_info);
                printf("]");
                cui_set_color(s_clr_file);
                printf(" %s | ", url);
            }
            else
            {
                /* 文件校验一样, 跳过 */
                cui_set_color(s_clr_info);
                printf("[");
                cui_set_color(s_clr_okay);
                printf("OKAY");
                cui_set_color(s_clr_info);
                printf("]");
                cui_set_color(s_clr_file);
                printf(" %s | ", url);
            }

            /* 打印文件大小 */
            cui_set_color(s_clr_info);
            print_file_size(fsize);

            ULARGE_INTEGER  left;

            /* 打印剩余空间大小 */
            GetDiskFreeSpaceExA(dsk_root, NULL, NULL, &left);
            cui_set_color(s_clr_file);
            printf(" [");
            cui_set_color(s_clr_fail);
            printf("DISK REST: ");
            cui_set_color(s_clr_okay);
            print_file_size(left.QuadPart);
            cui_set_color(s_clr_file);
            printf("]\n");
            cui_set_color(s_clr_back);

            /* 失败的重试 */
            if (upd <= 0)
            {
                /* 输出下载成功列表 */
                fp = fopen("__succ__.txt", "ab+");
                if (fp != NULL) {
                    fprintf(fp, "%s\r\n", pth);
                    fclose(fp);
                }

                /* 输出删除文件脚本 */
                fp = fopen("__kill__.bat", "ab+");
                if (fp != NULL) {
                    fprintf(fp, "attrib +R \"%s\"\r\n", pth);
                    fclose(fp);
                }

                ansi_t  show[64];

                /* 输出 HaSHer 文件 */
                fp = fopen("HaSHer-SHA.txt", "ab+");
                if (fp != NULL) {
                    mem_zero(show, sizeof(show));
                    if (have_sha)
                        hex2strA(show, sha, sizeof(sha));
                    else
                        mem_set(show, '?', sizeof(sha) * 2);
                    fprintf(fp, "%s + ", show);
                    if (have_crc)
                        fprintf(fp, "%08X: ", crc);
                    else
                        fprintf(fp, "????????: ");
                    fprintf(fp, "%s\r\n", pth);
                    fclose(fp);
                }
                fp = fopen("HaSHer-MD5.txt", "ab+");
                if (fp != NULL) {
                    mem_zero(show, sizeof(show));
                    if (have_md5)
                        hex2strA(show, md5, sizeof(md5));
                    else
                        mem_set(show, '?', sizeof(md5) * 2);
                    fprintf(fp, "%s + ", show);
                    if (have_crc)
                        fprintf(fp, "%08X: ", crc);
                    else
                        fprintf(fp, "????????: ");
                    fprintf(fp, "%s\r\n", pth);
                    fclose(fp);
                }
            }
            else
            {
                /* 删除文件重新开始 */
                if (!dwn) {
                    file_deleteA(dir);
                    tim = 0;
                    printf("-----------------------------\n");
                    goto _retry;
                }

                /* 下载完成但文件内容不对 */
                fp = fopen("__fail__.txt", "ab+");
                if (fp != NULL) {
                    fprintf(fp, "%s\r\n", pth);
                    fclose(fp);
                }
            }
            printf("======================================================\n");

            /* 释放内存 */
            mem_free(url);
            mem_free(dir);

            /* 磁盘空间不够则退出 */
            if (left.QuadPart < CR_M2B(16) * 1024ULL) {
                cui_set_color(s_clr_fail);
                printf("!!!!!!!! DISK FULL !!!!!!!!\n");
                cui_set_color(s_clr_back);
                mem_free(pth);
                break;
            }
_next:
            /* 下一个节点 */
            have_crc = FALSE;
            have_md5 = FALSE;
            have_sha = FALSE;
            fsize = 0;
            mem_free(pth);
            pth = NULL;
        }
    }

    /* 输出删除文件脚本 */
    fp = fopen("__kill__.bat", "ab+");
    if (fp != NULL) {
        fprintf(fp, "attrib +R __kill__.bat\r\n");
        fprintf(fp, "del /Q /S *.*\r\n");
        fprintf(fp, "attrib -R *.* /S\r\n");
        fclose(fp);
    }

    /* 打印文件数量 */
    printf("Total file count: %u, file size: ", tot);
    print_file_size(dsk);
    printf("\n");

    /* 释放内存 */
    xml_closeU(xml);
    mem_free(s_buffer);
    return (QST_OKAY);
}
