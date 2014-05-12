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
 *             ##       CREATE: 2012-06-27
 *              #
 ================================================
        QuestLAB 广播服务器
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 应用的名称 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstServ"
    #define WIN_TITLE   "QstServ"
    #define WIN_CLASS   "QstServCLSS"
    #define WIN_ICONF   "QstServ.ini"
    #define WIN_XCONF   "QstServ.xml"
#endif

/* 默认客户端数 */
#define QST_DEF_CLIENT  16
#define QST_MAX_CLIENT  64

/* 控制台窗口句柄 */
static HWND     s_hwnd = NULL;

/* 控制台输出句柄 */
static HANDLE   s_hcui = NULL;

/* 缺省的窗口大小 */
#define QSRV_DEF_WIDTH  668
#define QSRV_DEF_HEIGHT 434

/* 线程工作结构 */
struct  sQstWork
{
        uint_t      id;     /* 工作编号 */
        uint_t      cnt;    /* 单元个数 */
        sint_t      over;   /* 是否结束 */
        ansi_t*     name;   /* 工作名称 */
        void_t*     self;   /* 指向起始 */
        socket_t    netw;   /* 网络句柄 */
};

/* 防止警告的值 */
static bool_t   s_quit = FALSE;

/* 全局多线程锁 */
static lock_t   s_qst_lck_shw;  /* 显示信息的锁 */
static lock_t   s_qst_lck_wrk;  /* 线程工作的锁 */

/* 显示颜色配置 */
static int16u   s_qst_clr_cin;  /* 客户端连入颜色 */
static int16u   s_qst_clr_out;  /* 客户端断线颜色 */
static int16u   s_qst_clr_say;  /* 客户端说话颜色 */

/* 上线显示宏 */
#define _ENTER_SHOW_CIN_    mtlock_acquire(&s_qst_lck_shw); \
                            cui_set_color(s_qst_clr_cin);
#define _LEAVE_SHOW_CIN_    mtlock_release(&s_qst_lck_shw);

/* 下线显示宏 */
#define _ENTER_SHOW_OUT_    mtlock_acquire(&s_qst_lck_shw); \
                            cui_set_color(s_qst_clr_out);
#define _LEAVE_SHOW_OUT_    mtlock_release(&s_qst_lck_shw);

/* 广播显示宏 */
#define _ENTER_SHOW_SAY_    mtlock_acquire(&s_qst_lck_shw); \
                            cui_set_color(s_qst_clr_say);
#define _LEAVE_SHOW_SAY_    mtlock_release(&s_qst_lck_shw);

/*
---------------------------------------
    显示窗口位置
---------------------------------------
*/
static bool_t
qst_srv_win_show (void_t)
{
    misc_bring2top(s_hwnd, NULL);
    return (TRUE);
}

