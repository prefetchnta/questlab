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
        ansi_t  fg[9], bg[9];   /* 颜色串 */

} sQCOM_conf;

/* 通讯线程上下文结构 */
typedef struct
{
        bool_t  quit;   /* 是否结束 */
        thrd_t  thrd;   /* 线程句柄 */
        bool_t  text;   /* 文本模式 */

        /* 收发模式 */
        const ansi_t*   rtype;
        const ansi_t*   stype;
        const ansi_t*   title;

        /* 通讯对象 */
        union {
            void_t*     parm;   /* 传值用的 */
            uint_t      port;   /* RS232 */
            socket_t    netw;   /* TCPv4/UDPv4 */
        } obj;

        /* 数据变换 (默认直接发送) */
        void_t* (*tran) (const ansi_t *string, uint_t *ot_size);

        /* 发送调用 (由通讯类型决定) */
        void_t  (*send) (void_t *obj, const void_t *data, uint_t size);

        /* 接收数据的渲染 (默认直接显示) */
        void_t  (*render) (void_t *parm, const void_t *data, uint_t size);

} sQCOM_ctx;

/* 工作上下文结构 */
typedef struct
{
        /* 应用程序参数 */
        HWND        hwnd;       /* 窗口句柄 */
        bool_t      quit;       /* 是否退出 */
        lock_t      lock;       /* 同步的锁 */
        uint_t      page;       /* 显示编码 */
        uint_t      sbyt;       /* 字节断流 */
        void_t*     oper;       /* 操作对象 */
        uint_t      size;       /* 数据大小 */
        iDATOT*     bufs;       /* 临时缓存 */
        socket_t    netw;       /* 网络连接 */
        sQCOM_ctx   comm;       /* 通讯结构 */
        sQCOM_conf  cfgs;       /* 配置参数 */

        /* 给插件的回调 */
        void_t  (*clear) (void_t);
        void_t  (*setText) (const ansi_t *str);
        void_t  (*setHtml) (const ansi_t *str);
        uint_t  (*txtMode) (ansi_t *text, uint_t size);

} sQstComm;

/* 多线程锁简化宏 */
#define _ENTER_COM_SINGLE_  \
    mtlock_acquire(&(((sQstComm*)parm)->lock));
#define _LEAVE_COM_SINGLE_  \
    mtlock_release(&(((sQstComm*)parm)->lock));

/* 两个插件函数类型 */
typedef void_t* (*plugin_tran_t) (const ansi_t*, uint_t*);
typedef void_t  (*plugin_render_t) (void_t*, const void_t*, uint_t);

/* 公用的数据处理声明 */
CR_API uint_t   qst_txt_mode (ansi_t *text, uint_t size);

/* 公用的数据渲染声明 */
CR_API void_t   qst_txt_show (void_t *parm, const void_t *data, uint_t size);

#endif  /* !__QL_QSTCOMM_H__ */
