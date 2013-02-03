//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <OleCtrls.hpp>
#include "SHDocVw_OCX.h"
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:    // IDE-managed Components
        TCppWebBrowser *cppWeb;
        void __fastcall cppWebBeforeNavigate2(TObject *Sender,
          LPDISPATCH pDisp, Variant *URL, Variant *Flags,
          Variant *TargetFrameName, Variant *PostData, Variant *Headers,
          VARIANT_BOOL *Cancel);
private:        // User declarations
public:         // User declarations
        __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
