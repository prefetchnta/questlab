/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-13  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack JCALG1 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "CompLib.h"
#include "jcalg1/jcalg1.h"

/*
---------------------------------------
    JCALG1 分配内存
---------------------------------------
*/
static void* __stdcall
jcalg1_alloc (
  __CR_IN__ DWORD   size
    )
{
    return (mem_malloc(size));
}

/*
---------------------------------------
    JCALG1 释放内存
---------------------------------------
*/
static BOOL __stdcall
jcalg1_free (
  __CR_IN__ void*   pointer
    )
{
    mem_free(pointer);
    return (TRUE);
}

/*
---------------------------------------
    JCALG1 进度回调
---------------------------------------
*/
static BOOL __stdcall
jcalg1_callback (
  __CR_IN__ DWORD   src,
  __CR_IN__ DWORD   dst
    )
{
    CR_NOUSE(src);
    CR_NOUSE(dst);
    return (TRUE);
}

/*
=======================================
    JCALG1 压缩
=======================================
*/
CR_API leng_t
compr_jcalg1 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          winlen
    )
{
    if (dst == NULL)
        return (JCALG1_GetNeededBufferSize(srclen));
    CR_NOUSE(dstlen);
    return (JCALG1_Compress(src, srclen, dst, winlen, jcalg1_alloc,
                            jcalg1_free, jcalg1_callback, FALSE));
}

/*
=======================================
    JCALG1 解压
=======================================
*/
CR_API leng_t
uncompr_jcalg1 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    CR_NOUSE(dstlen); CR_NOUSE(srclen);
    return (JCALG1_Decompress_Fast(src, dst));
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
