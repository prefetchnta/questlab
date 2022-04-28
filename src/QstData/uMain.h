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
        TButton *btnLE;
        TEdit *edtInput;
        TButton *btnBE;
        TPageControl *pgeMain;
        TTabSheet *tabValue;
        TTabSheet *tabImage;
        TTabSheet *tabAudio;
        TTabSheet *tabData;
        TMemo *txtValue;
        TStaticText *txtClr332;
        TStaticText *txtClr444;
        TStaticText *txtClr555;
        TStaticText *txtClr565;
        TStaticText *txtClr888;
        TStaticText *txtClrA32;
        void __fastcall btnLEClick(TObject *Sender);
        void __fastcall btnBEClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
private:        // User declarations
public:         // User declarations
        __fastcall TfrmMain(TComponent* Owner);
        void __fastcall colorReset();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
