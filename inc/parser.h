/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-14  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2012-11-23  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 文本解析头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_PARSER_H__
#define __CR_PARSER_H__ 0x048914ACUL

#include "defs.h"

/*****************************************************************************/
/*                                INI 解析器                                 */
/*****************************************************************************/

/* 解析器结构U */
typedef struct
{
        ansi_t*     text;       /* 打断的文本 */
        ansi_t**    lines;      /* 文本行指针 */
        leng_t      count;      /* 文本的行数 */
                                /* 返回的部分 */
        leng_t      size;       /* 字符串大小 */
        bool_t      found;      /* 是否找到键 */
} sINIu;

/* 解析器结构W */
typedef struct
{
        wide_t*     text;       /* 打断的文本 */
        wide_t**    lines;      /* 文本行指针 */
        leng_t      count;      /* 文本的行数 */
                                /* 返回的部分 */
        leng_t      size;       /* 字符串大小 */
        bool_t      found;      /* 是否找到键 */
} sINIw;

CR_API sINIu*   ini_parseU (const ansi_t *str);
CR_API sINIw*   ini_parseW (const wide_t *str);
CR_API void_t   ini_closeU (sINIu *ini);
CR_API void_t   ini_closeW (sINIw *ini);
CR_API sINIu*   ini_from_textU (const ansi_t *text);
CR_API sINIw*   ini_from_textW (const ansi_t *text);

CR_API real_t   ini_key_realU (const ansi_t *name, real_t def, sINIu *ini);
CR_API real_t   ini_key_realW (const wide_t *name, real_t def, sINIw *ini);
CR_API fp32_t   ini_key_fp32U (const ansi_t *name, fp32_t def, sINIu *ini);
CR_API fp32_t   ini_key_fp32W (const wide_t *name, fp32_t def, sINIw *ini);
CR_API fp64_t   ini_key_fp64U (const ansi_t *name, fp64_t def, sINIu *ini);
CR_API fp64_t   ini_key_fp64W (const wide_t *name, fp64_t def, sINIw *ini);

CR_API uint_t   ini_key_intxU   (const ansi_t *name, uint_t def, sINIu *ini);
CR_API uint_t   ini_key_intxW   (const wide_t *name, uint_t def, sINIw *ini);
CR_API int32u   ini_key_intx32U (const ansi_t *name, int32u def, sINIu *ini);
CR_API int32u   ini_key_intx32W (const wide_t *name, int32u def, sINIw *ini);
CR_API int64u   ini_key_intx64U (const ansi_t *name, int64u def, sINIu *ini);
CR_API int64u   ini_key_intx64W (const wide_t *name, int64u def, sINIw *ini);

CR_API ansi_t*  ini_key_stringU (const ansi_t *name, sINIu *ini);
CR_API wide_t*  ini_key_stringW (const wide_t *name, sINIw *ini);
CR_API ansi_t*  ini_key_bufferU (const ansi_t *name, sINIu *ini);
CR_API wide_t*  ini_key_bufferW (const wide_t *name, sINIw *ini);
CR_API bool_t   ini_check_stringU (const ansi_t *str);
CR_API bool_t   ini_check_stringW (const wide_t *str);

/*****************************************************************************/
/*                                XML 解析器                                 */
/*****************************************************************************/

/* 节点结构U */
typedef struct
{
        ansi_t*     name;       /*  节点的名称  */
        ansi_t*     node;       /*  节点的数据  */
        ansi_t*     attr;       /*  属性的数据  */
        ansi_t*     shadow;     /* 属性数据辅助 */
        bool_t      closed;     /* 是否封闭节点 */
                                /*  返回的部分  */
        leng_t      size;       /* 字符串的大小 */
        bool_t      found;      /* 是否找到属性 */

} sXNODEu;

/* 节点结构W */
typedef struct
{
        wide_t*     name;       /*  节点的名称  */
        wide_t*     node;       /*  节点的数据  */
        wide_t*     attr;       /*  属性的数据  */
        wide_t*     shadow;     /* 属性数据辅助 */
        bool_t      closed;     /* 是否封闭节点 */
                                /*  返回的部分  */
        leng_t      size;       /* 字符串的大小 */
        bool_t      found;      /* 是否找到属性 */

} sXNODEw;

/* 解析器结构U */
typedef struct
{
        ansi_t*     text;       /* 打断的文本 */
        leng_t      count;      /* 节点的个数 */
        sXNODEu*    nodes;      /* 节点的列表 */
} sXMLu;

