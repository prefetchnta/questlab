/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-12-11  */
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
/*  >>>>>>>>>>>>>>>>> CrHack FreeImage FMTZ 引擎接口表模板 <<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef _CR_FMTZ_WIDE_
    #define CR_STR      CR_AS
    #define s_findx     s_finda
    #define s_loadx     s_loada
    #define sMATCHx     sMATCHa
    #define sTRY_LDRx   sTRY_LDRa
#else
#ifndef _CR_NO_WIDE_
    #define CR_STR      CR_WS
    #define s_findx     s_findw
    #define s_loadx     s_loadw
    #define sMATCHx     sMATCHw
    #define sTRY_LDRx   sTRY_LDRw
#endif
#endif

/* 文件名匹配表 */
#if !defined(s_findx)
    #define s_findw NULL
#else
static const sMATCHx _rom_ s_findx[] =
{
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".bmp"),
        CR_VFUNC(load_fi_bmp)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".cut"),
        CR_VFUNC(load_fi_cut)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".dds"),
        CR_VFUNC(load_fi_dds)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".exr"),
        CR_VFUNC(load_fi_exr)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".g3"),
        CR_VFUNC(load_fi_g3)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".gif"),
        CR_VFUNC(load_fi_gif)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".hdr"),
        CR_VFUNC(load_fi_hdr)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".ico"),
        CR_VFUNC(load_fi_ico)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".iff"),
        CR_VFUNC(load_fi_iff)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".lbm"),
        CR_VFUNC(load_fi_iff)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".j2k"),
        CR_VFUNC(load_fi_j2k)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".j2c"),
        CR_VFUNC(load_fi_j2k)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".jng"),
        CR_VFUNC(load_fi_jng)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".jp2"),
        CR_VFUNC(load_fi_jp2)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".jpg"),
        CR_VFUNC(load_fi_jpg)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".jif"),
        CR_VFUNC(load_fi_jpg)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".jpeg"),
        CR_VFUNC(load_fi_jpg)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".jpe"),
        CR_VFUNC(load_fi_jpg)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".jxr"),
        CR_VFUNC(load_fi_jxr)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".wdp"),
        CR_VFUNC(load_fi_jxr)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".hdp"),
        CR_VFUNC(load_fi_jxr)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".koa"),
        CR_VFUNC(load_fi_koa)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".mng"),
        CR_VFUNC(load_fi_mng)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".pbm"),
        CR_VFUNC(load_fi_pbm)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".pcd"),
        CR_VFUNC(load_fi_pcd)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".pcx"),
        CR_VFUNC(load_fi_pcx)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".pfm"),
        CR_VFUNC(load_fi_pfm)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".pgm"),
        CR_VFUNC(load_fi_pgm)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".pct"),
        CR_VFUNC(load_fi_pic)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".pict"),
        CR_VFUNC(load_fi_pic)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".pic"),
        CR_VFUNC(load_fi_pic)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".png"),
        CR_VFUNC(load_fi_png)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".ppm"),
        CR_VFUNC(load_fi_ppm)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".psd"),
        CR_VFUNC(load_fi_psd)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".ras"),
        CR_VFUNC(load_fi_ras)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".sgi"),
        CR_VFUNC(load_fi_sgi)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".tga"),
        CR_VFUNC(load_fi_tga)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".targa"),
        CR_VFUNC(load_fi_tga)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".tif"),
        CR_VFUNC(load_fi_tif)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".tiff"),
        CR_VFUNC(load_fi_tif)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".wbmp"),
        CR_VFUNC(load_fi_wbmp)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".webp"),
        CR_VFUNC(load_fi_webp)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".xbm"),
        CR_VFUNC(load_fi_xbm)
    },
    {
        CR_FMTZ_MASK_PIC,
        NULL, CR_STR(".xpm"),
        CR_VFUNC(load_fi_xpm)
    },
    { 0, NULL, NULL, NULL },
};
#endif  /* !s_findx */

/* 接口的尝试表 */
#if !defined(s_loadx)
    #define s_loadw NULL
#else
static const sTRY_LDRx _rom_ s_loadx[] =
{
    { CR_FMTZ_MASK_PIC, CR_VFUNC(load_fi_auto), NULL },
    { 0, NULL, NULL },
};
#endif  /* !s_loadx */

#undef  CR_STR
#undef  s_findx
#undef  s_loadx
#undef  sMATCHx
#undef  sTRY_LDRx

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
