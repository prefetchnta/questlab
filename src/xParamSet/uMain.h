//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:    // IDE-managed Components
        TPageControl *pgeMain;
        TTabSheet *tabQLAB;
        TButton *btnOK;
        TButton *btnCancel;
        TGroupBox *grpQstView2D;
        TGroupBox *grpQstMedia;
        TCheckBox *isQXMM_enabled;
        TCheckBox *isQXMM_loop_play;
        TColorDialog *dlgColor;
        TStaticText *txtQV2D_bkcolor;
        TStaticText *txtQV2D_def_keycolor;
        TLabel *Label1;
        TLabel *Label2;
        TCheckBox *isQV2D_is_blends;
        TCheckBox *isQV2D_is_center;
        TCheckBox *isQV2D_use_keycolor;
        TButton *btnApply;
        TCheckBox *isQV2D_use_keyalpha;
        TGroupBox *grpQstText;
        TLabeledEdit *edtQEDT_max_size_kb;
        TButton *btnQEDT_font;
        TFontDialog *dlgFont;
        void __fastcall btnOKClick(TObject *Sender);
        void __fastcall btnApplyClick(TObject *Sender);
        void __fastcall btnCancelClick(TObject *Sender);
        void __fastcall txtQV2D_bkcolorClick(TObject *Sender);
        void __fastcall txtQV2D_def_keycolorClick(TObject *Sender);
        void __fastcall btnQEDT_fontClick(TObject *Sender);
private:        // User declarations
public:         // User declarations
        __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
