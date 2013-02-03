// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'ThemeMgrDB.pas' rev: 6.00

#ifndef ThemeMgrDBHPP
#define ThemeMgrDBHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ThemeMgr.hpp>	// Pascal unit
#include <DBCtrls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Thememgrdb
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TThemeManagerDB;
class PASCALIMPLEMENTATION TThemeManagerDB : public Thememgr::TThemeManager 
{
	typedef Thememgr::TThemeManager inherited;
	
private:
	Thememgr::TWindowProcList* FDBLookupControlList;
	void __fastcall DBLookupControlWindowProc(Controls::TControl* Control, Messages::TMessage &Message);
	void __fastcall PreDBLookupControlWindowProc(Messages::TMessage &Message);
	
protected:
	virtual void __fastcall HandleControlChange(Controls::TControl* Control, bool Inserting);
	virtual bool __fastcall NeedsBorderPaint(Controls::TControl* Control);
	
public:
	__fastcall virtual TThemeManagerDB(Classes::TComponent* AOwner);
	__fastcall virtual ~TThemeManagerDB(void);
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Thememgrdb */
using namespace Thememgrdb;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// ThemeMgrDB
