//////////////////////////////////////////////////////////////////////////////
// GcaSDK Copyright (C) 2000-2003 SYN All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////
#ifndef __GcaSDK_h__
#define __GcaSDK_h__

// ヘッダとライブラリ
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// STL関係
#pragma warning(disable:4786)
#include <string>
#include <vector>
using namespace std;

typedef unsigned __int64 QWORD;

namespace nsGcaSDK{

	DWORD GetStartOffset(const string& strPathName);
	BOOL GetSpellFromKeyFile(string &strSpell, const string &strKeyFile);

	class CGca{
	public:
		enum ArcType{ GCA0 = 0, GCA1, };
	private:
		void *m_pContext;
		DWORD m_dwUser;
		BOOL (*m_pfnProgress)(QWORD qwProgress, QWORD qwAll, void *pContext);
		int (*m_pfnMessage)(int nNum, string strFileName, void *pContext);
	private:
		static BOOL ProgressProc(QWORD qwProgress, QWORD qwAll, void *pContext);
		static int MessageProc(int nNum, string strFileName, void *pContext);
	public:
		BOOL Init(void);
		void Free(void);

		void SetArcFilePath(const string &strArcFilePath);
		void SetOutputPath(const string &strOutputPath);

		void SetRestoreDirectory(BOOL bRestoreDirectory);

		void SetCrypto(BOOL bCrypto);
		void SetSpell(const string &strSpell);
		
		void SetOrder(const vector<string> &astrOrder);
		void SetCompress(BOOL bCompress);
		void SetArcType(int nType);
//		void SetVMAX(BOOL bVMAX);
		void SetStartOffset(int nStartOffset);
		void SetProgressProc(BOOL (*pfnProgress)(QWORD, QWORD, PVOID));
		void SetMessageProc(int (*pfnMessage)(int, string, PVOID));

		int GetArcType(void);
		int GetNumFiles(void);
		string GetFileName(int n);
		DWORD GetFileCRC(int n);
		QWORD GetDataSize(int n);
		QWORD GetFileSize(int n);
		DWORD GetFileAttributesA(int n);
		FILETIME GetFileTime(int n);

//		BOOL Archive(void);
		BOOL OpenArchive(void);
		BOOL CloseArchive(void);
		BOOL Extract(void);
		BOOL ExtractFile(int n);
		BOOL ExtractFileToMemory(int n, BYTE *pBuffer);

		void SetUser(DWORD dwUser) {m_dwUser = dwUser;} 
		DWORD GetUser(void){return m_dwUser;}

	private:
		CGca(const CGca&);            // コピー禁止
		CGca& operator=(const CGca&); // コピー禁止
	public:
		CGca();
		~CGca();
	};

	const int GCAM_OK        = 0;
	const int GCAM_CANCEL    = 1;
	const int nREPORT  =  4 << 24;
	const int nWARNING =  8 << 24;
	const int nERROR   = 12 << 24;
	const int nQUERY   = 16 << 24;
	const int nBEGIN   =  2 << 16;
	const int nEND     =  3 << 16;
	const int nENCODE  =  4 << 16;
	const int nDECODE  =  8 << 16;
	const int nEXIST   = 12 << 16;
	const int nOPEN    = 16 << 16;
	const int nCLOSE   = 20 << 16;
	const int nREAD    = 24 << 16;
	const int nWRITE   = 28 << 16;
	const int nCRC     = 32 << 16;
	const int nCRYPTO  = 36 << 16;
	const int nCREATE  = 40 << 16;
	const int nUNKNOWN_FORMAT = 128 << 16;
	const int nANALYZE_SOLID  = 132 << 16;
	const int nARC     =  4 << 8;
	const int nFILE    =  8 << 8;
	const int nDIRECTORY = 12 << 8;

	const int GCAM_FILE_ENCODE_BEGIN  = nFILE | nENCODE | nBEGIN | nREPORT;
	const int GCAM_FILE_ENCODE_END    = nFILE | nENCODE | nEND   | nREPORT;
	const int GCAM_FILE_DECODE_BEGIN  = nFILE | nDECODE | nBEGIN | nREPORT;
	const int GCAM_FILE_DECODE_END    = nFILE | nDECODE | nEND   | nREPORT;

	const int GCAM_ARC_UNKNOWN_FORMAT = nARC  | nUNKNOWN_FORMAT  | nERROR;
	const int GCAM_ARC_ANALYZE_SOLID  = nARC  | nANALYZE_SOLID   | nWARNING;

	const int GCAM_FILE_EXIST         = nFILE | nEXIST | nQUERY;
	const int GCAM_FILE_OPEN_ERROR    = nFILE | nOPEN  | nERROR;

	const int GCAM_ARC_READ_ERROR     = nARC  | nREAD  | nERROR;
	const int GCAM_FILE_READ_ERROR    = nFILE | nREAD  | nERROR;
	const int GCAM_ARC_WRITE_ERROR    = nARC  | nWRITE | nERROR;
	const int GCAM_FILE_WRITE_ERROR   = nFILE | nWRITE | nERROR;

	const int GCAM_ARC_CRC_ERROR      = nARC  | nCRC   | nERROR;
	const int GCAM_FILE_CRC_ERROR     = nFILE | nCRC   | nERROR;

	const int GCAM_ARC_CRYPTO_ERROR   = nARC  | nCRYPTO | nERROR;
	const int GCAM_FILE_CRYPTO_ERROR  = nFILE | nCRYPTO | nERROR;
	const int GCAM_ARC_CRYPTO         = nARC  | nCRYPTO | nREPORT;
	const int GCAM_FILE_CRYPTO        = nFILE | nCRYPTO | nREPORT;

	const int GCAM_DIRECTORY_CREATE_ERROR = nDIRECTORY | nCREATE | nERROR;
}
using namespace nsGcaSDK;


#endif  __GcaSDK_h__
