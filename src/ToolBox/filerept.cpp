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
 *             ##       CREATE: 2021-07-12
 *              #
 ================================================
        重复文件查找小工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 全局参数 */
static bool_t   s_del = FALSE;

/* 节点结构 */
typedef struct
{
        leng_t  count;
        byte_t  hash[64];
        ansi_t* position;

} sFILE_NODE;

/*
---------------------------------------
    成员比较回调
---------------------------------------
*/
static bool_t
unit_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    return (mem_cmp(key, ((sFILE_NODE*)obj)->hash, 64) == 0);
}

/*
---------------------------------------
    成员索引回调
---------------------------------------
*/
static uint_t
unit_find (
  __CR_IN__ const void_t*   key
    )
{
    int32u  val = 0;
    int32u* dat = (int32u*)key;

    for (uint_t idx = 0; idx < 64 / 4; idx++)
        val ^= dat[idx];
    return ((uint_t)val);
}

/*
---------------------------------------
    输出重复文件列表
---------------------------------------
*/
static void_t
print_repeat (
  __CR_IN__ sFILE_NODE* node
    )
{
    FILE*   fp;
    byte_t  hash[16];
    ansi_t  name[40], *str;

    if (s_del) {
        str_cpyA(name, "filerept.bat");
    }
    else {
        name[0] = '+';
        hash_md5_total(hash, node->hash, 64);
        hex2strA(name + 1, hash, sizeof(hash));
        str_catA(name, ".txt");
    }
    fp = fopen(name, "a+");
    if (fp != NULL) {
        if (s_del) {
            str = utf8_to_local(CR_LOCAL, node->position);
            if (str != NULL) {
                fprintf(fp, "del /Q \"%s\"\n", str);
                mem_free(str);
            }
        }
        else {
            fprintf(fp, "%s\n", node->position);
        }
        fclose(fp);
    }
}

/*
---------------------------------------
    遍历哈希表输出重复文件
---------------------------------------
*/
static void_t
final_output (
  __CR_IN__ sCURBEAD*   tbl
    )
{
    if (tbl->__list__ == NULL)
        return;
    for (uint_t idx = 0; idx < tbl->__size__; idx++)
    {
        sLIST*      lst;
        sLST_UNIT*  node;

        lst = &tbl->__list__[idx];
        if (lst->__size__ == 0)
            continue;
        node = lst->__head__;
        do
        {
            sFILE_NODE* file;

            file = slist_get_dataT(node, sFILE_NODE);
            if (file->count > 1)
                print_repeat(file);
            node = node->next;
        } while (node != NULL);
    }
}

