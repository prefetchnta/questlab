
#include "QstComm.h"

/*
=======================================
    TCPv4 接收线程
=======================================
*/
CR_API uint_t STDCALL
qst_tcpv4_main (
  __CR_IN__ void_t* param
    )
{
    void_t*     parm = param;
    ansi_t      cha[2] = { 0, 0 };
    sQstComm*   ctx = (sQstComm*)param;

    /* 工作循环 */
    while (!ctx->comm.quit)
    {
        /* 一个个字节读 */
        if (socket_tcp_recv(ctx->comm.obj.netw, cha, 1) != 1) {
            thread_sleep(1);
            continue;
        }

        /* 渲染读到的内容 */
        _ENTER_COM_SINGLE_
        if (ctx->comm.render != NULL)
        {
            /* 自定义渲染器 */
            ctx->comm.render(parm, cha, 1);
        }
        else
        {
            /* 直接显示内容 */
            if (cha[0] != CR_AC('\n') || cha[1] != CR_AC('\r'))
                qst_direct_show(parm, cha, 1);
            cha[1] = cha[0];
        }
        _LEAVE_COM_SINGLE_
    }

    /* 退出时关闭套接字 */
    socket_close(ctx->comm.obj.netw);
    return (QST_OKAY);
}

/*
=======================================
    TCPv4 数据发送
=======================================
*/
CR_API void_t
qst_tcpv4_send (
  __CR_IN__ void_t*         obj,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    socket_tcp_send((socket_t)obj, data, size);
}
