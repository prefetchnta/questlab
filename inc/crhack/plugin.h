/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-17  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 插件操作头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_PLUGIN_H__
#define __CR_PLUGIN_H__

#include "defs.h"

/*****************************************************************************/
/*                               动态模块相关                                */
/*****************************************************************************/

/* 动态模块句柄类型 */
typedef void_t*     sbin_t;

/* 加载卸载动态模块 */
CR_API sbin_t   sbin_loadA (const ansi_t *name);
CR_API sbin_t   sbin_loadW (const wide_t *name);
CR_API bool_t   sbin_unload (sbin_t sbin);

/* 获取模块导出项地址 */
CR_API void_t*  sbin_export (sbin_t sbin, const ansi_t *name);

/* 获取已加载的模块 */
CR_API sbin_t   sbin_testA (const ansi_t *name);
CR_API sbin_t   sbin_testW (const wide_t *name);

/* 调用接口获取接口表 */
CR_API void_t*  sbin_callget (sbin_t sbin, const ansi_t *name);

/* 导出项的类型包装宏 */
#define sbin_exportT(sbin, name, type) \
                (type)sbin_export(sbin, name)

#define sbin_callgetT(sbin, name, type) \
            (const type*)sbin_callget(sbin, name)

#endif  /* !__CR_PLUGIN_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
