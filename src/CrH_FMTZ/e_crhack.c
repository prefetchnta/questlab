/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-25  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack FMTZ 插件接口实现 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_E_HACK_C__
#define __CR_E_HACK_C__ 0x0BCA9EE8UL

#include "fmtint.h"
#include "pixels.h"

/*****************************************************************************/
/*                                 加载引擎                                  */
/*****************************************************************************/

/* 引擎常数表 */
#undef  _CR_FMTZ_WIDE_
#include "e_crhack.inl"
#define _CR_FMTZ_WIDE_
#include "e_crhack.inl"
#undef  _CR_FMTZ_WIDE_

/*
---------------------------------------
    文件格式加载接口
---------------------------------------
*/
static sFMTZ*
engine_crhack_load (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    if (!(engine->mask & CR_FMTZ_MASK_PAK) &&
        !(engine->mask & CR_FMTZ_MASK_PIC))
        return (NULL);
    return (fmtz_find(engine, loader));
}

/*
=======================================
    获取引擎插件接口
=======================================
*/
CR_API sENGINE*
engine_crhack (void_t)
{
    sENGINE*    engine;

    engine = engine_init(s_finda, s_findw, s_loada, s_loadw);
    if (engine == NULL) {
        err_set(__CR_E_HACK_C__, CR_NULL,
                "engine_crhack()", "engine_init() failure");
        return (NULL);
    }
    engine->fmtz_load = engine_crhack_load;
    engine->info = "CrHack FMTz Engine (Done by CrHackOS)";
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
    return (engine_crhack());
}
#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/*                                 文件保存                                  */
/*****************************************************************************/

/*
---------------------------------------
    RAW 图片保存 (无行对齐)
---------------------------------------
*/
static bool_t
save_img_raw (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          fmt,
  __CR_IN__ leng_t          bpc
    )
{
    uchar*  line;
    leng_t  back;
    leng_t  nbpl;
    file_t  file;
    uint_t  height;
    sIMAGE* convert;

    /* 创建文件 */
    file = file_openA(name, CR_FO_WO);
    if (file == NULL) {
        err_set(__CR_E_HACK_C__, CR_NULL,
                "save_img_raw()", "file_openA() failure");
        return (FALSE);
    }

    /* 转换格式 */
    if (img->fmt == fmt) {
        convert = (sIMAGE*)img;
    }
    else {
        convert = image_new(0, 0, img->position.ww,
                            img->position.hh, fmt, FALSE, 4);
        if (convert == NULL) {
            err_set(__CR_E_HACK_C__, CR_NULL,
                    "save_img_raw()", "image_new() failure");
            goto _failure;
        }
        if (img_auto_to_xx(convert, img) == NULL) {
            err_set(__CR_E_HACK_C__, CR_NULL,
                    "save_img_raw()", "img_auto_to_xx() failure");
            image_del(convert);
            goto _failure;
        }
    }

    /* 写入文件 */
    line = convert->data;
    height = convert->position.hh;
    nbpl = convert->position.ww;
    nbpl *= bpc;
    if (convert->gdi)
        line += convert->size - convert->bpl;
    for (; height != 0; height--) {
        back = file_write(line, nbpl, file);
        if (back != nbpl) {
            err_set(__CR_E_HACK_C__, back,
                    "save_img_raw()", "file_write() failure");
            if (convert != (sIMAGE*)img)
                image_del(convert);
            goto _failure;
        }
        if (convert->gdi)
            line -= convert->bpl;
        else
            line += convert->bpl;
    }
    if (convert != (sIMAGE*)img)
        image_del(convert);
    file_close(file);
    return (TRUE);

_failure:
    file_close(file);
    file_deleteA(name);
    return (FALSE);
}

/*
=======================================
    565 文件保存
=======================================
*/
CR_API bool_t
save_img_565 (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    CR_NOUSE(argc); CR_NOUSE(argv);
    return (save_img_raw(img, name, CR_ARGB565, 2));
}

/*
=======================================
    888 文件保存
=======================================
*/
CR_API bool_t
save_img_888 (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    CR_NOUSE(argc); CR_NOUSE(argv);
    return (save_img_raw(img, name, CR_ARGB888, 3));
}

/*
=======================================
    X555 文件保存
=======================================
*/
CR_API bool_t
save_img_X555 (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    CR_NOUSE(argc); CR_NOUSE(argv);
    return (save_img_raw(img, name, CR_ARGBX555, 2));
}

/*
=======================================
    1555 文件保存
=======================================
*/
CR_API bool_t
save_img_1555 (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    CR_NOUSE(argc); CR_NOUSE(argv);
    return (save_img_raw(img, name, CR_ARGB1555, 2));
}

/*
=======================================
    4444 文件保存
=======================================
*/
CR_API bool_t
save_img_4444 (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    CR_NOUSE(argc); CR_NOUSE(argv);
    return (save_img_raw(img, name, CR_ARGB4444, 2));
}

/*
=======================================
    8888 文件保存
=======================================
*/
CR_API bool_t
save_img_8888 (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    CR_NOUSE(argc); CR_NOUSE(argv);
    return (save_img_raw(img, name, CR_ARGB8888, 4));
}

#endif  /* !__CR_E_HACK_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
