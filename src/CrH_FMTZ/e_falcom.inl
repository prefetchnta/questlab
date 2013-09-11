/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-08-13  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack FALCOM FMTZ 引擎接口表模板 <<<<<<<<<<<<<<<<<<  */
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
        NULL, CR_STR(".aia"),
        CR_VFUNC(load_flc_aia)
    },
    {
        CR_FMTZ_MASK_PIC,
        CR_STR("*.aia.z"), CR_STR(".z"),
        CR_VFUNC(load_flc_aia)
    },
    {
        CR_FMTZ_MASK_PAK,
        NULL, CR_STR(".na"),
        CR_VFUNC(load_flc_nnk)
    },
    {
        CR_FMTZ_MASK_DAT,
        NULL, CR_STR(".z"),
        CR_VFUNC(load_flc_zed)
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
    { CR_FMTZ_MASK_PIC, CR_VFUNC(load_flc_aia), NULL },
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
