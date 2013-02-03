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
        TEdit *edtNow;
        TButton *btnStop;
        TButton *btnPlay;
        TTimer *timPlay;
        TProgressBar *proPlay;
        void __fastcall btnStopClick(TObject *Sender);
        void __fastcall btnPlayClick(TObject *Sender);
        void __fastcall timPlayTimer(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
private:        // User declarations
public:         // User declarations
        __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
