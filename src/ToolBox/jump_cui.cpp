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
 *             ##       CREATE: 2023-04-11
 *              #
 ================================================
        Windows 11 控制台跳转程序
 ================================================
 */

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>

/*
=======================================
    WinMain 程序入口
=======================================
*/
int WINAPI WinMain (HINSTANCE   curt_app,
                    HINSTANCE   prev_app,
                    LPSTR       cmd_line,
                    int         cmd_show)
{
    char*               pp;
    char*               tt;
    DWORD               cf;
    size_t              ii;
    size_t              nn;
    STARTUPINFOA        si;
    unsigned char       cc;
    PROCESS_INFORMATION pi;

    /* 第一个字符是隐藏标志 */
    if (*cmd_line != '0' && *cmd_line != '1')
        return (__LINE__);
    cf = 1 & *cmd_line++;

    /* 16进制转义转换回来 */
    nn = strlen(cmd_line);
    if (nn == 0 || nn % 2 != 0)
        return (__LINE__);
    pp = (char*)malloc(nn / 2 + 1);
    if (pp == NULL)
        return (__LINE__);
    for (ii = 0; ii < nn; ii += 2)
    {
        char    ch1 = cmd_line[ii];
        char    ch2 = cmd_line[ii + 1];

        if ((ch1 >= '0' && ch1 <= '9')) {
            cc = (unsigned char)((0x0F & ch1));
        }
        else
        if ((ch1 >= 'a' && ch1 <= 'f') ||
            (ch1 >= 'A' && ch1 <= 'F')) {
            cc = (unsigned char)((0x0F & ch1) + 9);
        }
        else {
            free(pp);
            return (__LINE__);
        }

        if ((ch2 >= '0' && ch2 <= '9')) {
            cc <<= 4;
            cc |= (unsigned char)((0x0F & ch2));
        }
        else
        if ((ch2 >= 'a' && ch2 <= 'f') ||
            (ch2 >= 'A' && ch2 <= 'F')) {
            cc <<= 4;
            cc |= (unsigned char)((0x0F & ch2) + 9);
        }
        else {
            free(pp);
            return (__LINE__);
        }
        pp[ii / 2] = (char)cc;
    }
    pp[ii / 2] = 0;

    /* 拼装 conhost.exe 到前面 */
    tt = (char*)malloc(32 + strlen(pp));
    if (tt == NULL) {
        free(pp);
        return (__LINE__);
    }
    sprintf(tt, "conhost.exe %s", pp);
    free(pp);

    /* 执行命令 */
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(STARTUPINFOA);
    cf = cf ? CREATE_NO_WINDOW : CREATE_NEW_CONSOLE;
    CreateProcessA(NULL, tt, NULL, NULL, FALSE, cf, NULL, NULL, &si, &pi);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    free(tt);

    /* 无用的参数 */
    curt_app = curt_app;
    prev_app = prev_app;
    cmd_show = cmd_show;
    return (0);
}
