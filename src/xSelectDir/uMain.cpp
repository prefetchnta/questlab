//---------------------------------------------------------------------------

#include <vcl.h>
#include "xSelectDir.h"
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
extern ansi_t*  g_root;
CR_API void_t   ximp_dir_write (const ansi_t *path);
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
        : TForm(Owner)
{
    /* 使用系统字体 */
    this->Font->Assign(Screen->MenuFont);

    /* 保存当前位置 */
    m_curt = GetCurrentDir();

    /* 设置初始目录 */
    if (g_root != NULL)
        lstDir->Directory = AnsiString(g_root);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnOKClick(TObject *Sender)
{
    /* 确定按钮 */
    if (SetCurrentDir(m_curt))
        ximp_dir_write(lstDir->Directory.c_str());
    this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnCancelClick(TObject *Sender)
{
    /* 取消按钮 */
    this->Close();
}
//---------------------------------------------------------------------------
