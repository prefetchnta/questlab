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
 *             ##       CREATE: 2013-03-22
 *              #
 ================================================
        基于 CrHack 的 QV2D 命令滤镜扩展
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 外部库引用 */
#ifndef _CR_NO_PRAGMA_LIB_
    #if defined(_CR_CC_BCC_)
        #pragma comment (lib, "CrH_BLIT_bcc.lib")
    #else
        #pragma comment (lib, "CrH_BLIT_msc.lib")
    #endif
#endif

/*
=======================================
    插件 DLL 入口点
=======================================
*/
#if defined(_CR_CC_BCC_)
int  WINAPI
DllEntryPoint (
  __CR_IN__ HINSTANCE       hinst,
  __CR_IN__ unsigned long   reason,
  __CR_UU__ void*           reserved
    )
#else
BOOL WINAPI
DllMain (
  __CR_IN__ HANDLE  hinst,
  __CR_IN__ DWORD   reason,
  __CR_UU__ LPVOID  reserved
    )
#endif
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}
