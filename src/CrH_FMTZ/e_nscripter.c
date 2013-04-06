/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-04-03  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-04-06  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack NScripter FMTZ 插件接口实现 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_E_NSCRIPTER_C__
#define __CR_E_NSCRIPTER_C__ 0x8F9AE459UL

#include "fmtint.h"
#include "fmtz/nscripter.h"

/* 引擎常数表 */
#undef  _CR_FMTZ_WIDE_
#include "e_nscripter.inl"
#define _CR_FMTZ_WIDE_
#include "e_nscripter.inl"
#undef  _CR_FMTZ_WIDE_

/*
---------------------------------------
    文件格式加载接口
---------------------------------------
*/
static sFMTZ*
engine_nscr_load (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    sFMTZ*  fmtz;
    iDATIN* datin;

    /* 过滤加载类型 */
    if (!(engine->mask & CR_FMTZ_MASK_PAK) &&
        !(engine->mask & CR_FMTZ_MASK_TXT))
        return (NULL);

    /* 生成读取接口 */
    datin = create_file_inX(loader);
    if (datin == NULL) {
        err_set(__CR_E_NSCRIPTER_C__, CR_NULL,
                "engine_nscr_load()", "create_file_inX() failure");
        return (NULL);
    }

    /* 查找匹配 */
    fmtz_find(engine, loader);

    /* 已有匹配则直接使用 */
    if (loader->nprm != NULL) {
        fmtz = ((load_fmtz_t)(loader->nprm))(datin, loader);
        if (fmtz != NULL)
            goto _func_out;
        loader->nprm = NULL;
    }

    fmtz = NULL;
_func_out:
    CR_VCALL(datin)->release(datin);
    return (fmtz);
}

/*
=======================================
    获取引擎插件接口
=======================================
*/
CR_API sENGINE*
engine_nscr (void_t)
{
    sENGINE*    engine;

    engine = engine_init(s_ansi, s_wide, cntsof(s_ansi));
    if (engine == NULL) {
        err_set(__CR_E_NSCRIPTER_C__, CR_NULL,
                "engine_nscr()", "engine_init() failure");
        return (NULL);
    }
    engine->fmtz_load = engine_nscr_load;
    engine->info = "NScripter FMTz Engine (Done by CrHackOS)";
    return (engine);
}

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    获取引擎插件接口 (同名)
=======================================
*/
CR_API sENGINE*
engine_get (void_t)
{
    return (engine_nscr());
}
#endif  /* _CR_BUILD_DLL_ */

#endif  /* !__CR_E_NSCRIPTER_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
