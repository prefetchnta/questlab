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
 *             ##       CREATE: 2012-09-06
 *              #
 ================================================
        QuestLAB 主启动程序
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 应用的名称 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QuestLAB"
    #define WIN_TITLE   "QuestLAB"
    #define WIN_CLASS   "QuestLABCLSS"
    #define WIN_ICONF   "QuestLAB.ini"
    #define WIN_XCONF   "QuestLAB.xml"
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

    /* 只允许一个例程 */
    if (misc_is_running(EXE_XNAME))
        return (QST_ERROR);

    /* 限定附加参数长度 */
    if (str_lenA(cmd_line) > 128)
        return (QST_ERROR);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_GUI))
        return (QST_ERROR);

    /* 获取命令行参数, 不包括进程文件名 */
    argv = misc_get_param(cmd_line, &argc);

    ansi_t  exec[256];
    uint_t  scn_w, scn_h;
    bool_t  user_cfg = FALSE;

    /* 参数解析 [执行列表] [窗口位置配置名] */
    if (argc > 1)
    {
        HANDLE              find;
        WIN32_FIND_DATAA    wfda;

        /* 验证目录是否存在 */
        sprintf(exec, QST_PATH_WINDOW "%s", argv[1]);
        find = FindFirstFileA(exec, &wfda);
        if (find != INVALID_HANDLE_VALUE) {
            if (wfda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                sprintf(exec, "cmd.exe /c copy " QST_PATH_WINDOW
                        "%s\\*.ini " QST_PATH_WINDOW, argv[1]);
                user_cfg = TRUE;
            }
            FindClose(find);
        }
    }

    /* 根据分辨率设置窗口布局 */
    if (!user_cfg) {
        scn_w = GetSystemMetrics(SM_CXSCREEN);
        scn_h = GetSystemMetrics(SM_CYSCREEN);
        sprintf(exec, "cmd.exe /c copy " QST_PATH_WINDOW
            "%ux%u\\*.ini " QST_PATH_WINDOW, scn_w, scn_h);
    }
    misc_call_exe(exec, TRUE, TRUE);

    /* 启动广播服务器程序 */
    if (!misc_call_exe("QstServ.exe", FALSE, FALSE))
        return (QST_ERROR);
    thread_sleep(100);

    sINIu*  ini;
    ansi_t* str = NULL;

    /* 根据配置文件执行各部件 */
    if (argc > 0) {
        sprintf(exec, QST_PATH_CONFIG "QuestLAB_%s.ini", argv[0]);
        str = file_load_as_strA(exec);
    }
    if (str == NULL)
        str = file_load_as_strA(QST_PATH_CONFIG WIN_ICONF);
    if (str == NULL)
        return (QST_ERROR);
    ini = ini_parseU(str);
    mem_free(str);
    if (ini == NULL)
        return (QST_ERROR);
    for (leng_t idx = 0; idx < ini->count; idx++) {
        str = skip_spaceA(ini->lines[idx]);
        if (str_lenA(str) == 0 ||
            str_strIA(str, ".exe") == NULL)
            continue;
        misc_call_exe(str, FALSE, FALSE);
        thread_sleep(100);
    }
    ini_closeU(ini);
    return (QST_OKAY);
}
