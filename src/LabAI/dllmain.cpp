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
 *             ##       CREATE: 2018-03-05
 *              #
 ================================================
        人工智能 LAB DLL 入口点
 ================================================
 */

#include "crhack.h"

/* 外部库引用 */
#pragma comment (lib, "CrH_CORE.lib")
#pragma comment (lib, "CrH_GFX2.lib")
#pragma comment (lib, "CrH_MATH.lib")
#pragma comment (lib, "opencv_world4120.lib")
#pragma comment (lib, "MNN.lib")
#pragma comment (lib, "ncnn.lib")

/*
=======================================
    DLL 入口点
=======================================
*/
BOOL WINAPI
DllMain (
  __CR_IN__ HANDLE  hinst,
  __CR_IN__ DWORD   reason,
  __CR_UU__ LPVOID  reserved
    )
{
    static bool_t xcom = FALSE;

    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            xcom = com_init();
            break;

        case DLL_PROCESS_DETACH:
            if (xcom)
                com_kill();
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}
