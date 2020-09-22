/*
 ================================================
 *      ######
 *    #########                            ##
 *   ####    ###                           ##
 *   ##       ##                           ##
 *  ##         ## ##   ##   ####     ### ######
 *  ##         ## ##   ##  ######  ##### ######
 *  ##         ## ##   ## ###  ## ###      ##
 *  ##    #    ## ##   ## ##  ##  #####    ##
 *   ##   ##   ## ##   ## ## ##    #####   ##
 *   ###   ## ##  ##   ## ###   #     ##   ##
 *    ##########  ####### ####### ######   ##
 *      #### ##    ######   ####   ####    ##
 *            ##
 *             ##       CREATE: 2017-01-19
 *              #
 ================================================
        压缩解压函数库
 ================================================
 */

#ifndef __QL_COMPLIB_H__
#define __QL_COMPLIB_H__

#include "crhack.h"

/* aPLib */
CR_API leng_t   compr_aplib (void_t *dst, leng_t dstlen,
                             const void_t *src, leng_t srclen);

CR_API leng_t   uncompr_aplib (void_t *dst, leng_t dstlen,
                               const void_t *src, leng_t srclen);
/* BriefLZ */
CR_API leng_t   compr_blz (void_t *dst, leng_t dstlen,
                           const void_t *src, leng_t srclen,
                           uint_t level CR_DEFAULT(1));

CR_API leng_t   uncompr_blz (void_t *dst, leng_t dstlen,
                             const void_t *src, leng_t srclen);
/* BROTLI */
typedef struct
{
        int32s  mode;   /*  0 - GENERIC, 1 - UTF8 TEXT, 2 - FONT WOFF 2.0 */
        int32s  lgwin;          /* [10 - 30], default = 22 */
        int32s  quality;        /* [ 0 - 11], default = 11 */

} sBROTLI;

CR_API leng_t   compr_brotli (void_t *dst, leng_t dstlen,
                              const void_t *src, leng_t srclen,
                              const sBROTLI *param);

CR_API leng_t   uncompr_brotli (void_t *dst, leng_t dstlen,
                                const void_t *src, leng_t srclen);
/* DENSITY */
/* CHAMELEON = 1, CHEETAH = 2, LION = 3 */
CR_API leng_t   compr_density (void_t *dst, leng_t dstlen,
                               const void_t *src, leng_t srclen,
                               uint_t type CR_DEFAULT(1));

CR_API leng_t   uncompr_density (void_t *dst, leng_t dstlen,
                                 const void_t *src, leng_t srclen);
/* JCALG1 */
CR_API leng_t   compr_jcalg1 (void_t *dst, leng_t dstlen,
                              const void_t *src, leng_t srclen,
                              uint_t winlen CR_DEFAULT(8192));

CR_API leng_t   uncompr_jcalg1 (void_t *dst, leng_t dstlen,
                                const void_t *src, leng_t srclen);
/* LZ4 */
CR_API leng_t   compr_lz4 (void_t *dst, leng_t dstlen,
                           const void_t *src, leng_t srclen,
                           uint_t accel CR_DEFAULT(1));

CR_API leng_t   compr_lz4hc (void_t *dst, leng_t dstlen,
                             const void_t *src, leng_t srclen,
                             uint_t level CR_DEFAULT(12));

CR_API leng_t   uncompr_lz4 (void_t *dst, leng_t dstlen,
                             const void_t *src, leng_t srclen);
/* LZMA */
typedef struct
{
        byte_t  props[8];   /*         压缩输出的额外参数        */
        int32u  level;      /* [ 0, 1   -   9  ], default =  5   */
        int32u  dictlen;    /* [ 0, 4KB - 128MB], default = 16MB */
        int32s  lc;         /* [-1, 0   -   8  ], default =  3   */
        int32s  lp;         /* [-1, 0   -   4  ], default =  0   */
        int32s  pb;         /* [-1, 0   -   4  ], default =  2   */
        int32s  fb;         /* [-1, 5   - 273  ], default = 32   */
        int32s  threads;    /* [-1, 1   -   2  ], default =  2   */
} sLZMA;

CR_API leng_t   compr_lzma (void_t *dst, leng_t dstlen,
                            const void_t *src, leng_t srclen,
                            sLZMA *param);

CR_API leng_t   uncompr_lzma (void_t *dst, leng_t dstlen,
                              const void_t *src, leng_t srclen,
                              const byte_t props[8]);
/* QuickLZ */
CR_API leng_t   compr_qlz (void_t *dst, leng_t dstlen,
                           const void_t *src, leng_t srclen,
                           uint_t level CR_DEFAULT(1));

CR_API leng_t   uncompr_qlz (void_t *dst, leng_t dstlen,
                             const void_t *src, leng_t srclen,
                             uint_t level CR_DEFAULT(1));
/* QuickLZ (Level1) */
CR_API leng_t   compr_qlz1 (void_t *dst, leng_t dstlen,
                            const void_t *src, leng_t srclen);

CR_API leng_t   uncompr_qlz1 (void_t *dst, leng_t dstlen,
                              const void_t *src, leng_t srclen);
/* QuickLZ (Level2) */
CR_API leng_t   compr_qlz2 (void_t *dst, leng_t dstlen,
                            const void_t *src, leng_t srclen);

CR_API leng_t   uncompr_qlz2 (void_t *dst, leng_t dstlen,
                              const void_t *src, leng_t srclen);
/* QuickLZ (Level3) */
CR_API leng_t   compr_qlz3 (void_t *dst, leng_t dstlen,
                            const void_t *src, leng_t srclen);

CR_API leng_t   uncompr_qlz3 (void_t *dst, leng_t dstlen,
                              const void_t *src, leng_t srclen);
/* SNAPPY */
CR_API leng_t   compr_snappy (void_t *dst, leng_t dstlen,
                              const void_t *src, leng_t srclen);

CR_API leng_t   uncompr_snappy (void_t *dst, leng_t dstlen,
                                const void_t *src, leng_t srclen);
/* ZSTD */
CR_API leng_t   compr_zstd (void_t *dst, leng_t dstlen,
                            const void_t *src, leng_t srclen,
                            uint_t level CR_DEFAULT(3));

CR_API leng_t   uncompr_zstd (void_t *dst, leng_t dstlen,
                              const void_t *src, leng_t srclen);

#endif  /* !__QL_COMPLIB_H__ */
