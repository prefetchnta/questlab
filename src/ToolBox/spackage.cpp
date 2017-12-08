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
 *             ##       CREATE: 2017-12-05
 *              #
 ================================================
        简易文件打包小工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 用来传递参数 */
typedef struct
{
        sARRAY  filelist;   /* 文件信息列表 */

} sSP_PARAM;

/*
---------------------------------------
    文件处理回调
---------------------------------------
*/
static bool_t do_each_file (void_t *param, sSEARCHa *finfo)
{
    sSP_PARAM*  ctx = (sSP_PARAM*)param;

    /* 打印文件信息并加入文件列表 */
    if (finfo->size == 0 || finfo->size > CR_M2B(64))
        return (TRUE);
    if (array_push_growT(&ctx->filelist, sSEARCHa, finfo) != NULL) {
        if (finfo->size <= 1024UL)
            printf("%s [%I64u B]\n", finfo->name, finfo->size);
        else
        if (finfo->size <= 1048576UL)
            printf("%s [%.2f KB]\n", finfo->name, finfo->size / 1024.0f);
        else
            printf("%s [%.2f MB]\n", finfo->name, finfo->size / 1048576.0f);
    }
    return (TRUE);
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 <打包文件> <搜索根目录> <通配符> [字符编码] */
    if (argc < 4) {
        printf("spackage <file.spak> <root> <match> [codepage]\n");
        return (QST_ERROR);
    }

    uint_t      cp;
    sSP_PARAM   ft;

    /* 参数初始化 */
    if (argc >= 5)
        cp = str2intA(argv[4]);
    else
        cp = get_sys_codepage();
    array_initT(&ft.filelist, sSEARCHa);

    leng_t      beg;
    leng_t      cnt;
    sSEARCHa*   lst;

    /* 打包的时候跳过绝对目录 */
    beg = str_lenA(argv[2]);
    if (!is_slashA((argv[2])[beg - 1]))
        beg += 1;

    /* 子目录递归 + 跳过隐藏目录和文件 */
    file_searchA(argv[2], TRUE, TRUE, FALSE, argv[3], do_each_file, &ft);
    cnt = array_get_sizeT(&ft.filelist, sSEARCHa);
    lst = array_get_dataT(&ft.filelist, sSEARCHa);
    printf("%u file(s) found.\n", cnt);

    FILE*   fp;

    /* 输出打包文件 */
    if (cnt != 0)
    {
        /* 有文件才输出 */
        fp = fopen(argv[1], "wb");
        if (fp != NULL)
        {
            int32u  sz;
            ansi_t* fl;

            /* 输出文件标志 */
            if (fwrite("SIMPLE_PACKAGE\0", 1, 16, fp) != 16) {
                printf("fwrite(tag) failure!\n");
                goto _failure;
            }

            /* 输出文件个数 */
            sz = (int32u)cnt;
            if (fwrite(&sz, 1, 4, fp) != 4) {
                printf("fwrite(cnt) failure!\n");
                goto _failure;
            }

            /* 输出文件信息 */
            for (leng_t idx = 0; idx < cnt; idx++)
            {
                /* 输出文件大小 */
                sz = (int32u)lst[idx].size;
                if (fwrite(&sz, 1, 4, fp) != 4) {
                    printf("fwrite(size) failure!\n");
                    goto _failure;
                }

                /* 文件名转换成 UTF-8 编码并输出 */
                fl = local_to_utf8(cp, &lst[idx].name[beg]);
                if (fl == NULL) {
                    printf("local_to_utf8() failure!\n");
                    goto _failure;
                }
                sz = (int32u)str_sizeA(fl);
                if (fwrite(fl, 1, sz, fp) != sz) {
                    mem_free(fl);
                    printf("fwrite(name) failure!\n");
                    goto _failure;
                }
                mem_free(fl);
            }

            /* 输出文件内容 */
            for (leng_t idx = 0; idx < cnt; idx++)
            {
                leng_t  size;
                void_t* data;

                /* 直接复制文件内容 */
                data = file_load_as_binA(lst[idx].name, &size);
                if (data == NULL) {
                    printf("file_load_as_binA() failure!\n");
                    goto _failure;
                }
                if (fwrite(data, 1, size, fp) != size) {
                    mem_free(data);
                    printf("fwrite(data) failure!\n");
                    goto _failure;
                }

                /* 输出 CRC32 校验 */
                sz = hash_crc32i_total(data, size);
                mem_free(data);
                if (fwrite(&sz, 1, 4, fp) != 4) {
                    printf("fwrite(crc32) failure!\n");
                    goto _failure;
                }
            }
            fclose(fp);
        }
        else
        {
            /* 出错提示 */
            printf("create file %s failed!\n", argv[1]);
        }
    }
    array_freeT(&ft.filelist, sSEARCHa);
    return (QST_OKAY);

_failure:
    fclose(fp);
    file_deleteA(argv[1]);
    array_freeT(&ft.filelist, sSEARCHa);
    return (QST_ERROR);
}
