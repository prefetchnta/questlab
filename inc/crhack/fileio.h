/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-23  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 文件操作头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_FILEIO_H__
#define __CR_FILEIO_H__

#include "morder.h"

/*****************************************************************************/
/*                               有用的宏映射                                */
/*****************************************************************************/

/* 确保有这些常数 */
#ifndef SEEK_SET
    #define SEEK_SET    0
    #define SEEK_CUR    1
    #define SEEK_END    2
#endif

/* 文件句柄类型 */
typedef void_t*     fraw_t;
typedef void_t*     fbuf_t;

/* 文件打开模式 */
#define CR_FO_RO      0     /*   只读打开   */
#define CR_FO_WO      1     /*   只写打开   */
#define CR_FO_RW      2     /*   读写打开   */
#define CR_FO_AW      3     /*   结尾添加   */
#define CR_FO_SEQ   0x8000  /* 是否顺序读写 */

/* 文件操作映射 */
#ifndef _CR_NO_BUF_FILEIO_
    #define file_t          fbuf_t
    #define file_openA      file_buf_openA
    #define file_openW      file_buf_openW
    #define file_close      file_buf_close
    #define file_tell       file_buf_tell
    #define file_size       file_buf_size
    #define file_seek       file_buf_seek
    #define file_seek64     file_buf_seek64
    #define file_rewind     file_buf_rewind
    #define file_eof        file_buf_eof
    #define file_flush      file_buf_flush
    #define file_read       file_buf_read
    #define file_write      file_buf_write
    #define file_putb       file_buf_putb
    #define file_putw       file_buf_putw
    #define file_putd       file_buf_putd
    #define file_putq       file_buf_putq
    #define file_getb       file_buf_getb
    #define file_getw       file_buf_getw
    #define file_getd       file_buf_getd
    #define file_getq       file_buf_getq
    #define file_putsA      file_buf_putsA
    #define file_putsW      file_buf_putsW
    #define file_putw_le    file_buf_putw_le
    #define file_putd_le    file_buf_putd_le
    #define file_putq_le    file_buf_putq_le
    #define file_putw_be    file_buf_putw_be
    #define file_putd_be    file_buf_putd_be
    #define file_putq_be    file_buf_putq_be
#else
    #define file_t          fraw_t
    #define file_openA      file_raw_openA
    #define file_openW      file_raw_openW
    #define file_close      file_raw_close
    #define file_tell       file_raw_tell
    #define file_size       file_raw_size
    #define file_seek       file_raw_seek
    #define file_seek64     file_raw_seek64
    #define file_rewind     file_raw_rewind
    #define file_eof        file_raw_eof
    #define file_flush      file_raw_flush
    #define file_read       file_raw_read
    #define file_write      file_raw_write
    #define file_putb       file_raw_putb
    #define file_putw       file_raw_putw
    #define file_putd       file_raw_putd
    #define file_putq       file_raw_putq
    #define file_getb       file_raw_getb
    #define file_getw       file_raw_getw
    #define file_getd       file_raw_getd
    #define file_getq       file_raw_getq
    #define file_putsA      file_raw_putsA
    #define file_putsW      file_raw_putsW
    #define file_putw_le    file_raw_putw_le
    #define file_putd_le    file_raw_putd_le
    #define file_putq_le    file_raw_putq_le
    #define file_putw_be    file_raw_putw_be
    #define file_putd_be    file_raw_putd_be
    #define file_putq_be    file_raw_putq_be
#endif

/*****************************************************************************/
/*                      基本 I/O 操作 (不支持二级缓冲)                       */
/*****************************************************************************/

/* 文件打开关闭 (不支持二级缓冲) */
CR_API fraw_t   file_raw_openA (const ansi_t *name, uint_t mode);
CR_API fraw_t   file_raw_openW (const wide_t *name, uint_t mode);
CR_API void_t   file_raw_close (fraw_t file);

/* 文件指针定位 (不支持二级缓冲) */
CR_API fsize_t  file_raw_tell (fraw_t file);
CR_API fsize_t  file_raw_size (fraw_t file);
CR_API bool_t   file_raw_seek (fraw_t file, fdist_t offset, uint_t whence);
CR_API bool_t   file_raw_seek64 (fraw_t file, int64s offset, uint_t whence);
CR_API bool_t   file_raw_rewind (fraw_t file);
CR_API bool_t   file_raw_eof (fraw_t file);

