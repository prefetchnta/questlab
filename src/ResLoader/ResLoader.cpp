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
 *             ##       CREATE: 2013-09-04
 *              #
 ================================================
        QuestLAB 外部资源文件加载接口
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 外部库引用 */
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "QstLibs.lib")
#endif

/* 从宿主传过来的参数 */
static socket_t         s_netw = NULL;
static const ansi_t*    s_root = NULL;

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    DLL 入口点
=======================================
*/
BOOL WINAPI
DllMain (
  __CR_IN__ HANDLE  hinst,
  __CR_IN__ DWORD   reason,
  __CR_UU__ LPVOID  reserved
    )
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            s_netw = NULL;
            s_root = NULL;
            break;

        case DLL_PROCESS_DETACH:
            TRY_FREE(s_root)
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}

#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/*                                 接口实现                                  */
/*****************************************************************************/

/*
---------------------------------------
    启动加载器
---------------------------------------
*/
static void_t
res_init (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   root
    )
{
    if (netw != NULL)
        s_netw = netw;
    SAFE_FREE(s_root)
    if (root != NULL)
        s_root = str_dupA(root);
}

/*
---------------------------------------
    释放加载器
---------------------------------------
*/
static void_t
res_kill (void_t)
{
    s_netw = NULL;
    SAFE_FREE(s_root)
}

/*
---------------------------------------
    加载外部文件
---------------------------------------
*/
static bool_t
res_load (
  __CR_OT__ sEX_FILE*       filex,
  __CR_IN__ const ansi_t*   type,
  __CR_IN__ const ansi_t*   mount,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          cpage
    )
{
    ansi_t* path;
    ansi_t* full;

    /* 跳过名称的根 */
    if (is_slashA(*name))
        name++;

    /* 直接使用名称预读 */
    if (s_root != NULL) {
        full = path_appendA(s_root, name);
        if (full == NULL)
            return (FALSE);
        if (file_existA(full)) {
            filex->is_free = TRUE;
            set_ldrA(&filex->ex_file, full, "", 0, 0);
            filex->ex_file.page = cpage;
            return (TRUE);
        }
        mem_free(full);
    }
    else {
        if (file_existA(name)) {
            filex->is_free = FALSE;
            set_ldrA(&filex->ex_file, name, "", 0, 0);
            filex->ex_file.page = cpage;
            return (TRUE);
        }
    }

    leng_t  idx, size;

    /* 以挂载名为根目录预读 */
    if (str_cmpA(mount, QST_STR_GLOBALS) != 0) {
        size = str_lenA(mount);
        for (idx = 0; idx < size; idx++) {
            if (mount[idx] == '|')
                break;
        }
        /* 包中包不做预读, 只做一层的预读 */
        if (idx >= size) {
            if (s_root != NULL)
                path = path_appendA(s_root, mount);
            else
                path = str_dupA(mount);
            if (path == NULL)
                return (FALSE);
            filext_removeA(path);
            full = path_appendA(path, name);
            mem_free(path);
            if (full == NULL)
                return (FALSE);
            if (file_existA(full)) {
                filex->is_free = TRUE;
                set_ldrA(&filex->ex_file, full, "", 0, 0);
                filex->ex_file.page = cpage;
                return (TRUE);
            }
            mem_free(full);
        }
    }

    /* 使用网络接口加载 */
    if (s_netw == NULL)
        return (FALSE);

    /* 发送资源加载命令 */
    full = str_fmtA("res:load \"%s\" \"%s\" \"%s\" %u",
                    type, mount, name, cpage);
    if (full == NULL)
        return (FALSE);
    if (!cmd_shl_send(s_netw, full)) {
        mem_free(full);
        return (FALSE);
    }
    mem_free(full);     /* 加大网络超时五倍 */
    socket_set_timeout(s_netw, -1, QST_TCP_TOUT * 5);

    /* 等待资源加载返回 */
    for (;;)
    {
        uint_t      argc;
        ansi_t**    argv;

        /* 接收一条消息, 出错退出 */
        full = netw_cmd_recv(s_netw);
        if (full == NULL)
            break;

        /* 判断是不是需要的消息 */
        path = cmd_shl_get(full);
        mem_free(full);
        if (path == NULL)
            continue;
        argv = cmd_shl_split(path, &argc);
        if (argv == NULL) {
            mem_free(path);
            break;
        }
        if ((argc < 2) ||
            (str_cmpA(argv[1], type) != 0)) {
            mem_free(argv);
            mem_free(path);
            continue;
        }
        if (str_cmpA(argv[0], "res:fail") == 0) {
            mem_free(argv);
            mem_free(path);
            break;
        }
        if (str_cmpA(argv[0], "res:okay") == 0) {
            if (argc < 3) {
                mem_free(argv);
                mem_free(path);
                break;
            }

            void_t* data;

            /* 返回读取的内存数据 */
            size = str2intA(argv[2]);
            mem_free(argv);
            mem_free(path);
            if ((dist_t)size <= 0)
                break;
            data = share_file_get(type, size);
            if (data == NULL)
                break;
            filex->is_free = TRUE;
            set_ldrM(&filex->ex_file, data, size, "", 0, 0);
            filex->ex_file.page = cpage;
            socket_set_timeout(s_netw, -1, QST_TCP_TOUT);
            return (TRUE);
        }
        mem_free(argv);
        mem_free(path);
    }

    /* 从循环出来表示失败 */
    socket_set_timeout(s_netw, -1, QST_TCP_TOUT);
    return (FALSE);
}

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

/* 外部文件加载接口表 */
static const sRES_LOADER
s_res_loader = { res_init, res_kill, res_load };

/*
=======================================
    获取外部文件加载接口表
=======================================
*/
CR_API const sRES_LOADER*
res_loader_get (void_t)
{
    return (&s_res_loader);
}
