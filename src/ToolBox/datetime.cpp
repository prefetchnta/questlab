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
 *             ##       CREATE: 2016-10-05
 *              #
 ================================================
        日期计算小工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/*
---------------------------------------
    打印时间差值
---------------------------------------
*/
static void_t dlt_print (int64u delta)
{
}

/*
---------------------------------------
    打印日期时间
---------------------------------------
*/
static void_t dtm_print (const sDATETIME *dtm)
{
}

/*
---------------------------------------
    显示工具用法
---------------------------------------
*/
static void_t usage (void_t)
{
    printf("datetime <0> <end dtm> [start dtm]\n");
    printf("datetime <1> <seconds> [start dtm]\n");
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    ansi_t      type;
    int64s      delta;
    int32s      seconds;
    sDATETIME   beg, end;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 <类型> <结束时间> [起始时间] */
    if ((argc < 3) ||
        (strcmp(argv[1], "0") != 0 && strcmp(argv[1], "1") != 0)) {
        usage();
        return (QST_ERROR);
    }
    type = *(argv[1]);

    /* 起始时间解析 */
    /* 格式: YYYY-MM-DD@HH:mm:SS */
    if (argc == 3 || !str2datetimeA(&beg, argv[3], '@'))
        datetime_get(&beg);
    printf("beg: ");
    dtm_print(&beg);

    /* 根据类型实现功能 */
    switch (type)
    {
        default:
            usage();
            return (QST_ERROR);

        case '0':
            if (!str2datetimeA(&end, argv[2], '@'))
                datetime_get(&end);
            printf("end: ");
            dtm_print(&end);
            delta = date_sub(&end, &beg);
            if (delta < 0) {
                delta = -delta;
                printf("[-]\n");
            }
            else {
                printf("[+]\n");
            }
            dlt_print(delta);
            break;

        case '1':
            seconds = str2int32A(argv[2]);
            struct_cpy(&end, &beg, sDATETIME);
            if (seconds < 0) {
                seconds = -seconds;
                time_dec(&end, seconds);
            }
            else {
                time_inc(&end, seconds);
            }
            printf("end: ");
            dtm_print(&end);
            break;
    }
    return (QST_OKAY);
}
