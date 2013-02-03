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
/*  >>>>>>>>>>>>>>>>>>> CrHack DevIL FMTZ 引擎接口表模板 <<<<<<<<<<<<<<<<<<  */
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
        CR_VFUNC(load_il_bmp)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.cut"), CR_STR(".cut"),
        CR_VFUNC(load_il_cut)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.ico"), CR_STR(".ico"),
        CR_VFUNC(load_il_ico)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.cur"), CR_STR(".cur"),
        CR_VFUNC(load_il_ico)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.jpg"), CR_STR(".jpg"),
        CR_VFUNC(load_il_jpg)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.jpe"), CR_STR(".jpe"),
        CR_VFUNC(load_il_jpg)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.jpeg"), CR_STR(".jpeg"),
        CR_VFUNC(load_il_jpg)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.iff"), CR_STR(".iff"),
        CR_VFUNC(load_il_ilbm)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.ilbm"), CR_STR(".ilbm"),
        CR_VFUNC(load_il_ilbm)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.lbm"), CR_STR(".lbm"),
        CR_VFUNC(load_il_ilbm)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pcd"), CR_STR(".pcd"),
        CR_VFUNC(load_il_pcd)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pcx"), CR_STR(".pcx"),
        CR_VFUNC(load_il_pcx)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pic"), CR_STR(".pic"),
        CR_VFUNC(load_il_pic)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.png"), CR_STR(".png"),
        CR_VFUNC(load_il_png)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pnm"), CR_STR(".pnm"),
        CR_VFUNC(load_il_pnm)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.sgi"), CR_STR(".sgi"),
        CR_VFUNC(load_il_sgi)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.bw"), CR_STR(".bw"),
        CR_VFUNC(load_il_sgi)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.rgb"), CR_STR(".rgb"),
        CR_VFUNC(load_il_sgi)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.rgba"), CR_STR(".rgba"),
        CR_VFUNC(load_il_sgi)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.tga"), CR_STR(".tga"),
        CR_VFUNC(load_il_tga)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.vda"), CR_STR(".vda"),
        CR_VFUNC(load_il_tga)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.icb"), CR_STR(".icb"),
        CR_VFUNC(load_il_tga)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.vst"), CR_STR(".vst"),
        CR_VFUNC(load_il_tga)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.tif"), CR_STR(".tif"),
        CR_VFUNC(load_il_tif)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.tiff"), CR_STR(".tiff"),
        CR_VFUNC(load_il_tif)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.h"), CR_STR(".h"),
        CR_VFUNC(load_il_chead)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.mdl"), CR_STR(".mdl"),
        CR_VFUNC(load_il_mdl)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.wal"), CR_STR(".wal"),
        CR_VFUNC(load_il_wal)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.lif"), CR_STR(".lif"),
        CR_VFUNC(load_il_lif)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.mng"), CR_STR(".mng"),
        CR_VFUNC(load_il_mng)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.gif"), CR_STR(".gif"),
        CR_VFUNC(load_il_gif)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.dds"), CR_STR(".dds"),
        CR_VFUNC(load_il_dds)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.dcx"), CR_STR(".dcx"),
        CR_VFUNC(load_il_dcx)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.psd"), CR_STR(".psd"),
        CR_VFUNC(load_il_psd)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.psp"), CR_STR(".psp"),
        CR_VFUNC(load_il_psp)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pix"), CR_STR(".pix"),
        CR_VFUNC(load_il_pix)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.pxr"), CR_STR(".pxr"),
        CR_VFUNC(load_il_pxr)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.xpm"), CR_STR(".xpm"),
        CR_VFUNC(load_il_xpm)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.hdr"), CR_STR(".hdr"),
        CR_VFUNC(load_il_hdr)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.icns"), CR_STR(".icns"),
        CR_VFUNC(load_il_icns)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.jp2"), CR_STR(".jp2"),
        CR_VFUNC(load_il_jp2)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.exr"), CR_STR(".exr"),
        CR_VFUNC(load_il_exr)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.wdp"), CR_STR(".wdp"),
        CR_VFUNC(load_il_wdp)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.hdp"), CR_STR(".hdp"),
        CR_VFUNC(load_il_wdp)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.vtf"), CR_STR(".vtf"),
        CR_VFUNC(load_il_vtf)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.wbmp"), CR_STR(".wbmp"),
        CR_VFUNC(load_il_wbmp)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.sun"), CR_STR(".sun"),
        CR_VFUNC(load_il_sun)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.ras"), CR_STR(".ras"),
        CR_VFUNC(load_il_sun)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.rs"), CR_STR(".rs"),
        CR_VFUNC(load_il_sun)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.im1"), CR_STR(".im1"),
        CR_VFUNC(load_il_sun)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.im8"), CR_STR(".im8"),
        CR_VFUNC(load_il_sun)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.im24"), CR_STR(".im24"),
        CR_VFUNC(load_il_sun)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.im32"), CR_STR(".im32"),
        CR_VFUNC(load_il_sun)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.iff"), CR_STR(".iff"),
        CR_VFUNC(load_il_iff)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.tpl"), CR_STR(".tpl"),
        CR_VFUNC(load_il_tpl)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.fit"), CR_STR(".fit"),
        CR_VFUNC(load_il_fits)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.fits"), CR_STR(".fits"),
        CR_VFUNC(load_il_fits)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.dcm"), CR_STR(".dcm"),
        CR_VFUNC(load_il_dicom)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.dicom"), CR_STR(".dicom"),
        CR_VFUNC(load_il_dicom)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.iwi"), CR_STR(".iwi"),
        CR_VFUNC(load_il_iwi)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.blp"), CR_STR(".blp"),
        CR_VFUNC(load_il_blp)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.ftx"), CR_STR(".ftx"),
        CR_VFUNC(load_il_ftx)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.rot"), CR_STR(".rot"),
        CR_VFUNC(load_il_rot)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.texture"), CR_STR(".texture"),
        CR_VFUNC(load_il_tex)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.dpx"), CR_STR(".dpx"),
        CR_VFUNC(load_il_dpx)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.utx"), CR_STR(".utx"),
        CR_VFUNC(load_il_utx)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.mp3"), CR_STR(".mp3"),
        CR_VFUNC(load_il_mp3)
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
