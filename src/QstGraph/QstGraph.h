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
 *             ##       CREATE: 2018-12-11
 *              #
 ================================================
        QuestLAB 图表显示器
 ================================================
 */

#ifndef __QL_QSTGRAPH_H__
#define __QL_QSTGRAPH_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstGraph"
    #define WIN_TITLE   "QstGraph"
    #define WIN_CLASS   "QstGraphCLSS"
    #define WIN_ICONF   "QstGraph.ini"
    #define WIN_XCONF   "QstGraph.xml"
#endif

/* 工作上下文结构 */
typedef struct
{
        /* 应用程序参数 */
        bool_t      quit;   /* 是否退出 */
        uint_t      type;   /* 图表类型 */
        sint_t      winx;   /* 窗口坐标 */
        sint_t      winy;   /* 窗口坐标 */
        uint_t      winw;   /* 窗口宽度 */
        uint_t      winh;   /* 窗口高度 */
        void_t*     form;   /* 窗口对象 */
        socket_t    netw;   /* 网络连接 */

} sQstGraph;

#endif  /* !__QL_QSTGRAPH_H__ */
