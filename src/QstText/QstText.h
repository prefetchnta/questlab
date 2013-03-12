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
 *             ##       CREATE: 2013-03-08
 *              #
 ================================================
        QuestLAB 文本浏览器
 ================================================
 */

#ifndef __QL_QSTTEXT_H__
#define __QL_QSTTEXT_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstText"
    #define WIN_TITLE   "QstText"
    #define WIN_CLASS   "QstTextCLSS"
    #define WIN_ICONF   "QstText.ini"
    #define WIN_XCONF   "QstText.xml"
#endif

/* 工作上下文结构 */
typedef struct
{
        /* 非实时参数 */
        bool_t      quit;   /* 是否退出 */
        void_t*     form;   /* 窗口对象 */
        socket_t    netw;   /* 网络连接 */

} sQstText;

#endif  /* !__QL_QSTTEXT_H__ */
