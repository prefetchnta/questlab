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
#include "crhack.h"
#include "sci/Scintilla.h"
#include "sci/SciLexer.h"

/* SCI 控件包装类 */
class TScEdit : public TWinControl
{
private:
    sptr_t          m_parm;     /* SCI 的上下文 */
    SciFnDirect     m_call;     /* SCI 消息调用 */

protected:
    ////////////////////////////////////////////
    //      设定 TWinControl 为 SCI 控件
    ////////////////////////////////////////////
    virtual void __fastcall CreateParams(Controls::TCreateParams &Params)
    {
        TWinControl::CreateParams(Params);
        CreateSubClass(Params, "Scintilla");
    }

    ////////////////////////////////////////////////////
    //      重载窗口消息处理 (加入控制键的处理)
    ////////////////////////////////////////////////////
    virtual void __fastcall WndProc(Messages::TMessage &Message)
    {
        /* 加了这个就可以消除闪烁了 */
        if (Message.Msg == WM_ERASEBKGND)
            return;
        TWinControl::WndProc(Message);
        if (Message.Msg != WM_GETDLGCODE)
            return;
        Message.Result = DLGC_WANTALLKEYS | DLGC_WANTARROWS | DLGC_WANTTAB;
    }

public:
    ////////////////////////
    //      构造函数
    ////////////////////////
    __fastcall TScEdit(Classes::TComponent* AOwner)
            : TWinControl(AOwner)
    {
    }

    ////////////////////////////////
    //      设置直接消息调用
    ////////////////////////////////
    void __fastcall setup_call()
    {
        m_parm = (sptr_t)SendMessage(
                this->Handle, SCI_GETDIRECTPOINTER, 0, 0);
        m_call = (SciFnDirect)SendMessage(
                this->Handle, SCI_GETDIRECTFUNCTION, 0, 0);
    }

    ////////////////////////////
    //      发送消息包装
    ////////////////////////////
    sptr_t send(uint_t iMsg, uptr_t wParam, sptr_t lParam) const
    {
        return (this->m_call(this->m_parm, iMsg, wParam, lParam));
    }
};
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:    // IDE-managed Components
        TPanel *pnlHead;
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

/* 简化调用的宏 */
#define sci_call(msg, wp, lp)   frm->sci(msg, (uptr_t)(wp), (sptr_t)(lp))
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
