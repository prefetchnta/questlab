//---------------------------------------------------------------------------

#define NO_WIN32_LEAN_AND_MEAN
#include <vcl.h>
#include <shlobj.h>
#include "uMain.h"
#include "xSelectDir.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
//---------------------------------------------------------------------------

/*
=======================================
    写入停止运行
=======================================
*/
CR_API void_t
ximp_stop_running (void_t)
{
    FILE*   fp;

    fp = fopen(QST_STOPS_NEXT, "w");
    if (fp != NULL) {
        fputc('x', fp);
        fclose(fp);
    }
}

/*
=======================================
    写入选择结果
=======================================
*/
CR_API void_t
ximp_dir_write (
  __CR_IN__ const ansi_t*   path
    )
{
    FILE*   fp;

    fp = fopen(QST_SELECT_DIR, "w");
    if (fp != NULL) {
        fprintf(fp, "%s", path);
        fclose(fp);
    }
}

/* 指定的起始目录 */
ansi_t* g_root = NULL;

/*
=======================================
    打开目录对话框回调
=======================================
*/
int CALLBACK
BrowseCallbackProc (
  __CR_IN__ HWND    hwnd,
  __CR_IN__ UINT    message,
  __CR_IN__ LPARAM  lparam,
  __CR_IN__ LPARAM  pdata
    )
{
    CR_NOUSE(pdata);
    CR_NOUSE(lparam);
    if (message == BFFM_INITIALIZED)
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)g_root);
    return (0);
}

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
    file_deleteA(QST_STOPS_NEXT);
    if (misc_is_running(EXE_XNAME))
        return (QST_ERROR);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_GUI))
        return (QST_ERROR);

    /* 获取命令行参数, 不包括进程文件名 */
    argv = misc_get_param(cmd_line, &argc);

    sINIu*  ini;
    ansi_t* str;
    bool_t  old = TRUE;

    /* 加载配置文件 */
    str = file_load_as_strA(QST_PATH_CONFIG WIN_ICONF);
    if (str != NULL) {
        ini = ini_parseU(str);
        mem_free(str);
        if (ini != NULL) {
            old = ini_key_intxU("sdir::old_style", TRUE, ini);
            ini_closeU(ini);
        }
    }

    /* 参数解析 [起始目录] */
    g_root = NULL;
    if (argc > 0)
    {
        HANDLE          find;
        WIN32_FIND_DATA wfda;

        /* 验证目录是否存在 */
        find = FindFirstFileA(argv[0], &wfda);
        if (find != INVALID_HANDLE_VALUE) {
            if (wfda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                g_root = argv[0];
            FindClose(find);
        }
    }

    /* 未指定目录则使用上次结果 */
    if (g_root == NULL)
        g_root = file_load_as_strA(QST_SELECT_DIR);
    if (!old)
    {
        BROWSEINFOA     bi;
        LPITEMIDLIST    pidl;
        ansi_t  path[MAX_PATH];

        /* 使用新风格的目录打开窗口 */
        CoInitialize(NULL);
        bi.hwndOwner = NULL;
        bi.pidlRoot  = NULL;
        bi.pszDisplayName = path;
        bi.lpszTitle = "";
        bi.ulFlags = BIF_RETURNONLYFSDIRS;
        if (g_root == NULL)
            bi.lpfn = NULL;
        else
            bi.lpfn = BrowseCallbackProc;
        bi.lParam = 0;
        bi.iImage = 0;
        pidl = SHBrowseForFolderA(&bi);
        if (pidl != NULL &&
            SHGetPathFromIDListA(pidl, path))
            ximp_dir_write(path);
        else
            ximp_stop_running();
        CoUninitialize();
        return (QST_OKAY);
    }

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
