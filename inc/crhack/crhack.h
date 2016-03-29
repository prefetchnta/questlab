/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-17  */
/*     #######          ###    ###      [SOFT]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>>>> CrHack 总头文件 <<<<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_HACK_H__
#define __CR_HACK_H__

#include "defs.h"
/*-------------*/
#include "blit.h"
#include "egui.h"
#include "extz.h"
#include "fmtz.h"
#include "gfx2.h"
#include "gfx3.h"
#include "hash.h"
#include "safe.h"
#include "applib.h"
#include "crypto.h"
#include "datlib.h"
#include "devlib.h"
#include "enclib.h"
#include "fileio.h"
#include "memlib.h"
#include "morder.h"
#include "msclib.h"
#include "mtplib.h"
#include "parser.h"
#include "phylib.h"
#include "pixels.h"
#include "plugin.h"
#include "rtclib.h"
#include "strlib.h"
/*---------------*/

/* 平台相关头文件包含 */
#if     defined(_CR_OS_MSWIN_)
    #include <windows.h>
#endif

/* CRHACK/CORE 公用 API */
CR_API bool_t           crhack_core_init (void_t);
CR_API void_t           crhack_core_free (void_t);
CR_API uint_t           crhack_comp_vers (void_t);
CR_API const ansi_t*    crhack_comp_name (void_t);
CR_API const ansi_t*    crhack_date_time (void_t);

#endif  /* !__CR_HACK_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
