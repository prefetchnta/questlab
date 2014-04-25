//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "xSelectFile.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
//---------------------------------------------------------------------------

/*
---------------------------------------
    写入停止运行
---------------------------------------
*/
static void_t
ximp_stop_running (void_t)
{
    FILE*   fp;

    fp = fopen(QST_STOPS_NEXT, "w");
    if (fp != NULL) {
        fputc('x', fp);
        fclose(fp);
    }
    file_deleteA(QST_LOAD_FILEX);
    file_deleteA(QST_SAVE_FILEX);
}

/*
=======================================
    写入打开结果
=======================================
*/
CR_API void_t
ximp_load_write (
  __CR_IN__ const ansi_t*   name
    )
{
    FILE*   fp;

    fp = fopen(QST_LOAD_FILEX, "w");
    if (fp != NULL) {
        file_deleteA(QST_STOPS_NEXT);
        fprintf(fp, "%s", name);
        fclose(fp);
    }
}

/*
=======================================
    写入保存结果
=======================================
*/
CR_API void_t
ximp_save_write (
  __CR_IN__ const ansi_t*   name
    )
{
    FILE*   fp;

    fp = fopen(QST_SAVE_FILEX, "w");
    if (fp != NULL) {
        file_deleteA(QST_STOPS_NEXT);
        fprintf(fp, "%s", name);
        fclose(fp);
    }
}

/* 使用哪个对话框 */
bool_t  g_is_load = FALSE;

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
    uint_t      argc;
    ansi_t**    argv;

    CR_NOUSE(curt_app);
    CR_NOUSE(prev_app);
    CR_NOUSE(cmd_show);

    /* 只允许一个例程 */
    ximp_stop_running();
    if (misc_is_running(EXE_XNAME))
        return (QST_ERROR);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_GUI))
        return (QST_ERROR);

    /* 获取命令行参数, 不包括进程文件名 */
    argv = misc_get_param(cmd_line, &argc);

    /* 参数解析 <load/save> */
    if (argc < 1)
        return (QST_ERROR);
    if (str_cmpA(argv[0], "load") == 0)
        g_is_load = TRUE;
    else
    if (str_cmpA(argv[0], "save") == 0)
        g_is_load = FALSE;
    else
        return (QST_ERROR);

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
