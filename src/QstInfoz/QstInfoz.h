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
 *             ##       CREATE: 2012-07-26
 *              #
 ================================================
        QuestLAB 信息显示器
 ================================================
 */

#ifndef __QL_QSTINFOZ_H__
#define __QL_QSTINFOZ_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstInfoz"
    #define WIN_TITLE   "QstInfoz"
    #define WIN_CLASS   "QstInfozCLSS"
    #define WIN_ICONF   "QstInfoz.ini"
    #define WIN_XCONF   "QstInfoz.xml"
#endif

/* 工作上下文结构 */
typedef struct
{
        /* 非实时参数 */
        bool_t      quit;   /* 是否退出 */
        void_t*     form;   /* 窗口对象 */
        socket_t    netw;   /* 网络连接 */

} sQstInfoz;

#endif  /* !__QL_QSTINFOZ_H__ */
