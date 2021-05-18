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
 *             ##       CREATE: 2012-06-29
 *              #
 ================================================
        QuestLAB 公用函数库
 ================================================
 */

#ifndef __QL_QSTLIBS_H__
#define __QL_QSTLIBS_H__

#include "crhack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 外部库引用 */
#pragma comment (lib, "CrH_CORE.lib")
#ifndef _CR_BUILD_DLL_
    #pragma comment (lib, "QstLibs.lib")
#endif

/*****************************************************************************/
/*                                 公用定义                                  */
/*****************************************************************************/

/* 库函数返回值 */
#define QST_OKAY       (0L)
#define QST_ERROR   (__LINE__)

/* 默认网络参数 */
#define QST_TCP_TOUT        1000        /* 网络读取超时 */
#define QST_DEF_PORT        2012        /* 登录缺省端口 */
#define QST_DEF_ADDR    "127.0.0.1"     /* 登录缺省地址 */

/* 缺省目录路径 */
#define QST_PATH_CONFIG ".\\config\\"   /* 应用程序配置 */
#define QST_PATH_EXT3RD ".\\ext3rd\\"   /* 外部应用程序 */
#define QST_PATH_OUTPUT ".\\output\\"   /* 临时文件目录 */
#define QST_PATH_PLUGIN ".\\plugin\\"   /* 应用程序插件 */
#define QST_PATH_SCRIPT ".\\script\\"   /* 外部脚本汇总 */
#define QST_PATH_SOURCE ".\\source\\"   /* 外部数据文件 */
#define QST_PATH_WINDOW ".\\window\\"   /* 窗口位置配置 */

/* 固定配置文件 */
#define QST_CFG_STARTUP ".\\startup.ini"    /* 启动配置文件 */

/* 一些临时文件 */
#define QST_STOPS_NEXT  ".\\output\\__ext__.tag"    /* 停止继续执行 */
#define QST_SELECT_DIR  ".\\output\\__pos__.txt"    /* 目录选择结果 */
#define QST_IMPORT_LST  ".\\output\\__dir__.xml"    /* 导入文件列表 */
#define QST_IMP_SCRIPT  ".\\output\\__imp__.xml"    /* 临时导入脚本 */
#define QST_TMP_SCRIPT  ".\\output\\__scr__.txt"    /* 临时执行脚本 */
#define QST_LOAD_FILEX  ".\\output\\__ldr__.txt"    /* 打开文件结果 */
#define QST_SAVE_FILEX  ".\\output\\__sav__.txt"    /* 保存文件结果 */
#define QST_ROOT_START  ".\\output\\__beg__.txt"    /* 保存启动目录 */

/* 一些字符串常数 */
#define QST_STR_GLOBALS     "|GLOBALS|"     /* 全局封包文件 */

/*****************************************************************************/
/*                                 公用函数                                  */
/*****************************************************************************/

/* 内存共享文件 */
typedef void_t* share_t;

CR_API share_t  STDCALL share_file_open (const ansi_t *name,
                                         ansi_t strn[50], leng_t size);
CR_API void_t   STDCALL share_file_close (share_t sfile);
CR_API bool_t   STDCALL share_file_fill (share_t sfile, const void_t *data,
                                         leng_t size);
CR_API void_t*  STDCALL share_file_get (const ansi_t *name, leng_t size);
CR_API bool_t   STDCALL share_file_set (const ansi_t *name, const void_t *data,
                                        leng_t size);
/* UDP 网络通讯 */
CR_API bool_t   STDCALL message_pipe_init (void_t);
CR_API socket_t STDCALL message_recv_open (int16u sport);
CR_API socket_t STDCALL message_send_open (int16u dport);
CR_API void_t   STDCALL message_pipe_close (socket_t mess);
CR_API void_t   STDCALL message_pipe_timeout (socket_t mess, int32s time);
CR_API bool_t   STDCALL message_send_buffer (socket_t mess, const void_t *data,
                                             uint_t size);
CR_API bool_t   STDCALL message_send_string (socket_t mess,
                                             const ansi_t *string);
CR_API uint_t   STDCALL message_recv_buffer (socket_t mess, void_t *data,
                                             uint_t size);
CR_API bool_t   STDCALL message_recv_string (socket_t mess, ansi_t *buff,
                                             uint_t size);
/* TCP 网络通讯 */
CR_API bool_t   STDCALL netw_cmd_send (socket_t netw, const ansi_t *string);
CR_API ansi_t*  STDCALL netw_cmd_recv (socket_t netw);
CR_API ansi_t*  STDCALL netw_cmd_recv_ex (socket_t netw);
CR_API bool_t   STDCALL netw_ack_send (socket_t netw, bool_t ack);
CR_API bool_t   STDCALL netw_ack_recv (socket_t netw, bool_t *ack);
CR_API bool_t   STDCALL netw_cli_init (void_t);
CR_API socket_t STDCALL netw_cli_open (const ansi_t *name);
CR_API void_t   STDCALL netw_cli_close (socket_t netw);
CR_API void_t   STDCALL netw_rcv_shutdwn (socket_t netw);
CR_API void_t   STDCALL netw_rcv_timeout (socket_t netw, int32s time);

/* 命令执行单元 */
typedef struct
{
        /* 命令的名称 */
        const ansi_t*   name;

        /* 命令执行回调 */
        bool_t  (*exec) (void_t*, uint_t, ansi_t**);

} sQST_CMD;

typedef void_t*     exec_t;     /* 命令执行对象 */
typedef void_t*     hcui_t;     /* CUI 输出句柄 */

