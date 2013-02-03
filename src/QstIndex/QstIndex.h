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
 *             ##       CREATE: 2012-07-25
 *              #
 ================================================
        QuestLAB 索引控制器
 ================================================
 */

#ifndef __QL_QSTINDEX_H__
#define __QL_QSTINDEX_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstIndex"
    #define WIN_TITLE   "QstIndex"
    #define WIN_CLASS   "QstIndexCLSS"
    #define WIN_ICONF   "QstIndex.ini"
    #define WIN_XCONF   "QstIndex.xml"
#endif

/* 工作上下文结构 */
typedef struct
{
        /* 非实时参数 */
        bool_t      quit;   /* 是否退出 */
        bool_t      send;   /* 是否发送 */
        void_t*     form;   /* 窗口对象 */
        socket_t    netw;   /* 网络连接 */

} sQstIndex;

#endif  /* !__QL_QSTINDEX_H__ */
