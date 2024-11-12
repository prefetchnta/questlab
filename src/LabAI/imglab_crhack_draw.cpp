/*
 ================================================
 *      ######
 *    #########                            ##
 *   ####    ###                           ##
 *   ##       ##                           ##
 *  ##         ## ##   ##   ####     ### ######
 *  ##         ## ##   ##  ######  ##### ######
 *  ##         ## ##   ## ###  ## ###      ##
 *  ##    #    ## ##   ## ##  ##  #####    ##
 *   ##   ##   ## ##   ## ## ##    #####   ##
 *   ###   ## ##  ##   ## ###   #     ##   ##
 *    ##########  ####### ####### ######   ##
 *      #### ##    ######   ####   ####    ##
 *            ##
 *             ##       CREATE: 2024-11-11
 *              #
 ================================================
        图像 LAB CrHack 绘制函数库
 ================================================
 */

#include "imglab_int.hpp"
#include "gfx2/asc6x12.h"
#include "gfx2/asc8x16.h"
#include "gfx2/hzk12x12.h"
#include "gfx2/hzk16x16.h"

/*
=======================================
    绘制 GB2312 点阵字符串
=======================================
*/
CR_API void_t
imglab_draw_gb2312 (
  __CR_IN__ ximage_t        mat,
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ sint_t          dx,
  __CR_IN__ sint_t          dy,
  __CR_IN__ uint_t          height,
  __CR_IN__ int32u          mode,
  __CR_IN__ cpix_t          color,
  __CR_IN__ cpix_t          bkcolor
    )
{
    sRECT   rct;
    iFONT*  fnt;
    iGFX2*  gfx;
    ansi_t* str;
    iDATIN* asc;
    iDATIN* hzk;

    // 创建点阵字体绘制对象
    if (height == 12) {
        asc = create_buff_in(g_asc6x12, sizeof(g_asc6x12), FALSE);
        if (asc == NULL)
            return;
        hzk = create_buff_in(g_hzk12x12, sizeof(g_hzk12x12), FALSE);
        if (hzk == NULL) {
            CR_VCALL(asc)->release(asc);
            return;
        }
    }
    else
    if (height == 16) {
        asc = create_buff_in(g_asc8x16, sizeof(g_asc8x16), FALSE);
        if (asc == NULL)
            return;
        hzk = create_buff_in(g_hzk16x16, sizeof(g_hzk16x16), FALSE);
        if (hzk == NULL) {
            CR_VCALL(asc)->release(asc);
            return;
        }
    }
    else {
        return;
    }
    fnt = create_bit_font(asc, hzk, height, 0, 0, "GB2312");
    if (fnt == NULL) {
        CR_VCALL(hzk)->release(hzk);
        CR_VCALL(asc)->release(asc);
        return;
    }

    // 绑定字体对象
    gfx = imglab_mat_to_igfx2(mat);
    if (gfx == NULL)
        goto _func_out1;
    if (!CR_VCALL(fnt)->bind(fnt, gfx) ||
        !CR_VCALL(fnt)->setMode(fnt, mode) ||
        !CR_VCALL(fnt)->setColor(fnt, color.val))
        goto _func_out2;

    // 输入为 UTF-8 编码字符串
    str = utf8_to_local(CR_GBK, text);
    if (str == NULL)
        goto _func_out2;

    // 绘制点阵字库
    rct.x1 = dx; rct.y1 = dy;
    CR_VCALL(fnt)->calc_rect(fnt, str, &rct, CR_LOCAL);
    if (bkcolor.val != 0) {
        if (!CR_VCALL(fnt)->setBkColor(fnt, bkcolor.val))
            goto _func_out3;
        CR_VCALL(fnt)->draw_text(fnt, str, &rct, CR_LOCAL);
    }
    else {
        CR_VCALL(fnt)->draw_tran(fnt, str, &rct, CR_LOCAL);
    }
_func_out3:
    mem_free(str);
_func_out2:
    mem_free(gfx);
_func_out1:
    CR_VCALL(fnt)->release(fnt);
    return;
}

/*
=======================================
    返回绘制 GB2312 点阵字符串大小
=======================================
*/
CR_API void_t
imglab_calc_gb2312 (
  __CR_OT__ sPNT2*          size,
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ uint_t          height
    )
{
    sRECT   rct;
    iFONT*  fnt;
    iDATIN  asc;
    ansi_t* str;

    size->x = size->y = 0;
    if (height != 12 && height != 16)
        return;
    asc.__size__ = sizeof(g_hzk16x16);
    fnt = create_bit_font(&asc, NULL, height, 0, 0, "GB2312");
    if (fnt == NULL)
        return;
    str = utf8_to_local(CR_GBK, text);
    if (str != NULL) {
        rct.x1 = rct.y1 = 0;
        CR_VCALL(fnt)->calc_rect(fnt, str, &rct, CR_LOCAL);
        size->x = rct.ww;
        size->y = rct.hh;
        mem_free(str);
    }
    mem_free(fnt);
}
