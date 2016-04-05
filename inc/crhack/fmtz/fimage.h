/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-12-10  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack FreeImage FMTZ 插件头文件 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_FIMAGE_H__
#define __CR_FIMAGE_H__

#include "../fmtz.h"

/*****************************************************************************/
/*                                 引擎接口                                  */
/*****************************************************************************/

CR_API sENGINE*     engine_fimage (void_t);

/*****************************************************************************/
/*                               图片文件格式                                */
/*****************************************************************************/

/***** 文件加载 API 组 *****/
CR_API sFMTZ*   load_fi_bmp  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_cut  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_dds  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_exr  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_g3   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_gif  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_hdr  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_ico  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_iff  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_j2k  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_jng  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_jp2  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_jpg  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_jxr  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_koa  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_mng  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_pbm  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_pcd  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_pcx  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_pfm  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_pgm  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_pic  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_png  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_ppm  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_psd  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_ras  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_raw  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_sgi  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_tga  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_tif  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_wbmp (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_webp (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_xbm  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_xpm  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_auto (iDATIN *datin, const sLOADER *param);

/***** 文件保存 API 组 *****/
CR_API bool_t   save_img_bmp  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_exr  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_gif  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_hdr  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_ico  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_j2k  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_jng  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_jp2  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_jpg  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_jxr  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_pbm  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_pfm  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_pgm  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_png  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_ppm  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_tif  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_tga  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_wbmp (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_webp (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);
CR_API bool_t   save_img_xpm  (const sIMAGE *img, const ansi_t *name,
                               uint_t argc, ansi_t *argv[]);

#endif  /* !__CR_FIMAGE_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
