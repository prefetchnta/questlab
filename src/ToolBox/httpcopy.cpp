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
#include "../Asylum/asylum.hpp"
using namespace asy;

/* 节点结构 */
struct fileNode
{
    int32u  count;

    void free () {}
};

/* 防重名的哈希表 */
static map_acs<fileNode>    s_fileTbl;

/* 简化代码宏 */
#define DO_FILE_NODE(_path, _root) \
\
path_buildA(_path); \
str = str_dupA(_path); \
if (str == NULL) { \
    printf("str_dupA() failure\n"); \
    return (QST_ERROR); \
} \
path_extractA(str, _path); \
len = str_lenA(str); \
if (len != 0 && _path[len] != 0) { \
    fileNode node, *ptr; \
    ansi_t tname[MAX_PATHA * 2]; \
    ansi_t *fname = &_path[len]; \
    str_cpyA(tname, fname); \
    str_lwrA(tname); \
    ptr = s_fileTbl.get(tname); \
    if (ptr == NULL) { \
        node.count = 0; \
        if (s_fileTbl.insert(tname, &node) == NULL) { \
            printf("asy::map_acs::insert() failure\n"); \
            return (QST_ERROR); \
        } \
        url = str_fmtA("@move /y \"%s\" \"%s\"\r\n", fname, str); \
    } \
    else { \
        ptr->count += 1; \
        str_cpyA(tname, fname); \
        filext_removeA(tname); \
        len = str_lenA(tname); \
        sprintf(&tname[len], ".%u", ptr->count); \
        len = str_lenA(tname); \
        filext_extractA(&tname[len], fname); \
        url = str_fmtA("@move /y \"%s\" \"%s%s\"\r\n", tname, str, fname); \
    } \
    if (url == NULL) { \
        printf("str_fmtA() failure\n"); \
        return (QST_ERROR); \
    } \
    len = str_lenA(url); \
    for (leng_t jj = 0; jj < len; jj++) { \
        if (url[jj] == '%') { \
            fputc('%', fpb); \
            fputc('%', fpb); \
        } \
        else { \
            fputc(url[jj], fpb); \
        } \
    } \
    mem_free(url); \
    fflush(fpb); \
} \
mem_free(str); \
len = str_lenA(_path); \
for (leng_t ii = 0; ii < len; ii++) { \
    if (_path[ii] == '\\') \
        _path[ii] = '/'; \
} \
if (http) { \
    url = url_encode(_path, FALSE); \
    if (url == NULL) { \
        printf("url_encode() failure\n"); \
        return (QST_ERROR); \
    } \
} \
else { \
    url = _path; \
} \
str = str_fmtA("%s%s", _root, url); \
if (http) mem_free(url); \
if (str == NULL) { \
    printf("str_fmtA() failure\n"); \
    return (QST_ERROR); \
} \
fprintf(fpu, "%s\r\n", str); \
fflush(fpu); \
mem_free(str)

/*
---------------------------------------
    XML 文件处理
---------------------------------------
*/
static int
do_xml_file (
  __CR_IN__ ansi_t*         str,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const ansi_t*   root,
  __CR_IN__ bool_t          http
    )
{
    FILE*   fpb;
    FILE*   fpu;
    FILE*   fph;
    sXMLu*  xml;
    int64u  tot;
    leng_t  len;
    ansi_t* pth;
    ansi_t* md5;
    ansi_t* crc;
    ansi_t* bat;
    ansi_t* url;

    /* XML 标签解析 */
    xml = xml_parseU(str);
    mem_free(str);
    if (xml == NULL) {
        printf("xml_parseU() failure\n");
        return (QST_ERROR);
    }

    /* 合成输出文件 */
    bat = str_fmtA("%s.bat", name);
    url = str_fmtA("%s.lst", name);
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

    /* 哈希输出文件 */
    fph = fopen("HaSHer.txt", "wb");
    if (fph == NULL) {
        printf("fopen() failure\n");
        return (QST_ERROR);
    }
    tot = 0;
    pth = NULL;
    md5 = crc = NULL;

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
            url = html_to_stringU(pth);
            mem_free(pth);
            if (url == NULL) {
                printf("html_to_stringU() failure\n");
                return (QST_ERROR);
            }
            pth = url;
        }
        else {
            if (str_cmpIA(node->name, "size") == 0)
            {
                /* 统计总大小 */
                tot += str2int64A(node->node);
            }
            else
            if (str_cmpIA(node->name, "md5") == 0)
            {
                /* 文件 MD5 */
                md5 = xml_node_stringU(node);
                if (md5 == NULL) {
                    printf("xml_node_stringU() failure\n");
                    return (QST_ERROR);
                }
                str_uprA(str_trimA(md5));
            }
            else
            if (str_cmpIA(node->name, "crc32") == 0)
            {
                /* 文件 CRC32 */
                crc = xml_node_stringU(node);
                if (crc == NULL) {
                    printf("xml_node_stringU() failure\n");
                    return (QST_ERROR);
                }
                str_uprA(str_trimA(crc));
            }
            else
            if (str_cmpIA(node->name, "/file") == 0)
            {
                /* 输出哈希列表项 */
                fprintf(fph, "%s + %s: %s\r\n", md5, crc, pth);
                fflush(fph);
                SAFE_FREE(md5);
                SAFE_FREE(crc);

                /* 创建下级目录 */
                /* 输出文件搬移脚本 */
                DO_FILE_NODE(pth, root);

                /* 下一个节点 */
                mem_free(pth);
                pth = NULL;
            }
        }
    }
    if (tot >= 1024 * 1024 * 1024)
        printf("total size: %.2f GB\n", (double)tot / 1024 / 1024 / 1024);
    else if (tot >= 1024 * 1024)
        printf("total size: %.2f MB\n", (double)tot / 1024 / 1024);
    else
        printf("total size: %.2f KB\n", (double)tot / 1024);
    xml_closeU(xml);
    fclose(fph);
    fclose(fpb);
    fclose(fpu);
    s_fileTbl.free();
    return (QST_OKAY);
}

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

    /* 参数解析 <列表文件> <URL 根目录> [是否 URL 转义] */
    if (argc < 3) {
        printf("httpcopy <list.txt> <url_root> [http=1]\n");
        return (QST_ERROR);
    }

    /* 必须是有效的 URL 根 */
    len = str_lenA(argv[2]);
    if (len <= 7 || (argv[2])[len - 1] != '/') {
        printf("invalid URL root\n");
        return (QST_ERROR);
    }

    bool_t  http = TRUE;

    /* 是否实行 URL 转义 */
    if (argc > 3)
        http = str2intxA(argv[3]);

    /* 读入列表文件 */
    str = file_load_as_strA(argv[1]);
    if (str == NULL) {
        printf("file_load_as_strA() failure\n");
        return (QST_ERROR);
    }

    /* 初始化哈希表 */
    if (!s_fileTbl.init()) {
        printf("asy::map_acs::init() failure\n");
        return (QST_ERROR);
    }

    /* XML 文件另外处理 */
    if (filext_checkA(argv[1], ".xml"))
        return (do_xml_file(str, argv[1], argv[2], http));

    /* INI 行解析 */
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
        /* 输出文件搬移脚本 */
        DO_FILE_NODE(line, argv[2]);
    }
    ini_closeU(ini);
    fclose(fpb);
    fclose(fpu);
    s_fileTbl.free();
    return (QST_OKAY);
}