/* 文件数据读写 (不支持二级缓冲) */
CR_API bool_t   file_raw_flush (fraw_t file);
CR_API leng_t   file_raw_read (void_t *data, leng_t size, fraw_t file);
CR_API leng_t   file_raw_write (const void_t *data, leng_t size, fraw_t file);

CR_API bool_t   file_raw_putb (byte_t val, fraw_t file);
CR_API bool_t   file_raw_putw (int16u val, fraw_t file);
CR_API bool_t   file_raw_putd (int32u val, fraw_t file);
CR_API bool_t   file_raw_putq (int64u val, fraw_t file);
CR_API bool_t   file_raw_getb (byte_t *val, fraw_t file);
CR_API bool_t   file_raw_getw (int16u *val, fraw_t file);
CR_API bool_t   file_raw_getd (int32u *val, fraw_t file);
CR_API bool_t   file_raw_getq (int64u *val, fraw_t file);

CR_API bool_t   file_raw_putsA (const ansi_t *str, fraw_t file);
CR_API bool_t   file_raw_putsW (const wide_t *str, fraw_t file);

#define file_raw_putw_le(val, file)     file_raw_putw( WORD_LE(val), file)
#define file_raw_putd_le(val, file)     file_raw_putd(DWORD_LE(val), file)
#define file_raw_putq_le(val, file)     file_raw_putq(QWORD_LE(val), file)
#define file_raw_putw_be(val, file)     file_raw_putw( WORD_BE(val), file)
#define file_raw_putd_be(val, file)     file_raw_putd(DWORD_BE(val), file)
#define file_raw_putq_be(val, file)     file_raw_putq(QWORD_BE(val), file)

/*****************************************************************************/
/*                       基本 I/O 操作 (支持二级缓冲)                        */
/*****************************************************************************/

/* 文件打开关闭 (支持二级缓冲) */
CR_API fbuf_t   file_buf_openA (const ansi_t *name, uint_t mode);
CR_API fbuf_t   file_buf_openW (const wide_t *name, uint_t mode);
CR_API void_t   file_buf_close (fbuf_t file);

/* 文件指针定位 (支持二级缓冲) */
CR_API fsize_t  file_buf_tell (fbuf_t file);
CR_API fsize_t  file_buf_size (fbuf_t file);
CR_API bool_t   file_buf_seek (fbuf_t file, fdist_t offset, uint_t whence);
CR_API bool_t   file_buf_seek64 (fbuf_t file, int64s offset, uint_t whence);
CR_API bool_t   file_buf_rewind (fbuf_t file);
CR_API bool_t   file_buf_eof (fbuf_t file);

/* 文件数据读写 (支持二级缓冲) */
CR_API bool_t   file_buf_flush (fbuf_t file);
CR_API leng_t   file_buf_read (void_t *data, leng_t size, fbuf_t file);
CR_API leng_t   file_buf_write (const void_t *data, leng_t size, fbuf_t file);

CR_API bool_t   file_buf_putb (byte_t val, fbuf_t file);
CR_API bool_t   file_buf_putw (int16u val, fbuf_t file);
CR_API bool_t   file_buf_putd (int32u val, fbuf_t file);
CR_API bool_t   file_buf_putq (int64u val, fbuf_t file);
CR_API bool_t   file_buf_getb (byte_t *val, fbuf_t file);
CR_API bool_t   file_buf_getw (int16u *val, fbuf_t file);
CR_API bool_t   file_buf_getd (int32u *val, fbuf_t file);
CR_API bool_t   file_buf_getq (int64u *val, fbuf_t file);

CR_API bool_t   file_buf_putsA (const ansi_t *str, fbuf_t file);
CR_API bool_t   file_buf_putsW (const wide_t *str, fbuf_t file);

#define file_buf_putw_le(val, file)     file_buf_putw( WORD_LE(val), file)
#define file_buf_putd_le(val, file)     file_buf_putd(DWORD_LE(val), file)
#define file_buf_putq_le(val, file)     file_buf_putq(QWORD_LE(val), file)
#define file_buf_putw_be(val, file)     file_buf_putw( WORD_BE(val), file)
#define file_buf_putd_be(val, file)     file_buf_putd(DWORD_BE(val), file)
#define file_buf_putq_be(val, file)     file_buf_putq(QWORD_BE(val), file)

