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
 *             ##       CREATE: 2013-11-13
 *              #
 ================================================
        QuestLAB 哈希计算器
 ================================================
 */

#ifndef __QL_QSTHASH_H__
#define __QL_QSTHASH_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstHash"
    #define WIN_TITLE   "QstHash"
    #define WIN_CLASS   "QstHashCLSS"
    #define WIN_ICONF   "QstHash.ini"
    #define WIN_XCONF   "QstHash.xml"
#endif

/* 工作上下文结构 */
typedef struct
{
        /* 应用程序参数 */
        bool_t      quit;   /* 是否退出 */
        void_t*     form;   /* 窗口对象 */
        socket_t    netw;   /* 网络连接 */

} sQstHash;

#endif  /* !__QL_QSTHASH_H__ */
