//---------------------------------------------------------------------------

#include <vcl.h>
#include "QstInfoz.h"
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
        : TForm(Owner)
{
    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 使用系统字体 */
    this->Font->Assign(Screen->MenuFont);

    /* 加载窗口位置配置 */
    misc_desk_init(WIN_ICONF, &x1, &y1, &ww, &hh,
                   this->Width, this->Height);
    this->Left   = x1;
    this->Top    = y1;
    this->Width  = ww;
    this->Height = hh;
    this->FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
    /* 区分左右区域 */
    txtInfoL->Width = (this->Width - sptLnR->Width) * 618 / 1000;
    sptLnR->MinSize = txtInfoL->Width * 382 / 1000;
}
//---------------------------------------------------------------------------