/*****************************************************************************/
/*                             文件操作助手函数                              */
/*****************************************************************************/

CR_API fsize_t  file_sizeA (const ansi_t *name);
CR_API fsize_t  file_sizeW (const wide_t *name);

CR_API bool_t   file_existA (const ansi_t *name);
CR_API bool_t   file_existW (const wide_t *name);

CR_API ansi_t*  file_load_as_strA (const ansi_t *name);
CR_API ansi_t*  file_load_as_strW (const wide_t *name);

CR_API void_t*  file_load_as_binA (const ansi_t *name, leng_t *size);
CR_API void_t*  file_load_as_binW (const wide_t *name, leng_t *size);

CR_API bool_t   file_saveA (const ansi_t *name, const void_t *data,
                            leng_t size);
CR_API bool_t   file_saveW (const wide_t *name, const void_t *data,
                            leng_t size);

CR_API fsize_t  file_replaceA (const ansi_t *new_name, const ansi_t *name,
                 leng_t count, const fdist_t offsets[], const fdist_t blocks[],
                               const ansi_t *files[]);

CR_API fsize_t  file_replaceW (const wide_t *new_name, const wide_t *name,
                 leng_t count, const fdist_t offsets[], const fdist_t blocks[],
                               const wide_t *files[]);
/* 文件替换数据处理回调 */
typedef void_t* (*replace_fileA_t) (const ansi_t*, leng_t*, void_t*);
typedef void_t* (*replace_fileW_t) (const wide_t*, leng_t*, void_t*);

CR_API fsize_t  file_replace2A (const ansi_t *new_name, const ansi_t *name,
                                leng_t count, const fdist_t offsets[],
                                const fdist_t blocks[], const ansi_t *files[],
                                replace_fileA_t get_data, void_t *param);

CR_API fsize_t  file_replace2W (const wide_t *new_name, const wide_t *name,
                                leng_t count, const fdist_t offsets[],
                                const fdist_t blocks[], const wide_t *files[],
                                replace_fileW_t get_data, void_t *param);
/* 文件系统部分 */
CR_API bool_t   path_buildA (const ansi_t *path);
CR_API bool_t   path_buildW (const wide_t *path);

CR_API bool_t   file_deleteA (const ansi_t *name);
CR_API bool_t   file_deleteW (const wide_t *name);

CR_API bool_t   file_renameA (const ansi_t *new_name, const ansi_t *old_name);
CR_API bool_t   file_renameW (const wide_t *new_name, const wide_t *old_name);

/* 内存映射文件结构 */
typedef struct
{
        leng_t  size;   /* 文件大小 */
        void_t* data;   /* 缓冲指针 */
        void_t* fmap;   /* 映射句柄 */
        void_t* file;   /* 文件句柄 */

} sVFILE;

CR_API void_t*  file_mappingA (const ansi_t *name, sVFILE *vfile);
CR_API void_t*  file_mappingW (const wide_t *name, sVFILE *vfile);
CR_API void_t   file_release (const sVFILE *vfile);

/* 文件搜索结构A */
typedef struct
{
        fsize_t     size;               /* 文件大小 */
        int32u      attr;               /* 文件属性 */
        uint_t      level;              /* 目录等级 */
        ansi_t      name[MAX_PATHA];    /* 文件名称 */

} sSEARCHa;

/* 文件搜索结构W */
typedef struct
{
        fsize_t     size;               /* 文件大小 */
        int32u      attr;               /* 文件属性 */
        uint_t      level;              /* 目录等级 */
        wide_t      name[MAX_PATHA];    /* 文件名称 */

} sSEARCHw;

/* 文件处理回调类型 */
typedef bool_t  (*search_filesA_t) (void_t*, sSEARCHa*);
typedef bool_t  (*search_filesW_t) (void_t*, sSEARCHw*);

/* [port] 通用文件属性映射 (需要包含相关 API 头文件)
   依次是: 普通, 系统, 隐藏, 存档, 只读, 加密, 临时, 压缩 */