/* 操作命令函数 */
CR_API ansi_t*  STDCALL cmd_ini_get (const ansi_t *string);
CR_API bool_t   STDCALL cmd_ini_send (socket_t netw, const ansi_t *string);
CR_API ansi_t*  STDCALL cmd_xml_get (const ansi_t *string);
CR_API bool_t   STDCALL cmd_xml_send (socket_t netw, const ansi_t *string);
CR_API ansi_t** STDCALL cmd_shl_split (ansi_t *string, uint_t *count);
CR_API ansi_t*  STDCALL cmd_shl_param (ansi_t **param, uint_t index);
CR_API ansi_t*  STDCALL cmd_shl_get (const ansi_t *string);
CR_API bool_t   STDCALL cmd_shl_send (socket_t netw, const ansi_t *string);
CR_API bool_t   STDCALL cmd_type_okay (const ansi_t *string);
CR_API void_t   STDCALL cmd_exec_free (exec_t objs);
CR_API exec_t   STDCALL cmd_exec_init (const sQST_CMD *list, leng_t count);
CR_API bool_t   STDCALL cmd_exec_addn (exec_t objs, const sQST_CMD* list);
CR_API bool_t   STDCALL cmd_exec_main (exec_t objs, void_t *param,
                                       const ansi_t *cmdz);
/* 杂项功能函数 */
CR_API bool_t   STDCALL misc_dir_exist (const ansi_t *path);
CR_API void_t   STDCALL misc_bring2top (hwnd_t hwnd, hwnd_t parent);
CR_API void_t*  STDCALL misc_mem_malloc (uint_t size);
CR_API void_t*  STDCALL misc_mem_calloc (uint_t num, uint_t size);
CR_API void_t*  STDCALL misc_mem_realloc (void_t *ptr, uint_t new_size);
CR_API void_t   STDCALL misc_mem_free (const void_t *data);
CR_API bool_t   STDCALL misc_cui_setwin (hwnd_t hwnd, hcui_t hcui,
                                sint_t x, sint_t y, uint_t w, uint_t h);
CR_API bool_t   STDCALL misc_is_running (const ansi_t *name);
CR_API bool_t   STDCALL misc_call_exe (const ansi_t *name, bool_t wait,
                                       bool_t hide);
CR_API ansi_t** STDCALL misc_get_param (const ansi_t *cmdz, uint_t *count);
CR_API bool_t   STDCALL misc_desk_save (const ansi_t *name,
                            sint_t left, sint_t top, uint_t width,
                                        uint_t height);
CR_API bool_t   STDCALL misc_desk_load (const ansi_t *name,
                            sint_t *left, sint_t *top, uint_t *width,
                                        uint_t *height);
CR_API void_t   STDCALL misc_desk_init (const ansi_t *name, sint_t *left,
                            sint_t *top, uint_t *width, uint_t *height,
                                        uint_t def_w, uint_t def_h);
/* 异步执行结构头 */
typedef struct
{
        volatile sint_t copyed;

} sQST_CTX;

CR_API void_t   STDCALL misc_async_call (mt_main_t func, sQST_CTX *param);
CR_API void_t   STDCALL misc_async_okay (sQST_CTX *param);

/*****************************************************************************/
/*                               QstGraph API                                */
/*****************************************************************************/

CR_API socket_t STDCALL QstGraph_Init (const ansi_t *addr, uint_t port);
CR_API void_t   STDCALL QstGraph_Free (socket_t netw);
CR_API void_t   STDCALL QstGraph_Clear (socket_t netw);
CR_API void_t   STDCALL QstGraph_SetAxisBottom (socket_t netw, uint_t max);
CR_API void_t   STDCALL QstGraph_SetAxisBottom2 (socket_t netw, fp32_t min,
                                                 fp32_t max);
CR_API void_t   STDCALL QstGraph_SetAxisLeft (socket_t netw, fp32_t min,
                                              fp32_t max);
CR_API void_t   STDCALL QstGraph_SetLineColor (socket_t netw, uint_t r,
                                               uint_t g, uint_t b);
CR_API void_t   STDCALL QstGraph_SetChar (socket_t netw, ansi_t value,
                                          bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetUChar (socket_t netw, byte_t value,
                                           bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetShort (socket_t netw, int16s value,
                                           bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetUShort (socket_t netw, int16u value,
                                            bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetInt (socket_t netw, sint_t value,
                                         bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetUInt (socket_t netw, uint_t value,
                                          bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetFloat (socket_t netw, fp32_t value,
                                           bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetDouble (socket_t netw, fp64_t value,
                                            bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetCharList (socket_t netw,
                                        const ansi_t *list, uint_t count,
                                            bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetUCharList (socket_t netw,
                                        const byte_t *list, uint_t count,
                                            bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetShortList (socket_t netw,
                                        const int16s *list, uint_t count,
                                            bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetUShortList (socket_t netw,
                                        const int16u *list, uint_t count,
                                            bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetIntList (socket_t netw,
                                        const sint_t *list, uint_t count,
                                            bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetUIntList (socket_t netw,
                                        const uint_t *list, uint_t count,
                                            bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetFloatList (socket_t netw,
                                        const fp32_t *list, uint_t count,
                                            bool_t move CR_DEFAULT(TRUE));
CR_API void_t   STDCALL QstGraph_SetDoubleList (socket_t netw,
                                        const fp64_t *list, uint_t count,
                                            bool_t move CR_DEFAULT(TRUE));

#endif  /* !__QL_QSTLIBS_H__ */
