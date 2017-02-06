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
        void_t*         rplc;
        const ansi_t*   name;

} sBP_FUNC;

void_t* func_load (const ansi_t *name);
void_t  func_setup (void_t *handle, sBP_FUNC *func);
void_t  func_free (void_t *handle);

/* 简化函数定义 */
#define BACKPORT_FUNC(__remap, __index, __name) \
__declspec(dllexport) \
__declspec(naked) void __name (void) \
{ \
    __asm mov eax, __index \
    __asm mov ebx, __remap \
    __asm lea ecx, [ebx + eax * 8] \
    __asm lea edx, [ecx + eax * 4] \
    __asm mov eax, dword ptr [edx] \
    __asm test eax, eax \
    __asm jz $+8 \
    __asm jmp dword ptr [eax] \
    __asm jmp dword ptr [edx + 4] \
}

/* 简化定义的宏 */
#define BACKPORT_FINISH         { NULL, NULL, NULL }
#define BACKPORT_JUMP(__name)   { NULL, NULL, #__name }
#define BACKPORT_RPLC(__name)   { NULL, (void_t*)(my##__name), #__name }

#endif  /* !__QL_BACKPORT_H__ */
