/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2020-09-22  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack LZ5 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "Complib.h"
#include "lz5/lz5_compress.h"
#include "lz5/lz5_decompress.h"

/*
=======================================
    LZ5 压缩
=======================================
*/
CR_API leng_t
compr_lz5 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          level
    )
{
    void *state;
    int ssize, dsize;

    if (srclen <= 0 || srclen > LZ5_MAX_INPUT_SIZE)
        return (0);
    ssize = (int)srclen;

    if (dstlen <= 0 || dstlen > LZ5_MAX_INPUT_SIZE)
        return (0);
    dsize = (int)dstlen;

    if (dst == NULL)
        return (LZ5_compressBound(ssize));

    state = mem_malloc(LZ5_sizeofState(level));
    if (state == NULL)
        return (0);
    dstlen = LZ5_compress_extState(state, (char*)src, (char*)dst,
                                   ssize, dsize, level);
    mem_free(state);
    return (dstlen);
}

/*
=======================================
    LZ5 解压
=======================================
*/
CR_API leng_t
uncompr_lz5 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    int ssize, dsize;

    if (srclen <= 0 || srclen > LZ5_MAX_INPUT_SIZE)
        return (0);
    ssize = (int)srclen;

    if (dstlen <= 0 || dstlen > LZ5_MAX_INPUT_SIZE)
        return (0);
    dsize = (int)dstlen;

    dsize = LZ5_decompress_safe((char*)src, (char*)dst, ssize, dsize);
    return ((dsize <= 0) ? 0 : dsize);
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
