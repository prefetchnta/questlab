//---------------------------------------------------------------------------

#include <vcl.h>
#include "xSelectFile.h"
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
extern bool_t   g_is_load;
CR_API void_t   ximp_load_write (const ansi_t *name);
CR_API void_t   ximp_save_write (const ansi_t *name);
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
        : TForm(Owner)
{
    if (g_is_load) {
        if (dlgOpen->Execute())
            ximp_load_write(dlgOpen->FileName.c_str());
    }
    else {
        if (dlgSave->Execute())
            ximp_save_write(dlgSave->FileName.c_str());
    }
    PostMessage(this->Handle, WM_CLOSE, 0, 0);
}
//---------------------------------------------------------------------------