#if     defined(_CR_OS_MSWIN_)
    #define CR_FILE_ATTR_NRM    FILE_ATTRIBUTE_NORMAL
    #define CR_FILE_ATTR_SYS    FILE_ATTRIBUTE_SYSTEM
    #define CR_FILE_ATTR_HID    FILE_ATTRIBUTE_HIDDEN
    #define CR_FILE_ATTR_ARC    FILE_ATTRIBUTE_ARCHIVE
    #define CR_FILE_ATTR_RDO    FILE_ATTRIBUTE_READONLY
    #define CR_FILE_ATTR_DIR    FILE_ATTRIBUTE_DIRECTORY
    #define CR_FILE_ATTR_ENC    FILE_ATTRIBUTE_ENCRYPTED
    #define CR_FILE_ATTR_TMP    FILE_ATTRIBUTE_TEMPORARY
    #define CR_FILE_ATTR_CMP    FILE_ATTRIBUTE_COMPRESSED
#endif

CR_API bool_t   file_searchA (const ansi_t *root, bool_t sub,
                            bool_t hide, bool_t fdir, const ansi_t *match,
                              search_filesA_t handler, void_t *param);

CR_API bool_t   file_searchW (const wide_t *root, bool_t sub,
                            bool_t hide, bool_t fdir, const wide_t *match,
                              search_filesW_t handler, void_t *param);

/*****************************************************************************/
/*                               文件存取接口                                */
/*****************************************************************************/

struct  _iDATIN;
struct  _iDATOT;
#define iDATIN  struct _iDATIN
#define iDATOT  struct _iDATOT

/* 文件读取接口表 */
typedef struct
{
        /* 通用组 */
        void_t  (*release) (iDATIN *that);
        void_t* (*getMore) (iDATIN *that, port_t iid);

        /* 定位组 */
        fsize_t (*tell) (iDATIN *that);
        bool_t  (*seek) (iDATIN *that, fdist_t offset, uint_t whence);
        bool_t  (*seek64) (iDATIN *that, int64s offset, uint_t whence);
        bool_t  (*isEOF)  (iDATIN *that);
        bool_t  (*rewind) (iDATIN *that);

        /* 读取组 */
        bool_t  (*getb_no) (iDATIN *that, byte_t *val);
        bool_t  (*getw_le) (iDATIN *that, int16u *val);
        bool_t  (*getd_le) (iDATIN *that, int32u *val);
        bool_t  (*getq_le) (iDATIN *that, int64u *val);
        bool_t  (*getw_be) (iDATIN *that, int16u *val);
        bool_t  (*getd_be) (iDATIN *that, int32u *val);
        bool_t  (*getq_be) (iDATIN *that, int64u *val);
        byte_t* (*get) (iDATIN *that, leng_t *size, bool_t rel);
        leng_t  (*read)  (iDATIN *that, void_t *data, leng_t size);
        bool_t  (*check) (iDATIN *that, void_t *temp, const ansi_t *tag,
                          uint_t size);
        /* 属性组 */
        bool_t  (*setArea) (iDATIN *that, fdist_t head, fdist_t tail);

} iDATIN_vtbl;

/* 精简接口方法个数 */
#if defined(_CR_ORDER_LE_)
    #define getw_no     getw_le
    #define getd_no     getd_le
    #define getq_no     getq_le
#else   /* (_CR_ORDER_BE_) */
    #define getw_no     getw_be
    #define getd_no     getd_be
    #define getq_no     getq_be
#endif

/* 读取结构体的宏
   判断返回值时必须在整个语句上加括号 */
#define geType(that, data, type) \
        read(that, data, sizeof(type)) == sizeof(type)

/* 文件写入接口表 */
typedef struct
{
        /* 通用组 */
        void_t  (*release) (iDATOT *that);
        void_t* (*getMore) (iDATOT *that, port_t iid);

        /* 写入组 */
        bool_t  (*putb_no) (iDATOT *that, byte_t val);
        bool_t  (*putw_le) (iDATOT *that, int16u val);
        bool_t  (*putd_le) (iDATOT *that, int32u val);
        bool_t  (*putq_le) (iDATOT *that, int64u val);
        bool_t  (*putw_be) (iDATOT *that, int16u val);
        bool_t  (*putd_be) (iDATOT *that, int32u val);
        bool_t  (*putq_be) (iDATOT *that, int64u val);
        bool_t  (*zero)  (iDATOT *that, leng_t size);
        leng_t  (*write) (iDATOT *that, const void_t *data, leng_t size);
        bool_t  (*putsA) (iDATOT *that, const ansi_t *str, leng_t len);
        bool_t  (*putsW) (iDATOT *that, const wide_t *str, leng_t len);

        /* 属性组 */
        void_t* (*flush) (iDATOT *that);
        bool_t  (*reput) (iDATOT *that, fsize_t offset);

} iDATOT_vtbl;

