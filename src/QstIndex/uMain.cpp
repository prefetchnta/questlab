//---------------------------------------------------------------------------

#include <vcl.h>
#include "QstIndex.h"
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
CR_API void_t   qst_refresh_text (void_t);
CR_API void_t   qst_send_index (sint_t idx);
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
        : TForm(Owner)
{
    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 使用系统字体 */
    this->Font->Assign(Screen->MenuFont);

    /* 设置拖条初始值 */
    tbrCount->Min = 0;
    tbrCount->Max = 0;
    tbrCount->Position = 0;
    qst_refresh_text();

    /* 加载窗口位置配置 */
    misc_desk_init(WIN_ICONF, &x1, &y1, &ww, &hh,
                   this->Width, this->Height);
    this->Left = x1;
    this->Top  = y1;
    this->FormResize(NULL);

    /* 编辑框居中对齐 */
    SetWindowLong(edtCount->Handle, GWL_STYLE,
        GetWindowLong(edtCount->Handle, GWL_STYLE) | ES_CENTER);
    edtCount->Invalidate();
    SetWindowLong(edtDelay->Handle, GWL_STYLE,
        GetWindowLong(edtDelay->Handle, GWL_STYLE) | ES_CENTER);
    edtDelay->Invalidate();

    /* 设置编辑框的值 */
    timPlay->Interval = 100;
    edtDelay->Text = IntToStr(timPlay->Interval);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::tbrCountChange(TObject *Sender)
{
    /* 改变拖条时发送命令 */
    qst_send_index(tbrCount->Position);
    qst_refresh_text();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnAddClick(TObject *Sender)
{
    /* 序号值加一 */
    if (tbrCount->Position != tbrCount->Max)
        tbrCount->Position = tbrCount->Position + 1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSubClick(TObject *Sender)
{
    /* 序号值减一 */
    if (tbrCount->Position != tbrCount->Min)
        tbrCount->Position = tbrCount->Position - 1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnPlayClick(TObject *Sender)
{
    sint_t  delay;

    /* 播放或停止 */
    delay = StrToIntDef(edtDelay->Text, 100);
    if (delay < 100) delay = 100;
    edtDelay->Text = IntToStr(delay);
    timPlay->Interval = delay;
    if (timPlay->Enabled)
    {
        /* 停止定时器 */
        timPlay->Enabled = false;
        btnPlay->Caption = "Play";

        /* 恢复界面 */
        btnAdd->Enabled = true;
        btnSub->Enabled = true;
        btnGoto->Enabled = true;
        btnLast->Enabled = true;
        btnFirst->Enabled = true;
        edtCount->Enabled = true;
        edtDelay->Enabled = true;
        tbrCount->Enabled = true;
    }
    else
    {
        /* 启动定时器 */
        timPlay->Enabled = true;
        btnPlay->Caption = "Stop";

        /* 锁定界面 */
        btnAdd->Enabled = false;
        btnSub->Enabled = false;
        btnGoto->Enabled = false;
        btnLast->Enabled = false;
        btnFirst->Enabled = false;
        edtCount->Enabled = false;
        edtDelay->Enabled = false;
        tbrCount->Enabled = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnGotoClick(TObject *Sender)
{
    sint_t  idx;

    /* 转到指定值 */
    idx = str2intxA(edtCount->Text.c_str());
    if (idx < tbrCount->Min)
        idx = tbrCount->Min;
    else
    if (idx > tbrCount->Max)
        idx = tbrCount->Max;
    tbrCount->Position = idx;
    qst_refresh_text();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::timPlayTimer(TObject *Sender)
{
    /* 自动播放序号 */
    if (tbrCount->Position != tbrCount->Max)
        tbrCount->Position = tbrCount->Position + 1;
    else
        tbrCount->Position = tbrCount->Min;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnFirstClick(TObject *Sender)
{
    /* 跳到最开头 */
    tbrCount->Position = tbrCount->Min;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnLastClick(TObject *Sender)
{
    /* 跳到最结尾 */
    tbrCount->Position = tbrCount->Max;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
    sint_t  sx, sy, hh;

    /* 调整控件的位置 */
    sx = tbrCount->Top;
    hh = edtCount->Height;
    if (sx < hh + 4)
        hh = sx - 4;
    sy = (sx - hh) / 2;
    btnFirst->Top = sy;
    btnSub->Top   = sy;
    btnAdd->Top   = sy;
    btnLast->Top  = sy;
    edtCount->Top = sy;
    btnGoto->Top  = sy;
    edtDelay->Top = sy;
    btnPlay->Top  = sy;
    btnFirst->Height = hh;
    btnSub->Height   = hh;
    btnAdd->Height   = hh;
    btnLast->Height  = hh;
    edtCount->Height = hh;
    btnGoto->Height  = hh;
    edtDelay->Height = hh;
    btnPlay->Height  = hh;
    sx = this->ClientWidth + btnFirst->Left;
    sx = sx - btnPlay->Left - btnPlay->Width;
    sx = sx / 2 - btnFirst->Left;
    btnFirst->Left = btnFirst->Left + sx;
    btnSub->Left   = btnSub->Left   + sx;
    btnAdd->Left   = btnAdd->Left   + sx;
    btnLast->Left  = btnLast->Left  + sx;
    edtCount->Left = edtCount->Left + sx;
    btnGoto->Left  = btnGoto->Left  + sx;
    edtDelay->Left = edtDelay->Left + sx;
    btnPlay->Left  = btnPlay->Left  + sx;
}
//---------------------------------------------------------------------------
