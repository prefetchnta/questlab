//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:    // IDE-managed Components
        TDirectoryListBox *lstDir;
        TDriveComboBox *lstDrv;
        TButton *btnOK;
        TButton *btnCancel;
        void __fastcall btnOKClick(TObject *Sender);
        void __fastcall btnCancelClick(TObject *Sender);
private:        // User declarations
        AnsiString  m_curt; /* 保存当前目录 */
public:         // User declarations
        __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
