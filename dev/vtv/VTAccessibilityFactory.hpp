// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'VTAccessibilityFactory.pas' rev: 6.00

#ifndef VTAccessibilityFactoryHPP
#define VTAccessibilityFactoryHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <VirtualTrees.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <MSAAIntf.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Vtaccessibilityfactory
{
//-- type declarations -------------------------------------------------------
__interface IVTAccessibleProvider;
typedef System::DelphiInterface<IVTAccessibleProvider> _di_IVTAccessibleProvider;
__interface IVTAccessibleProvider  : public IInterface 
{
	
public:
	virtual _di_IAccessible __fastcall CreateIAccessible(Virtualtrees::TBaseVirtualTree* ATree) = 0 ;
};

class DELPHICLASS TVTAccessibilityFactory;
class PASCALIMPLEMENTATION TVTAccessibilityFactory : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	Classes::TInterfaceList* FAccessibleProviders;
	
public:
	__fastcall TVTAccessibilityFactory(void);
	__fastcall virtual ~TVTAccessibilityFactory(void);
	_di_IAccessible __fastcall CreateIAccessible(Virtualtrees::TBaseVirtualTree* ATree);
	void __fastcall RegisterAccessibleProvider(_di_IVTAccessibleProvider AProvider);
	void __fastcall UnRegisterAccessibleProvider(_di_IVTAccessibleProvider AProvider);
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TVTAccessibilityFactory* __fastcall GetAccessibilityFactory(void);

}	/* namespace Vtaccessibilityfactory */
using namespace Vtaccessibilityfactory;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// VTAccessibilityFactory
