
#include "QstComm.h"

/*
=======================================
    UDPv4 接收线程
=======================================
*/
CR_API uint_t STDCALL
qst_udpv4_main (
  __CR_IN__ void_t* parm
    )
{
    sQstComm*   ctx = (sQstComm*)parm;

    /* 工作循环 */
    while (!ctx->comm.quit)
    {
        ansi_t  data[65536];

        /* 一个个封包读 */
        ctx->size = socket_udp_recv(ctx->comm.obj.netw, data, sizeof(data));
        if (ctx->size == 0 || ctx->size > sizeof(data)) {
            thread_sleep(20);
            continue;
        }

        /* 文本模式处理 */
        if (ctx->comm.text)
            ctx->size = qst_txt_mode(data, ctx->size);

        /* 渲染读到的内容 */
        _ENTER_COM_SINGLE_
        ctx->comm.render(parm, data, ctx->size);
        _LEAVE_COM_SINGLE_

        /* 1000ms / 60Hz */
        thread_sleep(16);
    }

    /* 退出时关闭套接字 */
    socket_close(ctx->comm.obj.netw);
    return (QST_OKAY);
}

/*
=======================================
    UDPv4 数据发送
=======================================
*/
CR_API void_t
qst_udpv4_send (
  __CR_IN__ void_t*         obj,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    socket_udp_send((socket_t)obj, NULL, 0, data, size);
}
