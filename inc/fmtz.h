/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-19  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-02-28  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 文件格式头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_FMTZ_H__
#define __CR_FMTZ_H__ 0x34434AADUL

#include "gfx2.h"
#include "datlib.h"
#include "fileio.h"
#include "plugin.h"

/*****************************************************************************/
/*                                文件总格式                                 */
/*****************************************************************************/

/* 文件格式类型 */
#define CR_FMTZ_NOP     0   /* 未知文件 */
#define CR_FMTZ_DEC     1   /* 解码文件 */
#define CR_FMTZ_PIC     2   /* 图片文件 */
#define CR_FMTZ_RCT     3   /* 区域文件 */
#define CR_FMTZ_TXT     4   /* 文本文件 */
#define CR_FMTZ_PRT     5   /* 对象文件 */

/* 文件格式总结构 */
typedef struct
{
        uint_t  type;   /* 格式类型 (每个子结构都以此开头) */
#if 0
        byte_t  data[];     /* 各自不同的结构体数据, 使用时根据
                                type 强制转换指针类型后使用 */
#endif
} sFMTZ;

/* 两个用得到的函数类型 */
typedef sFMTZ*  (*load_fmtz_t) (iDATIN*, const sLOADER*);

/* 格式最终结果掩码 */
#define CR_FMTZ_MASK_DAT    0x00000001UL    /* 数据文件 */
#define CR_FMTZ_MASK_PIC    0x00000002UL    /* 图片文件 */
#define CR_FMTZ_MASK_RCT    0x00000004UL    /* 区域文件 */
#define CR_FMTZ_MASK_TXT    0x00000008UL    /* 文本文件 */
#define CR_FMTZ_MASK_PAK    0x00000010UL    /* 封包文件 */
#define CR_FMTZ_MASK_XMM    0x00000020UL    /* 媒体文件 */
#define CR_FMTZ_MASK_ALL    0xFFFFFFFFUL    /* 所有文件 */

/*****************************************************************************/
/*                               数据文件格式                                */
/*****************************************************************************/

/* 数据文件格式结构 */
typedef struct
{
        uint_t          type;   /* 类型 (NOP/DEC) */
        leng_t          unsz;   /* 实际大小 */
        fsize_t         pksz;   /* 文件大小 */
        void_t*         data;   /* 文件数据 */
        const ansi_t*   infor;  /* 格式说明 */

} sFMT_DAT;

/*****************************************************************************/
/*                               图片文件格式                                */
/*****************************************************************************/

/* 文件的真实格式 */
#define CR_PIC_NONE     0   /* 未知格式 */
#define CR_PIC_GREY     1   /* 灰度格式 */
#define CR_PIC_PALS     2   /* 索引格式 */
#define CR_PIC_ARGB     3   /* 真彩格式 */
#define CR_PIC_SPEC     4   /* 特殊格式 */
#define CR_PIC_CMYK     5   /* CMYK格式 */
#define CR_PIC_CMPX     6   /* 复数格式 */
#define CR_PIC_IEEE     7   /* 浮点格式 */

/* 图片帧结构 */
typedef struct
{
        uint_t          bpp;    /* 真实位数 */
        uint_t          fmt;    /* 真实格式 */
        sIMAGE*         pic;    /* 图片数据 */
        const ansi_t*   clr;    /* 颜色说明 */
        byte_t          wh[5];  /* 分量位数 */

} sFMT_FRAME;

/* 图片文件格式结构 */
typedef struct
{
        uint_t          type;   /* 类型 (PIC) */
        int32u          count;  /* 帧数 */
        sFMT_FRAME*     frame;  /* 帧数据 */
        const ansi_t*   infor;  /* 格式说明 */

} sFMT_PIC;

/***** 文件加载 API 组 *****/
CR_API sFMT_PIC*    load_cr_bmp (iDATIN *datin, const sLOADER *param);
CR_API sFMT_PIC*    load_cr_dds (iDATIN *datin, const sLOADER *param);
CR_API sFMT_PIC*    load_cr_pcx (iDATIN *datin, const sLOADER *param);
CR_API sFMT_PIC*    load_cr_png (iDATIN *datin, const sLOADER *param);
CR_API sFMT_PIC*    load_cr_tga (iDATIN *datin, const sLOADER *param);

