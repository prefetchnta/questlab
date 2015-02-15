/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-27  */
/*     #######          ###    ###      [GFX2]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>> CrHack UCDOS 文字绘制接口实现 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "bltint.h"
#include "memlib.h"
#include "strlib.h"

/*
---------------------------------------
    释放文字绘制接口
---------------------------------------
*/
extern void_t
iFONT_UC_release (
  __CR_IN__ iFONT*  that
    )
{
    iFONT_UC*   real;

    real = (iFONT_UC*)that;
    if (real->m_ascf != NULL)
        CR_VCALL(real->m_ascf)->release(real->m_ascf);
    if (real->m_chrf != NULL)
        CR_VCALL(real->m_chrf)->release(real->m_chrf);
    if (real->m_hzkf != NULL)
        CR_VCALL(real->m_hzkf)->release(real->m_hzkf);
    mem_free(that);
}

/*
---------------------------------------
    获取扩展的文字接口
---------------------------------------
*/
static void_t*
iFONT_UC_getMore (
  __CR_IN__ iFONT*  that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iFONT::UC") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    开始批绘制
---------------------------------------
*/
extern void_t
iFONT_UC_enter (
  __CR_IN__ iFONT*  that
    )
{
    CR_NOUSE(that);
}

/*
---------------------------------------
    结束批绘制
---------------------------------------
*/
extern void_t
iFONT_UC_leave (
  __CR_IN__ iFONT*  that
    )
{
    CR_NOUSE(that);
}

/*
---------------------------------------
    设置绘制模式 (平台相关)
---------------------------------------
*/
extern bool_t
iFONT_UC_setMode (
  __CR_IO__ iFONT*  that,
  __CR_IN__ int32u  mode
    )
{
    pixdraw_t   draw;
    iFONT_UC*   real = (iFONT_UC*)that;

    if (real->m_draw == NULL)
        return (FALSE);

    /* 根据目标表面和渲染模式设置写点函数 */
    draw = pixel_find_draw(real->m_fcrh, real->m_flip, mode);
    if (draw == NULL)
        return (FALSE);
    real->pixel_draw = draw;
    real->font.__draw_mode__ = mode;
    return (TRUE);
}

/*
---------------------------------------
    绑定绘制接口
---------------------------------------
*/
extern bool_t
iFONT_UC_bind (
  __CR_IO__ iFONT*  that,
  __CR_IN__ iGFX2*  gfx2
    )
{
    sIMAGE*     dest;
    pixcnvt_t   cnvt;
    iFONT_UC*   real = (iFONT_UC*)that;

    dest = CR_VCALL(gfx2)->lock(gfx2);
    if (dest == NULL)
        return (FALSE);

    /* 根据目标表面设置颜色转换函数 (不支持低位索引格式) */
    cnvt = pixel_find_cnvt(dest->fmt);
    if (dest->fmt < CR_INDEX8 || cnvt == NULL) {
        CR_VCALL(gfx2)->unlock(gfx2);
        return (FALSE);
    }
    real->m_draw = gfx2;
    real->m_flip = dest->gdi;
    real->m_fcrh = dest->fmt;
    real->pixel_cnvt = cnvt;
    if (dest->fmt == CR_INDEX8)
        mem_cpy(real->m_pal, dest->pal, 1024);
    struct_cpy(&real->m_rect, &dest->clip_win, sRECT);
    CR_VCALL(gfx2)->unlock(gfx2);

    /* 设置初始渲染模式 */
    return (iFONT_UC_setMode(that, CR_BLT_SET));
}

/*
---------------------------------------
    设置前景色 (标准32位色)
---------------------------------------
*/
extern bool_t
iFONT_UC_setColor (
  __CR_IO__ iFONT*  that,
  __CR_IN__ cl32_t  color
    )
{
    cpix_t      cnvt;
    iFONT_UC*   real = (iFONT_UC*)that;

    if (real->m_draw == NULL)
        return (FALSE);
    cnvt = real->pixel_cnvt(real->m_pal, color);
    real->font.__color__ = cnvt.val;
    return (TRUE);
}

/*
---------------------------------------
    设置背景色 (标准32位色)
---------------------------------------
*/
extern bool_t
iFONT_UC_setBkColor (
  __CR_IO__ iFONT*  that,
  __CR_IN__ cl32_t  color
    )
{
    cpix_t      cnvt;
    iFONT_UC*   real = (iFONT_UC*)that;

    if (real->m_draw == NULL)
        return (FALSE);
    cnvt = real->pixel_cnvt(real->m_pal, color);
    real->font.__bkcolor__ = cnvt.val;
    return (TRUE);
}

/*
*********************************************************
**  ASC12 (08x12) [0x20 - 0x7E] <06x12>
**  ASC16 (08x16) [0x00 - 0xFF] <08x16>
**  ASC24 (24x24) [0x20 - 0x7E] <12x24>
**  ASC40 (24x40) [0x20 - 0x7E] <20x40>
**  ASC48 (24x48) [0x20 - 0x7E] <24x48>
**  HZK12 (16x12) [16区以前不独立成字库] <12x12>
**  HZK16 (16x16) [16区以前不独立成字库] <16x16>
**  HZK24 (24x24) [16区以前独立成 HZK24T] <24x24> (打印)
**  HZK40 (40x40) [16区以前独立成 HZK40T] <40x40>
**  HZK48 (48x48) [16区以前独立成 HZK48T] <48x48>
*********************************************************
*/

/*
---------------------------------------
    绘制12点阵文字 (透明 + 实体)
---------------------------------------
*/
#undef  CR_UC_PRINT
#undef  CR_UC_HZK_T
#define CR_UC_ASC_SW        8
#define CR_UC_HZK_SW        16
#define CR_UC_HZK_TW        12
#define CR_UC_HZK_TH        12
#define CR_UC_ASC_START     32
#define FONT_UC_DRAW_TRAN   iFONT_UC_draw_tran12
#define FONT_UC_DRAW_TEXT   iFONT_UC_draw_text12
#define FONT_UC_CALC_RECT   iFONT_UC_calc_rect12

#include "ucfont.inl"

#undef  CR_UC_PRINT
#undef  CR_UC_HZK_T
#undef  CR_UC_ASC_SW
#undef  CR_UC_HZK_SW
#undef  CR_UC_HZK_TW
#undef  CR_UC_HZK_TH
#undef  CR_UC_ASC_START
#undef  FONT_UC_DRAW_TRAN
#undef  FONT_UC_DRAW_TEXT
#undef  FONT_UC_CALC_RECT

/*
---------------------------------------
    绘制16点阵文字 (透明 + 实体)
---------------------------------------
*/
#undef  CR_UC_PRINT
#undef  CR_UC_HZK_T
#define CR_UC_ASC_SW        8
#define CR_UC_HZK_SW        16
#define CR_UC_HZK_TW        16
#define CR_UC_HZK_TH        16
#define CR_UC_ASC_START     0
#define FONT_UC_DRAW_TRAN   iFONT_UC_draw_tran16
#define FONT_UC_DRAW_TEXT   iFONT_UC_draw_text16
#define FONT_UC_CALC_RECT   iFONT_UC_calc_rect16

#include "ucfont.inl"

#undef  CR_UC_PRINT
#undef  CR_UC_HZK_T
#undef  CR_UC_ASC_SW
#undef  CR_UC_HZK_SW
#undef  CR_UC_HZK_TW
#undef  CR_UC_HZK_TH
#undef  CR_UC_ASC_START
#undef  FONT_UC_DRAW_TRAN
#undef  FONT_UC_DRAW_TEXT
#undef  FONT_UC_CALC_RECT

/*
---------------------------------------
    绘制24点阵文字 (透明 + 实体)
---------------------------------------
*/
#define CR_UC_PRINT
#define CR_UC_HZK_T
#define CR_UC_ASC_SW        24
#define CR_UC_HZK_SW        24
#define CR_UC_HZK_TW        24
#define CR_UC_HZK_TH        24
#define CR_UC_ASC_START     32
#define FONT_UC_DRAW_TRAN   iFONT_UC_draw_tran24
#define FONT_UC_DRAW_TEXT   iFONT_UC_draw_text24
#define FONT_UC_CALC_RECT   iFONT_UC_calc_rect24

#include "ucfont.inl"

#undef  CR_UC_PRINT
#undef  CR_UC_HZK_T
#undef  CR_UC_ASC_SW
#undef  CR_UC_HZK_SW
#undef  CR_UC_HZK_TW
#undef  CR_UC_HZK_TH
#undef  CR_UC_ASC_START
#undef  FONT_UC_DRAW_TRAN
#undef  FONT_UC_DRAW_TEXT
#undef  FONT_UC_CALC_RECT

/*
---------------------------------------
    绘制40点阵文字 (透明 + 实体)
---------------------------------------
*/
#undef  CR_UC_PRINT
#define CR_UC_HZK_T
#define CR_UC_ASC_SW        24
#define CR_UC_HZK_SW        40
#define CR_UC_HZK_TW        40
#define CR_UC_HZK_TH        40
#define CR_UC_ASC_START     32
#define FONT_UC_DRAW_TRAN   iFONT_UC_draw_tran40
#define FONT_UC_DRAW_TEXT   iFONT_UC_draw_text40
#define FONT_UC_CALC_RECT   iFONT_UC_calc_rect40

#include "ucfont.inl"

#undef  CR_UC_PRINT
#undef  CR_UC_HZK_T
#undef  CR_UC_ASC_SW
#undef  CR_UC_HZK_SW
#undef  CR_UC_HZK_TW
#undef  CR_UC_HZK_TH
#undef  CR_UC_ASC_START
#undef  FONT_UC_DRAW_TRAN
#undef  FONT_UC_DRAW_TEXT
#undef  FONT_UC_CALC_RECT

/*
---------------------------------------
    绘制48点阵文字 (透明 + 实体)
---------------------------------------
*/
#undef  CR_UC_PRINT
#define CR_UC_HZK_T
#define CR_UC_ASC_SW        24
#define CR_UC_HZK_SW        48
#define CR_UC_HZK_TW        48
#define CR_UC_HZK_TH        48
#define CR_UC_ASC_START     32
#define FONT_UC_DRAW_TRAN   iFONT_UC_draw_tran48
#define FONT_UC_DRAW_TEXT   iFONT_UC_draw_text48
#define FONT_UC_CALC_RECT   iFONT_UC_calc_rect48

#include "ucfont.inl"

#undef  CR_UC_PRINT
#undef  CR_UC_HZK_T
#undef  CR_UC_ASC_SW
#undef  CR_UC_HZK_SW
#undef  CR_UC_HZK_TW
#undef  CR_UC_HZK_TH
#undef  CR_UC_ASC_START
#undef  FONT_UC_DRAW_TRAN
#undef  FONT_UC_DRAW_TEXT
#undef  FONT_UC_CALC_RECT

/* 接口虚函数表 */
static const iFONT_vtbl _rom_ s_fnt12_vtbl =
{
    iFONT_UC_release, iFONT_UC_getMore,
    iFONT_UC_enter, iFONT_UC_leave, iFONT_UC_bind,
    iFONT_UC_setMode, iFONT_UC_setColor, iFONT_UC_setBkColor,
    iFONT_UC_draw_tran12, iFONT_UC_draw_text12, iFONT_UC_calc_rect12,
};

static const iFONT_vtbl _rom_ s_fnt16_vtbl =
{
    iFONT_UC_release, iFONT_UC_getMore,
    iFONT_UC_enter, iFONT_UC_leave, iFONT_UC_bind,
    iFONT_UC_setMode, iFONT_UC_setColor, iFONT_UC_setBkColor,
    iFONT_UC_draw_tran16, iFONT_UC_draw_text16, iFONT_UC_calc_rect16,
};

static const iFONT_vtbl _rom_ s_fnt24_vtbl =
{
    iFONT_UC_release, iFONT_UC_getMore,
    iFONT_UC_enter, iFONT_UC_leave, iFONT_UC_bind,
    iFONT_UC_setMode, iFONT_UC_setColor, iFONT_UC_setBkColor,
    iFONT_UC_draw_tran24, iFONT_UC_draw_text24, iFONT_UC_calc_rect24,
};

static const iFONT_vtbl _rom_ s_fnt40_vtbl =
{
    iFONT_UC_release, iFONT_UC_getMore,
    iFONT_UC_enter, iFONT_UC_leave, iFONT_UC_bind,
    iFONT_UC_setMode, iFONT_UC_setColor, iFONT_UC_setBkColor,
    iFONT_UC_draw_tran40, iFONT_UC_draw_text40, iFONT_UC_calc_rect40,
};

static const iFONT_vtbl _rom_ s_fnt48_vtbl =
{
    iFONT_UC_release, iFONT_UC_getMore,
    iFONT_UC_enter, iFONT_UC_leave, iFONT_UC_bind,
    iFONT_UC_setMode, iFONT_UC_setColor, iFONT_UC_setBkColor,
    iFONT_UC_draw_tran48, iFONT_UC_draw_text48, iFONT_UC_calc_rect48,
};

/*
=======================================
    生成 UCDOS 文字绘制接口
=======================================
*/
CR_API iFONT*
create_ucdos_font (
  __CR_IN__ iDATIN* asc,
  __CR_IN__ iDATIN* hzk,
  __CR_IN__ iDATIN* chr,
  __CR_IN__ uint_t  size,
  __CR_IN__ uint_t  xspc,
  __CR_IN__ uint_t  yspc
    )
{
    iFONT_UC*   font;

    font = struct_new(iFONT_UC);
    if (font == NULL)
        return (NULL);
    struct_zero(font, iFONT_UC);

    font->m_ascf = asc;
    font->m_chrf = chr;
    font->m_hzkf = hzk;
    font->m_xspc = xspc;
    font->m_yspc = yspc;
    switch (size)
    {
        case 12: font->font.__vptr__ = &s_fnt12_vtbl; break;
        case 16: font->font.__vptr__ = &s_fnt16_vtbl; break;
        case 24: font->font.__vptr__ = &s_fnt24_vtbl; break;
        case 40: font->font.__vptr__ = &s_fnt40_vtbl; break;
        case 48: font->font.__vptr__ = &s_fnt48_vtbl; break;

        default:
            mem_free(font);
            return (NULL);
    }
    return ((iFONT*)font);
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