/* 解析器结构W */
typedef struct
{
        wide_t*     text;       /* 打断的文本 */
        leng_t      count;      /* 节点的个数 */
        sXNODEw*    nodes;      /* 节点的列表 */
} sXMLw;

CR_API sXMLu*   xml_parseU (const ansi_t *str);
CR_API sXMLw*   xml_parseW (const wide_t *str);
CR_API void_t   xml_closeU (sXMLu *xml);
CR_API void_t   xml_closeW (sXMLw *xml);
CR_API sXMLu*   xml_from_textU (const ansi_t *text);
CR_API sXMLw*   xml_from_textW (const ansi_t *text);

CR_API real_t   xml_node_realU (real_t def, sXNODEu *node);
CR_API real_t   xml_node_realW (real_t def, sXNODEw *node);
CR_API fp32_t   xml_node_fp32U (fp32_t def, sXNODEu *node);
CR_API fp32_t   xml_node_fp32W (fp32_t def, sXNODEw *node);
CR_API fp64_t   xml_node_fp64U (fp64_t def, sXNODEu *node);
CR_API fp64_t   xml_node_fp64W (fp64_t def, sXNODEw *node);

CR_API uint_t   xml_node_intxU   (uint_t def, sXNODEu *node);
CR_API uint_t   xml_node_intxW   (uint_t def, sXNODEw *node);
CR_API int32u   xml_node_intx32U (int32u def, sXNODEu *node);
CR_API int32u   xml_node_intx32W (int32u def, sXNODEw *node);
CR_API int64u   xml_node_intx64U (int64u def, sXNODEu *node);
CR_API int64u   xml_node_intx64W (int64u def, sXNODEw *node);

CR_API ansi_t*  xml_node_stringU (sXNODEu *node);
CR_API wide_t*  xml_node_stringW (sXNODEw *node);
CR_API ansi_t*  xml_node_bufferU (sXNODEu *node);
CR_API wide_t*  xml_node_bufferW (sXNODEw *node);

CR_API real_t   xml_attr_realU (const ansi_t *name,real_t def,sXNODEu *node);
CR_API real_t   xml_attr_realW (const wide_t *name,real_t def,sXNODEw *node);
CR_API fp32_t   xml_attr_fp32U (const ansi_t *name,fp32_t def,sXNODEu *node);
CR_API fp32_t   xml_attr_fp32W (const wide_t *name,fp32_t def,sXNODEw *node);
CR_API fp64_t   xml_attr_fp64U (const ansi_t *name,fp64_t def,sXNODEu *node);
CR_API fp64_t   xml_attr_fp64W (const wide_t *name,fp64_t def,sXNODEw *node);

CR_API uint_t   xml_attr_intxU   (const ansi_t *name,uint_t def,sXNODEu *node);
CR_API uint_t   xml_attr_intxW   (const wide_t *name,uint_t def,sXNODEw *node);
CR_API int32u   xml_attr_intx32U (const ansi_t *name,int32u def,sXNODEu *node);
CR_API int32u   xml_attr_intx32W (const wide_t *name,int32u def,sXNODEw *node);
CR_API int64u   xml_attr_intx64U (const ansi_t *name,int64u def,sXNODEu *node);
CR_API int64u   xml_attr_intx64W (const wide_t *name,int64u def,sXNODEw *node);

CR_API ansi_t*  xml_attr_stringU (const ansi_t *name, sXNODEu *node);
CR_API wide_t*  xml_attr_stringW (const wide_t *name, sXNODEw *node);
CR_API ansi_t*  xml_attr_bufferU (const ansi_t *name, sXNODEu *node);
CR_API wide_t*  xml_attr_bufferW (const wide_t *name, sXNODEw *node);

CR_API bool_t   xml_check_stringU (const ansi_t *str);
CR_API bool_t   xml_check_stringW (const wide_t *str);
CR_API leng_t   xml_skip_tagU (const sXNODEu *nodes,
                               leng_t index, leng_t count);
CR_API leng_t   xml_skip_tagW (const sXNODEw *nodes,
                               leng_t index, leng_t count);

/*****************************************************************************/
/*                                XML 调用器                                 */
/*****************************************************************************/

/* XML 调用器类型 */
typedef void_t*     xmlcaller_t;

/* XML 调用插件入口结构 */
typedef struct
{
        /* 最后一个接口项为 NULL */
        const ansi_t*   name;   /* 调用的名称 */

        /* 指向接口函数的函数指针 (返回0成功) */
        uint_t  (*func) (void_t*, void_t*, sXNODEu*);

} sXC_PORT;