/*****************************************************************************/
/*                               区域文件格式                                */
/*****************************************************************************/

/* 区域文件格式结构 */
typedef struct
{
        uint_t          type;   /* 类型 (RCT) */
        sRECT           rect;   /* 坐标数据结构 */
        ansi_t*         pict;   /* 图片名称字符串 */
        int32u          index;  /* 图片对象帧数索引 */
        sIMAGE*         image;  /* 指向图片对象的指针 */
        const ansi_t*   infor;  /* 格式说明 */

} sFMT_RCT;

/*****************************************************************************/
/*                               文本文件格式                                */
/*****************************************************************************/

/* 文本文件结构格式 */
typedef struct
{
        uint_t          type;   /* 类型 (TXT) */
        ansi_t*         text;   /* 文本字符串 */
        const ansi_t*   infor;  /* 格式说明 */

} sFMT_TXT;

/*****************************************************************************/
/*                               对象文件格式                                */
/*****************************************************************************/

/* 对象文件格式结构 */
typedef struct
{
        uint_t          type;   /* 类型 (PRT) */
        iPORT*          port;   /* 对象接口指针 */
        port_t          more;   /* 对象类型前缀 */
        const ansi_t*   infor;  /* 格式说明 */

} sFMT_PRT;

/*****************************************************************************/
/*                              FMTZ 插件结构                                */
/*****************************************************************************/

/* 引擎插件结构 */
typedef struct _sENGINE
{
        sbin_t          sbin;   /* 模块句柄, 以后填写 */
        void_t*         data;   /* 附加的运行时的数据 */
        int32u          mask;   /* 期望结果的格式, 默认所有 */
        const ansi_t*   info;   /* 引擎模块的说明文字 */

        /* 格式加载调用接口 (匹配项放在 loader->nprm 里) */
        sFMTZ*  (*fmtz_load) (struct _sENGINE *engine, sLOADER *loader);

        /* 释放引擎插件内部数据的回调 */
        void_t  (*engine_free) (struct _sENGINE *engine);

} sENGINE;

/* 内置的引擎接口列表 */
CR_API sENGINE*     engine_crhack (void_t);

/* 获取引擎插件的接口类型 */
typedef sENGINE*    (*engine_init_t) (void_t);

/***** 插件相关 API 组 *****/
CR_API bool_t   fmtz_free (const sFMTZ *fmtz);
CR_API bool_t   fmtz_find (sENGINE *engine, sLOADER *loader);
CR_API sIMAGE*  fmtz_get_pic (const sFMTZ *fmtz, int32u index);
CR_API void_t   engine_free (sENGINE *engine);

/*
=======================================
    使用引擎插件
=======================================
*/
inline sFMTZ*
engine_do (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    /* 简单包装一下这个接口 */
    return (engine->fmtz_load(engine, loader));
}

/*****************************************************************************/
/*                               读包系统接口                                */
/*****************************************************************************/

/* 子文件信息单元 */
typedef struct
{
        leng_t          skip;   /* 结构的大小 */
        int32u          attr;   /* 文件的属性 */
        int64s          offs;   /* 文件的偏移 */
        int64u          pack;   /* 压缩的大小 */
        int64u          size;   /* 文件的大小 */
        const ansi_t*   name;   /* 文件全路径 (UTF-8) */
        const ansi_t*   memo;   /* 文件的备注文本信息 */
        const ansi_t*   find;   /* 统一路径名 (查找用) */
#if 0
        /* 附加的自定义数据 */
        byte_t  addons[skip - sizeof(sPAK_FILE)];
#endif
} sPAK_FILE;

/* 文件属性取值 */
#define PAK_FILE_CMP    0x00000001UL    /* 压缩 */
#define PAK_FILE_ENC    0x00000002UL    /* 加密 */

struct  _iPACKAGE;
#define iPACKAGE    struct _iPACKAGE

