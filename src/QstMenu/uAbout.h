//---------------------------------------------------------------------------

#ifndef uAboutH
#define uAboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class   TfrmAbout : public TForm
{
__published:    // IDE-managed Components
        TImage *imgLogo;
        TButton *btnOK;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        void __fastcall btnOKClick(TObject *Sender);
private:        // User declarations
public:         // User declarations
        __fastcall TfrmAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
#endif
