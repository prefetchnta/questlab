/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-09-12  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack SYN FMTZ 插件接口实现 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "fmtint.h"
#include "pixels.h"
#include "fmtz/syn.h"

/* 引擎常数表 */
#undef  _CR_FMTZ_WIDE_
#include "e_syn.inl"
#define _CR_FMTZ_WIDE_
#include "e_syn.inl"
#undef  _CR_FMTZ_WIDE_

/*****************************************************************************/
/*                                 加载引擎                                  */
/*****************************************************************************/

/*
---------------------------------------
    文件格式加载接口
---------------------------------------
*/
static sFMTZ*
engine_syn_load (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
#if defined(_CR_HAVE_PAK_GCA_)
    if (!(engine->mask & CR_FMTZ_MASK_PAK) &&
        !(engine->mask & CR_FMTZ_MASK_PIC))
        return (NULL);
#else
    if (!(engine->mask & CR_FMTZ_MASK_PIC))
        return (NULL);
#endif
    return (fmtz_find(engine, loader));
}

/*
=======================================
    获取引擎插件接口
=======================================
*/
CR_API sENGINE*
engine_syn (void_t)
{
    sENGINE*    engine;

    engine = engine_init(s_finda, s_findw, s_loada, s_loadw);
    if (engine == NULL)
        return (NULL);
    engine->fmtz_load = engine_syn_load;
    engine->info = "SYN FMTz Engine (Done by CrHackOS)";
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
    return (engine_syn());
}
#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/*                                 文件保存                                  */
/*****************************************************************************/

/*
=======================================
    ARGB 文件保存
=======================================
*/
CR_API bool_t
save_img_argb (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    uint_t  hh;
    leng_t  back;
    leng_t  nbpl;
    int32u  vals;
    file_t  file;
    byte_t* line;
    sIMAGE* cnvt;

    /* 创建文件 */
    CR_NOUSE(argc); CR_NOUSE(argv);
    file = file_openA(name, CR_FO_WO);
    if (file == NULL)
        return (FALSE);

    /* 保存文件头 */
    if (!file_putd(mk_tag4("BGRA"), file))
        goto _failure;
    if (!file_putd(0x08080808UL, file))
        goto _failure;
    vals = img->position.ww;
    if (!file_putd_le(vals, file))
        goto _failure;
    vals = img->position.hh;
    if (!file_putd_le(vals, file))
        goto _failure;

    /* 转换格式 */
    if (img->fmt == CR_ARGB8888) {
        cnvt = (sIMAGE*)img;
    }
    else {
        cnvt = img_auto_to_32(NULL, 0, 0, img);
        if (cnvt == NULL)
            goto _failure;
    }

    /* 写入文件 */
    line = cnvt->data;
    hh   = cnvt->position.hh;
    nbpl = cnvt->position.ww;
    nbpl *= sizeof(int32u);
    if (cnvt->gdi)
        line += cnvt->size - cnvt->bpl;
    for (; hh != 0; hh--) {
        back = file_write(line, nbpl, file);
        if (back != nbpl) {
            if (cnvt != (sIMAGE*)img)
                image_del(cnvt);
            goto _failure;
        }
        if (cnvt->gdi)
            line -= cnvt->bpl;
        else
            line += cnvt->bpl;
    }
    if (cnvt != (sIMAGE*)img)
        image_del(cnvt);
    file_close(file);
    return (TRUE);

_failure:
    file_close(file);
    file_deleteA(name);
    return (FALSE);
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
