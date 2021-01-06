
#include "QstComm.h"

/*
=======================================
    RS232 接收线程
=======================================
*/
CR_API uint_t STDCALL
qst_rs232_main (
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
        back = sio_read(ctx->comm.obj.port, &cha, 1);
        if (back == 1) {
            if (CR_VCALL(ctx->bufs)->putb_no(ctx->bufs, cha))
                ctx->size += 1;
            if (ctx->size < ctx->sbyt)
                continue;
            back = 0;
        }
        if (back != 0) {
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
        thread_sleep(16);
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
