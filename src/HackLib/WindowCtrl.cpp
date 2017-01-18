
#include "HackLib.h"

#include <vector>
#include <stdlib.h>

/*
---------------------------------------
    枚举回调
---------------------------------------
*/
static BOOL CALLBACK
window_enum (
  __CR_IN__ HWND    hwnd,
  __CR_IN__ LPARAM  param
    )
{
    std::vector<HWND>*  vlist;

    vlist = (std::vector<HWND>*)param;
    vlist->push_back(hwnd);
    return (TRUE);
}

/*
=======================================
    窗口枚举
=======================================
*/
CR_API UINT WINAPI
HWndList (
  __CR_IN__ HWND    parent,
  __CR_OT__ HWND**  list
    )
{
    std::vector<HWND>   vlist;

    /* 枚举所有子窗口 */
    if (!EnumChildWindows(parent, window_enum, (LPARAM)(&vlist)))
        return (0);

    size_t  size = vlist.size();

    /* 返回窗口列表 */
    if (size == 0)
        return (0);
    size *= sizeof(HWND);
    *list = (HWND*)mem_malloc(size);
    if (*list == NULL)
        return (0);
    memcpy(*list, &vlist[0], size);
    return (vlist.size());
}

/*
=======================================
    窗口查找
=======================================
*/
CR_API HWND WINAPI
HWndFind (
  __CR_IN__ HWND    parent,
  __CR_IN__ LPCWSTR wclass,
  __CR_IN__ LPCWSTR wtitle
    )
{
    return (FindWindowExW(parent, NULL, wclass, wtitle));
}
