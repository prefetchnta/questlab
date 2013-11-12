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

/* 默认配置名称 */
#define DEF_CONF_NAME   "base"

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
    ansi_t* conf_name;

    /* 参数解析 [配置名称] */
    if (argc == 0 || str_lenA(argv[0]) > 64)
        conf_name = DEF_CONF_NAME;
    else
        conf_name = argv[0];

    HANDLE              find;
    WIN32_FIND_DATAA    wfda;

    /* 复制指定的配置文件 */
    sprintf(exec, QST_PATH_CONFIG "%s", conf_name);
    find = FindFirstFileA(exec, &wfda);
    if ((find == INVALID_HANDLE_VALUE) ||
       !(wfda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        conf_name = DEF_CONF_NAME;
    sprintf(exec, "cmd.exe /c copy " QST_PATH_CONFIG
            "%s\\*.* " QST_PATH_CONFIG, conf_name);
    if (find != INVALID_HANDLE_VALUE)
        FindClose(find);
    misc_call_exe(exec, TRUE, TRUE);

    /* 复制指定的窗口布局 */
    sprintf(exec, QST_PATH_WINDOW "%s", conf_name);
    find = FindFirstFileA(exec, &wfda);
    if ((find == INVALID_HANDLE_VALUE) ||
       !(wfda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        conf_name = DEF_CONF_NAME;
    sprintf(exec, "cmd.exe /c copy " QST_PATH_WINDOW
            "%s\\%ux%u\\*.* " QST_PATH_WINDOW, conf_name,
                    GetSystemMetrics(SM_CXSCREEN),
                    GetSystemMetrics(SM_CYSCREEN));
    if (find != INVALID_HANDLE_VALUE)
        FindClose(find);
    misc_call_exe(exec, TRUE, TRUE);

    /* 启动广播服务器程序 */
    if (!misc_call_exe("QstServ.exe", FALSE, FALSE))
        return (QST_ERROR);

    /* 复制指定的 SciTE 配置文件 */
    sprintf(exec, QST_PATH_EXT3RD "wscite\\%s", conf_name);
    find = FindFirstFileA(exec, &wfda);
    if ((find == INVALID_HANDLE_VALUE) ||
       !(wfda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        conf_name = DEF_CONF_NAME;
    sprintf(exec, "cmd.exe /c copy " QST_PATH_EXT3RD "wscite\\"
            "%s\\*.* " QST_PATH_EXT3RD "wscite\\", conf_name);
    if (find != INVALID_HANDLE_VALUE)
        FindClose(find);
    misc_call_exe(exec, FALSE, TRUE);

    sINIu*  ini;
    ansi_t* str;

    /* 根据配置文件执行各部件 */
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
    }
    ini_closeU(ini);
    return (QST_OKAY);
}
