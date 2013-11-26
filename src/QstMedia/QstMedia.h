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
 *             ##       CREATE: 2012-12-15
 *              #
 ================================================
        QuestLAB 媒体播放器
 ================================================
 */

#ifndef __QL_QSTMEDIA_H__
#define __QL_QSTMEDIA_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstMedia"
    #define WIN_TITLE   "QstMedia"
    #define WIN_CLASS   "QstMediaCLSS"
    #define WIN_ICONF   "QstMedia.ini"
    #define WIN_XCONF   "QstMedia.xml"
#endif

/* 配置参数结构 */
typedef struct
{
        bool_t  enabled;    /* 是否启用媒体播放 */
        bool_t  loop_play;  /* 是否循环播放媒体 */

} sQXMM_conf;

/* 工作上下文结构 */
typedef struct
{
        /* 应用程序参数 */
        bool_t      quit;   /* 是否退出 */
        void_t*     form;   /* 窗口对象 */
        lock_t      lock;   /* 同步的锁 */
        sARRAY      extz;   /* 插件列表 */
        socket_t    netw;   /* 网络连接 */
        iXMMEDIA*   play;   /* 播放接口 */
        sFMT_PRT*   fmtz;   /* FMTZ对象 */
        sQXMM_conf  cfgs;   /* 配置参数 */

} sQstMedia;

/* 进度条位置刻度 */
#define QST_XMM_MAX     65535

#endif  /* !__QL_QSTMEDIA_H__ */