CR_API xmlcaller_t  xmlcall_load (const ansi_t *name, void_t *aparam);
CR_API xmlcaller_t  xmlcall_setup (const sXC_PORT *port, void_t *aparam);
CR_API void_t       xmlcall_unload (xmlcaller_t xmlcall);
CR_API bool_t       xmlcall_exec (xmlcaller_t xmlcall, void_t *uparam,
                                  const ansi_t *script);

/*****************************************************************************/
/*                             序列化/反序列化                               */
/*****************************************************************************/

/* 结构字段结构 */
typedef struct
{
        leng_t          offs;   /* 字段的偏移 */
        leng_t          idxs;   /* 数组的起始 */
        leng_t          cnts;   /* 数组的个数 */
        leng_t          unit;   /* 单元的大小 */
        leng_t          size;   /* 字段的大小 */
        bool_t          skip;   /* 忽略的成员 */
        bool_t          hash;   /* 是否为校验 */
        uint_t          attr;   /* 附加的属性 */
        const wide_t*   name;   /* 字段的名称 */
        const wide_t*   type;   /* 字段类型名 */
        const ansi_t*   show;   /* 字段的名称 */

} sSRZ_MEMB;

/* 校验出错结构 */
typedef struct
{
        leng_t          err_idxs;   /* 出错的索引值 */
        const wide_t*   err_type;   /* 出错的类型名 */
        const wide_t*   err_memb;   /* 出错的成员名 */

} sSRZ_HASH;

/* 有用的类型 */
typedef void_t*   stru_t;   /* 序列化对象类型 */
typedef bool_t  (*safe_check_t) (void_t*, leng_t);
typedef bool_t  (*bin_hash_t) (void_t*, leng_t, leng_t);
typedef void_t  (*err_hash_t) (void_t*, const sSRZ_HASH*);
typedef ansi_t* (*bin2str_t) (const void_t*, leng_t,
                              const sSRZ_MEMB*);
typedef bool_t  (*str2bin_t) (void_t*, leng_t, uint_t, leng_t,
                              uint_t, sXNODEw*);
/* 数据类型结构 */
typedef struct
{
        bool_t          real;   /* 是否为原生 */
        bool_t          free;   /* 是否要释放 */
        leng_t          size;   /* 类型的大小 */
        const wide_t*   name;   /* 类型的名称 */
        safe_check_t    safe;   /* 类型的限制 */

        /* 原生类型的相关属性 */
        bin2str_t   bin2str;    /* 二进制转字符串 */
        str2bin_t   str2bin;    /* 字符串转二进制 */

        /* 嵌套类型的相关属性 */
        leng_t              count;  /* 子成员个数 */
        const sSRZ_MEMB*    membs;  /* 子成员列表 */

} sSRZ_TYPE;

/***** 解析组 *****/
CR_API void_t   serial_free (stru_t serial);
CR_API uint_t   serial_codepage (stru_t serial);
CR_API void_t   serial_set_style (stru_t serial, const ansi_t *style);
CR_API stru_t   serial_init (uint_t types_n, uint_t value_n, uint_t safer_n,
                             uint_t codepage, const ansi_t *encoding);
CR_API bool_t   serial_set_type (stru_t serial, const sSRZ_TYPE *real_type);
CR_API bool_t   serial_set_check (stru_t serial, safe_check_t safer,
                                  const wide_t *name);
CR_API bool_t   serial_get_value (stru_t serial, leng_t *value,
                                  const wide_t *name);
CR_API bool_t   serial_set_value (stru_t serial, leng_t value,
                                  const wide_t *name, bool_t is_free);
CR_API bool_t   serial_setup (stru_t serial, const ansi_t *header);
CR_API bool_t   serial_find_type (stru_t serial, sSRZ_TYPE *stru_type,
                                  const wide_t *type);
CR_API bool_t   serial_find_memb (stru_t serial, sSRZ_MEMB *member,
                                  const wide_t *type, const wide_t *path);
/***** 数据组 *****/
CR_API bool_t   serial_hash (stru_t serial, void_t *data, leng_t size,
                             const sSRZ_MEMB *member, bin_hash_t hasher,
                             err_hash_t errors, void_t *param);
CR_API ansi_t*  serial_bin2str (stru_t serial, const void_t *data,
                                leng_t size, const sSRZ_MEMB *member);
CR_API bool_t   serial_str2bin (stru_t serial, void_t *data, leng_t size,
                                const ansi_t *text, const sSRZ_MEMB *member);

#endif  /* !__CR_PARSER_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
