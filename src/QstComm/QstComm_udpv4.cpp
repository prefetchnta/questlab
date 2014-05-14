
#include "QstComm.h"

/*
=======================================
    UDPv4 接收线程
=======================================
*/
CR_API uint_t STDCALL
qst_udpv4_main (
  __CR_IN__ void_t* param
    )
{
    ansi_t      cha[16384];
    void_t*     parm = param;
    sQstComm*   ctx = (sQstComm*)param;

    /* 工作循环 */
    while (!ctx->comm.quit)
    {
        uint_t  idx, rett;

        /* 一个个封包读 */
        rett = socket_udp_recv(ctx->comm.obj.netw, cha, sizeof(cha));
        if (rett == 0 || rett > sizeof(cha)) {
            thread_sleep(20);
            continue;
        }

        /* 渲染读到的内容 */
        _ENTER_COM_SINGLE_
        if (ctx->comm.text)
        {
            uint_t  size = 0;
            ansi_t* ptr = cha;

            /* 文本模式过滤换行符 */
            for (idx = 0; idx < rett - 1; idx++) {
                ptr[size++] = cha[idx];
                if (cha[idx] == CR_AC('\r') && cha[idx + 1] == CR_AC('\n'))
                    idx++;
            }
            if (idx == rett - 1)
                ptr[size++] = cha[idx];
            rett = size;
        }
        for (idx = 0; idx < rett; idx++)
            ctx->comm.render(parm, cha[idx]);
        _LEAVE_COM_SINGLE_
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
