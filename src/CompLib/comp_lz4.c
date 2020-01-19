/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2020-01-19  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack LZ4 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "Complib.h"
#include "lz4/lz4hc.h"

/*
=======================================
    LZ4 压缩
=======================================
*/
CR_API leng_t
compr_lz4 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          accel
    )
{
    void *state;
    int ssize, dsize;

    if (srclen <= 0 || srclen > LZ4_MAX_INPUT_SIZE)
        return (0);
    ssize = (int)srclen;

    if (dstlen <= 0 || dstlen > LZ4_MAX_INPUT_SIZE)
        return (0);
    dsize = (int)dstlen;

    if (dst == NULL)
        return (LZ4_compressBound(ssize));

    state = mem_malloc(LZ4_sizeofState());
    if (state == NULL)
        return (0);
    dstlen = LZ4_compress_fast_extState(state, (char*)src, (char*)dst,
                                        ssize, dsize, accel);
    mem_free(state);
    return (dstlen);
}

/*
=======================================
    LZ4HC 压缩
=======================================
*/
CR_API leng_t
compr_lz4hc (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          level
    )
{
    void *state;
    int ssize, dsize;

    if (srclen <= 0 || srclen > LZ4_MAX_INPUT_SIZE)
        return (0);
    ssize = (int)srclen;

    if (dstlen <= 0 || dstlen > LZ4_MAX_INPUT_SIZE)
        return (0);
    dsize = (int)dstlen;

    if (dst == NULL)
        return (LZ4_compressBound(ssize));

    state = mem_malloc(LZ4_sizeofStateHC());
    if (state == NULL)
        return (0);
    dstlen = LZ4_compress_HC_extStateHC(state, (char*)src, (char*)dst,
                                        ssize, dsize, level);
    mem_free(state);
    return (dstlen);
}

/*
=======================================
    LZ4 解压
=======================================
*/
CR_API leng_t
uncompr_lz4 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    int ssize, dsize;

    if (srclen <= 0 || srclen > LZ4_MAX_INPUT_SIZE)
        return (0);
    ssize = (int)srclen;

    if (dstlen <= 0 || dstlen > LZ4_MAX_INPUT_SIZE)
        return (0);
    dsize = (int)dstlen;

    dsize = LZ4_decompress_safe((char*)src, (char*)dst, ssize, dsize);
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
