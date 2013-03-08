/*
 ================================================
 *      ######
 *    #########                            ##
 *   ####    ###                           ##
 *   ##       ##                           ##
 *  ##         ## ##   ##   ####     ### ######
 *  ##         ## ##   ##  ######  ##### ######
 *  ##         ## ##   ## ###  ## ###      ##
 *  ##    #    ## ##   ## ##  ##  #####    ##
 *   ##   ##   ## ##   ## ## ##    #####   ##
 *   ###   ## ##  ##   ## ###   #     ##   ##
 *    ##########  ####### ####### ######   ##
 *      #### ##    ######   ####   ####    ##
 *            ##
 *             ##       CREATE: 2013-03-08
 *              #
 ================================================
        QuestLAB 文本浏览器
 ================================================
 */

#ifndef __QL_QSTTEXT_H__
#define __QL_QSTTEXT_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstText"
    #define WIN_TITLE   "QstText"
    #define WIN_CLASS   "QstTextCLSS"
    #define WIN_ICONF   "QstText.ini"
    #define WIN_XCONF   "QstText.xml"
#endif

/* 工作上下文结构 */
typedef struct
{
        /* 非实时参数 */
        bool_t      quit;   /* 是否退出 */
        void_t*     form;   /* 窗口对象 */
        socket_t    netw;   /* 网络连接 */

} sQstText;

#if defined(VCL_H)

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

#endif  /* VCL_H */

#endif  /* !__QL_QSTTEXT_H__ */
