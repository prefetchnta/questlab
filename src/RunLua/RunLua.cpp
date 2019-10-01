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
 *             ##       CREATE: 2017-11-09
 *              #
 ================================================
        QuestLAB Lua 脚本启动器
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 应用的名称 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "RunLua"
    #define WIN_TITLE   "RunLua"
    #define WIN_CLASS   "RunLuaCLSS"
    #define WIN_ICONF   "RunLua.ini"
    #define WIN_XCONF   "RunLua.xml"
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

    /* 参数解析 <脚本文件> */
    if (argc < 1)
        return (QST_ERROR);

    leng_t  size;
    ansi_t* root;
    ansi_t* temp;
    ansi_t* exec;
    ansi_t  path[MAX_PATHA];

    /* 合成 LUA_PATH & LUA_CPATH 目录 */
    exec = file_load_as_strA(QST_ROOT_START);
    if (exec == NULL) {
        size = GetCurrentDirectoryA(sizeof(path), path);
        if (size == 0 || size > sizeof(path))
            return (QST_ERROR);
        if (path[size - 1] == '\\')
            path[size - 1] = 0x00;
        exec = str_dupA(path);
        if (exec == NULL)
            return (QST_ERROR);
    }
    temp = str_dupA(exec);
    if (temp == NULL) {
        mem_free(exec);
        return (QST_ERROR);
    }
    root = str_fmtA("%s\\lualib", exec);
    mem_free(exec);
    if (root == NULL) {
        mem_free(temp);
        return (QST_ERROR);
    }

    DWORD   cf = 0;

    /* 根据扩展名选择用哪个启动 */
    if (filext_checkA(argv[0], ".lua")) {
        exec = str_fmtA("%s\\lua53.exe \"%s\"", temp, argv[0]);
        cf = CREATE_NEW_CONSOLE;
    }
    else
    if (filext_checkA(argv[0], ".wlua")) {
        exec = str_fmtA("%s\\wlua53.exe \"%s\"", temp, argv[0]);
        cf = CREATE_NO_WINDOW;
    }
    else {
        exec = NULL;
    }
    mem_free(temp);
    if (exec == NULL) {
        mem_free(root);
        return (QST_ERROR);
    }

    STARTUPINFOA        si;
    PROCESS_INFORMATION pi;

    /* 启动脚本 */
    mem_zero(&si, sizeof(si));
    si.cb = sizeof(STARTUPINFOA);
    SetEnvironmentVariableA("LUA_PATH", root);
    SetEnvironmentVariableA("LUA_CPATH", root);
    size  = str_lenA(root);
    size -= str_lenA("\\lualib");
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
