//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "QstGraph.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
//---------------------------------------------------------------------------

/* UDP 数据包大小 */
#define QST_UDP_MAX     1500

/* 合成通讯命令字 */
#define QST_MK_CMD(v1, v2)  (((v2) << 8) | (v1))

/* 全局工作上下文 */
static sQstGraph    s_wrk_ctx;

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_grp_main (
  __CR_IN__ void_t* param
    )
{
    TfrmMain*   frm;
    sQstGraph*  ctx;

    /* 工作循环 */
    ctx = (sQstGraph*)param;
    frm = (TfrmMain*)(ctx->form);
    while (!ctx->quit)
    {
        bool    move;
        double  vals;
        uint_t  back, leng, idx;
        byte_t  data[QST_UDP_MAX], *pntr;

        /* 读取命令数据包 */
        mem_zero(data, sizeof(data));
        back = socket_udp_recv(ctx->netw, data, QST_UDP_MAX);
        if (back < sizeof(int16u) || back > QST_UDP_MAX)
            continue;

        /* 第一个字为命令号 */
        switch (*(int16u*)data)
        {
            default:                        /* 不支持的命令 */
                break;

            case QST_MK_CMD(0x00, 0x00):    /* 清除显示内容 */
                if (back == sizeof(int16u))
                    frm->clear();
                break;

            case QST_MK_CMD(0x00, 0x01):    /* 设置横坐标值 */
                if (back == sizeof(int16u) + sizeof(int32u))
                    frm->setBottom(*(int32u*)(&data[2]));
                break;

            case QST_MK_CMD(0x00, 0x02):    /* 设置横坐标值 */
                if (back == sizeof(int16u) + sizeof(fp32_t) * 2)
                    frm->setBottom(*(fp32_t*)(&data[2]), *(fp32_t*)(&data[6]));
                break;

            case QST_MK_CMD(0x00, 0x03):    /* 设置纵坐标值 */
                if (back == sizeof(int16u) + sizeof(fp32_t) * 2)
                    frm->setLeft(*(fp32_t*)(&data[2]), *(fp32_t*)(&data[6]));
                break;

            case QST_MK_CMD(0x00, 0x04):    /* 设置前景颜色 */
                if (back == sizeof(int16u) + sizeof(int32u))
                    frm->setLineColor(*(int32u*)(&data[2]), false);
                break;

            case QST_MK_CMD(0x00, 0x05):    /* 设置背景颜色 */
                if (back == sizeof(int16u) + sizeof(int32u))
                    frm->setLineColor(*(int32u*)(&data[2]), true);
                break;

            case QST_MK_CMD(0x00, 0x06):    /* 设置绘制线宽 */
                if (back == sizeof(int16u) + sizeof(byte_t))
                    frm->setLineWidth(data[2]);
                break;

            case QST_MK_CMD(0x01, 0x00):    /* 设置一个数值 */
                if (back <= sizeof(int16u) + sizeof(byte_t))
                    break;
                back -= sizeof(int16u) + sizeof(byte_t);

                /* 是否为平移模式 */
                move = (data[2] & 0x80) ? false : true;
                pntr = &data[3];

                /* 判断数据类型 */
                data[2] &= 0x7F;
                switch (data[2])
                {
                    default:    /* none */
                        break;

                    case 0x00:  /* ansi_t */
                        if (back == sizeof(ansi_t)) {
                            vals = (double)(*(ansi_t*)pntr);
                            frm->setValue(vals, move);
                        }
                        break;

                    case 0x01:  /* byte_t */
                        if (back == sizeof(byte_t)) {
                            vals = (double)(*(byte_t*)pntr);
                            frm->setValue(vals, move);
                        }
                        break;

                    case 0x02:  /* int16s */
                        if (back == sizeof(int16s)) {
                            vals = (double)(*(int16s*)pntr);
                            frm->setValue(vals, move);
                        }
                        break;

                    case 0x03:  /* int16u */
                        if (back == sizeof(int16u)) {
                            vals = (double)(*(int16u*)pntr);
                            frm->setValue(vals, move);
                        }
                        break;

                    case 0x04:  /* int32s */
                        if (back == sizeof(int32s)) {
                            vals = (double)(*(int32s*)pntr);
                            frm->setValue(vals, move);
                        }
                        break;

                    case 0x05:  /* int32u */
                        if (back == sizeof(int32u)) {
                            vals = (double)(*(int32u*)pntr);
                            frm->setValue(vals, move);
                        }
                        break;

                    case 0x06:  /* fp32_t */
                        if (back == sizeof(fp32_t)) {
                            vals = (double)(*(fp32_t*)pntr);
                            frm->setValue(vals, move);
                        }
                        break;

                    case 0x07:  /* fp64_t */
                        if (back == sizeof(fp64_t)) {
                            vals = (double)(*(fp64_t*)pntr);
                            frm->setValue(vals, move);
                        }
                        break;
                }
                break;

            case QST_MK_CMD(0x01, 0x01):    /* 设置一组数值 */
                if (back <= sizeof(int16u) + sizeof(int16u))
                    break;
                back -= sizeof(int16u) + sizeof(int16u);

                /* 数据长度最大128 */
                leng = (uint_t)data[3];
                if (leng == 0 || leng > 128)
                    break;

                /* 是否为平移模式 */
                move = (data[2] & 0x80) ? false : true;
                pntr = &data[4];

                /* 判断数据类型 */
                data[2] &= 0x7F;
                switch (data[2])
                {
                    default:    /* none */
                        break;

                    case 0x00:  /* ansi_t */
                        if (back == leng * sizeof(ansi_t)) {
                            for (idx = 0; idx < leng; idx++) {
                                vals = (double)(*(ansi_t*)pntr);
                                frm->setValue(vals, move);
                                pntr += sizeof(ansi_t);
                            }
                        }
                        break;

                    case 0x01:  /* byte_t */
                        if (back == leng * sizeof(byte_t)) {
                            for (idx = 0; idx < leng; idx++) {
                                vals = (double)(*(byte_t*)pntr);
                                frm->setValue(vals, move);
                                pntr += sizeof(byte_t);
                            }
                        }
                        break;

                    case 0x02:  /* int16s */
                        if (back == leng * sizeof(int16s)) {
                            for (idx = 0; idx < leng; idx++) {
                                vals = (double)(*(int16s*)pntr);
                                frm->setValue(vals, move);
                                pntr += sizeof(int16s);
                            }
                        }
                        break;

                    case 0x03:  /* int16u */
                        if (back == leng * sizeof(int16u)) {
                            for (idx = 0; idx < leng; idx++) {
                                vals = (double)(*(int16u*)pntr);
                                frm->setValue(vals, move);
                                pntr += sizeof(int16u);
                            }
                        }
                        break;

                    case 0x04:  /* int32s */
                        if (back == leng * sizeof(int32s)) {
                            for (idx = 0; idx < leng; idx++) {
                                vals = (double)(*(int32s*)pntr);
                                frm->setValue(vals, move);
                                pntr += sizeof(int32s);
                            }
                        }
                        break;

                    case 0x05:  /* int32u */
                        if (back == leng * sizeof(int32u)) {
                            for (idx = 0; idx < leng; idx++) {
                                vals = (double)(*(int32u*)pntr);
                                frm->setValue(vals, move);
                                pntr += sizeof(int32u);
                            }
                        }
                        break;

                    case 0x06:  /* fp32_t */
                        if (back == leng * sizeof(fp32_t)) {
                            for (idx = 0; idx < leng; idx++) {
                                vals = (double)(*(fp32_t*)pntr);
                                frm->setValue(vals, move);
                                pntr += sizeof(fp32_t);
                            }
                        }
                        break;

                    case 0x07:  /* fp64_t */
                        if (back == leng * sizeof(fp64_t)) {
                            for (idx = 0; idx < leng; idx++) {
                                vals = (double)(*(fp64_t*)pntr);
                                frm->setValue(vals, move);
                                pntr += sizeof(fp64_t);
                            }
                        }
                        break;
                }
                break;
        }
    }
    return (QST_OKAY);
}

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
    uint_t      port;
    uint_t      argc;
    ansi_t**    argv;

    CR_NOUSE(curt_app);
    CR_NOUSE(prev_app);
    CR_NOUSE(cmd_show);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_GUI))
        return (QST_ERROR);

    /* 获取命令行参数, 不包括进程文件名 */
    argv = misc_get_param(cmd_line, &argc);

    /* 参数解析 <监听地址> <监听端口> <标题> [图表类型=1] [窗口配置文件] */
    if (argc < 3)
        return (QST_ERROR);
    port = str2intxA(argv[1]);
    if (port <= 1024 || port >= 32768)
        return (QST_ERROR);
    mem_zero(&s_wrk_ctx, sizeof(s_wrk_ctx));
    if (argc > 3) {
        s_wrk_ctx.type = str2intxA(argv[3]);
        if (s_wrk_ctx.type > 11)
            return (QST_ERROR);
        if (argc > 4) {
            if (!misc_desk_load(argv[4], &s_wrk_ctx.winx, &s_wrk_ctx.winy,
                                         &s_wrk_ctx.winw, &s_wrk_ctx.winh))
                s_wrk_ctx.winx = s_wrk_ctx.winy = -1;
        }
    }
    else {
        s_wrk_ctx.type = 1;
    }

    /* 初始化网络 */
    if (!socket_init())
        return (QST_ERROR);
    s_wrk_ctx.netw = server_udp_open(argv[0], (int16u)port);
    if (s_wrk_ctx.netw == NULL)
        return (QST_ERROR);
    socket_set_timeout(s_wrk_ctx.netw, -1, QST_TCP_TOUT);

    thrd_t  thrd;

    /* 生成工作线程 */
    s_wrk_ctx.quit = FALSE;
    thrd = thread_new(0, qst_grp_main, &s_wrk_ctx, TRUE);
    if (thrd == NULL)
        return (QST_ERROR);

    /* 标准 VCL 过程开始 */
    try
    {
        Application->Initialize();
        Application->Title = WIN_TITLE;
        Application->CreateForm(__classid(TfrmMain), &frmMain);
        frmMain->setup(s_wrk_ctx.type, AnsiString(argv[2]), s_wrk_ctx.winx,
                        s_wrk_ctx.winy, s_wrk_ctx.winw, s_wrk_ctx.winh);
        s_wrk_ctx.form = (void_t*)frmMain;
        if (!thread_goon(thrd))
            return (QST_ERROR);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }

    /* 关闭线程直接退出 */
    if (!s_wrk_ctx.quit)
        s_wrk_ctx.quit = TRUE;
    thread_wait(thrd);
    thread_del(thrd);
    socket_close(s_wrk_ctx.netw);
    return (QST_OKAY);
}
//---------------------------------------------------------------------------
