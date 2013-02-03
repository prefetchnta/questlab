//---------------------------------------------------------------------------

#include <vcl.h>
#include "QstMedia.h"
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
CR_API bool_t   qst_stop (void_t);
CR_API bool_t   qst_play (void_t);
CR_API bool_t   qst_pause (void_t);
CR_API bool_t   qst_is_over (bool_t *replay);
CR_API sint_t   qst_get_pos (int64u *time_ms);
CR_API ansi_t*  qst_ms2str (ansi_t *str, int64u time);
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
        : TForm(Owner)
{
    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 使用系统字体 */
    this->Font->Assign(Screen->MenuFont);

    /* 设置进度条范围 */
    proPlay->Min = 0;
    proPlay->Max = QST_XMM_MAX;
    proPlay->Position = 0;

    /* 加载窗口位置配置 */
    misc_desk_init(WIN_ICONF, &x1, &y1, &ww, &hh,
                   this->Width, this->Height);
    this->Left = x1;
    this->Top  = y1;
    this->FormResize(NULL);

    /* 编辑框居中对齐 */
    SetWindowLong(edtNow->Handle, GWL_STYLE,
        GetWindowLong(edtNow->Handle, GWL_STYLE) | ES_CENTER);
    edtNow->Invalidate();

    /* 进度条更新频率 */
    timPlay->Interval = 10;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnStopClick(TObject *Sender)
{
    /* 停止播放 */
    if (qst_stop()) {
        timPlay->Enabled = false;
        btnPlay->Caption = "Play";
        this->timPlayTimer(NULL);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnPlayClick(TObject *Sender)
{
    /* 播放暂停 */
    if (timPlay->Enabled) {
        if (qst_pause()) {
            timPlay->Enabled = false;
            btnPlay->Caption = "Play";
            this->timPlayTimer(NULL);
        }
    }
    else {
        if (qst_play()) {
            timPlay->Enabled = true;
            btnPlay->Caption = "Pause";
            this->timPlayTimer(NULL);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::timPlayTimer(TObject *Sender)
{
    bool_t  replay;
    ansi_t  str[64];
    int64u  time_ms;

    /* 进度条更新 */
    proPlay->Position = qst_get_pos(&time_ms);

    /* 当前时间更新 */
    edtNow->Text = AnsiString(qst_ms2str(str, time_ms));

    /* 判断是否结束 */
    if (qst_is_over(&replay)) {
        qst_stop();
        if (replay) {
            if (qst_play())
                return;
        }
        timPlay->Enabled = false;
        proPlay->Position = 0;
        btnPlay->Caption = "Play";
        edtNow->Text = "00:00:00";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
    sint_t  sx, sy, hh;

    /* 调整控件的位置 */
    sx = this->ClientHeight;
    hh = edtNow->Height;
    if (sx < hh + 4)
        hh = sx - 4;
    sy = (sx - hh) / 2;
    proPlay->Top = sy;
    edtNow->Top  = sy;
    btnPlay->Top = sy;
    btnStop->Top = sy;
    proPlay->Height = hh;
    edtNow->Height  = hh;
    btnPlay->Height = hh;
    btnStop->Height = hh;
    sx = this->ClientWidth + proPlay->Left;
    sx = sx - btnStop->Left - btnStop->Width;
    sx = sx / 2 - proPlay->Left;
    proPlay->Left = proPlay->Left + sx;
    edtNow->Left  = edtNow->Left  + sx;
    btnPlay->Left = btnPlay->Left + sx;
    btnStop->Left = btnStop->Left + sx;
}
//---------------------------------------------------------------------------
