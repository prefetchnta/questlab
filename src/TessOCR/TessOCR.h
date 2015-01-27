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
        TESSERACT-OCR 引擎封装库
 ================================================
 */

#ifndef __QL_TESSOCR_H__
#define __QL_TESSOCR_H__

#include "crhack.h"

/*****************************************************************************/
/*                              TESSERACT-OCR                                */
/*****************************************************************************/

/* TESS-OCR 对象类型 */
typedef void_t*     tessocr_t;

/* TESS-OCR API 封装 */
CR_API tessocr_t    tessocr_init (const ansi_t *path, const ansi_t *lang);
CR_API void_t       tessocr_kill (tessocr_t tessocr);
CR_API bool_t       tessocr_set_param (tessocr_t tessocr, const ansi_t *name,
                                       const ansi_t *value);
CR_API bool_t       tessocr_set_image (tessocr_t tessocr, const sIMAGE *image);
CR_API void_t       tessocr_set_ppi (tessocr_t tessocr, uint_t ppi);
CR_API void_t       tessocr_set_rect (tessocr_t tessocr, const sRECT *rect);
CR_API ansi_t*      tessocr_get_utf8 (tessocr_t tessocr);
CR_API void_t       tessocr_str_free (ansi_t *string);

#endif  /* !__QL_TESSOCR_H__ */
