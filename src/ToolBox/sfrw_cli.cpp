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
 *             ##       CREATE: 2012-06-18
 *              #
 ================================================
        SFRW 客户端命令行程序
 ================================================
 */

#include "util/commlib.h"
#include "../QstLibs/QstLibs.h"
#pragma comment (lib, "CrH_UTIL.lib")

/*
---------------------------------------
    显示工具用法
---------------------------------------
*/
static void_t usage (void_t)
{
    printf("sfrw_cli <server> [port=1234]\n");
}

/*
=======================================
    命令行主函数
=======================================
*/
int main (int argc, char *argv[])
{
    int16u  port;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 <服务器地址> [服务器端口] */
    if (argc < 2) {
        usage();
        return (QST_ERROR);
    }
    if (argc > 2)
        port = (int16u)str2intxA(argv[2]);
    else
        port = 1234;

    socket_t    netw;
    sSFRW_INFO  info;

    /* 初始化网络库 */
    if (!socket_init()) {
        printf("STOP: socket_init() failure!\n");
        return (QST_ERROR);
    }

    bool_t  quit = FALSE;

    /* 工作循环 */
    while (!quit)
    {
        uint_t  cmd = 0;
        ansi_t  name[MAX_PATHA];
        ansi_t  path[MAX_PATHA];

        /* 功能提示 */
        printf("=========\n");
        printf("1. find  \n");
        printf("2. pull  \n");
        printf("3. push  \n");
        printf("4. exit  \n");
        printf("=========\n> ");
        scanf("%u", &cmd);
        if (cmd < 1 || cmd > 4) {
            printf("\n");
            continue;
        }

        /* 命令解析 */
        switch (cmd)
        {
            default:
            case 1: /* find */
                printf("path: ");
                scanf("%s", path);
                netw = client_tcp_open(argv[1], port, 5000);
                if (netw == NULL) {
                    printf("STOP: client_tcp_open() failure!\n");
                    return (QST_ERROR);
                }
                if (!sfrw_tcp_cli_find(netw, &info, path)) {
                    printf("WARN: sfrw_tcp_cli_find() failure!\n\n");
                    socket_close(netw);
                    break;
                }
                printf("FSIZE: %u\n", info.size);
                printf("CRC32: %08X\n", info.crc32);
                printf("INFO: find success\n\n");
                socket_close(netw);
                break;

            case 2: /* pull */
                printf("path: ");
                scanf("%s", path);
                printf("name: ");
                scanf("%s", name);
                netw = client_tcp_open(argv[1], port, 5000);
                if (netw == NULL) {
                    printf("STOP: client_tcp_open() failure!\n");
                    return (QST_ERROR);
                }
                if (!sfrw_tcp_cli_pull(netw, path, name)) {
                    printf("WARN: sfrw_tcp_cli_pull() failure!\n\n");
                    socket_close(netw);
                    break;
                }
                printf("INFO: pull success\n\n");
                socket_close(netw);
                break;

            case 3: /* push */
                printf("path: ");
                scanf("%s", path);
                printf("name: ");
                scanf("%s", name);
                netw = client_tcp_open(argv[1], port, 5000);
                if (netw == NULL) {
                    printf("STOP: client_tcp_open() failure!\n");
                    return (QST_ERROR);
                }
                if (!sfrw_tcp_cli_push(netw, path, name)) {
                    printf("WARN: sfrw_tcp_cli_push() failure!\n\n");
                    socket_close(netw);
                    break;
                }
                printf("INFO: push success\n\n");
                socket_close(netw);
                break;

            case 4: /* exit */
                printf("GoodBye\n");
                quit = TRUE;
                break;
        }
    }
    return (QST_OKAY);
}
