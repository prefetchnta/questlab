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
 *             ##       CREATE: 2012-07-07
 *              #
 ================================================
        QuestLAB 命令行工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 应用的名称 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstCmdz"
    #define WIN_TITLE   "QstCmdz"
    #define WIN_CLASS   "QstCmdzCLSS"
    #define WIN_ICONF   "QstCmdz.ini"
    #define WIN_XCONF   "QstCmdz.xml"
#endif

/* 输出文件句柄 */
static FILE*    s_fp = NULL;

/* 控制台窗口句柄 */
static HWND     s_hwnd = NULL;

/* 控制台输出句柄 */
static HANDLE   s_hcui = NULL;

/* 缺省的窗口大小 */
#define QCMD_DEF_WIDTH  668
#define QCMD_DEF_HEIGHT 434

/* 显示颜色配置 */
static int16u   s_color_text;   /* 普通文本颜色 */
static int16u   s_color_errs;   /* 出错文本颜色 */
static int16u   s_color_help;   /* 帮助文本颜色 */

/* 内部函数声明 */
static bool_t
exec_one_line (ansi_t *cmd, socket_t netw, bool_t show);

/*
---------------------------------------
    显示窗口位置
---------------------------------------
*/
static bool_t
qst_cmd_win_show (void_t)
{
    /* 可能隐藏运行 */
    if (s_hwnd == NULL)
        return (FALSE);
    misc_bring2top(s_hwnd, NULL);
    return (TRUE);
}

/*
---------------------------------------
    保存窗口位置
---------------------------------------
*/
static bool_t
qst_cmd_win_save (void_t)
{
    RECT    win;

    /* 可能隐藏运行 */
    if (s_hwnd == NULL)
        return (FALSE);

    /* 获取窗口位置 */
    if (!GetWindowRect(s_hwnd, &win))
        return (FALSE);

    sint_t  x1 = win.left;
    sint_t  y1 = win.top;
    uint_t  ww = win.right - win.left;
    uint_t  hh = win.bottom - win.top;

    /* 保存窗口位置 */
    return (misc_desk_save(WIN_ICONF, x1, y1, ww, hh));
}

/*
---------------------------------------
    加载窗口位置
---------------------------------------
*/
static bool_t
qst_cmd_win_load (void_t)
{
    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 可能隐藏运行 */
    if (s_hwnd == NULL)
        return (FALSE);

    /* 保存窗口位置 */
    if (!misc_desk_load(WIN_ICONF, &x1, &y1, &ww, &hh))
        return (FALSE);

    /* 设置窗口位置 */
    misc_bring2top(s_hwnd, NULL);
    return (misc_cui_setwin(s_hwnd, s_hcui, x1, y1, ww, hh));
}

/*
---------------------------------------
    加载批处理
---------------------------------------
*/
static bool_t
load_bat_file (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ socket_t        netw
    )
{
    sINIu*  line;
    ansi_t* text;

    /* 加载文件 */
    text = file_load_as_strA(name);
    if (text == NULL) {
        cui_set_color(s_color_errs);
        printf("invalid batch file: %s\n", name);
        cui_set_color(s_color_text);
        return (TRUE);
    }

    /* 逐行执行命令 */
    line = ini_parseU(text);
    mem_free(text);
    if (line == NULL) {
        cui_set_color(s_color_errs);
        printf("invalid batch file: %s\n", name);
        cui_set_color(s_color_text);
        return (TRUE);
    }
    for (leng_t idx = 0; idx < line->count; idx++) {
        if (!exec_one_line(line->lines[idx], netw, TRUE)) {
            ini_closeU(line);
            return (FALSE);
        }
    }
    ini_closeU(line);
    return (TRUE);
}

/*
---------------------------------------
    调用外部程序
---------------------------------------
*/
static void_t
call_exe_file (
  __CR_IN__ const ansi_t*   name
    )
{
    /* 如果这里调用的是控制台程序
       做完事情后必须自己退出, 不然会死在那里 */
    if (!misc_call_exe(name, TRUE, TRUE)) {
        cui_set_color(s_color_errs);
        printf("call process failure\n");
    }
    else {
        cui_set_color(s_color_help);
        printf("the process is finished\n");
    }
    cui_set_color(s_color_text);
}

