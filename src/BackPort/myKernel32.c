
#include "crhack.h"

/*
=======================================
    FlsAlloc
=======================================
*/
unsigned long __stdcall
myFlsAlloc (
  __CR_IN__ void*   lpCallback
    )
{
    CR_NOUSE(lpCallback);
    return (TlsAlloc());
}

/*
=======================================
    FlsFree
=======================================
*/
int __stdcall
myFlsFree (
  __CR_IN__ unsigned long   dwFlsIndex
    )
{
    return (TlsFree(dwFlsIndex));
}

/*
=======================================
    FlsGetValue
=======================================
*/
void* __stdcall
myFlsGetValue (
  __CR_IN__ unsigned long   dwFlsIndex
    )
{
    return (TlsGetValue(dwFlsIndex));
}

/*
=======================================
    FlsSetValue
=======================================
*/
int __stdcall
myFlsSetValue (
  __CR_IN__ unsigned long   dwFlsIndex,
  __CR_IN__ void*           lpFlsData
    )
{
    return (TlsSetValue(dwFlsIndex, lpFlsData));
}

/* 回调函数类型 */
typedef BOOL CALLBACK (*init_once_t) (void*, void*, void**);

/*
=======================================
    InitOnceExecuteOnce
=======================================
*/
int __stdcall
myInitOnceExecuteOnce (
  __CR_IN__ void*   InitOnce,
  __CR_IN__ void*   InitFn,
  __CR_IN__ void*   Parameter,
  __CR_IN__ void**  Context
    )
{
    init_once_t func = (init_once_t)InitFn;

    if (!_InterlockedCompareExchange(InitOnce, TRUE, FALSE))
        func(InitOnce, Parameter, Context);
}

/*
=======================================
    GetTickCount64
=======================================
*/
unsigned __int64 __stdcall
myGetTickCount64 (void)
{
    return (GetTickCount());
}

/*
=======================================
    LCMapStringEx
=======================================
*/
int __stdcall
myLCMapStringEx (
  __CR_IN__ void*           lpLocaleName,
  __CR_IN__ unsigned long   dwMapFlags,
  __CR_IN__ void*           lpSrcStr,
  __CR_IN__ int             cchSrc,
  __CR_IN__ void*           lpDestStr,
  __CR_IN__ int             cchDest,
  __CR_IN__ void*           lpVersionInformation,
  __CR_IN__ void*           lpReserved,
  __CR_IN__ unsigned long   sortHandle
    )
{
}
