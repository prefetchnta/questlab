//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uAbout.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
__fastcall TfrmAbout::TfrmAbout(TComponent* Owner)
        : TForm(Owner)
{
    /* 使用系统字体 */
    this->Font->Assign(Screen->MenuFont);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAbout::btnOKClick(TObject *Sender)
{
    /* 确定按钮 */
    this->Close();
}
//---------------------------------------------------------------------------
