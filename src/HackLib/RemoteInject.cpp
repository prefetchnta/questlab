
#include "HackLib.h"

/*
=======================================
    进程提权
=======================================
*/
CR_API VOID WINAPI
EnableDebugPriv (VOID)
{
    HANDLE              app;
    HANDLE              tkn;
    TOKEN_PRIVILEGES    tkp;

    /* 开调试权限 */
    app = GetCurrentProcess();
    OpenProcessToken(app, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tkn);
    LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(tkn, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    CloseHandle(tkn);
}

/*
=======================================
    DLL 注入
=======================================
*/
CR_API UINT WINAPI
RemoteInject (
  __CR_IN__ DWORD   prcss_id,
  __CR_IN__ LPCWSTR dll_name
    )
{
    LPVOID  remote_file;
    HANDLE  thread, process;

    /* 打开远程进程 */
    process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, prcss_id);
    if (process == NULL)
        return (HLB_ERROR);

    int size = (lstrlenW(dll_name) + 1) * sizeof(WCHAR);

    /* 在远程进程中分配存贮 DLL 文件名的空间 */
    remote_file = VirtualAllocEx(process, NULL, size,
                                 MEM_COMMIT, PAGE_READWRITE);
    if (remote_file == NULL) {
        CloseHandle(process);
        return (HLB_ERROR);
    }

    /* 复制 DLL 文件名到远程刚分配的进程空间 */
    if (!WriteProcessMemory(process, remote_file, dll_name, size, NULL)) {
        VirtualFreeEx(process, remote_file, 0, MEM_RELEASE);
        CloseHandle(process);
        return (HLB_ERROR);
    }

    HMODULE                 krnl;
    PTHREAD_START_ROUTINE   func;

    /* 取得 LoadLibrary 函数在 kennel32.dll 中的地址 */
    krnl = GetModuleHandleW(L"kernel32.dll");
    func = (PTHREAD_START_ROUTINE)GetProcAddress(krnl, "LoadLibraryW");
    if (func == NULL) {
        VirtualFreeEx(process, remote_file, 0, MEM_RELEASE);
        CloseHandle(process);
        return (HLB_ERROR);
    }

    /* 创建远程线程执行之 */
    thread = CreateRemoteThread(process, NULL, 0, func, remote_file, 0, NULL);
    if (thread == NULL) {
        VirtualFreeEx(process, remote_file, 0, MEM_RELEASE);
        CloseHandle(process);
        return (HLB_ERROR);
    }

    /* 等待线程返回 */
    WaitForSingleObject(thread, INFINITE);

    /* 释放进程空间中的内存 */
    VirtualFreeEx(process, remote_file, 0, MEM_RELEASE);

    /* 关闭句柄 */
    CloseHandle(thread);
    CloseHandle(process);
    return (HLB_OKAY);
}

/*
=======================================
    代码注入
=======================================
*/
CR_API UINT WINAPI
RemoteInjectEx (
  __CR_IN__ DWORD   prcss_id,
  __CR_IN__ LPVOID  code,
  __CR_IN__ DWORD   code_size,
  __CR_IO__ LPVOID  parm,
  __CR_IN__ DWORD   parm_size
    )
{
    LPVOID  remote_code;
    LPVOID  remote_data;
    HANDLE  thread, process;

    /* 打开远程进程 */
    process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, prcss_id);
    if (process == NULL)
        return (HLB_ERROR);

    /* 在远程进程中分配存贮代码和参数的空间 */
    remote_code = VirtualAllocEx(process, NULL, code_size,
                                 MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    remote_data = VirtualAllocEx(process, NULL, parm_size,
                                 MEM_COMMIT, PAGE_READWRITE);
    if (remote_code == NULL || remote_data == NULL) {
        if (remote_code != NULL)
            VirtualFreeEx(process, remote_code, 0, MEM_RELEASE);
        if (remote_data != NULL)
            VirtualFreeEx(process, remote_data, 0, MEM_RELEASE);
        CloseHandle(process);
        return (HLB_ERROR);
    }

    /* 复制代码和参数到远程刚分配的进程空间 */
    if (!WriteProcessMemory(process, remote_code, code, code_size, NULL)) {
        VirtualFreeEx(process, remote_code, 0, MEM_RELEASE);
        VirtualFreeEx(process, remote_data, 0, MEM_RELEASE);
        CloseHandle(process);
        return (HLB_ERROR);
    }
    if (!WriteProcessMemory(process, remote_data, parm, parm_size, NULL)) {
        VirtualFreeEx(process, remote_code, 0, MEM_RELEASE);
        VirtualFreeEx(process, remote_data, 0, MEM_RELEASE);
        CloseHandle(process);
        return (HLB_ERROR);
    }

    /* 创建远程线程 */
    thread = CreateRemoteThread(process, NULL, 0, (PTHREAD_START_ROUTINE)
                                remote_code, remote_data, 0, NULL);
    if (thread == NULL) {
        VirtualFreeEx(process, remote_code, 0, MEM_RELEASE);
        VirtualFreeEx(process, remote_data, 0, MEM_RELEASE);
        CloseHandle(process);
        return (HLB_ERROR);
    }

    /* 等待线程返回 */
    WaitForSingleObject(thread, INFINITE);

    /* 读回参数数据 */
    ReadProcessMemory(process, remote_data, parm, parm_size, NULL);

    /* 释放进程空间中的内存 */
    VirtualFreeEx(process, remote_code, 0, MEM_RELEASE);
    VirtualFreeEx(process, remote_data, 0, MEM_RELEASE);

    /* 关闭句柄 */
    CloseHandle(thread);
    CloseHandle(process);
    return (HLB_OKAY);
}

