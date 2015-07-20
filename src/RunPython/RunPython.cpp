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
 *             ##       CREATE: 2015-07-20
 *              #
 ================================================
        QuestLAB Python 脚本启动器
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 应用的名称 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "RunPython"
    #define WIN_TITLE   "RunPython"
    #define WIN_CLASS   "RunPythonCLSS"
    #define WIN_ICONF   "RunPython.ini"
    #define WIN_XCONF   "RunPython.xml"
#endif

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

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_GUI))
        return (QST_ERROR);

    /* 获取命令行参数, 不包括进程文件名 */
    argv = misc_get_param(cmd_line, &argc);

    /* 参数解析 <脚本文件> [相对路径] */
    if (argc < 1)
        return (QST_ERROR);

    leng_t  size;
    ansi_t* root;
    ansi_t* exec;

    /* 合成 PYTHONHOME 目录 */
    exec = file_load_as_strA(QST_ROOT_START);
    if (exec == NULL) {
        if (argc < 2)
            return (QST_ERROR);
        root = str_fmtA("%s\\" QST_ROOT_START, argv[1]);
        if (root == NULL)
            return (QST_ERROR);
        exec = file_load_as_strA(root);
        mem_free(root);
        if (exec == NULL)
            return (QST_ERROR);
    }
    root = str_fmtA("%s\\python", exec);
    mem_free(exec);
    if (root == NULL)
        return (QST_ERROR);

    DWORD   cf = 0;

    /* 根据扩展名选择用哪个启动 */
    if (filext_checkA(argv[0], ".py")) {
        exec = str_fmtA("%s.exe \"%s\"", root, argv[0]);
        cf = CREATE_NEW_CONSOLE;
    }
    else
    if (filext_checkA(argv[0], ".pyw")) {
        exec = str_fmtA("%sw.exe \"%s\"", root, argv[0]);
        cf = CREATE_NO_WINDOW;
    }
    else {
        exec = NULL;
    }
    if (exec == NULL) {
        mem_free(root);
        return (QST_ERROR);
    }

    STARTUPINFOA        si;
    PROCESS_INFORMATION pi;

    /* 启动脚本 */
    mem_zero(&si, sizeof(si));
    si.cb = sizeof(STARTUPINFOA);
    SetEnvironmentVariableA("PYTHONHOME", root);
    size  = str_lenA(root);
    size -= str_lenA("\\python");
    root[size] = 0x00;
    if (CreateProcessA(NULL, exec, NULL, NULL, FALSE,
                       cf, NULL, root, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    mem_free(exec);
    mem_free(root);
    return (QST_OKAY);
}
