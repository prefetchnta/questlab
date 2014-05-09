
#include "QstCommInt.h"

/*
=======================================
    文本数据显示
=======================================
*/
CR_API void_t
qst_txt_show (
  __CR_IN__ void_t*         parm,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    ansi_t  cha;
    ansi_t* show;

    show = str_allocA(size + 1);
    if (show == NULL)
        return;

    sQstComm*   ctx = (sQstComm*)parm;
    CTextOper*  opr = (CTextOper*)ctx->oper;

    /* 过滤无法显示的字符 */
    for (uint_t idx = 0; idx < size; idx++) {
        cha = *(ansi_t*)data;
        data = (ansi_t*)data + 1;
        if (cha != CR_AC('\n') &&
            cha != CR_AC('\r') && !is_printA(cha))
            cha = CR_AC(' ');
        show[idx] = cha;
    }
    show[size] = NIL;
    opr->text(show);
    mem_free(show);
}

/*
=======================================
    16进制数据显示
=======================================
*/
CR_API void_t
qst_hex_show (
  __CR_IN__ void_t*         parm,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    ansi_t* ptr;
    ansi_t* show;

    show = str_allocA(size * 3 + 1);
    if (show == NULL)
        return;
    ptr = show;

    sQstComm*   ctx = (sQstComm*)parm;
    CTextOper*  opr = (CTextOper*)ctx->oper;

    /* 转换成16进制数显示 */
    for (uint_t idx = 0; idx < size; idx++) {
        sprintf(ptr, "%02X ", *(byte_t*)data);
        data = (byte_t*)data + 1;
        ptr += 3;
    }
    *ptr = NIL;
    opr->text(show);
    mem_free(show);
}
