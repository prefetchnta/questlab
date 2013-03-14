//---------------------------------------------------------------------------

#include <vcl.h>
#include "QstText.h"
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

    /* 创建 SCI 编辑控件 */
    m_edit = new TScEdit (this->pnlMain);
    m_edit->Parent = this->pnlMain;
    m_edit->Align = alClient;
    m_edit->setup_call();

    /* 加载窗口位置配置 */
    misc_desk_init(WIN_ICONF, &x1, &y1, &ww, &hh,
                   this->Width, this->Height);
    this->Left   = x1;
    this->Top    = y1;
    this->Width  = ww;
    this->Height = hh;
    this->FormResize(NULL);

    /* 编辑框居中对齐 */
    SetWindowLong(edtCPage->Handle, GWL_STYLE,
        GetWindowLong(edtCPage->Handle, GWL_STYLE) | ES_CENTER);
    edtCPage->Invalidate();
    edtCPage->Text = AnsiString(get_sys_codepage());
    SetWindowLong(edtGoto->Handle, GWL_STYLE,
        GetWindowLong(edtGoto->Handle, GWL_STYLE) | ES_CENTER);
    edtGoto->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnCPageClick(TObject *Sender)
{
    /* 切换文字编码 */
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnGotoClick(TObject *Sender)
{
    /* 跳转到指定行 */
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnRefreshClick(TObject *Sender)
{
    /* 刷新语法高亮 */
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnActionClick(TObject *Sender)
{
    /* 执行文本内容 */
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSaveClick(TObject *Sender)
{
    /* 另存文本文件 */
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnLoadClick(TObject *Sender)
{
    /* 加载文本文件 */
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
    sint_t  sx, sy, hh;

    /* 调整控件的位置 */
    sx = pnlHead->Height;
    hh = edtCPage->Height;
    sy = (sx - hh) / 2;
    edtCPage->Top   = sy;
    btnCPage->Top   = sy;
    edtGoto->Top    = sy;
    btnGoto->Top    = sy;
    lstType->Top    = sy;
    btnRefresh->Top = sy;
    btnAction->Top  = sy;
    btnSave->Top    = sy;
    edtCPage->Height   = hh;
    btnCPage->Height   = hh;
    edtGoto->Height    = hh;
    btnGoto->Height    = hh;
    lstType->Height    = hh;
    btnRefresh->Height = hh;
    btnAction->Height  = hh;
    btnSave->Height    = hh;
    /* ------------------ */
    sx = pnlFoot->Height;
    sy = (sx - hh) / 2;
    btnLoad->Top = sy;
    btnLoad->Height = hh;
}
//---------------------------------------------------------------------------
