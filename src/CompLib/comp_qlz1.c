/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2020-01-29  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack QuickLZ 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "Complib.h"

#define qlz_compress            qlz_compress_1
#define qlz_decompress          qlz_decompress_1
#define qlz_get_setting         qlz_get_setting_1
#define qlz_size_header         qlz_size_header_1
#define qlz_size_compressed     qlz_size_compressed_1
#define qlz_size_decompressed   qlz_size_decompressed_1
#define QLZ_COMPRESSION_LEVEL   1
#define QLZ_STREAMING_BUFFER    0
#include "quicklz/quicklz.c"

/*
=======================================
    QuickLZ 压缩 (Level1)
=======================================
*/
CR_API leng_t
compr_qlz1 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    qlz_state_compress* state;

    if (dst == NULL)
        return (srclen + (srclen >> 3) + 400);

    state = struct_new(qlz_state_compress);
    if (state == NULL)
        return (0);

    dstlen = qlz_compress_1(src, (char*)dst, srclen, state);
    mem_free(state);
    return (dstlen);
}

/*
=======================================
    QuickLZ 解压 (Level1)
=======================================
*/
CR_API leng_t
uncompr_qlz1 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    size_t                  ssize;
    size_t                  dsize;
    qlz_state_decompress*   state;

    ssize = qlz_size_compressed_1((const char*)src);
    dsize = qlz_size_decompressed_1((const char*)src);
    if (srclen < ssize || dstlen < dsize)
        return (0);

    state = struct_new(qlz_state_decompress);
    if (state == NULL)
        return (0);

    dstlen = qlz_decompress_1((const char*)src, dst, state);
    mem_free(state);
    return (dstlen);
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