/*
=======================================
    DLL 卸载
=======================================
*/
CR_API UINT WINAPI
FreeDllModule (
  __CR_IN__ DWORD   prcss_id,
  __CR_IN__ LPCWSTR dll_name
    )
{
    HANDLE          snapshot;
    HANDLE          thread, process;
    MODULEENTRY32W  mod = { sizeof(mod) };

    /* 取得指定进程的所有模块信息 */
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, prcss_id);
    if (snapshot == INVALID_HANDLE_VALUE)
        return (HLB_ERROR);

    BOOL    found;

    /* 循环取得想要的模块 */
    found = Module32FirstW(snapshot, &mod);
    if (!found) {
        CloseHandle(snapshot);
        return (HLB_ERROR);
    }
    for (; found; found = Module32NextW(snapshot, &mod)) {
        if (lstrcmpW(mod.szExePath, dll_name) == 0 ||
            lstrcmpW(mod.szModule,  dll_name) == 0)
            break;
    }
    if (!found) {
        CloseHandle(snapshot);
        return (HLB_ERROR);
    }

    /* 打开进程 */
    process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, prcss_id);
    if (process == NULL) {
        CloseHandle(snapshot);
        return (HLB_ERROR);
    }

    HMODULE                 krnl;
    PTHREAD_START_ROUTINE   func;

    /* 取得 FreeLibrary 函数在 kernel32.dll 中的地址 */
    krnl = GetModuleHandleW(L"kernel32.dll");
    func = (PTHREAD_START_ROUTINE)GetProcAddress(krnl, "FreeLibrary");
    if (func == NULL) {
        CloseHandle(process);
        CloseHandle(snapshot);
        return (HLB_ERROR);
    }

    /* 创建远程线程来执行 FreeLibrary 函数 */
    thread = CreateRemoteThread(process, NULL, 0, func,
                                mod.modBaseAddr, 0, NULL);
    if (thread == NULL) {
        CloseHandle(process);
        CloseHandle(snapshot);
        return (HLB_ERROR);
    }

    /* 等待线程返回 */
    WaitForSingleObject(thread, INFINITE);

    /* 关闭句柄 */
    CloseHandle(thread);
    CloseHandle(process);
    CloseHandle(snapshot);
    return (HLB_OKAY);
}

/*
=======================================
    进程加载
=======================================
*/
CR_API UINT WINAPI
ExeLoader (
  __CR_IO__ sLoaderParam*   param
    )
{
    VOID*           pt;
    DWORD           fg;
    STARTUPINFO     si;

    /* 启动进程 */
    pt = (VOID*)(&si);
    fg = sizeof(STARTUPINFO);
    __asm {
        mov edi, pt
        mov ecx, fg
        xor eax, eax
        cld
        rep stosb
    }
    pt = (VOID*)(&param->pi);
    fg = sizeof(PROCESS_INFORMATION);
    __asm {
        mov edi, pt
        mov ecx, fg
        xor eax, eax
        cld
        rep stosb
    }
    si.cb = sizeof(si);
    fg = param->isSuspended ? CREATE_SUSPENDED : 0;
    if (!param->CreateProcessA(NULL, param->cmdString, NULL, NULL, FALSE,
                            fg, NULL, param->workPath, &si, &param->pi))
        return (HLB_ERROR);

    /* 两个句柄在插入式加载时无用 */
    param->CloseHandle(param->pi.hProcess);
    param->CloseHandle(param->pi.hThread);
    param->pi.hProcess = NULL;
    param->pi.hThread  = NULL;
    return (HLB_OKAY);
}
