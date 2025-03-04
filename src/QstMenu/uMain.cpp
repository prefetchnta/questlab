//---------------------------------------------------------------------------

#include <vcl.h>
#include "QstMenu.h"
#pragma hdrstop

#include "uMain.h"
#include "uAbout.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
CR_API void_t   qst_quit_all_3rd (void_t);
CR_API void_t   qst_mnu_win_save2 (void_t);
CR_API void_t   qst_mnu_win_load2 (void_t);
CR_API void_t   qst_send_cmdz (const ansi_t *cmd);
CR_API bool_t   qst_load_menu (TMenuItem *menu, sCURBEAD *event);
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

    /* 外部程序初始状态 */
    m_use_scite = false;
    m_code_page = get_sys_codepage();

    /* 加载主菜单 */
    this->SetupMenu();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    /* 关闭时全部退出 */
    qst_quit_all_3rd();
    qst_send_cmdz("app:exit");
    misc_call_exe("RunPython script/python/kill_x64.pyw", TRUE, TRUE);
}
//---------------------------------------------------------------------------
#define QST_MENU_SOURCE
#include "QstMenu.inl"
#undef  QST_MENU_SOURCE
//---------------------------------------------------------------------------