/* 读包对象接口表 */
typedef struct
{
        /* 通用组 */
        void_t  (*release) (iPACKAGE *that);
        void_t* (*getMore) (iPACKAGE *that, port_t iid);

        /* 加载组 */
        int64u  (*getFileNum) (iPACKAGE *that);
        bool_t  (*getFileData) (iPACKAGE *that, sBUFFER *buff,
                                int64u index, bool_t hash);
        bool_t  (*getFileInfo) (iPACKAGE *that, sPAK_FILE **info,
                                int64u index);
} iPACKAGE_vtbl;

#undef  iPACKAGE

/* 读包对象接口类 */
typedef struct  _iPACKAGE
{
        /* 虚函数表 */
        const iPACKAGE_vtbl*    __vptr__;

        /* 数据成员 */
        sCURTAIN    __search__;     /* 文件查找表 */
        sPAK_FILE*  __filelst__;    /* 文件信息表 */
        bool_t      __caseless__;   /* 不区分大小写 */

} iPACKAGE;

/* 读取文件个数 */
CR_API int64u   pack_file_num (iPACKAGE *pack);

/* 查找文件索引 */
CR_API bool_t   pack_find_fileU (iPACKAGE *pack, int64u *index,
                                 const ansi_t *name);
CR_API bool_t   pack_find_fileW (iPACKAGE *pack, int64u *index,
                                 const wide_t *name);
CR_API bool_t   pack_find_fileA (iPACKAGE *pack, int64u *index,
                                 const ansi_t *name, uint_t cpage);
/* 读取文件数据 (根据索引) */
CR_API bool_t   pack_file_data (iPACKAGE *pack, sBUFFER *buff,
                                int64u index, bool_t hash);
/* 读取文件信息 (根据索引) */
CR_API bool_t   pack_file_info (iPACKAGE *pack, sPAK_FILE **info,
                                int64u index);
/* 读取文件数据 (根据名称) */
CR_API bool_t   pack_file_dataU (iPACKAGE *pack, sBUFFER *buff,
                                 const ansi_t *name, bool_t hash);
CR_API bool_t   pack_file_dataW (iPACKAGE *pack, sBUFFER *buff,
                                 const wide_t *name, bool_t hash);
CR_API bool_t   pack_file_dataA (iPACKAGE *pack, sBUFFER *buff,
                                 const ansi_t *name, uint_t cpage,
                                 bool_t hash);
/* 读取文件信息 (根据名称) */
CR_API bool_t   pack_file_infoU (iPACKAGE *pack, sPAK_FILE **info,
                                 const ansi_t *name);
CR_API bool_t   pack_file_infoW (iPACKAGE *pack, sPAK_FILE **info,
                                 const wide_t *name);
CR_API bool_t   pack_file_infoA (iPACKAGE *pack, sPAK_FILE **info,
                                 const ansi_t *name, uint_t cpage);
/* ZIP 文件信息结构 */
typedef struct
{
        /* 公用部分 */
        sPAK_FILE   base;

        /* 个性部分 */
        int32u  crc32;  /* 文件校验 */
        int16u  ftime;  /* 文件时间 (DOS) */
        int16u  fdate;  /* 文件日期 (DOS) */
        int16u  ftype;  /* 文件压缩类型 */

} sPAK_ZIP_FILE;

/***** 文件加载 API 组 *****/
CR_API sFMT_PRT*    load_cr_zip (iDATIN *datin, const sLOADER *param);

/*****************************************************************************/
/*                               媒体播放接口                                */
/*****************************************************************************/

struct  _iXMMEDIA;
#define iXMMEDIA    struct _iXMMEDIA

/* 媒体播放接口表 */
typedef struct
{
        /* 通用组 */
        void_t  (*release) (iXMMEDIA *that);
        void_t* (*getMore) (iXMMEDIA *that, port_t iid);

        /* 播放组 */
        bool_t  (*play) (iXMMEDIA *that);
        bool_t  (*pause) (iXMMEDIA *that);
        bool_t  (*stop) (iXMMEDIA *that);
        bool_t  (*set_pos) (iXMMEDIA *that, int64u curt);
        bool_t  (*get_pos) (iXMMEDIA *that, int64u *curt, int64u *total);
        bool_t  (*is_over) (iXMMEDIA *that, bool_t *over);

        /* 音频组 */
        bool_t  (*set_volume) (iXMMEDIA *that, sint_t percent);
        bool_t  (*get_volume) (iXMMEDIA *that, sint_t *percent);

        /* 视频组 */
        bool_t  (*is_video) (iXMMEDIA *that);
        bool_t  (*video_size) (iXMMEDIA *that, uint_t *width, uint_t *height);
        bool_t  (*video_setwin) (iXMMEDIA *that, hwnd_t hwnd, bool_t full);

} iXMMEDIA_vtbl;

