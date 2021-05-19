
#include "xCrHackSet.h"

/* 外部库引用 */
#pragma comment (lib, "QstLibs.lib")
#pragma comment (lib, "CrH_GFX2.lib")
#pragma comment (lib, "CrH_MATH.lib")
#pragma comment (lib, "GFX2_GDI.lib")
#pragma comment (lib, "libzbar-0.lib")
#pragma comment (lib, "libzxing.lib")
#pragma comment (lib, "libfacedetect.lib")

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    插件 DLL 入口点
=======================================
*/
BOOL WINAPI
DllMain (
  __CR_IN__ HANDLE  hinst,
  __CR_IN__ DWORD   reason,
  __CR_UU__ LPVOID  reserved
    )
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

#endif  /* _CR_BUILD_DLL_ */
