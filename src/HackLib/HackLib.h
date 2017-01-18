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
 *             ##       CREATE: 2017-01-17
 *              #
 ================================================
        HACK 相关的东西都放在这里面
 ================================================
 */

#ifndef __QL_HACKLIB_H__
#define __QL_HACKLIB_H__

#include "crhack.h"

#include <tlhelp32.h>

/* 库函数返回值 */
#define HLB_OKAY       (0L)
#define HLB_ERROR   (__LINE__)

/* 释放返回的内存指针 */
CR_API VOID WINAPI HLMemFree (LPVOID ptr);

/*****************************************************************************/
/*                                 代码注入                                  */
/*****************************************************************************/

/* 进程加载结构体 */
struct  sLoaderParam
{
    /* 注入代码块里用的到的几个 API 函数 */
    BOOL    (WINAPI *CreateProcessA) (LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES,
                                      LPSECURITY_ATTRIBUTES, BOOL,
                                      DWORD, LPVOID, LPCSTR, LPSTARTUPINFO,
                                      LPPROCESS_INFORMATION);
    BOOL    (WINAPI *CloseHandle) (HANDLE);

    /* 进程加载参数 */
    BOOL    isSuspended;
    CHAR    workPath[MAX_PATH];
    CHAR    cmdString[MAX_PATH * 2];

    /* 返回的进程参数 */
    PROCESS_INFORMATION     pi;
};

CR_API VOID WINAPI EnableDebugPriv (VOID);
CR_API UINT WINAPI RemoteInject (DWORD prcss_id, LPCWSTR dll_name);
CR_API UINT WINAPI RemoteInjectEx (DWORD prcss_id, LPVOID code,
                        DWORD code_size, LPVOID parm, DWORD parm_size);
CR_API UINT WINAPI FreeDllModule (DWORD prcss_id, LPCWSTR dll_name);
CR_API UINT WINAPI ExeLoader (sLoaderParam *param);

/*****************************************************************************/
/*                                 进程控制                                  */
/*****************************************************************************/

CR_API UINT WINAPI ProcessList (PROCESSENTRY32W **list);
CR_API UINT WINAPI ThreadList (DWORD prcss_id, THREADENTRY32 **list);
CR_API UINT WINAPI ModuleList (DWORD prcss_id, MODULEENTRY32W **list);
CR_API BOOL WINAPI ProcessCtrl (DWORD prcss_id, BOOL suspend);

/*****************************************************************************/
/*                                 窗口控制                                  */
/*****************************************************************************/

CR_API UINT WINAPI HWndList (HWND parent, HWND **list);
CR_API HWND WINAPI HWndFind (HWND parent, LPCWSTR wclass, LPCWSTR wtitle);

/*****************************************************************************/
/*                                 函数钩子                                  */
/*****************************************************************************/

/* 函数哈希查找表单元结构 */
struct  sHookFunc
{
        LPCSTR  name;   /* 函数名称字符串 */

        union {
                LPVOID  addr;   /* 普通函数的地址 */
                SIZE_T  index;  /* COM 方法的索引 */
        } func;

        void free () {}
};

/* 函数哈希表对象 */
typedef LPVOID  hookfunc_t;

/* 普通函数钩子查找 */
CR_API hookfunc_t WINAPI HookFuncNew (const sHookFunc *func, UINT count);
CR_API VOID WINAPI HookFuncDel (hookfunc_t table);
CR_API LPVOID WINAPI HookFuncGet (LPCSTR name, hookfunc_t table);

/* COM 方法钩子设置 */
CR_API UINT WINAPI HookVTable (LPVOID object, const sHookFunc *func,
                               UINT count, hookfunc_t table);

#endif  /* !__QL_HACKLIB_H__ */
