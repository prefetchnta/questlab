
#include "BackPort.h"
#include "myKernel32.h"

/* 函数重映射表 */
static sBP_FUNC s_remap1[] =
{
    BACKPORT_JUMP(CloseHandle),
    BACKPORT_JUMP(CreateFileW),
    BACKPORT_JUMP(DecodePointer),
    BACKPORT_JUMP(DeleteCriticalSection),
    BACKPORT_JUMP(EncodePointer),
    BACKPORT_JUMP(EnterCriticalSection),
    BACKPORT_JUMP(ExitProcess),
    BACKPORT_RPLC(FlsAlloc),
    BACKPORT_RPLC(FlsFree),
    BACKPORT_RPLC(FlsGetValue),
    BACKPORT_RPLC(FlsSetValue),
    BACKPORT_JUMP(FlushFileBuffers),
    BACKPORT_JUMP(FreeEnvironmentStringsW),
    BACKPORT_JUMP(GetACP),
    BACKPORT_JUMP(GetCommandLineA),
    BACKPORT_JUMP(GetConsoleCP),
    BACKPORT_JUMP(GetConsoleMode),
    BACKPORT_JUMP(GetCPInfo),
    BACKPORT_JUMP(GetCurrentProcess),
    BACKPORT_JUMP(GetCurrentThreadId),
    BACKPORT_JUMP(GetEnvironmentStringsW),
    BACKPORT_JUMP(GetFileType),
    BACKPORT_JUMP(GetLastError),
    BACKPORT_JUMP(GetModuleFileNameA),
    BACKPORT_JUMP(GetModuleFileNameW),
    BACKPORT_JUMP(GetModuleHandleExW),
    BACKPORT_JUMP(GetModuleHandleW),
    BACKPORT_JUMP(GetOEMCP),
    BACKPORT_JUMP(GetProcAddress),
    BACKPORT_JUMP(GetProcessHeap),
    BACKPORT_JUMP(GetStartupInfoW),
    BACKPORT_JUMP(GetStdHandle),
    BACKPORT_JUMP(GetStringTypeW),
    BACKPORT_JUMP(GetSystemTimeAsFileTime),
    BACKPORT_RPLC(GetTickCount64),
    BACKPORT_JUMP(HeapAlloc),
    BACKPORT_JUMP(HeapFree),
    BACKPORT_JUMP(HeapReAlloc),
    BACKPORT_JUMP(HeapSize),
    BACKPORT_JUMP(InitializeCriticalSectionAndSpinCount),
    BACKPORT_RPLC(InitOnceExecuteOnce),
    BACKPORT_JUMP(InterlockedDecrement),
    BACKPORT_JUMP(InterlockedIncrement),
    BACKPORT_JUMP(IsDebuggerPresent),
    BACKPORT_JUMP(IsProcessorFeaturePresent),
    BACKPORT_JUMP(IsValidCodePage),
    BACKPORT_RPLC(LCMapStringEx),
    BACKPORT_JUMP(LeaveCriticalSection),
    BACKPORT_JUMP(LoadLibraryExW),
    BACKPORT_JUMP(LoadLibraryW),
    BACKPORT_JUMP(MultiByteToWideChar),
    BACKPORT_JUMP(OutputDebugStringW),
    BACKPORT_JUMP(QueryPerformanceCounter),
    BACKPORT_JUMP(RaiseException),
    BACKPORT_JUMP(RtlUnwind),
    BACKPORT_JUMP(SetFilePointerEx),
    BACKPORT_JUMP(SetLastError),
    BACKPORT_JUMP(SetStdHandle),
    BACKPORT_JUMP(SetUnhandledExceptionFilter),
    BACKPORT_JUMP(Sleep),
    BACKPORT_JUMP(TerminateProcess),
    BACKPORT_JUMP(UnhandledExceptionFilter),
    BACKPORT_JUMP(WideCharToMultiByte),
    BACKPORT_JUMP(WriteConsoleW),
    BACKPORT_JUMP(WriteFile),
    BACKPORT_FINISH
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

/************/
/* s_remap1 */
/************/
BACKPORT_FUNC(s_remap1,  0, CloseHandle)
BACKPORT_FUNC(s_remap1,  1, CreateFileW)
BACKPORT_FUNC(s_remap1,  2, DecodePointer)
BACKPORT_FUNC(s_remap1,  3, DeleteCriticalSection)
BACKPORT_FUNC(s_remap1,  4, EncodePointer)
BACKPORT_FUNC(s_remap1,  5, EnterCriticalSection)
BACKPORT_FUNC(s_remap1,  6, ExitProcess)
BACKPORT_FUNC(s_remap1,  7, FlsAlloc)
BACKPORT_FUNC(s_remap1,  8, FlsFree)
BACKPORT_FUNC(s_remap1,  9, FlsGetValue)
BACKPORT_FUNC(s_remap1, 10, FlsSetValue)
BACKPORT_FUNC(s_remap1, 11, FlushFileBuffers)
BACKPORT_FUNC(s_remap1, 12, FreeEnvironmentStringsW)
BACKPORT_FUNC(s_remap1, 13, GetACP)
BACKPORT_FUNC(s_remap1, 14, GetCommandLineA)
BACKPORT_FUNC(s_remap1, 15, GetConsoleCP)
BACKPORT_FUNC(s_remap1, 16, GetConsoleMode)
BACKPORT_FUNC(s_remap1, 17, GetCPInfo)
BACKPORT_FUNC(s_remap1, 18, GetCurrentProcess)
BACKPORT_FUNC(s_remap1, 19, GetCurrentThreadId)
BACKPORT_FUNC(s_remap1, 20, GetEnvironmentStringsW)
BACKPORT_FUNC(s_remap1, 21, GetFileType)
BACKPORT_FUNC(s_remap1, 22, GetLastError)
BACKPORT_FUNC(s_remap1, 23, GetModuleFileNameA)
BACKPORT_FUNC(s_remap1, 24, GetModuleFileNameW)
BACKPORT_FUNC(s_remap1, 25, GetModuleHandleExW)
BACKPORT_FUNC(s_remap1, 26, GetModuleHandleW)
BACKPORT_FUNC(s_remap1, 27, GetOEMCP)
BACKPORT_FUNC(s_remap1, 28, GetProcAddress)
BACKPORT_FUNC(s_remap1, 29, GetProcessHeap)
BACKPORT_FUNC(s_remap1, 30, GetStartupInfoW)
BACKPORT_FUNC(s_remap1, 31, GetStdHandle)
BACKPORT_FUNC(s_remap1, 32, GetStringTypeW)
BACKPORT_FUNC(s_remap1, 33, GetSystemTimeAsFileTime)
BACKPORT_FUNC(s_remap1, 34, GetTickCount64)
BACKPORT_FUNC(s_remap1, 35, HeapAlloc)
BACKPORT_FUNC(s_remap1, 36, HeapFree)
BACKPORT_FUNC(s_remap1, 37, HeapReAlloc)
BACKPORT_FUNC(s_remap1, 38, HeapSize)
BACKPORT_FUNC(s_remap1, 39, InitializeCriticalSectionAndSpinCount)
BACKPORT_FUNC(s_remap1, 40, InitOnceExecuteOnce)
BACKPORT_FUNC(s_remap1, 41, InterlockedDecrement)
BACKPORT_FUNC(s_remap1, 42, InterlockedIncrement)
BACKPORT_FUNC(s_remap1, 43, IsDebuggerPresent)
BACKPORT_FUNC(s_remap1, 44, IsProcessorFeaturePresent)
BACKPORT_FUNC(s_remap1, 45, IsValidCodePage)
BACKPORT_FUNC(s_remap1, 46, LCMapStringEx)
BACKPORT_FUNC(s_remap1, 47, LeaveCriticalSection)
BACKPORT_FUNC(s_remap1, 48, LoadLibraryExW)
BACKPORT_FUNC(s_remap1, 49, LoadLibraryW)
BACKPORT_FUNC(s_remap1, 50, MultiByteToWideChar)
BACKPORT_FUNC(s_remap1, 51, OutputDebugStringW)
BACKPORT_FUNC(s_remap1, 52, QueryPerformanceCounter)
BACKPORT_FUNC(s_remap1, 53, RaiseException)
BACKPORT_FUNC(s_remap1, 54, RtlUnwind)
BACKPORT_FUNC(s_remap1, 55, SetFilePointerEx)
BACKPORT_FUNC(s_remap1, 56, SetLastError)
BACKPORT_FUNC(s_remap1, 57, SetStdHandle)
BACKPORT_FUNC(s_remap1, 58, SetUnhandledExceptionFilter)
BACKPORT_FUNC(s_remap1, 59, Sleep)
BACKPORT_FUNC(s_remap1, 60, TerminateProcess)
BACKPORT_FUNC(s_remap1, 61, UnhandledExceptionFilter)
BACKPORT_FUNC(s_remap1, 62, WideCharToMultiByte)
BACKPORT_FUNC(s_remap1, 63, WriteConsoleW)
BACKPORT_FUNC(s_remap1, 64, WriteFile)
