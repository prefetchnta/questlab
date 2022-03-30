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
        HTTP 下载列表辅助工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    FILE*   fpb;
    FILE*   fpu;
    sINIu*  ini;
    leng_t  len;
    ansi_t* bat;
    ansi_t* url;
    ansi_t* str;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 <列表文件> <URL 根目录> */
    if (argc < 3) {
        printf("httpcopy <list.txt> <url_root>\n");
        return (QST_ERROR);
    }

    /* 必须是有效的 URL 根 */
    len = str_lenA(argv[2]);
    if (len <= 7 || (argv[2])[len - 1] != '/') {
        printf("invalid URL root\n");
        return (QST_ERROR);
    }

    /* 读入列表文件 */
    str = file_load_as_strA(argv[1]);
    if (str == NULL) {
        printf("file_load_as_strA() failure\n");
        return (QST_ERROR);
    }
    ini = ini_parseU(str);
    mem_free(str);
    if (ini == NULL) {
        printf("ini_parseU() failure\n");
        return (QST_ERROR);
    }

    /* 合成输出文件 */
    bat = str_fmtA("%s.bat", argv[1]);
    url = str_fmtA("%s.lst", argv[1]);
    if (bat == NULL || url == NULL) {
        printf("str_fmtA() failure\n");
        return (QST_ERROR);
    }
    fpb = fopen(bat, "wb");
    fpu = fopen(url, "wb");
    mem_free(bat);
    mem_free(url);
    if (fpb == NULL || fpu == NULL) {
        printf("fopen() failure\n");
        return (QST_ERROR);
    }

    /* 逐行处理列表 */
    for (leng_t idx = 0; idx < ini->count; idx++)
    {
        ansi_t* line = ini->lines[idx];

        /* 跳过空行 */
        str = skip_spaceA(line);
        if (*str == 0)
            continue;

        /* 创建下级目录 */
        path_buildA(line);

        /* 输出文件搬移脚本 */
        str = str_dupA(line);
        if (str == NULL) {
            printf("str_dupA() failure\n");
            return (QST_ERROR);
        }
        path_extractA(str, line);
        len = str_lenA(str);
        if (len != 0 && line[len] != 0) {
            url = str_fmtA("@move /y \"%s\" \"%s\"\r\n", &line[len], str);
            if (url == NULL) {
                fprintf(fpb, "@move /y \"%s\" \"%s\"\r\n", &line[len], str);
            }
            else {
                len = str_lenA(url);
                for (leng_t jj = 0; jj < len; jj++) {
                    if (url[jj] == '%') {
                        fputc('%', fpb);
                        fputc('%', fpb);
                    }
                    else {
                        fputc(url[jj], fpb);
                    }
                }
                mem_free(url);
            }
            fflush(fpb);
        }
        mem_free(str);

        /* URL 转义 */
        len = str_lenA(line);
        for (leng_t ii = 0; ii < len; ii++) {
            if (line[ii] == '\\')
                line[ii] = '/';
        }
        url = url_encode(line, FALSE);
        if (url == NULL) {
            printf("url_encode() failure\n");
            return (QST_ERROR);
        }

        /* 合成完整 URL 串 */
        str = str_fmtA("%s%s", argv[2], url);
        mem_free(url);
        if (str == NULL) {
            printf("str_fmtA() failure\n");
            return (QST_ERROR);
        }
        fprintf(fpu, "%s\r\n", str);
        fflush(fpu);
        mem_free(str);
    }
    ini_closeU(ini);
    fclose(fpb);
    fclose(fpu);
    return (QST_OKAY);
}
