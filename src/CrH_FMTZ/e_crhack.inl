/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-12-11  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-02-28  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack FMTZ 引擎接口表模板 <<<<<<<<<<<<<<<<<<<<<  */
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
        CR_VFUNC(load_cr_bmp)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.png"), CR_STR(".png"),
        CR_VFUNC(load_cr_png)
    },
    {
        CR_FMTZ_MASK_PAK,
        CR_STR("*.zip"), CR_STR(".zip"),
        CR_VFUNC(load_cr_zip)
    },
};
#endif  /* !s_findx */

/* 引擎的接口表 */
#if !defined(_CR_FMTZ_WIDE_)
static const sTRY_LDR _rom_ s_load[] =
{
    { CR_FMTZ_MASK_PIC, CR_VFUNC(load_cr_bmp) },
    { CR_FMTZ_MASK_PIC, CR_VFUNC(load_cr_png) },
    { CR_FMTZ_MASK_PAK, CR_VFUNC(load_cr_zip) },
};
#endif  /* !_CR_FMTZ_WIDE_ */

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
