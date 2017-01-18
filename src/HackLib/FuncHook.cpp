
#include "HackLib.h"
#include "../Asylum/asylum.hpp"
using namespace asy;

/*
=======================================
    创建函数哈希表
=======================================
*/
CR_API hookfunc_t WINAPI
HookFuncNew (
  __CR_IN__ const sHookFunc*    func,
  __CR_IN__ UINT                count
    )
{
    map_acs<sHookFunc>* tbl;

    tbl = new map_acs<sHookFunc> ();
    if (tbl == NULL)
        return (NULL);
    if (!tbl->init())
        goto _failure1;
    for (UINT idx = 0; idx < count; idx++) {
        if (tbl->insert(func[idx].name, (sHookFunc*)(&func[idx])) == NULL)
            goto _failure2;
    }
    return ((hookfunc_t)tbl);

_failure2:
    tbl->free();
_failure1:
    delete tbl;
    return (NULL);
}

/*
=======================================
    释放函数哈希表
=======================================
*/
CR_API VOID WINAPI
HookFuncDel (
  __CR_IN__ hookfunc_t  table
    )
{
    map_acs<sHookFunc>* tbl;

    tbl = (map_acs<sHookFunc>*)table;
    tbl->free();
    delete tbl;
}

/*
=======================================
    获取函数地址 (通过名字)
=======================================
*/
CR_API LPVOID WINAPI
HookFuncGet (
  __CR_IN__ LPCSTR      name,
  __CR_IN__ hookfunc_t  table
    )
{
    map_acs<sHookFunc>* tbl;

    tbl = (map_acs<sHookFunc>*)table;
    return (tbl->get(name)->func.addr);
}

/*
=======================================
    COM 方法钩子设置
=======================================
*/
CR_API UINT WINAPI
HookVTable (
  __CR_IO__ LPVOID              object,
  __CR_IN__ const sHookFunc*    hooks,
  __CR_IN__ UINT                count,
  __CR_IN__ hookfunc_t          table
    )
{
    DWORD       old1;
    DWORD       old2;
    SIZE_T*     vptr;
    sHookFunc*  func;

    vptr = (SIZE_T*)(*(SIZE_T*)object);

    /* 解除页面保护 (假设虚函数表不大于1页) */
    if (!VirtualProtect(vptr, 4096, PAGE_READWRITE, &old1))
        return (HLB_ERROR);

    map_acs<sHookFunc>* tbl;

    /* 根据函数名修改函数指针 */
    tbl = (map_acs<sHookFunc>*)table;
    for (UINT idx = 0; idx < count; idx++) {
        func = tbl->get(hooks[idx].name);
        if (func != NULL)
            vptr[func->func.index] = hooks[idx].func.index;
    }
    VirtualProtect(vptr, 4096, old1, &old2);
    return (HLB_OKAY);
}
