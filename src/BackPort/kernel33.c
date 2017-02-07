
#include "BackPort.h"
#include "myKernel32.h"

/* 函数重映射表 */
static sBP_FUNC s_remap1_tbl[] =
{
    BACKPORT_JUMP(ActivateActCtx),
    BACKPORT_JUMP(AddAtomA),
    BACKPORT_JUMP(AddAtomW),
    BACKPORT_JUMP(AddConsoleAliasA),
    BACKPORT_JUMP(AddConsoleAliasW),
    BACKPORT_JUMP(AddLocalAlternateComputerNameA),
    BACKPORT_JUMP(AddLocalAlternateComputerNameW),
    BACKPORT_JUMP(AddRefActCtx),
    BACKPORT_JUMP(AddVectoredExceptionHandler),
    BACKPORT_JUMP(AllocateUserPhysicalPages),
    BACKPORT_JUMP(AllocConsole),
    BACKPORT_JUMP(AreFileApisANSI),
    BACKPORT_JUMP(AssignProcessToJobObject),
    BACKPORT_JUMP(AttachConsole),
    BACKPORT_JUMP(BackupRead),
    BACKPORT_JUMP(BackupSeek),
    BACKPORT_JUMP(BackupWrite),
    BACKPORT_JUMP(BaseCheckAppcompatCache),
    BACKPORT_JUMP(BaseCleanupAppcompatCacheSupport),
    BACKPORT_JUMP(BaseDumpAppcompatCache),
    BACKPORT_JUMP(BaseFlushAppcompatCache),
    BACKPORT_JUMP(BaseInitAppcompatCacheSupport),
    BACKPORT_JUMP(BasepCheckWinSaferRestrictions),
    BACKPORT_JUMP(BaseQueryModuleData),
    BACKPORT_JUMP(BaseUpdateAppcompatCache),
    BACKPORT_JUMP(Beep),
    BACKPORT_JUMP(BeginUpdateResourceA),
    BACKPORT_JUMP(BeginUpdateResourceW),
    BACKPORT_JUMP(BindIoCompletionCallback),
    BACKPORT_JUMP(BuildCommDCBA),
    BACKPORT_JUMP(BuildCommDCBAndTimeoutsA),
    BACKPORT_JUMP(BuildCommDCBAndTimeoutsW),
    BACKPORT_JUMP(BuildCommDCBW),
    BACKPORT_JUMP(CallNamedPipeA),
    BACKPORT_JUMP(CallNamedPipeW),
    BACKPORT_JUMP(CancelDeviceWakeupRequest),
    BACKPORT_JUMP(CancelIo),
    BACKPORT_JUMP(CancelTimerQueueTimer),
    BACKPORT_JUMP(CancelWaitableTimer),
    BACKPORT_JUMP(ChangeTimerQueueTimer),
    BACKPORT_JUMP(CheckNameLegalDOS8Dot3A),
    BACKPORT_JUMP(CheckNameLegalDOS8Dot3W),
    BACKPORT_JUMP(CheckRemoteDebuggerPresent),
    BACKPORT_JUMP(ClearCommBreak),
    BACKPORT_JUMP(ClearCommError),
    BACKPORT_JUMP(CloseConsoleHandle),
    BACKPORT_JUMP(CloseHandle),
    BACKPORT_JUMP(CloseProfileUserMapping),
    BACKPORT_JUMP(CmdBatNotification),
    BACKPORT_JUMP(CommConfigDialogA),
    BACKPORT_JUMP(CommConfigDialogW),
    BACKPORT_JUMP(CompareFileTime),
    BACKPORT_JUMP(CompareStringA),
    BACKPORT_JUMP(CompareStringW),
    BACKPORT_JUMP(ConnectNamedPipe),
    BACKPORT_JUMP(ConsoleMenuControl),
    BACKPORT_JUMP(ContinueDebugEvent),
    BACKPORT_JUMP(ConvertDefaultLocale),
    BACKPORT_JUMP(ConvertFiberToThread),
    BACKPORT_JUMP(ConvertThreadToFiber),
    BACKPORT_JUMP(CopyFileA),
    BACKPORT_JUMP(CopyFileExA),
    BACKPORT_JUMP(CopyFileExW),
    BACKPORT_JUMP(CopyFileW),
    BACKPORT_JUMP(CopyLZFile),
    BACKPORT_JUMP(CreateActCtxA),
    BACKPORT_JUMP(CreateActCtxW),
    BACKPORT_JUMP(CreateConsoleScreenBuffer),
    BACKPORT_JUMP(CreateDirectoryA),
    BACKPORT_JUMP(CreateDirectoryExA),
    BACKPORT_JUMP(CreateDirectoryExW),
    BACKPORT_JUMP(CreateDirectoryW),
    BACKPORT_JUMP(CreateEventA),
    BACKPORT_JUMP(CreateEventW),
    BACKPORT_JUMP(CreateFiber),
    BACKPORT_JUMP(CreateFiberEx),
    BACKPORT_JUMP(CreateFileA),
    BACKPORT_JUMP(CreateFileMappingA),
    BACKPORT_JUMP(CreateFileMappingW),
    BACKPORT_JUMP(CreateFileW),
    BACKPORT_JUMP(CreateHardLinkA),
    BACKPORT_JUMP(CreateHardLinkW),
    BACKPORT_JUMP(CreateIoCompletionPort),
    BACKPORT_JUMP(CreateJobObjectA),
    BACKPORT_JUMP(CreateJobObjectW),
    BACKPORT_JUMP(CreateJobSet),
    BACKPORT_JUMP(CreateMailslotA),
    BACKPORT_JUMP(CreateMailslotW),
    BACKPORT_JUMP(CreateMemoryResourceNotification),
    BACKPORT_JUMP(CreateMutexA),
    BACKPORT_JUMP(CreateMutexW),
    BACKPORT_JUMP(CreateNamedPipeA),
    BACKPORT_JUMP(CreateNamedPipeW),
    BACKPORT_JUMP(CreatePipe),
    BACKPORT_JUMP(CreateProcessA),
    BACKPORT_JUMP(CreateProcessInternalA),
    BACKPORT_JUMP(CreateProcessInternalW),
    BACKPORT_JUMP(CreateProcessW),
    BACKPORT_JUMP(CreateRemoteThread),
    BACKPORT_JUMP(CreateSemaphoreA),
    BACKPORT_JUMP(CreateSemaphoreW),
    BACKPORT_JUMP(CreateSocketHandle),
    BACKPORT_JUMP(CreateTapePartition),
    BACKPORT_JUMP(CreateThread),
    BACKPORT_JUMP(CreateTimerQueue),
    BACKPORT_JUMP(CreateTimerQueueTimer),
    BACKPORT_JUMP(CreateToolhelp32Snapshot),
    BACKPORT_JUMP(CreateWaitableTimerA),
    BACKPORT_JUMP(CreateWaitableTimerW),
    BACKPORT_JUMP(DeactivateActCtx),
    BACKPORT_JUMP(DebugActiveProcess),
    BACKPORT_JUMP(DebugActiveProcessStop),
    BACKPORT_JUMP(DebugBreak),
    BACKPORT_JUMP(DebugBreakProcess),
    BACKPORT_JUMP(DebugSetProcessKillOnExit),
    BACKPORT_JUMP(DecodePointer),
    BACKPORT_JUMP(DecodeSystemPointer),
    BACKPORT_JUMP(DefineDosDeviceA),
    BACKPORT_JUMP(DefineDosDeviceW),
    BACKPORT_JUMP(DelayLoadFailureHook),
    BACKPORT_JUMP(DeleteAtom),
    BACKPORT_JUMP(DeleteCriticalSection),
    BACKPORT_JUMP(DeleteFiber),
    BACKPORT_JUMP(DeleteFileA),
    BACKPORT_JUMP(DeleteFileW),
    BACKPORT_JUMP(DeleteTimerQueue),
    BACKPORT_JUMP(DeleteTimerQueueEx),
    BACKPORT_JUMP(DeleteTimerQueueTimer),
    BACKPORT_JUMP(DeleteVolumeMountPointA),
    BACKPORT_JUMP(DeleteVolumeMountPointW),
    BACKPORT_JUMP(DeviceIoControl),
    BACKPORT_JUMP(DisableThreadLibraryCalls),
    BACKPORT_JUMP(DisconnectNamedPipe),
    BACKPORT_JUMP(DnsHostnameToComputerNameA),
    BACKPORT_JUMP(DnsHostnameToComputerNameW),
    BACKPORT_JUMP(DosDateTimeToFileTime),
    BACKPORT_JUMP(DosPathToSessionPathA),
    BACKPORT_JUMP(DosPathToSessionPathW),
    BACKPORT_JUMP(DuplicateConsoleHandle),
    BACKPORT_JUMP(DuplicateHandle),
    BACKPORT_JUMP(EncodePointer),
    BACKPORT_JUMP(EncodeSystemPointer),
    BACKPORT_JUMP(EndUpdateResourceA),
    BACKPORT_JUMP(EndUpdateResourceW),
    BACKPORT_JUMP(EnterCriticalSection),
    BACKPORT_JUMP(EnumCalendarInfoA),
    BACKPORT_JUMP(EnumCalendarInfoExA),
    BACKPORT_JUMP(EnumCalendarInfoExW),
    BACKPORT_JUMP(EnumCalendarInfoW),
    BACKPORT_JUMP(EnumDateFormatsA),
    BACKPORT_JUMP(EnumDateFormatsExA),
    BACKPORT_JUMP(EnumDateFormatsExW),
    BACKPORT_JUMP(EnumDateFormatsW),
    BACKPORT_JUMP(EnumerateLocalComputerNamesA),
    BACKPORT_JUMP(EnumerateLocalComputerNamesW),
    BACKPORT_JUMP(EnumLanguageGroupLocalesA),
    BACKPORT_JUMP(EnumLanguageGroupLocalesW),
    BACKPORT_JUMP(EnumResourceLanguagesA),
    BACKPORT_JUMP(EnumResourceLanguagesW),
    BACKPORT_JUMP(EnumResourceNamesA),
    BACKPORT_JUMP(EnumResourceNamesW),
    BACKPORT_JUMP(EnumResourceTypesA),
    BACKPORT_JUMP(EnumResourceTypesW),
    BACKPORT_JUMP(EnumSystemCodePagesA),
    BACKPORT_JUMP(EnumSystemCodePagesW),
    BACKPORT_JUMP(EnumSystemGeoID),
    BACKPORT_JUMP(EnumSystemLanguageGroupsA),
    BACKPORT_JUMP(EnumSystemLanguageGroupsW),
    BACKPORT_JUMP(EnumSystemLocalesA),
    BACKPORT_JUMP(EnumSystemLocalesW),
    BACKPORT_JUMP(EnumTimeFormatsA),
    BACKPORT_JUMP(EnumTimeFormatsW),
    BACKPORT_JUMP(EnumUILanguagesA),
    BACKPORT_JUMP(EnumUILanguagesW),
    BACKPORT_JUMP(EraseTape),
    BACKPORT_JUMP(EscapeCommFunction),
    BACKPORT_JUMP(ExitProcess),
    BACKPORT_JUMP(ExitThread),
    BACKPORT_JUMP(ExitVDM),
    BACKPORT_JUMP(ExpandEnvironmentStringsA),
    BACKPORT_JUMP(ExpandEnvironmentStringsW),
    BACKPORT_JUMP(ExpungeConsoleCommandHistoryA),
    BACKPORT_JUMP(ExpungeConsoleCommandHistoryW),
    BACKPORT_JUMP(FatalAppExitA),
    BACKPORT_JUMP(FatalAppExitW),
    BACKPORT_JUMP(FatalExit),
    BACKPORT_JUMP(FileTimeToDosDateTime),
    BACKPORT_JUMP(FileTimeToLocalFileTime),
    BACKPORT_JUMP(FileTimeToSystemTime),
    BACKPORT_JUMP(FillConsoleOutputAttribute),
    BACKPORT_JUMP(FillConsoleOutputCharacterA),
    BACKPORT_JUMP(FillConsoleOutputCharacterW),
    BACKPORT_JUMP(FindActCtxSectionGuid),
    BACKPORT_JUMP(FindActCtxSectionStringA),
    BACKPORT_JUMP(FindActCtxSectionStringW),
    BACKPORT_JUMP(FindAtomA),
    BACKPORT_JUMP(FindAtomW),
    BACKPORT_JUMP(FindClose),
    BACKPORT_JUMP(FindCloseChangeNotification),
    BACKPORT_JUMP(FindFirstChangeNotificationA),
    BACKPORT_JUMP(FindFirstChangeNotificationW),
    BACKPORT_JUMP(FindFirstFileA),
    BACKPORT_JUMP(FindFirstFileExA),
    BACKPORT_JUMP(FindFirstFileExW),
    BACKPORT_JUMP(FindFirstFileW),
    BACKPORT_JUMP(FindFirstVolumeA),
    BACKPORT_JUMP(FindFirstVolumeMountPointA),
    BACKPORT_JUMP(FindFirstVolumeMountPointW),
    BACKPORT_JUMP(FindFirstVolumeW),
    BACKPORT_JUMP(FindNextChangeNotification),
    BACKPORT_JUMP(FindNextFileA),
    BACKPORT_JUMP(FindNextFileW),
    BACKPORT_JUMP(FindNextVolumeA),
    BACKPORT_JUMP(FindNextVolumeMountPointA),
    BACKPORT_JUMP(FindNextVolumeMountPointW),
    BACKPORT_JUMP(FindNextVolumeW),
    BACKPORT_JUMP(FindResourceA),
    BACKPORT_JUMP(FindResourceExA),
    BACKPORT_JUMP(FindResourceExW),
    BACKPORT_JUMP(FindResourceW),
    BACKPORT_JUMP(FindVolumeClose),
    BACKPORT_JUMP(FindVolumeMountPointClose),
    BACKPORT_RPLC(FlsAlloc),
    BACKPORT_RPLC(FlsFree),
    BACKPORT_RPLC(FlsGetValue),
    BACKPORT_RPLC(FlsSetValue),
    BACKPORT_JUMP(FlushConsoleInputBuffer),
    BACKPORT_JUMP(FlushFileBuffers),
    BACKPORT_JUMP(FlushInstructionCache),
    BACKPORT_JUMP(FlushViewOfFile),
    BACKPORT_JUMP(FoldStringA),
    BACKPORT_JUMP(FoldStringW),
    BACKPORT_JUMP(FormatMessageA),
    BACKPORT_JUMP(FormatMessageW),
    BACKPORT_JUMP(FreeConsole),
    BACKPORT_JUMP(FreeEnvironmentStringsA),
    BACKPORT_JUMP(FreeEnvironmentStringsW),
    BACKPORT_JUMP(FreeLibrary),
    BACKPORT_JUMP(FreeLibraryAndExitThread),
    BACKPORT_JUMP(FreeResource),
    BACKPORT_JUMP(FreeUserPhysicalPages),
    BACKPORT_JUMP(GenerateConsoleCtrlEvent),
    BACKPORT_JUMP(GetACP),
    BACKPORT_JUMP(GetAtomNameA),
    BACKPORT_JUMP(GetAtomNameW),
    BACKPORT_JUMP(GetBinaryType),
    BACKPORT_JUMP(GetBinaryTypeA),
    BACKPORT_JUMP(GetBinaryTypeW),
    BACKPORT_JUMP(GetCalendarInfoA),
    BACKPORT_JUMP(GetCalendarInfoW),
    BACKPORT_JUMP(GetCommandLineA),
    BACKPORT_JUMP(GetCommandLineW),
    BACKPORT_JUMP(GetCommConfig),
    BACKPORT_JUMP(GetCommMask),
    BACKPORT_JUMP(GetCommModemStatus),
    BACKPORT_JUMP(GetCommProperties),
    BACKPORT_JUMP(GetCommState),
    BACKPORT_JUMP(GetCommTimeouts),
    BACKPORT_JUMP(GetComPlusPackageInstallStatus),
    BACKPORT_JUMP(GetCompressedFileSizeA),
    BACKPORT_JUMP(GetCompressedFileSizeW),
    BACKPORT_JUMP(GetComputerNameA),
    BACKPORT_JUMP(GetComputerNameExA),
    BACKPORT_JUMP(GetComputerNameExW),
    BACKPORT_JUMP(GetComputerNameW),
    BACKPORT_JUMP(GetConsoleAliasA),
    BACKPORT_JUMP(GetConsoleAliasesA),
    BACKPORT_JUMP(GetConsoleAliasesLengthA),
    BACKPORT_JUMP(GetConsoleAliasesLengthW),
    BACKPORT_JUMP(GetConsoleAliasesW),
    BACKPORT_JUMP(GetConsoleAliasExesA),
    BACKPORT_JUMP(GetConsoleAliasExesLengthA),
    BACKPORT_JUMP(GetConsoleAliasExesLengthW),
    BACKPORT_JUMP(GetConsoleAliasExesW),
    BACKPORT_JUMP(GetConsoleAliasW),
    BACKPORT_JUMP(GetConsoleCharType),
    BACKPORT_JUMP(GetConsoleCommandHistoryA),
    BACKPORT_JUMP(GetConsoleCommandHistoryLengthA),
    BACKPORT_JUMP(GetConsoleCommandHistoryLengthW),
    BACKPORT_JUMP(GetConsoleCommandHistoryW),
    BACKPORT_JUMP(GetConsoleCP),
    BACKPORT_JUMP(GetConsoleCursorInfo),
    BACKPORT_JUMP(GetConsoleCursorMode),
    BACKPORT_JUMP(GetConsoleDisplayMode),
    BACKPORT_JUMP(GetConsoleFontInfo),
    BACKPORT_JUMP(GetConsoleFontSize),
    BACKPORT_JUMP(GetConsoleHardwareState),
    BACKPORT_JUMP(GetConsoleInputExeNameA),
    BACKPORT_JUMP(GetConsoleInputExeNameW),
    BACKPORT_JUMP(GetConsoleInputWaitHandle),
    BACKPORT_JUMP(GetConsoleKeyboardLayoutNameA),
    BACKPORT_JUMP(GetConsoleKeyboardLayoutNameW),
    BACKPORT_JUMP(GetConsoleMode),
    BACKPORT_JUMP(GetConsoleNlsMode),
    BACKPORT_JUMP(GetConsoleOutputCP),
    BACKPORT_JUMP(GetConsoleProcessList),
    BACKPORT_JUMP(GetConsoleScreenBufferInfo),
    BACKPORT_JUMP(GetConsoleSelectionInfo),
    BACKPORT_JUMP(GetConsoleTitleA),
    BACKPORT_JUMP(GetConsoleTitleW),
    BACKPORT_JUMP(GetConsoleWindow),
    BACKPORT_JUMP(GetCPInfo),
    BACKPORT_JUMP(GetCPInfoExA),
    BACKPORT_JUMP(GetCPInfoExW),
    BACKPORT_JUMP(GetCurrencyFormatA),
    BACKPORT_JUMP(GetCurrencyFormatW),
    BACKPORT_JUMP(GetCurrentActCtx),
    BACKPORT_JUMP(GetCurrentConsoleFont),
    BACKPORT_JUMP(GetCurrentDirectoryA),
    BACKPORT_JUMP(GetCurrentDirectoryW),
    BACKPORT_JUMP(GetCurrentProcess),
    BACKPORT_JUMP(GetCurrentProcessId),
    BACKPORT_JUMP(GetCurrentThread),
    BACKPORT_JUMP(GetCurrentThreadId),
    BACKPORT_JUMP(GetDateFormatA),
    BACKPORT_JUMP(GetDateFormatW),
    BACKPORT_JUMP(GetDefaultCommConfigA),
    BACKPORT_JUMP(GetDefaultCommConfigW),
    BACKPORT_JUMP(GetDevicePowerState),
    BACKPORT_JUMP(GetDiskFreeSpaceA),
    BACKPORT_JUMP(GetDiskFreeSpaceExA),
    BACKPORT_JUMP(GetDiskFreeSpaceExW),
    BACKPORT_JUMP(GetDiskFreeSpaceW),
    BACKPORT_JUMP(GetDllDirectoryA),
    BACKPORT_JUMP(GetDllDirectoryW),
    BACKPORT_JUMP(GetDriveTypeA),
    BACKPORT_JUMP(GetDriveTypeW),
    BACKPORT_JUMP(GetEnvironmentStrings),
    BACKPORT_JUMP(GetEnvironmentStringsA),
    BACKPORT_JUMP(GetEnvironmentStringsW),
    BACKPORT_JUMP(GetEnvironmentVariableA),
    BACKPORT_JUMP(GetEnvironmentVariableW),
    BACKPORT_JUMP(GetExitCodeProcess),
    BACKPORT_JUMP(GetExitCodeThread),
    BACKPORT_JUMP(GetExpandedNameA),
    BACKPORT_JUMP(GetExpandedNameW),
    BACKPORT_JUMP(GetFileAttributesA),
    BACKPORT_JUMP(GetFileAttributesExA),
    BACKPORT_JUMP(GetFileAttributesExW),
    BACKPORT_JUMP(GetFileAttributesW),
    BACKPORT_JUMP(GetFileInformationByHandle),
    BACKPORT_JUMP(GetFileSize),
    BACKPORT_JUMP(GetFileSizeEx),
    BACKPORT_JUMP(GetFileTime),
    BACKPORT_JUMP(GetFileType),
    BACKPORT_JUMP(GetFirmwareEnvironmentVariableA),
    BACKPORT_JUMP(GetFirmwareEnvironmentVariableW),
    BACKPORT_JUMP(GetFullPathNameA),
    BACKPORT_JUMP(GetFullPathNameW),
    BACKPORT_JUMP(GetGeoInfoA),
    BACKPORT_JUMP(GetGeoInfoW),
    BACKPORT_JUMP(GetHandleContext),
    BACKPORT_JUMP(GetHandleInformation),
    BACKPORT_JUMP(GetLargestConsoleWindowSize),
    BACKPORT_JUMP(GetLastError),
    BACKPORT_JUMP(GetLocaleInfoA),
    BACKPORT_JUMP(GetLocaleInfoW),
    BACKPORT_JUMP(GetLocalTime),
    BACKPORT_JUMP(GetLogicalDrives),
    BACKPORT_JUMP(GetLogicalDriveStringsA),
    BACKPORT_JUMP(GetLogicalDriveStringsW),
    BACKPORT_JUMP(GetLogicalProcessorInformation),
    BACKPORT_JUMP(GetLongPathNameA),
    BACKPORT_JUMP(GetLongPathNameW),
    BACKPORT_JUMP(GetMailslotInfo),
    BACKPORT_JUMP(GetModuleFileNameA),
    BACKPORT_JUMP(GetModuleFileNameW),
    BACKPORT_JUMP(GetModuleHandleA),
    BACKPORT_JUMP(GetModuleHandleExA),
    BACKPORT_JUMP(GetModuleHandleExW),
    BACKPORT_JUMP(GetModuleHandleW),
    BACKPORT_JUMP(GetNamedPipeHandleStateA),
    BACKPORT_JUMP(GetNamedPipeHandleStateW),
    BACKPORT_JUMP(GetNamedPipeInfo),
    BACKPORT_JUMP(GetNativeSystemInfo),
    BACKPORT_JUMP(GetNextVDMCommand),
    BACKPORT_JUMP(GetNumaAvailableMemoryNode),
    BACKPORT_JUMP(GetNumaHighestNodeNumber),
    BACKPORT_JUMP(GetNumaNodeProcessorMask),
    BACKPORT_JUMP(GetNumaProcessorNode),
    BACKPORT_JUMP(GetNumberFormatA),
    BACKPORT_JUMP(GetNumberFormatW),
    BACKPORT_JUMP(GetNumberOfConsoleFonts),
    BACKPORT_JUMP(GetNumberOfConsoleInputEvents),
    BACKPORT_JUMP(GetNumberOfConsoleMouseButtons),
    BACKPORT_JUMP(GetOEMCP),
    BACKPORT_JUMP(GetOverlappedResult),
    BACKPORT_JUMP(GetPriorityClass),
    BACKPORT_JUMP(GetPrivateProfileIntA),
    BACKPORT_JUMP(GetPrivateProfileIntW),
    BACKPORT_JUMP(GetPrivateProfileSectionA),
    BACKPORT_JUMP(GetPrivateProfileSectionNamesA),
    BACKPORT_JUMP(GetPrivateProfileSectionNamesW),
    BACKPORT_JUMP(GetPrivateProfileSectionW),
    BACKPORT_JUMP(GetPrivateProfileStringA),
    BACKPORT_JUMP(GetPrivateProfileStringW),
    BACKPORT_JUMP(GetPrivateProfileStructA),
    BACKPORT_JUMP(GetPrivateProfileStructW),
    BACKPORT_JUMP(GetProcAddress),
    BACKPORT_JUMP(GetProcessAffinityMask),
    BACKPORT_JUMP(GetProcessDEPPolicy),
    BACKPORT_JUMP(GetProcessHandleCount),
    BACKPORT_JUMP(GetProcessHeap),
    BACKPORT_JUMP(GetProcessHeaps),
    BACKPORT_JUMP(GetProcessId),
    BACKPORT_JUMP(GetProcessIoCounters),
    BACKPORT_JUMP(GetProcessPriorityBoost),
    BACKPORT_JUMP(GetProcessShutdownParameters),
    BACKPORT_JUMP(GetProcessTimes),
    BACKPORT_JUMP(GetProcessVersion),
    BACKPORT_JUMP(GetProcessWorkingSetSize),
    BACKPORT_JUMP(GetProfileIntA),
    BACKPORT_JUMP(GetProfileIntW),
    BACKPORT_JUMP(GetProfileSectionA),
    BACKPORT_JUMP(GetProfileSectionW),
    BACKPORT_JUMP(GetProfileStringA),
    BACKPORT_JUMP(GetProfileStringW),
    BACKPORT_JUMP(GetQueuedCompletionStatus),
    BACKPORT_JUMP(GetShortPathNameA),
    BACKPORT_JUMP(GetShortPathNameW),
    BACKPORT_JUMP(GetStartupInfoA),
    BACKPORT_JUMP(GetStartupInfoW),
    BACKPORT_JUMP(GetStdHandle),
    BACKPORT_JUMP(GetStringTypeA),
    BACKPORT_JUMP(GetStringTypeExA),
    BACKPORT_JUMP(GetStringTypeExW),
    BACKPORT_JUMP(GetStringTypeW),
    BACKPORT_JUMP(GetSystemDefaultLangID),
    BACKPORT_JUMP(GetSystemDefaultLCID),
    BACKPORT_JUMP(GetSystemDefaultUILanguage),
    BACKPORT_JUMP(GetSystemDEPPolicy),
    BACKPORT_JUMP(GetSystemDirectoryA),
    BACKPORT_JUMP(GetSystemDirectoryW),
    BACKPORT_JUMP(GetSystemInfo),
    BACKPORT_JUMP(GetSystemPowerStatus),
    BACKPORT_JUMP(GetSystemRegistryQuota),
    BACKPORT_JUMP(GetSystemTime),
    BACKPORT_JUMP(GetSystemTimeAdjustment),
    BACKPORT_JUMP(GetSystemTimeAsFileTime),
    BACKPORT_JUMP(GetSystemTimes),
    BACKPORT_JUMP(GetSystemWindowsDirectoryA),
    BACKPORT_JUMP(GetSystemWindowsDirectoryW),
    BACKPORT_JUMP(GetSystemWow64DirectoryA),
    BACKPORT_JUMP(GetSystemWow64DirectoryW),
    BACKPORT_JUMP(GetTapeParameters),
    BACKPORT_JUMP(GetTapePosition),
    BACKPORT_JUMP(GetTapeStatus),
    BACKPORT_JUMP(GetTempFileNameA),
    BACKPORT_JUMP(GetTempFileNameW),
    BACKPORT_JUMP(GetTempPathA),
    BACKPORT_JUMP(GetTempPathW),
    BACKPORT_JUMP(GetThreadContext),
    BACKPORT_JUMP(GetThreadIOPendingFlag),
    BACKPORT_JUMP(GetThreadLocale),
    BACKPORT_JUMP(GetThreadPriority),
    BACKPORT_JUMP(GetThreadPriorityBoost),
    BACKPORT_JUMP(GetThreadSelectorEntry),
    BACKPORT_JUMP(GetThreadTimes),
    BACKPORT_JUMP(GetTickCount),
    BACKPORT_RPLC(GetTickCount64),
    BACKPORT_JUMP(GetTimeFormatA),
    BACKPORT_JUMP(GetTimeFormatW),
    BACKPORT_JUMP(GetTimeZoneInformation),
    BACKPORT_JUMP(GetUserDefaultLangID),
    BACKPORT_JUMP(GetUserDefaultLCID),
    BACKPORT_JUMP(GetUserDefaultUILanguage),
    BACKPORT_JUMP(GetUserGeoID),
    BACKPORT_JUMP(GetVDMCurrentDirectories),
    BACKPORT_JUMP(GetVersion),
    BACKPORT_JUMP(GetVersionExA),
    BACKPORT_JUMP(GetVersionExW),
    BACKPORT_JUMP(GetVolumeInformationA),
    BACKPORT_JUMP(GetVolumeInformationW),
    BACKPORT_JUMP(GetVolumeNameForVolumeMountPointA),
    BACKPORT_JUMP(GetVolumeNameForVolumeMountPointW),
    BACKPORT_JUMP(GetVolumePathNameA),
    BACKPORT_JUMP(GetVolumePathNamesForVolumeNameA),
    BACKPORT_JUMP(GetVolumePathNamesForVolumeNameW),
    BACKPORT_JUMP(GetVolumePathNameW),
    BACKPORT_JUMP(GetWindowsDirectoryA),
    BACKPORT_JUMP(GetWindowsDirectoryW),
    BACKPORT_JUMP(GetWriteWatch),
    BACKPORT_JUMP(GlobalAddAtomA),
    BACKPORT_JUMP(GlobalAddAtomW),
    BACKPORT_JUMP(GlobalAlloc),
    BACKPORT_JUMP(GlobalCompact),
    BACKPORT_JUMP(GlobalDeleteAtom),
    BACKPORT_JUMP(GlobalFindAtomA),
    BACKPORT_JUMP(GlobalFindAtomW),
    BACKPORT_JUMP(GlobalFix),
    BACKPORT_JUMP(GlobalFlags),
    BACKPORT_JUMP(GlobalFree),
    BACKPORT_JUMP(GlobalGetAtomNameA),
    BACKPORT_JUMP(GlobalGetAtomNameW),
    BACKPORT_JUMP(GlobalHandle),
    BACKPORT_JUMP(GlobalLock),
    BACKPORT_JUMP(GlobalMemoryStatus),
    BACKPORT_JUMP(GlobalMemoryStatusEx),
    BACKPORT_JUMP(GlobalReAlloc),
    BACKPORT_JUMP(GlobalSize),
    BACKPORT_JUMP(GlobalUnfix),
    BACKPORT_JUMP(GlobalUnlock),
    BACKPORT_JUMP(GlobalUnWire),
    BACKPORT_JUMP(GlobalWire),
    BACKPORT_JUMP(Heap32First),
    BACKPORT_JUMP(Heap32ListFirst),
    BACKPORT_JUMP(Heap32ListNext),
    BACKPORT_JUMP(Heap32Next),
    BACKPORT_JUMP(HeapAlloc),
    BACKPORT_JUMP(HeapCompact),
    BACKPORT_JUMP(HeapCreate),
    BACKPORT_JUMP(HeapDestroy),
    BACKPORT_JUMP(HeapFree),
    BACKPORT_JUMP(HeapLock),
    BACKPORT_JUMP(HeapQueryInformation),
    BACKPORT_JUMP(HeapReAlloc),
    BACKPORT_JUMP(HeapSetInformation),
    BACKPORT_JUMP(HeapSize),
    BACKPORT_JUMP(HeapSummary),
    BACKPORT_JUMP(HeapUnlock),
    BACKPORT_JUMP(HeapValidate),
    BACKPORT_JUMP(HeapWalk),
    BACKPORT_JUMP(InitAtomTable),
    BACKPORT_JUMP(InitializeCriticalSection),
    BACKPORT_JUMP(InitializeCriticalSectionAndSpinCount),
    BACKPORT_JUMP(InitializeSListHead),
    BACKPORT_RPLC(InitOnceExecuteOnce),
    BACKPORT_JUMP(InterlockedCompareExchange),
    BACKPORT_JUMP(InterlockedDecrement),
    BACKPORT_JUMP(InterlockedExchange),
    BACKPORT_JUMP(InterlockedExchangeAdd),
    BACKPORT_JUMP(InterlockedFlushSList),
    BACKPORT_JUMP(InterlockedIncrement),
    BACKPORT_JUMP(InterlockedPopEntrySList),
    BACKPORT_JUMP(InterlockedPushEntrySList),
    BACKPORT_JUMP(InvalidateConsoleDIBits),
    BACKPORT_JUMP(IsBadCodePtr),
    BACKPORT_JUMP(IsBadHugeReadPtr),
    BACKPORT_JUMP(IsBadHugeWritePtr),
    BACKPORT_JUMP(IsBadReadPtr),
    BACKPORT_JUMP(IsBadStringPtrA),
    BACKPORT_JUMP(IsBadStringPtrW),
    BACKPORT_JUMP(IsBadWritePtr),
    BACKPORT_JUMP(IsDBCSLeadByte),
    BACKPORT_JUMP(IsDBCSLeadByteEx),
    BACKPORT_JUMP(IsDebuggerPresent),
    BACKPORT_JUMP(IsProcessInJob),
    BACKPORT_JUMP(IsProcessorFeaturePresent),
    BACKPORT_JUMP(IsSystemResumeAutomatic),
    BACKPORT_JUMP(IsValidCodePage),
    BACKPORT_JUMP(IsValidLanguageGroup),
    BACKPORT_JUMP(IsValidLocale),
    BACKPORT_JUMP(IsWow64Process),
    BACKPORT_JUMP(LCMapStringA),
    BACKPORT_RPLC(LCMapStringEx),
    BACKPORT_JUMP(LCMapStringW),
    BACKPORT_JUMP(LeaveCriticalSection),
    BACKPORT_JUMP(LoadLibraryA),
    BACKPORT_JUMP(LoadLibraryExA),
    BACKPORT_JUMP(LoadLibraryExW),
    BACKPORT_JUMP(LoadLibraryW),
    BACKPORT_JUMP(LoadModule),
    BACKPORT_JUMP(LoadResource),
    BACKPORT_JUMP(LocalAlloc),
    BACKPORT_JUMP(LocalCompact),
    BACKPORT_JUMP(LocalFileTimeToFileTime),
    BACKPORT_JUMP(LocalFlags),
    BACKPORT_JUMP(LocalFree),
    BACKPORT_JUMP(LocalHandle),
    BACKPORT_JUMP(LocalLock),
    BACKPORT_JUMP(LocalReAlloc),
    BACKPORT_JUMP(LocalShrink),
    BACKPORT_JUMP(LocalSize),
    BACKPORT_JUMP(LocalUnlock),
    BACKPORT_JUMP(LockFile),
    BACKPORT_JUMP(LockFileEx),
    BACKPORT_JUMP(LockResource),
    BACKPORT_JUMP(lstrcat),
    BACKPORT_JUMP(lstrcatA),
    BACKPORT_JUMP(lstrcatW),
    BACKPORT_JUMP(lstrcmp),
    BACKPORT_JUMP(lstrcmpA),
    BACKPORT_JUMP(lstrcmpi),
    BACKPORT_JUMP(lstrcmpiA),
    BACKPORT_JUMP(lstrcmpiW),
    BACKPORT_JUMP(lstrcmpW),
    BACKPORT_JUMP(lstrcpy),
    BACKPORT_JUMP(lstrcpyA),
    BACKPORT_JUMP(lstrcpyn),
    BACKPORT_JUMP(lstrcpynA),
    BACKPORT_JUMP(lstrcpynW),
    BACKPORT_JUMP(lstrcpyW),
    BACKPORT_JUMP(lstrlen),
    BACKPORT_JUMP(lstrlenA),
    BACKPORT_JUMP(lstrlenW),
    BACKPORT_JUMP(LZClose),
    BACKPORT_JUMP(LZCloseFile),
    BACKPORT_JUMP(LZCopy),
    BACKPORT_JUMP(LZCreateFileW),
    BACKPORT_JUMP(LZDone),
    BACKPORT_JUMP(LZInit),
    BACKPORT_JUMP(LZOpenFileA),
    BACKPORT_JUMP(LZOpenFileW),
    BACKPORT_JUMP(LZRead),
    BACKPORT_JUMP(LZSeek),
    BACKPORT_JUMP(LZStart),
    BACKPORT_JUMP(MapUserPhysicalPages),
    BACKPORT_JUMP(MapUserPhysicalPagesScatter),
    BACKPORT_JUMP(MapViewOfFile),
    BACKPORT_JUMP(MapViewOfFileEx),
    BACKPORT_JUMP(Module32First),
    BACKPORT_JUMP(Module32FirstW),
    BACKPORT_JUMP(Module32Next),
    BACKPORT_JUMP(Module32NextW),
    BACKPORT_JUMP(MoveFileA),
    BACKPORT_JUMP(MoveFileExA),
    BACKPORT_JUMP(MoveFileExW),
    BACKPORT_JUMP(MoveFileW),
    BACKPORT_JUMP(MoveFileWithProgressA),
    BACKPORT_JUMP(MoveFileWithProgressW),
    BACKPORT_JUMP(MulDiv),
    BACKPORT_JUMP(MultiByteToWideChar),
    BACKPORT_JUMP(NlsGetCacheUpdateCount),
    BACKPORT_JUMP(OpenConsoleW),
    BACKPORT_JUMP(OpenEventA),
    BACKPORT_JUMP(OpenEventW),
    BACKPORT_JUMP(OpenFile),
    BACKPORT_JUMP(OpenFileMappingA),
    BACKPORT_JUMP(OpenFileMappingW),
    BACKPORT_JUMP(OpenJobObjectA),
    BACKPORT_JUMP(OpenJobObjectW),
    BACKPORT_JUMP(OpenMutexA),
    BACKPORT_JUMP(OpenMutexW),
    BACKPORT_JUMP(OpenProcess),
    BACKPORT_JUMP(OpenProfileUserMapping),
    BACKPORT_JUMP(OpenSemaphoreA),
    BACKPORT_JUMP(OpenSemaphoreW),
    BACKPORT_JUMP(OpenThread),
    BACKPORT_JUMP(OpenWaitableTimerA),
    BACKPORT_JUMP(OpenWaitableTimerW),
    BACKPORT_JUMP(OutputDebugStringA),
    BACKPORT_JUMP(OutputDebugStringW),
    BACKPORT_JUMP(PeekConsoleInputA),
    BACKPORT_JUMP(PeekConsoleInputW),
    BACKPORT_JUMP(PeekNamedPipe),
    BACKPORT_JUMP(PostQueuedCompletionStatus),
    BACKPORT_JUMP(PrepareTape),
    BACKPORT_JUMP(PrivCopyFileExW),
    BACKPORT_JUMP(PrivMoveFileIdentityW),
    BACKPORT_JUMP(Process32First),
    BACKPORT_JUMP(Process32FirstW),
    BACKPORT_JUMP(Process32Next),
    BACKPORT_JUMP(Process32NextW),
    BACKPORT_JUMP(ProcessIdToSessionId),
    BACKPORT_JUMP(PulseEvent),
    BACKPORT_JUMP(PurgeComm),
    BACKPORT_JUMP(QueryActCtxW),
    BACKPORT_JUMP(QueryDepthSList),
    BACKPORT_JUMP(QueryDosDeviceA),
    BACKPORT_JUMP(QueryDosDeviceW),
    BACKPORT_JUMP(QueryInformationJobObject),
    BACKPORT_JUMP(QueryMemoryResourceNotification),
    BACKPORT_JUMP(QueryPerformanceCounter),
    BACKPORT_JUMP(QueryPerformanceFrequency),
    BACKPORT_JUMP(QueueUserAPC),
    BACKPORT_JUMP(QueueUserWorkItem),
    BACKPORT_JUMP(RaiseException),
    BACKPORT_JUMP(ReadConsoleA),
    BACKPORT_JUMP(ReadConsoleInputA),
    BACKPORT_JUMP(ReadConsoleInputExA),
    BACKPORT_JUMP(ReadConsoleInputExW),
    BACKPORT_JUMP(ReadConsoleInputW),
    BACKPORT_JUMP(ReadConsoleOutputA),
    BACKPORT_JUMP(ReadConsoleOutputAttribute),
    BACKPORT_JUMP(ReadConsoleOutputCharacterA),
    BACKPORT_JUMP(ReadConsoleOutputCharacterW),
    BACKPORT_JUMP(ReadConsoleOutputW),
    BACKPORT_JUMP(ReadConsoleW),
    BACKPORT_JUMP(ReadDirectoryChangesW),
    BACKPORT_JUMP(ReadFile),
    BACKPORT_JUMP(ReadFileEx),
    BACKPORT_JUMP(ReadFileScatter),
    BACKPORT_JUMP(ReadProcessMemory),
    BACKPORT_JUMP(RegisterConsoleIME),
    BACKPORT_JUMP(RegisterConsoleOS2),
    BACKPORT_JUMP(RegisterConsoleVDM),
    BACKPORT_JUMP(RegisterWaitForInputIdle),
    BACKPORT_JUMP(RegisterWaitForSingleObject),
    BACKPORT_JUMP(RegisterWaitForSingleObjectEx),
    BACKPORT_JUMP(RegisterWowBaseHandlers),
    BACKPORT_JUMP(RegisterWowExec),
    BACKPORT_JUMP(ReleaseActCtx),
    BACKPORT_JUMP(ReleaseMutex),
    BACKPORT_JUMP(ReleaseSemaphore),
    BACKPORT_JUMP(RemoveDirectoryA),
    BACKPORT_JUMP(RemoveDirectoryW),
    BACKPORT_JUMP(RemoveLocalAlternateComputerNameA),
    BACKPORT_JUMP(RemoveLocalAlternateComputerNameW),
    BACKPORT_JUMP(RemoveVectoredExceptionHandler),
    BACKPORT_JUMP(ReplaceFile),
    BACKPORT_JUMP(ReplaceFileA),
    BACKPORT_JUMP(ReplaceFileW),
    BACKPORT_JUMP(RequestDeviceWakeup),
    BACKPORT_JUMP(RequestWakeupLatency),
    BACKPORT_JUMP(ResetEvent),
    BACKPORT_JUMP(ResetWriteWatch),
    BACKPORT_JUMP(RestoreLastError),
    BACKPORT_JUMP(ResumeThread),
    BACKPORT_JUMP(RtlCaptureContext),
    BACKPORT_JUMP(RtlCaptureStackBackTrace),
    BACKPORT_JUMP(RtlFillMemory),
    BACKPORT_JUMP(RtlMoveMemory),
    BACKPORT_JUMP(RtlUnwind),
    BACKPORT_JUMP(RtlZeroMemory),
    BACKPORT_JUMP(ScrollConsoleScreenBufferA),
    BACKPORT_JUMP(ScrollConsoleScreenBufferW),
    BACKPORT_JUMP(SearchPathA),
    BACKPORT_JUMP(SearchPathW),
    BACKPORT_JUMP(SetCalendarInfoA),
    BACKPORT_JUMP(SetCalendarInfoW),
    BACKPORT_JUMP(SetClientTimeZoneInformation),
    BACKPORT_JUMP(SetCommBreak),
    BACKPORT_JUMP(SetCommConfig),
    BACKPORT_JUMP(SetCommMask),
    BACKPORT_JUMP(SetCommState),
    BACKPORT_JUMP(SetCommTimeouts),
    BACKPORT_JUMP(SetComPlusPackageInstallStatus),
    BACKPORT_JUMP(SetComputerNameA),
    BACKPORT_JUMP(SetComputerNameExA),
    BACKPORT_JUMP(SetComputerNameExW),
    BACKPORT_JUMP(SetComputerNameW),
    BACKPORT_JUMP(SetConsoleActiveScreenBuffer),
    BACKPORT_JUMP(SetConsoleCP),
    BACKPORT_JUMP(SetConsoleCtrlHandler),
    BACKPORT_JUMP(SetConsoleCursor),
    BACKPORT_JUMP(SetConsoleCursorInfo),
    BACKPORT_JUMP(SetConsoleCursorMode),
    BACKPORT_JUMP(SetConsoleCursorPosition),
    BACKPORT_JUMP(SetConsoleDisplayMode),
    BACKPORT_JUMP(SetConsoleFont),
    BACKPORT_JUMP(SetConsoleHardwareState),
    BACKPORT_JUMP(SetConsoleIcon),
    BACKPORT_JUMP(SetConsoleInputExeNameA),
    BACKPORT_JUMP(SetConsoleInputExeNameW),
    BACKPORT_JUMP(SetConsoleKeyShortcuts),
    BACKPORT_JUMP(SetConsoleLocalEUDC),
    BACKPORT_JUMP(SetConsoleMaximumWindowSize),
    BACKPORT_JUMP(SetConsoleMenuClose),
    BACKPORT_JUMP(SetConsoleMode),
    BACKPORT_JUMP(SetConsoleNlsMode),
    BACKPORT_JUMP(SetConsoleNumberOfCommandsA),
    BACKPORT_JUMP(SetConsoleNumberOfCommandsW),
    BACKPORT_JUMP(SetConsoleOS2OemFormat),
    BACKPORT_JUMP(SetConsoleOutputCP),
    BACKPORT_JUMP(SetConsolePalette),
    BACKPORT_JUMP(SetConsoleScreenBufferSize),
    BACKPORT_JUMP(SetConsoleTextAttribute),
    BACKPORT_JUMP(SetConsoleTitleA),
    BACKPORT_JUMP(SetConsoleTitleW),
    BACKPORT_JUMP(SetConsoleWindowInfo),
    BACKPORT_JUMP(SetCriticalSectionSpinCount),
    BACKPORT_JUMP(SetCurrentDirectoryA),
    BACKPORT_JUMP(SetCurrentDirectoryW),
    BACKPORT_JUMP(SetDefaultCommConfigA),
    BACKPORT_JUMP(SetDefaultCommConfigW),
    BACKPORT_JUMP(SetDllDirectoryA),
    BACKPORT_JUMP(SetDllDirectoryW),
    BACKPORT_JUMP(SetEndOfFile),
    BACKPORT_JUMP(SetEnvironmentVariableA),
    BACKPORT_JUMP(SetEnvironmentVariableW),
    BACKPORT_JUMP(SetErrorMode),
    BACKPORT_JUMP(SetEvent),
    BACKPORT_JUMP(SetFileApisToANSI),
    BACKPORT_JUMP(SetFileApisToOEM),
    BACKPORT_JUMP(SetFileAttributesA),
    BACKPORT_JUMP(SetFileAttributesW),
    BACKPORT_JUMP(SetFilePointer),
    BACKPORT_JUMP(SetFilePointerEx),
    BACKPORT_JUMP(SetFileShortNameA),
    BACKPORT_JUMP(SetFileShortNameW),
    BACKPORT_JUMP(SetFileTime),
    BACKPORT_JUMP(SetFileValidData),
    BACKPORT_JUMP(SetFirmwareEnvironmentVariableA),
    BACKPORT_JUMP(SetFirmwareEnvironmentVariableW),
    BACKPORT_JUMP(SetHandleContext),
    BACKPORT_JUMP(SetHandleCount),
    BACKPORT_JUMP(SetHandleInformation),
    BACKPORT_JUMP(SetInformationJobObject),
    BACKPORT_JUMP(SetLastConsoleEventActive),
    BACKPORT_JUMP(SetLastError),
    BACKPORT_JUMP(SetLocaleInfoA),
    BACKPORT_JUMP(SetLocaleInfoW),
    BACKPORT_JUMP(SetLocalPrimaryComputerNameA),
    BACKPORT_JUMP(SetLocalPrimaryComputerNameW),
    BACKPORT_JUMP(SetLocalTime),
    BACKPORT_JUMP(SetMailslotInfo),
    BACKPORT_JUMP(SetMessageWaitingIndicator),
    BACKPORT_JUMP(SetNamedPipeHandleState),
    BACKPORT_JUMP(SetPriorityClass),
    BACKPORT_JUMP(SetProcessAffinityMask),
    BACKPORT_JUMP(SetProcessDEPPolicy),
    BACKPORT_JUMP(SetProcessPriorityBoost),
    BACKPORT_JUMP(SetProcessShutdownParameters),
    BACKPORT_JUMP(SetProcessWorkingSetSize),
    BACKPORT_JUMP(SetSearchPathMode),
    BACKPORT_JUMP(SetStdHandle),
    BACKPORT_JUMP(SetSystemPowerState),
    BACKPORT_JUMP(SetSystemTime),
    BACKPORT_JUMP(SetSystemTimeAdjustment),
    BACKPORT_JUMP(SetTapeParameters),
    BACKPORT_JUMP(SetTapePosition),
    BACKPORT_JUMP(SetTermsrvAppInstallMode),
    BACKPORT_JUMP(SetThreadAffinityMask),
    BACKPORT_JUMP(SetThreadContext),
    BACKPORT_JUMP(SetThreadExecutionState),
    BACKPORT_JUMP(SetThreadIdealProcessor),
    BACKPORT_JUMP(SetThreadLocale),
    BACKPORT_JUMP(SetThreadPriority),
    BACKPORT_JUMP(SetThreadPriorityBoost),
    BACKPORT_JUMP(SetThreadUILanguage),
    BACKPORT_JUMP(SetTimerQueueTimer),
    BACKPORT_JUMP(SetTimeZoneInformation),
    BACKPORT_JUMP(SetUnhandledExceptionFilter),
    BACKPORT_JUMP(SetupComm),
    BACKPORT_JUMP(SetUserGeoID),
    BACKPORT_JUMP(SetVDMCurrentDirectories),
    BACKPORT_JUMP(SetVolumeLabelA),
    BACKPORT_JUMP(SetVolumeLabelW),
    BACKPORT_JUMP(SetVolumeMountPointA),
    BACKPORT_JUMP(SetVolumeMountPointW),
    BACKPORT_JUMP(SetWaitableTimer),
    BACKPORT_JUMP(ShowConsoleCursor),
    BACKPORT_JUMP(SignalObjectAndWait),
    BACKPORT_JUMP(SizeofResource),
    BACKPORT_JUMP(Sleep),
    BACKPORT_JUMP(SleepEx),
    BACKPORT_JUMP(SuspendThread),
    BACKPORT_JUMP(SwitchToFiber),
    BACKPORT_JUMP(SwitchToThread),
    BACKPORT_JUMP(SystemTimeToFileTime),
    BACKPORT_JUMP(SystemTimeToTzSpecificLocalTime),
    BACKPORT_JUMP(TerminateJobObject),
    BACKPORT_JUMP(TerminateProcess),
    BACKPORT_JUMP(TerminateThread),
    BACKPORT_JUMP(TermsrvAppInstallMode),
    BACKPORT_JUMP(Thread32First),
    BACKPORT_JUMP(Thread32Next),
    BACKPORT_JUMP(TlsAlloc),
    BACKPORT_JUMP(TlsFree),
    BACKPORT_JUMP(TlsGetValue),
    BACKPORT_JUMP(TlsSetValue),
    BACKPORT_JUMP(Toolhelp32ReadProcessMemory),
    BACKPORT_JUMP(TransactNamedPipe),
    BACKPORT_JUMP(TransmitCommChar),
    BACKPORT_JUMP(TryEnterCriticalSection),
    BACKPORT_JUMP(TzSpecificLocalTimeToSystemTime),
    BACKPORT_JUMP(UnhandledExceptionFilter),
    BACKPORT_JUMP(UnlockFile),
    BACKPORT_JUMP(UnlockFileEx),
    BACKPORT_JUMP(UnmapViewOfFile),
    BACKPORT_JUMP(UnregisterConsoleIME),
    BACKPORT_JUMP(UnregisterWait),
    BACKPORT_JUMP(UnregisterWaitEx),
    BACKPORT_JUMP(UpdateResourceA),
    BACKPORT_JUMP(UpdateResourceW),
    BACKPORT_JUMP(UTRegister),
    BACKPORT_JUMP(UTUnRegister),
    BACKPORT_JUMP(VDMConsoleOperation),
    BACKPORT_JUMP(VDMOperationStarted),
    BACKPORT_JUMP(VerifyConsoleIoHandle),
    BACKPORT_JUMP(VerifyVersionInfoA),
    BACKPORT_JUMP(VerifyVersionInfoW),
    BACKPORT_JUMP(VerLanguageNameA),
    BACKPORT_JUMP(VerLanguageNameW),
    BACKPORT_JUMP(VerSetConditionMask),
    BACKPORT_JUMP(VirtualAlloc),
    BACKPORT_JUMP(VirtualAllocEx),
    BACKPORT_JUMP(VirtualFree),
    BACKPORT_JUMP(VirtualFreeEx),
    BACKPORT_JUMP(VirtualLock),
    BACKPORT_JUMP(VirtualProtect),
    BACKPORT_JUMP(VirtualProtectEx),
    BACKPORT_JUMP(VirtualQuery),
    BACKPORT_JUMP(VirtualQueryEx),
    BACKPORT_JUMP(VirtualUnlock),
    BACKPORT_JUMP(WaitCommEvent),
    BACKPORT_JUMP(WaitForDebugEvent),
    BACKPORT_JUMP(WaitForMultipleObjects),
    BACKPORT_JUMP(WaitForMultipleObjectsEx),
    BACKPORT_JUMP(WaitForSingleObject),
    BACKPORT_JUMP(WaitForSingleObjectEx),
    BACKPORT_JUMP(WaitNamedPipeA),
    BACKPORT_JUMP(WaitNamedPipeW),
    BACKPORT_JUMP(WideCharToMultiByte),
    BACKPORT_JUMP(WinExec),
    BACKPORT_JUMP(WriteConsoleA),
    BACKPORT_JUMP(WriteConsoleInputA),
    BACKPORT_JUMP(WriteConsoleInputVDMA),
    BACKPORT_JUMP(WriteConsoleInputVDMW),
    BACKPORT_JUMP(WriteConsoleInputW),
    BACKPORT_JUMP(WriteConsoleOutputA),
    BACKPORT_JUMP(WriteConsoleOutputAttribute),
    BACKPORT_JUMP(WriteConsoleOutputCharacterA),
    BACKPORT_JUMP(WriteConsoleOutputCharacterW),
    BACKPORT_JUMP(WriteConsoleOutputW),
    BACKPORT_JUMP(WriteConsoleW),
    BACKPORT_JUMP(WriteFile),
    BACKPORT_JUMP(WriteFileEx),
    BACKPORT_JUMP(WriteFileGather),
    BACKPORT_JUMP(WritePrivateProfileSectionA),
    BACKPORT_JUMP(WritePrivateProfileSectionW),
    BACKPORT_JUMP(WritePrivateProfileStringA),
    BACKPORT_JUMP(WritePrivateProfileStringW),
    BACKPORT_JUMP(WritePrivateProfileStructA),
    BACKPORT_JUMP(WritePrivateProfileStructW),
    BACKPORT_JUMP(WriteProcessMemory),
    BACKPORT_JUMP(WriteProfileSectionA),
    BACKPORT_JUMP(WriteProfileSectionW),
    BACKPORT_JUMP(WriteProfileStringA),
    BACKPORT_JUMP(WriteProfileStringW),
    BACKPORT_JUMP(WriteTapemark),
    BACKPORT_JUMP(WTSGetActiveConsoleSessionId),
    BACKPORT_JUMP(ZombifyActCtx),
    BACKPORT_JUMP(_hread),
    BACKPORT_JUMP(_hwrite),
    BACKPORT_JUMP(_lclose),
    BACKPORT_JUMP(_lcreat),
    BACKPORT_JUMP(_llseek),
    BACKPORT_JUMP(_lopen),
    BACKPORT_JUMP(_lread),
    BACKPORT_JUMP(_lwrite),
    BACKPORT_FINISH
};
static sBP_FUNC *s_remap1 = s_remap1_tbl;

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
BACKPORT_FUNC(s_remap1,   0, ActivateActCtx)
BACKPORT_FUNC(s_remap1,   1, AddAtomA)
BACKPORT_FUNC(s_remap1,   2, AddAtomW)
BACKPORT_FUNC(s_remap1,   3, AddConsoleAliasA)
BACKPORT_FUNC(s_remap1,   4, AddConsoleAliasW)
BACKPORT_FUNC(s_remap1,   5, AddLocalAlternateComputerNameA)
BACKPORT_FUNC(s_remap1,   6, AddLocalAlternateComputerNameW)
BACKPORT_FUNC(s_remap1,   7, AddRefActCtx)
BACKPORT_FUNC(s_remap1,   8, AddVectoredExceptionHandler)
BACKPORT_FUNC(s_remap1,   9, AllocateUserPhysicalPages)
BACKPORT_FUNC(s_remap1,  10, AllocConsole)
BACKPORT_FUNC(s_remap1,  11, AreFileApisANSI)
BACKPORT_FUNC(s_remap1,  12, AssignProcessToJobObject)
BACKPORT_FUNC(s_remap1,  13, AttachConsole)
BACKPORT_FUNC(s_remap1,  14, BackupRead)
BACKPORT_FUNC(s_remap1,  15, BackupSeek)
BACKPORT_FUNC(s_remap1,  16, BackupWrite)
BACKPORT_FUNC(s_remap1,  17, BaseCheckAppcompatCache)
BACKPORT_FUNC(s_remap1,  18, BaseCleanupAppcompatCacheSupport)
BACKPORT_FUNC(s_remap1,  19, BaseDumpAppcompatCache)
BACKPORT_FUNC(s_remap1,  20, BaseFlushAppcompatCache)
BACKPORT_FUNC(s_remap1,  21, BaseInitAppcompatCacheSupport)
BACKPORT_FUNC(s_remap1,  22, BasepCheckWinSaferRestrictions)
BACKPORT_FUNC(s_remap1,  23, BaseQueryModuleData)
BACKPORT_FUNC(s_remap1,  24, BaseUpdateAppcompatCache)
BACKPORT_FUNC(s_remap1,  25, Beep)
BACKPORT_FUNC(s_remap1,  26, BeginUpdateResourceA)
BACKPORT_FUNC(s_remap1,  27, BeginUpdateResourceW)
BACKPORT_FUNC(s_remap1,  28, BindIoCompletionCallback)
BACKPORT_FUNC(s_remap1,  29, BuildCommDCBA)
BACKPORT_FUNC(s_remap1,  30, BuildCommDCBAndTimeoutsA)
BACKPORT_FUNC(s_remap1,  31, BuildCommDCBAndTimeoutsW)
BACKPORT_FUNC(s_remap1,  32, BuildCommDCBW)
BACKPORT_FUNC(s_remap1,  33, CallNamedPipeA)
BACKPORT_FUNC(s_remap1,  34, CallNamedPipeW)
BACKPORT_FUNC(s_remap1,  35, CancelDeviceWakeupRequest)
BACKPORT_FUNC(s_remap1,  36, CancelIo)
BACKPORT_FUNC(s_remap1,  37, CancelTimerQueueTimer)
BACKPORT_FUNC(s_remap1,  38, CancelWaitableTimer)
BACKPORT_FUNC(s_remap1,  39, ChangeTimerQueueTimer)
BACKPORT_FUNC(s_remap1,  40, CheckNameLegalDOS8Dot3A)
BACKPORT_FUNC(s_remap1,  41, CheckNameLegalDOS8Dot3W)
BACKPORT_FUNC(s_remap1,  42, CheckRemoteDebuggerPresent)
BACKPORT_FUNC(s_remap1,  43, ClearCommBreak)
BACKPORT_FUNC(s_remap1,  44, ClearCommError)
BACKPORT_FUNC(s_remap1,  45, CloseConsoleHandle)
BACKPORT_FUNC(s_remap1,  46, CloseHandle)
BACKPORT_FUNC(s_remap1,  47, CloseProfileUserMapping)
BACKPORT_FUNC(s_remap1,  48, CmdBatNotification)
BACKPORT_FUNC(s_remap1,  49, CommConfigDialogA)
BACKPORT_FUNC(s_remap1,  50, CommConfigDialogW)
BACKPORT_FUNC(s_remap1,  51, CompareFileTime)
BACKPORT_FUNC(s_remap1,  52, CompareStringA)
BACKPORT_FUNC(s_remap1,  53, CompareStringW)
BACKPORT_FUNC(s_remap1,  54, ConnectNamedPipe)
BACKPORT_FUNC(s_remap1,  55, ConsoleMenuControl)
BACKPORT_FUNC(s_remap1,  56, ContinueDebugEvent)
BACKPORT_FUNC(s_remap1,  57, ConvertDefaultLocale)
BACKPORT_FUNC(s_remap1,  58, ConvertFiberToThread)
BACKPORT_FUNC(s_remap1,  59, ConvertThreadToFiber)
BACKPORT_FUNC(s_remap1,  60, CopyFileA)
BACKPORT_FUNC(s_remap1,  61, CopyFileExA)
BACKPORT_FUNC(s_remap1,  62, CopyFileExW)
BACKPORT_FUNC(s_remap1,  63, CopyFileW)
BACKPORT_FUNC(s_remap1,  64, CopyLZFile)
BACKPORT_FUNC(s_remap1,  65, CreateActCtxA)
BACKPORT_FUNC(s_remap1,  66, CreateActCtxW)
BACKPORT_FUNC(s_remap1,  67, CreateConsoleScreenBuffer)
BACKPORT_FUNC(s_remap1,  68, CreateDirectoryA)
BACKPORT_FUNC(s_remap1,  69, CreateDirectoryExA)
BACKPORT_FUNC(s_remap1,  70, CreateDirectoryExW)
BACKPORT_FUNC(s_remap1,  71, CreateDirectoryW)
BACKPORT_FUNC(s_remap1,  72, CreateEventA)
BACKPORT_FUNC(s_remap1,  73, CreateEventW)
BACKPORT_FUNC(s_remap1,  74, CreateFiber)
BACKPORT_FUNC(s_remap1,  75, CreateFiberEx)
BACKPORT_FUNC(s_remap1,  76, CreateFileA)
BACKPORT_FUNC(s_remap1,  77, CreateFileMappingA)
BACKPORT_FUNC(s_remap1,  78, CreateFileMappingW)
BACKPORT_FUNC(s_remap1,  79, CreateFileW)
BACKPORT_FUNC(s_remap1,  80, CreateHardLinkA)
BACKPORT_FUNC(s_remap1,  81, CreateHardLinkW)
BACKPORT_FUNC(s_remap1,  82, CreateIoCompletionPort)
BACKPORT_FUNC(s_remap1,  83, CreateJobObjectA)
BACKPORT_FUNC(s_remap1,  84, CreateJobObjectW)
BACKPORT_FUNC(s_remap1,  85, CreateJobSet)
BACKPORT_FUNC(s_remap1,  86, CreateMailslotA)
BACKPORT_FUNC(s_remap1,  87, CreateMailslotW)
BACKPORT_FUNC(s_remap1,  88, CreateMemoryResourceNotification)
BACKPORT_FUNC(s_remap1,  89, CreateMutexA)
BACKPORT_FUNC(s_remap1,  90, CreateMutexW)
BACKPORT_FUNC(s_remap1,  91, CreateNamedPipeA)
BACKPORT_FUNC(s_remap1,  92, CreateNamedPipeW)
BACKPORT_FUNC(s_remap1,  93, CreatePipe)
BACKPORT_FUNC(s_remap1,  94, CreateProcessA)
BACKPORT_FUNC(s_remap1,  95, CreateProcessInternalA)
BACKPORT_FUNC(s_remap1,  96, CreateProcessInternalW)
BACKPORT_FUNC(s_remap1,  97, CreateProcessW)
BACKPORT_FUNC(s_remap1,  98, CreateRemoteThread)
BACKPORT_FUNC(s_remap1,  99, CreateSemaphoreA)
BACKPORT_FUNC(s_remap1, 100, CreateSemaphoreW)
BACKPORT_FUNC(s_remap1, 101, CreateSocketHandle)
BACKPORT_FUNC(s_remap1, 102, CreateTapePartition)
BACKPORT_FUNC(s_remap1, 103, CreateThread)
BACKPORT_FUNC(s_remap1, 104, CreateTimerQueue)
BACKPORT_FUNC(s_remap1, 105, CreateTimerQueueTimer)
BACKPORT_FUNC(s_remap1, 106, CreateToolhelp32Snapshot)
BACKPORT_FUNC(s_remap1, 107, CreateWaitableTimerA)
BACKPORT_FUNC(s_remap1, 108, CreateWaitableTimerW)
BACKPORT_FUNC(s_remap1, 109, DeactivateActCtx)
BACKPORT_FUNC(s_remap1, 110, DebugActiveProcess)
BACKPORT_FUNC(s_remap1, 111, DebugActiveProcessStop)
BACKPORT_FUNC(s_remap1, 112, DebugBreak)
BACKPORT_FUNC(s_remap1, 113, DebugBreakProcess)
BACKPORT_FUNC(s_remap1, 114, DebugSetProcessKillOnExit)
BACKPORT_FUNC(s_remap1, 115, DecodePointer)
BACKPORT_FUNC(s_remap1, 116, DecodeSystemPointer)
BACKPORT_FUNC(s_remap1, 117, DefineDosDeviceA)
BACKPORT_FUNC(s_remap1, 118, DefineDosDeviceW)
BACKPORT_FUNC(s_remap1, 119, DelayLoadFailureHook)
BACKPORT_FUNC(s_remap1, 120, DeleteAtom)
BACKPORT_FUNC(s_remap1, 121, DeleteCriticalSection)
BACKPORT_FUNC(s_remap1, 122, DeleteFiber)
BACKPORT_FUNC(s_remap1, 123, DeleteFileA)
BACKPORT_FUNC(s_remap1, 124, DeleteFileW)
BACKPORT_FUNC(s_remap1, 125, DeleteTimerQueue)
BACKPORT_FUNC(s_remap1, 126, DeleteTimerQueueEx)
BACKPORT_FUNC(s_remap1, 127, DeleteTimerQueueTimer)
BACKPORT_FUNC(s_remap1, 128, DeleteVolumeMountPointA)
BACKPORT_FUNC(s_remap1, 129, DeleteVolumeMountPointW)
BACKPORT_FUNC(s_remap1, 130, DeviceIoControl)
BACKPORT_FUNC(s_remap1, 131, DisableThreadLibraryCalls)
BACKPORT_FUNC(s_remap1, 132, DisconnectNamedPipe)
BACKPORT_FUNC(s_remap1, 133, DnsHostnameToComputerNameA)
BACKPORT_FUNC(s_remap1, 134, DnsHostnameToComputerNameW)
BACKPORT_FUNC(s_remap1, 135, DosDateTimeToFileTime)
BACKPORT_FUNC(s_remap1, 136, DosPathToSessionPathA)
BACKPORT_FUNC(s_remap1, 137, DosPathToSessionPathW)
BACKPORT_FUNC(s_remap1, 138, DuplicateConsoleHandle)
BACKPORT_FUNC(s_remap1, 139, DuplicateHandle)
BACKPORT_FUNC(s_remap1, 140, EncodePointer)
BACKPORT_FUNC(s_remap1, 141, EncodeSystemPointer)
BACKPORT_FUNC(s_remap1, 142, EndUpdateResourceA)
BACKPORT_FUNC(s_remap1, 143, EndUpdateResourceW)
BACKPORT_FUNC(s_remap1, 144, EnterCriticalSection)
BACKPORT_FUNC(s_remap1, 145, EnumCalendarInfoA)
BACKPORT_FUNC(s_remap1, 146, EnumCalendarInfoExA)
BACKPORT_FUNC(s_remap1, 147, EnumCalendarInfoExW)
BACKPORT_FUNC(s_remap1, 148, EnumCalendarInfoW)
BACKPORT_FUNC(s_remap1, 149, EnumDateFormatsA)
BACKPORT_FUNC(s_remap1, 150, EnumDateFormatsExA)
BACKPORT_FUNC(s_remap1, 151, EnumDateFormatsExW)
BACKPORT_FUNC(s_remap1, 152, EnumDateFormatsW)
BACKPORT_FUNC(s_remap1, 153, EnumerateLocalComputerNamesA)
BACKPORT_FUNC(s_remap1, 154, EnumerateLocalComputerNamesW)
BACKPORT_FUNC(s_remap1, 155, EnumLanguageGroupLocalesA)
BACKPORT_FUNC(s_remap1, 156, EnumLanguageGroupLocalesW)
BACKPORT_FUNC(s_remap1, 157, EnumResourceLanguagesA)
BACKPORT_FUNC(s_remap1, 158, EnumResourceLanguagesW)
BACKPORT_FUNC(s_remap1, 159, EnumResourceNamesA)
BACKPORT_FUNC(s_remap1, 160, EnumResourceNamesW)
BACKPORT_FUNC(s_remap1, 161, EnumResourceTypesA)
BACKPORT_FUNC(s_remap1, 162, EnumResourceTypesW)
BACKPORT_FUNC(s_remap1, 163, EnumSystemCodePagesA)
BACKPORT_FUNC(s_remap1, 164, EnumSystemCodePagesW)
BACKPORT_FUNC(s_remap1, 165, EnumSystemGeoID)
BACKPORT_FUNC(s_remap1, 166, EnumSystemLanguageGroupsA)
BACKPORT_FUNC(s_remap1, 167, EnumSystemLanguageGroupsW)
BACKPORT_FUNC(s_remap1, 168, EnumSystemLocalesA)
BACKPORT_FUNC(s_remap1, 169, EnumSystemLocalesW)
BACKPORT_FUNC(s_remap1, 170, EnumTimeFormatsA)
BACKPORT_FUNC(s_remap1, 171, EnumTimeFormatsW)
BACKPORT_FUNC(s_remap1, 172, EnumUILanguagesA)
BACKPORT_FUNC(s_remap1, 173, EnumUILanguagesW)
BACKPORT_FUNC(s_remap1, 174, EraseTape)
BACKPORT_FUNC(s_remap1, 175, EscapeCommFunction)
BACKPORT_FUNC(s_remap1, 176, ExitProcess)
BACKPORT_FUNC(s_remap1, 177, ExitThread)
BACKPORT_FUNC(s_remap1, 178, ExitVDM)
BACKPORT_FUNC(s_remap1, 179, ExpandEnvironmentStringsA)
BACKPORT_FUNC(s_remap1, 180, ExpandEnvironmentStringsW)
BACKPORT_FUNC(s_remap1, 181, ExpungeConsoleCommandHistoryA)
BACKPORT_FUNC(s_remap1, 182, ExpungeConsoleCommandHistoryW)
BACKPORT_FUNC(s_remap1, 183, FatalAppExitA)
BACKPORT_FUNC(s_remap1, 184, FatalAppExitW)
BACKPORT_FUNC(s_remap1, 185, FatalExit)
BACKPORT_FUNC(s_remap1, 186, FileTimeToDosDateTime)
BACKPORT_FUNC(s_remap1, 187, FileTimeToLocalFileTime)
BACKPORT_FUNC(s_remap1, 188, FileTimeToSystemTime)
BACKPORT_FUNC(s_remap1, 189, FillConsoleOutputAttribute)
BACKPORT_FUNC(s_remap1, 190, FillConsoleOutputCharacterA)
BACKPORT_FUNC(s_remap1, 191, FillConsoleOutputCharacterW)
BACKPORT_FUNC(s_remap1, 192, FindActCtxSectionGuid)
BACKPORT_FUNC(s_remap1, 193, FindActCtxSectionStringA)
BACKPORT_FUNC(s_remap1, 194, FindActCtxSectionStringW)
BACKPORT_FUNC(s_remap1, 195, FindAtomA)
BACKPORT_FUNC(s_remap1, 196, FindAtomW)
BACKPORT_FUNC(s_remap1, 197, FindClose)
BACKPORT_FUNC(s_remap1, 198, FindCloseChangeNotification)
BACKPORT_FUNC(s_remap1, 199, FindFirstChangeNotificationA)
BACKPORT_FUNC(s_remap1, 200, FindFirstChangeNotificationW)
BACKPORT_FUNC(s_remap1, 201, FindFirstFileA)
BACKPORT_FUNC(s_remap1, 202, FindFirstFileExA)
BACKPORT_FUNC(s_remap1, 203, FindFirstFileExW)
BACKPORT_FUNC(s_remap1, 204, FindFirstFileW)
BACKPORT_FUNC(s_remap1, 205, FindFirstVolumeA)
BACKPORT_FUNC(s_remap1, 206, FindFirstVolumeMountPointA)
BACKPORT_FUNC(s_remap1, 207, FindFirstVolumeMountPointW)
BACKPORT_FUNC(s_remap1, 208, FindFirstVolumeW)
BACKPORT_FUNC(s_remap1, 209, FindNextChangeNotification)
BACKPORT_FUNC(s_remap1, 210, FindNextFileA)
BACKPORT_FUNC(s_remap1, 211, FindNextFileW)
BACKPORT_FUNC(s_remap1, 212, FindNextVolumeA)
BACKPORT_FUNC(s_remap1, 213, FindNextVolumeMountPointA)
BACKPORT_FUNC(s_remap1, 214, FindNextVolumeMountPointW)
BACKPORT_FUNC(s_remap1, 215, FindNextVolumeW)
BACKPORT_FUNC(s_remap1, 216, FindResourceA)
BACKPORT_FUNC(s_remap1, 217, FindResourceExA)
BACKPORT_FUNC(s_remap1, 218, FindResourceExW)
BACKPORT_FUNC(s_remap1, 219, FindResourceW)
BACKPORT_FUNC(s_remap1, 220, FindVolumeClose)
BACKPORT_FUNC(s_remap1, 221, FindVolumeMountPointClose)
BACKPORT_FUNC(s_remap1, 222, FlsAlloc)
BACKPORT_FUNC(s_remap1, 223, FlsFree)
BACKPORT_FUNC(s_remap1, 224, FlsGetValue)
BACKPORT_FUNC(s_remap1, 225, FlsSetValue)
BACKPORT_FUNC(s_remap1, 226, FlushConsoleInputBuffer)
BACKPORT_FUNC(s_remap1, 227, FlushFileBuffers)
BACKPORT_FUNC(s_remap1, 228, FlushInstructionCache)
BACKPORT_FUNC(s_remap1, 229, FlushViewOfFile)
BACKPORT_FUNC(s_remap1, 230, FoldStringA)
BACKPORT_FUNC(s_remap1, 231, FoldStringW)
BACKPORT_FUNC(s_remap1, 232, FormatMessageA)
BACKPORT_FUNC(s_remap1, 233, FormatMessageW)
BACKPORT_FUNC(s_remap1, 234, FreeConsole)
BACKPORT_FUNC(s_remap1, 235, FreeEnvironmentStringsA)
BACKPORT_FUNC(s_remap1, 236, FreeEnvironmentStringsW)
BACKPORT_FUNC(s_remap1, 237, FreeLibrary)
BACKPORT_FUNC(s_remap1, 238, FreeLibraryAndExitThread)
BACKPORT_FUNC(s_remap1, 239, FreeResource)
BACKPORT_FUNC(s_remap1, 240, FreeUserPhysicalPages)
BACKPORT_FUNC(s_remap1, 241, GenerateConsoleCtrlEvent)
BACKPORT_FUNC(s_remap1, 242, GetACP)
BACKPORT_FUNC(s_remap1, 243, GetAtomNameA)
BACKPORT_FUNC(s_remap1, 244, GetAtomNameW)
BACKPORT_FUNC(s_remap1, 245, GetBinaryType)
BACKPORT_FUNC(s_remap1, 246, GetBinaryTypeA)
BACKPORT_FUNC(s_remap1, 247, GetBinaryTypeW)
BACKPORT_FUNC(s_remap1, 248, GetCalendarInfoA)
BACKPORT_FUNC(s_remap1, 249, GetCalendarInfoW)
BACKPORT_FUNC(s_remap1, 250, GetCommandLineA)
BACKPORT_FUNC(s_remap1, 251, GetCommandLineW)
BACKPORT_FUNC(s_remap1, 252, GetCommConfig)
BACKPORT_FUNC(s_remap1, 253, GetCommMask)
BACKPORT_FUNC(s_remap1, 254, GetCommModemStatus)
BACKPORT_FUNC(s_remap1, 255, GetCommProperties)
BACKPORT_FUNC(s_remap1, 256, GetCommState)
BACKPORT_FUNC(s_remap1, 257, GetCommTimeouts)
BACKPORT_FUNC(s_remap1, 258, GetComPlusPackageInstallStatus)
BACKPORT_FUNC(s_remap1, 259, GetCompressedFileSizeA)
BACKPORT_FUNC(s_remap1, 260, GetCompressedFileSizeW)
BACKPORT_FUNC(s_remap1, 261, GetComputerNameA)
BACKPORT_FUNC(s_remap1, 262, GetComputerNameExA)
BACKPORT_FUNC(s_remap1, 263, GetComputerNameExW)
BACKPORT_FUNC(s_remap1, 264, GetComputerNameW)
BACKPORT_FUNC(s_remap1, 265, GetConsoleAliasA)
BACKPORT_FUNC(s_remap1, 266, GetConsoleAliasesA)
BACKPORT_FUNC(s_remap1, 267, GetConsoleAliasesLengthA)
BACKPORT_FUNC(s_remap1, 268, GetConsoleAliasesLengthW)
BACKPORT_FUNC(s_remap1, 269, GetConsoleAliasesW)
BACKPORT_FUNC(s_remap1, 270, GetConsoleAliasExesA)
BACKPORT_FUNC(s_remap1, 271, GetConsoleAliasExesLengthA)
BACKPORT_FUNC(s_remap1, 272, GetConsoleAliasExesLengthW)
BACKPORT_FUNC(s_remap1, 273, GetConsoleAliasExesW)
BACKPORT_FUNC(s_remap1, 274, GetConsoleAliasW)
BACKPORT_FUNC(s_remap1, 275, GetConsoleCharType)
BACKPORT_FUNC(s_remap1, 276, GetConsoleCommandHistoryA)
BACKPORT_FUNC(s_remap1, 277, GetConsoleCommandHistoryLengthA)
BACKPORT_FUNC(s_remap1, 278, GetConsoleCommandHistoryLengthW)
BACKPORT_FUNC(s_remap1, 279, GetConsoleCommandHistoryW)
BACKPORT_FUNC(s_remap1, 280, GetConsoleCP)
BACKPORT_FUNC(s_remap1, 281, GetConsoleCursorInfo)
BACKPORT_FUNC(s_remap1, 282, GetConsoleCursorMode)
BACKPORT_FUNC(s_remap1, 283, GetConsoleDisplayMode)
BACKPORT_FUNC(s_remap1, 284, GetConsoleFontInfo)
BACKPORT_FUNC(s_remap1, 285, GetConsoleFontSize)
BACKPORT_FUNC(s_remap1, 286, GetConsoleHardwareState)
BACKPORT_FUNC(s_remap1, 287, GetConsoleInputExeNameA)
BACKPORT_FUNC(s_remap1, 288, GetConsoleInputExeNameW)
BACKPORT_FUNC(s_remap1, 289, GetConsoleInputWaitHandle)
BACKPORT_FUNC(s_remap1, 290, GetConsoleKeyboardLayoutNameA)
BACKPORT_FUNC(s_remap1, 291, GetConsoleKeyboardLayoutNameW)
BACKPORT_FUNC(s_remap1, 292, GetConsoleMode)
BACKPORT_FUNC(s_remap1, 293, GetConsoleNlsMode)
BACKPORT_FUNC(s_remap1, 294, GetConsoleOutputCP)
BACKPORT_FUNC(s_remap1, 295, GetConsoleProcessList)
BACKPORT_FUNC(s_remap1, 296, GetConsoleScreenBufferInfo)
BACKPORT_FUNC(s_remap1, 297, GetConsoleSelectionInfo)
BACKPORT_FUNC(s_remap1, 298, GetConsoleTitleA)
BACKPORT_FUNC(s_remap1, 299, GetConsoleTitleW)
BACKPORT_FUNC(s_remap1, 300, GetConsoleWindow)
BACKPORT_FUNC(s_remap1, 301, GetCPInfo)
BACKPORT_FUNC(s_remap1, 302, GetCPInfoExA)
BACKPORT_FUNC(s_remap1, 303, GetCPInfoExW)
BACKPORT_FUNC(s_remap1, 304, GetCurrencyFormatA)
BACKPORT_FUNC(s_remap1, 305, GetCurrencyFormatW)
BACKPORT_FUNC(s_remap1, 306, GetCurrentActCtx)
BACKPORT_FUNC(s_remap1, 307, GetCurrentConsoleFont)
BACKPORT_FUNC(s_remap1, 308, GetCurrentDirectoryA)
BACKPORT_FUNC(s_remap1, 309, GetCurrentDirectoryW)
BACKPORT_FUNC(s_remap1, 310, GetCurrentProcess)
BACKPORT_FUNC(s_remap1, 311, GetCurrentProcessId)
BACKPORT_FUNC(s_remap1, 312, GetCurrentThread)
BACKPORT_FUNC(s_remap1, 313, GetCurrentThreadId)
BACKPORT_FUNC(s_remap1, 314, GetDateFormatA)
BACKPORT_FUNC(s_remap1, 315, GetDateFormatW)
BACKPORT_FUNC(s_remap1, 316, GetDefaultCommConfigA)
BACKPORT_FUNC(s_remap1, 317, GetDefaultCommConfigW)
BACKPORT_FUNC(s_remap1, 318, GetDevicePowerState)
BACKPORT_FUNC(s_remap1, 319, GetDiskFreeSpaceA)
BACKPORT_FUNC(s_remap1, 320, GetDiskFreeSpaceExA)
BACKPORT_FUNC(s_remap1, 321, GetDiskFreeSpaceExW)
BACKPORT_FUNC(s_remap1, 322, GetDiskFreeSpaceW)
BACKPORT_FUNC(s_remap1, 323, GetDllDirectoryA)
BACKPORT_FUNC(s_remap1, 324, GetDllDirectoryW)
BACKPORT_FUNC(s_remap1, 325, GetDriveTypeA)
BACKPORT_FUNC(s_remap1, 326, GetDriveTypeW)
BACKPORT_FUNC(s_remap1, 327, GetEnvironmentStrings)
BACKPORT_FUNC(s_remap1, 328, GetEnvironmentStringsA)
BACKPORT_FUNC(s_remap1, 329, GetEnvironmentStringsW)
BACKPORT_FUNC(s_remap1, 330, GetEnvironmentVariableA)
BACKPORT_FUNC(s_remap1, 331, GetEnvironmentVariableW)
BACKPORT_FUNC(s_remap1, 332, GetExitCodeProcess)
BACKPORT_FUNC(s_remap1, 333, GetExitCodeThread)
BACKPORT_FUNC(s_remap1, 334, GetExpandedNameA)
BACKPORT_FUNC(s_remap1, 335, GetExpandedNameW)
BACKPORT_FUNC(s_remap1, 336, GetFileAttributesA)
BACKPORT_FUNC(s_remap1, 337, GetFileAttributesExA)
BACKPORT_FUNC(s_remap1, 338, GetFileAttributesExW)
BACKPORT_FUNC(s_remap1, 339, GetFileAttributesW)
BACKPORT_FUNC(s_remap1, 340, GetFileInformationByHandle)
BACKPORT_FUNC(s_remap1, 341, GetFileSize)
BACKPORT_FUNC(s_remap1, 342, GetFileSizeEx)
BACKPORT_FUNC(s_remap1, 343, GetFileTime)
BACKPORT_FUNC(s_remap1, 344, GetFileType)
BACKPORT_FUNC(s_remap1, 345, GetFirmwareEnvironmentVariableA)
BACKPORT_FUNC(s_remap1, 346, GetFirmwareEnvironmentVariableW)
BACKPORT_FUNC(s_remap1, 347, GetFullPathNameA)
BACKPORT_FUNC(s_remap1, 348, GetFullPathNameW)
BACKPORT_FUNC(s_remap1, 349, GetGeoInfoA)
BACKPORT_FUNC(s_remap1, 350, GetGeoInfoW)
BACKPORT_FUNC(s_remap1, 351, GetHandleContext)
BACKPORT_FUNC(s_remap1, 352, GetHandleInformation)
BACKPORT_FUNC(s_remap1, 353, GetLargestConsoleWindowSize)
BACKPORT_FUNC(s_remap1, 354, GetLastError)
BACKPORT_FUNC(s_remap1, 355, GetLocaleInfoA)
BACKPORT_FUNC(s_remap1, 356, GetLocaleInfoW)
BACKPORT_FUNC(s_remap1, 357, GetLocalTime)
BACKPORT_FUNC(s_remap1, 358, GetLogicalDrives)
BACKPORT_FUNC(s_remap1, 359, GetLogicalDriveStringsA)
BACKPORT_FUNC(s_remap1, 360, GetLogicalDriveStringsW)
BACKPORT_FUNC(s_remap1, 361, GetLogicalProcessorInformation)
BACKPORT_FUNC(s_remap1, 362, GetLongPathNameA)
BACKPORT_FUNC(s_remap1, 363, GetLongPathNameW)
BACKPORT_FUNC(s_remap1, 364, GetMailslotInfo)
BACKPORT_FUNC(s_remap1, 365, GetModuleFileNameA)
BACKPORT_FUNC(s_remap1, 366, GetModuleFileNameW)
BACKPORT_FUNC(s_remap1, 367, GetModuleHandleA)
BACKPORT_FUNC(s_remap1, 368, GetModuleHandleExA)
BACKPORT_FUNC(s_remap1, 369, GetModuleHandleExW)
BACKPORT_FUNC(s_remap1, 370, GetModuleHandleW)
BACKPORT_FUNC(s_remap1, 371, GetNamedPipeHandleStateA)
BACKPORT_FUNC(s_remap1, 372, GetNamedPipeHandleStateW)
BACKPORT_FUNC(s_remap1, 373, GetNamedPipeInfo)
BACKPORT_FUNC(s_remap1, 374, GetNativeSystemInfo)
BACKPORT_FUNC(s_remap1, 375, GetNextVDMCommand)
BACKPORT_FUNC(s_remap1, 376, GetNumaAvailableMemoryNode)
BACKPORT_FUNC(s_remap1, 377, GetNumaHighestNodeNumber)
BACKPORT_FUNC(s_remap1, 378, GetNumaNodeProcessorMask)
BACKPORT_FUNC(s_remap1, 379, GetNumaProcessorNode)
BACKPORT_FUNC(s_remap1, 380, GetNumberFormatA)
BACKPORT_FUNC(s_remap1, 381, GetNumberFormatW)
BACKPORT_FUNC(s_remap1, 382, GetNumberOfConsoleFonts)
BACKPORT_FUNC(s_remap1, 383, GetNumberOfConsoleInputEvents)
BACKPORT_FUNC(s_remap1, 384, GetNumberOfConsoleMouseButtons)
BACKPORT_FUNC(s_remap1, 385, GetOEMCP)
BACKPORT_FUNC(s_remap1, 386, GetOverlappedResult)
BACKPORT_FUNC(s_remap1, 387, GetPriorityClass)
BACKPORT_FUNC(s_remap1, 388, GetPrivateProfileIntA)
BACKPORT_FUNC(s_remap1, 389, GetPrivateProfileIntW)
BACKPORT_FUNC(s_remap1, 390, GetPrivateProfileSectionA)
BACKPORT_FUNC(s_remap1, 391, GetPrivateProfileSectionNamesA)
BACKPORT_FUNC(s_remap1, 392, GetPrivateProfileSectionNamesW)
BACKPORT_FUNC(s_remap1, 393, GetPrivateProfileSectionW)
BACKPORT_FUNC(s_remap1, 394, GetPrivateProfileStringA)
BACKPORT_FUNC(s_remap1, 395, GetPrivateProfileStringW)
BACKPORT_FUNC(s_remap1, 396, GetPrivateProfileStructA)
BACKPORT_FUNC(s_remap1, 397, GetPrivateProfileStructW)
BACKPORT_FUNC(s_remap1, 398, GetProcAddress)
BACKPORT_FUNC(s_remap1, 399, GetProcessAffinityMask)
BACKPORT_FUNC(s_remap1, 400, GetProcessDEPPolicy)
BACKPORT_FUNC(s_remap1, 401, GetProcessHandleCount)
BACKPORT_FUNC(s_remap1, 402, GetProcessHeap)
BACKPORT_FUNC(s_remap1, 403, GetProcessHeaps)
BACKPORT_FUNC(s_remap1, 404, GetProcessId)
BACKPORT_FUNC(s_remap1, 405, GetProcessIoCounters)
BACKPORT_FUNC(s_remap1, 406, GetProcessPriorityBoost)
BACKPORT_FUNC(s_remap1, 407, GetProcessShutdownParameters)
BACKPORT_FUNC(s_remap1, 408, GetProcessTimes)
BACKPORT_FUNC(s_remap1, 409, GetProcessVersion)
BACKPORT_FUNC(s_remap1, 410, GetProcessWorkingSetSize)
BACKPORT_FUNC(s_remap1, 411, GetProfileIntA)
BACKPORT_FUNC(s_remap1, 412, GetProfileIntW)
BACKPORT_FUNC(s_remap1, 413, GetProfileSectionA)
BACKPORT_FUNC(s_remap1, 414, GetProfileSectionW)
BACKPORT_FUNC(s_remap1, 415, GetProfileStringA)
BACKPORT_FUNC(s_remap1, 416, GetProfileStringW)
BACKPORT_FUNC(s_remap1, 417, GetQueuedCompletionStatus)
BACKPORT_FUNC(s_remap1, 418, GetShortPathNameA)
BACKPORT_FUNC(s_remap1, 419, GetShortPathNameW)
BACKPORT_FUNC(s_remap1, 420, GetStartupInfoA)
BACKPORT_FUNC(s_remap1, 421, GetStartupInfoW)
BACKPORT_FUNC(s_remap1, 422, GetStdHandle)
BACKPORT_FUNC(s_remap1, 423, GetStringTypeA)
BACKPORT_FUNC(s_remap1, 424, GetStringTypeExA)
BACKPORT_FUNC(s_remap1, 425, GetStringTypeExW)
BACKPORT_FUNC(s_remap1, 426, GetStringTypeW)
BACKPORT_FUNC(s_remap1, 427, GetSystemDefaultLangID)
BACKPORT_FUNC(s_remap1, 428, GetSystemDefaultLCID)
BACKPORT_FUNC(s_remap1, 429, GetSystemDefaultUILanguage)
BACKPORT_FUNC(s_remap1, 430, GetSystemDEPPolicy)
BACKPORT_FUNC(s_remap1, 431, GetSystemDirectoryA)
BACKPORT_FUNC(s_remap1, 432, GetSystemDirectoryW)
BACKPORT_FUNC(s_remap1, 433, GetSystemInfo)
BACKPORT_FUNC(s_remap1, 434, GetSystemPowerStatus)
BACKPORT_FUNC(s_remap1, 435, GetSystemRegistryQuota)
BACKPORT_FUNC(s_remap1, 436, GetSystemTime)
BACKPORT_FUNC(s_remap1, 437, GetSystemTimeAdjustment)
BACKPORT_FUNC(s_remap1, 438, GetSystemTimeAsFileTime)
BACKPORT_FUNC(s_remap1, 439, GetSystemTimes)
BACKPORT_FUNC(s_remap1, 440, GetSystemWindowsDirectoryA)
BACKPORT_FUNC(s_remap1, 441, GetSystemWindowsDirectoryW)
BACKPORT_FUNC(s_remap1, 442, GetSystemWow64DirectoryA)
BACKPORT_FUNC(s_remap1, 443, GetSystemWow64DirectoryW)
BACKPORT_FUNC(s_remap1, 444, GetTapeParameters)
BACKPORT_FUNC(s_remap1, 445, GetTapePosition)
BACKPORT_FUNC(s_remap1, 446, GetTapeStatus)
BACKPORT_FUNC(s_remap1, 447, GetTempFileNameA)
BACKPORT_FUNC(s_remap1, 448, GetTempFileNameW)
BACKPORT_FUNC(s_remap1, 449, GetTempPathA)
BACKPORT_FUNC(s_remap1, 450, GetTempPathW)
BACKPORT_FUNC(s_remap1, 451, GetThreadContext)
BACKPORT_FUNC(s_remap1, 452, GetThreadIOPendingFlag)
BACKPORT_FUNC(s_remap1, 453, GetThreadLocale)
BACKPORT_FUNC(s_remap1, 454, GetThreadPriority)
BACKPORT_FUNC(s_remap1, 455, GetThreadPriorityBoost)
BACKPORT_FUNC(s_remap1, 456, GetThreadSelectorEntry)
BACKPORT_FUNC(s_remap1, 457, GetThreadTimes)
BACKPORT_FUNC(s_remap1, 458, GetTickCount)
BACKPORT_FUNC(s_remap1, 459, GetTickCount64)
BACKPORT_FUNC(s_remap1, 460, GetTimeFormatA)
BACKPORT_FUNC(s_remap1, 461, GetTimeFormatW)
BACKPORT_FUNC(s_remap1, 462, GetTimeZoneInformation)
BACKPORT_FUNC(s_remap1, 463, GetUserDefaultLangID)
BACKPORT_FUNC(s_remap1, 464, GetUserDefaultLCID)
BACKPORT_FUNC(s_remap1, 465, GetUserDefaultUILanguage)
BACKPORT_FUNC(s_remap1, 466, GetUserGeoID)
BACKPORT_FUNC(s_remap1, 467, GetVDMCurrentDirectories)
BACKPORT_FUNC(s_remap1, 468, GetVersion)
BACKPORT_FUNC(s_remap1, 469, GetVersionExA)
BACKPORT_FUNC(s_remap1, 470, GetVersionExW)
BACKPORT_FUNC(s_remap1, 471, GetVolumeInformationA)
BACKPORT_FUNC(s_remap1, 472, GetVolumeInformationW)
BACKPORT_FUNC(s_remap1, 473, GetVolumeNameForVolumeMountPointA)
BACKPORT_FUNC(s_remap1, 474, GetVolumeNameForVolumeMountPointW)
BACKPORT_FUNC(s_remap1, 475, GetVolumePathNameA)
BACKPORT_FUNC(s_remap1, 476, GetVolumePathNamesForVolumeNameA)
BACKPORT_FUNC(s_remap1, 477, GetVolumePathNamesForVolumeNameW)
BACKPORT_FUNC(s_remap1, 478, GetVolumePathNameW)
BACKPORT_FUNC(s_remap1, 479, GetWindowsDirectoryA)
BACKPORT_FUNC(s_remap1, 480, GetWindowsDirectoryW)
BACKPORT_FUNC(s_remap1, 481, GetWriteWatch)
BACKPORT_FUNC(s_remap1, 482, GlobalAddAtomA)
BACKPORT_FUNC(s_remap1, 483, GlobalAddAtomW)
BACKPORT_FUNC(s_remap1, 484, GlobalAlloc)
BACKPORT_FUNC(s_remap1, 485, GlobalCompact)
BACKPORT_FUNC(s_remap1, 486, GlobalDeleteAtom)
BACKPORT_FUNC(s_remap1, 487, GlobalFindAtomA)
BACKPORT_FUNC(s_remap1, 488, GlobalFindAtomW)
BACKPORT_FUNC(s_remap1, 489, GlobalFix)
BACKPORT_FUNC(s_remap1, 490, GlobalFlags)
BACKPORT_FUNC(s_remap1, 491, GlobalFree)
BACKPORT_FUNC(s_remap1, 492, GlobalGetAtomNameA)
BACKPORT_FUNC(s_remap1, 493, GlobalGetAtomNameW)
BACKPORT_FUNC(s_remap1, 494, GlobalHandle)
BACKPORT_FUNC(s_remap1, 495, GlobalLock)
BACKPORT_FUNC(s_remap1, 496, GlobalMemoryStatus)
BACKPORT_FUNC(s_remap1, 497, GlobalMemoryStatusEx)
BACKPORT_FUNC(s_remap1, 498, GlobalReAlloc)
BACKPORT_FUNC(s_remap1, 499, GlobalSize)
BACKPORT_FUNC(s_remap1, 500, GlobalUnfix)
BACKPORT_FUNC(s_remap1, 501, GlobalUnlock)
BACKPORT_FUNC(s_remap1, 502, GlobalUnWire)
BACKPORT_FUNC(s_remap1, 503, GlobalWire)
BACKPORT_FUNC(s_remap1, 504, Heap32First)
BACKPORT_FUNC(s_remap1, 505, Heap32ListFirst)
BACKPORT_FUNC(s_remap1, 506, Heap32ListNext)
BACKPORT_FUNC(s_remap1, 507, Heap32Next)
BACKPORT_FUNC(s_remap1, 508, HeapAlloc)
BACKPORT_FUNC(s_remap1, 509, HeapCompact)
BACKPORT_FUNC(s_remap1, 510, HeapCreate)
BACKPORT_FUNC(s_remap1, 511, HeapDestroy)
BACKPORT_FUNC(s_remap1, 512, HeapFree)
BACKPORT_FUNC(s_remap1, 513, HeapLock)
BACKPORT_FUNC(s_remap1, 514, HeapQueryInformation)
BACKPORT_FUNC(s_remap1, 515, HeapReAlloc)
BACKPORT_FUNC(s_remap1, 516, HeapSetInformation)
BACKPORT_FUNC(s_remap1, 517, HeapSize)
BACKPORT_FUNC(s_remap1, 518, HeapSummary)
BACKPORT_FUNC(s_remap1, 519, HeapUnlock)
BACKPORT_FUNC(s_remap1, 520, HeapValidate)
BACKPORT_FUNC(s_remap1, 521, HeapWalk)
BACKPORT_FUNC(s_remap1, 522, InitAtomTable)
BACKPORT_FUNC(s_remap1, 523, InitializeCriticalSection)
BACKPORT_FUNC(s_remap1, 524, InitializeCriticalSectionAndSpinCount)
BACKPORT_FUNC(s_remap1, 525, InitializeSListHead)
BACKPORT_FUNC(s_remap1, 526, InitOnceExecuteOnce)
BACKPORT_FUNC(s_remap1, 527, InterlockedCompareExchange)
BACKPORT_FUNC(s_remap1, 528, InterlockedDecrement)
BACKPORT_FUNC(s_remap1, 529, InterlockedExchange)
BACKPORT_FUNC(s_remap1, 530, InterlockedExchangeAdd)
BACKPORT_FUNC(s_remap1, 531, InterlockedFlushSList)
BACKPORT_FUNC(s_remap1, 532, InterlockedIncrement)
BACKPORT_FUNC(s_remap1, 533, InterlockedPopEntrySList)
BACKPORT_FUNC(s_remap1, 534, InterlockedPushEntrySList)
BACKPORT_FUNC(s_remap1, 535, InvalidateConsoleDIBits)
BACKPORT_FUNC(s_remap1, 536, IsBadCodePtr)
BACKPORT_FUNC(s_remap1, 537, IsBadHugeReadPtr)
BACKPORT_FUNC(s_remap1, 538, IsBadHugeWritePtr)
BACKPORT_FUNC(s_remap1, 539, IsBadReadPtr)
BACKPORT_FUNC(s_remap1, 540, IsBadStringPtrA)
BACKPORT_FUNC(s_remap1, 541, IsBadStringPtrW)
BACKPORT_FUNC(s_remap1, 542, IsBadWritePtr)
BACKPORT_FUNC(s_remap1, 543, IsDBCSLeadByte)
BACKPORT_FUNC(s_remap1, 544, IsDBCSLeadByteEx)
BACKPORT_FUNC(s_remap1, 545, IsDebuggerPresent)
BACKPORT_FUNC(s_remap1, 546, IsProcessInJob)
BACKPORT_FUNC(s_remap1, 547, IsProcessorFeaturePresent)
BACKPORT_FUNC(s_remap1, 548, IsSystemResumeAutomatic)
BACKPORT_FUNC(s_remap1, 549, IsValidCodePage)
BACKPORT_FUNC(s_remap1, 550, IsValidLanguageGroup)
BACKPORT_FUNC(s_remap1, 551, IsValidLocale)
BACKPORT_FUNC(s_remap1, 552, IsWow64Process)
BACKPORT_FUNC(s_remap1, 553, LCMapStringA)
BACKPORT_FUNC(s_remap1, 554, LCMapStringEx)
BACKPORT_FUNC(s_remap1, 555, LCMapStringW)
BACKPORT_FUNC(s_remap1, 556, LeaveCriticalSection)
BACKPORT_FUNC(s_remap1, 557, LoadLibraryA)
BACKPORT_FUNC(s_remap1, 558, LoadLibraryExA)
BACKPORT_FUNC(s_remap1, 559, LoadLibraryExW)
BACKPORT_FUNC(s_remap1, 560, LoadLibraryW)
BACKPORT_FUNC(s_remap1, 561, LoadModule)
BACKPORT_FUNC(s_remap1, 562, LoadResource)
BACKPORT_FUNC(s_remap1, 563, LocalAlloc)
BACKPORT_FUNC(s_remap1, 564, LocalCompact)
BACKPORT_FUNC(s_remap1, 565, LocalFileTimeToFileTime)
BACKPORT_FUNC(s_remap1, 566, LocalFlags)
BACKPORT_FUNC(s_remap1, 567, LocalFree)
BACKPORT_FUNC(s_remap1, 568, LocalHandle)
BACKPORT_FUNC(s_remap1, 569, LocalLock)
BACKPORT_FUNC(s_remap1, 570, LocalReAlloc)
BACKPORT_FUNC(s_remap1, 571, LocalShrink)
BACKPORT_FUNC(s_remap1, 572, LocalSize)
BACKPORT_FUNC(s_remap1, 573, LocalUnlock)
BACKPORT_FUNC(s_remap1, 574, LockFile)
BACKPORT_FUNC(s_remap1, 575, LockFileEx)
BACKPORT_FUNC(s_remap1, 576, LockResource)
BACKPORT_FUNC(s_remap1, 577, lstrcat)
BACKPORT_FUNC(s_remap1, 578, lstrcatA)
BACKPORT_FUNC(s_remap1, 579, lstrcatW)
BACKPORT_FUNC(s_remap1, 580, lstrcmp)
BACKPORT_FUNC(s_remap1, 581, lstrcmpA)
BACKPORT_FUNC(s_remap1, 582, lstrcmpi)
BACKPORT_FUNC(s_remap1, 583, lstrcmpiA)
BACKPORT_FUNC(s_remap1, 584, lstrcmpiW)
BACKPORT_FUNC(s_remap1, 585, lstrcmpW)
BACKPORT_FUNC(s_remap1, 586, lstrcpy)
BACKPORT_FUNC(s_remap1, 587, lstrcpyA)
BACKPORT_FUNC(s_remap1, 588, lstrcpyn)
BACKPORT_FUNC(s_remap1, 589, lstrcpynA)
BACKPORT_FUNC(s_remap1, 590, lstrcpynW)
BACKPORT_FUNC(s_remap1, 591, lstrcpyW)
BACKPORT_FUNC(s_remap1, 592, lstrlen)
BACKPORT_FUNC(s_remap1, 593, lstrlenA)
BACKPORT_FUNC(s_remap1, 594, lstrlenW)
BACKPORT_FUNC(s_remap1, 595, LZClose)
BACKPORT_FUNC(s_remap1, 596, LZCloseFile)
BACKPORT_FUNC(s_remap1, 597, LZCopy)
BACKPORT_FUNC(s_remap1, 598, LZCreateFileW)
BACKPORT_FUNC(s_remap1, 599, LZDone)
BACKPORT_FUNC(s_remap1, 600, LZInit)
BACKPORT_FUNC(s_remap1, 601, LZOpenFileA)
BACKPORT_FUNC(s_remap1, 602, LZOpenFileW)
BACKPORT_FUNC(s_remap1, 603, LZRead)
BACKPORT_FUNC(s_remap1, 604, LZSeek)
BACKPORT_FUNC(s_remap1, 605, LZStart)
BACKPORT_FUNC(s_remap1, 606, MapUserPhysicalPages)
BACKPORT_FUNC(s_remap1, 607, MapUserPhysicalPagesScatter)
BACKPORT_FUNC(s_remap1, 608, MapViewOfFile)
BACKPORT_FUNC(s_remap1, 609, MapViewOfFileEx)
BACKPORT_FUNC(s_remap1, 610, Module32First)
BACKPORT_FUNC(s_remap1, 611, Module32FirstW)
BACKPORT_FUNC(s_remap1, 612, Module32Next)
BACKPORT_FUNC(s_remap1, 613, Module32NextW)
BACKPORT_FUNC(s_remap1, 614, MoveFileA)
BACKPORT_FUNC(s_remap1, 615, MoveFileExA)
BACKPORT_FUNC(s_remap1, 616, MoveFileExW)
BACKPORT_FUNC(s_remap1, 617, MoveFileW)
BACKPORT_FUNC(s_remap1, 618, MoveFileWithProgressA)
BACKPORT_FUNC(s_remap1, 619, MoveFileWithProgressW)
BACKPORT_FUNC(s_remap1, 620, MulDiv)
BACKPORT_FUNC(s_remap1, 621, MultiByteToWideChar)
BACKPORT_FUNC(s_remap1, 622, NlsGetCacheUpdateCount)
BACKPORT_FUNC(s_remap1, 623, OpenConsoleW)
BACKPORT_FUNC(s_remap1, 624, OpenEventA)
BACKPORT_FUNC(s_remap1, 625, OpenEventW)
BACKPORT_FUNC(s_remap1, 626, OpenFile)
BACKPORT_FUNC(s_remap1, 627, OpenFileMappingA)
BACKPORT_FUNC(s_remap1, 628, OpenFileMappingW)
BACKPORT_FUNC(s_remap1, 629, OpenJobObjectA)
BACKPORT_FUNC(s_remap1, 630, OpenJobObjectW)
BACKPORT_FUNC(s_remap1, 631, OpenMutexA)
BACKPORT_FUNC(s_remap1, 632, OpenMutexW)
BACKPORT_FUNC(s_remap1, 633, OpenProcess)
BACKPORT_FUNC(s_remap1, 634, OpenProfileUserMapping)
BACKPORT_FUNC(s_remap1, 635, OpenSemaphoreA)
BACKPORT_FUNC(s_remap1, 636, OpenSemaphoreW)
BACKPORT_FUNC(s_remap1, 637, OpenThread)
BACKPORT_FUNC(s_remap1, 638, OpenWaitableTimerA)
BACKPORT_FUNC(s_remap1, 639, OpenWaitableTimerW)
BACKPORT_FUNC(s_remap1, 640, OutputDebugStringA)
BACKPORT_FUNC(s_remap1, 641, OutputDebugStringW)
BACKPORT_FUNC(s_remap1, 642, PeekConsoleInputA)
BACKPORT_FUNC(s_remap1, 643, PeekConsoleInputW)
BACKPORT_FUNC(s_remap1, 644, PeekNamedPipe)
BACKPORT_FUNC(s_remap1, 645, PostQueuedCompletionStatus)
BACKPORT_FUNC(s_remap1, 646, PrepareTape)
BACKPORT_FUNC(s_remap1, 647, PrivCopyFileExW)
BACKPORT_FUNC(s_remap1, 648, PrivMoveFileIdentityW)
BACKPORT_FUNC(s_remap1, 649, Process32First)
BACKPORT_FUNC(s_remap1, 650, Process32FirstW)
BACKPORT_FUNC(s_remap1, 651, Process32Next)
BACKPORT_FUNC(s_remap1, 652, Process32NextW)
BACKPORT_FUNC(s_remap1, 653, ProcessIdToSessionId)
BACKPORT_FUNC(s_remap1, 654, PulseEvent)
BACKPORT_FUNC(s_remap1, 655, PurgeComm)
BACKPORT_FUNC(s_remap1, 656, QueryActCtxW)
BACKPORT_FUNC(s_remap1, 657, QueryDepthSList)
BACKPORT_FUNC(s_remap1, 658, QueryDosDeviceA)
BACKPORT_FUNC(s_remap1, 659, QueryDosDeviceW)
BACKPORT_FUNC(s_remap1, 660, QueryInformationJobObject)
BACKPORT_FUNC(s_remap1, 661, QueryMemoryResourceNotification)
BACKPORT_FUNC(s_remap1, 662, QueryPerformanceCounter)
BACKPORT_FUNC(s_remap1, 663, QueryPerformanceFrequency)
BACKPORT_FUNC(s_remap1, 664, QueueUserAPC)
BACKPORT_FUNC(s_remap1, 665, QueueUserWorkItem)
BACKPORT_FUNC(s_remap1, 666, RaiseException)
BACKPORT_FUNC(s_remap1, 667, ReadConsoleA)
BACKPORT_FUNC(s_remap1, 668, ReadConsoleInputA)
BACKPORT_FUNC(s_remap1, 669, ReadConsoleInputExA)
BACKPORT_FUNC(s_remap1, 670, ReadConsoleInputExW)
BACKPORT_FUNC(s_remap1, 671, ReadConsoleInputW)
BACKPORT_FUNC(s_remap1, 672, ReadConsoleOutputA)
BACKPORT_FUNC(s_remap1, 673, ReadConsoleOutputAttribute)
BACKPORT_FUNC(s_remap1, 674, ReadConsoleOutputCharacterA)
BACKPORT_FUNC(s_remap1, 675, ReadConsoleOutputCharacterW)
BACKPORT_FUNC(s_remap1, 676, ReadConsoleOutputW)
BACKPORT_FUNC(s_remap1, 677, ReadConsoleW)
BACKPORT_FUNC(s_remap1, 678, ReadDirectoryChangesW)
BACKPORT_FUNC(s_remap1, 679, ReadFile)
BACKPORT_FUNC(s_remap1, 680, ReadFileEx)
BACKPORT_FUNC(s_remap1, 681, ReadFileScatter)
BACKPORT_FUNC(s_remap1, 682, ReadProcessMemory)
BACKPORT_FUNC(s_remap1, 683, RegisterConsoleIME)
BACKPORT_FUNC(s_remap1, 684, RegisterConsoleOS2)
BACKPORT_FUNC(s_remap1, 685, RegisterConsoleVDM)
BACKPORT_FUNC(s_remap1, 686, RegisterWaitForInputIdle)
BACKPORT_FUNC(s_remap1, 687, RegisterWaitForSingleObject)
BACKPORT_FUNC(s_remap1, 688, RegisterWaitForSingleObjectEx)
BACKPORT_FUNC(s_remap1, 689, RegisterWowBaseHandlers)
BACKPORT_FUNC(s_remap1, 690, RegisterWowExec)
BACKPORT_FUNC(s_remap1, 691, ReleaseActCtx)
BACKPORT_FUNC(s_remap1, 692, ReleaseMutex)
BACKPORT_FUNC(s_remap1, 693, ReleaseSemaphore)
BACKPORT_FUNC(s_remap1, 694, RemoveDirectoryA)
BACKPORT_FUNC(s_remap1, 695, RemoveDirectoryW)
BACKPORT_FUNC(s_remap1, 696, RemoveLocalAlternateComputerNameA)
BACKPORT_FUNC(s_remap1, 697, RemoveLocalAlternateComputerNameW)
BACKPORT_FUNC(s_remap1, 698, RemoveVectoredExceptionHandler)
BACKPORT_FUNC(s_remap1, 699, ReplaceFile)
BACKPORT_FUNC(s_remap1, 700, ReplaceFileA)
BACKPORT_FUNC(s_remap1, 701, ReplaceFileW)
BACKPORT_FUNC(s_remap1, 702, RequestDeviceWakeup)
BACKPORT_FUNC(s_remap1, 703, RequestWakeupLatency)
BACKPORT_FUNC(s_remap1, 704, ResetEvent)
BACKPORT_FUNC(s_remap1, 705, ResetWriteWatch)
BACKPORT_FUNC(s_remap1, 706, RestoreLastError)
BACKPORT_FUNC(s_remap1, 707, ResumeThread)
BACKPORT_FUNC(s_remap1, 708, RtlCaptureContext)
BACKPORT_FUNC(s_remap1, 709, RtlCaptureStackBackTrace)
BACKPORT_FUNC(s_remap1, 710, RtlFillMemory)
BACKPORT_FUNC(s_remap1, 711, RtlMoveMemory)
BACKPORT_FUNC(s_remap1, 712, RtlUnwind)
BACKPORT_FUNC(s_remap1, 713, RtlZeroMemory)
BACKPORT_FUNC(s_remap1, 714, ScrollConsoleScreenBufferA)
BACKPORT_FUNC(s_remap1, 715, ScrollConsoleScreenBufferW)
BACKPORT_FUNC(s_remap1, 716, SearchPathA)
BACKPORT_FUNC(s_remap1, 717, SearchPathW)
BACKPORT_FUNC(s_remap1, 718, SetCalendarInfoA)
BACKPORT_FUNC(s_remap1, 719, SetCalendarInfoW)
BACKPORT_FUNC(s_remap1, 720, SetClientTimeZoneInformation)
BACKPORT_FUNC(s_remap1, 721, SetCommBreak)
BACKPORT_FUNC(s_remap1, 722, SetCommConfig)
BACKPORT_FUNC(s_remap1, 723, SetCommMask)
BACKPORT_FUNC(s_remap1, 724, SetCommState)
BACKPORT_FUNC(s_remap1, 725, SetCommTimeouts)
BACKPORT_FUNC(s_remap1, 726, SetComPlusPackageInstallStatus)
BACKPORT_FUNC(s_remap1, 727, SetComputerNameA)
BACKPORT_FUNC(s_remap1, 728, SetComputerNameExA)
BACKPORT_FUNC(s_remap1, 729, SetComputerNameExW)
BACKPORT_FUNC(s_remap1, 730, SetComputerNameW)
BACKPORT_FUNC(s_remap1, 731, SetConsoleActiveScreenBuffer)
BACKPORT_FUNC(s_remap1, 732, SetConsoleCP)
BACKPORT_FUNC(s_remap1, 733, SetConsoleCtrlHandler)
BACKPORT_FUNC(s_remap1, 734, SetConsoleCursor)
BACKPORT_FUNC(s_remap1, 735, SetConsoleCursorInfo)
BACKPORT_FUNC(s_remap1, 736, SetConsoleCursorMode)
BACKPORT_FUNC(s_remap1, 737, SetConsoleCursorPosition)
BACKPORT_FUNC(s_remap1, 738, SetConsoleDisplayMode)
BACKPORT_FUNC(s_remap1, 739, SetConsoleFont)
BACKPORT_FUNC(s_remap1, 740, SetConsoleHardwareState)
BACKPORT_FUNC(s_remap1, 741, SetConsoleIcon)
BACKPORT_FUNC(s_remap1, 742, SetConsoleInputExeNameA)
BACKPORT_FUNC(s_remap1, 743, SetConsoleInputExeNameW)
BACKPORT_FUNC(s_remap1, 744, SetConsoleKeyShortcuts)
BACKPORT_FUNC(s_remap1, 745, SetConsoleLocalEUDC)
BACKPORT_FUNC(s_remap1, 746, SetConsoleMaximumWindowSize)
BACKPORT_FUNC(s_remap1, 747, SetConsoleMenuClose)
BACKPORT_FUNC(s_remap1, 748, SetConsoleMode)
BACKPORT_FUNC(s_remap1, 749, SetConsoleNlsMode)
BACKPORT_FUNC(s_remap1, 750, SetConsoleNumberOfCommandsA)
BACKPORT_FUNC(s_remap1, 751, SetConsoleNumberOfCommandsW)
BACKPORT_FUNC(s_remap1, 752, SetConsoleOS2OemFormat)
BACKPORT_FUNC(s_remap1, 753, SetConsoleOutputCP)
BACKPORT_FUNC(s_remap1, 754, SetConsolePalette)
BACKPORT_FUNC(s_remap1, 755, SetConsoleScreenBufferSize)
BACKPORT_FUNC(s_remap1, 756, SetConsoleTextAttribute)
BACKPORT_FUNC(s_remap1, 757, SetConsoleTitleA)
BACKPORT_FUNC(s_remap1, 758, SetConsoleTitleW)
BACKPORT_FUNC(s_remap1, 759, SetConsoleWindowInfo)
BACKPORT_FUNC(s_remap1, 760, SetCriticalSectionSpinCount)
BACKPORT_FUNC(s_remap1, 761, SetCurrentDirectoryA)
BACKPORT_FUNC(s_remap1, 762, SetCurrentDirectoryW)
BACKPORT_FUNC(s_remap1, 763, SetDefaultCommConfigA)
BACKPORT_FUNC(s_remap1, 764, SetDefaultCommConfigW)
BACKPORT_FUNC(s_remap1, 765, SetDllDirectoryA)
BACKPORT_FUNC(s_remap1, 766, SetDllDirectoryW)
BACKPORT_FUNC(s_remap1, 767, SetEndOfFile)
BACKPORT_FUNC(s_remap1, 768, SetEnvironmentVariableA)
BACKPORT_FUNC(s_remap1, 769, SetEnvironmentVariableW)
BACKPORT_FUNC(s_remap1, 770, SetErrorMode)
BACKPORT_FUNC(s_remap1, 771, SetEvent)
BACKPORT_FUNC(s_remap1, 772, SetFileApisToANSI)
BACKPORT_FUNC(s_remap1, 773, SetFileApisToOEM)
BACKPORT_FUNC(s_remap1, 774, SetFileAttributesA)
BACKPORT_FUNC(s_remap1, 775, SetFileAttributesW)
BACKPORT_FUNC(s_remap1, 776, SetFilePointer)
BACKPORT_FUNC(s_remap1, 777, SetFilePointerEx)
BACKPORT_FUNC(s_remap1, 778, SetFileShortNameA)
BACKPORT_FUNC(s_remap1, 779, SetFileShortNameW)
BACKPORT_FUNC(s_remap1, 780, SetFileTime)
BACKPORT_FUNC(s_remap1, 781, SetFileValidData)
BACKPORT_FUNC(s_remap1, 782, SetFirmwareEnvironmentVariableA)
BACKPORT_FUNC(s_remap1, 783, SetFirmwareEnvironmentVariableW)
BACKPORT_FUNC(s_remap1, 784, SetHandleContext)
BACKPORT_FUNC(s_remap1, 785, SetHandleCount)
BACKPORT_FUNC(s_remap1, 786, SetHandleInformation)
BACKPORT_FUNC(s_remap1, 787, SetInformationJobObject)
BACKPORT_FUNC(s_remap1, 788, SetLastConsoleEventActive)
BACKPORT_FUNC(s_remap1, 789, SetLastError)
BACKPORT_FUNC(s_remap1, 790, SetLocaleInfoA)
BACKPORT_FUNC(s_remap1, 791, SetLocaleInfoW)
BACKPORT_FUNC(s_remap1, 792, SetLocalPrimaryComputerNameA)
BACKPORT_FUNC(s_remap1, 793, SetLocalPrimaryComputerNameW)
BACKPORT_FUNC(s_remap1, 794, SetLocalTime)
BACKPORT_FUNC(s_remap1, 795, SetMailslotInfo)
BACKPORT_FUNC(s_remap1, 796, SetMessageWaitingIndicator)
BACKPORT_FUNC(s_remap1, 797, SetNamedPipeHandleState)
BACKPORT_FUNC(s_remap1, 798, SetPriorityClass)
BACKPORT_FUNC(s_remap1, 799, SetProcessAffinityMask)
BACKPORT_FUNC(s_remap1, 800, SetProcessDEPPolicy)
BACKPORT_FUNC(s_remap1, 801, SetProcessPriorityBoost)
BACKPORT_FUNC(s_remap1, 802, SetProcessShutdownParameters)
BACKPORT_FUNC(s_remap1, 803, SetProcessWorkingSetSize)
BACKPORT_FUNC(s_remap1, 804, SetSearchPathMode)
BACKPORT_FUNC(s_remap1, 805, SetStdHandle)
BACKPORT_FUNC(s_remap1, 806, SetSystemPowerState)
BACKPORT_FUNC(s_remap1, 807, SetSystemTime)
BACKPORT_FUNC(s_remap1, 808, SetSystemTimeAdjustment)
BACKPORT_FUNC(s_remap1, 809, SetTapeParameters)
BACKPORT_FUNC(s_remap1, 810, SetTapePosition)
BACKPORT_FUNC(s_remap1, 811, SetTermsrvAppInstallMode)
BACKPORT_FUNC(s_remap1, 812, SetThreadAffinityMask)
BACKPORT_FUNC(s_remap1, 813, SetThreadContext)
BACKPORT_FUNC(s_remap1, 814, SetThreadExecutionState)
BACKPORT_FUNC(s_remap1, 815, SetThreadIdealProcessor)
BACKPORT_FUNC(s_remap1, 816, SetThreadLocale)
BACKPORT_FUNC(s_remap1, 817, SetThreadPriority)
BACKPORT_FUNC(s_remap1, 818, SetThreadPriorityBoost)
BACKPORT_FUNC(s_remap1, 819, SetThreadUILanguage)
BACKPORT_FUNC(s_remap1, 820, SetTimerQueueTimer)
BACKPORT_FUNC(s_remap1, 821, SetTimeZoneInformation)
BACKPORT_FUNC(s_remap1, 822, SetUnhandledExceptionFilter)
BACKPORT_FUNC(s_remap1, 823, SetupComm)
BACKPORT_FUNC(s_remap1, 824, SetUserGeoID)
BACKPORT_FUNC(s_remap1, 825, SetVDMCurrentDirectories)
BACKPORT_FUNC(s_remap1, 826, SetVolumeLabelA)
BACKPORT_FUNC(s_remap1, 827, SetVolumeLabelW)
BACKPORT_FUNC(s_remap1, 828, SetVolumeMountPointA)
BACKPORT_FUNC(s_remap1, 829, SetVolumeMountPointW)
BACKPORT_FUNC(s_remap1, 830, SetWaitableTimer)
BACKPORT_FUNC(s_remap1, 831, ShowConsoleCursor)
BACKPORT_FUNC(s_remap1, 832, SignalObjectAndWait)
BACKPORT_FUNC(s_remap1, 833, SizeofResource)
BACKPORT_FUNC(s_remap1, 834, Sleep)
BACKPORT_FUNC(s_remap1, 835, SleepEx)
BACKPORT_FUNC(s_remap1, 836, SuspendThread)
BACKPORT_FUNC(s_remap1, 837, SwitchToFiber)
BACKPORT_FUNC(s_remap1, 838, SwitchToThread)
BACKPORT_FUNC(s_remap1, 839, SystemTimeToFileTime)
BACKPORT_FUNC(s_remap1, 840, SystemTimeToTzSpecificLocalTime)
BACKPORT_FUNC(s_remap1, 841, TerminateJobObject)
BACKPORT_FUNC(s_remap1, 842, TerminateProcess)
BACKPORT_FUNC(s_remap1, 843, TerminateThread)
BACKPORT_FUNC(s_remap1, 844, TermsrvAppInstallMode)
BACKPORT_FUNC(s_remap1, 845, Thread32First)
BACKPORT_FUNC(s_remap1, 846, Thread32Next)
BACKPORT_FUNC(s_remap1, 847, TlsAlloc)
BACKPORT_FUNC(s_remap1, 848, TlsFree)
BACKPORT_FUNC(s_remap1, 849, TlsGetValue)
BACKPORT_FUNC(s_remap1, 850, TlsSetValue)
BACKPORT_FUNC(s_remap1, 851, Toolhelp32ReadProcessMemory)
BACKPORT_FUNC(s_remap1, 852, TransactNamedPipe)
BACKPORT_FUNC(s_remap1, 853, TransmitCommChar)
BACKPORT_FUNC(s_remap1, 854, TryEnterCriticalSection)
BACKPORT_FUNC(s_remap1, 855, TzSpecificLocalTimeToSystemTime)
BACKPORT_FUNC(s_remap1, 856, UnhandledExceptionFilter)
BACKPORT_FUNC(s_remap1, 857, UnlockFile)
BACKPORT_FUNC(s_remap1, 858, UnlockFileEx)
BACKPORT_FUNC(s_remap1, 859, UnmapViewOfFile)
BACKPORT_FUNC(s_remap1, 860, UnregisterConsoleIME)
BACKPORT_FUNC(s_remap1, 861, UnregisterWait)
BACKPORT_FUNC(s_remap1, 862, UnregisterWaitEx)
BACKPORT_FUNC(s_remap1, 863, UpdateResourceA)
BACKPORT_FUNC(s_remap1, 864, UpdateResourceW)
BACKPORT_FUNC(s_remap1, 865, UTRegister)
BACKPORT_FUNC(s_remap1, 866, UTUnRegister)
BACKPORT_FUNC(s_remap1, 867, VDMConsoleOperation)
BACKPORT_FUNC(s_remap1, 868, VDMOperationStarted)
BACKPORT_FUNC(s_remap1, 869, VerifyConsoleIoHandle)
BACKPORT_FUNC(s_remap1, 870, VerifyVersionInfoA)
BACKPORT_FUNC(s_remap1, 871, VerifyVersionInfoW)
BACKPORT_FUNC(s_remap1, 872, VerLanguageNameA)
BACKPORT_FUNC(s_remap1, 873, VerLanguageNameW)
BACKPORT_FUNC(s_remap1, 874, VerSetConditionMask)
BACKPORT_FUNC(s_remap1, 875, VirtualAlloc)
BACKPORT_FUNC(s_remap1, 876, VirtualAllocEx)
BACKPORT_FUNC(s_remap1, 877, VirtualFree)
BACKPORT_FUNC(s_remap1, 878, VirtualFreeEx)
BACKPORT_FUNC(s_remap1, 879, VirtualLock)
BACKPORT_FUNC(s_remap1, 880, VirtualProtect)
BACKPORT_FUNC(s_remap1, 881, VirtualProtectEx)
BACKPORT_FUNC(s_remap1, 882, VirtualQuery)
BACKPORT_FUNC(s_remap1, 883, VirtualQueryEx)
BACKPORT_FUNC(s_remap1, 884, VirtualUnlock)
BACKPORT_FUNC(s_remap1, 885, WaitCommEvent)
BACKPORT_FUNC(s_remap1, 886, WaitForDebugEvent)
BACKPORT_FUNC(s_remap1, 887, WaitForMultipleObjects)
BACKPORT_FUNC(s_remap1, 888, WaitForMultipleObjectsEx)
BACKPORT_FUNC(s_remap1, 889, WaitForSingleObject)
BACKPORT_FUNC(s_remap1, 890, WaitForSingleObjectEx)
BACKPORT_FUNC(s_remap1, 891, WaitNamedPipeA)
BACKPORT_FUNC(s_remap1, 892, WaitNamedPipeW)
BACKPORT_FUNC(s_remap1, 893, WideCharToMultiByte)
BACKPORT_FUNC(s_remap1, 894, WinExec)
BACKPORT_FUNC(s_remap1, 895, WriteConsoleA)
BACKPORT_FUNC(s_remap1, 896, WriteConsoleInputA)
BACKPORT_FUNC(s_remap1, 897, WriteConsoleInputVDMA)
BACKPORT_FUNC(s_remap1, 898, WriteConsoleInputVDMW)
BACKPORT_FUNC(s_remap1, 899, WriteConsoleInputW)
BACKPORT_FUNC(s_remap1, 900, WriteConsoleOutputA)
BACKPORT_FUNC(s_remap1, 901, WriteConsoleOutputAttribute)
BACKPORT_FUNC(s_remap1, 902, WriteConsoleOutputCharacterA)
BACKPORT_FUNC(s_remap1, 903, WriteConsoleOutputCharacterW)
BACKPORT_FUNC(s_remap1, 904, WriteConsoleOutputW)
BACKPORT_FUNC(s_remap1, 905, WriteConsoleW)
BACKPORT_FUNC(s_remap1, 906, WriteFile)
BACKPORT_FUNC(s_remap1, 907, WriteFileEx)
BACKPORT_FUNC(s_remap1, 908, WriteFileGather)
BACKPORT_FUNC(s_remap1, 909, WritePrivateProfileSectionA)
BACKPORT_FUNC(s_remap1, 910, WritePrivateProfileSectionW)
BACKPORT_FUNC(s_remap1, 911, WritePrivateProfileStringA)
BACKPORT_FUNC(s_remap1, 912, WritePrivateProfileStringW)
BACKPORT_FUNC(s_remap1, 913, WritePrivateProfileStructA)
BACKPORT_FUNC(s_remap1, 914, WritePrivateProfileStructW)
BACKPORT_FUNC(s_remap1, 915, WriteProcessMemory)
BACKPORT_FUNC(s_remap1, 916, WriteProfileSectionA)
BACKPORT_FUNC(s_remap1, 917, WriteProfileSectionW)
BACKPORT_FUNC(s_remap1, 918, WriteProfileStringA)
BACKPORT_FUNC(s_remap1, 919, WriteProfileStringW)
BACKPORT_FUNC(s_remap1, 920, WriteTapemark)
BACKPORT_FUNC(s_remap1, 921, WTSGetActiveConsoleSessionId)
BACKPORT_FUNC(s_remap1, 922, ZombifyActCtx)
BACKPORT_FUNC(s_remap1, 923, _hread)
BACKPORT_FUNC(s_remap1, 924, _hwrite)
BACKPORT_FUNC(s_remap1, 925, _lclose)
BACKPORT_FUNC(s_remap1, 926, _lcreat)
BACKPORT_FUNC(s_remap1, 927, _llseek)
BACKPORT_FUNC(s_remap1, 928, _lopen)
BACKPORT_FUNC(s_remap1, 929, _lread)
BACKPORT_FUNC(s_remap1, 930, _lwrite)
