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

    DWORD   leng;
    ansi_t* conf_name;
    ansi_t  exec[MAX_PATHA];

    /* 输出当前的绝对目录 */
    leng = GetCurrentDirectoryA(sizeof(exec), exec);
    if (leng > 1 && leng < sizeof(exec)) {
        if (exec[leng - 1] == '\\')
            exec[leng - 1] = 0x00;
        file_saveA(QST_ROOT_START, exec, str_lenA(exec));
    }

    /* 参数解析 [配置名称] */
    if (argc == 0 || str_lenA(argv[0]) > 64)
        conf_name = NULL;
    else
        conf_name = argv[0];

    /* 复制指定的配置文件 */
    misc_call_exe("cmd.exe /c copy " QST_PATH_CONFIG
                  "base\\*.* " QST_PATH_CONFIG, TRUE, TRUE);
    if (conf_name != NULL) {
        sprintf(exec, QST_PATH_CONFIG "%s", conf_name);
        if (misc_dir_exist(exec)) {
            sprintf(exec, "cmd.exe /c copy " QST_PATH_CONFIG
                    "%s\\*.* " QST_PATH_CONFIG, conf_name);
            misc_call_exe(exec, TRUE, TRUE);
        }
    }

    uint_t  scn_w = GetSystemMetrics(SM_CXSCREEN);
    uint_t  scn_h = GetSystemMetrics(SM_CYSCREEN);

    /* 复制指定的窗口布局 */
    sprintf(exec, "cmd.exe /c copy " QST_PATH_WINDOW
            "base\\%ux%u\\*.* " QST_PATH_WINDOW, scn_w, scn_h);
    misc_call_exe(exec, TRUE, TRUE);
    if (conf_name != NULL) {
        sprintf(exec, QST_PATH_WINDOW "%s", conf_name);
        if (misc_dir_exist(exec)) {
            sprintf(exec, "cmd.exe /c copy " QST_PATH_WINDOW
                    "%s\\%ux%u\\*.* " QST_PATH_WINDOW, conf_name,
                                scn_w, scn_h);
            misc_call_exe(exec, TRUE, TRUE);
        }
    }

    /* 启动广播服务器程序 */
    if (!misc_call_exe("QstServ.exe", FALSE, FALSE))
        return (QST_ERROR);

    /* 复制指定的 SciTE 配置文件 */
    misc_call_exe("cmd.exe /c copy " QST_PATH_EXT3RD "wscite\\"
                  "base\\*.* " QST_PATH_EXT3RD "wscite\\", TRUE, TRUE);
    if (conf_name != NULL) {
        sprintf(exec, QST_PATH_EXT3RD "wscite\\%s", conf_name);
        if (misc_dir_exist(exec)) {
            sprintf(exec, "cmd.exe /c copy " QST_PATH_EXT3RD "wscite\\"
                    "%s\\*.* " QST_PATH_EXT3RD "wscite\\", conf_name);
            misc_call_exe(exec, TRUE, TRUE);
        }
    }
    misc_call_exe("cmd.exe /c copy /b " QST_PATH_EXT3RD "wscite\\"
            "SciTEGlobal.properties+" QST_PATH_WINDOW "wscite.ini "
                QST_PATH_EXT3RD "wscite\\SciTEGlobal.properties",
                                TRUE, TRUE);
    FILE*   fp;

    /* 添加 SciTE 的默认编码 */
    fp = fopen(QST_PATH_EXT3RD "wscite\\SciTEGlobal.properties", "a+");
    if (fp != NULL) {
        fprintf(fp, "code.page=%u\n", get_sys_codepage());
        fclose(fp);
    }

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
