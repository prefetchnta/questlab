/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-06-07  */
/*     #######          ###    ###      [KRNL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>> CrHack SAMSUNG K9F1208X0C 器件定义头文件 <<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_K9F1208C_H__
#define __CR_K9F1208C_H__

#include "../defs.h"

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* K9F1208X0C 器件 ID */
CR_TYPEDEF struct
{
        byte_t  mak_id;     /* 厂家代码 0xEC */
        byte_t  dev_id;     /* 设备代码 0x76 */
        byte_t  thd_id;     /* 不支持回拷贝操作 0x5A */
        byte_t  fth_id;     /* 不支持多平面操作 0x3F */

} CR_PACKED sK9F1208C;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* K9F1208X0C 参数值 */
#define K9F1208C_CSEC_SIZE    16UL  /* C 区空间大小 */
#define K9F1208C_PAGE_PBLK    32UL  /* 一块空间页数 */
#define K9F1208C_PAGE_SIZE   512UL  /* 一页空间大小 */
#define K9F1208C_ABCA_SIZE   528UL  /* ABC 区总大小 */
#define K9F1208C_BLOCK_NUM  4096UL  /* 器件总的块数 */

#endif  /* !__CR_K9F1208C_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
