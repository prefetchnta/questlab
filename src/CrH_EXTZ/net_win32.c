/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-11-23  */
/*     #######          ###    ###      [EXTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>> CrHack 网络功能扩展库 for Windows <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_NET_WIN32_C__
#define __CR_NET_WIN32_C__ 0xFCA58A72UL

#include "extz.h"
#include "memlib.h"
#if defined(_CR_OS_WINCE_)
    #include "strlib.h"
#endif

#include <windows.h>
#include <wininet.h>
#if  !defined(_CR_CC_MINGW_) && \
    (!defined(_CR_OS_WINCE_) || (_WIN32_WCE >= 0x0400))
    #include <urlmon.h>
#else
    #define _CR_NO_HTTP_
#endif

/* Windows 网络导入库 */
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "wininet.lib")
    #ifndef _CR_NO_HTTP_
        #pragma comment (lib, "urlmon.lib")
    #endif
#endif

/*****************************************************************************/
/*                               FTP 操作接口                                */
/*****************************************************************************/

/* FTP 操作内部结构 */
typedef struct
{
        HINTERNET   open;
        HINTERNET   conn;
} sWFTP;

/*
=======================================
    关闭 FTP 会话
=======================================
*/
CR_API void_t
win_ftp_close (
  __CR_IN__ wftp_t  ftps
    )
{
    sWFTP*  real;

    real = (sWFTP*)ftps;
    InternetCloseHandle(real->conn);
    InternetCloseHandle(real->open);
    mem_free(ftps);
}

/*
=======================================
    打开 FTP 会话A
=======================================
*/
CR_API wftp_t
win_ftp_openA (
  __CR_IN__ const ansi_t*   remote,
  __CR_IN__ const ansi_t*   user,
  __CR_IN__ const ansi_t*   password
    )
{
    sWFTP*  rett;

#ifndef _CR_OS_WINCE_

    /* 创建结构 */
    rett = struct_new(sWFTP);
    if (rett == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_ftp_openA()", "struct_new() failure");
        return (NULL);
    }

    /* 连到标准 FTP 服务端 */
    rett->open = InternetOpenA(CR_AS("CrHack"),
                    INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (rett->open == NULL) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_openA()", "InternetOpenA() failure");
        goto _failure;
    }
    rett->conn = InternetConnectA(rett->open,
                remote, INTERNET_INVALID_PORT_NUMBER, user, password,
                    INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
    if (rett->conn == NULL) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_openA()", "InternetConnectA() failure");
        InternetCloseHandle(rett->open);
        goto _failure;
    }
#else
    wide_t* rmtw;
    wide_t* usrw;
    wide_t* pssw;

    /* 创建结构 */
    rett = struct_new(sWFTP);
    if (rett == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_ftp_openA()", "struct_new() failure");
        return (NULL);
    }

    /* 转换编码 */
    rmtw = local_to_utf16(CR_LOCAL, remote);
    if (rmtw == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_ftp_openA()", "local_to_utf16() failure");
        goto _failure;
    }
    usrw = local_to_utf16(CR_LOCAL, user);
    if (usrw == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_ftp_openA()", "local_to_utf16() failure");
        mem_free(rmtw);
        goto _failure;
    }
    pssw = local_to_utf16(CR_LOCAL, password);
    if (usrw == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_ftp_openA()", "local_to_utf16() failure");
        mem_free(usrw);
        mem_free(rmtw);
        goto _failure;
    }

    /* 连到标准 FTP 服务端 */
    rett->open = InternetOpenW(CR_WS("CrHack"),
                    INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (rett->open == NULL) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_openA()", "InternetOpenW() failure");
        mem_free(pssw);
        mem_free(usrw);
        mem_free(rmtw);
        goto _failure;
    }
    rett->conn = InternetConnectW(rett->open,
                rmtw, INTERNET_INVALID_PORT_NUMBER, usrw, pssw,
                    INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
    if (rett->conn == NULL) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_openA()", "InternetConnectW() failure");
        InternetCloseHandle(rett->open);
        mem_free(pssw);
        mem_free(usrw);
        mem_free(rmtw);
        goto _failure;
    }
    mem_free(pssw);
    mem_free(usrw);
    mem_free(rmtw);
#endif
    /* 转到 FTP 根目录 */
    if (!FtpSetCurrentDirectoryW(rett->conn, CR_WS("/"))) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_openA()", "FtpSetCurrentDirectoryW() failure");
        win_ftp_close((wftp_t)rett);
        return (NULL);
    }
    return ((wftp_t)rett);

_failure:
    mem_free(rett);
    return (NULL);
}

