
#include "QstComm.h"

/*
=======================================
    RS232 接收线程
=======================================
*/
CR_API uint_t STDCALL
qst_rs232_main (
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
        if (sio_read(ctx->comm.obj.port, cha, 1) != 1) {
            thread_sleep(20);
            continue;
        }

        /* 渲染读到的内容 */
        _ENTER_COM_SINGLE_
        if (ctx->comm.text) {
            if (cha[0] != CR_AC('\n') || cha[1] != CR_AC('\r'))
                ctx->comm.render(parm, cha[0]);
            cha[1] = cha[0];
        }
        else {
            ctx->comm.render(parm, cha[0]);
        }
        _LEAVE_COM_SINGLE_
    }

    /* 退出时关闭串口 */
    sio_close(ctx->comm.obj.port);
    return (QST_OKAY);
}

/*
=======================================
    RS232 数据发送
=======================================
*/
CR_API void_t
qst_rs232_send (
  __CR_IN__ void_t*         obj,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    sio_write((uint_t)obj, data, size);
}
