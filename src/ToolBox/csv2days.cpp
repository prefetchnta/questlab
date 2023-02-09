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
 *             ##       CREATE: 2022-01-19
 *              #
 ================================================
        日Ｋ数据转换小工具 (网易)
 ================================================
 */

#include "../QstLibs/QstLibs.h"

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
    ch = *str;
    if ((ch == 0x2D) || (ch >= 0x30 && ch <= 0x39)) {
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
    ansi_t* csv;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 <输入文件名> <输出文件名> */
    if (argc < 3) {
        printf("csv2days <input.csv> <output.day>\n");
        return (QST_ERROR);
    }

    /* 装载文件并解析 */
    csv = file_load_as_strA(argv[1]);
    if (csv == NULL) {
        printf("can't open %s\n", argv[1]);
        return (QST_ERROR);
    }
    ini = ini_parseU(csv);
    mem_free(csv);
    if (ini == NULL) {
        printf("invalid csv file format\n");
        return (QST_ERROR);
    }

    FILE*   fp;

    /* 创建输出文件 */
    fp = fopen(argv[2], "wb");
    if (fp == NULL) {
        printf("can't save %s\n", argv[2]);
        ini_closeU(ini);
        return (QST_ERROR);
    }

    /* 从第二行开始解析 */
    for (leng_t ii = 1; ii < ini->count; ii++)
    {
        leng_t      cnts;
        int64u      tick;
        ansi_t**    list;

        /* 跳过空行 */
        csv = skip_spaceA(ini->lines[ii]);
        if (*csv == 0x00)
            continue;

        /* 逗号分割 */
        list = str_splitA(csv, ',', &cnts);
        if (list == NULL || cnts != 12) {
            printf("invalid csv file line\n");
            if (list != NULL)
                mem_free(list);
            fclose(fp);
            ini_closeU(ini);
            return (QST_ERROR);
        }

        sDATETIME   dttm;

        /* 0日期 - 转换成时间戳 */
        csv = skip_spaceA(list[0]);
        if (!str2datetimeA(&dttm, csv, '@') ||
            !datetime_to_unx(&tick, &dttm)) {
            printf("invalid csv line date\n");
            mem_free(list);
            fclose(fp);
            ini_closeU(ini);
            return (QST_ERROR);
        }
        fwrite(&tick, sizeof(tick), 1, fp);

        double  vals[9];

        /* 1股票代码 & 2名称 - 跳过 */
        /* 3收盘价, 4最高价, 5最低价, 6开盘价 */
        /* 7前收盘, 8涨跌额, 9涨跌幅, 10成交量, 11成交额 */
        if (!is_value_okay(&vals[0], list[3], 0)) {
            printf("invalid csv line tclose\n");
            mem_free(list);
            fclose(fp);
            ini_closeU(ini);
            return (QST_ERROR);
        }
        is_value_okay(&vals[1], list[4], vals[0]);
        is_value_okay(&vals[2], list[5], vals[0]);
        is_value_okay(&vals[3], list[6], vals[0]);
        is_value_okay(&vals[4], list[7], vals[0]);
        is_value_okay(&vals[5], list[8], vals[0] - vals[4]);
        is_value_okay(&vals[6], list[9], (vals[5] / vals[4]) * 100);
        is_value_okay(&vals[7], list[10], 0);
        is_value_okay(&vals[8], list[11], 0);
        fwrite(vals, sizeof(vals), 1, fp);
        mem_free(list);
    }
    fclose(fp);
    ini_closeU(ini);
    return (QST_OKAY);
}