/*
---------------------------------------
    处理文件信息哈希
---------------------------------------
*/
static bool_t
compt_repeat (
  __CR_IO__ sCURBEAD*   tbl,
  __CR_IN__ sFILE_NODE* node
    )
{
    sFILE_NODE* find;

    /* 查找是否在哈希表里 */
    find = curbead_findT(tbl, sFILE_NODE, node->hash);
    if (find == NULL)
    {
        /* 插入哈希表 */
        if (curbead_insertT(tbl, sFILE_NODE, node->hash, node) == NULL) {
            printf("curbead_insertT() failure!!!\n");
            return (FALSE);
        }
    }
    else
    {
        /* 出现重复文件, 输出到对应列表 */
        print_repeat(find);

        /* 替换节点数据, 并增加计数的值 */
        find->position = node->position;
        find->count += 1;
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
    uint_t  type;
    ansi_t* list;
    wide_t* cnvt;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 [是否生成删除脚本] */
    if (argc < 1) {
        printf("filerept [0/1]\n");
        return (QST_ERROR);
    }
    if (argc > 1)
        s_del = str2intxA(argv[1]);
    else
        s_del = FALSE;

    /* 支持两种哈希文件模式 */
    list = file_load_as_strA("__hash__.txt");
    if (list != NULL) {
        type = 0;
    }
    else {
        list = file_load_as_strA("HaSHer.txt");
        if (list != NULL) {
            if (mem_cmp(list, BOM_UTF16LE, 2) != 0) {
                mem_free(list);
                printf("Invalid HaSHer.txt!!!\n");
                return (QST_ERROR);
            }
            cnvt = (wide_t*)(&list[2]);
            list = utf16_to_utf8(cnvt);
            mem_free(cnvt - 1);
            if (list == NULL) {
                printf("Invalid HaSHer.txt!!!\n");
                return (QST_ERROR);
            }
            type = 1;
        }
        else {
            printf("Missing __hash__.txt or HaSHer.txt!!!\n");
            return (QST_ERROR);
        }
    }

    sINIu*      ini;
    leng_t      len;
    sSHA512     ctx;
    sCURBEAD    tbl;

    /* 拆分行 */
    ini = ini_parseU(list);
    mem_free(list);
    if (ini == NULL) {
        printf("ini_parseU() failure!!!\n");
        return (QST_ERROR);
    }

    /* 创建哈希表 */
    if (!curbead_initT(&tbl, sFILE_NODE, ini->count)) {
        ini_closeU(ini);
        printf("curbead_initT() failure!!!\n");
        return (QST_ERROR);
    }
    tbl.find = unit_find;
    tbl.comp = unit_comp;

    sFILE_NODE  node;

    /* 两种格式的解析 */
    if (type == 0)
    {
        /* __hash__.txt */
        if (ini->count <= 5 || (ini->count - 1) % 5 != 0) {
            ini_closeU(ini);
            curbead_freeT(&tbl, sFILE_NODE);
            printf("Invalid __hash__.txt!!!\n");
            return (QST_ERROR);
        }

        /* 逐行计算文件信息的哈希 */
        for (leng_t idx = 0; idx < ini->count; idx += 5) {
            if (*(ini->lines[idx]) == 0x00)
                break;
            if (*(ini->lines[idx + 4]) != 0x00) {
                ini_closeU(ini);
                curbead_freeT(&tbl, sFILE_NODE);
                printf("Invalid __hash__.txt!!!\n");
                return (QST_ERROR);
            }
            hash_sha512_init(&ctx);
            len = str_lenA(ini->lines[idx + 1]);
            hash_sha512_update(&ctx, ini->lines[idx + 1], len);
            len = str_lenA(ini->lines[idx + 2]);
            hash_sha512_update(&ctx, ini->lines[idx + 2], len);
            len = str_lenA(ini->lines[idx + 3]);
            hash_sha512_update(&ctx, ini->lines[idx + 3], len);
            hash_sha512_finish(node.hash, &ctx);
            node.position = ini->lines[idx];
            node.count = 1;
            if (!compt_repeat(&tbl, &node)) {
                ini_closeU(ini);
                curbead_freeT(&tbl, sFILE_NODE);
                return (QST_ERROR);
            }
        }
    }
    else
    {
        leng_t  size;

        /* HaSHer.txt */
        if (ini->count <= 1) {
            ini_closeU(ini);
            curbead_freeT(&tbl, sFILE_NODE);
            printf("Invalid HaSHer.txt!!!\n");
            return (QST_ERROR);
        }
        list = str_strA(ini->lines[0], ": ");
        size = (leng_t)(list - ini->lines[0]);
        if (size != 43 && size != 51) {
            ini_closeU(ini);
            curbead_freeT(&tbl, sFILE_NODE);
            printf("Invalid HaSHer.txt!!!\n");
            return (QST_ERROR);
        }

        /* 逐行计算文件信息的哈希 */
        for (leng_t idx = 0; idx < ini->count; idx += 1) {
            if (*(ini->lines[idx]) == 0x00)
                break;
            hash_sha512_init(&ctx);
            hash_sha512_update(&ctx, ini->lines[idx], size);
            hash_sha512_finish(node.hash, &ctx);
            node.position = ini->lines[idx];
            node.position += size + 2;
            node.count = 1;
            if (!compt_repeat(&tbl, &node)) {
                ini_closeU(ini);
                curbead_freeT(&tbl, sFILE_NODE);
                return (QST_ERROR);
            }
        }
    }

    /* 最后输出一遍重复节点 */
    if (!s_del) final_output(&tbl);

    /* 释放资源 */
    ini_closeU(ini);
    curbead_freeT(&tbl, sFILE_NODE);
    return (QST_OKAY);
}
