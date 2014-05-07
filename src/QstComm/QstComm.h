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

/* 配置参数结构 */
typedef struct
{
        cl32_t  color;      /* 默认前景颜色 */
        cl32_t  bkcolor;    /* 默认背景颜色 */
        sint_t  font_size;  /* 全局字体大小 */
        ansi_t* font_face;  /* 全局字体名称 */

} sQCOM_conf;

/* 工作上下文结构 */
typedef struct
{
        /* 应用程序参数 */
        HWND        hwnd;       /* 窗口句柄 */
        bool_t      quit;       /* 是否退出 */
        lock_t      lock;       /* 同步的锁 */
        void_t*     form;       /* 窗口对象 */
        void_t*     view;       /* 显示对象 */
        void_t*     oper;       /* 操作对象 */
        socket_t    netw;       /* 网络连接 */
        sQCOM_conf  cfgs;       /* 配置参数 */
        sint_t      fw, fh;     /* 边框宽高 */

} sQstComm;

#endif  /* !__QL_QSTCOMM_H__ */
