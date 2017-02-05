
#include "BackPort.h"

/* 函数重映射表 */
static sBP_FUNC s_remap1[] =
{
    { NULL, NULL, "CloseHandle" },
    { NULL, NULL, "CreateFileW" },
    { NULL, NULL, "DecodePointer" },
    { NULL, NULL, "DeleteCriticalSection" },
    { NULL, NULL, "EncodePointer" },
    { NULL, NULL, "EnterCriticalSection" },
    { NULL, NULL, "ExitProcess" },
    { NULL, NULL, "FlsAlloc" },
    { NULL, NULL, "FlsFree" },
    { NULL, NULL, "FlsGetValue" },
    { NULL, NULL, "FlsSetValue" },
    { NULL, NULL, "FlushFileBuffers" },
    { NULL, NULL, "FreeEnvironmentStringsW" },
    { NULL, NULL, "GetACP" },
    { NULL, NULL, "GetCommandLineA" },
    { NULL, NULL, "GetConsoleCP" },
    { NULL, NULL, "GetConsoleMode" },
    { NULL, NULL, "GetCPInfo" },
    { NULL, NULL, "GetCurrentProcess" },
    { NULL, NULL, "GetCurrentThreadId" },
    { NULL, NULL, "GetEnvironmentStringsW" },
    { NULL, NULL, "GetFileType" },
    { NULL, NULL, "GetLastError" },
    { NULL, NULL, "GetModuleFileNameA" },
    { NULL, NULL, "GetModuleFileNameW" },
    { NULL, NULL, "GetModuleHandleExW" },
    { NULL, NULL, "GetModuleHandleW" },
    { NULL, NULL, "GetOEMCP" },
    { NULL, NULL, "GetProcAddress" },
    { NULL, NULL, "GetProcessHeap" },
    { NULL, NULL, "GetStartupInfoW" },
    { NULL, NULL, "GetStdHandle" },
    { NULL, NULL, "GetStringTypeW" },
    { NULL, NULL, "GetSystemTimeAsFileTime" },
    { NULL, NULL, "GetTickCount64" },
    { NULL, NULL, "HeapAlloc" },
    { NULL, NULL, "HeapFree" },
    { NULL, NULL, "HeapReAlloc" },
    { NULL, NULL, "HeapSize" },
    { NULL, NULL, "InitializeCriticalSectionAndSpinCount" },
    { NULL, NULL, "InitOnceExecuteOnce" },
    { NULL, NULL, "InterlockedDecrement" },
    { NULL, NULL, "InterlockedIncrement" },
    { NULL, NULL, "IsDebuggerPresent" },
    { NULL, NULL, "IsProcessorFeaturePresent" },
    { NULL, NULL, "IsValidCodePage" },
    { NULL, NULL, "LCMapStringEx" },
    { NULL, NULL, "LeaveCriticalSection" },
    { NULL, NULL, "LoadLibraryExW" },
    { NULL, NULL, "LoadLibraryW" },
    { NULL, NULL, "MultiByteToWideChar" },
    { NULL, NULL, "OutputDebugStringW" },
    { NULL, NULL, "QueryPerformanceCounter" },
    { NULL, NULL, "RaiseException" },
    { NULL, NULL, "RtlUnwind" },
    { NULL, NULL, "SetFilePointerEx" },
    { NULL, NULL, "SetLastError" },
    { NULL, NULL, "SetStdHandle" },
    { NULL, NULL, "SetUnhandledExceptionFilter" },
    { NULL, NULL, "Sleep" },
    { NULL, NULL, "TerminateProcess" },
    { NULL, NULL, "UnhandledExceptionFilter" },
    { NULL, NULL, "WideCharToMultiByte" },
    { NULL, NULL, "WriteConsoleW" },
    { NULL, NULL, "WriteFile" },
    { NULL, NULL, NULL },
};

/* DLL 句柄 */
static void_t*  s_kernel32 = NULL;

/*
=======================================
    DLL 入口点
=======================================
*/
int __stdcall
DllMain (
  __CR_IN__ void*   hinst,
  __CR_IN__ int     reason,
  __CR_UU__ void*   reserved
    )
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            s_kernel32 = func_load("KERNEL32.dll");
            if (s_kernel32 != NULL)
                func_setup(s_kernel32, s_remap1);
            break;

        case DLL_PROCESS_DETACH:
            if (s_kernel32 != NULL)
                func_free(s_kernel32);
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}
