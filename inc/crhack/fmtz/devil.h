/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-09-20  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack DevIL FMTZ 插件头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DEVIL_H__
#define __CR_DEVIL_H__

#include "../fmtz.h"

/*****************************************************************************/
/*                                 引擎接口                                  */
/*****************************************************************************/

CR_API bool_t       devil_init (void_t);
CR_API void_t       devil_kill (void_t);
CR_API sENGINE*     engine_devil (void_t);

/*****************************************************************************/
/*                               图片文件格式                                */
/*****************************************************************************/

CR_API sFMTZ*   load_il_bmp   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_cut   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_doomw (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_doomf (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_ico   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_jpg   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_ilbm  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_pcd   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_pcx   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_pic   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_png   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_pnm   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_sgi   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_tga   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_tif   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_chead (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_raw   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_mdl   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_wal   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_lif   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_mng   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_gif   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_dds   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_dcx   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_psd   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_psp   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_pix   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_pxr   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_xpm   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_hdr   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_icns  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_jp2   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_exr   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_wdp   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_vtf   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_wbmp  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_sun   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_iff   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_tpl   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_fits  (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_dicom (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_iwi   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_blp   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_ftx   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_rot   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_tex   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_dpx   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_utx   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_mp3   (iDATIN *datin, const sLOADER *param);
CR_API sFMTZ*   load_il_auto  (iDATIN *datin, const sLOADER *param);

#endif  /* !__CR_DEVIL_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
