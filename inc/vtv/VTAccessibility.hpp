// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'VTAccessibility.pas' rev: 6.00

#ifndef VTAccessibilityHPP
#define VTAccessibilityHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Controls.hpp>	// Pascal unit
#include <VTAccessibilityFactory.hpp>	// Pascal unit
#include <VirtualTrees.hpp>	// Pascal unit
#include <MSAAIntf.hpp>	// Pascal unit
#include <ActiveX.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Vtaccessibility
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TVirtualTreeAccessibility;
class PASCALIMPLEMENTATION TVirtualTreeAccessibility : public System::TInterfacedObject 
{
	typedef System::TInterfacedObject inherited;
	
private:
	Virtualtrees::TVirtualStringTree* FVirtualTree;
	
public:
	HRESULT __stdcall Get_accParent(/* out */ _di_IDispatch &ppdispParent);
	HRESULT __stdcall Get_accChildCount(/* out */ int &pcountChildren);
	HRESULT __stdcall Get_accChild(const OleVariant varChild, /* out */ _di_IDispatch &ppdispChild);
	HRESULT __stdcall Get_accName(const OleVariant varChild, /* out */ WideString &pszName);
	HRESULT __stdcall Get_accValue(const OleVariant varChild, /* out */ WideString &pszValue);
	HRESULT __stdcall Get_accDescription(const OleVariant varChild, /* out */ WideString &pszDescription);
	HRESULT __stdcall Get_accRole(const OleVariant varChild, /* out */ OleVariant &pvarRole);
	HRESULT __stdcall Get_accState(const OleVariant varChild, /* out */ OleVariant &pvarState);
	HRESULT __stdcall Get_accHelp(const OleVariant varChild, /* out */ WideString &pszHelp);
	HRESULT __stdcall Get_accHelpTopic(/* out */ WideString &pszHelpFile, const OleVariant varChild, /* out */ int &pidTopic);
	HRESULT __stdcall Get_accKeyboardShortcut(const OleVariant varChild, /* out */ WideString &pszKeyboardShortcut);
	HRESULT __stdcall Get_accFocus(/* out */ OleVariant &pvarChild);
	HRESULT __stdcall Get_accSelection(/* out */ OleVariant &pvarChildren);
	HRESULT __stdcall Get_accDefaultAction(const OleVariant varChild, /* out */ WideString &pszDefaultAction);
	HRESULT __stdcall accSelect(int flagsSelect, const OleVariant varChild);
	HRESULT __stdcall accLocation(/* out */ int &pxLeft, /* out */ int &pyTop, /* out */ int &pcxWidth, /* out */ int &pcyHeight, const OleVariant varChild);
	HRESULT __stdcall accNavigate(int navDir, const OleVariant varStart, /* out */ OleVariant &pvarEndUpAt);
	HRESULT __stdcall accHitTest(int xLeft, int yTop, /* out */ OleVariant &pvarChild);
	HRESULT __stdcall accDoDefaultAction(const OleVariant varChild);
	HRESULT __stdcall Set_accName(const OleVariant varChild, const WideString pszName);
	HRESULT __stdcall Set_accValue(const OleVariant varChild, const WideString pszValue);
	HRESULT __stdcall GetIDsOfNames(const GUID &IID, void * Names, int NameCount, int LocaleID, void * DispIDs);
	HRESULT __stdcall GetTypeInfo(int Index, int LocaleID, /* out */ void *TypeInfo);
	HRESULT __stdcall GetTypeInfoCount(/* out */ int &Count);
	HRESULT __stdcall Invoke(int DispID, const GUID &IID, int LocaleID, Word Flags, void *Params, void * VarResult, void * ExcepInfo, void * ArgErr);
	__fastcall TVirtualTreeAccessibility(Virtualtrees::TVirtualStringTree* VirtualTree);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TVirtualTreeAccessibility(void) { }
	#pragma option pop
	
private:
	void *__IAccessible;	/* IAccessible */
	
public:
	operator IAccessible*(void) { return (IAccessible*)&__IAccessible; }
	operator IDispatch*(void) { return (IDispatch*)&__IAccessible; }
	
};


