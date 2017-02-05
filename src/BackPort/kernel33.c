
#include "BackPort.h"

/* 函数重映射表 */
static sBP_FUNC s_remap[] =
{
    { NULL, "CloseHandle" },
    { NULL, "CreateFileW" },
    { NULL, "DecodePointer" },
    { NULL, "DeleteCriticalSection" },
    { NULL, "EncodePointer" },
    { NULL, "EnterCriticalSection" },
    { NULL, "ExitProcess" },
    { NULL, "FlsAlloc" },
    { NULL, "FlsFree" },
    { NULL, "FlsGetValue" },
    { NULL, "FlsSetValue" },
    { NULL, "FlushFileBuffers" },
    { NULL, "FreeEnvironmentStringsW" },
    { NULL, "GetACP" },
    { NULL, "GetCommandLineA" },
    { NULL, "GetConsoleCP" },
    { NULL, "GetConsoleMode" },
    { NULL, "GetCPInfo" },
    { NULL, "GetCurrentProcess" },
    { NULL, "GetCurrentThreadId" },
    { NULL, "GetEnvironmentStringsW" },
    { NULL, "GetFileType" },
    { NULL, "GetLastError" },
    { NULL, "GetModuleFileNameA" },
    { NULL, "GetModuleFileNameW" },
    { NULL, "GetModuleHandleExW" },
    { NULL, "GetModuleHandleW" },
    { NULL, "GetOEMCP" },
    { NULL, "GetProcAddress" },
    { NULL, "GetProcessHeap" },
    { NULL, "GetStartupInfoW" },
    { NULL, "GetStdHandle" },
    { NULL, "GetStringTypeW" },
    { NULL, "GetSystemTimeAsFileTime" },
    { NULL, "GetTickCount64" },
    { NULL, "HeapAlloc" },
    { NULL, "HeapFree" },
    { NULL, "HeapReAlloc" },
    { NULL, "HeapSize" },
    { NULL, "InitializeCriticalSectionAndSpinCount" },
    { NULL, "InitOnceExecuteOnce" },
    { NULL, "InterlockedDecrement" },
    { NULL, "InterlockedIncrement" },
    { NULL, "IsDebuggerPresent" },
    { NULL, "IsProcessorFeaturePresent" },
    { NULL, "IsValidCodePage" },
    { NULL, "LCMapStringEx" },
    { NULL, "LeaveCriticalSection" },
    { NULL, "LoadLibraryExW" },
    { NULL, "LoadLibraryW" },
    { NULL, "MultiByteToWideChar" },
    { NULL, "OutputDebugStringW" },
    { NULL, "QueryPerformanceCounter" },
    { NULL, "RaiseException" },
    { NULL, "RtlUnwind" },
    { NULL, "SetFilePointerEx" },
    { NULL, "SetLastError" },
    { NULL, "SetStdHandle" },
    { NULL, "SetUnhandledExceptionFilter" },
    { NULL, "Sleep" },
    { NULL, "TerminateProcess" },
    { NULL, "UnhandledExceptionFilter" },
    { NULL, "WideCharToMultiByte" },
    { NULL, "WriteConsoleW" },
    { NULL, "WriteFile" },
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
            s_kernel32 = func_load(s_remap, "KERNEL32.dll");
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
