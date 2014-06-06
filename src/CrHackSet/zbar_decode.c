
#include "../QstLibs/QstLibs.h"

#define inline cr_inline
#include "zbar/zbar.h"

/*
=======================================
    ZBar 识别部分代码
=======================================
*/
CR_API void_t
zbar_do_decode (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const sIMAGE*   gray
    )
{
    uint_t                  ww;
    uint_t                  hh;
    uint_t                  yy;
    size_t                  sz;
    ansi_t*                 val;
    ansi_t*                 str;
    ansi_t*                 tmp;
    byte_t*                 dst;
    byte_t*                 src;
    byte_t*                 dat;
    zbar_image_t*           img;
    zbar_processor_t*       bar;
    zbar_symbol_type_t      typ;
    const zbar_symbol_t*    sym;

    /* 创建对象 */
    bar = zbar_processor_create(1);
    if (bar == NULL)
        return;
    if (zbar_processor_init(bar, NULL, 0))
        goto _func_out1;

    /* 创建图片 */
    img = zbar_image_create();
    if (img == NULL)
        goto _func_out1;
    ww = gray->position.ww;
    hh = gray->position.hh;
    sz = ww * hh;
    zbar_image_set_format(img, *(unsigned long*)"Y800");
    zbar_image_set_size(img, ww, hh);
    dat = (byte_t*)mem_malloc(sz);
    if (dat == NULL)
        goto _func_out2;
    dst = dat;
    src = gray->data;
    for (yy = 0; yy < hh; yy++) {
        mem_cpy(dst, src, ww);
        dst += ww;
        src += gray->bpl;
    }
    zbar_image_set_data(img, dat, sz, NULL);
    zbar_process_image(bar, img);

    /* 输出结果 */
    sym = zbar_image_first_symbol(img);
    for (; sym != NULL; sym = zbar_symbol_next(sym)) {
        typ = zbar_symbol_get_type(sym);
        if (typ == ZBAR_PARTIAL)
            continue;
        sz = zbar_symbol_get_data_length(sym);
        val = str_allocA(sz + 1);
        if (val != NULL) {
            mem_cpy(val, zbar_symbol_get_data(sym), sz);
            val[sz] = NIL;
            str = str_fmtA("%s%s:%s", zbar_get_symbol_name(typ),
                            zbar_get_addon_name(typ), val);
            if (str != NULL) {
                tmp = str_esc_makeU(str);
                mem_free(str);
                if (tmp != NULL) {
                    str = str_fmtA("info::main=\"0> %s\"", tmp);
                    mem_free(tmp);
                    if (str != NULL) {
                        if (netw != NULL)
                            cmd_ini_send(netw, str);
                        mem_free(str);
                    }
                }
            }
            mem_free(val);
        }
    }
    mem_free(dat);
_func_out2:
    zbar_image_destroy(img);
_func_out1:
    zbar_processor_destroy(bar);
    return;
}
