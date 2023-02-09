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
 *             ##       CREATE: 2023-02-08
 *              #
 ================================================
        日Ｋ数据转换小工具 (同花顺)
 ================================================
 */

#include "../QstLibs/QstLibs.h"
#include "../Asylum/asylum.hpp"

/*
---------------------------------------
    查找下一个值
---------------------------------------
*/
static ansi_t*
find_next_value (
  __CR_IN__ const ansi_t*   str
    )
{
    for (;;) {
        if (*str == 0x00)
            break;
        if (*str == '\t') {
            str = skip_spaceA(str);
            if (*str == 0x00)
                break;
            return ((ansi_t*)str);
        }
        str++;
    }
    return (NULL);
}

/*
---------------------------------------
    判断是否是有效的值
---------------------------------------
*/
static bool_t
is_value_okay (
  __CR_OT__ double*         val,
  __CR_IN__ const ansi_t*   str,
  __CR_IN__ double          defv
    )
{
    byte_t  ch;

    str = skip_spaceA(str);
    if (str[0] == 0x2D && str[1] == 0x2D) {
        *val = defv;
        return (FALSE);
    }
    ch = *str;
    if ((ch == 0x2D) || (ch == 0x2B) || (ch >= 0x30 && ch <= 0x39)) {
        *val = str2fp64A(str);
        return (TRUE);
    }
    *val = defv;
    return (FALSE);
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    sINIu*  ini;
    ansi_t* txt;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 <输入文件名> <输出文件名> */
    if (argc < 3) {
        printf("ths2days <input.txt> <output.day>\n");
        return (QST_ERROR);
    }

    /* 装载文件并解析 */
    txt = file_load_as_strA(argv[1]);
    if (txt == NULL) {
        printf("can't open %s\n", argv[1]);
        return (QST_ERROR);
    }
    ini = ini_parseU(txt);
    mem_free(txt);
    if (ini == NULL) {
        printf("invalid txt file format\n");
        return (QST_ERROR);
    }
    if (ini->count <= 1) {
        printf("invalid txt file format\n");
        return (QST_ERROR);
    }

    sDATETIME                   dttm;
    asy::array<asy::kday_node>  list;

    /* 初始化列表 */
    list.init();

    /* 倒过来逐行解析 */
    for (leng_t ii = ini->count - 1; ii != 0; ii--)
    {
        /* 跳过空行 */
        txt = str_trimA(ini->lines[ii]);
        if (*txt == 0x00)
            continue;

        asy::kday_node  node;

        /* 0日期 - 转换成时间戳 */
        if (!str2datetimeA(&dttm, txt, '@') ||
            !datetime_to_unx(&node.time, &dttm)) {
            printf("invalid txt line date\n");
            return (QST_ERROR);
        }
        txt = find_next_value(txt);
        if (txt == NULL) {
            printf("invalid txt line column1\n");
            return (QST_ERROR);
        }

        /* 1开盘, 2最高, 3最低, 4收盘 */
        /* 5涨跌, 6涨幅, 7振幅, 8总手, 9金额 */
        is_value_okay(&node.data.detail.TOPEN, txt, -1000);
        txt = find_next_value(txt);
        if (txt == NULL) {
            printf("invalid txt line column2\n");
            return (QST_ERROR);
        }
        is_value_okay(&node.data.detail.HIGH, txt, -1000);
        txt = find_next_value(txt);
        if (txt == NULL) {
            printf("invalid txt line column3\n");
            return (QST_ERROR);
        }
        is_value_okay(&node.data.detail.LOW, txt, -1000);
        txt = find_next_value(txt);
        if (txt == NULL) {
            printf("invalid txt line column4\n");
            return (QST_ERROR);
        }
        if (!is_value_okay(&node.data.detail.TCLOSE, txt, 0)) {
            printf("invalid txt line tclose\n");
            return (QST_ERROR);
        }
        if (node.data.detail.TOPEN < 0)
            node.data.detail.TOPEN = node.data.detail.TCLOSE;
        if (node.data.detail.HIGH < 0)
            node.data.detail.HIGH = node.data.detail.TCLOSE;
        if (node.data.detail.LOW < 0)
            node.data.detail.LOW = node.data.detail.TCLOSE;

        /* 跳过的值 */
        for (uint_t jj = 0; jj < 4; jj++) {
            txt = find_next_value(txt);
            if (txt == NULL) {
                printf("invalid txt line column%u\n", jj + 5);
                return (QST_ERROR);
            }
        }

        /* 量价值 */
        is_value_okay(&node.data.detail.VOTURNOVER, txt, 0);
        txt = find_next_value(txt);
        if (txt == NULL) {
            printf("invalid txt line column9\n");
            return (QST_ERROR);
        }
        is_value_okay(&node.data.detail.VATURNOVER, txt, 0);

        /* 压入数据列表 */
        if (list.append(&node) == NULL) {
            printf("asy::array<>::append() failure\n");
            return (QST_ERROR);
        }
    }
    ini_closeU(ini);

    size_t          cnt = list.size();
    asy::kday_node* ptr = list.data();

    /* 计算未填写的值 */
    printf("\xC8\xD5\xC6\xDA\x2C\xB9\xC9\xC6\xB1\xB4\xFA\xC2\xEB\x2C\xC3\xFB"
           "\xB3\xC6\x2C\xCA\xD5\xC5\xCC\xBC\xDB\x2C\xD7\xEE\xB8\xDF\xBC\xDB"
           "\x2C\xD7\xEE\xB5\xCD\xBC\xDB\x2C\xBF\xAA\xC5\xCC\xBC\xDB\x2C\xC7"
           "\xB0\xCA\xD5\xC5\xCC\x2C\xD5\xC7\xB5\xF8\xB6\xEE\x2C\xD5\xC7\xB5"
           "\xF8\xB7\xF9\x2C\xB3\xC9\xBD\xBB\xC1\xBF\x2C\xB3\xC9\xBD\xBB\xBD"
           "\xF0\xB6\xEE\n");
    for (size_t idx = 0; idx < cnt; idx++, ptr++)
    {
        ansi_t  date[16];

        if (idx == cnt - 1)
            ptr->data.detail.LCLOSE = ptr[0].data.detail.TCLOSE;
        else
            ptr->data.detail.LCLOSE = ptr[1].data.detail.TCLOSE;
        ptr->data.detail.CHG = ptr->data.detail.TCLOSE-ptr->data.detail.LCLOSE;
        ptr->data.detail.PCHG = ptr->data.detail.CHG / ptr->data.detail.LCLOSE;
        ptr->data.detail.PCHG *= 100;
        ptr->date(&dttm);
        sprintf(date, "%04u-%02u-%02u", dttm.year, dttm.month, dttm.day);
        printf("%s,%s,%s,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%I64u,%I64u\n",
                date, argv[1], argv[2], ptr->data.detail.TCLOSE,
                ptr->data.detail.HIGH, ptr->data.detail.LOW,
                ptr->data.detail.TOPEN, ptr->data.detail.LCLOSE,
                ptr->data.detail.CHG, ptr->data.detail.PCHG,
                (int64u)ptr->data.detail.VOTURNOVER,
                (int64u)ptr->data.detail.VATURNOVER);
    }

    FILE*   fp;

    /* 创建输出文件 */
    fp = fopen(argv[2], "wb");
    if (fp == NULL) {
        printf("can't save %s\n", argv[2]);
        return (QST_ERROR);
    }
    fwrite(list.data(), cnt, sizeof(asy::kday_node), fp);
    fclose(fp);
    list.free();
    return (QST_OKAY);
}
