/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-08-07  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-08-13  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack TGL FMTZ 插件接口实现 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_E_TGL_C__
#define __CR_E_TGL_C__ 0xC66CFC41UL

#include "fmtint.h"
#include "fmtz/tgl.h"

/* 引擎常数表 */
#undef  _CR_FMTZ_WIDE_
#include "e_tgl.inl"
#define _CR_FMTZ_WIDE_
#include "e_tgl.inl"
#undef  _CR_FMTZ_WIDE_

/*
---------------------------------------
    文件格式加载接口
---------------------------------------
*/
static sFMTZ*
engine_tgl_load (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    sFMTZ*  fmtz;
    iDATIN* datin;

    /* 过滤加载类型 */
    if (!(engine->mask & CR_FMTZ_MASK_PAK) &&
        !(engine->mask & CR_FMTZ_MASK_DAT))
        return (NULL);

    /* 生成读取接口 */
    datin = create_file_inX(loader);
    if (datin == NULL) {
        err_set(__CR_E_TGL_C__, CR_NULL,
                "engine_tgl_load()", "create_file_inX() failure");
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

        /* 返回文件的头部 */
        if (!CR_VCALL(datin)->seek(datin, 0, SEEK_SET)) {
            err_set(__CR_E_TGL_C__, FALSE,
                    "engine_tgl_load()", "iDATIN::seek() failure");
            goto _func_out;
        }
    }

    /* 否则逐个调用识别格式 */
    fmtz = fmtz_load(datin, s_load, cntsof(s_load),
                     loader, engine->mask);
    if (fmtz == NULL)
        err_set(__CR_E_TGL_C__, CR_NULL,
                "engine_tgl_load()", "fmtz_load() failure");
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
engine_tgl (void_t)
{
    sENGINE*    engine;

    engine = engine_init(s_ansi, s_wide, cntsof(s_ansi));
    if (engine == NULL) {
        err_set(__CR_E_TGL_C__, CR_NULL,
                "engine_tgl()", "engine_init() failure");
        return (NULL);
    }
    engine->fmtz_load = engine_tgl_load;
    engine->info = "TGL FMTz Engine (Done by CrHackOS)";
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
    return (engine_tgl());
}
#endif  /* _CR_BUILD_DLL_ */

#endif  /* !__CR_E_TGL_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
