/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2020-09-15  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack DENSITY 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "Complib.h"
#include "density/density_api.h"

/*
=======================================
    DENSITY 压缩
=======================================
*/
CR_API leng_t
compr_density (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          type
    )
{
    density_processing_result   rett;
    /* --------------------------- */
    leng_t  request = (leng_t)density_compress_safe_size(srclen);

    /* 目前狮子有问题 */
    if (type == 3)
        return (0);

    if (dst == NULL)
        return (request);

    /* 比较特殊, 需要安全检查 */
    if (dstlen < request)
        return (0);
    rett = density_compress((uint8_t*)src, srclen,
                            (uint8_t*)dst, dstlen, (DENSITY_ALGORITHM)type);
    return (rett.state ? 0 : (leng_t)rett.bytesWritten);
}

/*
=======================================
    DENSITY 解压
=======================================
*/
CR_API leng_t
uncompr_density (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    void_t*                     buff;
    density_processing_result   rett;
    /* --------------------------- */
    leng_t  request = (leng_t)density_decompress_safe_size(dstlen);

    /* 需要更大的解压缓冲区 */
    buff = mem_malloc(request);
    if (buff == NULL)
        return (0);
    rett = density_decompress((uint8_t*)src, srclen, (uint8_t*)buff, request);
    if (!rett.state) {
        if (rett.bytesWritten > dstlen) {
            mem_free(buff);
            return (0);
        }
        request = (leng_t)rett.bytesWritten;
        mem_cpy(dst, buff, request);
        mem_free(buff);
        return (request);
    }
    mem_free(buff);
    return (0);
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
