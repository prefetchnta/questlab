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
 *             ##       CREATE: 2013-11-25
 *              #
 ================================================
        QuestLAB 数据探测器
 ================================================
 */

#ifndef __QL_QSTDATA_H__
#define __QL_QSTDATA_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstData"
    #define WIN_TITLE   "QstData"
    #define WIN_CLASS   "QstDataCLSS"
    #define WIN_ICONF   "QstData.ini"
    #define WIN_XCONF   "QstData.xml"
#endif

/* 数值探测执行单元 */
typedef struct
{
        /* 数值类型串 */
        const ansi_t*   name;

        /* 数值观察器函数 */
        ansi_t* (*data_view) (const void_t *data, leng_t size,
                              bool_t is_be);
} sQDAT_UNIT;

/* 工作上下文结构 */
typedef struct
{
        /* 应用程序参数 */
        bool_t      quit;   /* 是否退出 */
        void_t*     form;   /* 窗口对象 */
        lock_t      lock;   /* 同步的锁 */
        socket_t    netw;   /* 网络连接 */

        /* 输入文件的读取对象 */
        iDATIN*     datin;

        /* 支持的数值类型列表 */
        const sQDAT_UNIT*   viewer;

} sQstData;

/* 多线程锁简化宏 */
#define _ENTER_DAT_SINGLE_  \
    mtlock_acquire(&s_wrk_ctx.lock);
#define _LEAVE_DAT_SINGLE_  \
    mtlock_release(&s_wrk_ctx.lock);

#endif  /* !__QL_QSTDATA_H__ */
