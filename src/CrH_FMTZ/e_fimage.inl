/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-12-11  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-01-31  */
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
/*  >>>>>>>>>>>>>>>>> CrHack FreeImage FMTZ 引擎接口表模板 <<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef _CR_FMTZ_WIDE_
    #define CR_STR  CR_AS
    #define s_findx s_ansi
    #define sMATCHx sMATCHa
#else
    #define CR_STR  CR_WS
    #ifndef _CR_NO_WIDE_
        #define s_findx s_wide
        #define sMATCHx sMATCHw
    #else
        #undef  s_findx
    #endif
#endif

/* 引擎的匹配表 */
#if !defined(s_findx)
    #define s_wide  NULL
#else
static const sMATCHx _rom_ s_findx[] =
{
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.bmp"), CR_STR(".bmp"),
        CR_VFUNC(load_fi_bmp)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.cut"), CR_STR(".cut"),
        CR_VFUNC(load_fi_cut)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.dds"), CR_STR(".dds"),
        CR_VFUNC(load_fi_dds)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.exr"), CR_STR(".exr"),
        CR_VFUNC(load_fi_exr)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.g3"), CR_STR(".g3"),
        CR_VFUNC(load_fi_g3)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.gif"), CR_STR(".gif"),
        CR_VFUNC(load_fi_gif)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.hdr"), CR_STR(".hdr"),
        CR_VFUNC(load_fi_hdr)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.ico"), CR_STR(".ico"),
        CR_VFUNC(load_fi_ico)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.iff"), CR_STR(".iff"),
        CR_VFUNC(load_fi_iff)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.lbm"), CR_STR(".lbm"),
        CR_VFUNC(load_fi_iff)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.j2k"), CR_STR(".j2k"),
        CR_VFUNC(load_fi_j2k)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.j2c"), CR_STR(".j2c"),
        CR_VFUNC(load_fi_j2k)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.jng"), CR_STR(".jng"),
        CR_VFUNC(load_fi_jng)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.jp2"), CR_STR(".jp2"),
        CR_VFUNC(load_fi_jp2)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.jpg"), CR_STR(".jpg"),
        CR_VFUNC(load_fi_jpg)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.jif"), CR_STR(".jif"),
        CR_VFUNC(load_fi_jpg)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.jpeg"), CR_STR(".jpeg"),
        CR_VFUNC(load_fi_jpg)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.jpe"), CR_STR(".jpe"),
        CR_VFUNC(load_fi_jpg)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.koa"), CR_STR(".koa"),
        CR_VFUNC(load_fi_koa)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.mng"), CR_STR(".mng"),
        CR_VFUNC(load_fi_mng)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pbm"), CR_STR(".pbm"),
        CR_VFUNC(load_fi_pbm)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pcd"), CR_STR(".pcd"),
        CR_VFUNC(load_fi_pcd)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pcx"), CR_STR(".pcx"),
        CR_VFUNC(load_fi_pcx)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pfm"), CR_STR(".pfm"),
        CR_VFUNC(load_fi_pfm)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pgm"), CR_STR(".pgm"),
        CR_VFUNC(load_fi_pgm)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pct"), CR_STR(".pct"),
        CR_VFUNC(load_fi_pic)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pict"), CR_STR(".pict"),
        CR_VFUNC(load_fi_pic)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pic"), CR_STR(".pic"),
        CR_VFUNC(load_fi_pic)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.png"), CR_STR(".png"),
        CR_VFUNC(load_fi_png)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.ppm"), CR_STR(".ppm"),
        CR_VFUNC(load_fi_ppm)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.psd"), CR_STR(".psd"),
        CR_VFUNC(load_fi_psd)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.ras"), CR_STR(".ras"),
        CR_VFUNC(load_fi_ras)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.sgi"), CR_STR(".sgi"),
        CR_VFUNC(load_fi_sgi)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.tga"), CR_STR(".tga"),
        CR_VFUNC(load_fi_tga)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.targa"), CR_STR(".targa"),
        CR_VFUNC(load_fi_tga)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.tif"), CR_STR(".tif"),
        CR_VFUNC(load_fi_tif)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.tiff"), CR_STR(".tiff"),
        CR_VFUNC(load_fi_tif)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.wbmp"), CR_STR(".wbmp"),
        CR_VFUNC(load_fi_wbmp)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.xbm"), CR_STR(".xbm"),
        CR_VFUNC(load_fi_xbm)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.xpm"), CR_STR(".xpm"),
        CR_VFUNC(load_fi_xpm)
    },
};
#endif  /* !s_findx */

#undef  CR_STR
#undef  s_findx
#undef  sMATCHx

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
