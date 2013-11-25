//---------------------------------------------------------------------------

#include <vcl.h>
#include "QstData.h"
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
CR_API void_t   qst_data_view (const void_t *data, leng_t size,
                               const ansi_t *title);
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
    leng_t      len;
    void_t*     dat;
    AnsiString  str = edtInput->Text;

    /* 显示16进制数值 */
    txtValue->Clear();
    len = str.Length();
    if (len == 0)
        return;
    len += 1;
    if (len > 1) len /= 2;
    dat = mem_malloc(len + 1);
    if (dat == NULL)
        return;
    str2datA(dat, &len, str.c_str());
    qst_data_view(dat, len, NULL);
    mem_free(dat);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSTRClick(TObject *Sender)
{
    leng_t      len;
    AnsiString  str = edtInput->Text;

    /* 显示字符串数值 */
    txtValue->Clear();
    len = str.Length();
    if (len == 0)
        return;
    qst_data_view(str.c_str(), len, NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
    sint_t  sx, sy, hh;

    /* 调整控件的位置 */
    sx = pgeMain->Top;
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
    sx = this->ClientWidth - pgeMain->Width;
    sx = sx / 2 - pgeMain->Left;
    btnHEX->Left   = btnHEX->Left   + sx;
    btnSTR->Left   = btnSTR->Left   + sx;
    edtInput->Left = edtInput->Left + sx;
    pgeMain->Left  = pgeMain->Left  + sx;
}
//---------------------------------------------------------------------------
