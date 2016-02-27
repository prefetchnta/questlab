/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-14  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 文本解析头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_PARSER_H__
#define __CR_PARSER_H__

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

        /* 指向接口函数的函数指针 */
        bool_t  (*func) (void_t*, void_t*, sXNODEu*);

} sXC_PORT;

CR_API xmlcaller_t  xmlcall_load (const ansi_t *name, leng_t count,
                                  void_t *aparam);
CR_API bool_t       xmlcall_setup (xmlcaller_t xmlcall, const sXC_PORT *port);
CR_API void_t       xmlcall_unload (xmlcaller_t xmlcall);
CR_API bool_t       xmlcall_exec (xmlcaller_t xmlcall, void_t *uparam,
                                  const ansi_t *script, sXMLu *preload);

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
