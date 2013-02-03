// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'MSAAIntf.pas' rev: 6.00

#ifndef MSAAIntfHPP
#define MSAAIntfHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ActiveX.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------
#include <oleacc.h>
typedef DelphiInterface<IAccessible> _di_IAccessible;
#pragma link "oleacc.lib"

namespace Msaaintf
{
//-- type declarations -------------------------------------------------------
typedef Byte *PByte1;

typedef GUID *PUserType1;

#pragma pack(push, 4)
struct __MIDL_IWinTypes_0009
{
	
	union
	{
		struct 
		{
			int hRemote;
			
		};
		struct 
		{
			int hInproc;
			
		};
		
	};
} ;
#pragma pack(pop)

#pragma pack(push, 1)
struct _RemotableHandle
{
	int fContext;
	__MIDL_IWinTypes_0009 u;
} ;
#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
static const Shortint AccessibilityMajorVersion = 0x1;
static const Shortint AccessibilityMinorVersion = 0x1;
extern PACKAGE bool __fastcall InitAccLibrary(void);
extern PACKAGE void __fastcall FreeAccLibrary(void);

}	/* namespace Msaaintf */
using namespace Msaaintf;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// MSAAIntf
