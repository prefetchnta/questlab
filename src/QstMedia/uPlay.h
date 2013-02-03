//---------------------------------------------------------------------------

#ifndef uPlayH
#define uPlayH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmPlay : public TForm
{
__published:    // IDE-managed Components
        void __fastcall FormShow(TObject *Sender);
private:        // User declarations
public:         // User declarations
        __fastcall TfrmPlay(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPlay *frmPlay;
//---------------------------------------------------------------------------
#endif