/*
---------------------------------------
    执行一条命令
---------------------------------------
*/
static bool_t
exec_one_line (
  __CR_IO__ ansi_t*     cmd,
  __CR_IN__ socket_t    netw,
  __CR_IN__ bool_t      show
    )
{
    /* 去除头尾空白 */
    cmd = str_trimA(cmd);

    /* 空命令直接返回 */
    if (str_lenA(cmd) == 0)
        return (TRUE);

    /* 是注释直接返回 */
    if ((cmd[0] == ';') ||
        (cmd[0] == '/' && cmd[1] == '/') ||
        (cmd[0] == '-' && cmd[1] == '-'))
        return (TRUE);

    /* 输出命令行 */
    if (show) printf("> %s\n", cmd);

    /* 命令解析 */
    if (cmd[0] == '$')
    {
        /* 本地命令 - 退出程序 */
        if (chr_cmpA(&cmd[1], "exit", 5) == 0)
            return (FALSE);

        /* 本地命令 - 是否继续执行 */
        if (chr_cmpA(&cmd[1], "continue?", 10) == 0)
            return (!file_existA(QST_STOPS_NEXT));

        /* 本地命令 - 执行批处理 */
        if (chr_cmpA(&cmd[1], "exec ", 5) == 0)
            return (load_bat_file(skip_spaceA(&cmd[6]), netw));

        /* 本地命令 - 调用程序 */
        if (chr_cmpA(&cmd[1], "call ", 5) == 0) {
            call_exe_file(skip_spaceA(&cmd[6]));
            return (TRUE);
        }

        /* 本地命令 - 删除文件 */
        if (chr_cmpA(&cmd[1], "del ", 4) == 0) {
            file_deleteA(skip_spaceA(&cmd[5]));
            return (TRUE);
        }

        /* 本地命令 - 创建文件 */
        if (chr_cmpA(&cmd[1], "fopen ", 6) == 0) {
            s_fp = fopen(skip_spaceA(&cmd[7]), "w");
            return ((s_fp == NULL) ? FALSE : TRUE);
        }

        /* 本地命令 - 关闭文件 */
        if (chr_cmpA(&cmd[1], "fclose", 7) == 0) {
            if (s_fp != NULL) {
                fclose(s_fp);
                s_fp = NULL;
            }
            return (TRUE);
        }

        /* 本地命令 - 显示帮助 */
        if (chr_cmpA(&cmd[1], "help", 5) == 0) {
            cui_set_color(s_color_help);
            printf("\t$call <...> --- make an external call\n");
            printf("\t$continue? ---- continue or quit running\n");
            printf("\t$del <file> --- delete a disk file\n");
            printf("\t$exec <file> -- execute a batch file\n");
            printf("\t$exit --------- exit current program\n");
            printf("\t$fclose ------- close current file\n");
            printf("\t$fopen <file> - create an output file\n");
            printf("\t$help --------- print this message\n");
            cui_set_color(s_color_text);
            return (TRUE);
        }

        /* 尚未支持的本地命令 */
        cui_set_color(s_color_errs);
        printf("command not supported yet\n");
        cui_set_color(s_color_text);
    }
    else
    {
        /* 输入重定向到文件 */
        if (s_fp != NULL) {
            fprintf(s_fp, "%s\n", cmd);
            return (TRUE);
        }

        /* 可以发送直接的命令 */
        if (cmd_type_okay(cmd))
            return (netw_cmd_send(netw, cmd));

        /* 远程命令 - 转换后发送 */
        if (!cmd_shl_send(netw, cmd))
            return (FALSE);

        /* 处理自身发出的指令 */
        if (str_cmpA(cmd, "app:exit") == 0 ||
            str_cmpA(cmd, "qcmd:app:exit") == 0)
            return (FALSE);
        if (str_cmpA(cmd, "win:load") == 0)
            qst_cmd_win_load();
        else
        if (str_cmpA(cmd, "win:save") == 0)
            qst_cmd_win_save();
        else
        if (str_cmpA(cmd, "win:show") == 0 ||
            str_cmpA(cmd, "qcmd:win:show") == 0)
            qst_cmd_win_show();
    }
    return (TRUE);
}

