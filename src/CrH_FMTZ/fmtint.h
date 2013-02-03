/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-25  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2012-12-11  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack FMTZ 插件内部头文件 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_FMTINT_H__
#define __CR_FMTINT_H__ 0x284BD973UL

#include "fmtz.h"

/*****************************************************************************/
/*                                 插件部分                                  */
/*****************************************************************************/

/* 引擎插件内部结构 */
typedef struct
{
        /* 通用部分 */
        sENGINE     engine;

        /* 个性部分 */
        sCURTAIN    m_ansi; /* 单字节版文件查找表 */
        sCURTAIN    m_wide; /* 双字节版文件查找表 */

} sENGINE_INT;

/* 自动匹配查找结构 */
typedef struct
{
        int32u          mask;   /* 结果类型掩码 */
        const void_t*   func;   /* 尝试加载接口 */

} sTRY_LDR;

/* 单字节版查找单元 */
typedef struct
{
        int32u          maskz;      /* 结果类型掩码 */
        const ansi_t*   match;      /* 匹配的字符串 */
        const ansi_t*   filext;     /* 匹配的扩展名 */
        const void_t*   loader;     /* 存放加载调用 */

} sMATCHa;

/* 双字节版查找单元 */
typedef struct
{
        int32u          maskz;      /* 结果类型掩码 */
        const wide_t*   match;      /* 匹配的字符串 */
        const wide_t*   filext;     /* 匹配的扩展名 */
        const void_t*   loader;     /* 存放加载调用 */

} sMATCHw;

/***** 插件相关 API 组 (内部使用) *****/
CR_API sENGINE* engine_init (const sMATCHa *ansi,
                             const sMATCHw *wide, uint_t count);
CR_API sFMTZ*   fmtz_load (iDATIN *datin, const sTRY_LDR *ldrs,
                           uint_t count, sLOADER *param, int32u maskz);

/*****************************************************************************/
/*                                 读包部分                                  */
/*****************************************************************************/

/* 内部文件查找单元 */
typedef struct
{
        int64u              index;  /* 文件索引 */
        const sPAK_FILE*    finfo;  /* 文件信息 */

} sFINDER;

/***** 读包相关 API 组 (内部使用) *****/
CR_API bool_t   pack_init_list (iPACKAGE *pack, bool_t caseless);
CR_API void_t   pack_free_list (iPACKAGE *pack);

#endif  /* !__CR_FMTINT_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
