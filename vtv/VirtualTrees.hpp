// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'VirtualTrees.pas' rev: 6.00

#ifndef VirtualTreesHPP
#define VirtualTreesHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <UxTheme.hpp>	// Pascal unit
#include <TmSchema.hpp>	// Pascal unit
#include <ThemeSrv.hpp>	// Pascal unit
#include <CommCtrl.hpp>	// Pascal unit
#include <Printers.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <ActiveX.hpp>	// Pascal unit
#include <ImgList.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <MSAAIntf.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------
#include <objidl.h>
#include <oleidl.h>
#include <oleacc.h>

namespace Virtualtrees
{
//-- type declarations -------------------------------------------------------
typedef WideString UnicodeString;

typedef char *PByte;

class DELPHICLASS EVirtualTreeError;
class PASCALIMPLEMENTATION EVirtualTreeError : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EVirtualTreeError(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EVirtualTreeError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EVirtualTreeError(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EVirtualTreeError(int Ident, const System::TVarRec * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EVirtualTreeError(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EVirtualTreeError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EVirtualTreeError(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EVirtualTreeError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EVirtualTreeError(void) { }
	#pragma option pop
	
};


typedef unsigned *PCardinal;

typedef Word TAutoScrollInterval;

#pragma pack(push, 1)
struct TRealWMNCPaint
{
	unsigned Msg;
	HRGN Rgn;
	int lParam;
	int Result;
} ;
#pragma pack(pop)

#pragma pack(push, 1)
struct TWMPrint
{
	unsigned Msg;
	HDC DC;
	unsigned Flags;
	int Result;
} ;
#pragma pack(pop)

typedef TWMPrint  TWMPrintClient;

#pragma option push -b-
enum TVirtualNodeState { vsInitialized, vsChecking, vsCutOrCopy, vsDisabled, vsDeleting, vsExpanded, vsHasChildren, vsVisible, vsSelected, vsInitialUserData, vsAllChildrenHidden, vsClearing, vsMultiline, vsHeightMeasured, vsToggling };
#pragma option pop

typedef Set<TVirtualNodeState, vsInitialized, vsToggling>  TVirtualNodeStates;

#pragma option push -b-
enum TVirtualNodeInitState { ivsDisabled, ivsExpanded, ivsHasChildren, ivsMultiline, ivsSelected };
#pragma option pop

typedef Set<TVirtualNodeInitState, ivsDisabled, ivsSelected>  TVirtualNodeInitStates;

#pragma option push -b-
enum TScrollBarStyle { sbmRegular, sbmFlat, sbm3D };
#pragma option pop

#pragma option push -b-
enum TVTColumnOption { coAllowClick, coDraggable, coEnabled, coParentBidiMode, coParentColor, coResizable, coShowDropMark, coVisible, coAutoSpring, coFixed, coSmartResize, coAllowFocus, coDisableAnimatedResize, coWrapCaption, coUseCaptionAlignment };
#pragma option pop

typedef Set<TVTColumnOption, coAllowClick, coUseCaptionAlignment>  TVTColumnOptions;

#pragma option push -b-
enum TVTHeaderHitPosition { hhiNoWhere, hhiOnColumn, hhiOnIcon, hhiOnCheckbox };
#pragma option pop

typedef Set<TVTHeaderHitPosition, hhiNoWhere, hhiOnCheckbox>  TVTHeaderHitPositions;

#pragma option push -b-
enum THitPosition { hiAbove, hiBelow, hiNowhere, hiOnItem, hiOnItemButton, hiOnItemButtonExact, hiOnItemCheckbox, hiOnItemIndent, hiOnItemLabel, hiOnItemLeft, hiOnItemRight, hiOnNormalIcon, hiOnStateIcon, hiToLeft, hiToRight, hiUpperSplitter, hiLowerSplitter };
#pragma option pop

typedef Set<THitPosition, hiAbove, hiLowerSplitter>  THitPositions;

#pragma option push -b-
enum TCheckType { ctNone, ctTriStateCheckBox, ctCheckBox, ctRadioButton, ctButton };
#pragma option pop

#pragma option push -b-
enum TCheckState { csUncheckedNormal, csUncheckedPressed, csCheckedNormal, csCheckedPressed, csMixedNormal, csMixedPressed };
#pragma option pop

#pragma option push -b-
enum TCheckImageKind { ckLightCheck, ckDarkCheck, ckLightTick, ckDarkTick, ckFlat, ckXP, ckCustom, ckSystemFlat, ckSystemDefault };
#pragma option pop

#pragma option push -b-
enum TVTNodeAttachMode { amNoWhere, amInsertBefore, amInsertAfter, amAddChildFirst, amAddChildLast };
#pragma option pop

#pragma option push -b-
enum TDropMode { dmNowhere, dmAbove, dmOnNode, dmBelow };
#pragma option pop

#pragma option push -b-
enum TDragOperation { doCopy, doMove, doLink };
#pragma option pop

typedef Set<TDragOperation, doCopy, doLink>  TDragOperations;

#pragma option push -b-
enum TVTImageKind { ikNormal, ikSelected, ikState, ikOverlay };
#pragma option pop

#pragma option push -b-
enum TVTHintMode { hmDefault, hmHint, hmHintAndDefault, hmTooltip };
#pragma option pop

#pragma option push -b-
enum TVTTooltipLineBreakStyle { hlbDefault, hlbForceSingleLine, hlbForceMultiLine };
#pragma option pop

typedef Set<Controls::TMouseButton, mbLeft, mbMiddle>  TMouseButtons;

#pragma option push -b-
enum TItemEraseAction { eaColor, eaDefault, eaNone };
#pragma option pop

#pragma option push -b-
enum TVTPaintOption { toHideFocusRect, toHideSelection, toHotTrack, toPopupMode, toShowBackground, toShowButtons, toShowDropmark, toShowHorzGridLines, toShowRoot, toShowTreeLines, toShowVertGridLines, toThemeAware, toUseBlendedImages, toGhostedIfUnfocused, toFullVertGridLines, toAlwaysHideSelection, toUseBlendedSelection, toStaticBackground, toChildrenAbove, toFixedIndent, toUseExplorerTheme };
#pragma option pop

typedef Set<TVTPaintOption, toHideFocusRect, toUseExplorerTheme>  TVTPaintOptions;

#pragma option push -b-
enum TVTAnimationOption { toAnimatedToggle, toAdvancedAnimatedToggle };
#pragma option pop

typedef Set<TVTAnimationOption, toAnimatedToggle, toAdvancedAnimatedToggle>  TVTAnimationOptions;

#pragma option push -b-
enum TVTAutoOption { toAutoDropExpand, toAutoExpand, toAutoScroll, toAutoScrollOnExpand, toAutoSort, toAutoSpanColumns, toAutoTristateTracking, toAutoHideButtons, toAutoDeleteMovedNodes, toDisableAutoscrollOnFocus, toAutoChangeScale, toAutoFreeOnCollapse, toDisableAutoscrollOnEdit, toAutoBidiColumnOrdering };
#pragma option pop

typedef Set<TVTAutoOption, toAutoDropExpand, toAutoBidiColumnOrdering>  TVTAutoOptions;

#pragma option push -b-
enum TVTSelectionOption { toDisableDrawSelection, toExtendedFocus, toFullRowSelect, toLevelSelectConstraint, toMiddleClickSelect, toMultiSelect, toRightClickSelect, toSiblingSelectConstraint, toCenterScrollIntoView, toSimpleDrawSelection };
#pragma option pop

typedef Set<TVTSelectionOption, toDisableDrawSelection, toSimpleDrawSelection>  TVTSelectionOptions;

#pragma option push -b-
enum TVTMiscOption { toAcceptOLEDrop, toCheckSupport, toEditable, toFullRepaintOnResize, toGridExtensions, toInitOnSave, toReportMode, toToggleOnDblClick, toWheelPanning, toReadOnly, toVariableNodeHeight, toFullRowDrag, toNodeHeightResize, toNodeHeightDblClickResize, toEditOnClick, toEditOnDblClick };
#pragma option pop

typedef Set<TVTMiscOption, toAcceptOLEDrop, toEditOnDblClick>  TVTMiscOptions;

#pragma option push -b-
enum TVTExportMode { emAll, emChecked, emUnchecked };
#pragma option pop

typedef TMetaClass*TVirtualTreeClass;

struct TVirtualNode;
typedef TVirtualNode *PVirtualNode;

typedef int TColumnIndex;

typedef unsigned TColumnPosition;

#pragma pack(push, 4)
struct TCacheEntry
{
	TVirtualNode *Node;
	unsigned AbsoluteTop;
} ;
#pragma pack(pop)

typedef DynamicArray<TCacheEntry >  TCache;

typedef DynamicArray<PVirtualNode >  TNodeArray;

class DELPHICLASS TCustomVirtualTreeOptions;
class DELPHICLASS TBaseVirtualTree;
class DELPHICLASS TVTHeader;
class DELPHICLASS TVirtualTreeColumns;
typedef DynamicArray<TColumnIndex >  TIndexArray;

class DELPHICLASS TVirtualTreeColumn;
typedef DynamicArray<TVirtualTreeColumn* >  TColumnsArray;

class PASCALIMPLEMENTATION TVirtualTreeColumns : public Classes::TCollection 
{
	typedef Classes::TCollection inherited;
	
public:
	TVirtualTreeColumn* operator[](TColumnIndex Index) { return Items[Index]; }
	
private:
	TVTHeader* FHeader;
	Graphics::TBitmap* FHeaderBitmap;
	TColumnIndex FHoverIndex;
	TColumnIndex FDownIndex;
	TColumnIndex FTrackIndex;
	TColumnIndex FClickIndex;
	DynamicArray<TColumnIndex >  FPositionToIndex;
	int FDefaultWidth;
	bool FNeedPositionsFix;
	bool FClearing;
	TColumnIndex FDragIndex;
	TColumnIndex FDropTarget;
	bool FDropBefore;
	HIDESBASE int __fastcall GetCount(void);
	HIDESBASE TVirtualTreeColumn* __fastcall GetItem(TColumnIndex Index);
	bool __fastcall GetNewIndex(const Types::TPoint &P, TColumnIndex &OldIndex);
	void __fastcall SetDefaultWidth(int Value);
	HIDESBASE void __fastcall SetItem(TColumnIndex Index, TVirtualTreeColumn* Value);
	
protected:
	void __fastcall AdjustAutoSize(TColumnIndex CurrentIndex, bool Force = false);
	TColumnIndex __fastcall AdjustDownColumn(const Types::TPoint &P);
	bool __fastcall AdjustHoverColumn(const Types::TPoint &P);
	void __fastcall AdjustPosition(TVirtualTreeColumn* Column, unsigned Position);
	bool __fastcall CanSplitterResize(const Types::TPoint &P, TColumnIndex Column);
	void __fastcall DoCanSplitterResize(const Types::TPoint &P, TColumnIndex Column, bool &Allowed);
	void __fastcall DrawButtonText(HDC DC, WideString Caption, const Types::TRect &Bounds, bool Enabled, bool Hot, unsigned DrawFormat, bool WrapCaption);
	void __fastcall DrawXPButton(HDC DC, const Types::TRect &ButtonR, bool DrawSplitter, bool Down, bool Hover);
	void __fastcall FixPositions(void);
	int __fastcall GetColumnAndBounds(const Types::TPoint &P, int &ColumnLeft, int &ColumnRight, bool Relative = true);
	DYNAMIC Classes::TPersistent* __fastcall GetOwner(void);
	virtual void __fastcall HandleClick(const Types::TPoint &P, Controls::TMouseButton Button, bool Force, bool DblClick);
	void __fastcall IndexChanged(int OldIndex, int NewIndex);
	void __fastcall InitializePositionArray(void);
	void __fastcall ReorderColumns(bool RTL);
	virtual void __fastcall Update(Classes::TCollectionItem* Item);
	void __fastcall UpdatePositions(bool Force = false);
	__property Graphics::TBitmap* HeaderBitmap = {read=FHeaderBitmap};
	__property TIndexArray PositionToIndex = {read=FPositionToIndex};
	
public:
	__fastcall TVirtualTreeColumns(TVTHeader* AOwner);
	__fastcall virtual ~TVirtualTreeColumns(void);
	HIDESBASE virtual TVirtualTreeColumn* __fastcall Add(void);
	void __fastcall AnimatedResize(TColumnIndex Column, int NewWidth);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	HIDESBASE virtual void __fastcall Clear(void);
	virtual TColumnIndex __fastcall ColumnFromPosition(const Types::TPoint &P, bool Relative = true)/* overload */;
	virtual TColumnIndex __fastcall ColumnFromPosition(TColumnPosition PositionIndex)/* overload */;
	bool __fastcall Equals(System::TObject* OtherColumnsObj);
	void __fastcall GetColumnBounds(TColumnIndex Column, int &Left, int &Right);
	TColumnIndex __fastcall GetFirstVisibleColumn(bool ConsiderAllowFocus = false);
	TColumnIndex __fastcall GetLastVisibleColumn(bool ConsiderAllowFocus = false);
	TColumnIndex __fastcall GetNextColumn(TColumnIndex Column);
	TColumnIndex __fastcall GetNextVisibleColumn(TColumnIndex Column, bool ConsiderAllowFocus = false);
	TColumnIndex __fastcall GetPreviousColumn(TColumnIndex Column);
	TColumnIndex __fastcall GetPreviousVisibleColumn(TColumnIndex Column, bool ConsiderAllowFocus = false);
	int __fastcall GetScrollWidth(void);
	TColumnsArray __fastcall GetVisibleColumns();
	int __fastcall GetVisibleFixedWidth(void);
	bool __fastcall IsValidColumn(TColumnIndex Column);
	void __fastcall LoadFromStream(const Classes::TStream* Stream, int Version);
	virtual void __fastcall PaintHeader(HDC DC, const Types::TRect &R, int HOffset);
	void __fastcall SaveToStream(const Classes::TStream* Stream);
	int __fastcall TotalWidth(void);
	__property int Count = {read=GetCount, nodefault};
	__property TColumnIndex ClickIndex = {read=FClickIndex, nodefault};
	__property int DefaultWidth = {read=FDefaultWidth, write=SetDefaultWidth, default=50};
	__property TVirtualTreeColumn* Items[TColumnIndex Index] = {read=GetItem, write=SetItem/*, default*/};
	__property TVTHeader* Header = {read=FHeader};
	__property TColumnIndex TrackIndex = {read=FTrackIndex, nodefault};
};


#pragma option push -b-
enum TVTHeaderOption { hoAutoResize, hoColumnResize, hoDblClickResize, hoDrag, hoHotTrack, hoOwnerDraw, hoRestrictDrag, hoShowHint, hoShowImages, hoShowSortGlyphs, hoVisible, hoAutoSpring, hoFullRepaintOnResize, hoDisableAnimatedResize, hoHeightResize, hoHeightDblClickResize };
#pragma option pop

typedef Set<TVTHeaderOption, hoAutoResize, hoHeightDblClickResize>  TVTHeaderOptions;

#pragma option push -b-
enum THeaderState { hsAutoSizing, hsDragging, hsDragPending, hsLoading, hsColumnWidthTracking, hsColumnWidthTrackPending, hsHeightTracking, hsHeightTrackPending, hsResizing, hsScaling, hsNeedScaling };
#pragma option pop

typedef Set<THeaderState, hsAutoSizing, hsNeedScaling>  THeaderStates;

#pragma option push -b-
enum TVTHeaderStyle { hsThickButtons, hsFlatButtons, hsPlates, hsXPStyle };
#pragma option pop

class DELPHICLASS TVTFixedAreaConstraints;
typedef Shortint TVTConstraintPercent;

class PASCALIMPLEMENTATION TVTFixedAreaConstraints : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	TVTHeader* FHeader;
	TVTConstraintPercent FMaxHeightPercent;
	TVTConstraintPercent FMaxWidthPercent;
	TVTConstraintPercent FMinHeightPercent;
	TVTConstraintPercent FMinWidthPercent;
	Classes::TNotifyEvent FOnChange;
	void __fastcall SetConstraints(int Index, TVTConstraintPercent Value);
	
protected:
	void __fastcall Change(void);
	__property TVTHeader* Header = {read=FHeader};
	
public:
	__fastcall TVTFixedAreaConstraints(TVTHeader* AOwner);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	__property Classes::TNotifyEvent OnChange = {read=FOnChange, write=FOnChange};
	
__published:
	__property TVTConstraintPercent MaxHeightPercent = {read=FMaxHeightPercent, write=SetConstraints, index=0, default=0};
	__property TVTConstraintPercent MaxWidthPercent = {read=FMaxWidthPercent, write=SetConstraints, index=1, default=0};
	__property TVTConstraintPercent MinHeightPercent = {read=FMinHeightPercent, write=SetConstraints, index=2, default=0};
	__property TVTConstraintPercent MinWidthPercent = {read=FMinWidthPercent, write=SetConstraints, index=3, default=0};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TVTFixedAreaConstraints(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TSortDirection { sdAscending, sdDescending };
#pragma option pop

class DELPHICLASS TVTDragImage;
typedef Byte TVTTransparency;

typedef Shortint TVTBias;

#pragma option push -b-
enum TVTDragMoveRestriction { dmrNone, dmrHorizontalOnly, dmrVerticalOnly };
#pragma option pop

#pragma option push -b-
enum VirtualTrees__01 { disHidden, disInDrag, disPrepared, disSystemSupport };
#pragma option pop

typedef Set<VirtualTrees__01, disHidden, disSystemSupport>  TVTDragImageStates;

class PASCALIMPLEMENTATION TVTDragImage : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	TBaseVirtualTree* FOwner;
	Graphics::TBitmap* FBackImage;
	Graphics::TBitmap* FAlphaImage;
	Graphics::TBitmap* FDragImage;
	#pragma pack(push, 1)
	Types::TPoint FImagePosition;
	#pragma pack(pop)
	
	#pragma pack(push, 1)
	Types::TPoint FLastPosition;
	#pragma pack(pop)
	
	TVTTransparency FTransparency;
	TVTBias FPreBlendBias;
	TVTBias FPostBlendBias;
	bool FFade;
	TVTDragMoveRestriction FRestriction;
	Graphics::TColor FColorKey;
	TVTDragImageStates FStates;
	bool __fastcall GetVisible(void);
	
protected:
	void __fastcall InternalShowDragImage(HDC ScreenDC);
	void __fastcall MakeAlphaChannel(Graphics::TBitmap* Source, Graphics::TBitmap* Target);
	
public:
	__fastcall TVTDragImage(TBaseVirtualTree* AOwner);
	__fastcall virtual ~TVTDragImage(void);
	bool __fastcall DragTo(const Types::TPoint &P, bool ForceRepaint);
	void __fastcall EndDrag(void);
	Types::TRect __fastcall GetDragImageRect();
	void __fastcall HideDragImage(void);
	void __fastcall PrepareDrag(Graphics::TBitmap* DragImage, const Types::TPoint &ImagePosition, const Types::TPoint &HotSpot, const _di_IDataObject DataObject);
	void __fastcall RecaptureBackground(TBaseVirtualTree* Tree, const Types::TRect &R, HRGN VisibleRegion, bool CaptureNCArea, bool ReshowDragImage);
	void __fastcall ShowDragImage(void);
	bool __fastcall WillMove(const Types::TPoint &P);
	__property Graphics::TColor ColorKey = {read=FColorKey, write=FColorKey, default=-2147483643};
	__property bool Fade = {read=FFade, write=FFade, default=0};
	__property TVTDragMoveRestriction MoveRestriction = {read=FRestriction, write=FRestriction, default=0};
	__property TVTBias PostBlendBias = {read=FPostBlendBias, write=FPostBlendBias, default=0};
	__property TVTBias PreBlendBias = {read=FPreBlendBias, write=FPreBlendBias, default=0};
	__property TVTTransparency Transparency = {read=FTransparency, write=FTransparency, default=128};
	__property bool Visible = {read=GetVisible, nodefault};
};


#pragma option push -b-
enum TSmartAutoFitType { smaAllColumns, smaNoColumn, smaUseColumnOption };
#pragma option pop

typedef TMetaClass*TVirtualTreeColumnsClass;

class PASCALIMPLEMENTATION TVTHeader : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	TBaseVirtualTree* FOwner;
	TVirtualTreeColumns* FColumns;
	int FHeight;
	Graphics::TFont* FFont;
	bool FParentFont;
	TVTHeaderOptions FOptions;
	THeaderStates FStates;
	#pragma pack(push, 1)
	Types::TPoint FTrackPoint;
	#pragma pack(pop)
	
	TVTHeaderStyle FStyle;
	Graphics::TColor FBackground;
	TColumnIndex FAutoSizeIndex;
	Menus::TPopupMenu* FPopupMenu;
	TColumnIndex FMainColumn;
	int FMaxHeight;
	int FMinHeight;
	int FDefaultHeight;
	TVTFixedAreaConstraints* FFixedAreaConstraints;
	Imglist::TCustomImageList* FImages;
	Imglist::TChangeLink* FImageChangeLink;
	TColumnIndex FSortColumn;
	TSortDirection FSortDirection;
	#pragma pack(push, 1)
	Types::TPoint FTrackStart;
	#pragma pack(pop)
	
	#pragma pack(push, 1)
	Types::TPoint FDragStart;
	#pragma pack(pop)
	
	TVTDragImage* FDragImage;
	int FLastWidth;
	void __fastcall FontChanged(System::TObject* Sender);
	TColumnIndex __fastcall GetMainColumn(void);
	bool __fastcall GetUseColumns(void);
	bool __fastcall IsFontStored(void);
	void __fastcall SetAutoSizeIndex(TColumnIndex Value);
	void __fastcall SetBackground(Graphics::TColor Value);
	void __fastcall SetColumns(TVirtualTreeColumns* Value);
	void __fastcall SetDefaultHeight(int Value);
	void __fastcall SetFont(const Graphics::TFont* Value);
	void __fastcall SetHeight(int Value);
	void __fastcall SetImages(const Imglist::TCustomImageList* Value);
	void __fastcall SetMainColumn(TColumnIndex Value);
	void __fastcall SetMaxHeight(int Value);
	void __fastcall SetMinHeight(int Value);
	void __fastcall SetOptions(TVTHeaderOptions Value);
	void __fastcall SetParentFont(bool Value);
	void __fastcall SetSortColumn(TColumnIndex Value);
	void __fastcall SetSortDirection(const TSortDirection Value);
	void __fastcall SetStyle(TVTHeaderStyle Value);
	
protected:
	virtual bool __fastcall CanWriteColumns(void);
	virtual void __fastcall ChangeScale(int M, int D);
	virtual bool __fastcall DetermineSplitterIndex(const Types::TPoint &P);
	virtual void __fastcall DoAfterAutoFitColumn(TColumnIndex Column);
	virtual void __fastcall DoAfterColumnWidthTracking(TColumnIndex Column);
	virtual void __fastcall DoAfterHeightTracking(void);
	virtual bool __fastcall DoBeforeAutoFitColumn(TColumnIndex Column, TSmartAutoFitType SmartAutoFitType);
	virtual void __fastcall DoBeforeColumnWidthTracking(TColumnIndex Column, Classes::TShiftState Shift);
	virtual void __fastcall DoBeforeHeightTracking(Classes::TShiftState Shift);
	virtual bool __fastcall DoColumnWidthDblClickResize(TColumnIndex Column, const Types::TPoint &P, Classes::TShiftState Shift);
	virtual bool __fastcall DoColumnWidthTracking(TColumnIndex Column, Classes::TShiftState Shift, Types::TPoint &TrackPoint, const Types::TPoint &P);
	virtual Menus::TPopupMenu* __fastcall DoGetPopupMenu(TColumnIndex Column, const Types::TPoint &Position);
	virtual bool __fastcall DoHeightTracking(Types::TPoint &P, Classes::TShiftState Shift);
	virtual bool __fastcall DoHeightDblClickResize(Types::TPoint &P, Classes::TShiftState Shift);
	virtual void __fastcall DoSetSortColumn(TColumnIndex Value);
	void __fastcall DragTo(const Types::TPoint &P);
	void __fastcall FixedAreaConstraintsChanged(System::TObject* Sender);
	virtual TMetaClass* __fastcall GetColumnsClass(void);
	DYNAMIC Classes::TPersistent* __fastcall GetOwner(void);
	Classes::TShiftState __fastcall GetShiftState(void);
	bool __fastcall HandleHeaderMouseMove(Messages::TWMMouse &Message);
	virtual bool __fastcall HandleMessage(Messages::TMessage &Message);
	void __fastcall ImageListChange(System::TObject* Sender);
	void __fastcall PrepareDrag(const Types::TPoint &P, const Types::TPoint &Start);
	void __fastcall ReadColumns(Classes::TReader* Reader);
	virtual void __fastcall RecalculateHeader(void);
	void __fastcall RescaleHeader(void);
	void __fastcall UpdateMainColumn(void);
	void __fastcall UpdateSpringColumns(void);
	void __fastcall WriteColumns(Classes::TWriter* Writer);
	
public:
	__fastcall virtual TVTHeader(TBaseVirtualTree* AOwner);
	__fastcall virtual ~TVTHeader(void);
	bool __fastcall AllowFocus(TColumnIndex ColumnIndex);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	void __fastcall AutoFitColumns(bool Animated = true, TSmartAutoFitType SmartAutoFitType = (TSmartAutoFitType)(0x2), int RangeStartCol = 0xffffffff, int RangeEndCol = 0xffffffff);
	virtual bool __fastcall InHeader(const Types::TPoint &P);
	virtual bool __fastcall InHeaderSplitterArea(const Types::TPoint &P);
	void __fastcall Invalidate(TVirtualTreeColumn* Column, bool ExpandToBorder = false);
	virtual void __fastcall LoadFromStream(const Classes::TStream* Stream);
	int __fastcall ResizeColumns(int ChangeBy, TColumnIndex RangeStartCol, TColumnIndex RangeEndCol, TVTColumnOptions Options = (System::Set<TVTColumnOption, coAllowClick, coUseCaptionAlignment> () << TVTColumnOption(7) ));
	void __fastcall RestoreColumns(void);
	virtual void __fastcall SaveToStream(const Classes::TStream* Stream);
	__property TVTDragImage* DragImage = {read=FDragImage};
	__property THeaderStates States = {read=FStates, nodefault};
	__property TBaseVirtualTree* Treeview = {read=FOwner};
	__property bool UseColumns = {read=GetUseColumns, nodefault};
	
__published:
	__property TColumnIndex AutoSizeIndex = {read=FAutoSizeIndex, write=SetAutoSizeIndex, nodefault};
	__property Graphics::TColor Background = {read=FBackground, write=SetBackground, default=-2147483633};
	__property TVirtualTreeColumns* Columns = {read=FColumns, write=SetColumns, stored=false};
	__property int DefaultHeight = {read=FDefaultHeight, write=SetDefaultHeight, nodefault};
	__property Graphics::TFont* Font = {read=FFont, write=SetFont, stored=IsFontStored};
	__property TVTFixedAreaConstraints* FixedAreaConstraints = {read=FFixedAreaConstraints, write=FFixedAreaConstraints};
	__property int Height = {read=FHeight, write=SetHeight, default=17};
	__property Imglist::TCustomImageList* Images = {read=FImages, write=SetImages};
	__property TColumnIndex MainColumn = {read=GetMainColumn, write=SetMainColumn, default=0};
	__property int MaxHeight = {read=FMaxHeight, write=SetMaxHeight, default=10000};
	__property int MinHeight = {read=FMinHeight, write=SetMinHeight, default=10};
	__property TVTHeaderOptions Options = {read=FOptions, write=SetOptions, default=522};
	__property bool ParentFont = {read=FParentFont, write=SetParentFont, default=0};
	__property Menus::TPopupMenu* PopupMenu = {read=FPopupMenu, write=FPopupMenu};
	__property TColumnIndex SortColumn = {read=FSortColumn, write=SetSortColumn, default=-1};
	__property TSortDirection SortDirection = {read=FSortDirection, write=SetSortDirection, default=0};
	__property TVTHeaderStyle Style = {read=FStyle, write=SetStyle, default=0};
};


#pragma option push -b-
enum VirtualTrees__71 { tsCancelHintAnimation, tsChangePending, tsCheckPropagation, tsCollapsing, tsToggleFocusedSelection, tsClearPending, tsClipboardFlushing, tsCopyPending, tsCutPending, tsDrawSelPending, tsDrawSelecting, tsEditing, tsEditPending, tsExpanding, tsNodeHeightTracking, tsNodeHeightTrackPending, tsHint, tsInAnimation, tsIncrementalSearching, tsIncrementalSearchPending, tsIterating, tsKeyCheckPending, tsLeftButtonDown, tsLeftDblClick, tsMouseCheckPending, tsMiddleButtonDown, tsMiddleDblClick, tsNeedScale, tsNeedRootCountUpdate, tsOLEDragging, tsOLEDragPending, tsPainting, tsRightButtonDown, tsRightDblClick, tsPopupMenuShown, tsScrolling, tsScrollPending, tsSizing, tsStopValidation, tsStructureChangePending, tsSynchMode, tsThumbTracking, tsToggling, tsUpdateHiddenChildrenNeeded, tsUpdating, tsUseCache, tsUserDragObject, tsUseThemes, tsValidating, tsValidationNeeded, tsVCLDragging, tsVCLDragPending, tsWheelPanning, tsWheelScrolling, tsWindowCreating };
#pragma option pop

typedef Set<VirtualTrees__71, tsCancelHintAnimation, tsWindowCreating>  TVirtualTreeStates;

#pragma option push -b-
enum VirtualTrees__5 { sdLeft, sdUp, sdRight, sdDown };
#pragma option pop

typedef Set<VirtualTrees__5, sdLeft, sdDown>  TScrollDirections;

#pragma option push -b-
enum TChangeReason { crIgnore, crAccumulated, crChildAdded, crChildDeleted, crNodeAdded, crNodeCopied, crNodeMoved };
#pragma option pop

#pragma option push -b-
enum THintAnimationType { hatNone, hatFade, hatSlide, hatSystemDefault };
#pragma option pop

#pragma pack(push, 4)
struct TVTHintData
{
	TBaseVirtualTree* Tree;
	TVirtualNode *Node;
	TColumnIndex Column;
	Types::TRect HintRect;
	WideString DefaultHint;
	WideString HintText;
	Classes::TBiDiMode BidiMode;
	Classes::TAlignment Alignment;
	TVTTooltipLineBreakStyle LineBreakStyle;
} ;
#pragma pack(pop)

__interface IVTEditLink;
typedef System::DelphiInterface<IVTEditLink> _di_IVTEditLink;
__interface INTERFACE_UUID("{2BE3EAFA-5ACB-45B4-9D9A-B58BCC496E17}") IVTEditLink  : public IInterface 
{
	
public:
	virtual bool __stdcall BeginEdit(void) = 0 ;
	virtual bool __stdcall CancelEdit(void) = 0 ;
	virtual bool __stdcall EndEdit(void) = 0 ;
	virtual bool __stdcall PrepareEdit(TBaseVirtualTree* Tree, PVirtualNode Node, TColumnIndex Column) = 0 ;
	virtual Types::TRect __stdcall GetBounds(void) = 0 ;
	virtual void __stdcall ProcessMessage(Messages::TMessage &Message) = 0 ;
	virtual void __stdcall SetBounds(const Types::TRect R) = 0 ;
};

#pragma option push -b-
enum TVTNodeAlignment { naFromBottom, naFromTop, naProportional };
#pragma option pop

class DELPHICLASS TVTColors;
class PASCALIMPLEMENTATION TVTColors : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	TBaseVirtualTree* FOwner;
	Graphics::TColor FColors[15];
	Graphics::TColor __fastcall GetColor(const int Index);
	void __fastcall SetColor(const int Index, const Graphics::TColor Value);
	
public:
	__fastcall TVTColors(TBaseVirtualTree* AOwner);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	
__published:
	__property Graphics::TColor BorderColor = {read=GetColor, write=SetColor, index=7, default=-2147483633};
	__property Graphics::TColor DisabledColor = {read=GetColor, write=SetColor, index=0, default=-2147483632};
	__property Graphics::TColor DropMarkColor = {read=GetColor, write=SetColor, index=1, default=-2147483635};
	__property Graphics::TColor DropTargetColor = {read=GetColor, write=SetColor, index=2, default=-2147483635};
	__property Graphics::TColor DropTargetBorderColor = {read=GetColor, write=SetColor, index=11, default=-2147483635};
	__property Graphics::TColor FocusedSelectionColor = {read=GetColor, write=SetColor, index=3, default=-2147483635};
	__property Graphics::TColor FocusedSelectionBorderColor = {read=GetColor, write=SetColor, index=9, default=-2147483635};
	__property Graphics::TColor GridLineColor = {read=GetColor, write=SetColor, index=4, default=-2147483633};
	__property Graphics::TColor HeaderHotColor = {read=GetColor, write=SetColor, index=14, default=-2147483632};
	__property Graphics::TColor HotColor = {read=GetColor, write=SetColor, index=8, default=-2147483640};
	__property Graphics::TColor SelectionRectangleBlendColor = {read=GetColor, write=SetColor, index=12, default=-2147483635};
	__property Graphics::TColor SelectionRectangleBorderColor = {read=GetColor, write=SetColor, index=13, default=-2147483635};
	__property Graphics::TColor TreeLineColor = {read=GetColor, write=SetColor, index=5, default=-2147483632};
	__property Graphics::TColor UnfocusedSelectionColor = {read=GetColor, write=SetColor, index=6, default=-2147483633};
	__property Graphics::TColor UnfocusedSelectionBorderColor = {read=GetColor, write=SetColor, index=10, default=-2147483633};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TVTColors(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TVTButtonStyle { bsRectangle, bsTriangle };
#pragma option pop

#pragma option push -b-
enum TVTButtonFillMode { fmTreeColor, fmWindowColor, fmShaded, fmTransparent };
#pragma option pop

#pragma option push -b-
enum TVTLineStyle { lsCustomStyle, lsDotted, lsSolid };
#pragma option pop

#pragma option push -b-
enum TVTLineMode { lmNormal, lmBands };
#pragma option pop

#pragma option push -b-
enum TVTDrawSelectionMode { smDottedRectangle, smBlendedRectangle };
#pragma option pop

#pragma option push -b-
enum TVTDragImageKind { diComplete, diMainColumnOnly, diNoImage };
#pragma option pop

__interface IVTDragManager;
typedef System::DelphiInterface<IVTDragManager> _di_IVTDragManager;
__interface INTERFACE_UUID("{C4B25559-14DA-446B-8901-0C879000EB16}") IVTDragManager  : public IInterface 
{
	
public:
	virtual void __stdcall ForceDragLeave(void) = 0 ;
	virtual _di_IDataObject __stdcall GetDataObject(void) = 0 ;
	virtual TBaseVirtualTree* __stdcall GetDragSource(void) = 0 ;
	virtual bool __stdcall GetDropTargetHelperSupported(void) = 0 ;
	virtual bool __stdcall GetIsDropTarget(void) = 0 ;
	__property _di_IDataObject DataObject = {read=GetDataObject};
	__property TBaseVirtualTree* DragSource = {read=GetDragSource};
	__property bool DropTargetHelperSupported = {read=GetDropTargetHelperSupported};
	__property bool IsDropTarget = {read=GetIsDropTarget};
};

#pragma option push -b-
enum TVTDragType { dtOLE, dtVCL };
#pragma option pop

class DELPHICLASS TClipboardFormats;
class PASCALIMPLEMENTATION TClipboardFormats : public Classes::TStringList 
{
	typedef Classes::TStringList inherited;
	
private:
	TBaseVirtualTree* FOwner;
	
public:
	__fastcall virtual TClipboardFormats(TBaseVirtualTree* AOwner);
	virtual int __fastcall Add(const AnsiString S);
	virtual void __fastcall Insert(int Index, const AnsiString S);
	__property TBaseVirtualTree* Owner = {read=FOwner};
public:
	#pragma option push -w-inl
	/* TStringList.Destroy */ inline __fastcall virtual ~TClipboardFormats(void) { }
	#pragma option pop
	
};


class DELPHICLASS TScrollBarOptions;
typedef Word TVTScrollIncrement;

class PASCALIMPLEMENTATION TScrollBarOptions : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	bool FAlwaysVisible;
	TBaseVirtualTree* FOwner;
	Stdctrls::TScrollStyle FScrollBars;
	TScrollBarStyle FScrollBarStyle;
	TVTScrollIncrement FIncrementX;
	TVTScrollIncrement FIncrementY;
	void __fastcall SetAlwaysVisible(bool Value);
	void __fastcall SetScrollBars(Stdctrls::TScrollStyle Value);
	void __fastcall SetScrollBarStyle(TScrollBarStyle Value);
	
protected:
	DYNAMIC Classes::TPersistent* __fastcall GetOwner(void);
	
public:
	__fastcall TScrollBarOptions(TBaseVirtualTree* AOwner);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	
__published:
	__property bool AlwaysVisible = {read=FAlwaysVisible, write=SetAlwaysVisible, default=0};
	__property TVTScrollIncrement HorizontalIncrement = {read=FIncrementX, write=FIncrementX, default=20};
	__property Stdctrls::TScrollStyle ScrollBars = {read=FScrollBars, write=SetScrollBars, default=3};
	__property TScrollBarStyle ScrollBarStyle = {read=FScrollBarStyle, write=SetScrollBarStyle, default=0};
	__property TVTScrollIncrement VerticalIncrement = {read=FIncrementY, write=FIncrementY, default=20};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TScrollBarOptions(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TVTIncrementalSearch { isAll, isNone, isInitializedOnly, isVisibleOnly };
#pragma option pop

#pragma option push -b-
enum TVTSearchDirection { sdForward, sdBackward };
#pragma option pop

#pragma option push -b-
enum TVTSearchStart { ssAlwaysStartOver, ssLastHit, ssFocusedNode };
#pragma option pop

#pragma option push -b-
enum TVTExportType { etRTF, etHTML, etText, etExcel, etWord, etCustom };
#pragma option pop

typedef bool __fastcall (__closure *TVTNodeExportEvent)(TBaseVirtualTree* Sender, TVTExportType aExportType, PVirtualNode Node);

typedef void __fastcall (__closure *TVTColumnExportEvent)(TBaseVirtualTree* Sender, TVTExportType aExportType, TVirtualTreeColumn* Column);

typedef void __fastcall (__closure *TVTTreeExportEvent)(TBaseVirtualTree* Sender, TVTExportType aExportType);

typedef void __fastcall (__closure *TVTChangeEvent)(TBaseVirtualTree* Sender, PVirtualNode Node);

typedef void __fastcall (__closure *TVTStructureChangeEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TChangeReason Reason);

typedef void __fastcall (__closure *TVTInitChildrenEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, unsigned &ChildCount);

typedef void __fastcall (__closure *TVTInitNodeEvent)(TBaseVirtualTree* Sender, PVirtualNode ParentNode, PVirtualNode Node, TVirtualNodeInitStates &InitialStates);

typedef void __fastcall (__closure *TVTFreeNodeEvent)(TBaseVirtualTree* Sender, PVirtualNode Node);

typedef void __fastcall (__closure *TVTGetImageEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column, bool &Ghosted, int &ImageIndex);

typedef void __fastcall (__closure *TVTGetImageExEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column, bool &Ghosted, int &ImageIndex, Imglist::TCustomImageList* &ImageList);

typedef void __fastcall (__closure *TVTGetImageTextEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column, WideString &ImageText);

typedef void __fastcall (__closure *TVTHotNodeChangeEvent)(TBaseVirtualTree* Sender, PVirtualNode OldNode, PVirtualNode NewNode);

typedef void __fastcall (__closure *TVTChangingEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, bool &Allowed);

typedef void __fastcall (__closure *TVTCheckChangingEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TCheckState &NewState, bool &Allowed);

typedef void __fastcall (__closure *TVTNodeMovingEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, PVirtualNode Target, bool &Allowed);

typedef void __fastcall (__closure *TVTNodeMovedEvent)(TBaseVirtualTree* Sender, PVirtualNode Node);

typedef void __fastcall (__closure *TVTNodeCopyingEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, PVirtualNode Target, bool &Allowed);

typedef void __fastcall (__closure *TVTNodeHeightTrackingEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, Classes::TShiftState Shift, Types::TPoint &TrackPoint, const Types::TPoint &P, bool &Allowed);

typedef void __fastcall (__closure *TVTNodeHeightDblClickResizeEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, Classes::TShiftState Shift, const Types::TPoint &P, bool &Allowed);

typedef void __fastcall (__closure *TVTNodeCopiedEvent)(TBaseVirtualTree* Sender, PVirtualNode Node);

typedef void __fastcall (__closure *TVTEditChangingEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, bool &Allowed);

typedef void __fastcall (__closure *TVTEditCancelEvent)(TBaseVirtualTree* Sender, TColumnIndex Column);

typedef void __fastcall (__closure *TVTEditChangeEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column);

typedef void __fastcall (__closure *TVTFocusChangingEvent)(TBaseVirtualTree* Sender, PVirtualNode OldNode, PVirtualNode NewNode, TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);

typedef void __fastcall (__closure *TVTFocusChangeEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column);

typedef void __fastcall (__closure *TVTPopupEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, const Types::TPoint &P, bool &AskParent, Menus::TPopupMenu* &PopupMenu);

typedef void __fastcall (__closure *TVTHelpContextEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, int &HelpContext);

typedef void __fastcall (__closure *TVTCreateEditorEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, /* out */ _di_IVTEditLink &EditLink);

typedef void __fastcall (__closure *TVTSaveNodeEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, Classes::TStream* Stream);

typedef void __fastcall (__closure *TVTAfterAutoFitColumnEvent)(TVTHeader* Sender, TColumnIndex Column);

typedef void __fastcall (__closure *TVTAfterAutoFitColumnsEvent)(TVTHeader* Sender);

typedef void __fastcall (__closure *TVTBeforeAutoFitColumnsEvent)(TVTHeader* Sender, TSmartAutoFitType &SmartAutoFitType);

typedef void __fastcall (__closure *TVTBeforeAutoFitColumnEvent)(TVTHeader* Sender, TColumnIndex Column, TSmartAutoFitType &SmartAutoFitType, bool &Allowed);

typedef void __fastcall (__closure *TVTHeaderClickEvent)(TVTHeader* Sender, TColumnIndex Column, Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);

typedef void __fastcall (__closure *TVTAfterHeaderHeightTrackingEvent)(TVTHeader* Sender);

typedef void __fastcall (__closure *TVTBeforeHeaderHeightTrackingEvent)(TVTHeader* Sender, Classes::TShiftState Shift);

typedef void __fastcall (__closure *TVTHeaderHeightTrackingEvent)(TVTHeader* Sender, Types::TPoint &P, Classes::TShiftState Shift, bool &Allowed);

typedef void __fastcall (__closure *TVTHeaderHeightDblClickResizeEvent)(TVTHeader* Sender, Types::TPoint &P, Classes::TShiftState Shift, bool &Allowed);

typedef void __fastcall (__closure *TVTHeaderMouseEvent)(TVTHeader* Sender, Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);

typedef void __fastcall (__closure *TVTHeaderMouseMoveEvent)(TVTHeader* Sender, Classes::TShiftState Shift, int X, int Y);

typedef void __fastcall (__closure *TVTAfterGetMaxColumnWidthEvent)(TVTHeader* Sender, TColumnIndex Column, int &MaxWidth);

typedef void __fastcall (__closure *TVTBeforeGetMaxColumnWidthEvent)(TVTHeader* Sender, TColumnIndex Column, bool &UseSmartColumnWidth);

typedef void __fastcall (__closure *TVTColumnClickEvent)(TBaseVirtualTree* Sender, TColumnIndex Column, Classes::TShiftState Shift);

typedef void __fastcall (__closure *TVTColumnDblClickEvent)(TBaseVirtualTree* Sender, TColumnIndex Column, Classes::TShiftState Shift);

typedef void __fastcall (__closure *TVTHeaderNotifyEvent)(TVTHeader* Sender, TColumnIndex Column);

typedef void __fastcall (__closure *TVTColumnWidthDblClickResizeEvent)(TVTHeader* Sender, TColumnIndex Column, Classes::TShiftState Shift, const Types::TPoint &P, bool &Allowed);

typedef void __fastcall (__closure *TVTAfterColumnWidthTrackingEvent)(TVTHeader* Sender, TColumnIndex Column);

typedef void __fastcall (__closure *TVTBeforeColumnWidthTrackingEvent)(TVTHeader* Sender, TColumnIndex Column, Classes::TShiftState Shift);

typedef void __fastcall (__closure *TVTColumnWidthTrackingEvent)(TVTHeader* Sender, TColumnIndex Column, Classes::TShiftState Shift, Types::TPoint &TrackPoint, const Types::TPoint &P, bool &Allowed);

typedef void __fastcall (__closure *TVTGetHeaderCursorEvent)(TVTHeader* Sender, HICON &Cursor);

typedef void __fastcall (__closure *TVTCanSplitterResizeColumnEvent)(TVTHeader* Sender, const Types::TPoint &P, TColumnIndex Column, bool &Allowed);

typedef void __fastcall (__closure *TVTPaintEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas);

typedef void __fastcall (__closure *TVTAfterItemPaintEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas, PVirtualNode Node, const Types::TRect &ItemRect);

typedef void __fastcall (__closure *TVTBeforeItemPaintEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas, PVirtualNode Node, const Types::TRect &ItemRect, bool &CustomDraw);

typedef void __fastcall (__closure *TVTBeforeItemEraseEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas, PVirtualNode Node, const Types::TRect &ItemRect, Graphics::TColor &ItemColor, TItemEraseAction &EraseAction);

typedef void __fastcall (__closure *TVTAfterItemEraseEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas, PVirtualNode Node, const Types::TRect &ItemRect);

typedef void __fastcall (__closure *TVTAfterCellPaintEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas, PVirtualNode Node, TColumnIndex Column, const Types::TRect &CellRect);

#pragma option push -b-
enum TVTCellPaintMode { cpmPaint, cpmGetContentMargin };
#pragma option pop

typedef void __fastcall (__closure *TVTBeforeCellPaintEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas, PVirtualNode Node, TColumnIndex Column, TVTCellPaintMode CellPaintMode, const Types::TRect &CellRect, Types::TRect &ContentRect);

#pragma option push -b-
enum TVTDropMarkMode { dmmNone, dmmLeft, dmmRight };
#pragma option pop

typedef void __fastcall (__closure *TVTHeaderPaintEvent)(TVTHeader* Sender, Graphics::TCanvas* HeaderCanvas, TVirtualTreeColumn* Column, const Types::TRect &R, bool Hover, bool Pressed, TVTDropMarkMode DropMark);

struct THeaderPaintInfo;
#pragma option push -b-
enum VirtualTrees__61 { hpeBackground, hpeDropMark, hpeHeaderGlyph, hpeSortGlyph, hpeText };
#pragma option pop

typedef Set<VirtualTrees__61, hpeBackground, hpeText>  THeaderPaintElements;

typedef void __fastcall (__closure *TVTHeaderPaintQueryElementsEvent)(TVTHeader* Sender, THeaderPaintInfo &PaintInfo, THeaderPaintElements &Elements);

typedef void __fastcall (__closure *TVTAdvancedHeaderPaintEvent)(TVTHeader* Sender, THeaderPaintInfo &PaintInfo, const THeaderPaintElements Elements);

typedef void __fastcall (__closure *TVTGetLineStyleEvent)(TBaseVirtualTree* Sender, void * &Bits);

typedef void __fastcall (__closure *TVTBackgroundPaintEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas, const Types::TRect &R, bool &Handled);

typedef void __fastcall (__closure *TVTMeasureItemEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas, PVirtualNode Node, int &NodeHeight);

typedef void __fastcall (__closure *TVTCreateDragManagerEvent)(TBaseVirtualTree* Sender, /* out */ _di_IVTDragManager &DragManager);

typedef void __fastcall (__closure *TVTCreateDataObjectEvent)(TBaseVirtualTree* Sender, /* out */ _di_IDataObject &IDataObject);

typedef void __fastcall (__closure *TVTDragAllowedEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, bool &Allowed);

typedef void __fastcall (__closure *TVTDragOverEvent)(TBaseVirtualTree* Sender, System::TObject* Source, Classes::TShiftState Shift, Controls::TDragState State, const Types::TPoint &Pt, TDropMode Mode, int &Effect, bool &Accept);

typedef DynamicArray<Word >  TFormatArray;

typedef void __fastcall (__closure *TVTDragDropEvent)(TBaseVirtualTree* Sender, System::TObject* Source, _di_IDataObject DataObject, TFormatArray Formats, Classes::TShiftState Shift, const Types::TPoint &Pt, int &Effect, TDropMode Mode);

typedef void __fastcall (__closure *TVTHeaderDraggedEvent)(TVTHeader* Sender, TColumnIndex Column, int OldPosition);

typedef void __fastcall (__closure *TVTHeaderDraggedOutEvent)(TVTHeader* Sender, TColumnIndex Column, const Types::TPoint &DropPosition);

typedef void __fastcall (__closure *TVTHeaderDraggingEvent)(TVTHeader* Sender, TColumnIndex Column, bool &Allowed);

typedef void __fastcall (__closure *TVTRenderOLEDataEvent)(TBaseVirtualTree* Sender, const tagFORMATETC &FormatEtcIn, /* out */ tagSTGMEDIUM &Medium, bool ForClipboard, HRESULT &Result);

typedef DynamicArray<tagFORMATETC >  TFormatEtcArray;

typedef void __fastcall (__closure *TVTGetUserClipboardFormatsEvent)(TBaseVirtualTree* Sender, TFormatEtcArray &Formats);

typedef void __fastcall (__closure *TVTGetNodeDataSizeEvent)(TBaseVirtualTree* Sender, int &NodeDataSize);

typedef void __fastcall (__closure *TVTKeyActionEvent)(TBaseVirtualTree* Sender, Word &CharCode, Classes::TShiftState &Shift, bool &DoDefault);

typedef void __fastcall (__closure *TVTScrollEvent)(TBaseVirtualTree* Sender, int DeltaX, int DeltaY);

#pragma option push -b-
enum TVTUpdateState { usBegin, usBeginSynch, usSynch, usUpdate, usEnd, usEndSynch };
#pragma option pop

typedef void __fastcall (__closure *TVTUpdatingEvent)(TBaseVirtualTree* Sender, TVTUpdateState State);

typedef void __fastcall (__closure *TVTGetCursorEvent)(TBaseVirtualTree* Sender, Controls::TCursor &Cursor);

typedef void __fastcall (__closure *TVTStateChangeEvent)(TBaseVirtualTree* Sender, const TVirtualTreeStates &Enter, const TVirtualTreeStates &Leave);

typedef void __fastcall (__closure *TVTGetCellIsEmptyEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, bool &IsEmpty);

typedef void __fastcall (__closure *TVTScrollbarShowEvent)(TBaseVirtualTree* Sender, int Bar, bool Show);

typedef void __fastcall (__closure *TVTCompareEvent)(TBaseVirtualTree* Sender, PVirtualNode Node1, PVirtualNode Node2, TColumnIndex Column, int &Result);

typedef void __fastcall (__closure *TVTIncrementalSearchEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, const WideString SearchText, int &Result);

struct TVTPaintInfo;
struct TVTImageInfo;

#pragma option push -b-
enum TVTLineType { ltNone, ltBottomRight, ltTopDown, ltTopDownRight, ltRight, ltTopRight, ltLeft, ltLeftBottom };
#pragma option pop
typedef DynamicArray<TVTLineType >  TLineImage;

typedef bool __fastcall (__closure *TVTAnimationCallback)(int Step, int StepSize, void * Data);

#pragma pack(push, 4)
struct THitInfo
{
	TVirtualNode *HitNode;
	THitPositions HitPositions;
	TColumnIndex HitColumn;
} ;
#pragma pack(pop)

#pragma option push -b-
enum TVTCellContentMarginType { ccmtAllSides, ccmtTopLeftOnly, ccmtBottomRightOnly };
#pragma option pop

#pragma option push -b-
enum VirtualTrees__12 { suoRepaintHeader, suoRepaintScrollbars, suoScrollClientArea, suoUpdateNCArea };
#pragma option pop

typedef Set<VirtualTrees__12, suoRepaintHeader, suoUpdateNCArea>  TScrollUpdateOptions;

typedef TMetaClass*TVirtualTreeColumnClass;

typedef TMetaClass*TVTHeaderClass;

#pragma option push -b-
enum TVTImageInfoIndex { iiNormal, iiState, iiCheck, iiOverlay };
#pragma option pop

typedef TMetaClass*TTreeOptionsClass;

typedef void __fastcall (__closure *TVTGetNodeProc)(TBaseVirtualTree* Sender, PVirtualNode Node, void * Data, bool &Abort);

#pragma option push -b-
enum TVTInternalPaintOption { poBackground, poColumnColor, poDrawFocusRect, poDrawSelection, poDrawDropMark, poGridLines, poMainOnly, poSelectedOnly };
#pragma option pop

typedef Set<TVTInternalPaintOption, poBackground, poSelectedOnly>  TVTInternalPaintOptions;

class PASCALIMPLEMENTATION TBaseVirtualTree : public Controls::TCustomControl 
{
	typedef Controls::TCustomControl inherited;
	
private:
	Forms::TFormBorderStyle FBorderStyle;
	TVTHeader* FHeader;
	TVirtualNode *FRoot;
	unsigned FDefaultNodeHeight;
	unsigned FIndent;
	TCustomVirtualTreeOptions* FOptions;
	unsigned FUpdateCount;
	unsigned FSynchUpdateCount;
	int FNodeDataSize;
	TVirtualTreeStates FStates;
	TVirtualNode *FLastSelected;
	TVirtualNode *FFocusedNode;
	TColumnIndex FEditColumn;
	TColumnIndex FFocusedColumn;
	#pragma pack(push, 1)
	Types::TPoint FHeightTrackPoint;
	#pragma pack(pop)
	
	TVirtualNode *FHeightTrackNode;
	TColumnIndex FHeightTrackColumn;
	TScrollDirections FScrollDirections;
	TChangeReason FLastStructureChangeReason;
	TVirtualNode *FLastStructureChangeNode;
	TVirtualNode *FLastChangedNode;
	TVirtualNode *FCurrentHotNode;
	TColumnIndex FCurrentHotColumn;
	bool FHotNodeButtonHit;
	#pragma pack(push, 1)
	Types::TRect FLastSelRect;
	#pragma pack(pop)
	
	#pragma pack(push, 1)
	Types::TRect FNewSelRect;
	#pragma pack(pop)
	
	Controls::TCursor FHotCursor;
	THintAnimationType FAnimationType;
	TVTHintMode FHintMode;
	TVTHintData FHintData;
	unsigned FChangeDelay;
	unsigned FEditDelay;
	DynamicArray<TCacheEntry >  FPositionCache;
	unsigned FVisibleCount;
	unsigned FStartIndex;
	DynamicArray<PVirtualNode >  FSelection;
	int FSelectionCount;
	bool FSelectionLocked;
	TVirtualNode *FRangeAnchor;
	TVirtualNode *FCheckNode;
	TCheckState FPendingCheckState;
	unsigned FCheckPropagationCount;
	int FLastSelectionLevel;
	Classes::TShiftState FDrawSelShiftState;
	_di_IVTEditLink FEditLink;
	DynamicArray<PVirtualNode >  FTempNodeCache;
	unsigned FTempNodeCount;
	Graphics::TPicture* FBackground;
	int FMargin;
	int FTextMargin;
	int FBackgroundOffsetX;
	int FBackgroundOffsetY;
	unsigned FAnimationDuration;
	bool FWantTabs;
	TVTNodeAlignment FNodeAlignment;
	#pragma pack(push, 1)
	Types::TRect FHeaderRect;
	#pragma pack(pop)
	
	#pragma pack(push, 1)
	Types::TRect FLastHintRect;
	#pragma pack(pop)
	
	#pragma pack(push, 1)
	Types::TRect FUpdateRect;
	#pragma pack(pop)
	
	Graphics::TBitmap* FPlusBM;
	Graphics::TBitmap* FMinusBM;
	Graphics::TBitmap* FHotPlusBM;
	Graphics::TBitmap* FHotMinusBM;
	Imglist::TCustomImageList* FImages;
	Imglist::TCustomImageList* FStateImages;
	Imglist::TCustomImageList* FCustomCheckImages;
	TCheckImageKind FCheckImageKind;
	Imglist::TCustomImageList* FCheckImages;
	Imglist::TChangeLink* FImageChangeLink;
	Imglist::TChangeLink* FStateChangeLink;
	Imglist::TChangeLink* FCustomCheckChangeLink;
	Classes::TNotifyEvent FOldFontChange;
	bool FFontChanged;
	TVTColors* FColors;
	TVTButtonStyle FButtonStyle;
	TVTButtonFillMode FButtonFillMode;
	TVTLineStyle FLineStyle;
	TVTLineMode FLineMode;
	HBRUSH FDottedBrush;
	unsigned FSelectionCurveRadius;
	Byte FSelectionBlendFactor;
	TVTDrawSelectionMode FDrawSelectionMode;
	Classes::TAlignment FAlignment;
	TVTDragImageKind FDragImageKind;
	TDragOperations FDragOperations;
	int FDragThreshold;
	_di_IVTDragManager FDragManager;
	TVirtualNode *FDropTargetNode;
	TDropMode FLastDropMode;
	DynamicArray<PVirtualNode >  FDragSelection;
	TVTDragType FDragType;
	TVTDragImage* FDragImage;
	int FDragWidth;
	int FDragHeight;
	TClipboardFormats* FClipboardFormats;
	TVirtualNode *FLastVCLDragTarget;
	int FVCLDragEffect;
	TScrollBarOptions* FScrollBarOptions;
	TAutoScrollInterval FAutoScrollInterval;
	unsigned FAutoScrollDelay;
	unsigned FAutoExpandDelay;
	int FOffsetX;
	int FOffsetY;
	int FEffectiveOffsetX;
	unsigned FRangeX;
	unsigned FRangeY;
	unsigned FBottomSpace;
	TVTNodeAttachMode FDefaultPasteMode;
	DynamicArray<PVirtualNode >  FSingletonNodeArray;
	unsigned FDragScrollStart;
	TVTIncrementalSearch FIncrementalSearch;
	unsigned FSearchTimeout;
	WideString FSearchBuffer;
	TVirtualNode *FLastSearchNode;
	TVTSearchDirection FSearchDirection;
	TVTSearchStart FSearchStart;
	unsigned FTotalInternalDataSize;
	HWND FPanningWindow;
	HICON FPanningCursor;
	Graphics::TBitmap* FPanningImage;
	#pragma pack(push, 1)
	Types::TPoint FLastClickPos;
	#pragma pack(pop)
	
	unsigned FOperationCount;
	bool FOperationCanceled;
	_di_IAccessible FAccessible;
	_di_IAccessible FAccessibleItem;
	AnsiString FAccessibleName;
	TVTNodeExportEvent FOnBeforeNodeExport;
	TVTNodeExportEvent FOnNodeExport;
	TVTNodeExportEvent FOnAfterNodeExport;
	TVTColumnExportEvent FOnBeforeColumnExport;
	TVTColumnExportEvent FOnColumnExport;
	TVTColumnExportEvent FOnAfterColumnExport;
	TVTTreeExportEvent FOnBeforeTreeExport;
	TVTTreeExportEvent FOnAfterTreeExport;
	TVTTreeExportEvent FOnBeforeHeaderExport;
	TVTTreeExportEvent FOnAfterHeaderExport;
	TVTChangeEvent FOnChange;
	TVTStructureChangeEvent FOnStructureChange;
	TVTInitChildrenEvent FOnInitChildren;
	TVTInitNodeEvent FOnInitNode;
	TVTFreeNodeEvent FOnFreeNode;
	TVTGetImageEvent FOnGetImage;
	TVTGetImageExEvent FOnGetImageEx;
	TVTGetImageTextEvent FOnGetImageText;
	TVTHotNodeChangeEvent FOnHotChange;
	TVTChangingEvent FOnExpanding;
	TVTChangingEvent FOnCollapsing;
	TVTCheckChangingEvent FOnChecking;
	TVTChangeEvent FOnExpanded;
	TVTChangeEvent FOnCollapsed;
	TVTChangeEvent FOnChecked;
	TVTChangeEvent FOnResetNode;
	TVTNodeMovingEvent FOnNodeMoving;
	TVTNodeMovedEvent FOnNodeMoved;
	TVTNodeCopyingEvent FOnNodeCopying;
	TVTNodeHeightTrackingEvent FOnNodeHeightTracking;
	TVTNodeHeightDblClickResizeEvent FOnNodeHeightDblClickResize;
	TVTNodeCopiedEvent FOnNodeCopied;
	TVTEditChangingEvent FOnEditing;
	TVTEditCancelEvent FOnEditCancelled;
	TVTEditChangeEvent FOnEdited;
	TVTFocusChangingEvent FOnFocusChanging;
	TVTFocusChangeEvent FOnFocusChanged;
	TVTPopupEvent FOnGetPopupMenu;
	TVTHelpContextEvent FOnGetHelpContext;
	TVTCreateEditorEvent FOnCreateEditor;
	TVTSaveNodeEvent FOnLoadNode;
	TVTSaveNodeEvent FOnSaveNode;
	TVTAfterAutoFitColumnEvent FOnAfterAutoFitColumn;
	TVTAfterAutoFitColumnsEvent FOnAfterAutoFitColumns;
	TVTBeforeAutoFitColumnsEvent FOnBeforeAutoFitColumns;
	TVTBeforeAutoFitColumnEvent FOnBeforeAutoFitColumn;
	TVTHeaderClickEvent FOnHeaderClick;
	TVTHeaderClickEvent FOnHeaderImageClick;
	TVTHeaderClickEvent FOnHeaderCheckBoxClick;
	TVTHeaderClickEvent FOnHeaderDblClick;
	TVTAfterHeaderHeightTrackingEvent FOnAfterHeaderHeightTracking;
	TVTBeforeHeaderHeightTrackingEvent FOnBeforeHeaderHeightTracking;
	TVTHeaderHeightTrackingEvent FOnHeaderHeightTracking;
	TVTHeaderHeightDblClickResizeEvent FOnHeaderHeightDblClickResize;
	TVTHeaderMouseEvent FOnHeaderMouseDown;
	TVTHeaderMouseEvent FOnHeaderMouseUp;
	TVTHeaderMouseMoveEvent FOnHeaderMouseMove;
	TVTAfterGetMaxColumnWidthEvent FOnAfterGetMaxColumnWidth;
	TVTBeforeGetMaxColumnWidthEvent FOnBeforeGetMaxColumnWidth;
	TVTColumnClickEvent FOnColumnClick;
	TVTColumnDblClickEvent FOnColumnDblClick;
	TVTHeaderNotifyEvent FOnColumnResize;
	TVTColumnWidthDblClickResizeEvent FOnColumnWidthDblClickResize;
	TVTAfterColumnWidthTrackingEvent FOnAfterColumnWidthTracking;
	TVTBeforeColumnWidthTrackingEvent FOnBeforeColumnWidthTracking;
	TVTColumnWidthTrackingEvent FOnColumnWidthTracking;
	TVTGetHeaderCursorEvent FOnGetHeaderCursor;
	TVTCanSplitterResizeColumnEvent FOnCanSplitterResizeColumn;
	TVTPaintEvent FOnAfterPaint;
	TVTPaintEvent FOnBeforePaint;
	TVTAfterItemPaintEvent FOnAfterItemPaint;
	TVTBeforeItemPaintEvent FOnBeforeItemPaint;
	TVTBeforeItemEraseEvent FOnBeforeItemErase;
	TVTAfterItemEraseEvent FOnAfterItemErase;
	TVTAfterCellPaintEvent FOnAfterCellPaint;
	TVTBeforeCellPaintEvent FOnBeforeCellPaint;
	TVTHeaderPaintEvent FOnHeaderDraw;
	TVTHeaderPaintQueryElementsEvent FOnHeaderDrawQueryElements;
	TVTAdvancedHeaderPaintEvent FOnAdvancedHeaderDraw;
	TVTGetLineStyleEvent FOnGetLineStyle;
	TVTBackgroundPaintEvent FOnPaintBackground;
	TVTMeasureItemEvent FOnMeasureItem;
	TVTCreateDragManagerEvent FOnCreateDragManager;
	TVTCreateDataObjectEvent FOnCreateDataObject;
	TVTDragAllowedEvent FOnDragAllowed;
	TVTDragOverEvent FOnDragOver;
	TVTDragDropEvent FOnDragDrop;
	TVTHeaderDraggedEvent FOnHeaderDragged;
	TVTHeaderDraggedOutEvent FOnHeaderDraggedOut;
	TVTHeaderDraggingEvent FOnHeaderDragging;
	TVTRenderOLEDataEvent FOnRenderOLEData;
	TVTGetUserClipboardFormatsEvent FOnGetUserClipboardFormats;
	TVTGetNodeDataSizeEvent FOnGetNodeDataSize;
	TVTKeyActionEvent FOnKeyAction;
	TVTScrollEvent FOnScroll;
	TVTUpdatingEvent FOnUpdating;
	TVTGetCursorEvent FOnGetCursor;
	TVTStateChangeEvent FOnStateChange;
	TVTGetCellIsEmptyEvent FOnGetCellIsEmpty;
	TVTScrollbarShowEvent FOnShowScrollbar;
	TVTCompareEvent FOnCompareNodes;
	TVTIncrementalSearchEvent FOnIncrementalSearch;
	void __fastcall AdjustCoordinatesByIndent(TVTPaintInfo &PaintInfo, int Indent);
	void __fastcall AdjustImageBorder(Imglist::TCustomImageList* Images, Classes::TBiDiMode BidiMode, int VAlign, Types::TRect &R, TVTImageInfo &ImageInfo);
	void __fastcall AdjustTotalCount(PVirtualNode Node, int Value, bool relative = false);
	void __fastcall AdjustTotalHeight(PVirtualNode Node, int Value, bool relative = false);
	int __fastcall CalculateCacheEntryCount(void);
	void __fastcall CalculateVerticalAlignments(bool ShowImages, bool ShowStateImages, PVirtualNode Node, int &VAlign, int &VButtonAlign);
	bool __fastcall ChangeCheckState(PVirtualNode Node, TCheckState Value);
	bool __fastcall CollectSelectedNodesLTR(int MainColumn, int NodeLeft, int NodeRight, Classes::TAlignment Alignment, const Types::TRect &OldRect, const Types::TRect &NewRect);
	bool __fastcall CollectSelectedNodesRTL(int MainColumn, int NodeLeft, int NodeRight, Classes::TAlignment Alignment, const Types::TRect &OldRect, const Types::TRect &NewRect);
	void __fastcall ClearNodeBackground(const TVTPaintInfo &PaintInfo, bool UseBackground, bool Floating, const Types::TRect &R);
	int __fastcall CompareNodePositions(PVirtualNode Node1, PVirtualNode Node2, bool ConsiderChildrenAbove = false);
	int __fastcall DetermineLineImageAndSelectLevel(PVirtualNode Node, TLineImage &LineImage);
	void __fastcall DrawLineImage(const TVTPaintInfo &PaintInfo, int X, int Y, int H, int VAlign, TVTLineType Style, bool Reverse);
	PVirtualNode __fastcall FindInPositionCache(PVirtualNode Node, unsigned &CurrentPos)/* overload */;
	PVirtualNode __fastcall FindInPositionCache(unsigned Position, unsigned &CurrentPos)/* overload */;
	void __fastcall FixupTotalCount(PVirtualNode Node);
	void __fastcall FixupTotalHeight(PVirtualNode Node);
	PVirtualNode __fastcall GetBottomNode(void);
	int __fastcall GetCheckedCount(void);
	TCheckState __fastcall GetCheckState(PVirtualNode Node);
	TCheckType __fastcall GetCheckType(PVirtualNode Node);
	unsigned __fastcall GetChildCount(PVirtualNode Node);
	bool __fastcall GetChildrenInitialized(PVirtualNode Node);
	int __fastcall GetCutCopyCount(void);
	bool __fastcall GetDisabled(PVirtualNode Node);
	_di_IVTDragManager __fastcall GetDragManager();
	bool __fastcall GetExpanded(PVirtualNode Node);
	bool __fastcall GetFullyVisible(PVirtualNode Node);
	bool __fastcall GetHasChildren(PVirtualNode Node);
	bool __fastcall GetMultiline(PVirtualNode Node);
	unsigned __fastcall GetNodeHeight(PVirtualNode Node);
	PVirtualNode __fastcall GetNodeParent(PVirtualNode Node);
	Types::TPoint __fastcall GetOffsetXY();
	unsigned __fastcall GetRootNodeCount(void);
	bool __fastcall GetSelected(PVirtualNode Node);
	PVirtualNode __fastcall GetTopNode(void);
	unsigned __fastcall GetTotalCount(void);
	Byte __fastcall GetVerticalAlignment(PVirtualNode Node);
	bool __fastcall GetVisible(PVirtualNode Node);
	bool __fastcall GetVisiblePath(PVirtualNode Node);
	void __fastcall HandleClickSelection(PVirtualNode LastFocused, PVirtualNode NewNode, Classes::TShiftState Shift, bool DragPending);
	bool __fastcall HandleDrawSelection(int X, int Y);
	bool __fastcall HasVisibleNextSibling(PVirtualNode Node);
	bool __fastcall HasVisiblePreviousSibling(PVirtualNode Node);
	void __fastcall ImageListChange(System::TObject* Sender);
	void __fastcall InitializeFirstColumnValues(TVTPaintInfo &PaintInfo);
	void __fastcall InitRootNode(unsigned OldSize = (unsigned)(0x0));
	void __fastcall InterruptValidation(void);
	bool __fastcall IsFirstVisibleChild(PVirtualNode Parent, PVirtualNode Node);
	bool __fastcall IsLastVisibleChild(PVirtualNode Parent, PVirtualNode Node);
	void __fastcall LimitPaintingToArea(Graphics::TCanvas* Canvas, const Types::TRect &ClipRect, HRGN VisibleRegion = (HRGN)(0x0));
	PVirtualNode __fastcall MakeNewNode(void);
	int __fastcall PackArray(TNodeArray TheArray, int Count);
	void __fastcall PrepareBitmaps(bool NeedButtons, bool NeedLines);
	void __fastcall ReadOldOptions(Classes::TReader* Reader);
	void __fastcall SetAlignment(const Classes::TAlignment Value);
	void __fastcall SetAnimationDuration(const unsigned Value);
	void __fastcall SetBackground(const Graphics::TPicture* Value);
	void __fastcall SetBackgroundOffset(const int Index, const int Value);
	void __fastcall SetBorderStyle(Forms::TBorderStyle Value);
	void __fastcall SetBottomNode(PVirtualNode Node);
	void __fastcall SetBottomSpace(const unsigned Value);
	void __fastcall SetButtonFillMode(const TVTButtonFillMode Value);
	void __fastcall SetButtonStyle(const TVTButtonStyle Value);
	void __fastcall SetCheckImageKind(TCheckImageKind Value);
	void __fastcall SetCheckState(PVirtualNode Node, TCheckState Value);
	void __fastcall SetCheckType(PVirtualNode Node, TCheckType Value);
	void __fastcall SetChildCount(PVirtualNode Node, unsigned NewChildCount);
	void __fastcall SetClipboardFormats(const TClipboardFormats* Value);
	void __fastcall SetColors(const TVTColors* Value);
	void __fastcall SetCustomCheckImages(const Imglist::TCustomImageList* Value);
	void __fastcall SetDefaultNodeHeight(unsigned Value);
	void __fastcall SetDisabled(PVirtualNode Node, bool Value);
	void __fastcall SetExpanded(PVirtualNode Node, bool Value);
	void __fastcall SetFocusedColumn(TColumnIndex Value);
	void __fastcall SetFocusedNode(PVirtualNode Value);
	void __fastcall SetFullyVisible(PVirtualNode Node, bool Value);
	void __fastcall SetHasChildren(PVirtualNode Node, bool Value);
	void __fastcall SetHeader(const TVTHeader* Value);
	void __fastcall SetImages(const Imglist::TCustomImageList* Value);
	void __fastcall SetIndent(unsigned Value);
	void __fastcall SetLineMode(const TVTLineMode Value);
	void __fastcall SetLineStyle(const TVTLineStyle Value);
	void __fastcall SetMargin(int Value);
	void __fastcall SetMultiline(PVirtualNode Node, const bool Value);
	void __fastcall SetNodeAlignment(const TVTNodeAlignment Value);
	void __fastcall SetNodeDataSize(int Value);
	void __fastcall SetNodeHeight(PVirtualNode Node, unsigned Value);
	void __fastcall SetNodeParent(PVirtualNode Node, const PVirtualNode Value);
	void __fastcall SetOffsetX(const int Value);
	void __fastcall SetOffsetXY(const Types::TPoint &Value);
	void __fastcall SetOffsetY(const int Value);
	void __fastcall SetOptions(const TCustomVirtualTreeOptions* Value);
	void __fastcall SetRootNodeCount(unsigned Value);
	void __fastcall SetScrollBarOptions(TScrollBarOptions* Value);
	void __fastcall SetSearchOption(const TVTIncrementalSearch Value);
	void __fastcall SetSelected(PVirtualNode Node, bool Value);
	void __fastcall SetSelectionCurveRadius(const unsigned Value);
	void __fastcall SetStateImages(const Imglist::TCustomImageList* Value);
	void __fastcall SetTextMargin(int Value);
	void __fastcall SetTopNode(PVirtualNode Node);
	void __fastcall SetUpdateState(bool Updating);
	void __fastcall SetVerticalAlignment(PVirtualNode Node, Byte Value);
	HIDESBASE void __fastcall SetVisible(PVirtualNode Node, bool Value);
	void __fastcall SetVisiblePath(PVirtualNode Node, bool Value);
	void __fastcall StaticBackground(Graphics::TBitmap* Source, Graphics::TCanvas* Target, const Types::TPoint &Offset, const Types::TRect &R);
	void __fastcall StopTimer(int ID);
	void __fastcall TileBackground(Graphics::TBitmap* Source, Graphics::TCanvas* Target, const Types::TPoint &Offset, const Types::TRect &R);
	bool __fastcall ToggleCallback(int Step, int StepSize, void * Data);
	MESSAGE void __fastcall CMColorChange(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMCtl3DChanged(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMBiDiModeChanged(Messages::TMessage &Message);
	MESSAGE void __fastcall CMDenySubclassing(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMDrag(Controls::TCMDrag &Message);
	HIDESBASE MESSAGE void __fastcall CMEnabledChanged(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMFontChanged(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMHintShow(Forms::TCMHintShow &Message);
	MESSAGE void __fastcall CMHintShowPause(Forms::TCMHintShowPause &Message);
	HIDESBASE MESSAGE void __fastcall CMMouseLeave(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMMouseWheel(Controls::TCMMouseWheel &Message);
	HIDESBASE MESSAGE void __fastcall CMSysColorChange(Messages::TMessage &Message);
	MESSAGE void __fastcall TVMGetItem(Messages::TMessage &Message);
	MESSAGE void __fastcall TVMGetItemRect(Messages::TMessage &Message);
	MESSAGE void __fastcall TVMGetNextItem(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMCancelMode(Messages::TWMNoParams &Message);
	MESSAGE void __fastcall WMChangeState(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMChar(Messages::TWMKey &Message);
	HIDESBASE MESSAGE void __fastcall WMContextMenu(Messages::TWMContextMenu &Message);
	MESSAGE void __fastcall WMCopy(Messages::TWMNoParams &Message);
	MESSAGE void __fastcall WMCut(Messages::TWMNoParams &Message);
	MESSAGE void __fastcall WMEnable(Messages::TWMEnable &Message);
	HIDESBASE MESSAGE void __fastcall WMEraseBkgnd(Messages::TWMEraseBkgnd &Message);
	MESSAGE void __fastcall WMGetDlgCode(Messages::TWMNoParams &Message);
	MESSAGE void __fastcall WMGetObject(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMHScroll(Messages::TWMScroll &Message);
	HIDESBASE MESSAGE void __fastcall WMKeyDown(Messages::TWMKey &Message);
	HIDESBASE MESSAGE void __fastcall WMKeyUp(Messages::TWMKey &Message);
	HIDESBASE MESSAGE void __fastcall WMKillFocus(Messages::TWMKillFocus &Msg);
	HIDESBASE MESSAGE void __fastcall WMLButtonDblClk(Messages::TWMMouse &Message);
	HIDESBASE MESSAGE void __fastcall WMLButtonDown(Messages::TWMMouse &Message);
	HIDESBASE MESSAGE void __fastcall WMLButtonUp(Messages::TWMMouse &Message);
	HIDESBASE MESSAGE void __fastcall WMMButtonDblClk(Messages::TWMMouse &Message);
	HIDESBASE MESSAGE void __fastcall WMMButtonDown(Messages::TWMMouse &Message);
	HIDESBASE MESSAGE void __fastcall WMMButtonUp(Messages::TWMMouse &Message);
	HIDESBASE MESSAGE void __fastcall WMNCCalcSize(Messages::TWMNCCalcSize &Message);
	HIDESBASE MESSAGE void __fastcall WMNCDestroy(Messages::TWMNoParams &Message);
	HIDESBASE MESSAGE void __fastcall WMNCHitTest(Messages::TWMNCHitTest &Message);
	HIDESBASE MESSAGE void __fastcall WMNCPaint(TRealWMNCPaint &Message);
	HIDESBASE MESSAGE void __fastcall WMPaint(Messages::TWMPaint &Message);
	MESSAGE void __fastcall WMPaste(Messages::TWMNoParams &Message);
	MESSAGE void __fastcall WMPrint(TWMPrint &Message);
	MESSAGE void __fastcall WMPrintClient(TWMPrint &Message);
	HIDESBASE MESSAGE void __fastcall WMRButtonDblClk(Messages::TWMMouse &Message);
	HIDESBASE MESSAGE void __fastcall WMRButtonDown(Messages::TWMMouse &Message);
	HIDESBASE MESSAGE void __fastcall WMRButtonUp(Messages::TWMMouse &Message);
	HIDESBASE MESSAGE void __fastcall WMSetCursor(Messages::TWMSetCursor &Message);
	HIDESBASE MESSAGE void __fastcall WMSetFocus(Messages::TWMSetFocus &Msg);
	HIDESBASE MESSAGE void __fastcall WMSize(Messages::TWMSize &Message);
	MESSAGE void __fastcall WMTimer(Messages::TWMTimer &Message);
	MESSAGE void __fastcall WMThemeChanged(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMVScroll(Messages::TWMScroll &Message);
	
protected:
	virtual void __fastcall AddToSelection(PVirtualNode Node)/* overload */;
	virtual void __fastcall AddToSelection(const TNodeArray NewItems, int NewLength, bool ForceInsert = false)/* overload */;
	virtual void __fastcall AdjustPaintCellRect(TVTPaintInfo &PaintInfo, TColumnIndex &NextNonEmpty);
	virtual void __fastcall AdjustPanningCursor(int X, int Y);
	virtual void __fastcall AdviseChangeEvent(bool StructureChange, PVirtualNode Node, TChangeReason Reason);
	virtual unsigned __fastcall AllocateInternalDataArea(unsigned Size);
	virtual void __fastcall Animate(unsigned Steps, unsigned Duration, TVTAnimationCallback Callback, void * Data);
	void __fastcall BeginOperation(void);
	virtual bool __fastcall CalculateSelectionRect(int X, int Y);
	virtual bool __fastcall CanAutoScroll(void);
	virtual bool __fastcall CanShowDragImage(void);
	virtual void __fastcall Change(PVirtualNode Node);
	DYNAMIC void __fastcall ChangeScale(int M, int D);
	virtual bool __fastcall CheckParentCheckState(PVirtualNode Node, TCheckState NewCheckState);
	virtual void __fastcall ClearTempCache(void);
	virtual bool __fastcall ColumnIsEmpty(PVirtualNode Node, TColumnIndex Column);
	virtual int __fastcall ComputeRTLOffset(bool ExcludeScrollbar = false);
	virtual int __fastcall CountLevelDifference(PVirtualNode Node1, PVirtualNode Node2);
	virtual unsigned __fastcall CountVisibleChildren(PVirtualNode Node);
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	virtual void __fastcall CreateWnd(void);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	virtual void __fastcall DetermineHiddenChildrenFlag(PVirtualNode Node);
	virtual void __fastcall DetermineHiddenChildrenFlagAllNodes(void);
	virtual void __fastcall DetermineHitPositionLTR(THitInfo &HitInfo, int Offset, int Right, Classes::TAlignment Alignment);
	virtual void __fastcall DetermineHitPositionRTL(THitInfo &HitInfo, int Offset, int Right, Classes::TAlignment Alignment);
	virtual TCheckState __fastcall DetermineNextCheckState(TCheckType CheckType, TCheckState CheckState);
	virtual TScrollDirections __fastcall DetermineScrollDirections(int X, int Y);
	virtual void __fastcall DoAdvancedHeaderDraw(THeaderPaintInfo &PaintInfo, const THeaderPaintElements Elements);
	virtual void __fastcall DoAfterCellPaint(Graphics::TCanvas* Canvas, PVirtualNode Node, TColumnIndex Column, const Types::TRect &CellRect);
	virtual void __fastcall DoAfterItemErase(Graphics::TCanvas* Canvas, PVirtualNode Node, const Types::TRect &ItemRect);
	virtual void __fastcall DoAfterItemPaint(Graphics::TCanvas* Canvas, PVirtualNode Node, const Types::TRect &ItemRect);
	virtual void __fastcall DoAfterPaint(Graphics::TCanvas* Canvas);
	virtual void __fastcall DoAutoScroll(int X, int Y);
	virtual bool __fastcall DoBeforeDrag(PVirtualNode Node, TColumnIndex Column);
	virtual void __fastcall DoBeforeCellPaint(Graphics::TCanvas* Canvas, PVirtualNode Node, TColumnIndex Column, TVTCellPaintMode CellPaintMode, const Types::TRect &CellRect, Types::TRect &ContentRect);
	virtual void __fastcall DoBeforeItemErase(Graphics::TCanvas* Canvas, PVirtualNode Node, const Types::TRect &ItemRect, Graphics::TColor &Color, TItemEraseAction &EraseAction);
	virtual bool __fastcall DoBeforeItemPaint(Graphics::TCanvas* Canvas, PVirtualNode Node, const Types::TRect &ItemRect);
	virtual void __fastcall DoBeforePaint(Graphics::TCanvas* Canvas);
	virtual bool __fastcall DoCancelEdit(void);
	virtual void __fastcall DoCanEdit(PVirtualNode Node, TColumnIndex Column, bool &Allowed);
	virtual void __fastcall DoChange(PVirtualNode Node);
	virtual void __fastcall DoCheckClick(PVirtualNode Node, TCheckState NewCheckState);
	virtual void __fastcall DoChecked(PVirtualNode Node);
	virtual bool __fastcall DoChecking(PVirtualNode Node, TCheckState &NewCheckState);
	virtual void __fastcall DoCollapsed(PVirtualNode Node);
	virtual bool __fastcall DoCollapsing(PVirtualNode Node);
	virtual void __fastcall DoColumnClick(TColumnIndex Column, Classes::TShiftState Shift);
	virtual void __fastcall DoColumnDblClick(TColumnIndex Column, Classes::TShiftState Shift);
	virtual void __fastcall DoColumnResize(TColumnIndex Column);
	virtual int __fastcall DoCompare(PVirtualNode Node1, PVirtualNode Node2, TColumnIndex Column);
	virtual _di_IDataObject __fastcall DoCreateDataObject();
	virtual _di_IVTDragManager __fastcall DoCreateDragManager();
	virtual _di_IVTEditLink __fastcall DoCreateEditor(PVirtualNode Node, TColumnIndex Column);
	virtual void __fastcall DoDragging(const Types::TPoint &P);
	virtual void __fastcall DoDragExpand(void);
	virtual bool __fastcall DoDragOver(System::TObject* Source, Classes::TShiftState Shift, Controls::TDragState State, const Types::TPoint &Pt, TDropMode Mode, int &Effect);
	virtual void __fastcall DoDragDrop(System::TObject* Source, _di_IDataObject DataObject, TFormatArray Formats, Classes::TShiftState Shift, const Types::TPoint &Pt, int &Effect, TDropMode Mode);
	virtual void __fastcall DoEdit(void);
	DYNAMIC void __fastcall DoEndDrag(System::TObject* Target, int X, int Y);
	virtual bool __fastcall DoEndEdit(void);
	virtual void __fastcall DoExpanded(PVirtualNode Node);
	virtual bool __fastcall DoExpanding(PVirtualNode Node);
	virtual void __fastcall DoFocusChange(PVirtualNode Node, TColumnIndex Column);
	virtual bool __fastcall DoFocusChanging(PVirtualNode OldNode, PVirtualNode NewNode, TColumnIndex OldColumn, TColumnIndex NewColumn);
	virtual void __fastcall DoFocusNode(PVirtualNode Node, bool Ask);
	virtual void __fastcall DoFreeNode(PVirtualNode Node);
	virtual THintAnimationType __fastcall DoGetAnimationType(void);
	virtual Types::TPoint __fastcall DoGetCellContentMargin(PVirtualNode Node, TColumnIndex Column, TVTCellContentMarginType CellContentMarginType = (TVTCellContentMarginType)(0x0), Graphics::TCanvas* Canvas = (Graphics::TCanvas*)(0x0));
	virtual void __fastcall DoGetCursor(Controls::TCursor &Cursor);
	virtual void __fastcall DoGetHeaderCursor(HICON &Cursor);
	virtual Imglist::TCustomImageList* __fastcall DoGetImageIndex(PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column, bool &Ghosted, int &Index);
	virtual void __fastcall DoGetImageText(PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column, WideString &ImageText);
	virtual void __fastcall DoGetLineStyle(void * &Bits);
	virtual WideString __fastcall DoGetNodeHint(PVirtualNode Node, TColumnIndex Column, TVTTooltipLineBreakStyle &LineBreakStyle);
	virtual WideString __fastcall DoGetNodeTooltip(PVirtualNode Node, TColumnIndex Column, TVTTooltipLineBreakStyle &LineBreakStyle);
	virtual int __fastcall DoGetNodeWidth(PVirtualNode Node, TColumnIndex Column, Graphics::TCanvas* Canvas = (Graphics::TCanvas*)(0x0));
	virtual Menus::TPopupMenu* __fastcall DoGetPopupMenu(PVirtualNode Node, TColumnIndex Column, const Types::TPoint &Position);
	virtual void __fastcall DoGetUserClipboardFormats(TFormatEtcArray &Formats);
	void __fastcall DoHeaderClick(TColumnIndex Column, Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	void __fastcall DoHeaderDblClick(TColumnIndex Column, Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	void __fastcall DoHeaderImageClick(TColumnIndex Column, Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	void __fastcall DoHeaderCheckBoxClick(TColumnIndex Column, Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall DoHeaderDragged(TColumnIndex Column, TColumnPosition OldPosition);
	virtual void __fastcall DoHeaderDraggedOut(TColumnIndex Column, const Types::TPoint &DropPosition);
	virtual bool __fastcall DoHeaderDragging(TColumnIndex Column);
	virtual void __fastcall DoHeaderDraw(Graphics::TCanvas* Canvas, TVirtualTreeColumn* Column, const Types::TRect &R, bool Hover, bool Pressed, TVTDropMarkMode DropMark);
	virtual void __fastcall DoHeaderDrawQueryElements(THeaderPaintInfo &PaintInfo, THeaderPaintElements &Elements);
	virtual void __fastcall DoHeaderMouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall DoHeaderMouseMove(Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall DoHeaderMouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall DoHotChange(PVirtualNode Old, PVirtualNode New);
	virtual int __fastcall DoIncrementalSearch(PVirtualNode Node, const WideString Text);
	virtual void __fastcall DoInitChildren(PVirtualNode Node, unsigned &ChildCount);
	virtual void __fastcall DoInitNode(PVirtualNode Parent, PVirtualNode Node, TVirtualNodeInitStates &InitStates);
	virtual bool __fastcall DoKeyAction(Word &CharCode, Classes::TShiftState &Shift);
	virtual void __fastcall DoLoadUserData(PVirtualNode Node, Classes::TStream* Stream);
	virtual void __fastcall DoMeasureItem(Graphics::TCanvas* TargetCanvas, PVirtualNode Node, int &NodeHeight);
	virtual void __fastcall DoNodeCopied(PVirtualNode Node);
	virtual bool __fastcall DoNodeCopying(PVirtualNode Node, PVirtualNode NewParent);
	virtual bool __fastcall DoNodeHeightDblClickResize(PVirtualNode Node, TColumnIndex Column, Classes::TShiftState Shift, const Types::TPoint &P);
	virtual bool __fastcall DoNodeHeightTracking(PVirtualNode Node, TColumnIndex Column, Classes::TShiftState Shift, Types::TPoint &TrackPoint, const Types::TPoint &P);
	virtual void __fastcall DoNodeMoved(PVirtualNode Node);
	virtual bool __fastcall DoNodeMoving(PVirtualNode Node, PVirtualNode NewParent);
	virtual bool __fastcall DoPaintBackground(Graphics::TCanvas* Canvas, const Types::TRect &R);
	virtual void __fastcall DoPaintDropMark(Graphics::TCanvas* Canvas, PVirtualNode Node, const Types::TRect &R);
	virtual void __fastcall DoPaintNode(TVTPaintInfo &PaintInfo);
	virtual void __fastcall DoPopupMenu(PVirtualNode Node, TColumnIndex Column, const Types::TPoint &Position);
	virtual HRESULT __fastcall DoRenderOLEData(const tagFORMATETC &FormatEtcIn, /* out */ tagSTGMEDIUM &Medium, bool ForClipboard);
	virtual void __fastcall DoReset(PVirtualNode Node);
	virtual void __fastcall DoSaveUserData(PVirtualNode Node, Classes::TStream* Stream);
	virtual void __fastcall DoScroll(int DeltaX, int DeltaY);
	virtual bool __fastcall DoSetOffsetXY(const Types::TPoint &Value, TScrollUpdateOptions Options, Types::PRect ClipRect = (void *)(0x0));
	virtual void __fastcall DoShowScrollbar(int Bar, bool Show);
	DYNAMIC void __fastcall DoStartDrag(Controls::TDragObject* &DragObject);
	virtual void __fastcall DoStateChange(const TVirtualTreeStates &Enter, const TVirtualTreeStates &Leave = System::Set<VirtualTrees__71, tsCancelHintAnimation, tsWindowCreating> () );
	virtual void __fastcall DoStructureChange(PVirtualNode Node, TChangeReason Reason);
	virtual void __fastcall DoTimerScroll(void);
	virtual void __fastcall DoUpdating(TVTUpdateState State);
	virtual bool __fastcall DoValidateCache(void);
	virtual void __fastcall DragAndDrop(int AllowedEffects, _di_IDataObject DataObject, int DragEffect);
	DYNAMIC void __fastcall DragCanceled(void);
	HIDESBASE virtual HRESULT __fastcall DragDrop(const _di_IDataObject DataObject, int KeyState, const Types::TPoint &Pt, int &Effect);
	virtual HRESULT __fastcall DragEnter(int KeyState, const Types::TPoint &Pt, int &Effect);
	virtual void __fastcall DragFinished(void);
	virtual void __fastcall DragLeave(void);
	HIDESBASE virtual HRESULT __fastcall DragOver(System::TObject* Source, int KeyState, Controls::TDragState DragState, const Types::TPoint &Pt, int &Effect);
	virtual void __fastcall DrawDottedHLine(const TVTPaintInfo &PaintInfo, int Left, int Right, int Top);
	virtual void __fastcall DrawDottedVLine(const TVTPaintInfo &PaintInfo, int Top, int Bottom, int Left);
	void __fastcall EndOperation(void);
	virtual bool __fastcall FindNodeInSelection(PVirtualNode P, int &Index, int LowBound, int HighBound);
	virtual void __fastcall FinishChunkHeader(Classes::TStream* Stream, int StartPos, int EndPos);
	HIDESBASE virtual void __fastcall FontChanged(System::TObject* AFont);
	virtual tagSIZE __fastcall GetBorderDimensions();
	virtual int __fastcall GetCheckImage(PVirtualNode Node, TCheckType ImgCheckType = (TCheckType)(0x0), TCheckState ImgCheckState = (TCheckState)(0x0), bool ImgEnabled = false);
	/* virtual class method */ virtual Imglist::TCustomImageList* __fastcall GetCheckImageListFor(TMetaClass* vmt, TCheckImageKind Kind);
	virtual TMetaClass* __fastcall GetColumnClass(void);
	virtual TMetaClass* __fastcall GetHeaderClass(void);
	virtual TMetaClass* __fastcall GetHintWindowClass(void);
	virtual void __fastcall GetImageIndex(TVTPaintInfo &Info, TVTImageKind Kind, TVTImageInfoIndex InfoIndex, Imglist::TCustomImageList* DefaultImages);
	virtual unsigned __fastcall GetMaxRightExtend(void);
	virtual void __fastcall GetNativeClipboardFormats(TFormatEtcArray &Formats);
	bool __fastcall GetOperationCanceled(void);
	virtual TMetaClass* __fastcall GetOptionsClass(void);
	virtual TBaseVirtualTree* __fastcall GetTreeFromDataObject(const _di_IDataObject DataObject);
	virtual void __fastcall HandleHotTrack(int X, int Y);
	virtual void __fastcall HandleIncrementalSearch(Word CharCode);
	virtual void __fastcall HandleMouseDblClick(Messages::TWMMouse &Message, const THitInfo &HitInfo);
	virtual void __fastcall HandleMouseDown(Messages::TWMMouse &Message, THitInfo &HitInfo);
	virtual void __fastcall HandleMouseUp(Messages::TWMMouse &Message, const THitInfo &HitInfo);
	virtual bool __fastcall HasImage(PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column);
	virtual bool __fastcall HasPopupMenu(PVirtualNode Node, TColumnIndex Column, const Types::TPoint &Pos);
	virtual void __fastcall InitChildren(PVirtualNode Node);
	virtual void __fastcall InitNode(PVirtualNode Node);
	virtual void __fastcall InternalAddFromStream(Classes::TStream* Stream, int Version, PVirtualNode Node);
	bool __fastcall InternalAddToSelection(PVirtualNode Node, bool ForceInsert)/* overload */;
	bool __fastcall InternalAddToSelection(const TNodeArray NewItems, int NewLength, bool ForceInsert)/* overload */;
	virtual void __fastcall InternalCacheNode(PVirtualNode Node);
	virtual void __fastcall InternalClearSelection(void);
	virtual void __fastcall InternalConnectNode(PVirtualNode Node, PVirtualNode Destination, TBaseVirtualTree* Target, TVTNodeAttachMode Mode);
	void * __fastcall InternalData(PVirtualNode Node);
	virtual void __fastcall InternalDisconnectNode(PVirtualNode Node, bool KeepFocus, bool Reindex = true);
	virtual void __fastcall InternalRemoveFromSelection(PVirtualNode Node);
	void __fastcall InvalidateCache(void);
	virtual void __fastcall Loaded(void);
	virtual void __fastcall MainColumnChanged(void);
	virtual void __fastcall MarkCutCopyNodes(void);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	virtual void __fastcall OriginalWMNCPaint(HDC DC);
	virtual void __fastcall Paint(void);
	virtual void __fastcall PaintCheckImage(const TVTPaintInfo &PaintInfo);
	virtual void __fastcall PaintImage(TVTPaintInfo &PaintInfo, TVTImageInfoIndex ImageInfoIndex, bool DoOverlay);
	virtual void __fastcall PaintNodeButton(Graphics::TCanvas* Canvas, PVirtualNode Node, TColumnIndex Column, const Types::TRect &R, int ButtonX, int ButtonY, Classes::TBiDiMode BidiMode);
	virtual void __fastcall PaintTreeLines(const TVTPaintInfo &PaintInfo, int VAlignment, int IndentSize, TLineImage LineImage);
	virtual void __fastcall PaintSelectionRectangle(Graphics::TCanvas* Target, int WindowOrgX, const Types::TRect &SelectionRect, const Types::TRect &TargetRect);
	virtual void __fastcall PanningWindowProc(Messages::TMessage &Message);
	virtual void __fastcall PrepareCell(TVTPaintInfo &PaintInfo, int WindowOrgX, int MaxWidth);
	virtual bool __fastcall ReadChunk(Classes::TStream* Stream, int Version, PVirtualNode Node, int ChunkType, int ChunkSize);
	virtual void __fastcall ReadNode(Classes::TStream* Stream, int Version, PVirtualNode Node);
	virtual void __fastcall RedirectFontChangeEvent(Graphics::TCanvas* Canvas);
	virtual void __fastcall RemoveFromSelection(PVirtualNode Node);
	virtual HRESULT __fastcall RenderOLEData(const tagFORMATETC &FormatEtcIn, /* out */ tagSTGMEDIUM &Medium, bool ForClipboard);
	virtual void __fastcall ResetRangeAnchor(void);
	virtual void __fastcall RestoreFontChangeEvent(Graphics::TCanvas* Canvas);
	virtual void __fastcall SelectNodes(PVirtualNode StartNode, PVirtualNode EndNode, bool AddOnly);
	virtual void __fastcall SetFocusedNodeAndColumn(PVirtualNode Node, TColumnIndex Column);
	virtual void __fastcall SkipNode(Classes::TStream* Stream);
	virtual void __fastcall StartWheelPanning(const Types::TPoint &Position);
	virtual void __fastcall StopWheelPanning(void);
	virtual void __fastcall StructureChange(PVirtualNode Node, TChangeReason Reason);
	virtual int __fastcall SuggestDropEffect(System::TObject* Source, Classes::TShiftState Shift, const Types::TPoint &Pt, int AllowedEffects);
	virtual void __fastcall ToggleSelection(PVirtualNode StartNode, PVirtualNode EndNode);
	virtual void __fastcall UnselectNodes(PVirtualNode StartNode, PVirtualNode EndNode);
	void __fastcall UpdateColumnCheckState(TVirtualTreeColumn* Col);
	virtual void __fastcall UpdateDesigner(void);
	virtual void __fastcall UpdateEditBounds(void);
	virtual void __fastcall UpdateHeaderRect(void);
	virtual void __fastcall UpdateWindowAndDragImage(const TBaseVirtualTree* Tree, const Types::TRect &TreeRect, bool UpdateNCArea, bool ReshowDragImage);
	virtual void __fastcall ValidateCache(void);
	virtual void __fastcall ValidateNodeDataSize(int &Size);
	virtual void __fastcall WndProc(Messages::TMessage &Message);
	virtual void __fastcall WriteChunks(Classes::TStream* Stream, PVirtualNode Node);
	virtual void __fastcall WriteNode(Classes::TStream* Stream, PVirtualNode Node);
	__property Classes::TAlignment Alignment = {read=FAlignment, write=SetAlignment, default=0};
	__property unsigned AnimationDuration = {read=FAnimationDuration, write=SetAnimationDuration, default=200};
	__property unsigned AutoExpandDelay = {read=FAutoExpandDelay, write=FAutoExpandDelay, default=1000};
	__property unsigned AutoScrollDelay = {read=FAutoScrollDelay, write=FAutoScrollDelay, default=1000};
	__property TAutoScrollInterval AutoScrollInterval = {read=FAutoScrollInterval, write=FAutoScrollInterval, default=1};
	__property Graphics::TPicture* Background = {read=FBackground, write=SetBackground};
	__property int BackgroundOffsetX = {read=FBackgroundOffsetX, write=SetBackgroundOffset, index=0, default=0};
	__property int BackgroundOffsetY = {read=FBackgroundOffsetY, write=SetBackgroundOffset, index=1, default=0};
	__property Forms::TBorderStyle BorderStyle = {read=FBorderStyle, write=SetBorderStyle, default=1};
	__property unsigned BottomSpace = {read=FBottomSpace, write=SetBottomSpace, default=0};
	__property TVTButtonFillMode ButtonFillMode = {read=FButtonFillMode, write=SetButtonFillMode, default=0};
	__property TVTButtonStyle ButtonStyle = {read=FButtonStyle, write=SetButtonStyle, default=0};
	__property unsigned ChangeDelay = {read=FChangeDelay, write=FChangeDelay, default=0};
	__property TCheckImageKind CheckImageKind = {read=FCheckImageKind, write=SetCheckImageKind, default=8};
	__property TClipboardFormats* ClipboardFormats = {read=FClipboardFormats, write=SetClipboardFormats};
	__property TVTColors* Colors = {read=FColors, write=SetColors};
	__property Imglist::TCustomImageList* CustomCheckImages = {read=FCustomCheckImages, write=SetCustomCheckImages};
	__property unsigned DefaultNodeHeight = {read=FDefaultNodeHeight, write=SetDefaultNodeHeight, default=18};
	__property TVTNodeAttachMode DefaultPasteMode = {read=FDefaultPasteMode, write=FDefaultPasteMode, default=4};
	__property int DragHeight = {read=FDragHeight, write=FDragHeight, default=350};
	__property TVTDragImageKind DragImageKind = {read=FDragImageKind, write=FDragImageKind, default=0};
	__property TDragOperations DragOperations = {read=FDragOperations, write=FDragOperations, default=3};
	__property TNodeArray DragSelection = {read=FDragSelection};
	__property TVTDragType DragType = {read=FDragType, write=FDragType, default=0};
	__property int DragWidth = {read=FDragWidth, write=FDragWidth, default=200};
	__property TVTDrawSelectionMode DrawSelectionMode = {read=FDrawSelectionMode, write=FDrawSelectionMode, default=0};
	__property TColumnIndex EditColumn = {read=FEditColumn, write=FEditColumn, nodefault};
	__property unsigned EditDelay = {read=FEditDelay, write=FEditDelay, default=1000};
	__property TVTHeader* Header = {read=FHeader, write=SetHeader};
	__property Types::TRect HeaderRect = {read=FHeaderRect};
	__property THintAnimationType HintAnimation = {read=FAnimationType, write=FAnimationType, default=3};
	__property TVTHintMode HintMode = {read=FHintMode, write=FHintMode, default=0};
	__property Controls::TCursor HotCursor = {read=FHotCursor, write=FHotCursor, default=0};
	__property Imglist::TCustomImageList* Images = {read=FImages, write=SetImages};
	__property TVTIncrementalSearch IncrementalSearch = {read=FIncrementalSearch, write=SetSearchOption, default=1};
	__property TVTSearchDirection IncrementalSearchDirection = {read=FSearchDirection, write=FSearchDirection, default=0};
	__property TVTSearchStart IncrementalSearchStart = {read=FSearchStart, write=FSearchStart, default=2};
	__property unsigned IncrementalSearchTimeout = {read=FSearchTimeout, write=FSearchTimeout, default=1000};
	__property unsigned Indent = {read=FIndent, write=SetIndent, default=18};
	__property Types::TPoint LastClickPos = {read=FLastClickPos, write=FLastClickPos};
	__property TDropMode LastDropMode = {read=FLastDropMode, write=FLastDropMode, nodefault};
	__property TVTLineMode LineMode = {read=FLineMode, write=SetLineMode, default=0};
	__property TVTLineStyle LineStyle = {read=FLineStyle, write=SetLineStyle, default=1};
	__property int Margin = {read=FMargin, write=SetMargin, default=4};
	__property TVTNodeAlignment NodeAlignment = {read=FNodeAlignment, write=SetNodeAlignment, default=2};
	__property int NodeDataSize = {read=FNodeDataSize, write=SetNodeDataSize, default=-1};
	__property bool OperationCanceled = {read=GetOperationCanceled, nodefault};
	__property unsigned RootNodeCount = {read=GetRootNodeCount, write=SetRootNodeCount, default=0};
	__property TScrollBarOptions* ScrollBarOptions = {read=FScrollBarOptions, write=SetScrollBarOptions};
	__property Byte SelectionBlendFactor = {read=FSelectionBlendFactor, write=FSelectionBlendFactor, default=128};
	__property unsigned SelectionCurveRadius = {read=FSelectionCurveRadius, write=SetSelectionCurveRadius, default=0};
	__property Imglist::TCustomImageList* StateImages = {read=FStateImages, write=SetStateImages};
	__property int TextMargin = {read=FTextMargin, write=SetTextMargin, default=4};
	__property unsigned TotalInternalDataSize = {read=FTotalInternalDataSize, nodefault};
	__property TCustomVirtualTreeOptions* TreeOptions = {read=FOptions, write=SetOptions};
	__property bool WantTabs = {read=FWantTabs, write=FWantTabs, default=0};
	__property TVTAdvancedHeaderPaintEvent OnAdvancedHeaderDraw = {read=FOnAdvancedHeaderDraw, write=FOnAdvancedHeaderDraw};
	__property TVTAfterAutoFitColumnEvent OnAfterAutoFitColumn = {read=FOnAfterAutoFitColumn, write=FOnAfterAutoFitColumn};
	__property TVTAfterAutoFitColumnsEvent OnAfterAutoFitColumns = {read=FOnAfterAutoFitColumns, write=FOnAfterAutoFitColumns};
	__property TVTAfterCellPaintEvent OnAfterCellPaint = {read=FOnAfterCellPaint, write=FOnAfterCellPaint};
	__property TVTColumnExportEvent OnAfterColumnExport = {read=FOnAfterColumnExport, write=FOnAfterColumnExport};
	__property TVTAfterColumnWidthTrackingEvent OnAfterColumnWidthTracking = {read=FOnAfterColumnWidthTracking, write=FOnAfterColumnWidthTracking};
	__property TVTAfterGetMaxColumnWidthEvent OnAfterGetMaxColumnWidth = {read=FOnAfterGetMaxColumnWidth, write=FOnAfterGetMaxColumnWidth};
	__property TVTTreeExportEvent OnAfterHeaderExport = {read=FOnBeforeHeaderExport, write=FOnBeforeHeaderExport};
	__property TVTAfterHeaderHeightTrackingEvent OnAfterHeaderHeightTracking = {read=FOnAfterHeaderHeightTracking, write=FOnAfterHeaderHeightTracking};
	__property TVTAfterItemEraseEvent OnAfterItemErase = {read=FOnAfterItemErase, write=FOnAfterItemErase};
	__property TVTAfterItemPaintEvent OnAfterItemPaint = {read=FOnAfterItemPaint, write=FOnAfterItemPaint};
	__property TVTNodeExportEvent OnAfterNodeExport = {read=FOnAfterNodeExport, write=FOnAfterNodeExport};
	__property TVTPaintEvent OnAfterPaint = {read=FOnAfterPaint, write=FOnAfterPaint};
	__property TVTTreeExportEvent OnAfterTreeExport = {read=FOnAfterTreeExport, write=FOnAfterTreeExport};
	__property TVTBeforeAutoFitColumnEvent OnBeforeAutoFitColumn = {read=FOnBeforeAutoFitColumn, write=FOnBeforeAutoFitColumn};
	__property TVTBeforeAutoFitColumnsEvent OnBeforeAutoFitColumns = {read=FOnBeforeAutoFitColumns, write=FOnBeforeAutoFitColumns};
	__property TVTBeforeCellPaintEvent OnBeforeCellPaint = {read=FOnBeforeCellPaint, write=FOnBeforeCellPaint};
	__property TVTColumnExportEvent OnBeforeColumnExport = {read=FOnBeforeColumnExport, write=FOnBeforeColumnExport};
	__property TVTBeforeColumnWidthTrackingEvent OnBeforeColumnWidthTracking = {read=FOnBeforeColumnWidthTracking, write=FOnBeforeColumnWidthTracking};
	__property TVTBeforeGetMaxColumnWidthEvent OnBeforeGetMaxColumnWidth = {read=FOnBeforeGetMaxColumnWidth, write=FOnBeforeGetMaxColumnWidth};
	__property TVTTreeExportEvent OnBeforeHeaderExport = {read=FOnBeforeHeaderExport, write=FOnBeforeHeaderExport};
	__property TVTBeforeHeaderHeightTrackingEvent OnBeforeHeaderHeightTracking = {read=FOnBeforeHeaderHeightTracking, write=FOnBeforeHeaderHeightTracking};
	__property TVTBeforeItemEraseEvent OnBeforeItemErase = {read=FOnBeforeItemErase, write=FOnBeforeItemErase};
	__property TVTBeforeItemPaintEvent OnBeforeItemPaint = {read=FOnBeforeItemPaint, write=FOnBeforeItemPaint};
	__property TVTNodeExportEvent OnBeforeNodeExport = {read=FOnBeforeNodeExport, write=FOnBeforeNodeExport};
	__property TVTPaintEvent OnBeforePaint = {read=FOnBeforePaint, write=FOnBeforePaint};
	__property TVTTreeExportEvent OnBeforeTreeExport = {read=FOnBeforeTreeExport, write=FOnBeforeTreeExport};
	__property TVTCanSplitterResizeColumnEvent OnCanSplitterResizeColumn = {read=FOnCanSplitterResizeColumn, write=FOnCanSplitterResizeColumn};
	__property TVTChangeEvent OnChange = {read=FOnChange, write=FOnChange};
	__property TVTChangeEvent OnChecked = {read=FOnChecked, write=FOnChecked};
	__property TVTCheckChangingEvent OnChecking = {read=FOnChecking, write=FOnChecking};
	__property TVTChangeEvent OnCollapsed = {read=FOnCollapsed, write=FOnCollapsed};
	__property TVTChangingEvent OnCollapsing = {read=FOnCollapsing, write=FOnCollapsing};
	__property TVTColumnClickEvent OnColumnClick = {read=FOnColumnClick, write=FOnColumnClick};
	__property TVTColumnDblClickEvent OnColumnDblClick = {read=FOnColumnDblClick, write=FOnColumnDblClick};
	__property TVTColumnExportEvent OnColumnExport = {read=FOnColumnExport, write=FOnColumnExport};
	__property TVTHeaderNotifyEvent OnColumnResize = {read=FOnColumnResize, write=FOnColumnResize};
	__property TVTColumnWidthDblClickResizeEvent OnColumnWidthDblClickResize = {read=FOnColumnWidthDblClickResize, write=FOnColumnWidthDblClickResize};
	__property TVTColumnWidthTrackingEvent OnColumnWidthTracking = {read=FOnColumnWidthTracking, write=FOnColumnWidthTracking};
	__property TVTCompareEvent OnCompareNodes = {read=FOnCompareNodes, write=FOnCompareNodes};
	__property TVTCreateDataObjectEvent OnCreateDataObject = {read=FOnCreateDataObject, write=FOnCreateDataObject};
	__property TVTCreateDragManagerEvent OnCreateDragManager = {read=FOnCreateDragManager, write=FOnCreateDragManager};
	__property TVTCreateEditorEvent OnCreateEditor = {read=FOnCreateEditor, write=FOnCreateEditor};
	__property TVTDragAllowedEvent OnDragAllowed = {read=FOnDragAllowed, write=FOnDragAllowed};
	__property TVTDragOverEvent OnDragOver = {read=FOnDragOver, write=FOnDragOver};
	__property TVTDragDropEvent OnDragDrop = {read=FOnDragDrop, write=FOnDragDrop};
	__property TVTEditCancelEvent OnEditCancelled = {read=FOnEditCancelled, write=FOnEditCancelled};
	__property TVTEditChangingEvent OnEditing = {read=FOnEditing, write=FOnEditing};
	__property TVTEditChangeEvent OnEdited = {read=FOnEdited, write=FOnEdited};
	__property TVTChangeEvent OnExpanded = {read=FOnExpanded, write=FOnExpanded};
	__property TVTChangingEvent OnExpanding = {read=FOnExpanding, write=FOnExpanding};
	__property TVTFocusChangeEvent OnFocusChanged = {read=FOnFocusChanged, write=FOnFocusChanged};
	__property TVTFocusChangingEvent OnFocusChanging = {read=FOnFocusChanging, write=FOnFocusChanging};
	__property TVTFreeNodeEvent OnFreeNode = {read=FOnFreeNode, write=FOnFreeNode};
	__property TVTGetCellIsEmptyEvent OnGetCellIsEmpty = {read=FOnGetCellIsEmpty, write=FOnGetCellIsEmpty};
	__property TVTGetCursorEvent OnGetCursor = {read=FOnGetCursor, write=FOnGetCursor};
	__property TVTGetHeaderCursorEvent OnGetHeaderCursor = {read=FOnGetHeaderCursor, write=FOnGetHeaderCursor};
	__property TVTHelpContextEvent OnGetHelpContext = {read=FOnGetHelpContext, write=FOnGetHelpContext};
	__property TVTGetImageEvent OnGetImageIndex = {read=FOnGetImage, write=FOnGetImage};
	__property TVTGetImageExEvent OnGetImageIndexEx = {read=FOnGetImageEx, write=FOnGetImageEx};
	__property TVTGetImageTextEvent OnGetImageText = {read=FOnGetImageText, write=FOnGetImageText};
	__property TVTGetLineStyleEvent OnGetLineStyle = {read=FOnGetLineStyle, write=FOnGetLineStyle};
	__property TVTGetNodeDataSizeEvent OnGetNodeDataSize = {read=FOnGetNodeDataSize, write=FOnGetNodeDataSize};
	__property TVTPopupEvent OnGetPopupMenu = {read=FOnGetPopupMenu, write=FOnGetPopupMenu};
	__property TVTGetUserClipboardFormatsEvent OnGetUserClipboardFormats = {read=FOnGetUserClipboardFormats, write=FOnGetUserClipboardFormats};
	__property TVTHeaderClickEvent OnHeaderCheckBoxClick = {read=FOnHeaderCheckBoxClick, write=FOnHeaderCheckBoxClick};
	__property TVTHeaderClickEvent OnHeaderClick = {read=FOnHeaderClick, write=FOnHeaderClick};
	__property TVTHeaderClickEvent OnHeaderDblClick = {read=FOnHeaderDblClick, write=FOnHeaderDblClick};
	__property TVTHeaderDraggedEvent OnHeaderDragged = {read=FOnHeaderDragged, write=FOnHeaderDragged};
	__property TVTHeaderDraggedOutEvent OnHeaderDraggedOut = {read=FOnHeaderDraggedOut, write=FOnHeaderDraggedOut};
	__property TVTHeaderDraggingEvent OnHeaderDragging = {read=FOnHeaderDragging, write=FOnHeaderDragging};
	__property TVTHeaderPaintEvent OnHeaderDraw = {read=FOnHeaderDraw, write=FOnHeaderDraw};
	__property TVTHeaderPaintQueryElementsEvent OnHeaderDrawQueryElements = {read=FOnHeaderDrawQueryElements, write=FOnHeaderDrawQueryElements};
	__property TVTHeaderHeightTrackingEvent OnHeaderHeightTracking = {read=FOnHeaderHeightTracking, write=FOnHeaderHeightTracking};
	__property TVTHeaderHeightDblClickResizeEvent OnHeaderHeightDblClickResize = {read=FOnHeaderHeightDblClickResize, write=FOnHeaderHeightDblClickResize};
	__property TVTHeaderClickEvent OnHeaderImageClick = {read=FOnHeaderImageClick, write=FOnHeaderImageClick};
	__property TVTHeaderMouseEvent OnHeaderMouseDown = {read=FOnHeaderMouseDown, write=FOnHeaderMouseDown};
	__property TVTHeaderMouseMoveEvent OnHeaderMouseMove = {read=FOnHeaderMouseMove, write=FOnHeaderMouseMove};
	__property TVTHeaderMouseEvent OnHeaderMouseUp = {read=FOnHeaderMouseUp, write=FOnHeaderMouseUp};
	__property TVTHotNodeChangeEvent OnHotChange = {read=FOnHotChange, write=FOnHotChange};
	__property TVTIncrementalSearchEvent OnIncrementalSearch = {read=FOnIncrementalSearch, write=FOnIncrementalSearch};
	__property TVTInitChildrenEvent OnInitChildren = {read=FOnInitChildren, write=FOnInitChildren};
	__property TVTInitNodeEvent OnInitNode = {read=FOnInitNode, write=FOnInitNode};
	__property TVTKeyActionEvent OnKeyAction = {read=FOnKeyAction, write=FOnKeyAction};
	__property TVTSaveNodeEvent OnLoadNode = {read=FOnLoadNode, write=FOnLoadNode};
	__property TVTMeasureItemEvent OnMeasureItem = {read=FOnMeasureItem, write=FOnMeasureItem};
	__property TVTNodeCopiedEvent OnNodeCopied = {read=FOnNodeCopied, write=FOnNodeCopied};
	__property TVTNodeCopyingEvent OnNodeCopying = {read=FOnNodeCopying, write=FOnNodeCopying};
	__property TVTNodeExportEvent OnNodeExport = {read=FOnNodeExport, write=FOnNodeExport};
	__property TVTNodeHeightTrackingEvent OnNodeHeightTracking = {read=FOnNodeHeightTracking, write=FOnNodeHeightTracking};
	__property TVTNodeHeightDblClickResizeEvent OnNodeHeightDblClickResize = {read=FOnNodeHeightDblClickResize, write=FOnNodeHeightDblClickResize};
	__property TVTNodeMovedEvent OnNodeMoved = {read=FOnNodeMoved, write=FOnNodeMoved};
	__property TVTNodeMovingEvent OnNodeMoving = {read=FOnNodeMoving, write=FOnNodeMoving};
	__property TVTBackgroundPaintEvent OnPaintBackground = {read=FOnPaintBackground, write=FOnPaintBackground};
	__property TVTRenderOLEDataEvent OnRenderOLEData = {read=FOnRenderOLEData, write=FOnRenderOLEData};
	__property TVTChangeEvent OnResetNode = {read=FOnResetNode, write=FOnResetNode};
	__property TVTSaveNodeEvent OnSaveNode = {read=FOnSaveNode, write=FOnSaveNode};
	__property TVTScrollEvent OnScroll = {read=FOnScroll, write=FOnScroll};
	__property TVTScrollbarShowEvent OnShowScrollbar = {read=FOnShowScrollbar, write=FOnShowScrollbar};
	__property TVTStateChangeEvent OnStateChange = {read=FOnStateChange, write=FOnStateChange};
	__property TVTStructureChangeEvent OnStructureChange = {read=FOnStructureChange, write=FOnStructureChange};
	__property TVTUpdatingEvent OnUpdating = {read=FOnUpdating, write=FOnUpdating};
	
public:
	__fastcall virtual TBaseVirtualTree(Classes::TComponent* AOwner);
	__fastcall virtual ~TBaseVirtualTree(void);
	unsigned __fastcall AbsoluteIndex(PVirtualNode Node);
	virtual PVirtualNode __fastcall AddChild(PVirtualNode Parent, void * UserData = (void *)(0x0));
	void __fastcall AddFromStream(Classes::TStream* Stream, PVirtualNode TargetNode);
	virtual void __fastcall AfterConstruction(void);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	HIDESBASE void __fastcall BeginDrag(bool Immediate, int Threshold = 0xffffffff);
	void __fastcall BeginSynch(void);
	virtual void __fastcall BeginUpdate(void);
	void __fastcall CancelCutOrCopy(void);
	bool __fastcall CancelEditNode(void);
	void __fastcall CancelOperation(void);
	virtual bool __fastcall CanEdit(PVirtualNode Node, TColumnIndex Column);
	DYNAMIC bool __fastcall CanFocus(void);
	virtual void __fastcall Clear(void);
	void __fastcall ClearChecked(void);
	void __fastcall ClearSelection(void);
	PVirtualNode __fastcall CopyTo(PVirtualNode Source, TBaseVirtualTree* Tree, TVTNodeAttachMode Mode, bool ChildrenOnly)/* overload */;
	PVirtualNode __fastcall CopyTo(PVirtualNode Source, PVirtualNode Target, TVTNodeAttachMode Mode, bool ChildrenOnly)/* overload */;
	virtual void __fastcall CopyToClipBoard(void);
	virtual void __fastcall CutToClipBoard(void);
	void __fastcall DeleteChildren(PVirtualNode Node, bool ResetHasChildren = false);
	void __fastcall DeleteNode(PVirtualNode Node, bool Reindex = true);
	virtual void __fastcall DeleteSelectedNodes(void);
	HIDESBASE bool __fastcall Dragging(void);
	virtual bool __fastcall EditNode(PVirtualNode Node, TColumnIndex Column);
	bool __fastcall EndEditNode(void);
	void __fastcall EndSynch(void);
	virtual void __fastcall EndUpdate(void);
	DYNAMIC bool __fastcall ExecuteAction(Classes::TBasicAction* Action);
	void __fastcall FinishCutOrCopy(void);
	void __fastcall FlushClipboard(void);
	virtual void __fastcall FullCollapse(PVirtualNode Node = (void *)(0x0));
	virtual void __fastcall FullExpand(PVirtualNode Node = (void *)(0x0));
	DYNAMIC Classes::TAlignment __fastcall GetControlsAlignment(void);
	Types::TRect __fastcall GetDisplayRect(PVirtualNode Node, TColumnIndex Column, bool TextOnly, bool Unclipped = false, bool ApplyCellContentMargin = false);
	PVirtualNode __fastcall GetFirst(bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetFirstChecked(TCheckState State = (TCheckState)(0x2), bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetFirstChild(PVirtualNode Node);
	PVirtualNode __fastcall GetFirstCutCopy(bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetFirstInitialized(bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetFirstLeaf(void);
	PVirtualNode __fastcall GetFirstLevel(unsigned NodeLevel);
	PVirtualNode __fastcall GetFirstNoInit(bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetFirstSelected(bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetFirstVisible(PVirtualNode Node = (void *)(0x0), bool ConsiderChildrenAbove = true);
	PVirtualNode __fastcall GetFirstVisibleChild(PVirtualNode Node);
	PVirtualNode __fastcall GetFirstVisibleChildNoInit(PVirtualNode Node);
	PVirtualNode __fastcall GetFirstVisibleNoInit(PVirtualNode Node = (void *)(0x0), bool ConsiderChildrenAbove = true);
	virtual void __fastcall GetHitTestInfoAt(int X, int Y, bool Relative, THitInfo &HitInfo);
	PVirtualNode __fastcall GetLast(PVirtualNode Node = (void *)(0x0), bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetLastInitialized(PVirtualNode Node = (void *)(0x0), bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetLastNoInit(PVirtualNode Node = (void *)(0x0), bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetLastChild(PVirtualNode Node);
	PVirtualNode __fastcall GetLastChildNoInit(PVirtualNode Node);
	PVirtualNode __fastcall GetLastVisible(PVirtualNode Node = (void *)(0x0), bool ConsiderChildrenAbove = true);
	PVirtualNode __fastcall GetLastVisibleChild(PVirtualNode Node);
	PVirtualNode __fastcall GetLastVisibleChildNoInit(PVirtualNode Node);
	PVirtualNode __fastcall GetLastVisibleNoInit(PVirtualNode Node = (void *)(0x0), bool ConsiderChildrenAbove = true);
	int __fastcall GetMaxColumnWidth(TColumnIndex Column, bool UseSmartColumnWidth = false);
	PVirtualNode __fastcall GetNext(PVirtualNode Node, bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetNextChecked(PVirtualNode Node, TCheckState State = (TCheckState)(0x2), bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetNextCutCopy(PVirtualNode Node, bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetNextInitialized(PVirtualNode Node, bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetNextLeaf(PVirtualNode Node);
	PVirtualNode __fastcall GetNextLevel(PVirtualNode Node, unsigned NodeLevel);
	PVirtualNode __fastcall GetNextNoInit(PVirtualNode Node, bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetNextSelected(PVirtualNode Node, bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetNextSibling(PVirtualNode Node);
	PVirtualNode __fastcall GetNextVisible(PVirtualNode Node, bool ConsiderChildrenAbove = true);
	PVirtualNode __fastcall GetNextVisibleNoInit(PVirtualNode Node, bool ConsiderChildrenAbove = true);
	PVirtualNode __fastcall GetNextVisibleSibling(PVirtualNode Node);
	PVirtualNode __fastcall GetNextVisibleSiblingNoInit(PVirtualNode Node);
	PVirtualNode __fastcall GetNodeAt(int X, int Y)/* overload */;
	PVirtualNode __fastcall GetNodeAt(int X, int Y, bool Relative, int &NodeTop)/* overload */;
	void * __fastcall GetNodeData(PVirtualNode Node);
	unsigned __fastcall GetNodeLevel(PVirtualNode Node);
	PVirtualNode __fastcall GetPrevious(PVirtualNode Node, bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetPreviousChecked(PVirtualNode Node, TCheckState State = (TCheckState)(0x2), bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetPreviousCutCopy(PVirtualNode Node, bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetPreviousInitialized(PVirtualNode Node, bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetPreviousLeaf(PVirtualNode Node);
	PVirtualNode __fastcall GetPreviousLevel(PVirtualNode Node, unsigned NodeLevel);
	PVirtualNode __fastcall GetPreviousNoInit(PVirtualNode Node, bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetPreviousSelected(PVirtualNode Node, bool ConsiderChildrenAbove = false);
	PVirtualNode __fastcall GetPreviousSibling(PVirtualNode Node);
	PVirtualNode __fastcall GetPreviousVisible(PVirtualNode Node, bool ConsiderChildrenAbove = true);
	PVirtualNode __fastcall GetPreviousVisibleNoInit(PVirtualNode Node, bool ConsiderChildrenAbove = true);
	PVirtualNode __fastcall GetPreviousVisibleSibling(PVirtualNode Node);
	PVirtualNode __fastcall GetPreviousVisibleSiblingNoInit(PVirtualNode Node);
	TNodeArray __fastcall GetSortedCutCopySet(bool Resolve);
	TNodeArray __fastcall GetSortedSelection(bool Resolve);
	virtual void __fastcall GetTextInfo(PVirtualNode Node, TColumnIndex Column, const Graphics::TFont* AFont, Types::TRect &R, WideString &Text);
	Types::TRect __fastcall GetTreeRect();
	PVirtualNode __fastcall GetVisibleParent(PVirtualNode Node);
	bool __fastcall HasAsParent(PVirtualNode Node, PVirtualNode PotentialParent);
	PVirtualNode __fastcall InsertNode(PVirtualNode Node, TVTNodeAttachMode Mode, void * UserData = (void *)(0x0));
	void __fastcall InvalidateChildren(PVirtualNode Node, bool Recursive);
	void __fastcall InvalidateColumn(TColumnIndex Column);
	virtual Types::TRect __fastcall InvalidateNode(PVirtualNode Node);
	void __fastcall InvalidateToBottom(PVirtualNode Node);
	void __fastcall InvertSelection(bool VisibleOnly);
	bool __fastcall IsEditing(void);
	bool __fastcall IsMouseSelecting(void);
	PVirtualNode __fastcall IterateSubtree(PVirtualNode Node, TVTGetNodeProc Callback, void * Data, TVirtualNodeStates Filter = System::Set<TVirtualNodeState, vsInitialized, vsToggling> () , bool DoInit = false, bool ChildNodesOnly = false);
	virtual void __fastcall LoadFromFile(const AnsiString FileName);
	virtual void __fastcall LoadFromStream(Classes::TStream* Stream);
	void __fastcall MeasureItemHeight(const Graphics::TCanvas* Canvas, PVirtualNode Node);
	void __fastcall MoveTo(PVirtualNode Source, PVirtualNode Target, TVTNodeAttachMode Mode, bool ChildrenOnly)/* overload */;
	void __fastcall MoveTo(PVirtualNode Node, TBaseVirtualTree* Tree, TVTNodeAttachMode Mode, bool ChildrenOnly)/* overload */;
	void __fastcall PaintTree(Graphics::TCanvas* TargetCanvas, const Types::TRect &Window, const Types::TPoint &Target, TVTInternalPaintOptions PaintOptions, Graphics::TPixelFormat PixelFormat = (Graphics::TPixelFormat)(0x0));
	virtual bool __fastcall PasteFromClipboard(void);
	void __fastcall PrepareDragImage(const Types::TPoint &HotSpot, const _di_IDataObject DataObject);
	void __fastcall Print(Printers::TPrinter* Printer, bool PrintHeader);
	bool __fastcall ProcessDrop(_di_IDataObject DataObject, PVirtualNode TargetNode, int &Effect, TVTNodeAttachMode Mode);
	bool __fastcall ProcessOLEData(TBaseVirtualTree* Source, _di_IDataObject DataObject, PVirtualNode TargetNode, TVTNodeAttachMode Mode, bool Optimized);
	void __fastcall RepaintNode(PVirtualNode Node);
	virtual void __fastcall ReinitChildren(PVirtualNode Node, bool Recursive);
	virtual void __fastcall ReinitNode(PVirtualNode Node, bool Recursive);
	virtual void __fastcall ResetNode(PVirtualNode Node);
	void __fastcall SaveToFile(const AnsiString FileName);
	virtual void __fastcall SaveToStream(Classes::TStream* Stream, PVirtualNode Node = (void *)(0x0));
	bool __fastcall ScrollIntoView(PVirtualNode Node, bool Center, bool Horizontally = false)/* overload */;
	bool __fastcall ScrollIntoView(TColumnIndex Column, bool Center)/* overload */;
	void __fastcall SelectAll(bool VisibleOnly);
	virtual void __fastcall Sort(PVirtualNode Node, TColumnIndex Column, TSortDirection Direction, bool DoInit = true);
	virtual void __fastcall SortTree(TColumnIndex Column, TSortDirection Direction, bool DoInit = true);
	void __fastcall ToggleNode(PVirtualNode Node);
	DYNAMIC bool __fastcall UpdateAction(Classes::TBasicAction* Action);
	void __fastcall UpdateHorizontalScrollBar(bool DoRepaint);
	virtual void __fastcall UpdateScrollBars(bool DoRepaint);
	void __fastcall UpdateVerticalScrollBar(bool DoRepaint);
	HIDESBASE bool __fastcall UseRightToLeftReading(void);
	void __fastcall ValidateChildren(PVirtualNode Node, bool Recursive);
	void __fastcall ValidateNode(PVirtualNode Node, bool Recursive);
	__property _di_IAccessible Accessible = {read=FAccessible, write=FAccessible};
	__property _di_IAccessible AccessibleItem = {read=FAccessibleItem, write=FAccessibleItem};
	__property AnsiString AccessibleName = {read=FAccessibleName, write=FAccessibleName};
	__property PVirtualNode BottomNode = {read=GetBottomNode, write=SetBottomNode};
	__property int CheckedCount = {read=GetCheckedCount, nodefault};
	__property Imglist::TCustomImageList* CheckImages = {read=FCheckImages};
	__property TCheckState CheckState[PVirtualNode Node] = {read=GetCheckState, write=SetCheckState};
	__property TCheckType CheckType[PVirtualNode Node] = {read=GetCheckType, write=SetCheckType};
	__property unsigned ChildCount[PVirtualNode Node] = {read=GetChildCount, write=SetChildCount};
	__property bool ChildrenInitialized[PVirtualNode Node] = {read=GetChildrenInitialized};
	__property int CutCopyCount = {read=GetCutCopyCount, nodefault};
	__property TVTDragImage* DragImage = {read=FDragImage};
	__property _di_IVTDragManager DragManager = {read=GetDragManager};
	__property PVirtualNode DropTargetNode = {read=FDropTargetNode};
	__property _di_IVTEditLink EditLink = {read=FEditLink};
	__property bool Expanded[PVirtualNode Node] = {read=GetExpanded, write=SetExpanded};
	__property TColumnIndex FocusedColumn = {read=FFocusedColumn, write=SetFocusedColumn, default=-2};
	__property PVirtualNode FocusedNode = {read=FFocusedNode, write=SetFocusedNode};
	__property Font ;
	__property bool FullyVisible[PVirtualNode Node] = {read=GetFullyVisible, write=SetFullyVisible};
	__property bool HasChildren[PVirtualNode Node] = {read=GetHasChildren, write=SetHasChildren};
	__property PVirtualNode HotNode = {read=FCurrentHotNode};
	__property bool IsDisabled[PVirtualNode Node] = {read=GetDisabled, write=SetDisabled};
	__property bool IsVisible[PVirtualNode Node] = {read=GetVisible, write=SetVisible};
	__property bool MultiLine[PVirtualNode Node] = {read=GetMultiline, write=SetMultiline};
	__property unsigned NodeHeight[PVirtualNode Node] = {read=GetNodeHeight, write=SetNodeHeight};
	__property PVirtualNode NodeParent[PVirtualNode Node] = {read=GetNodeParent, write=SetNodeParent};
	__property int OffsetX = {read=FOffsetX, write=SetOffsetX, nodefault};
	__property Types::TPoint OffsetXY = {read=GetOffsetXY, write=SetOffsetXY};
	__property int OffsetY = {read=FOffsetY, write=SetOffsetY, nodefault};
	__property PVirtualNode RootNode = {read=FRoot};
	__property WideString SearchBuffer = {read=FSearchBuffer};
	__property bool Selected[PVirtualNode Node] = {read=GetSelected, write=SetSelected};
	__property bool SelectionLocked = {read=FSelectionLocked, write=FSelectionLocked, nodefault};
	__property unsigned TotalCount = {read=GetTotalCount, nodefault};
	__property TVirtualTreeStates TreeStates = {read=FStates, write=FStates};
	__property int SelectedCount = {read=FSelectionCount, nodefault};
	__property PVirtualNode TopNode = {read=GetTopNode, write=SetTopNode};
	__property Byte VerticalAlignment[PVirtualNode Node] = {read=GetVerticalAlignment, write=SetVerticalAlignment};
	__property unsigned VisibleCount = {read=FVisibleCount, nodefault};
	__property bool VisiblePath[PVirtualNode Node] = {read=GetVisiblePath, write=SetVisiblePath};
	__property unsigned UpdateCount = {read=FUpdateCount, nodefault};
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TBaseVirtualTree(HWND ParentWindow) : Controls::TCustomControl(ParentWindow) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TCustomVirtualTreeOptions : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	TBaseVirtualTree* FOwner;
	TVTPaintOptions FPaintOptions;
	TVTAnimationOptions FAnimationOptions;
	TVTAutoOptions FAutoOptions;
	TVTSelectionOptions FSelectionOptions;
	TVTMiscOptions FMiscOptions;
	TVTExportMode FExportMode;
	void __fastcall SetAnimationOptions(const TVTAnimationOptions Value);
	void __fastcall SetAutoOptions(const TVTAutoOptions Value);
	void __fastcall SetMiscOptions(const TVTMiscOptions Value);
	void __fastcall SetPaintOptions(const TVTPaintOptions Value);
	void __fastcall SetSelectionOptions(const TVTSelectionOptions Value);
	
protected:
	__property TVTAnimationOptions AnimationOptions = {read=FAnimationOptions, write=SetAnimationOptions, default=0};
	__property TVTAutoOptions AutoOptions = {read=FAutoOptions, write=SetAutoOptions, default=329};
	__property TVTExportMode ExportMode = {read=FExportMode, write=FExportMode, default=0};
	__property TVTMiscOptions MiscOptions = {read=FMiscOptions, write=SetMiscOptions, default=16809};
	__property TVTPaintOptions PaintOptions = {read=FPaintOptions, write=SetPaintOptions, default=7008};
	__property TVTSelectionOptions SelectionOptions = {read=FSelectionOptions, write=SetSelectionOptions, default=0};
	
public:
	__fastcall virtual TCustomVirtualTreeOptions(TBaseVirtualTree* AOwner);
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
	__property TBaseVirtualTree* Owner = {read=FOwner};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TCustomVirtualTreeOptions(void) { }
	#pragma option pop
	
};


class DELPHICLASS TVirtualTreeOptions;
class PASCALIMPLEMENTATION TVirtualTreeOptions : public TCustomVirtualTreeOptions 
{
	typedef TCustomVirtualTreeOptions inherited;
	
__published:
	__property AnimationOptions  = {default=0};
	__property AutoOptions  = {default=329};
	__property ExportMode  = {default=0};
	__property MiscOptions  = {default=16809};
	__property PaintOptions  = {default=7008};
	__property SelectionOptions  = {default=0};
public:
	#pragma option push -w-inl
	/* TCustomVirtualTreeOptions.Create */ inline __fastcall virtual TVirtualTreeOptions(TBaseVirtualTree* AOwner) : TCustomVirtualTreeOptions(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TVirtualTreeOptions(void) { }
	#pragma option pop
	
};


struct TVTReference;
typedef TVTReference *PVTReference;

#pragma pack(push, 4)
struct TVTReference
{
	unsigned Process;
	TBaseVirtualTree* Tree;
} ;
#pragma pack(pop)

#pragma pack(push, 1)
struct VirtualTrees__4
{
	
} ;
#pragma pack(pop)

#pragma pack(push, 1)
struct TVirtualNode
{
	unsigned Index;
	unsigned ChildCount;
	Word NodeHeight;
	TVirtualNodeStates States;
	Byte Align;
	TCheckState CheckState;
	TCheckType CheckType;
	Byte Dummy;
	unsigned TotalCount;
	unsigned TotalHeight;
	TVirtualNode *Parent;
	TVirtualNode *PrevSibling;
	TVirtualNode *NextSibling;
	TVirtualNode *FirstChild;
	TVirtualNode *LastChild;
	VirtualTrees__4 Data;
} ;
#pragma pack(pop)

#pragma pack(push, 1)
struct TInternalStgMedium
{
	Word Format;
	tagSTGMEDIUM Medium;
} ;
#pragma pack(pop)

typedef DynamicArray<TInternalStgMedium >  TInternalStgMediumArray;

class DELPHICLASS TEnumFormatEtc;
class PASCALIMPLEMENTATION TEnumFormatEtc : public System::TInterfacedObject 
{
	typedef System::TInterfacedObject inherited;
	
private:
	TBaseVirtualTree* FTree;
	DynamicArray<tagFORMATETC >  FFormatEtcArray;
	int FCurrentIndex;
	
public:
	__fastcall TEnumFormatEtc(TBaseVirtualTree* Tree, TFormatEtcArray AFormatEtcArray);
	HRESULT __stdcall Clone(/* out */ _di_IEnumFORMATETC &Enum);
	HRESULT __stdcall Next(int celt, /* out */ void *elt, PLongint pceltFetched);
	HRESULT __stdcall Reset(void);
	HRESULT __stdcall Skip(int celt);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TEnumFormatEtc(void) { }
	#pragma option pop
	
private:
	void *__IEnumFORMATETC;	/* IEnumFORMATETC */
	
public:
	operator IEnumFORMATETC*(void) { return (IEnumFORMATETC*)&__IEnumFORMATETC; }
	
};


__interface IDropTargetHelper;
typedef System::DelphiInterface<IDropTargetHelper> _di_IDropTargetHelper;
__interface INTERFACE_UUID("{4657278B-411B-11D2-839A-00C04FD918D0}") IDropTargetHelper  : public IInterface 
{
	
public:
	virtual HRESULT __stdcall DragEnter(HWND hwndTarget, _di_IDataObject pDataObject, Types::TPoint &ppt, int dwEffect) = 0 ;
	virtual HRESULT __stdcall DragLeave(void) = 0 ;
	virtual HRESULT __stdcall DragOver(Types::TPoint &ppt, int dwEffect) = 0 ;
	virtual HRESULT __stdcall Drop(_di_IDataObject pDataObject, Types::TPoint &ppt, int dwEffect) = 0 ;
	virtual HRESULT __stdcall Show(bool fShow) = 0 ;
};

struct TSHDragImage;
typedef TSHDragImage *PSHDragImage;

#pragma pack(push, 1)
struct TSHDragImage
{
	tagSIZE sizeDragImage;
	Types::TPoint ptOffset;
	HBITMAP hbmpDragImage;
	unsigned ColorRef;
} ;
#pragma pack(pop)

__interface IDragSourceHelper;
typedef System::DelphiInterface<IDragSourceHelper> _di_IDragSourceHelper;
__interface INTERFACE_UUID("{DE5BF786-477A-11D2-839D-00C04FD918D0}") IDragSourceHelper  : public IInterface 
{
	
public:
	virtual HRESULT __stdcall InitializeFromBitmap(TSHDragImage &SHDragImage, _di_IDataObject pDataObject) = 0 ;
	virtual HRESULT __stdcall InitializeFromWindow(HWND Window, Types::TPoint &ppt, _di_IDataObject pDataObject) = 0 ;
};

class DELPHICLASS TVTDataObject;
class PASCALIMPLEMENTATION TVTDataObject : public System::TInterfacedObject 
{
	typedef System::TInterfacedObject inherited;
	
private:
	TBaseVirtualTree* FOwner;
	bool FForClipboard;
	DynamicArray<tagFORMATETC >  FFormatEtcArray;
	DynamicArray<TInternalStgMedium >  FInternalStgMediumArray;
	_di_IDataAdviseHolder FAdviseHolder;
	
protected:
	System::_di_IInterface __fastcall CanonicalIUnknown(System::_di_IInterface TestUnknown);
	bool __fastcall EqualFormatEtc(const tagFORMATETC &FormatEtc1, const tagFORMATETC &FormatEtc2);
	int __fastcall FindFormatEtc(const tagFORMATETC &TestFormatEtc, const TFormatEtcArray FormatEtcArray);
	Activex::PStgMedium __fastcall FindInternalStgMedium(Word Format);
	unsigned __fastcall HGlobalClone(unsigned HGlobal);
	bool __fastcall RenderInternalOLEData(const tagFORMATETC &FormatEtcIn, tagSTGMEDIUM &Medium, HRESULT &OLEResult);
	HRESULT __fastcall StgMediumIncRef(const tagSTGMEDIUM &InStgMedium, tagSTGMEDIUM &OutStgMedium, bool CopyInMedium, _di_IDataObject DataObject);
	__property bool ForClipboard = {read=FForClipboard, nodefault};
	__property TFormatEtcArray FormatEtcArray = {read=FFormatEtcArray, write=FFormatEtcArray};
	__property TInternalStgMediumArray InternalStgMediumArray = {read=FInternalStgMediumArray, write=FInternalStgMediumArray};
	__property TBaseVirtualTree* Owner = {read=FOwner};
	
public:
	__fastcall virtual TVTDataObject(TBaseVirtualTree* AOwner, bool ForClipboard);
	__fastcall virtual ~TVTDataObject(void);
	virtual HRESULT __stdcall DAdvise(const tagFORMATETC &FormatEtc, int advf, const _di_IAdviseSink advSink, /* out */ int &dwConnection);
	virtual HRESULT __stdcall DUnadvise(int dwConnection);
	virtual HRESULT __stdcall EnumDAdvise(/* out */ _di_IEnumSTATDATA &enumAdvise);
	virtual HRESULT __stdcall EnumFormatEtc(int Direction, /* out */ _di_IEnumFORMATETC &EnumFormatEtc);
	virtual HRESULT __stdcall GetCanonicalFormatEtc(const tagFORMATETC &FormatEtc, /* out */ tagFORMATETC &FormatEtcOut);
	virtual HRESULT __stdcall GetData(const tagFORMATETC &FormatEtcIn, /* out */ tagSTGMEDIUM &Medium);
	virtual HRESULT __stdcall GetDataHere(const tagFORMATETC &FormatEtc, /* out */ tagSTGMEDIUM &Medium);
	virtual HRESULT __stdcall QueryGetData(const tagFORMATETC &FormatEtc);
	virtual HRESULT __stdcall SetData(const tagFORMATETC &FormatEtc, tagSTGMEDIUM &Medium, BOOL DoRelease);
private:
	void *__IDataObject;	/* IDataObject */
	
public:
	operator IDataObject*(void) { return (IDataObject*)&__IDataObject; }
	
};


class DELPHICLASS TVTDragManager;
class PASCALIMPLEMENTATION TVTDragManager : public System::TInterfacedObject 
{
	typedef System::TInterfacedObject inherited;
	
private:
	TBaseVirtualTree* FOwner;
	TBaseVirtualTree* FDragSource;
	bool FIsDropTarget;
	_di_IDataObject FDataObject;
	_di_IDropTargetHelper FDropTargetHelper;
	BOOL FFullDragging;
	_di_IDataObject __stdcall GetDataObject();
	TBaseVirtualTree* __stdcall GetDragSource(void);
	bool __stdcall GetDropTargetHelperSupported(void);
	bool __stdcall GetIsDropTarget(void);
	
public:
	__fastcall virtual TVTDragManager(TBaseVirtualTree* AOwner);
	__fastcall virtual ~TVTDragManager(void);
	HRESULT __stdcall DragEnter(const _di_IDataObject DataObject, int KeyState, const Types::TPoint Pt, int &Effect);
	HRESULT __stdcall DragLeave(void);
	HRESULT __stdcall DragOver(int KeyState, const Types::TPoint Pt, int &Effect);
	HRESULT __stdcall Drop(const _di_IDataObject DataObject, int KeyState, const Types::TPoint Pt, int &Effect);
	void __stdcall ForceDragLeave(void);
	HRESULT __stdcall GiveFeedback(int Effect);
	HRESULT __stdcall QueryContinueDrag(BOOL EscapePressed, int KeyState);
private:
	void *__IVTDragManager;	/* Virtualtrees::IVTDragManager */
	void *__IDropSource;	/* IDropSource */
	void *__IDropTarget;	/* IDropTarget */
	
public:
	operator IDropTarget*(void) { return (IDropTarget*)&__IDropTarget; }
	operator IDropSource*(void) { return (IDropSource*)&__IDropSource; }
	operator IVTDragManager*(void) { return (IVTDragManager*)&__IVTDragManager; }
	
};


typedef TVTHintData *PVTHintData;

class DELPHICLASS TVirtualTreeHintWindow;
class PASCALIMPLEMENTATION TVirtualTreeHintWindow : public Controls::THintWindow 
{
	typedef Controls::THintWindow inherited;
	
private:
	TVTHintData FHintData;
	Graphics::TBitmap* FBackground;
	Graphics::TBitmap* FDrawBuffer;
	Graphics::TBitmap* FTarget;
	int FTextHeight;
	bool __fastcall AnimationCallback(int Step, int StepSize, void * Data);
	void __fastcall InternalPaint(int Step, int StepSize);
	HIDESBASE MESSAGE void __fastcall CMTextChanged(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMEraseBkgnd(Messages::TWMEraseBkgnd &Message);
	HIDESBASE MESSAGE void __fastcall WMNCPaint(Messages::TMessage &Message);
	MESSAGE void __fastcall WMShowWindow(Messages::TWMShowWindow &Message);
	
protected:
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	virtual void __fastcall Paint(void);
	
public:
	__fastcall virtual TVirtualTreeHintWindow(Classes::TComponent* AOwner);
	__fastcall virtual ~TVirtualTreeHintWindow(void);
	virtual void __fastcall ActivateHint(const Types::TRect &Rect, const AnsiString AHint);
	virtual Types::TRect __fastcall CalcHintRect(int MaxWidth, const AnsiString AHint, void * AData);
	virtual bool __fastcall IsHintMsg(tagMSG &Msg);
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TVirtualTreeHintWindow(HWND ParentWindow) : Controls::THintWindow(ParentWindow) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TVirtualTreeColumnStyle { vsText, vsOwnerDraw };
#pragma option pop

#pragma option push -b-
enum TVTHeaderColumnLayout { blGlyphLeft, blGlyphRight, blGlyphTop, blGlyphBottom };
#pragma option pop

class PASCALIMPLEMENTATION TVirtualTreeColumn : public Classes::TCollectionItem 
{
	typedef Classes::TCollectionItem inherited;
	
private:
	WideString FText;
	WideString FHint;
	int FLeft;
	int FWidth;
	TColumnPosition FPosition;
	int FMinWidth;
	int FMaxWidth;
	TVirtualTreeColumnStyle FStyle;
	Imglist::TImageIndex FImageIndex;
	Classes::TBiDiMode FBiDiMode;
	TVTHeaderColumnLayout FLayout;
	int FMargin;
	int FSpacing;
	TVTColumnOptions FOptions;
	int FTag;
	Classes::TAlignment FAlignment;
	Classes::TAlignment FCaptionAlignment;
	int FLastWidth;
	Graphics::TColor FColor;
	bool FBonusPixel;
	float FSpringRest;
	WideString FCaptionText;
	bool FCheckBox;
	TCheckType FCheckType;
	TCheckState FCheckState;
	#pragma pack(push, 1)
	Types::TRect FImageRect;
	#pragma pack(pop)
	
	bool FHasImage;
	Classes::TAlignment __fastcall GetCaptionAlignment(void);
	int __fastcall GetLeft(void);
	bool __fastcall IsBiDiModeStored(void);
	bool __fastcall IsCaptionAlignmentStored(void);
	bool __fastcall IsColorStored(void);
	void __fastcall SetAlignment(const Classes::TAlignment Value);
	void __fastcall SetBiDiMode(Classes::TBiDiMode Value);
	void __fastcall SetCaptionAlignment(const Classes::TAlignment Value);
	void __fastcall SetCheckBox(bool Value);
	void __fastcall SetCheckState(TCheckState Value);
	void __fastcall SetCheckType(TCheckType Value);
	void __fastcall SetColor(const Graphics::TColor Value);
	void __fastcall SetImageIndex(Imglist::TImageIndex Value);
	void __fastcall SetLayout(TVTHeaderColumnLayout Value);
	void __fastcall SetMargin(int Value);
	void __fastcall SetMaxWidth(int Value);
	void __fastcall SetMinWidth(int Value);
	void __fastcall SetOptions(TVTColumnOptions Value);
	void __fastcall SetPosition(TColumnPosition Value);
	void __fastcall SetSpacing(int Value);
	void __fastcall SetStyle(TVirtualTreeColumnStyle Value);
	void __fastcall SetText(const WideString Value);
	void __fastcall SetWidth(int Value);
	
protected:
	void __fastcall ComputeHeaderLayout(HDC DC, const Types::TRect &Client, bool UseHeaderGlyph, bool UseSortGlyph, Types::TPoint &HeaderGlyphPos, Types::TPoint &SortGlyphPos, Types::TRect &TextBounds, unsigned DrawFormat, bool CalculateTextRect = false);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	void __fastcall GetAbsoluteBounds(int &Left, int &Right);
	virtual AnsiString __fastcall GetDisplayName();
	HIDESBASE TVirtualTreeColumns* __fastcall GetOwner(void);
	void __fastcall ReadHint(Classes::TReader* Reader);
	void __fastcall ReadText(Classes::TReader* Reader);
	void __fastcall WriteHint(Classes::TWriter* Writer);
	void __fastcall WriteText(Classes::TWriter* Writer);
	
public:
	__fastcall virtual TVirtualTreeColumn(Classes::TCollection* Collection);
	__fastcall virtual ~TVirtualTreeColumn(void);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	bool __fastcall Equals(System::TObject* OtherColumnObj);
	virtual Types::TRect __fastcall GetRect();
	void __fastcall LoadFromStream(const Classes::TStream* Stream, int Version);
	void __fastcall ParentBiDiModeChanged(void);
	void __fastcall ParentColorChanged(void);
	void __fastcall RestoreLastWidth(void);
	void __fastcall SaveToStream(const Classes::TStream* Stream);
	bool __fastcall UseRightToLeftReading(void);
	__property int Left = {read=GetLeft, nodefault};
	__property TVirtualTreeColumns* Owner = {read=GetOwner};
	
__published:
	__property Classes::TAlignment Alignment = {read=FAlignment, write=SetAlignment, default=0};
	__property Classes::TBiDiMode BiDiMode = {read=FBiDiMode, write=SetBiDiMode, stored=IsBiDiModeStored, nodefault};
	__property Classes::TAlignment CaptionAlignment = {read=GetCaptionAlignment, write=SetCaptionAlignment, stored=IsCaptionAlignmentStored, default=0};
	__property WideString CaptionText = {read=FCaptionText, stored=false};
	__property TCheckType CheckType = {read=FCheckType, write=SetCheckType, default=2};
	__property TCheckState CheckState = {read=FCheckState, write=SetCheckState, default=0};
	__property bool CheckBox = {read=FCheckBox, write=SetCheckBox, default=0};
	__property Graphics::TColor Color = {read=FColor, write=SetColor, stored=IsColorStored, nodefault};
	__property WideString Hint = {read=FHint, write=FHint, stored=false};
	__property Imglist::TImageIndex ImageIndex = {read=FImageIndex, write=SetImageIndex, default=-1};
	__property TVTHeaderColumnLayout Layout = {read=FLayout, write=SetLayout, default=0};
	__property int Margin = {read=FMargin, write=SetMargin, default=4};
	__property int MaxWidth = {read=FMaxWidth, write=SetMaxWidth, default=10000};
	__property int MinWidth = {read=FMinWidth, write=SetMinWidth, default=10};
	__property TVTColumnOptions Options = {read=FOptions, write=SetOptions, default=2303};
	__property TColumnPosition Position = {read=FPosition, write=SetPosition, nodefault};
	__property int Spacing = {read=FSpacing, write=SetSpacing, default=4};
	__property TVirtualTreeColumnStyle Style = {read=FStyle, write=SetStyle, default=0};
	__property int Tag = {read=FTag, write=FTag, default=0};
	__property WideString Text = {read=FText, write=SetText, stored=false};
	__property int Width = {read=FWidth, write=SetWidth, default=50};
};


typedef DynamicArray<unsigned >  TCardinalArray;

#pragma pack(push, 4)
struct THeaderPaintInfo
{
	Graphics::TCanvas* TargetCanvas;
	TVirtualTreeColumn* Column;
	Types::TRect PaintRectangle;
	Types::TRect TextRectangle;
	bool IsHoverIndex;
	bool IsDownIndex;
	bool IsEnabled;
	bool ShowHeaderGlyph;
	bool ShowSortGlyph;
	bool ShowRightBorder;
	TVTDropMarkMode DropMark;
	Types::TPoint GlyphPos;
	Types::TPoint SortGlyphPos;
} ;
#pragma pack(pop)

#pragma option push -b-
enum VirtualTrees__81 { csStopValidation, csUseCache, csValidating, csValidationNeeded };
#pragma option pop

typedef Set<VirtualTrees__81, csStopValidation, csValidationNeeded>  TChangeStates;

#pragma pack(push, 4)
struct TVTImageInfo
{
	int Index;
	int XPos;
	int YPos;
	bool Ghosted;
	Imglist::TCustomImageList* Images;
} ;
#pragma pack(pop)

#pragma pack(push, 4)
struct TVTPaintInfo
{
	Graphics::TCanvas* Canvas;
	TVTInternalPaintOptions PaintOptions;
	TVirtualNode *Node;
	TColumnIndex Column;
	TColumnPosition Position;
	Types::TRect CellRect;
	Types::TRect ContentRect;
	int NodeWidth;
	Classes::TAlignment Alignment;
	Classes::TAlignment CaptionAlignment;
	Classes::TBiDiMode BidiMode;
	Types::TPoint BrushOrigin;
	TVTImageInfo ImageInfo[4];
} ;
#pragma pack(pop)

typedef PVirtualNode __fastcall (__closure *TGetFirstNodeProc)(void);

typedef PVirtualNode __fastcall (__closure *TGetNextNodeProc)(PVirtualNode Node, bool ConsiderChildrenAbove = false);

#pragma option push -b-
enum TVTStringOption { toSaveCaptions, toShowStaticText, toAutoAcceptEditChange };
#pragma option pop

typedef Set<TVTStringOption, toSaveCaptions, toAutoAcceptEditChange>  TVTStringOptions;

class DELPHICLASS TCustomStringTreeOptions;
class PASCALIMPLEMENTATION TCustomStringTreeOptions : public TCustomVirtualTreeOptions 
{
	typedef TCustomVirtualTreeOptions inherited;
	
private:
	TVTStringOptions FStringOptions;
	void __fastcall SetStringOptions(const TVTStringOptions Value);
	
protected:
	__property TVTStringOptions StringOptions = {read=FStringOptions, write=SetStringOptions, default=5};
	
public:
	__fastcall virtual TCustomStringTreeOptions(TBaseVirtualTree* AOwner);
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TCustomStringTreeOptions(void) { }
	#pragma option pop
	
};


class DELPHICLASS TStringTreeOptions;
class PASCALIMPLEMENTATION TStringTreeOptions : public TCustomStringTreeOptions 
{
	typedef TCustomStringTreeOptions inherited;
	
__published:
	__property AnimationOptions  = {default=0};
	__property AutoOptions  = {default=329};
	__property ExportMode  = {default=0};
	__property MiscOptions  = {default=16809};
	__property PaintOptions  = {default=7008};
	__property SelectionOptions  = {default=0};
	__property StringOptions  = {default=5};
public:
	#pragma option push -w-inl
	/* TCustomStringTreeOptions.Create */ inline __fastcall virtual TStringTreeOptions(TBaseVirtualTree* AOwner) : TCustomStringTreeOptions(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TStringTreeOptions(void) { }
	#pragma option pop
	
};


class DELPHICLASS TVTEdit;
class DELPHICLASS TStringEditLink;
class DELPHICLASS TCustomVirtualStringTree;
#pragma option push -b-
enum TVSTTextType { ttNormal, ttStatic };
#pragma option pop

typedef void __fastcall (__closure *TVTPaintText)(TBaseVirtualTree* Sender, const Graphics::TCanvas* TargetCanvas, PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType);

typedef void __fastcall (__closure *TVSTGetTextEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType, WideString &CellText);

typedef void __fastcall (__closure *TVSTGetHintEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, TVTTooltipLineBreakStyle &LineBreakStyle, WideString &HintText);

typedef void __fastcall (__closure *TVSTNewTextEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, WideString NewText);

typedef void __fastcall (__closure *TVSTShortenStringEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas, PVirtualNode Node, TColumnIndex Column, const WideString S, int TextSpace, WideString &Result, bool &Done);

typedef void __fastcall (__closure *TVTMeasureTextWidthEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas, PVirtualNode Node, TColumnIndex Column, const WideString Text, int &Width);

typedef void __fastcall (__closure *TVTDrawTextEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* TargetCanvas, PVirtualNode Node, TColumnIndex Column, const WideString Text, const Types::TRect &CellRect, bool &DefaultDraw);

#pragma option push -b-
enum TVSTTextSourceType { tstAll, tstInitialized, tstSelected, tstCutCopySet, tstVisible };
#pragma option pop

class PASCALIMPLEMENTATION TCustomVirtualStringTree : public TBaseVirtualTree 
{
	typedef TBaseVirtualTree inherited;
	
private:
	WideString FDefaultText;
	int FTextHeight;
	int FEllipsisWidth;
	unsigned FInternalDataOffset;
	TVTPaintText FOnPaintText;
	TVSTGetTextEvent FOnGetText;
	TVSTGetHintEvent FOnGetHint;
	TVSTNewTextEvent FOnNewText;
	TVSTShortenStringEvent FOnShortenString;
	TVTMeasureTextWidthEvent FOnMeasureTextWidth;
	TVTDrawTextEvent FOnDrawText;
	WideString __fastcall GetImageText(PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column);
	void __fastcall GetRenderStartValues(TVSTTextSourceType Source, PVirtualNode &Node, TGetNextNodeProc &NextNodeProc);
	TCustomStringTreeOptions* __fastcall GetOptions(void);
	HIDESBASE WideString __fastcall GetText(PVirtualNode Node, TColumnIndex Column);
	void __fastcall InitializeTextProperties(TVTPaintInfo &PaintInfo);
	void __fastcall PaintNormalText(TVTPaintInfo &PaintInfo, int TextOutFlags, WideString Text);
	void __fastcall PaintStaticText(const TVTPaintInfo &PaintInfo, int TextOutFlags, const WideString Text);
	void __fastcall ReadText(Classes::TReader* Reader);
	void __fastcall SetDefaultText(const WideString Value);
	HIDESBASE void __fastcall SetOptions(const TCustomStringTreeOptions* Value);
	HIDESBASE void __fastcall SetText(PVirtualNode Node, TColumnIndex Column, const WideString Value);
	void __fastcall WriteText(Classes::TWriter* Writer);
	MESSAGE void __fastcall WMSetFont(Messages::TWMSetFont &Msg);
	
protected:
	virtual void __fastcall AdjustPaintCellRect(TVTPaintInfo &PaintInfo, TColumnIndex &NextNonEmpty);
	bool __fastcall CanExportNode(PVirtualNode Node);
	virtual int __fastcall CalculateTextWidth(Graphics::TCanvas* Canvas, PVirtualNode Node, TColumnIndex Column, WideString Text);
	virtual bool __fastcall ColumnIsEmpty(PVirtualNode Node, TColumnIndex Column);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	virtual _di_IVTEditLink __fastcall DoCreateEditor(PVirtualNode Node, TColumnIndex Column);
	virtual WideString __fastcall DoGetNodeHint(PVirtualNode Node, TColumnIndex Column, TVTTooltipLineBreakStyle &LineBreakStyle);
	virtual WideString __fastcall DoGetNodeTooltip(PVirtualNode Node, TColumnIndex Column, TVTTooltipLineBreakStyle &LineBreakStyle);
	virtual int __fastcall DoGetNodeWidth(PVirtualNode Node, TColumnIndex Column, Graphics::TCanvas* Canvas = (Graphics::TCanvas*)(0x0));
	virtual void __fastcall DoGetText(PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType, WideString &Text);
	virtual int __fastcall DoIncrementalSearch(PVirtualNode Node, const WideString Text);
	virtual void __fastcall DoNewText(PVirtualNode Node, TColumnIndex Column, WideString Text);
	virtual void __fastcall DoPaintNode(TVTPaintInfo &PaintInfo);
	virtual void __fastcall DoPaintText(PVirtualNode Node, const Graphics::TCanvas* Canvas, TColumnIndex Column, TVSTTextType TextType);
	virtual WideString __fastcall DoShortenString(Graphics::TCanvas* Canvas, PVirtualNode Node, TColumnIndex Column, const WideString S, int Width, int EllipsisWidth = 0x0);
	virtual void __fastcall DoTextDrawing(TVTPaintInfo &PaintInfo, WideString Text, const Types::TRect &CellRect, unsigned DrawFormat);
	virtual int __fastcall DoTextMeasuring(Graphics::TCanvas* Canvas, PVirtualNode Node, TColumnIndex Column, WideString Text);
	virtual TMetaClass* __fastcall GetOptionsClass(void);
	HIDESBASE void * __fastcall InternalData(PVirtualNode Node);
	virtual void __fastcall MainColumnChanged(void);
	virtual bool __fastcall ReadChunk(Classes::TStream* Stream, int Version, PVirtualNode Node, int ChunkType, int ChunkSize);
	void __fastcall ReadOldStringOptions(Classes::TReader* Reader);
	virtual HRESULT __fastcall RenderOLEData(const tagFORMATETC &FormatEtcIn, /* out */ tagSTGMEDIUM &Medium, bool ForClipboard);
	virtual void __fastcall WriteChunks(Classes::TStream* Stream, PVirtualNode Node);
	__property WideString DefaultText = {read=FDefaultText, write=SetDefaultText, stored=false};
	__property int EllipsisWidth = {read=FEllipsisWidth, nodefault};
	__property TCustomStringTreeOptions* TreeOptions = {read=GetOptions, write=SetOptions};
	__property TVSTGetHintEvent OnGetHint = {read=FOnGetHint, write=FOnGetHint};
	__property TVSTGetTextEvent OnGetText = {read=FOnGetText, write=FOnGetText};
	__property TVSTNewTextEvent OnNewText = {read=FOnNewText, write=FOnNewText};
	__property TVTPaintText OnPaintText = {read=FOnPaintText, write=FOnPaintText};
	__property TVSTShortenStringEvent OnShortenString = {read=FOnShortenString, write=FOnShortenString};
	__property TVTMeasureTextWidthEvent OnMeasureTextWidth = {read=FOnMeasureTextWidth, write=FOnMeasureTextWidth};
	__property TVTDrawTextEvent OnDrawText = {read=FOnDrawText, write=FOnDrawText};
	
public:
	__fastcall virtual TCustomVirtualStringTree(Classes::TComponent* AOwner);
	virtual int __fastcall ComputeNodeHeight(Graphics::TCanvas* Canvas, PVirtualNode Node, TColumnIndex Column, WideString S = L"");
	unsigned __fastcall ContentToClipboard(Word Format, TVSTTextSourceType Source);
	void __fastcall ContentToCustom(TVSTTextSourceType Source);
	AnsiString __fastcall ContentToHTML(TVSTTextSourceType Source, WideString Caption = L"");
	AnsiString __fastcall ContentToRTF(TVSTTextSourceType Source);
	AnsiString __fastcall ContentToText(TVSTTextSourceType Source, char Separator)/* overload */;
	AnsiString __fastcall ContentToText(TVSTTextSourceType Source, const AnsiString Separator)/* overload */;
	WideString __fastcall ContentToUnicode(TVSTTextSourceType Source, wchar_t Separator)/* overload */;
	WideString __fastcall ContentToUnicode(TVSTTextSourceType Source, const WideString Separator)/* overload */;
	virtual void __fastcall GetTextInfo(PVirtualNode Node, TColumnIndex Column, const Graphics::TFont* AFont, Types::TRect &R, WideString &Text);
	virtual Types::TRect __fastcall InvalidateNode(PVirtualNode Node);
	WideString __fastcall Path(PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType, wchar_t Delimiter);
	virtual void __fastcall ReinitNode(PVirtualNode Node, bool Recursive);
	__property WideString ImageText[PVirtualNode Node][TVTImageKind Kind][TColumnIndex Column] = {read=GetImageText};
	__property WideString Text[PVirtualNode Node][TColumnIndex Column] = {read=GetText, write=SetText};
public:
	#pragma option push -w-inl
	/* TBaseVirtualTree.Destroy */ inline __fastcall virtual ~TCustomVirtualStringTree(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TCustomVirtualStringTree(HWND ParentWindow) : TBaseVirtualTree(ParentWindow) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TStringEditLink : public System::TInterfacedObject 
{
	typedef System::TInterfacedObject inherited;
	
private:
	TVTEdit* FEdit;
	void __fastcall SetEdit(const TVTEdit* Value);
	
protected:
	TCustomVirtualStringTree* FTree;
	TVirtualNode *FNode;
	TColumnIndex FColumn;
	Classes::TAlignment FAlignment;
	#pragma pack(push, 1)
	Types::TRect FTextBounds;
	#pragma pack(pop)
	
	bool FStopping;
	
public:
	__fastcall virtual TStringEditLink(void);
	__fastcall virtual ~TStringEditLink(void);
	virtual bool __stdcall BeginEdit(void);
	virtual bool __stdcall CancelEdit(void);
	__property TVTEdit* Edit = {read=FEdit, write=SetEdit};
	virtual bool __stdcall EndEdit(void);
	virtual Types::TRect __stdcall GetBounds();
	virtual bool __stdcall PrepareEdit(TBaseVirtualTree* Tree, PVirtualNode Node, TColumnIndex Column);
	virtual void __stdcall ProcessMessage(Messages::TMessage &Message);
	virtual void __stdcall SetBounds(const Types::TRect R);
private:
	void *__IVTEditLink;	/* Virtualtrees::IVTEditLink */
	
public:
	operator IVTEditLink*(void) { return (IVTEditLink*)&__IVTEditLink; }
	
};


class PASCALIMPLEMENTATION TVTEdit : public Stdctrls::TCustomEdit 
{
	typedef Stdctrls::TCustomEdit inherited;
	
private:
	MESSAGE void __fastcall CMAutoAdjust(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMExit(Messages::TMessage &Message);
	MESSAGE void __fastcall CMRelease(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CNCommand(Messages::TWMCommand &Message);
	HIDESBASE MESSAGE void __fastcall WMChar(Messages::TWMKey &Message);
	HIDESBASE MESSAGE void __fastcall WMDestroy(Messages::TWMNoParams &Message);
	MESSAGE void __fastcall WMGetDlgCode(Messages::TWMNoParams &Message);
	HIDESBASE MESSAGE void __fastcall WMKeyDown(Messages::TWMKey &Message);
	
protected:
	_di_IVTEditLink FRefLink;
	TStringEditLink* FLink;
	virtual void __fastcall AutoAdjustSize(void);
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	
public:
	__fastcall TVTEdit(TStringEditLink* Link);
	virtual void __fastcall Release(void);
	__property AutoSelect  = {default=1};
	__property AutoSize  = {default=1};
	__property BorderStyle  = {default=1};
	__property CharCase  = {default=0};
	__property HideSelection  = {default=1};
	__property MaxLength  = {default=0};
	__property OEMConvert  = {default=0};
	__property PasswordChar  = {default=0};
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TVTEdit(HWND ParentWindow) : Stdctrls::TCustomEdit(ParentWindow) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TWinControl.Destroy */ inline __fastcall virtual ~TVTEdit(void) { }
	#pragma option pop
	
};


class DELPHICLASS TVirtualStringTree;
class PASCALIMPLEMENTATION TVirtualStringTree : public TCustomVirtualStringTree 
{
	typedef TCustomVirtualStringTree inherited;
	
private:
	HIDESBASE TStringTreeOptions* __fastcall GetOptions(void);
	HIDESBASE void __fastcall SetOptions(const TStringTreeOptions* Value);
	
protected:
	virtual TMetaClass* __fastcall GetOptionsClass(void);
	
public:
	__property Canvas ;
	
__published:
	__property AccessibleName ;
	__property Action ;
	__property Align  = {default=0};
	__property Alignment  = {default=0};
	__property Anchors  = {default=3};
	__property AnimationDuration  = {default=200};
	__property AutoExpandDelay  = {default=1000};
	__property AutoScrollDelay  = {default=1000};
	__property AutoScrollInterval  = {default=1};
	__property Background ;
	__property BackgroundOffsetX  = {index=0, default=0};
	__property BackgroundOffsetY  = {index=1, default=0};
	__property BiDiMode ;
	__property BevelEdges  = {default=15};
	__property BevelInner  = {index=0, default=2};
	__property BevelOuter  = {index=1, default=1};
	__property BevelKind  = {default=0};
	__property BevelWidth  = {default=1};
	__property BorderStyle  = {default=1};
	__property BottomSpace  = {default=0};
	__property ButtonFillMode  = {default=0};
	__property ButtonStyle  = {default=0};
	__property BorderWidth  = {default=0};
	__property ChangeDelay  = {default=0};
	__property CheckImageKind  = {default=8};
	__property ClipboardFormats ;
	__property Color  = {default=-2147483643};
	__property Colors ;
	__property Constraints ;
	__property Ctl3D ;
	__property CustomCheckImages ;
	__property DefaultNodeHeight  = {default=18};
	__property DefaultPasteMode  = {default=4};
	__property DefaultText ;
	__property DragCursor  = {default=-12};
	__property DragHeight  = {default=350};
	__property DragKind  = {default=0};
	__property DragImageKind  = {default=0};
	__property DragMode  = {default=0};
	__property DragOperations  = {default=3};
	__property DragType  = {default=0};
	__property DragWidth  = {default=200};
	__property DrawSelectionMode  = {default=0};
	__property EditDelay  = {default=1000};
	__property Enabled  = {default=1};
	__property Font ;
	__property Header ;
	__property HintAnimation  = {default=3};
	__property HintMode  = {default=0};
	__property HotCursor  = {default=0};
	__property Images ;
	__property IncrementalSearch  = {default=1};
	__property IncrementalSearchDirection  = {default=0};
	__property IncrementalSearchStart  = {default=2};
	__property IncrementalSearchTimeout  = {default=1000};
	__property Indent  = {default=18};
	__property LineMode  = {default=0};
	__property LineStyle  = {default=1};
	__property Margin  = {default=4};
	__property NodeAlignment  = {default=2};
	__property NodeDataSize  = {default=-1};
	__property OperationCanceled ;
	__property ParentBiDiMode  = {default=1};
	__property ParentColor  = {default=0};
	__property ParentCtl3D  = {default=1};
	__property ParentFont  = {default=1};
	__property ParentShowHint  = {default=1};
	__property PopupMenu ;
	__property RootNodeCount  = {default=0};
	__property ScrollBarOptions ;
	__property SelectionBlendFactor  = {default=128};
	__property SelectionCurveRadius  = {default=0};
	__property ShowHint ;
	__property StateImages ;
	__property TabOrder  = {default=-1};
	__property TabStop  = {default=1};
	__property TextMargin  = {default=4};
	__property TStringTreeOptions* TreeOptions = {read=GetOptions, write=SetOptions};
	__property Visible  = {default=1};
	__property WantTabs  = {default=0};
	__property OnAdvancedHeaderDraw ;
	__property OnAfterAutoFitColumn ;
	__property OnAfterAutoFitColumns ;
	__property OnAfterCellPaint ;
	__property OnAfterColumnExport ;
	__property OnAfterColumnWidthTracking ;
	__property OnAfterGetMaxColumnWidth ;
	__property OnAfterHeaderExport ;
	__property OnAfterHeaderHeightTracking ;
	__property OnAfterItemErase ;
	__property OnAfterItemPaint ;
	__property OnAfterNodeExport ;
	__property OnAfterPaint ;
	__property OnAfterTreeExport ;
	__property OnBeforeAutoFitColumn ;
	__property OnBeforeAutoFitColumns ;
	__property OnBeforeCellPaint ;
	__property OnBeforeColumnExport ;
	__property OnBeforeColumnWidthTracking ;
	__property OnBeforeGetMaxColumnWidth ;
	__property OnBeforeHeaderExport ;
	__property OnBeforeHeaderHeightTracking ;
	__property OnBeforeItemErase ;
	__property OnBeforeItemPaint ;
	__property OnBeforeNodeExport ;
	__property OnBeforePaint ;
	__property OnBeforeTreeExport ;
	__property OnCanSplitterResizeColumn ;
	__property OnChange ;
	__property OnChecked ;
	__property OnChecking ;
	__property OnClick ;
	__property OnCollapsed ;
	__property OnCollapsing ;
	__property OnColumnClick ;
	__property OnColumnDblClick ;
	__property OnColumnExport ;
	__property OnColumnResize ;
	__property OnColumnWidthDblClickResize ;
	__property OnColumnWidthTracking ;
	__property OnCompareNodes ;
	__property OnContextPopup ;
	__property OnCreateDataObject ;
	__property OnCreateDragManager ;
	__property OnCreateEditor ;
	__property OnDblClick ;
	__property OnDragAllowed ;
	__property OnDragOver ;
	__property OnDragDrop ;
	__property OnDrawText ;
	__property OnEditCancelled ;
	__property OnEdited ;
	__property OnEditing ;
	__property OnEndDock ;
	__property OnEndDrag ;
	__property OnEnter ;
	__property OnExit ;
	__property OnExpanded ;
	__property OnExpanding ;
	__property OnFocusChanged ;
	__property OnFocusChanging ;
	__property OnFreeNode ;
	__property OnGetCellIsEmpty ;
	__property OnGetCursor ;
	__property OnGetHeaderCursor ;
	__property OnGetText ;
	__property OnPaintText ;
	__property OnGetHelpContext ;
	__property OnGetImageIndex ;
	__property OnGetImageIndexEx ;
	__property OnGetImageText ;
	__property OnGetHint ;
	__property OnGetLineStyle ;
	__property OnGetNodeDataSize ;
	__property OnGetPopupMenu ;
	__property OnGetUserClipboardFormats ;
	__property OnHeaderCheckBoxClick ;
	__property OnHeaderClick ;
	__property OnHeaderDblClick ;
	__property OnHeaderDragged ;
	__property OnHeaderDraggedOut ;
	__property OnHeaderDragging ;
	__property OnHeaderDraw ;
	__property OnHeaderDrawQueryElements ;
	__property OnHeaderHeightDblClickResize ;
	__property OnHeaderHeightTracking ;
	__property OnHeaderImageClick ;
	__property OnHeaderMouseDown ;
	__property OnHeaderMouseMove ;
	__property OnHeaderMouseUp ;
	__property OnHotChange ;
	__property OnIncrementalSearch ;
	__property OnInitChildren ;
	__property OnInitNode ;
	__property OnKeyAction ;
	__property OnKeyDown ;
	__property OnKeyPress ;
	__property OnKeyUp ;
	__property OnLoadNode ;
	__property OnMeasureItem ;
	__property OnMeasureTextWidth ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnMouseWheel ;
	__property OnNewText ;
	__property OnNodeCopied ;
	__property OnNodeCopying ;
	__property OnNodeExport ;
	__property OnNodeHeightDblClickResize ;
	__property OnNodeHeightTracking ;
	__property OnNodeMoved ;
	__property OnNodeMoving ;
	__property OnPaintBackground ;
	__property OnRenderOLEData ;
	__property OnResetNode ;
	__property OnResize ;
	__property OnSaveNode ;
	__property OnScroll ;
	__property OnShortenString ;
	__property OnShowScrollbar ;
	__property OnStartDock ;
	__property OnStartDrag ;
	__property OnStateChange ;
	__property OnStructureChange ;
	__property OnUpdating ;
public:
	#pragma option push -w-inl
	/* TCustomVirtualStringTree.Create */ inline __fastcall virtual TVirtualStringTree(Classes::TComponent* AOwner) : TCustomVirtualStringTree(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TBaseVirtualTree.Destroy */ inline __fastcall virtual ~TVirtualStringTree(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TVirtualStringTree(HWND ParentWindow) : TCustomVirtualStringTree(ParentWindow) { }
	#pragma option pop
	
};


typedef void __fastcall (__closure *TVTDrawHintEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* HintCanvas, PVirtualNode Node, const Types::TRect &R, TColumnIndex Column);

typedef void __fastcall (__closure *TVTDrawNodeEvent)(TBaseVirtualTree* Sender, const TVTPaintInfo &PaintInfo);

typedef void __fastcall (__closure *TVTGetCellContentMarginEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* HintCanvas, PVirtualNode Node, TColumnIndex Column, TVTCellContentMarginType CellContentMarginType, Types::TPoint &CellContentMargin);

typedef void __fastcall (__closure *TVTGetNodeWidthEvent)(TBaseVirtualTree* Sender, Graphics::TCanvas* HintCanvas, PVirtualNode Node, TColumnIndex Column, int &NodeWidth);

typedef void __fastcall (__closure *TVTGetHintSizeEvent)(TBaseVirtualTree* Sender, PVirtualNode Node, TColumnIndex Column, Types::TRect &R);

class DELPHICLASS TCustomVirtualDrawTree;
class PASCALIMPLEMENTATION TCustomVirtualDrawTree : public TBaseVirtualTree 
{
	typedef TBaseVirtualTree inherited;
	
private:
	TVTDrawNodeEvent FOnDrawNode;
	TVTGetCellContentMarginEvent FOnGetCellContentMargin;
	TVTGetNodeWidthEvent FOnGetNodeWidth;
	TVTGetHintSizeEvent FOnGetHintSize;
	TVTDrawHintEvent FOnDrawHint;
	
protected:
	void __fastcall DoDrawHint(Graphics::TCanvas* Canvas, PVirtualNode Node, const Types::TRect &R, TColumnIndex Column);
	virtual Types::TPoint __fastcall DoGetCellContentMargin(PVirtualNode Node, TColumnIndex Column, TVTCellContentMarginType CellContentMarginType = (TVTCellContentMarginType)(0x0), Graphics::TCanvas* Canvas = (Graphics::TCanvas*)(0x0));
	virtual void __fastcall DoGetHintSize(PVirtualNode Node, TColumnIndex Column, Types::TRect &R);
	virtual int __fastcall DoGetNodeWidth(PVirtualNode Node, TColumnIndex Column, Graphics::TCanvas* Canvas = (Graphics::TCanvas*)(0x0));
	virtual void __fastcall DoPaintNode(TVTPaintInfo &PaintInfo);
	__property TVTDrawHintEvent OnDrawHint = {read=FOnDrawHint, write=FOnDrawHint};
	__property TVTDrawNodeEvent OnDrawNode = {read=FOnDrawNode, write=FOnDrawNode};
	__property TVTGetCellContentMarginEvent OnGetCellContentMargin = {read=FOnGetCellContentMargin, write=FOnGetCellContentMargin};
	__property TVTGetHintSizeEvent OnGetHintSize = {read=FOnGetHintSize, write=FOnGetHintSize};
	__property TVTGetNodeWidthEvent OnGetNodeWidth = {read=FOnGetNodeWidth, write=FOnGetNodeWidth};
public:
	#pragma option push -w-inl
	/* TBaseVirtualTree.Create */ inline __fastcall virtual TCustomVirtualDrawTree(Classes::TComponent* AOwner) : TBaseVirtualTree(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBaseVirtualTree.Destroy */ inline __fastcall virtual ~TCustomVirtualDrawTree(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TCustomVirtualDrawTree(HWND ParentWindow) : TBaseVirtualTree(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TVirtualDrawTree;
class PASCALIMPLEMENTATION TVirtualDrawTree : public TCustomVirtualDrawTree 
{
	typedef TCustomVirtualDrawTree inherited;
	
private:
	TVirtualTreeOptions* __fastcall GetOptions(void);
	HIDESBASE void __fastcall SetOptions(const TVirtualTreeOptions* Value);
	
protected:
	virtual TMetaClass* __fastcall GetOptionsClass(void);
	
public:
	__property Canvas ;
	
__published:
	__property Action ;
	__property Align  = {default=0};
	__property Alignment  = {default=0};
	__property Anchors  = {default=3};
	__property AnimationDuration  = {default=200};
	__property AutoExpandDelay  = {default=1000};
	__property AutoScrollDelay  = {default=1000};
	__property AutoScrollInterval  = {default=1};
	__property Background ;
	__property BackgroundOffsetX  = {index=0, default=0};
	__property BackgroundOffsetY  = {index=1, default=0};
	__property BiDiMode ;
	__property BevelEdges  = {default=15};
	__property BevelInner  = {index=0, default=2};
	__property BevelOuter  = {index=1, default=1};
	__property BevelKind  = {default=0};
	__property BevelWidth  = {default=1};
	__property BorderStyle  = {default=1};
	__property BottomSpace  = {default=0};
	__property ButtonFillMode  = {default=0};
	__property ButtonStyle  = {default=0};
	__property BorderWidth  = {default=0};
	__property ChangeDelay  = {default=0};
	__property CheckImageKind  = {default=8};
	__property ClipboardFormats ;
	__property Color  = {default=-2147483643};
	__property Colors ;
	__property Constraints ;
	__property Ctl3D ;
	__property CustomCheckImages ;
	__property DefaultNodeHeight  = {default=18};
	__property DefaultPasteMode  = {default=4};
	__property DragCursor  = {default=-12};
	__property DragHeight  = {default=350};
	__property DragKind  = {default=0};
	__property DragImageKind  = {default=0};
	__property DragMode  = {default=0};
	__property DragOperations  = {default=3};
	__property DragType  = {default=0};
	__property DragWidth  = {default=200};
	__property DrawSelectionMode  = {default=0};
	__property EditDelay  = {default=1000};
	__property Enabled  = {default=1};
	__property Font ;
	__property Header ;
	__property HintAnimation  = {default=3};
	__property HintMode  = {default=0};
	__property HotCursor  = {default=0};
	__property Images ;
	__property IncrementalSearch  = {default=1};
	__property IncrementalSearchDirection  = {default=0};
	__property IncrementalSearchStart  = {default=2};
	__property IncrementalSearchTimeout  = {default=1000};
	__property Indent  = {default=18};
	__property LineMode  = {default=0};
	__property LineStyle  = {default=1};
	__property Margin  = {default=4};
	__property NodeAlignment  = {default=2};
	__property NodeDataSize  = {default=-1};
	__property OperationCanceled ;
	__property ParentBiDiMode  = {default=1};
	__property ParentColor  = {default=0};
	__property ParentCtl3D  = {default=1};
	__property ParentFont  = {default=1};
	__property ParentShowHint  = {default=1};
	__property PopupMenu ;
	__property RootNodeCount  = {default=0};
	__property ScrollBarOptions ;
	__property SelectionBlendFactor  = {default=128};
	__property SelectionCurveRadius  = {default=0};
	__property ShowHint ;
	__property StateImages ;
	__property TabOrder  = {default=-1};
	__property TabStop  = {default=1};
	__property TextMargin  = {default=4};
	__property TVirtualTreeOptions* TreeOptions = {read=GetOptions, write=SetOptions};
	__property Visible  = {default=1};
	__property WantTabs  = {default=0};
	__property OnAdvancedHeaderDraw ;
	__property OnAfterAutoFitColumn ;
	__property OnAfterAutoFitColumns ;
	__property OnAfterCellPaint ;
	__property OnAfterColumnExport ;
	__property OnAfterColumnWidthTracking ;
	__property OnAfterGetMaxColumnWidth ;
	__property OnAfterHeaderExport ;
	__property OnAfterHeaderHeightTracking ;
	__property OnAfterItemErase ;
	__property OnAfterItemPaint ;
	__property OnAfterNodeExport ;
	__property OnAfterPaint ;
	__property OnAfterTreeExport ;
	__property OnBeforeAutoFitColumn ;
	__property OnBeforeAutoFitColumns ;
	__property OnBeforeCellPaint ;
	__property OnBeforeColumnExport ;
	__property OnBeforeColumnWidthTracking ;
	__property OnBeforeGetMaxColumnWidth ;
	__property OnBeforeHeaderExport ;
	__property OnBeforeHeaderHeightTracking ;
	__property OnBeforeItemErase ;
	__property OnBeforeItemPaint ;
	__property OnBeforeNodeExport ;
	__property OnBeforePaint ;
	__property OnBeforeTreeExport ;
	__property OnCanSplitterResizeColumn ;
	__property OnChange ;
	__property OnChecked ;
	__property OnChecking ;
	__property OnClick ;
	__property OnCollapsed ;
	__property OnCollapsing ;
	__property OnColumnClick ;
	__property OnColumnDblClick ;
	__property OnColumnExport ;
	__property OnColumnResize ;
	__property OnColumnWidthDblClickResize ;
	__property OnColumnWidthTracking ;
	__property OnCompareNodes ;
	__property OnContextPopup ;
	__property OnCreateDataObject ;
	__property OnCreateDragManager ;
	__property OnCreateEditor ;
	__property OnDblClick ;
	__property OnDragAllowed ;
	__property OnDragOver ;
	__property OnDragDrop ;
	__property OnDrawHint ;
	__property OnDrawNode ;
	__property OnEdited ;
	__property OnEditing ;
	__property OnEndDock ;
	__property OnEndDrag ;
	__property OnEnter ;
	__property OnExit ;
	__property OnExpanded ;
	__property OnExpanding ;
	__property OnFocusChanged ;
	__property OnFocusChanging ;
	__property OnFreeNode ;
	__property OnGetCellIsEmpty ;
	__property OnGetCursor ;
	__property OnGetHeaderCursor ;
	__property OnGetHelpContext ;
	__property OnGetHintSize ;
	__property OnGetImageIndex ;
	__property OnGetImageIndexEx ;
	__property OnGetLineStyle ;
	__property OnGetNodeDataSize ;
	__property OnGetNodeWidth ;
	__property OnGetPopupMenu ;
	__property OnGetUserClipboardFormats ;
	__property OnHeaderCheckBoxClick ;
	__property OnHeaderClick ;
	__property OnHeaderDblClick ;
	__property OnHeaderDragged ;
	__property OnHeaderDraggedOut ;
	__property OnHeaderDragging ;
	__property OnHeaderDraw ;
	__property OnHeaderDrawQueryElements ;
	__property OnHeaderHeightTracking ;
	__property OnHeaderHeightDblClickResize ;
	__property OnHeaderImageClick ;
	__property OnHeaderMouseDown ;
	__property OnHeaderMouseMove ;
	__property OnHeaderMouseUp ;
	__property OnHotChange ;
	__property OnIncrementalSearch ;
	__property OnInitChildren ;
	__property OnInitNode ;
	__property OnKeyAction ;
	__property OnKeyDown ;
	__property OnKeyPress ;
	__property OnKeyUp ;
	__property OnLoadNode ;
	__property OnMeasureItem ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnMouseWheel ;
	__property OnNodeCopied ;
	__property OnNodeCopying ;
	__property OnNodeExport ;
	__property OnNodeHeightTracking ;
	__property OnNodeHeightDblClickResize ;
	__property OnNodeMoved ;
	__property OnNodeMoving ;
	__property OnPaintBackground ;
	__property OnRenderOLEData ;
	__property OnResetNode ;
	__property OnResize ;
	__property OnSaveNode ;
	__property OnScroll ;
	__property OnShowScrollbar ;
	__property OnStartDock ;
	__property OnStartDrag ;
	__property OnStateChange ;
	__property OnStructureChange ;
	__property OnUpdating ;
public:
	#pragma option push -w-inl
	/* TBaseVirtualTree.Create */ inline __fastcall virtual TVirtualDrawTree(Classes::TComponent* AOwner) : TCustomVirtualDrawTree(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TBaseVirtualTree.Destroy */ inline __fastcall virtual ~TVirtualDrawTree(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TVirtualDrawTree(HWND ParentWindow) : TCustomVirtualDrawTree(ParentWindow) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TBlendMode { bmConstantAlpha, bmPerPixelAlpha, bmMasterAlpha, bmConstantAlphaAndColor };
#pragma option pop

//-- var, const, procedure ---------------------------------------------------
#define VTVersion "4.8.7"
static const Shortint VTTreeStreamVersion = 0x2;
static const Shortint VTHeaderStreamVersion = 0x6;
static const Word CacheThreshold = 0x7d0;
static const Byte FadeAnimationStepCount = 0xff;
static const Shortint ShadowSize = 0x5;
static const Shortint NoColumn = 0xffffffff;
static const Shortint InvalidColumn = 0xfffffffe;
static const Shortint ckEmpty = 0x0;
static const Shortint ckRadioUncheckedNormal = 0x1;
static const Shortint ckRadioUncheckedHot = 0x2;
static const Shortint ckRadioUncheckedPressed = 0x3;
static const Shortint ckRadioUncheckedDisabled = 0x4;
static const Shortint ckRadioCheckedNormal = 0x5;
static const Shortint ckRadioCheckedHot = 0x6;
static const Shortint ckRadioCheckedPressed = 0x7;
static const Shortint ckRadioCheckedDisabled = 0x8;
static const Shortint ckCheckUncheckedNormal = 0x9;
static const Shortint ckCheckUncheckedHot = 0xa;
static const Shortint ckCheckUncheckedPressed = 0xb;
static const Shortint ckCheckUncheckedDisabled = 0xc;
static const Shortint ckCheckCheckedNormal = 0xd;
static const Shortint ckCheckCheckedHot = 0xe;
static const Shortint ckCheckCheckedPressed = 0xf;
static const Shortint ckCheckCheckedDisabled = 0x10;
static const Shortint ckCheckMixedNormal = 0x11;
static const Shortint ckCheckMixedHot = 0x12;
static const Shortint ckCheckMixedPressed = 0x13;
static const Shortint ckCheckMixedDisabled = 0x14;
static const Shortint ckButtonNormal = 0x15;
static const Shortint ckButtonHot = 0x16;
static const Shortint ckButtonPressed = 0x17;
static const Shortint ckButtonDisabled = 0x18;
static const Shortint ExpandTimer = 0x1;
static const Shortint EditTimer = 0x2;
static const Shortint HeaderTimer = 0x3;
static const Shortint ScrollTimer = 0x4;
static const Shortint ChangeTimer = 0x5;
static const Shortint StructureChangeTimer = 0x6;
static const Shortint SearchTimer = 0x7;
static const Word WM_CHANGESTATE = 0x8020;
static const Word CM_DENYSUBCLASSING = 0xb7d0;
static const Word CM_AUTOADJUST = 0xb7d5;
#define CFSTR_VIRTUALTREE "Virtual Tree Data"
#define CFSTR_VTREFERENCE "Virtual Tree Reference"
#define CFSTR_HTML "HTML Format"
#define CFSTR_RTF "Rich Text Format"
#define CFSTR_RTFNOOBJS "Rich Text Format Without Objects"
#define CFSTR_CSV "CSV"
extern PACKAGE GUID IID_IDropTargetHelper;
extern PACKAGE GUID IID_IDragSourceHelper;
extern PACKAGE GUID IID_IDropTarget;
extern PACKAGE GUID CLSID_DragDropHelper;
#define SID_IDropTargetHelper "{4657278B-411B-11D2-839A-00C04FD918D0}"
#define SID_IDragSourceHelper "{DE5BF786-477A-11D2-839D-00C04FD918D0}"
#define SID_IDropTarget "{00000122-0000-0000-C000-000000000046}"
static const Word hcTFEditLinkIsNil = 0x7d0;
static const Word hcTFWrongMoveError = 0x7d1;
static const Word hcTFWrongStreamFormat = 0x7d2;
static const Word hcTFWrongStreamVersion = 0x7d3;
static const Word hcTFStreamTooSmall = 0x7d4;
static const Word hcTFCorruptStream1 = 0x7d5;
static const Word hcTFCorruptStream2 = 0x7d6;
static const Word hcTFClipboardFailed = 0x7d7;
static const Word hcTFCannotSetUserData = 0x7d8;
static const Controls::TCursor crHeaderSplit = 0x3f;
static const Controls::TCursor crVertSplit = 0x3e;
static const Shortint UtilityImageSize = 0x10;
extern PACKAGE Word CF_VIRTUALTREE;
extern PACKAGE Word CF_VTREFERENCE;
extern PACKAGE Word CF_VRTF;
extern PACKAGE Word CF_VRTFNOOBJS;
extern PACKAGE Word CF_HTML;
extern PACKAGE Word CF_CSV;
extern PACKAGE bool MMXAvailable;
extern PACKAGE bool IsWinNT;
extern PACKAGE bool IsWin2K;
extern PACKAGE bool IsWinXP;
extern PACKAGE bool IsWinVistaOrAbove;
#define DefaultPaintOptions (System::Set<TVTPaintOption, toHideFocusRect, toUseExplorerTheme> () << TVTPaintOption(5) << TVTPaintOption(6) << TVTPaintOption(8) << TVTPaintOption(9) << TVTPaintOption(11) << TVTPaintOption(12) )
#define DefaultAnimationOptions EMPTYSET
#define DefaultAutoOptions (System::Set<TVTAutoOption, toAutoDropExpand, toAutoBidiColumnOrdering> () << TVTAutoOption(0) << TVTAutoOption(3) << TVTAutoOption(6) << TVTAutoOption(8) )
#define DefaultSelectionOptions EMPTYSET
#define DefaultMiscOptions (System::Set<TVTMiscOption, toAcceptOLEDrop, toEditOnDblClick> () << TVTMiscOption(0) << TVTMiscOption(3) << TVTMiscOption(5) << TVTMiscOption(7) << TVTMiscOption(8) << TVTMiscOption(14) )
#define DefaultColumnOptions (System::Set<TVTColumnOption, coAllowClick, coUseCaptionAlignment> () << TVTColumnOption(0) << TVTColumnOption(1) << TVTColumnOption(2) << TVTColumnOption(3) << TVTColumnOption(4) << TVTColumnOption(5) << TVTColumnOption(6) << TVTColumnOption(7) << TVTColumnOption(11) )
#define DefaultStringOptions (System::Set<TVTStringOption, toSaveCaptions, toAutoAcceptEditChange> () << TVTStringOption(0) << TVTStringOption(2) )
extern PACKAGE void __fastcall EnumerateVTClipboardFormats(TMetaClass* TreeClass, const Classes::TStrings* List)/* overload */;
extern PACKAGE void __fastcall EnumerateVTClipboardFormats(TMetaClass* TreeClass, TFormatEtcArray &Formats)/* overload */;
extern PACKAGE AnsiString __fastcall GetVTClipboardFormatDescription(Word AFormat);
extern PACKAGE void __fastcall RegisterVTClipboardFormat(Word AFormat, TMetaClass* TreeClass, unsigned Priority)/* overload */;
extern PACKAGE Word __fastcall RegisterVTClipboardFormat(AnsiString Description, TMetaClass* TreeClass, unsigned Priority, int tymed = 0x1, Activex::PDVTargetDevice ptd = (void *)(0x0), int dwAspect = 0x1, int lindex = 0xffffffff)/* overload */;
extern PACKAGE TBaseVirtualTree* __fastcall TreeFromNode(PVirtualNode Node);
extern PACKAGE void __fastcall DrawTextW(HDC DC, wchar_t * lpString, int nCount, Types::TRect &lpRect, unsigned uFormat, bool AdjustRight);
extern PACKAGE WideString __fastcall ShortenString(HDC DC, const WideString S, int Width, int EllipsisWidth = 0x0);
extern PACKAGE WideString __fastcall WrapString(HDC DC, const WideString S, const Types::TRect &Bounds, bool RTL, unsigned DrawFormat);
extern PACKAGE void __fastcall GetStringDrawRect(HDC DC, const WideString S, Types::TRect &Bounds, unsigned DrawFormat);
extern PACKAGE void __fastcall AlphaBlend(HDC Source, HDC Destination, const Types::TRect &R, const Types::TPoint &Target, TBlendMode Mode, int ConstantAlpha, int Bias);
extern PACKAGE void __fastcall PrtStretchDrawDIB(Graphics::TCanvas* Canvas, const Types::TRect &DestRect, Graphics::TBitmap* ABitmap);

}	/* namespace Virtualtrees */
using namespace Virtualtrees;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// VirtualTrees
