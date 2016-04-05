/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-09-09  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack KiriKiri FMTZ 插件头文件 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_KIRIKIRI_H__
#define __CR_KIRIKIRI_H__

#include "../fmtz.h"

/* XP3 文件分段结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
        int32u  zlib;       /* ZLib 压缩 */
        int64u  offset;     /* 数据的偏移 */
        int64u  unsize;     /* 解压缩大小 */
        int64u  pksize;     /* 存放的大小 */

} CR_PACKED sPAK_XP3_SEGM;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* XP3 文件信息结构 */
typedef struct
{
        /* 公用部分 */
        sPAK_FILE   base;

        /* 个性部分 */
        int32u          protect;    /* 保护字段？ */
        int32u          adlr_key;   /* 附加的密钥 */
        leng_t          segm_cnt;   /* 文件分段数量 */
        sPAK_XP3_SEGM*  segm_lst;   /* 文件分段列表 */

} sPAK_XP3_FILE;

/* 文件加密标识 (用附加串的 CRC32 来确定) */
#define KRKR_AUTO_XOR8  0xCA6CA894UL    /* "auto_xor8" 自识别8位异或 */

/*****************************************************************************/
/*                                 引擎接口                                  */
/*****************************************************************************/

CR_API sENGINE*     engine_krkr (void_t);

/*****************************************************************************/
/*                               封包文件格式                                */
/*****************************************************************************/

CR_API sFMT_PRT*    load_krkr_xp3 (iDATIN *datin, const sLOADER *param);

#endif  /* !__CR_KIRIKIRI_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
