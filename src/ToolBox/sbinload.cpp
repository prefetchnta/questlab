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
 *             ##       CREATE: 2020-10-14
 *              #
 ================================================
        动态库加载测试小工具
 ================================================
 */

#include <stdio.h>
#include <stdlib.h>
#undef  WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    int     cnts;
    DWORD   code;
    HMODULE sbin;

    /* 参数解析 <分隔符个数> <动态库文件名> */
    if (argc < 3) {
        printf("sbinload <count> <dll filename>\n");
        return (1);
    }
    cnts = atoi(argv[1]);
    printf("%s ", argv[2]);
    sbin = LoadLibraryA(argv[2]);
    if (sbin != NULL) {
        // FreeLibrary(sbin);  // 这里有可能卡死
        for (int idx = 0; idx < cnts; idx++)
            printf("#");
        printf(" [OKAY]\n");
        return (0);
    }
    code = GetLastError();
    for (int idx = 0; idx < cnts; idx++)
        printf("-");
    printf(" [FAIL] %u\n", code);
    return (-1);
}
