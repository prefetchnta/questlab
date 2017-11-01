
#include "QstComm.h"

/*
=======================================
    TCPv4 接收线程
=======================================
*/
CR_API uint_t STDCALL
qst_tcpv4_main (
  __CR_IN__ void_t* parm
    )
{
    sQstComm*   ctx = (sQstComm*)parm;

    /* 工作循环 */
    while (!ctx->comm.quit)
    {
        uint_t  back;
        ansi_t  cha, *data;

        /* 一个个字节读 */
        back = socket_tcp_recv(ctx->comm.obj.netw, &cha, 1);
        if (back == 1) {
            if (CR_VCALL(ctx->bufs)->putb_no(ctx->bufs, cha))
                ctx->size += 1;
            continue;
        }
        if (back != CR_SOCKET_TIMEOUT) {
            thread_sleep(20);
            continue;
        }

        /* 文本模式处理 */
        if (ctx->size == 0)
            continue;
        data = (ansi_t*)(CR_VCALL(ctx->bufs)->flush(ctx->bufs));
        if (ctx->comm.text)
            ctx->size = qst_txt_mode(data, ctx->size);

        /* 渲染读到的内容 */
        _ENTER_COM_SINGLE_
        ctx->comm.render(parm, data, ctx->size);
        _LEAVE_COM_SINGLE_

        /* 缓存指针复位 */
        CR_VCALL(ctx->bufs)->reput(ctx->bufs, 0);
        ctx->size = 0;
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