/* 防止警告的值 */
static bool_t   s_quit = FALSE;

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_cmd_main (
  __CR_IN__ void_t* param
    )
{
    socket_t    netw;

    netw = (socket_t)param;
    while (!s_quit)
    {
        ansi_t* shell;
        ansi_t* string;

        string = netw_cmd_recv(netw);
        if (string == NULL) {
            thread_sleep(1);
            continue;
        }

        /* 处理一些公共指令 */
        shell = cmd_shl_get(string);
        mem_free(string);
        if (shell == NULL)
            continue;
        if (str_cmpA(shell, "app:exit") == 0 ||
            str_cmpA(shell, "qcmd:app:exit") == 0)
            exit(0);
        if (str_cmpA(shell, "win:load") == 0)
            qst_cmd_win_load();
        else
        if (str_cmpA(shell, "win:save") == 0)
            qst_cmd_win_save();
        else
        if (str_cmpA(shell, "win:show") == 0 ||
            str_cmpA(shell, "qcmd:win:show") == 0)
            qst_cmd_win_show();
        mem_free(shell);
    }
    return (QST_OKAY);
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);
    SetConsoleTitleA(WIN_TITLE);

    bool_t  exist;
    ansi_t* file = "";

    /* 参数解析 [批处理脚本] */
    if (argc > 1)
        file = argv[1];

    /* 只允许一个命令行例程 */
    exist = file_existA(file);
    if (!exist) {
        if (misc_is_running(EXE_XNAME))
            return (QST_ERROR);
    }

    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 初始化窗口 */
    s_hcui = GetStdHandle(STD_OUTPUT_HANDLE);
    if (s_hcui == INVALID_HANDLE_VALUE)
        return (QST_ERROR);

    /* 在 Win7 下以 CREATE_NO_WINDOW 标志运行
       进程会造成这个函数失败，但 XP 下则不会 */
    s_hwnd = GetConsoleWindow();
    if (s_hwnd != NULL) {
        misc_desk_init(WIN_ICONF, &x1, &y1, &ww, &hh,
                       QCMD_DEF_WIDTH, QCMD_DEF_HEIGHT);
        misc_cui_setwin(s_hwnd, s_hcui, x1, y1, ww, hh);
    }

    socket_t    netw;

    /* 初始化网络 */
    if (!socket_init())
        return (QST_ERROR);
    netw = netw_cli_open(EXE_XNAME);
    if (netw == NULL)
        return (QST_ERROR);
    s_fp = NULL;

    thrd_t  thrd;

    /* 建立工作线程 */
    /* 每一个程序都要有一个读数据包的过程
       否则, 发送缓冲溢出后会造成服务器死锁 */
    thrd = thread_new(0, qst_cmd_main, netw, FALSE);
    if (thrd == NULL)
        return (QST_ERROR);
    thread_del(thrd);

    /* 建立颜色值 */
    s_color_errs = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_RED);
    s_color_help = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_GREEN);
    s_color_text = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_GREEN |
                        CR_CUI_TEXT_RED | CR_CUI_TEXT_BLUE);
    cui_set_color(s_color_text);
    printf("######################################\n");
    printf("##    QUESTLAB COMMAND LINE TOOL    ##\n");
    printf("######################################\n");
    s_color_text = cui_make_attr(0, CR_CUI_TEXT_LIGHT |
                        CR_CUI_TEXT_GREEN | CR_CUI_TEXT_RED);
    cui_set_color(s_color_text);

    /* 输入文件有效, 执行后自动退出 */
    if (exist) {
        file = str_fmtA("$exec %s", file);
        if (file != NULL) {
            exec_one_line(file, netw, FALSE);
            mem_free(file);
        }
    }
    else
    {
        /* 工作循环 */
        for (;;)
        {
            ansi_t  cmd[512];

            /* 获取输入 */
            printf("> ");
            fgets(cmd, sizeof(cmd), stdin);

            /* 执行命令, 严重错误直接退出 */
            if (!exec_one_line(cmd, netw, FALSE))
                break;
        }
    }
    netw_cli_close(netw);
    return (QST_OKAY);
}