/* 精简接口方法个数 */
#if defined(_CR_ORDER_LE_)
    #define putw_no     putw_le
    #define putd_no     putd_le
    #define putq_no     putq_le
#else   /* (_CR_ORDER_BE_) */
    #define putw_no     putw_be
    #define putd_no     putd_be
    #define putq_no     putq_be
#endif

/* 写入结构体的宏
   判断返回值时必须在整个语句上加括号 */
#define puType(that, data, type) \
       write(that, data, sizeof(type)) == sizeof(type)

#undef  iDATIN
#undef  iDATOT

/* 文件读取接口类 */
typedef struct  _iDATIN
{
        /* 虚函数表 */
        const iDATIN_vtbl*  __vptr__;

        /* 数据成员 */
        fdist_t     __head__;   /* 逻辑起始位置 */
        fdist_t     __tail__;   /* 逻辑结束位置 */
        fdist_t     __size__;   /* 数据逻辑大小 */

} iDATIN;

/*
=======================================
    获取逻辑起始位置
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define dati_get_head(that) \
        ((fsize_t)((that)->__head__))
#else
cr_inline fsize_t
dati_get_head (
  __CR_IN__ const iDATIN*   that
    )
{
    return (that->__head__);
}
#endif  /* _CR_NO_INLINE_ */

/*
=======================================
    获取逻辑结束位置
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define dati_get_tail(that) \
        ((fsize_t)((that)->__tail__))
#else
cr_inline fsize_t
dati_get_tail (
  __CR_IN__ const iDATIN*   that
    )
{
    return (that->__tail__);
}
#endif  /* _CR_NO_INLINE_ */

/*
=======================================
    获取数据逻辑大小
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define dati_get_size(that) \
        ((fsize_t)((that)->__size__))
#else
cr_inline fsize_t
dati_get_size (
  __CR_IN__ const iDATIN*   that
    )
{
    return (that->__size__);
}
#endif  /* _CR_NO_INLINE_ */

/* 文件写入接口类 */
typedef struct  _iDATOT
{
        /* 虚函数表 */
        const iDATOT_vtbl*  __vptr__;

        /* 数据成员 */
        fsize_t     __size__;   /* 当前数据大小 */

} iDATOT;

/*
=======================================
    获取当前数据大小
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define dato_get_size(that) \
        ((fsize_t)((that)->__size__))
#else
cr_inline fsize_t
dato_get_size (
  __CR_IN__ const iDATOT*   that
    )
{
    return (that->__size__);
}
#endif  /* _CR_NO_INLINE_ */

/*****************************************************************************/
/*                             文件存取接口助手                              */
/*****************************************************************************/

/* 位流输入包装结构 */
typedef struct
{
        /* 输入接口 */
        iDATIN* datin;

        /* 读入数据的缓存
           最大支持一次读入4字节 */
        byte_t  buff[4];

        /* 还剩下的位数 */
        uint_t  rest_bits;

        /* 一次读入的字节数 */
        uint_t  cur_size, max_size;

} sBITIN;

/* 建立位流输入结构 */
CR_API void_t   bitin_init (sBITIN *bitin, iDATIN *datin,
                            uint_t max_size);
CR_API void_t   bitin_reset (sBITIN *bitin, uint_t max_size);

/* 高位先读调用 (推挤方式/层叠方式) */
CR_API bool_t   bitin_hi_push (sBITIN *bitin, uint_t *value, ufast_t count);
CR_API bool_t   bitin_hi_casc (sBITIN *bitin, uint_t *value, ufast_t count);

