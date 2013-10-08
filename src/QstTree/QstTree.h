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
 *             ##       CREATE: 2012-08-04
 *              #
 ================================================
        QuestLAB 文件树形表
 ================================================
 */

#ifndef __QL_QSTTREE_H__
#define __QL_QSTTREE_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstTree"
    #define WIN_TITLE   "QstTree"
    #define WIN_CLASS   "QstTreeCLSS"
    #define WIN_ICONF   "QstTree.ini"
    #define WIN_XCONF   "QstTree.xml"
#endif

/* 图标索引值 */
#define QST_ICON_DSK    0   /* 磁盘文件 */
#define QST_ICON_DIR    1   /* 目录项目 */
#define QST_ICON_PAK    2   /* 打包文件 */
#define QST_ICON_MEM    3   /* 包内文件 */
#define QST_ICON_CAM    4   /* 设备：摄像头 */
#define QST_ICON_MAX    4   /* 最大编号 */

/* 文件属性值 */
#define QST_FILE_CMP    0x00000001UL    /* 压缩 */
#define QST_FILE_HID    0x00000002UL    /* 隐藏 */
#define QST_FILE_ENC    0x00000004UL    /* 加密 */
#define QST_FILE_DIR    0x00000008UL    /* 目录 */
#define QST_FILE_PAK    0x00000010UL    /* 打包 */
#define QST_FILE_MEM    0x00000020UL    /* 包内 */

/* 文件单元结构 */
typedef struct
{
        int64s          offs;   /* 文件的偏移 */
        int64u          pack;   /* 压缩的大小 */
        int64u          size;   /* 文件的大小 */
        leng_t          head;   /* 路径的开始 */
        uint_t          page;   /* 文件名编码 */
        uint_t          icon;   /* 图标的编号 */
        int32u          attr;   /* 文件的属性 */
        void_t*         root;   /* 父文件节点 */
        const wide_t*   name;   /* 文件的名称 */
        const wide_t*   path;   /* 文件全路径 */
        const wide_t*   memo;   /* 文件的备注 */

} sQTEE_file;

/* 工作上下文结构 */
typedef struct
{
        /* 非实时参数 */
        bool_t      quit;   /* 是否退出 */
        bool_t      busy;   /* 是否忙碌 */
        void_t*     form;   /* 窗口对象 */
        sARRAY      list;   /* 文件列表 */
        socket_t    netw;   /* 网络连接 */

} sQstTree;

#endif  /* !__QL_QSTTREE_H__ */
