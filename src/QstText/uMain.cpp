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
CR_API bool_t   qst_change_cpage (uint_t cpage);
CR_API void_t   qst_file_action (uint_t item_idx);
CR_API bool_t   qst_load_file (const ansi_t *name);
CR_API bool_t   qst_save_file (const ansi_t *name, uint_t page);
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
    edtCPage->Text = IntToStr(get_sys_codepage());
    SetWindowLong(edtSPage->Handle, GWL_STYLE,
        GetWindowLong(edtSPage->Handle, GWL_STYLE) | ES_CENTER);
    edtSPage->Invalidate();
    edtSPage->Text = IntToStr(CR_UTF8);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnCPageClick(TObject *Sender)
{
    uint_t      cpage, native;
    AnsiString  xrsave = edtCPage->Text;

    /* 切换文字编码 */
    native = get_sys_codepage();
    cpage = StrToIntDef(edtCPage->Text, native);
    if ((sint_t)cpage <= 0) cpage = native;
    if (qst_change_cpage(cpage))
        edtCPage->Text = IntToStr(cpage);
    else
        edtCPage->Text = xrsave;
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
    if (lstType->ItemIndex < 0)
        return;
    qst_file_action(lstType->ItemIndex);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSaveClick(TObject *Sender)
{
    uint_t  cpage;

    /* 另存文本文件 */
    if (!dlgSave->Execute())
        return;
    cpage = StrToIntDef(edtSPage->Text, CR_UTF8);
    if ((sint_t)cpage <= 0) cpage = CR_UTF8;
    edtSPage->Text = IntToStr(cpage);
    if (qst_save_file(dlgSave->FileName.c_str(), cpage)) {
        MessageBox(this->Handle, "Save file okay!   ",
                "SUCCESS", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBox(this->Handle, "Save file fail!   ",
                "FAILURE", MB_OK | MB_ICONSTOP);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnLoadClick(TObject *Sender)
{
    /* 加载文本文件 */
    if (!dlgOpen->Execute())
        return;
    if (qst_load_file(dlgOpen->FileName.c_str())) {
        MessageBox(this->Handle, "Load file okay!   ",
                "SUCCESS", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBox(this->Handle, "Load file fail!   ",
                "FAILURE", MB_OK | MB_ICONSTOP);
    }
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
    lstType->Top    = sy;
    btnRefresh->Top = sy;
    btnAction->Top  = sy;
    btnLoad->Top    = sy;
    edtSPage->Top   = sy;
    btnSave->Top    = sy;
    edtCPage->Height   = hh;
    btnCPage->Height   = hh;
    lstType->Height    = hh;
    btnRefresh->Height = hh;
    btnAction->Height  = hh;
    btnLoad->Height    = hh;
    edtSPage->Height   = hh;
    btnSave->Height    = hh;
}
//---------------------------------------------------------------------------
