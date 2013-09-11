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

#include "fmtz.h"

/*****************************************************************************/
/*                                 引擎接口                                  */
/*****************************************************************************/

CR_API sENGINE*     engine_fimage (void_t);

/*****************************************************************************/
/*                               图片文件格式                                */
/*****************************************************************************/

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
CR_API sFMTZ*   load_fi_xbm  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_xpm  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_fi_auto (iDATIN *datin, const sLOADER *param);

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