/*
---------------------------------------
    保存窗口位置
---------------------------------------
*/
static bool_t
qst_srv_win_save (void_t)
{
    RECT    win;

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
qst_srv_win_load (void_t)
{
    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 保存窗口位置 */
    if (!misc_desk_load(WIN_ICONF, &x1, &y1, &ww, &hh))
        return (FALSE);

    /* 设置窗口位置 */
    misc_bring2top(s_hwnd, NULL);
    return (misc_cui_setwin(s_hwnd, s_hcui, x1, y1, ww, hh));
}

/*****************************************************************************/
/*                                 工作单元                                  */
/*****************************************************************************/

/*
---------------------------------------
    生成工作单元组
---------------------------------------
*/
static sQstWork*
qst_wrk_create (
  __CR_IN__ uint_t  count
    )
{
    sQstWork*   work;

    /* 工作编号要这里填入 */
    if (count > QST_MAX_CLIENT)
        return (NULL);
    work = mem_talloc(count, sQstWork);
    if (work == NULL)
        return (NULL);
    for (uint_t idx = 0; idx < count; idx++) {
        work[idx].id = idx;
        work[idx].cnt = count;
        work[idx].over = TRUE;
        work[idx].name = NULL;
        work[idx].netw = NULL;
        work[idx].self = work;
    }
    return (work);
}

/*
---------------------------------------
    启动一个工作单元
---------------------------------------
*/
static bool_t
qst_wrk_wakeup (
  __CR_IO__ sQstWork*   work,
  __CR_IN__ ansi_t*     name,
  __CR_IN__ socket_t    netw,
  __CR_IN__ mt_main_t   proc
    )
{
    thrd_t  thrd;
    uint_t  count;

    /* 查找空闲单元 */
    count = work[0].cnt;
    for (uint_t idx = 0; idx < count; idx++) {
        if (work[idx].over) {
            if (!netw_ack_send(netw, TRUE))
                return (FALSE);
            thrd = thread_new(0, proc, &work[idx], TRUE);
            if (thrd == NULL)
                return (FALSE);
            work[idx].name = name;
            work[idx].netw = netw;
            work[idx].over = FALSE;
            if (thread_goon(thrd))
            {
                /* 显示已经连线 */
                _ENTER_SHOW_CIN_
                printf("[%s] is online.\n", name);
                _LEAVE_SHOW_CIN_
            }
            thread_del(thrd);
            return (TRUE);
        }
    }
    netw_ack_send(netw, FALSE);
    return (FALSE);
}

/*****************************************************************************/
/*                                 工作线程                                  */
/*****************************************************************************/

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_srv_main (
  __CR_IN__ void_t* param
    )
{
    uint_t      self, count;
    sQstWork*   ctx = (sQstWork*)param;
    sQstWork*   tot = (sQstWork*)ctx->self;

    /* 工作循环 */
    self = ctx->id;
    count = ctx->cnt;
    for (;;)
    {
        ansi_t* shell;
        ansi_t* string;

        /* 接收一条命令 */
        /* 一有错误表示非法客户端, 直接踢 */
        string = netw_cmd_recv_ex(ctx->netw);
        if (string == NULL)
            break;

        _ENTER_SHOW_SAY_
        printf("[%s] say: %s\n", ctx->name, string);
        _LEAVE_SHOW_SAY_

        /* 广播这条消息 */
        mtlock_acquire(&s_qst_lck_wrk);
        for (uint_t idx = 0; idx < count; idx++)
        {
            /* 跳过自己和空闲的单元 */
            if (idx == self || tot[idx].over)
                continue;
            netw_cmd_send(tot[idx].netw, string);
        }
        mtlock_release(&s_qst_lck_wrk);

        /* 处理一些公共指令 */
        shell = cmd_shl_get(string);
        mem_free(string);
        if (shell == NULL)
            continue;
        if (str_cmpA(shell, "app:exit") == 0 ||
            str_cmpA(shell, "qsrv:app:exit") == 0) {
            thread_sleep(1000);
            exit(0);
        }
        if (str_cmpA(shell, "win:load") == 0)
            qst_srv_win_load();
        else
        if (str_cmpA(shell, "win:save") == 0)
            qst_srv_win_save();
        else
        if (str_cmpA(shell, "win:show") == 0 ||
            str_cmpA(shell, "qsrv:win:show") == 0)
            qst_srv_win_show();
        mem_free(shell);
    }

    /* 显示已经断线 */
    _ENTER_SHOW_OUT_
    printf("[%s] is offline.\n", ctx->name);
    _LEAVE_SHOW_OUT_

    /* 释放资源 */
    mem_free(ctx->name);
    ctx->name = NULL;
    mtlock_acquire(&s_qst_lck_wrk);
    netw_cli_close(ctx->netw);
    ctx->netw = NULL;
    ctx->over = TRUE;
    mtlock_release(&s_qst_lck_wrk);
    return (QST_OKAY);
}

/*****************************************************************************/
/*                                 工作流程                                  */
/*****************************************************************************/

/* 客户端名称最大长度 */
#define QST_MAX_NAME    32

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);
    SetConsoleTitleA(WIN_TITLE);

    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 初始化窗口 */
    s_hcui = GetStdHandle(STD_OUTPUT_HANDLE);
    if (s_hcui == INVALID_HANDLE_VALUE)
        return (QST_ERROR);
    s_hwnd = GetConsoleWindow();
    if (s_hwnd == NULL)
        return (QST_ERROR);
    misc_desk_init(WIN_ICONF, &x1, &y1, &ww, &hh,
                   QSRV_DEF_WIDTH, QSRV_DEF_HEIGHT);
    misc_cui_setwin(s_hwnd, s_hcui, x1, y1, ww, hh);

    sINIu*  ini;
    uint_t  temp;
    ansi_t* text;
    int16u  port = QST_DEF_PORT;
    ansi_t* addr = QST_DEF_ADDR;
    uint_t  count = QST_DEF_CLIENT;

    /* 读取参数 */
    text = file_load_as_strA(QST_CFG_STARTUP);
    if (text != NULL) {
        ini = ini_parseU(text);
        mem_free(text);
        text = NULL;
        if (ini != NULL) {
            text = ini_key_stringU("serv::bind", ini);
            if (text != NULL)
                addr = text;
            temp = ini_key_intxU("serv::port", QST_DEF_PORT, ini);
            if (temp > 1024 && temp < 32768)
                port = (int16u)temp;
            temp = ini_key_intxU("serv::count", QST_DEF_CLIENT, ini);
            if (temp > 0 && temp <= QST_MAX_CLIENT)
                count = temp;
            ini_closeU(ini);
        }
    }

    socket_t    serv;
    sQstWork*   work;

    /* 建立工作单元组 */
    work = qst_wrk_create(count);
    if (work == NULL)
        return (QST_ERROR);

    /* 初始化网络 */
    if (!socket_init())
        return (QST_ERROR);
    serv = server_tcp_open(addr, port);
    TRY_FREE(text);
    if (serv == NULL)
        return (QST_ERROR);

    /* 初始化锁 */
    mtlock_init(&s_qst_lck_shw);
    mtlock_init(&s_qst_lck_wrk);

    int16u  logo_color;

    /* 建立颜色值 */
    logo_color = cui_make_attr(0, CR_CUI_TEXT_LIGHT |
                        CR_CUI_TEXT_GREEN | CR_CUI_TEXT_RED |
                        CR_CUI_TEXT_BLUE);
    s_qst_clr_cin = cui_make_attr(0, CR_CUI_TEXT_LIGHT |
                        CR_CUI_TEXT_GREEN | CR_CUI_TEXT_RED);
    s_qst_clr_say = cui_make_attr(0, CR_CUI_TEXT_LIGHT |
                        CR_CUI_TEXT_GREEN);
    s_qst_clr_out = cui_make_attr(0, CR_CUI_TEXT_LIGHT |
                        CR_CUI_TEXT_RED);
    /* 工作循环 */
    cui_set_color(logo_color);
    printf("######################################\n");
    printf("##    QUESTLAB BROADCAST SERVER     ##\n");
    printf("######################################\n");
    while (!s_quit)
    {
        ansi_t*     ptr;
        bool_t      okay;
        ansi_t*     name;
        socket_t    netw;

        /* 接受一个客户端 */
        netw = server_tcp_accept(serv);
        if (netw == NULL)
            continue;

        /* 接收客户端名称 */
        socket_set_timeout(netw, -1, QST_TCP_TOUT);
        name = netw_cmd_recv(netw);
        if (name == NULL) {
            socket_close(netw);
            continue;
        }

        /* 客户端名称过滤 */
        if (str_lenA(name) > QST_MAX_NAME) {
            netw_ack_send(netw, FALSE);
            netw_cli_close(netw);
            mem_free(name);
            continue;
        }
        ptr = name;
        okay = TRUE;
        while (*ptr != NIL) {
            if (is_cntrlA(*ptr++)) {
                okay = FALSE;
                break;
            }
        }
        if (!okay) {
            netw_ack_send(netw, FALSE);
            netw_cli_close(netw);
            mem_free(name);
            continue;
        }

        /* 启动一个工作单元 */
        if (!qst_wrk_wakeup(work, name, netw, qst_srv_main)) {
            netw_cli_close(netw);
            mem_free(name);
            continue;
        }
    }
    return (QST_OKAY);
}
