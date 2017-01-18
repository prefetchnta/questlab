
#include "HackLib.h"
#pragma comment (lib, "CrH_CORE.lib")

#if defined(_CR_BUILD_DLL_)
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

/*
=======================================
    释放内存
=======================================
*/
CR_API VOID WINAPI
HLMemPtrFree (
  __CR_IN__ LPVOID  ptr
    )
{
    mem_free(ptr);
}
