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
}
//---------------------------------------------------------------------------
