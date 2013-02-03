//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:    // IDE-managed Components
        TTrackBar *tbrCount;
        TEdit *edtCount;
        TEdit *edtDelay;
        TButton *btnAdd;
        TButton *btnSub;
        TButton *btnPlay;
        TButton *btnGoto;
        TTimer *timPlay;
        TButton *btnLast;
        TButton *btnFirst;
        void __fastcall tbrCountChange(TObject *Sender);
        void __fastcall btnAddClick(TObject *Sender);
        void __fastcall btnSubClick(TObject *Sender);
        void __fastcall btnPlayClick(TObject *Sender);
        void __fastcall btnGotoClick(TObject *Sender);
        void __fastcall timPlayTimer(TObject *Sender);
        void __fastcall btnFirstClick(TObject *Sender);
        void __fastcall btnLastClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
private:        // User declarations
public:         // User declarations
        __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
