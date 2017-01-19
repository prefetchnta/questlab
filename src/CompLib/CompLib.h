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

#endif  /* !__QL_COMPLIB_H__ */
