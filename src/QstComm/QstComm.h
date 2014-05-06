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
 *             ##       CREATE: 2014-05-05
 *              #
 ================================================
        QuestLAB 通讯收发器
 ================================================
 */

#ifndef __QL_QSTCOMM_H__
#define __QL_QSTCOMM_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstComm"
    #define WIN_TITLE   "QstComm"
    #define WIN_CLASS   "QstCommCLSS"
    #define WIN_ICONF   "QstComm.ini"
    #define WIN_XCONF   "QstComm.xml"
#endif

/* 工作上下文结构 */
typedef struct
{
        /* 应用程序参数 */
        HWND        hwnd;       /* 窗口句柄 */
        bool_t      quit;       /* 是否退出 */
        void_t*     form;       /* 窗口对象 */
        void_t*     view;       /* 显示对象 */
        socket_t    netw;       /* 网络连接 */
        sint_t      fw, fh;     /* 边框宽高 */

} sQstComm;

#endif  /* !__QL_QSTCOMM_H__ */
