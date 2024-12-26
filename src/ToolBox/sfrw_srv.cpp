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
 *             ##       CREATE: 2012-06-15
 *              #
 ================================================
        SFRW 服务端命令行程序
 ================================================
 */

#include "util/commlib.h"
#include "../QstLibs/QstLibs.h"
#pragma comment (lib, "CrH_UTIL.lib")

/* 显示信息的锁 */
static lock_t   s_lck_shw;

/* 防止警告的值 */
static bool_t   s_quit = FALSE;

/* 用户工作单元 */
struct  sfrw_t
{
        uint_t          type;   /* 功能值 */
        socket_t        netw;   /* 套接字 */
        const ansi_t*   down;   /* 下载目录 */
        const ansi_t*   uppe;   /* 上传目录 */
};

/*
---------------------------------------
    SFRW 工作线程
---------------------------------------
*/
static uint_t STDCALL
sfrw_main (
  __CR_IN__ void_t* param
    )
{
    int16u  port;
    ansi_t  ip[16];
    sfrw_t* user = (sfrw_t*)param;

    port = socket_remote_ipA(user->netw, ip);
    mtlock_acquire(&s_lck_shw);
    printf("INFO: client in %s:%u\n", ip, port);
    mtlock_release(&s_lck_shw);
    socket_set_timeout(user->netw, -1, 5000);
    sfrw_tcp_srv_loop(user->netw, user->type, user->down, user->uppe);
    socket_close(user->netw);
    mtlock_acquire(&s_lck_shw);
    printf("INFO: client out %s:%u\n", ip, port);
    mtlock_release(&s_lck_shw);
    return (0);
}

/*
=======================================
    命令行主函数
=======================================
*/
int main (int argc, char *argv[])
{
    int16u  port = 1234;
    uint_t  type = SFRW_DOWN | SFRW_UPPE;
    /* -------------------------------- */
    const ansi_t*   down = ".";
    const ansi_t*   uppe = ".";

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 [端口号] [功能参数] [下载根目录] [上传根目录] */
    if (argc > 1)
    {
        /* 参数1为端口号 */
        port = (int16u)str2intxA(argv[1]);
        if (argc > 2)
        {
            /* 参数2为功能参数 */
            type = (uint_t)str2intxA(argv[2]);
            if (argc > 3)
            {
                /* 参数3为下载根目录 */
                down = (const ansi_t*)argv[3];
                if (argc > 4)
                {
                    /* 参数4为上传根目录 */
                    uppe = (const ansi_t*)argv[4];
                }
            }
        }
    }

    socket_t    srv, cli;

    /* 初始化网络库 */
    if (!socket_init()) {
        printf("STOP: socket_init() failure!\n");
        return (QST_ERROR);
    }
    srv = server_tcp_open(NULL, port);
    if (srv == NULL) {
        printf("STOP: server_tcp_open() failure!\n");
        return (QST_ERROR);
    }

    /* 工作循环 */
    mtlock_init(&s_lck_shw);
    printf("Wellcom to SFRW server ;-)\n");
    while (!s_quit)
    {
        thrd_t  thrd;
        sfrw_t  user;

        cli = server_tcp_accept(srv);
        if (cli == NULL) {
            printf("WARN: server_tcp_accept() failure!\n");
            continue;
        }
        user.netw = cli;
        user.type = type;
        user.down = down;
        user.uppe = uppe;
        thrd = thread_new(0, sfrw_main, &user, FALSE);
        if (thrd == NULL) {
            printf("WARN: thread_new() failure!\n");
            socket_close(cli);
            continue;
        }
        thread_del(thrd);
    }
    return (QST_OKAY);
}
