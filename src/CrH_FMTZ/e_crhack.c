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

#include "fmtint.h"
#include "pixels.h"
#include "strlib.h"

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
    if (engine == NULL)
        return (NULL);
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
    RAW 图片保存
---------------------------------------
*/
static bool_t
save_img_raw (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          fmt,
  __CR_IN__ leng_t          bpc,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    uchar*  line;
    file_t  file;
    uint_t  flag;
    sIMAGE* temp;
    sIMAGE* cnvt;
    leng_t  back, nbpl;
    uint_t  xx, ww, hh;
    /* ------------- */
    byte_t*         dpt;
    byte_t*         dln;
    const byte_t*   spt;
    const byte_t*   sln;

    /* 创建文件 */
    file = file_openA(name, CR_FO_WO);
    if (file == NULL)
        return (FALSE);
    ww = img->position.ww;
    hh = img->position.hh;

    /* 解析参数 [除了最低位外为行对齐字节数] */
    flag = (argc < 1) ? 0 : str2intxA(argv[0], NULL);
    if (flag & 1)
    {
        /* 保存宽高 */
        if (!file_putd(ww, file))
            goto _failure;
        if (!file_putd(hh, file))
            goto _failure;
        flag -= 1;
    }

    /* 转换格式 */
    if (fmt == CR_INDEX8)
    {
        /* 灰度图片 */
        temp = img_auto_to_32(NULL, 0, 0, img);
        if (temp == NULL)
            goto _failure;
        cnvt = image_new(0, 0, ww, hh, CR_INDEX8, temp->gdi, 4);
        if (cnvt == NULL) {
            image_del(temp);
            goto _failure;
        }

        /* 灰度转换 */
        for (sln = temp->data, dln = cnvt->data; hh != 0; hh--) {
            for (spt = sln, dpt = dln, xx = ww; xx != 0; xx--, spt += 4)
                *dpt++ = (byte_t)rgb2light(spt[2], spt[1], spt[0]);
            sln += temp->bpl;
            dln += cnvt->bpl;
        }
        image_del(temp);
    }
    else
    {
        /* 高彩图片 */
        if (img->fmt == fmt) {
            cnvt = (sIMAGE*)img;
        }
        else {
            cnvt = image_new(0, 0, ww, hh, fmt, FALSE, 4);
            if (cnvt == NULL)
                goto _failure;
            if (img_auto_to_xx(cnvt, img) == NULL) {
                image_del(cnvt);
                goto _failure;
            }
        }
    }

    /* 写入文件 */
    line = cnvt->data;
    hh   = cnvt->position.hh;
    nbpl = cnvt->position.ww;
    nbpl *= bpc;
    if (flag != 0) {
        bpc = nbpl % flag;
        if (bpc != 0)
            flag = (uint_t)(flag - bpc);
        else
            flag = 0;
    }
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
        for (xx = 0; xx < flag; xx++) {
            if (!file_putb(0x00, file)) {
                if (cnvt != (sIMAGE*)img)
                    image_del(cnvt);
                goto _failure;
            }
        }
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
    return (save_img_raw(img, name, CR_ARGB565, 2, argc, argv));
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
    return (save_img_raw(img, name, CR_ARGB888, 3, argc, argv));
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
    return (save_img_raw(img, name, CR_ARGBX555, 2, argc, argv));
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
    return (save_img_raw(img, name, CR_ARGB1555, 2, argc, argv));
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
    return (save_img_raw(img, name, CR_ARGB4444, 2, argc, argv));
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
    return (save_img_raw(img, name, CR_ARGB8888, 4, argc, argv));
}

/*
=======================================
    灰度文件保存
=======================================
*/
CR_API bool_t
save_img_grey (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (save_img_raw(img, name, CR_INDEX8, 1, argc, argv));
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
