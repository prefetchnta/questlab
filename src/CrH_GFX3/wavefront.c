/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-12-04  */
/*     #######          ###    ###      [GFX3]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack WAVEFRONT 模型解析函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_WAVEFRONT_C__
#define __CR_WAVEFRONT_C__ 0x390259D6UL

#include "gfx3.h"
#include "datlib.h"
#include "strlib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
=======================================
    解析 OBJ 字符串
=======================================
*/
CR_API bool_t
wfront_obj_load (
  __CR_OT__ sWAVEFRONT*     obj,
  __CR_IN__ const ansi_t*   str
    )
{

}

/*
=======================================
    释放 OBJ 对象
=======================================
*/
CR_API void_t
wfront_obj_free (
  __CR_IN__ const sWAVEFRONT*   obj
    )
{
    leng_t  idx;

    TRY_FREE(obj->mtl);
    mem_free(obj->p_v);
    TRY_FREE(obj->p_vt);
    TRY_FREE(obj->p_vn);
    mem_free(obj->p_f);
    for (idx = 0; idx < obj->n_g; idx++) {
        mem_free(obj->p_g[idx].name);
        TRY_FREE(obj->p_g[idx].mtl);
    }
    mem_free(obj->p_g);
}

#endif  /* !__CR_WAVEFRONT_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
