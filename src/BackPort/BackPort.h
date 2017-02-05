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
 *             ##       CREATE: 2017-02-05
 *              #
 ================================================
        BackPort 公用函数库
 ================================================
 */

#ifndef __QL_BACKPORT_H__
#define __QL_BACKPORT_H__

#include "defs.h"

#define DLL_PROCESS_ATTACH  1
#define DLL_THREAD_ATTACH   2
#define DLL_THREAD_DETACH   3
#define DLL_PROCESS_DETACH  0

/* 函数列表单元 */
typedef struct
{
        void_t*         addr;
        const ansi_t*   name;

} sBP_FUNC;

void_t* func_load (sBP_FUNC *func, const ansi_t *dll);
void_t  func_free (void_t *handle);

/* 简化函数定义 */

#endif  /* !__QL_BACKPORT_H__ */