/* 低位先读调用 (推挤方式/层叠方式) */
CR_API bool_t   bitin_lo_push (sBITIN *bitin, uint_t *value, ufast_t count);
CR_API bool_t   bitin_lo_casc (sBITIN *bitin, uint_t *value, ufast_t count);

/*****************************************************************************/
/*                             文件存取接口创建                              */
/*****************************************************************************/

/* 通用文件加载结构 */
typedef struct
{
        uint_t  type;   /* 加载的类型 */
        uint_t  page;   /* 文件名编码 */
        void_t* aprm;   /* 附加的参数 */
        void_t* nprm;   /* 保留的参数 */
        fdist_t head;   /* 文件逻辑头 */
        fdist_t tail;   /* 文件逻辑尾 */
        msize_t smem;   /* 加载的参数 */

        /* 通过名称加载 */
        struct {
                const ansi_t*   ansi;
                const wide_t*   wide;
        } name;

        /* 通过内存加载 */
        struct {
                leng_t          size;
                const void_t*   data;
        } buff;

} sLOADER;

/* 加载的类型值 */
#define CR_LDR_ANSI     0   /* 使用 name.ansi 加载 */
#define CR_LDR_WIDE     1   /* 使用 name.wide 加载 */
#define CR_LDR_BUFF     2   /* 使用 buff 结构 加载 */

#if !defined(_CR_SICK_INLINE_)
/*
=======================================
    使用 ANSI 填充结构
=======================================
*/
cr_inline void_t
set_ldrA (
  __CR_OT__ sLOADER*        that,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ void_t*         aprm,
  __CR_IN__ fdist_t         head,
  __CR_IN__ fdist_t         tail
    )
{
    that->smem = 0;
    that->aprm = aprm;
    that->nprm = NULL;
    that->head = head;
    that->tail = tail;
    that->page = CR_LOCAL;
    that->name.ansi = name;
    that->name.wide = NULL;
    that->type = CR_LDR_ANSI;
}

/*
=======================================
    使用 WIDE 填充结构
=======================================
*/
cr_inline void_t
set_ldrW (
  __CR_OT__ sLOADER*        that,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ void_t*         aprm,
  __CR_IN__ fdist_t         head,
  __CR_IN__ fdist_t         tail
    )
{
    that->smem = 0;
    that->aprm = aprm;
    that->nprm = NULL;
    that->head = head;
    that->tail = tail;
    that->page = CR_LOCAL;
    that->name.ansi = NULL;
    that->name.wide = name;
    that->type = CR_LDR_WIDE;
}

/*
=======================================
    使用 BUFF 填充结构
=======================================
*/
cr_inline void_t
set_ldrM (
  __CR_OT__ sLOADER*        that,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ void_t*         aprm,
  __CR_IN__ fdist_t         head,
  __CR_IN__ fdist_t         tail
    )
{
    that->smem = 0;
    that->aprm = aprm;
    that->nprm = NULL;
    that->head = head;
    that->tail = tail;
    that->page = CR_LOCAL;
    that->buff.size = size;
    that->buff.data = data;
    that->type = CR_LDR_BUFF;
}

#endif  /* !_CR_SICK_INLINE_ */

/* 文件读取接口生成 */
CR_API iDATIN*  create_disk_inA (const ansi_t *name);
CR_API iDATIN*  create_disk_inW (const wide_t *name);

#ifndef _CR_NO_VPAGE_
CR_API iDATIN*  create_page_inA (const ansi_t *name);
CR_API iDATIN*  create_page_inW (const wide_t *name);
#else
    #define create_page_inA     create_disk_inA
    #define create_page_inW     create_disk_inW
#endif

CR_API iDATIN*  create_file_inX (const sLOADER *loader);
CR_API iDATIN*  create_file_inA (const ansi_t *name, msize_t param);
CR_API iDATIN*  create_file_inW (const wide_t *name, msize_t param);

CR_API iDATIN*  create_buff_in (const void_t *data, leng_t size,
                                bool_t rel_after_del CR_DEFAULT(FALSE));
/* 文件写入接口生成 */
CR_API iDATOT*  create_disk_outA (const ansi_t *name);
CR_API iDATOT*  create_disk_outW (const wide_t *name);
CR_API iDATOT*  create_buff_out (leng_t init_size CR_DEFAULT(64));

#endif  /* !__CR_FILEIO_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
