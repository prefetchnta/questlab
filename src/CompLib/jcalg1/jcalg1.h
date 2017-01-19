#ifdef  __cplusplus
extern "C" {
#endif

typedef BOOL (__stdcall *PFNCALLBACKFUNC)(DWORD, DWORD);
typedef void * (__stdcall *PFNALLOCFUNC)(DWORD);
typedef BOOL (__stdcall *PFNDEALLOCFUNC)(void *);

typedef struct  tagJCALG1_Info
{
	DWORD majorVer;
	DWORD minorVer;
	DWORD nFastSize;
	DWORD nSmallSize;
} _JCALG1_Info;

DWORD __stdcall JCALG1_Compress(
	const void *Source,
	DWORD Length,
	void *Destination,
	DWORD WindowSize,
	PFNALLOCFUNC,
	PFNDEALLOCFUNC,
	PFNCALLBACKFUNC,
	BOOL bDisableChecksum);

DWORD __stdcall JCALG1_Decompress_Fast(
	const void *Source,
	void *Destination);

DWORD __stdcall JCALG1_Decompress_Small(
	const void *Source,
	void *Destination);

DWORD __stdcall JCALG1_GetNeededBufferSize(
	DWORD nSize);

DWORD __stdcall JCALG1_GetInfo(
	_JCALG1_Info *JCALG1_Info);

DWORD __stdcall JCALG1_GetUncompressedSizeOfCompressedBlock(
	const void *pBlock);

DWORD __stdcall JCALG1_GetNeededBufferSize(
	DWORD nSize);

#ifdef  __cplusplus
}
#endif
