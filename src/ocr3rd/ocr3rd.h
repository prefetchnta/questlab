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
 *             ##       CREATE: 2013-06-08
 *              #
 ================================================
        开源 OCR 引擎封装库
 ================================================
 */

#ifndef __QL_OCR3RD_H__
#define __QL_OCR3RD_H__

#include "../QstLibs/QstLibs.h"

/*****************************************************************************/
/*                              TESSERACT-OCR                                */
/*****************************************************************************/

/* TESS-OCR 对象类型 */
typedef void_t*     tess_ocr_t;

CR_API tess_ocr_t   tess_ocr_init (const ansi_t *path,
                                   const ansi_t *lang);
CR_API void_t       tess_ocr_kill (tess_ocr_t tess_ocr);
CR_API bool_t       tess_ocr_set_param (tess_ocr_t tess_ocr,
                                        const ansi_t *name,
                                        const ansi_t *value);
CR_API bool_t       tess_ocr_set_image (tess_ocr_t tess_ocr,
                                        const sIMAGE *image);
CR_API void_t       tess_ocr_set_ppi (tess_ocr_t tess_ocr,
                                      uint_t ppi);
CR_API void_t       tess_ocr_set_rect (tess_ocr_t tess_ocr,
                                       const sRECT *rect);
CR_API ansi_t*      tess_ocr_get_utf8 (tess_ocr_t tess_ocr);
CR_API void_t       tess_ocr_str_free (ansi_t *string);

#endif  /* !__QL_OCR3RD_H__ */
