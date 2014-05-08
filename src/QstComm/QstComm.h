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

/* 通讯线程上下文结构 */
typedef struct
{
        bool_t  quit;   /* 是否结束 */
        thrd_t  thrd;   /* 线程句柄 */

        /* 通讯对象 */
        union {
            void_t*     parm;   /* 传值用的 */
            uint_t      port;   /* RS232 */
            socket_t    netw;   /* TCPv4/UDPv4 */
        } obj;

        /* 数据变换 (默认直接发送) */
        void_t* (*tran) (const ansi_t *string, leng_t *ot_size);

        /* 发送调用 (由通讯类型决定) */
        void_t  (*send) (void_t *obj, const void_t *data, leng_t size);

        /* 接收数据的渲染 (默认直接显示) */
        void_t  (*render) (void_t *parm, const void_t *data, leng_t size);

} sQCOM_ctx;

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
        sQCOM_ctx   comm;       /* 通讯结构 */
        sQCOM_conf  cfgs;       /* 配置参数 */

} sQstComm;

#endif  /* !__QL_QSTCOMM_H__ */
