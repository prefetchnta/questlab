
#include "BackPort.h"

#include <windows.h>

/*
=======================================
    加载 DLL 库文件
=======================================
*/
extern void_t*
func_load (
  __CR_IN__ const ansi_t*   name
    )
{
    return ((void_t*)LoadLibraryA(name));
}

/*
=======================================
    加载映射表里的函数
=======================================
*/
extern void_t
func_setup (
  __CR_IN__ void_t*     handle,
  __CR_IO__ sBP_FUNC*   func
    )
{
    uint_t  idx;

    for (idx = 0; func[idx].name != NULL; idx++)
        func[idx].addr = (void_t*)GetProcAddress(handle, func[idx].name);
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
