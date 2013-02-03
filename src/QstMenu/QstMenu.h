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
 *             ##       CREATE: 2012-08-30
 *              #
 ================================================
        QuestLAB 主菜单程序
 ================================================
 */

#ifndef __QL_QSTMENU_H__
#define __QL_QSTMENU_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstMenu"
    #define WIN_TITLE   "QstMenu"
    #define WIN_CLASS   "QstMenuCLSS"
    #define WIN_ICONF   "QstMenu.ini"
    #define WIN_XCONF   "QstMenu.xml"
#endif

/* 工作上下文结构 */
typedef struct
{
        /* 非实时参数 */
        bool_t      quit;   /* 是否退出 */
        void_t*     form;   /* 窗口对象 */
        socket_t    netw;   /* 网络连接 */

} sQstMenu;

#endif  /* !__QL_QSTMENU_H__ */