/*
=======================================
    打开 FTP 会话W
=======================================
*/
CR_API wftp_t
win_ftp_openW (
  __CR_IN__ const wide_t*   remote,
  __CR_IN__ const wide_t*   user,
  __CR_IN__ const wide_t*   password
    )
{
    sWFTP*  rett;

    /* 创建结构 */
    rett = struct_new(sWFTP);
    if (rett == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_ftp_openW()", "struct_new() failure");
        return (NULL);
    }

    /* 连到标准 FTP 服务端 */
    rett->open = InternetOpenW(CR_WS("CrHack"),
                    INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (rett->open == NULL) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_openW()", "InternetOpenW() failure");
        goto _failure;
    }
    rett->conn = InternetConnectW(rett->open,
                remote, INTERNET_INVALID_PORT_NUMBER, user, password,
                    INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
    if (rett->conn == NULL) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_openW()", "InternetConnectW() failure");
        InternetCloseHandle(rett->open);
        goto _failure;
    }

    /* 转到 FTP 根目录 */
    if (!FtpSetCurrentDirectoryW(rett->conn, CR_WS("/"))) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_openW()", "FtpSetCurrentDirectoryW() failure");
        win_ftp_close((wftp_t)rett);
        return (NULL);
    }
    return ((wftp_t)rett);

_failure:
    mem_free(rett);
    return (NULL);
}

/*
=======================================
    从 FTP 下载文件A
=======================================
*/
CR_API bool_t
win_ftp_getfileA (
  __CR_IN__ wftp_t          ftps,
  __CR_IN__ const ansi_t*   root,
  __CR_IN__ const ansi_t*   ftp_file,
  __CR_IN__ const ansi_t*   loc_file
    )
{
    sWFTP*  real = (sWFTP*)ftps;

#ifndef _CR_OS_WINCE_
    if (root == NULL)
        root = CR_AS("/");
    if (!FtpSetCurrentDirectoryA(real->conn, root)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_getfileA()", "FtpSetCurrentDirectoryA() failure");
        return (FALSE);
    }
    if (!FtpGetFileA(real->conn, ftp_file, loc_file, FALSE,
            FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY |
                        INTERNET_FLAG_RELOAD, 0)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_getfileA()", "FtpGetFileA() failure");
        return (FALSE);
    }
    return (TRUE);
#else
    wide_t* rotw;
    wide_t* ftpw;
    wide_t* locw;

    /* 转换编码 */
    if (root == NULL) {
        rotw = (wide_t*)CR_WS("/");
    }
    else {
        rotw = local_to_utf16(CR_LOCAL, root);
        if (rotw == NULL) {
            err_set(__CR_NET_WIN32_C__, CR_NULL,
                    "win_ftp_getfileA()", "local_to_utf16() failure");
            return (FALSE);
        }
    }
    ftpw = local_to_utf16(CR_LOCAL, ftp_file);
    if (ftpw == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_ftp_getfileA()", "local_to_utf16() failure");
        if (root != NULL)
            mem_free(rotw);
        return (FALSE);
    }
    locw = local_to_utf16(CR_LOCAL, loc_file);
    if (locw == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_ftp_getfileA()", "local_to_utf16() failure");
        mem_free(ftpw);
        if (root != NULL)
            mem_free(rotw);
        return (FALSE);
    }

    /* 下载指定文件 */
    if (!FtpSetCurrentDirectoryW(real->conn, rotw)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_getfileA()", "FtpSetCurrentDirectoryW() failure");
        mem_free(locw);
        mem_free(ftpw);
        if (root != NULL)
            mem_free(rotw);
        return (FALSE);
    }
    if (root != NULL)
        mem_free(rotw);
    if (!FtpGetFileW(real->conn, ftpw, locw, FALSE,
            FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY |
                        INTERNET_FLAG_RELOAD, 0)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_getfileA()", "FtpGetFileW() failure");
        mem_free(locw);
        mem_free(ftpw);
        return (FALSE);
    }
    mem_free(locw);
    mem_free(ftpw);
    return (TRUE);
#endif
}

