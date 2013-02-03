//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uPlay.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmPlay *frmPlay;
//---------------------------------------------------------------------------
__fastcall TfrmPlay::TfrmPlay(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPlay::FormShow(TObject *Sender)
{
    RECT    full;
    int     x1, y1;
    int     ww, hh;
    int     sw, sh;

    /* 调整窗口显示位置 */
    x1 = this->Left;
    y1 = this->Top;
    ww = this->Width;
    hh = this->Height;
    if (SystemParametersInfo(SPI_GETWORKAREA, 0, &full, 0))
    {
        /* 取桌面大小 */
        sw = full.right - full.left;
        sh = full.bottom - full.top;
    }
    else
    {
        /* 失败, 取屏幕大小 */
        sw = GetSystemMetrics(SM_CXSCREEN);
        sh = GetSystemMetrics(SM_CYSCREEN);
    }

    /* 保证不超出边界 */
    sw -= 40;
    sh -= 40;
    if (x1 + ww > sw)
        this->Left = (ww >= sw) ? 0 : (sw - ww);
    if (y1 + hh > sh)
        this->Top  = (hh >= sh) ? 0 : (sh - hh);
}
//---------------------------------------------------------------------------
