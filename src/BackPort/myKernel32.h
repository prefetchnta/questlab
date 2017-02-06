#ifndef __MY_KERNEL32_H__
#define __MY_KERNEL32_H__

/* XP 下未实现的 API 函数 */
unsigned long __stdcall myFlsAlloc (void *lpCallback);
int __stdcall myFlsFree (unsigned long dwFlsIndex);
void* __stdcall myFlsGetValue (unsigned long dwFlsIndex);
int __stdcall myFlsSetValue (unsigned long dwFlsIndex, void *lpFlsData);
int __stdcall myInitOnceExecuteOnce (void *InitOnce, void *InitFn,
                                     void *Parameter, void **Context);
unsigned __int64 __stdcall myGetTickCount64 (void);
int __stdcall myLCMapStringEx (void *lpLocaleName, unsigned long dwMapFlags,
                               void *lpSrcStr, int cchSrc, void *lpDestStr,
                               int cchDest, void *lpVersionInformation,
                               void *lpReserved, unsigned long sortHandle);

#endif  /* !__MY_KERNEL32_H__ */
