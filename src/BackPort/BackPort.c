
#include "BackPort.h"

#include <windows.h>

/*
=======================================
    加载映射表里的函数
=======================================
*/
extern void_t*
func_load (
  __CR_IO__ sBP_FUNC*       func,
  __CR_IN__ const ansi_t*   name
    )
{
    uint_t  idx;
    HANDLE  hdl;

    hdl = LoadLibraryA(name);
    if (hdl == NULL)
        return (NULL);
    for (idx = 0; func[idx].name != NULL; idx++)
        func[idx].addr = (void_t*)GetProcAddress(hdl, func[idx].name);
    return ((void_t*)hdl);
}

/*
=======================================
    释放 DLL 句柄
=======================================
*/
extern void_t
func_free (
  __CR_IN__ void_t* handle
    )
{
    FreeLibrary((HMODULE)handle);
}
