//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:    // IDE-managed Components
        TPanel *pnlTool;
        TPanel *pnlMain;
        TPanel *pnlFoot;
private:        // User declarations
        TScEdit *m_edit;
public:         // User declarations
        __fastcall TfrmMain(TComponent* Owner);

        ////////////////////////////////
        //      发送消息到编辑器
        ////////////////////////////////
        sptr_t sci(uint_t iMsg, uptr_t wParam, sptr_t lParam) const
        {
            return (this->m_edit->send(iMsg, wParam, lParam));
        }
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
