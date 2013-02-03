//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
struct  sMenuEvent
{
    const char*     name;
    TNotifyEvent    call;
};
#define QST_MENU_HEADER
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:    // IDE-managed Components
        TMainMenu *mnuMain;
        TToolBar *barDummy;
        TOpenDialog *dlgOpen;
        #include "QstMenu.inl"
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:        // User declarations
public:         // User declarations
        __fastcall TfrmMain(TComponent* Owner);
};
#undef  QST_MENU_HEADER
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
