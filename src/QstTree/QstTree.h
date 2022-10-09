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
#define QST_ICON_CAM            4       /* 设备:摄像头 */
/* ------------------------------------------------------------------------- */
#define QST_ICON_ASM            5       /* ASM 文件 */
#define QST_ICON_BAT            6       /* BAT 文件 */
#define QST_ICON_C              7       /* C 文件 */
#define QST_ICON_CPP            8       /* C++ 文件 */
#define QST_ICON_CS             9       /* C# 文件 */
#define QST_ICON_RTF            10      /* RTF 文件 */
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
#define QST_ICON_COM            21      /* COM 文件 */
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
#define QST_ICON_AUDIO          39      /* 音频文件 */
#define QST_ICON_AVI            40      /* AVI 文件 */
#define QST_ICON_CD             41      /* CD 光盘 */
#define QST_ICON_DVD            42      /* DVD 光盘 */
#define QST_ICON_DVDA           43      /* DVDA 光盘 */
#define QST_ICON_FLIC           44      /* FLIC 文件 */
#define QST_ICON_FLV            45      /* FLV 文件 */
#define QST_ICON_LOSSLESS       46      /* 无损音频文件 */
#define QST_ICON_MKV            47      /* MKV 文件 */
#define QST_ICON_MP4            48      /* MP4 文件 */
#define QST_ICON_MPG            49      /* MPG 文件 */
#define QST_ICON_OGM            50      /* OGM 文件 */
#define QST_ICON_PLAYLIST       51      /* 播放列表文件 */
#define QST_ICON_QUICKTIME      52      /* QTIME 文件 */
#define QST_ICON_REAL           53      /* REAL 文件 */
#define QST_ICON_SWF            54      /* SWF 文件 */
#define QST_ICON_TS             55      /* TS 文件 */
#define QST_ICON_VIDEO          56      /* 视频文件 */
#define QST_ICON_WEBM           57      /* WEBM 文件 */
#define QST_ICON_WMA            58      /* WMA 文件 */
#define QST_ICON_WMV            59      /* WMV 文件 */
#define QST_ICON_PY             60      /* Python 源文件 */
#define QST_ICON_PYC            61      /* Python 字节文件 */
#define QST_ICON_PYD            62      /* Python 库文件 */
/* ------------------------------------------------------------------------- */
#define QST_ICON_7Z             63      /* 7ZIP 文件 */
#define QST_ICON_ZIP            64      /* ZIP 文件 */
#define QST_ICON_BZ2            65      /* BZ2 文件 */
#define QST_ICON_RAR            66      /* RAR 文件 */
#define QST_ICON_ARJ            67      /* ARJ 文件 */
#define QST_ICON_Z              68      /* Z 文件 */
#define QST_ICON_CAB            69      /* CAB 文件 */
#define QST_ICON_SPLIT          70      /* 7Z 分卷文件 */
#define QST_ICON_RPM            71      /* RPM 文件 */
#define QST_ICON_DEB            72      /* DEB 文件 */
#define QST_ICON_CPIO           73      /* CPIO 文件 */
#define QST_ICON_TAR            74      /* TAR 文件 */
#define QST_ICON_GZ             75      /* GZ 文件 */
#define QST_ICON_WIM            76      /* WIM 文件 */
#define QST_ICON_LZH            77      /* LZH 文件 */
#define QST_ICON_DMG            78      /* DMG 文件 */
#define QST_ICON_HFS            79      /* HFS 文件 */
#define QST_ICON_XAR            80      /* XAR 文件 */
#define QST_ICON_BMP            81      /* BMP 文件 (无压缩) */
#define QST_ICON_GIF            82      /* GIF 文件 (无损压缩) */
#define QST_ICON_JPG            83      /* JPG 文件 (有损压缩) */
#define QST_ICON_TIF            84      /* TIF 文件 (混合模式) */
#define QST_ICON_PDF            85      /* PDF 文件 */
#define QST_ICON_MDB            86      /* MDB 文件 */
#define QST_ICON_ACCDB          87      /* ACCDB 文件 */
#define QST_ICON_DOC            88      /* DOC 文件 */
#define QST_ICON_DOCX           89      /* DOCX 文件 */
#define QST_ICON_PPT            90      /* PPT 文件 */
#define QST_ICON_PPTX           91      /* PPTX 文件 */
#define QST_ICON_XLS            92      /* XLS 文件 */
#define QST_ICON_XLSX           93      /* XLSX 文件 */
#define QST_ICON_MARKDOWN       94      /* MD 文件 */
#define QST_ICON_SH             95      /* SH 文件 */
#define QST_ICON_CHM            96      /* CHM 文件 */
#define QST_ICON_CER            97      /* CER 文件 */
#define QST_ICON_PAL            98      /* PAL 文件 */
#define QST_ICON_RC             99      /* RC 文件 */
#define QST_ICON_RCT            100     /* RCT 文件 */
#define QST_ICON_RES            101     /* RES 文件 */
#define QST_ICON_RESX           102     /* RESX 文件 */
/* ------------------------------------------------------------------------- */
#define QST_ICON_SLN60          103     /* _SLN60 文件 */
#define QST_ICON_SLN70          104     /* _SLN70 文件 */
#define QST_ICON_SLN71          105     /* _SLN71 文件 */
#define QST_ICON_SLN80          106     /* _SLN80 文件 */
#define QST_ICON_SLN90          107     /* _SLN90 文件 */
#define QST_ICON_SLN100         108     /* _SLN100 文件 */
#define QST_ICON_A              109     /* A 文件 */
#define QST_ICON_ACA            110     /* ACA 文件 */
#define QST_ICON_ASAX           111     /* ASAX 文件 */
#define QST_ICON_ASCX           112     /* ASCX 文件 */
#define QST_ICON_ASHX           113     /* ASHX 文件 */
#define QST_ICON_ASMX           114     /* ASMX 文件 */
#define QST_ICON_ASP            115     /* ASP 文件 */
#define QST_ICON_ASPX           116     /* ASPX 文件 */
#define QST_ICON_BIN            117     /* BIN 文件 */
#define QST_ICON_CMD            118     /* CMD 文件 */
#define QST_ICON_CSDPROJ        119     /* CSDPROJ 文件 */
#define QST_ICON_CSHTML         120     /* CSHTML 文件 */
#define QST_ICON_CSPROJ         121     /* CSPROJ 文件 */
#define QST_ICON_CUR            122     /* CUR 文件 */
#define QST_ICON_D88            123     /* D88 文件 */
#define QST_ICON_DMP            124     /* DMP 文件 */
#define QST_ICON_DSP            125     /* DSP 文件 */
#define QST_ICON_DSW            126     /* DSW 文件 */
#define QST_ICON_DTD            127     /* DTD 文件 */
#define QST_ICON_EXE            128     /* EXE 文件 */
#define QST_ICON_EXP            129     /* EXP 文件 */
#define QST_ICON_FDI            130     /* FDI 文件 */
#define QST_ICON_FILTERS        131     /* FILTERS 文件 */
#define QST_ICON_FS             132     /* FS 文件 */
#define QST_ICON_FSI            133     /* FSI 文件 */
#define QST_ICON_FSPROJ         134     /* FSPROJ 文件 */
#define QST_ICON_FSX            135     /* FSX 文件 */
#define QST_ICON_HTM            136     /* HTM 文件 */
#define QST_ICON_ICC            137     /* ICC 文件 */
#define QST_ICON_MANIFEST       138     /* MANIFEST 文件 */
#define QST_ICON_MHT            139     /* MHT 文件 */
#define QST_ICON_MID            140     /* MID 文件 */
#define QST_ICON_O              141     /* O 文件 */
#define QST_ICON_PCH            142     /* PCH 文件 */
#define QST_ICON_PIF            143     /* PIF 文件 */
#define QST_ICON_PS1            144     /* PS1 文件 */
#define QST_ICON_REG            145     /* REG 文件 */
#define QST_ICON_S              146     /* S 文件 */
#define QST_ICON_SLN            147     /* SLN 文件 */
#define QST_ICON_SO             148     /* SO 文件 */
#define QST_ICON_THEME          149     /* THEME 文件 */
#define QST_ICON_TTC            150     /* TTC 文件 */
#define QST_ICON_USER           151     /* USER 文件 */
#define QST_ICON_VBA            152     /* VBA 文件 */
#define QST_ICON_VBDPROJ        153     /* VBDPROJ 文件 */
#define QST_ICON_VBHTML         154     /* VBHTML 文件 */
#define QST_ICON_VBPROJ         155     /* VBPROJ 文件 */
#define QST_ICON_VBS            156     /* VBS 文件 */
#define QST_ICON_VCXPROJ        157     /* VCXPROJ 文件 */
#define QST_ICON_VDP            158     /* VDP 文件 */
#define QST_ICON_VSPROPS        159     /* VSPROPS 文件 */
#define QST_ICON_WSH            160     /* WSH 文件 */
#define QST_ICON_XAML           161     /* XAML 文件 */
#define QST_ICON_XAMLX          162     /* XAMLX 文件 */
#define QST_ICON_XBAP           163     /* XBAP 文件 */
/* ------------------------------------------------------------------------- */
#define QST_ICON_MAX            163     /* 最大编号 */

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