#undef  iXMMEDIA

/* 媒体播放接口类 */
typedef struct  _iXMMEDIA
{
        /* 虚函数表 */
        const iXMMEDIA_vtbl*    __vptr__;

        /* 数据成员 */
        hwnd_t  __hwnd__;       /* 保存窗口对象 */
        bool_t  __full__;       /* 保存是否全屏 */
        sint_t  __volume__;     /* 保存音量设置 */

} iXMMEDIA;

/*
=======================================
    获取窗口对象
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define xmms_get_hwnd(that) \
        ((hwnd_t)((that)->__hwnd__))
#else
inline hwnd_t
xmms_get_hwnd (
  __CR_IN__ const iXMMEDIA* that
    )
{
    return (that->__hwnd__);
}
#endif  /* _CR_NO_INLINE_ */

/*
=======================================
    获取是否全屏
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define xmms_get_full(that) \
        ((bool_t)((that)->__full__))
#else
inline bool_t
xmms_get_full (
  __CR_IN__ const iXMMEDIA* that
    )
{
    return (that->__full__);
}
#endif  /* _CR_NO_INLINE_ */

/*
=======================================
    获取音量设置
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define xmms_get_volume(that) \
        ((sint_t)((that)->__volume__))
#else
inline sint_t
xmms_get_volume (
  __CR_IN__ const iXMMEDIA* that
    )
{
    return (that->__volume__);
}
#endif  /* _CR_NO_INLINE_ */

/*
=======================================
    重新播放媒体
=======================================
*/
inline bool_t
xmms_replay (
  __CR_IN__ iXMMEDIA*   that
    )
{
    if (!CR_VCALL(that)->stop(that)) {
        err_set(__CR_FMTZ_H__, FALSE,
                "xmms_replay()", "iXMMEDIA::stop() failure");
        return (FALSE);
    }
    if (!CR_VCALL(that)->play(that)) {
        err_set(__CR_FMTZ_H__, FALSE,
                "xmms_replay()", "iXMMEDIA::play() failure");
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    修改播放音量
=======================================
*/
inline bool_t
xmms_adj_volume (
  __CR_IO__ iXMMEDIA*   that,
  __CR_IN__ sint_t      value
    )
{
    value += that->__volume__;
    if (!CR_VCALL(that)->set_volume(that, value)) {
        err_set(__CR_FMTZ_H__, FALSE,
                "xmms_adj_volume()", "iXMMEDIA::set_volume() failure");
        return (FALSE);
    }
    return (TRUE);
}

/*****************************************************************************/
/*                               多帧图片接口                                */
/*****************************************************************************/

struct  _iPICTURE;
#define iPICTURE    struct _iPICTURE

/* 多帧图片接口表 */
typedef struct
{
        /* 通用组 */
        void_t  (*release) (iPICTURE *that);
        void_t* (*getMore) (iPICTURE *that, port_t iid);

        /* 获取组 */
        sFMT_PIC*   (*get) (iPICTURE *that, int32u index);

} iPICTURE_vtbl;

#undef  iPICTURE

/* 多帧图片接口类 */
typedef struct  _iPICTURE
{
        /* 虚函数表 */
        const iPICTURE_vtbl*    __vptr__;

        /* 数据成员 */
        int32u  __count__;  /* 子图片数量 */

} iPICTURE;

/*
=======================================
    获取子图片数量
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define pict_get_count(that) \
        ((int32u)((that)->__count__))
#else
inline int32u
pict_get_count (
  __CR_IN__ const iPICTURE* that
    )
{
    return (that->__count__);
}
#endif  /* _CR_NO_INLINE_ */

#endif  /* !__CR_FMTZ_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
