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
#define QST_ICON_DSK            0       /* 磁盘文件 */
#define QST_ICON_DIR            1       /* 目录项目 */
#define QST_ICON_PAK            2       /* 打包文件 */
#define QST_ICON_MEM            3       /* 包内文件 */
#define QST_ICON_CAM            4       /* 设备：摄像头 */
/* ------------------------------------------------------------------------- */
#define QST_ICON_ASM            5       /* 汇编文件 */
#define QST_ICON_BAT            6       /* 批处理文件 */
#define QST_ICON_C              7       /* C 文件 */
#define QST_ICON_CPP            8       /* C++ 文件 */
#define QST_ICON_CS             9       /* C# 文件 */
#define QST_ICON_DOC            10      /* 文档文件 */
#define QST_ICON_H              11      /* 头文件 */
#define QST_ICON_INL            12      /* 内嵌文件 */
#define QST_ICON_TXT            13      /* 文本文件 */
#define QST_ICON_VB             14      /* VB 文件 */
/* ------------------------------------------------------------------------- */
#define QST_ICON_INI            15      /* 配置文件 */
#define QST_ICON_DLL            16      /* DLL 文件 */
#define QST_ICON_FON            17      /* FON 文件 */
#define QST_ICON_TTF            18      /* TTF 文件 */
#define QST_ICON_ISO            19      /* ISO 文件 */
#define QST_ICON_WRI            20      /* WRI 文件 */
#define QST_ICON_EXE            21      /* EXE 文件 */
/* ------------------------------------------------------------------------- */
#define QST_ICON_FNT            22      /* FNT 文件 */
#define QST_ICON_HLP            23      /* HLP 文件 */
#define QST_ICON_ILK            24      /* ILK 文件 */
#define QST_ICON_LIB            25      /* LIB 文件 */
#define QST_ICON_MAK            26      /* MAK 文件 */
#define QST_ICON_NCB            27      /* NCB 文件 */
#define QST_ICON_OBJ            28      /* OBJ 文件 */
#define QST_ICON_PDB            29      /* PDB 文件 */
#define QST_ICON_SQL            30      /* SQL 文件 */
#define QST_ICON_XDR            31      /* XDR 文件 */
#define QST_ICON_XML            32      /* XML 文件 */
#define QST_ICON_XSD            33      /* XSD 文件 */
#define QST_ICON_XSL            34      /* XSL 文件 */
#define QST_ICON_PCB            35      /* PCB 文件 */
#define QST_ICON_SCH            36      /* SCH 文件 */
#define QST_ICON_PCBLIB         37      /* PCBLIB 文件 */
#define QST_ICON_SCHLIB         38      /* SCHLIB 文件 */
/* ------------------------------------------------------------------------- */
#define QST_ICON_MAX            38      /* 最大编号 */

/* 扩展名2图标编号 */
typedef struct
{
        uint_t          idx;    /* 索引值 */
        const wide_t*   ext;    /* 扩展名 */

} sQTEE_icon;

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
        /* 应用程序参数 */
        bool_t      quit;   /* 是否退出 */
        bool_t      busy;   /* 是否忙碌 */
        void_t*     form;   /* 窗口对象 */
        sARRAY      list;   /* 文件列表 */
        sCURBEAD    icon;   /* 图标列表 */
        socket_t    netw;   /* 网络连接 */

} sQstTree;

#endif  /* !__QL_QSTTREE_H__ */
