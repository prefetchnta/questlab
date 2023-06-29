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
 *             ##       CREATE: 2021-06-08
 *              #
 ================================================
    【64】QUEST64 QV2D f 扩展服务
 ================================================
 */

#define _QUEST64_
#include "../QstLibs/QstLibs.h"
#include "../CrHackSet/xCrHackSet.h"
#include "labai/imglab.h"

/* 一些公用宏 */
#define EXE_XNAME   "fQUEST64"

/* 外部库引用 */
#pragma comment (lib, "LabAI.lib")
#pragma comment (lib, "CrH_GFX2.lib")

/* 桥接函数 */
#include "../bridge.inl"

/* 工作变量 */
/* Width / Height / BPL / Size */
static int32u   s_info[4];
static share_t  s_smem = NULL;

/* 简化代码 */
#define quest64_return_true()   message_send_buffer(mess, "OKAY", 4)
#define quest64_return_false()  message_send_buffer(mess, "FAIL", 4)

/*
---------------------------------------
    填充图片结构
---------------------------------------
*/
static void_t
quest64_set_image (
  __CR_OT__ sIMAGE* image,
  __CR_IN__ void_t* pixels
    )
{
    image_set(image, pixels, s_info[3], 0, 0, s_info[0],
              s_info[1], CR_ARGB8888);
}

/*
---------------------------------------
    发送返回字符串
---------------------------------------
*/
static void_t
quest64_return_string (
  __CR_IN__ socket_t        mess,
  __CR_IN__ const ansi_t*   string
    )
{
    int32u  size;

    if (string == NULL) {
        size = 0;
        message_send_buffer(mess, &size, sizeof(size));
    }
    else {
        size = (int32u)str_sizeA(string);
        message_send_buffer(mess, &size, sizeof(size));
        message_send_buffer(mess, string, size);
    }
}

/* 滤镜函数 */
#include "filter.inl"

/* 滤镜接口表 */
static const sXC_PORT   quest64_filter[] =
{
    { "quest64_helloworld", quest64_helloworld },
    { "quest64_opencv_cascade", quest64_ocv_cascade },
    { NULL, NULL },
};

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
    uint_t      back;
    leng_t      size;
    void_t*     data;
    socket_t    mess;
    xmlcaller_t xmlc;

    CR_NOUSE(curt_app);
    CR_NOUSE(prev_app);
    CR_NOUSE(cmd_line);
    CR_NOUSE(cmd_show);

    /* 只允许一个例程 */
    if (misc_is_running(EXE_XNAME))
        return (QST_ERROR);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_GUI))
        return (QST_ERROR);
    message_pipe_init();
    mem_zero(s_info, sizeof(s_info));
    s_smem = NULL;

    /* 创建工作端口 */
    mess = message_recv_open(CRH_REMOTE_FLT_PORT);
    if (mess == NULL)
        return (QST_ERROR);
    message_pipe_timeout(mess, QST_TCP_TOUT);

    /* 加载接口列表 */
    xmlc = xmlcall_load(NULL, cntsof(quest64_filter), mess);
    if (xmlc == NULL) {
        message_pipe_close(mess);
        return (QST_ERROR);
    }
    if (!xmlcall_setup(xmlc, quest64_filter)) {
        xmlcall_unload(xmlc);
        message_pipe_close(mess);
        return (QST_ERROR);
    }

    /* 开始工作 */
    for (;;)
    {
        ansi_t  str[65536];

        /* 接收数据包 */
        back = message_recv_buffer(mess, s_info, sizeof(int64u));
        if (back == sizeof(int64u))
        {
            /* 创建 RGB 缓冲区 */
            size  = s_info[0];
            size *= sizeof(int32u);
            s_info[2] = (int32u)size;
            size *= s_info[1];
            if (s_info[3] < size) {
                if (s_smem != NULL)
                    share_file_close(s_smem);
                s_smem = share_file_open(CRH_FLT_RGB_DATA, NULL, size);
                if (s_smem == NULL) {
                    quest64_return_false();
                    s_info[3] = 0;
                    continue;
                }
                s_info[3] = (int32u)size;
            }
            data = share_file_map(s_smem, size);
            if (data == NULL) {
                quest64_return_false();
                continue;
            }
            quest64_return_true();

            /* 接收命令字符串 */
            if (message_recv_string(mess, str, sizeof(str))) {
                try {
                    xmlcall_exec(xmlc, data, str, NULL);
                }
                catch (...) {
                    MessageBoxA(NULL, "EXCEPTION OCCURED !!!",
                                "STOP", MB_OK | MB_ICONSTOP);
                }
            }

            /* 用完释放之 */
            share_file_unmap(s_smem);
        }
        else
        if (back == sizeof(int16u))
        {
            /* 退出工作线程 */
            if (*(int16u*)s_info == 0x55AA)
                break;
        }
        else
        {
            /* 返回出错 */
            quest64_return_false();
        }
    }

    /* 释放资源 */
    if (s_smem != NULL)
        share_file_close(s_smem);
    xmlcall_unload(xmlc);
    message_pipe_close(mess);
    return (QST_OKAY);
}
