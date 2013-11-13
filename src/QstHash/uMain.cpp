//---------------------------------------------------------------------------

#include <vcl.h>
#include "QstHash.h"
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
    this->Left = x1;
    this->Top  = y1;
    this->FormResize(NULL);

    /* 编辑框居中对齐 */
    SetWindowLong(edtInput->Handle, GWL_STYLE,
        GetWindowLong(edtInput->Handle, GWL_STYLE) | ES_CENTER);
    edtInput->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnHEXClick(TObject *Sender)
{
    /* 计算16进制哈希 */
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSTRClick(TObject *Sender)
{
    /* 计算字符串哈希 */
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::txtResultDblClick(TObject *Sender)
{
    /* 清除结果文本框 */
    txtResult->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
    sint_t  sx, sy, hh;

    /* 调整控件的位置 */
    sx = lstHash->Top;
    hh = edtInput->Height;
    if (sx < hh + 4)
        hh = sx - 4;
    sy = (sx - hh) / 2;
    btnHEX->Top   = sy;
    btnSTR->Top   = sy;
    edtInput->Top = sy;
    btnHEX->Height   = hh;
    btnSTR->Height   = hh;
    edtInput->Height = hh;
    sx = this->ClientWidth + lstHash->Left;
    sx = sx - txtResult->Left - txtResult->Width;
    sx = sx / 2 - lstHash->Left;
    btnHEX->Left    = btnHEX->Left    + sx;
    btnSTR->Left    = btnSTR->Left    + sx;
    edtInput->Left  = edtInput->Left  + sx;
    lstHash->Left   = lstHash->Left   + sx;
    txtResult->Left = txtResult->Left + sx;
}
//---------------------------------------------------------------------------
