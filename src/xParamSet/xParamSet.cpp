//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "xParamSet.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
//---------------------------------------------------------------------------

/*
=======================================
    WinMain 程序入口
=======================================
*/
int WINAPI
WinMain (
  __CR_IN__ HINSTANCE   curt_app,
  __CR_IN__ HINSTANCE   prev_app,
  __CR_IN__ LPSTR       cmd_line,
  __CR_IN__ int         cmd_show
    )
{
    CR_NOUSE(curt_app);
    CR_NOUSE(prev_app);
    CR_NOUSE(cmd_line);
    CR_NOUSE(cmd_show);

    /* 只允许一个例程 */
    if (misc_is_running(EXE_XNAME))
        return (QST_ERROR);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_GUI))
        return (QST_ERROR);

    /* 标准 VCL 过程开始 */
    try
    {
        Application->Initialize();
        Application->Title = WIN_TITLE;
        Application->CreateForm(__classid(TfrmMain), &frmMain);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }
    return (QST_OKAY);
}
//---------------------------------------------------------------------------
