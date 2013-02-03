//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include "ThemeMgr.hpp"
#include "VirtualTrees.hpp"
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:    // IDE-managed Components
        TThemeManager *themeMgr;
        TVirtualStringTree *lstTree;
        TSplitter *sptTnB;
        TMemo *txtInfo;
        TOpenDialog *dlgOpen;
        TImageList *imgIcoLst;
        void __fastcall FormResize(TObject *Sender);
        void __fastcall lstTreeFreeNode(TBaseVirtualTree *Sender,
          PVirtualNode Node);
        void __fastcall lstTreeGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
        void __fastcall lstTreeGetImageIndex(TBaseVirtualTree *Sender,
          PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
          bool &Ghosted, int &ImageIndex);
        void __fastcall lstTreePaintText(TBaseVirtualTree *Sender,
          const TCanvas *TargetCanvas, PVirtualNode Node,
          TColumnIndex Column, TVSTTextType TextType);
        void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall lstTreeChange(TBaseVirtualTree *Sender,
          PVirtualNode Node);
private:        // User declarations
public:         // User declarations
        __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