class DELPHICLASS TVirtualTreeItemAccessibility;
class PASCALIMPLEMENTATION TVirtualTreeItemAccessibility : public TVirtualTreeAccessibility 
{
	typedef TVirtualTreeAccessibility inherited;
	
public:
	HIDESBASE HRESULT __stdcall Get_accParent(/* out */ _di_IDispatch &ppdispParent);
	HIDESBASE HRESULT __stdcall Get_accChildCount(/* out */ int &pcountChildren);
	HIDESBASE HRESULT __stdcall Get_accChild(const OleVariant varChild, /* out */ _di_IDispatch &ppdispChild);
	HIDESBASE HRESULT __stdcall Get_accName(const OleVariant varChild, /* out */ WideString &pszName);
	HIDESBASE HRESULT __stdcall Get_accValue(const OleVariant varChild, /* out */ WideString &pszValue);
	HIDESBASE HRESULT __stdcall Get_accDescription(const OleVariant varChild, /* out */ WideString &pszDescription);
	HIDESBASE HRESULT __stdcall Get_accRole(const OleVariant varChild, /* out */ OleVariant &pvarRole);
	HIDESBASE HRESULT __stdcall Get_accState(const OleVariant varChild, /* out */ OleVariant &pvarState);
	HIDESBASE HRESULT __stdcall accLocation(/* out */ int &pxLeft, /* out */ int &pyTop, /* out */ int &pcxWidth, /* out */ int &pcyHeight, const OleVariant varChild);
	__fastcall TVirtualTreeItemAccessibility(Virtualtrees::TVirtualStringTree* VirtualTree);
	HIDESBASE HRESULT __stdcall Get_accFocus(/* out */ OleVariant &pvarChild);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TVirtualTreeItemAccessibility(void) { }
	#pragma option pop
	
private:
	void *__IAccessible;	/* IAccessible */
	
public:
	operator IAccessible*(void) { return (IAccessible*)&__IAccessible; }
	
};


class DELPHICLASS TVTMultiColumnItemAccessibility;
class PASCALIMPLEMENTATION TVTMultiColumnItemAccessibility : public TVirtualTreeItemAccessibility 
{
	typedef TVirtualTreeItemAccessibility inherited;
	
private:
	HRESULT __stdcall GetItemDescription(const OleVariant varChild, /* out */ WideString &pszDescription, bool IncludeMainColumn);
	
public:
	HIDESBASE HRESULT __stdcall Get_accName(const OleVariant varChild, /* out */ WideString &pszName);
	HIDESBASE HRESULT __stdcall Get_accDescription(const OleVariant varChild, /* out */ WideString &pszDescription);
public:
	#pragma option push -w-inl
	/* TVirtualTreeItemAccessibility.Create */ inline __fastcall TVTMultiColumnItemAccessibility(Virtualtrees::TVirtualStringTree* VirtualTree) : TVirtualTreeItemAccessibility(VirtualTree) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TVTMultiColumnItemAccessibility(void) { }
	#pragma option pop
	
private:
	void *__IAccessible;	/* IAccessible */
	
public:
	operator IAccessible*(void) { return (IAccessible*)&__IAccessible; }
	
};


class DELPHICLASS TVTDefaultAccessibleProvider;
class PASCALIMPLEMENTATION TVTDefaultAccessibleProvider : public System::TInterfacedObject 
{
	typedef System::TInterfacedObject inherited;
	
public:
	_di_IAccessible __fastcall CreateIAccessible(Virtualtrees::TBaseVirtualTree* ATree);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TVTDefaultAccessibleProvider(void) : System::TInterfacedObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TVTDefaultAccessibleProvider(void) { }
	#pragma option pop
	
private:
	void *__IVTAccessibleProvider;	/* Vtaccessibilityfactory::IVTAccessibleProvider */
	
public:
	operator IVTAccessibleProvider*(void) { return (IVTAccessibleProvider*)&__IVTAccessibleProvider; }
	
};


class DELPHICLASS TVTDefaultAccessibleItemProvider;
class PASCALIMPLEMENTATION TVTDefaultAccessibleItemProvider : public System::TInterfacedObject 
{
	typedef System::TInterfacedObject inherited;
	
public:
	_di_IAccessible __fastcall CreateIAccessible(Virtualtrees::TBaseVirtualTree* ATree);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TVTDefaultAccessibleItemProvider(void) : System::TInterfacedObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TVTDefaultAccessibleItemProvider(void) { }
	#pragma option pop
	
private:
	void *__IVTAccessibleProvider;	/* Vtaccessibilityfactory::IVTAccessibleProvider */
	
public:
	operator IVTAccessibleProvider*(void) { return (IVTAccessibleProvider*)&__IVTAccessibleProvider; }
	
};


class DELPHICLASS TVTMultiColumnAccessibleItemProvider;
class PASCALIMPLEMENTATION TVTMultiColumnAccessibleItemProvider : public System::TInterfacedObject 
{
	typedef System::TInterfacedObject inherited;
	
public:
	_di_IAccessible __fastcall CreateIAccessible(Virtualtrees::TBaseVirtualTree* ATree);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TVTMultiColumnAccessibleItemProvider(void) : System::TInterfacedObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TVTMultiColumnAccessibleItemProvider(void) { }
	#pragma option pop
	
private:
	void *__IVTAccessibleProvider;	/* Vtaccessibilityfactory::IVTAccessibleProvider */
	
public:
	operator IVTAccessibleProvider*(void) { return (IVTAccessibleProvider*)&__IVTAccessibleProvider; }
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Vtaccessibility */
using namespace Vtaccessibility;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// VTAccessibility