/*
=======================================
    从 FTP 下载文件W
=======================================
*/
CR_API bool_t
win_ftp_getfileW (
  __CR_IN__ wftp_t          ftps,
  __CR_IN__ const wide_t*   root,
  __CR_IN__ const wide_t*   ftp_file,
  __CR_IN__ const wide_t*   loc_file
    )
{
    sWFTP*  real = (sWFTP*)ftps;

    if (root == NULL)
        root = CR_WS("/");
    if (!FtpSetCurrentDirectoryW(real->conn, root)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_getfileW()", "FtpSetCurrentDirectoryW() failure");
        return (FALSE);
    }
    if (!FtpGetFileW(real->conn, ftp_file, loc_file, FALSE,
            FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY |
                        INTERNET_FLAG_RELOAD, 0)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_getfileW()", "FtpGetFileW() failure");
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    上传文件到 FTP-A
=======================================
*/
CR_API bool_t
win_ftp_putfileA (
  __CR_IN__ wftp_t          ftps,
  __CR_IN__ const ansi_t*   root,
  __CR_IN__ const ansi_t*   ftp_file,
  __CR_IN__ const ansi_t*   loc_file
    )
{
    sWFTP*  real = (sWFTP*)ftps;

#ifndef _CR_OS_WINCE_
    if (root == NULL)
        root = CR_AS("/");
    if (!FtpSetCurrentDirectoryA(real->conn, root)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_putfileA()", "FtpSetCurrentDirectoryA() failure");
        return (FALSE);
    }
    if (!FtpPutFileA(real->conn, loc_file, ftp_file,
            FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_putfileA()", "FtpPutFileA() failure");
        return (FALSE);
    }
    return (TRUE);
#else
    wide_t* rotw;
    wide_t* ftpw;
    wide_t* locw;

    /* 转换编码 */
    if (root == NULL) {
        rotw = (wide_t*)CR_WS("/");
    }
    else {
        rotw = local_to_utf16(CR_LOCAL, root);
        if (rotw == NULL) {
            err_set(__CR_NET_WIN32_C__, CR_NULL,
                    "win_ftp_putfileA()", "local_to_utf16() failure");
            return (FALSE);
        }
    }
    ftpw = local_to_utf16(CR_LOCAL, ftp_file);
    if (ftpw == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_ftp_putfileA()", "local_to_utf16() failure");
        if (root != NULL)
            mem_free(rotw);
        return (FALSE);
    }
    locw = local_to_utf16(CR_LOCAL, loc_file);
    if (locw == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_ftp_putfileA()", "local_to_utf16() failure");
        mem_free(ftpw);
        if (root != NULL)
            mem_free(rotw);
        return (FALSE);
    }

    /* 下载指定文件 */
    if (!FtpSetCurrentDirectoryW(real->conn, rotw)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_putfileA()", "FtpSetCurrentDirectoryW() failure");
        mem_free(locw);
        mem_free(ftpw);
        if (root != NULL)
            mem_free(rotw);
        return (FALSE);
    }
    if (root != NULL)
        mem_free(rotw);
    if (!FtpPutFileW(real->conn, locw, ftpw,
            FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_putfileA()", "FtpPutFileW() failure");
        mem_free(locw);
        mem_free(ftpw);
        return (FALSE);
    }
    mem_free(locw);
    mem_free(ftpw);
    return (TRUE);
#endif
}

/*
=======================================
    上传文件到 FTP-W
=======================================
*/
CR_API bool_t
win_ftp_putfileW (
  __CR_IN__ wftp_t          ftps,
  __CR_IN__ const wide_t*   root,
  __CR_IN__ const wide_t*   ftp_file,
  __CR_IN__ const wide_t*   loc_file
    )
{
    sWFTP*  real = (sWFTP*)ftps;

    if (root == NULL)
        root = CR_WS("/");
    if (!FtpSetCurrentDirectoryW(real->conn, root)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_putfileW()", "FtpSetCurrentDirectoryW() failure");
        return (FALSE);
    }
    if (!FtpPutFileW(real->conn, loc_file, ftp_file,
            FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0)) {
        err_set(__CR_NET_WIN32_C__, GetLastError(),
                "win_ftp_putfileW()", "FtpPutFileW() failure");
        return (FALSE);
    }
    return (TRUE);
}

/*****************************************************************************/
/*                              HTTP 操作接口                                */
/*****************************************************************************/

#ifndef _CR_NO_HTTP_
/*
=======================================
    从 HTTP 下载文件A
=======================================
*/
CR_API bool_t
win_http_getfileA (
  __CR_IN__ const ansi_t*   url,
  __CR_IN__ const ansi_t*   name
    )
{
    HRESULT rett;

#ifndef _CR_OS_WINCE_
    DeleteUrlCacheEntryA(url);
    rett = URLDownloadToFileA(NULL, url, name, 0, NULL);
    if (rett != S_OK) {
        err_set(__CR_NET_WIN32_C__, rett,
                "win_http_getfileA()", "URLDownloadToFileA() failure");
        return (FALSE);
    }
    return (TRUE);
#else
    wide_t* urlw;
    wide_t* wname;

    /* 转换编码 */
    urlw = local_to_utf16(CR_LOCAL, url);
    if (urlw == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_http_getfileA()", "local_to_utf16() failure");
        return (FALSE);
    }
    wname = local_to_utf16(CR_LOCAL, name);
    if (wname == NULL) {
        err_set(__CR_NET_WIN32_C__, CR_NULL,
                "win_http_getfileA()", "local_to_utf16() failure");
        mem_free(urlw);
        return (FALSE);
    }

    /* 清缓存并下载文件 */
    DeleteUrlCacheEntryW(urlw);
    rett = URLDownloadToFileW(NULL, urlw, wname, 0, NULL);
    mem_free(urlw); mem_free(wname);
    if (rett != S_OK) {
        err_set(__CR_NET_WIN32_C__, rett,
                "win_http_getfileA()", "URLDownloadToFileW() failure");
        return (FALSE);
    }
    return (TRUE);
#endif
}

/*
=======================================
    从 HTTP 下载文件W
=======================================
*/
CR_API bool_t
win_http_getfileW (
  __CR_IN__ const wide_t*   url,
  __CR_IN__ const wide_t*   name
    )
{
    HRESULT rett;

    DeleteUrlCacheEntryW(url);
    rett = URLDownloadToFileW(NULL, url, name, 0, NULL);
    if (rett != S_OK) {
        err_set(__CR_NET_WIN32_C__, rett,
                "win_http_getfileW()", "URLDownloadToFileW() failure");
        return (FALSE);
    }
    return (TRUE);
}

#endif  /* !_CR_NO_HTTP_ */

#endif  /* !__CR_NET_WIN32_C__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
