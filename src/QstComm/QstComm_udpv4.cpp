
#include "QstCommInt.h"

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
        uint_t  rett;

        /* 一个个封包读 */
        rett = socket_udp_recv(ctx->comm.obj.netw, cha, sizeof(cha));
        if (rett == 0 || rett > sizeof(cha)) {
            thread_sleep(1);
            continue;
        }

        /* 渲染读到的内容 */
        _ENTER_COM_SINGLE_
        if (ctx->comm.render != NULL)
        {
            /* 自定义渲染器 */
            ctx->comm.render(parm, cha, rett);
        }
        else
        {
            ansi_t  *ptr = cha;
            uint_t  idx, size = 0;

            /* 直接显示内容 */
            for (idx = 0; idx < rett - 1; idx++) {
                ptr[size++] = cha[idx];
                if (cha[idx] == CR_AC('\r') && cha[idx + 1] == CR_AC('\n'))
                    idx++;
            }
            if (idx == rett - 1)
                ptr[size++] = cha[idx];
            qst_direct_show(parm, cha, size);
        }
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
