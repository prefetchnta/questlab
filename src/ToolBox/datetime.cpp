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
        日期时间计算小工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 星期字符串查表 */
static const ansi_t* s_weeks[] =
{
    " Sun.", " Mon.", "Tues.", " Wed.", "Thur.", " Fri.", " Sat.",
};

/*
---------------------------------------
    打印时间差值
---------------------------------------
*/
static void_t dlt_print (int64u delta)
{
    int64u  days, secs;

    days = delta / 86400;
    secs = delta % 86400;
    printf("delta: %I64u(s)\n", delta);
    printf("delta: %I64u(d) %I64u(s)\n", days, secs);
    printf("delta: %I64u(d) %I64u(h) %I64u(s)\n", days,
                    secs / 3600, secs % 3600);
    printf("delta: %I64u(d) %I64u(h) %I64u(m) %I64u(s)\n", days,
                    secs / 3600, (secs % 3600) / 60, (secs % 3600) % 60);
}

/*
---------------------------------------
    打印日期时间
---------------------------------------
*/
static void_t dtm_print (const sDATETIME *dtm)
{
    sDATEATTR   attr;

    datetime_attr(&attr, dtm);
    printf("[%04u-%02u-%02u %02u:%02u:%02u %s]\n", dtm->year, dtm->month,
            dtm->day, dtm->hour, dtm->minute, dtm->second, s_weeks[dtm->week]);
    printf("     {y_leap=%u,y_count=%u,y_index=%u,d_seconds=%u,m_count=%u}\n",
            attr.year_leap, attr.year_days, attr.days_year, attr.days_secs,
            attr.month_day);
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

        case '0':   /* 计算时间差 */
            if (!str2datetimeA(&end, argv[2], '@'))
                datetime_get(&end);
            printf("end: ");
            dtm_print(&end);
            delta = datetime_sub(&end, &beg);
            if (delta < 0) {
                delta = -delta;
                printf("[-]\n");
            }
            else {
                printf("[+]\n");
            }
            dlt_print(delta);
            break;

        case '1':   /* 计算目标时间 */
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
