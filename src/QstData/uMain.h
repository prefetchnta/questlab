//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:    // IDE-managed Components
        TButton *btnHEX;
        TEdit *edtInput;
        TButton *btnSTR;
        TPageControl *pgeMain;
        TTabSheet *tabValue;
        TTabSheet *tabImage;
        TTabSheet *tabAudio;
        TTabSheet *tabData;
        TMemo *txtValue;
        void __fastcall btnHEXClick(TObject *Sender);
        void __fastcall btnSTRClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
private:        // User declarations
public:         // User declarations
        __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
