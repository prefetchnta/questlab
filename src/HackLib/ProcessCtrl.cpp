
#include "HackLib.h"

#include <vector>
#include <stdlib.h>

/*
=======================================
    进程枚举
=======================================
*/
CR_API UINT WINAPI
ProcessList (
  __CR_OT__ PROCESSENTRY32W**   list
    )
{
    BOOL            goon;
    HANDLE          snap;
    PROCESSENTRY32W temp;

    /* 取得所有进程信息 */
    snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE)
        return (0);

    std::vector<PROCESSENTRY32W>    vlist;

    /* 枚举进程 */
    temp.dwSize = sizeof(PROCESSENTRY32W);
    goon = Process32FirstW(snap, &temp);
    while (goon) {
        vlist.push_back(temp);
        temp.dwSize = sizeof(PROCESSENTRY32W);
        goon = Process32NextW(snap, &temp);
    }
    CloseHandle(snap);

    size_t  size = vlist.size();

    /* 返回进程列表 */
    if (size == 0)
        return (0);
    size *= sizeof(PROCESSENTRY32W);
    *list = (PROCESSENTRY32W*)malloc(size);
    if (*list == NULL)
        return (0);
    memcpy(*list, &vlist[0], size);
    return (vlist.size());
}

/*
=======================================
    线程枚举
=======================================
*/
CR_API UINT WINAPI
ThreadList (
  __CR_IN__ DWORD           prcss_id,
  __CR_OT__ THREADENTRY32** list
    )
{
    BOOL            goon;
    HANDLE          snap;
    THREADENTRY32   temp;

    /* 取得指定进程的所有线程信息 */
    snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, prcss_id);
    if (snap == INVALID_HANDLE_VALUE)
        return (0);

    std::vector<THREADENTRY32>  vlist;

    /* 枚举线程 */
    temp.dwSize = sizeof(THREADENTRY32);
    goon = Thread32First(snap, &temp);
    while (goon) {
        if (temp.th32OwnerProcessID == prcss_id)
            vlist.push_back(temp);
        temp.dwSize = sizeof(THREADENTRY32);
        goon = Thread32Next(snap, &temp);
    }
    CloseHandle(snap);

    size_t  size = vlist.size();

    /* 返回线程列表 */
    if (size == 0)
        return (0);
    size *= sizeof(THREADENTRY32);
    *list = (THREADENTRY32*)malloc(size);
    if (*list == NULL)
        return (0);
    memcpy(*list, &vlist[0], size);
    return (vlist.size());
}

/*
=======================================
    模块枚举
=======================================
*/
CR_API UINT WINAPI
ModuleList (
  __CR_IN__ DWORD               prcss_id,
  __CR_OT__ MODULEENTRY32W**    list
    )
{
    BOOL            goon;
    HANDLE          snap;
    MODULEENTRY32W  temp;

    /* 取得指定进程的所有模块信息 */
    snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, prcss_id);
    if (snap == INVALID_HANDLE_VALUE)
        return (0);

    std::vector<MODULEENTRY32W> vlist;

    /* 枚举进程 */
    temp.dwSize = sizeof(MODULEENTRY32W);
    goon = Module32FirstW(snap, &temp);
    while (goon) {
        vlist.push_back(temp);
        temp.dwSize = sizeof(MODULEENTRY32W);
        goon = Module32NextW(snap, &temp);
    }
    CloseHandle(snap);

    size_t  size = vlist.size();

    /* 返回进程列表 */
    if (size == 0)
        return (0);
    size *= sizeof(MODULEENTRY32W);
    *list = (MODULEENTRY32W*)malloc(size);
    if (*list == NULL)
        return (0);
    memcpy(*list, &vlist[0], size);
    return (vlist.size());
}

/*
=======================================
    进程启停
=======================================
*/
CR_API BOOL WINAPI
ProcessCtrl (
  __CR_IN__ DWORD   prcss_id,
  __CR_IN__ BOOL    suspend
    )
{
    HANDLE          snap;
    THREADENTRY32   te32;

    /* 取得指定进程的所有线程信息 */
    snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, prcss_id);
    if (snap == INVALID_HANDLE_VALUE)
        return (HLB_ERROR);

    /* 枚举线程 */
    te32.dwSize = sizeof(THREADENTRY32);
    if(!Thread32First(snap, &te32)) {
        CloseHandle(snap);
        return (HLB_ERROR);
    }

    do {
        if (te32.th32OwnerProcessID == prcss_id)
        {
            HANDLE  handle;

            handle = OpenThread(THREAD_SUSPEND_RESUME, FALSE,
                                te32.th32ThreadID);
            if (suspend)
                SuspendThread(handle);
            else
                ResumeThread(handle);
            CloseHandle(handle);
        }
    } while (Thread32Next(snap, &te32));

    CloseHandle(snap);
    return (HLB_OKAY);
}
