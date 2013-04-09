
#include "../QstView2D/QstView2D.h"

/* 有用的函数类型 */
typedef iFONT*  (*create_font_t) (const LOGFONTA*);
typedef iGFX2*  (*create_surf_t) (uint_t, uint_t, uint_t,
                                int32u, const int32u*, uint_t);
typedef void_t  (*flldraw_t) (const sIMAGE*, const sFILL*,
                              cpix_t, const sRECT*);
/* 全局绘制参数 */
static iFONT*       s_font;     /* 文字绘制 */
static int32u       s_mode;     /* 绘制模式 */
static sint_t       s_posx;     /* 当前坐标 */
static sint_t       s_posy;     /* 当前坐标 */
static cpix_t       s_color;    /* 绘制颜色 */
static cpix_t       s_bkcolor;  /* 背景颜色 */
static pixdraw_t    s_pixdraw;  /* 绘制模式 */
static flldraw_t    s_flldraw;  /* 填充模式 */

/* 用到的 GFX2_GDI.dll 里的函数 */
static create_font_t    s_create_gdi_fontA;
static create_surf_t    s_create_gdi_bitmap;

/*
---------------------------------------
    是否为 GDI 文字输出对象
---------------------------------------
*/
inline bool_t
is_gdi_text_out (void_t)
{
    if (CR_VCALL(s_font)->getMore(s_font, "iFONT::GDI") != NULL)
        return (TRUE);
    return (FALSE);
}

/*
---------------------------------------
    初始化参数
---------------------------------------
*/
static bool_t
qst_crh_init (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(parm);
    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 设为默认值 */
    s_font = NULL;
    s_mode = CR_BLT_SET;
    s_posx = s_posy = 0;
    s_pixdraw = pixel_set32z;
    s_flldraw = fill_set32_c;
    s_color.val = 0xFF000000;
    s_bkcolor.val = 0xFFFFFFFF;

    sbin_t  sbin;

    /* 两个用到的外部函数 */
    sbin = sbin_testA("GFX2_GDI.dll");
    if (sbin == NULL)
        sbin = sbin_loadA("GFX2_GDI.dll");
    if (sbin == NULL) {
        s_create_gdi_fontA = NULL;
        s_create_gdi_bitmap = NULL;
    }
    else {
        s_create_gdi_fontA = sbin_exportT(sbin,
                        "create_gdi_fontA", create_font_t);
        s_create_gdi_bitmap = sbin_exportT(sbin,
                        "create_gdi_bitmap", create_surf_t);
    }
    return (TRUE);
}

/*
---------------------------------------
    设置颜色
---------------------------------------
*/
static bool_t
qst_crh_color (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(parm);

    /* 参数解析 <Red> <Green> <Blue> [Alpha] */
    if (argc < 4)
        return (FALSE);
    s_color.c32.rrr = (byte_t)str2intxA(argv[1]);
    s_color.c32.ggg = (byte_t)str2intxA(argv[2]);
    s_color.c32.bbb = (byte_t)str2intxA(argv[3]);
    if (argc > 4)
        s_color.c32.lrp = (byte_t)str2intxA(argv[4]);
    else
        s_color.c32.lrp = 255;
    if (s_font == NULL)
        return (TRUE);
    return (CR_VCALL(s_font)->setColor(s_font, s_color.val));
}

/*
---------------------------------------
    设置模式
---------------------------------------
*/
static bool_t
qst_crh_mode (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(parm);

    /* 参数解析 <Mode> */
    if (argc < 2)
        return (FALSE);
    if (str_cmpIA(argv[1], "set") == 0) {
        s_mode = CR_BLT_SET;
        s_pixdraw = pixel_set32z;
        s_flldraw = fill_set32_c;
    }
    else
    if (str_cmpIA(argv[1], "lrp") == 0) {
        s_mode = CR_BLT_ALP;
        s_pixdraw = pixel_lrp32z;
        s_flldraw = fill_lrp32_c;
    }
    else
    if (str_cmpIA(argv[1], "add") == 0) {
        s_mode = CR_BLT_ADD;
        s_pixdraw = pixel_add32z;
        s_flldraw = fill_add32_c;
    }
    else
    if (str_cmpIA(argv[1], "sub") == 0) {
        s_mode = CR_BLT_SUB;
        s_pixdraw = pixel_sub32z;
        s_flldraw = fill_sub32_c;
    }
    else
    if (str_cmpIA(argv[1], "xor") == 0) {
        s_mode = CR_BLT_XOR;
        s_pixdraw = pixel_xor32z;
        s_flldraw = fill_xor32_c;
    }
    else
    if (str_cmpIA(argv[1], "and") == 0) {
        s_mode = CR_BLT_AND;
        s_pixdraw = pixel_and32z;
        s_flldraw = fill_and32_c;
    }
    else
    if (str_cmpIA(argv[1], "orr") == 0) {
        s_mode = CR_BLT_ORR;
        s_pixdraw = pixel_orr32z;
        s_flldraw = fill_orr32_c;
    }
    else
    if (str_cmpIA(argv[1], "not") == 0) {
        s_mode = CR_BLT_NOT;
        s_pixdraw = pixel_not32z;
        s_flldraw = fill_not32_c;
    }
    else {
        return (FALSE);
    }
    if (s_font == NULL)
        return (TRUE);
    if (is_gdi_text_out())
        return (TRUE);
    return (CR_VCALL(s_font)->setMode(s_font, s_mode));
}

/*
---------------------------------------
    移动坐标
---------------------------------------
*/
static bool_t
qst_crh_moveto (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(parm);

    /* 参数解析 <X> <Y> */
    if (argc < 3)
        return (FALSE);
    s_posx = (sint_t)str2intxA(argv[1]);
    s_posy = (sint_t)str2intxA(argv[2]);
    return (TRUE);
}

/*
---------------------------------------
    移动坐标 (绘制)
---------------------------------------
*/
static bool_t
qst_crh_lineto (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sLINE   line;
    sint_t  newx;
    sint_t  newy;
    sIMAGE* draw;

    /* 参数解析 <X> <Y> */
    if (argc < 3)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    newx = (sint_t)str2intxA(argv[1]);
    newy = (sint_t)str2intxA(argv[2]);
    rect_set_xy(&line, s_posx, s_posy, newx, newy);
    draw_line(draw, &line, s_color, s_pixdraw);
    s_posx = newx;
    s_posy = newy;
    return (TRUE);
}

/*
---------------------------------------
    绘制像素
---------------------------------------
*/
static bool_t
qst_crh_pixel (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  posx;
    sint_t  posy;
    sIMAGE* draw;

    /* 参数解析 <X> <Y> */
    if (argc < 3)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    posx = (sint_t)str2intxA(argv[1]);
    posy = (sint_t)str2intxA(argv[2]);
    if (!clip_pixel(posx, posy, &draw->clip_win))
        return (TRUE);
    s_pixdraw(draw, posx, posy, s_color);
    return (TRUE);
}

/*
---------------------------------------
    绘制线段
---------------------------------------
*/
static bool_t
qst_crh_line (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sLINE   line;
    sIMAGE* draw;
    sint_t  sx, sy;
    sint_t  dx, dy;

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    rect_set_xy(&line, sx, sy, dx, dy);
    draw_line(draw, &line, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制方框 (坐标)
---------------------------------------
*/
static bool_t
qst_crh_rect_xy (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT   rect;
    sIMAGE* draw;
    sint_t  sx, sy;
    sint_t  dx, dy;

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    rect_set_xy(&rect, sx, sy, dx, dy);
    draw_rect(draw, &rect, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制方框 (宽高)
---------------------------------------
*/
static bool_t
qst_crh_rect_wh (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT   rect;
    sIMAGE* draw;
    sint_t  sx, sy;
    uint_t  ww, hh;

    /* 参数解析 <X> <Y> <Width> <Height> */
    if (argc < 5)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    ww = (uint_t)str2intxA(argv[3]);
    hh = (uint_t)str2intxA(argv[4]);
    rect_set_wh(&rect, sx, sy, ww, hh);
    draw_rect(draw, &rect, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制正圆
---------------------------------------
*/
static bool_t
qst_crh_circle (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sIMAGE* draw;
    sint_t  cx, cy, rr;

    /* 参数解析 <X> <Y> <Radius> */
    if (argc < 4)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    cx = (sint_t)str2intxA(argv[1]);
    cy = (sint_t)str2intxA(argv[2]);
    rr = (sint_t)str2intxA(argv[3]);
    draw_circle(draw, cx, cy, rr, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制椭圆 (坐标)
---------------------------------------
*/
static bool_t
qst_crh_ellps_xy (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT   rect;
    sIMAGE* draw;
    sint_t  sx, sy;
    sint_t  dx, dy;

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    rect_set_xy(&rect, sx, sy, dx, dy);
    draw_ellipse(draw, &rect, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制椭圆 (宽高)
---------------------------------------
*/
static bool_t
qst_crh_ellps_wh (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT   rect;
    sIMAGE* draw;
    sint_t  sx, sy;
    uint_t  ww, hh;

    /* 参数解析 <X> <Y> <Width> <Height> */
    if (argc < 5)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    ww = (uint_t)str2intxA(argv[3]);
    hh = (uint_t)str2intxA(argv[4]);
    rect_set_wh(&rect, sx, sy, ww, hh);
    draw_ellipse(draw, &rect, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制填充 (坐标)
---------------------------------------
*/
static bool_t
qst_crh_fill_xy (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sFILL   fill;
    sRECT   rect;
    sIMAGE* draw;
    sint_t  sx, sy;
    sint_t  dx, dy;

    /* 参数解析 <X1> <Y1> <X2> <Y2> */
    if (argc < 5)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);
    dx = (sint_t)str2intxA(argv[3]);
    dy = (sint_t)str2intxA(argv[4]);
    rect_set_xy(&rect, sx, sy, dx, dy);
    fill.dx = rect.x1;
    fill.dy = rect.y1;
    fill.dw = rect.ww;
    fill.dh = rect.hh;
    s_flldraw(draw, &fill, s_color, NULL);
    return (TRUE);
}

/*
---------------------------------------
    绘制填充 (宽高)
---------------------------------------
*/
static bool_t
qst_crh_fill_wh (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sFILL   fill;
    sIMAGE* draw;

    /* 参数解析 <X> <Y> <Width> <Height> */
    if (argc < 5)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);
    fill.dx = (sint_t)str2intxA(argv[1]);
    fill.dy = (sint_t)str2intxA(argv[2]);
    fill.dw = (uint_t)str2intxA(argv[3]);
    fill.dh = (uint_t)str2intxA(argv[4]);
    s_flldraw(draw, &fill, s_color, NULL);
    return (TRUE);
}

/*
---------------------------------------
    设置背景颜色
---------------------------------------
*/
static bool_t
qst_crh_bkcolor (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(parm);

    /* 参数解析 <Red> <Green> <Blue> [Alpha] */
    if (argc < 4)
        return (FALSE);
    s_bkcolor.c32.rrr = (byte_t)str2intxA(argv[1]);
    s_bkcolor.c32.ggg = (byte_t)str2intxA(argv[2]);
    s_bkcolor.c32.bbb = (byte_t)str2intxA(argv[3]);
    if (argc > 4)
        s_bkcolor.c32.lrp = (byte_t)str2intxA(argv[4]);
    else
        s_bkcolor.c32.lrp = 255;
    if (s_font == NULL)
        return (TRUE);
    return (CR_VCALL(s_font)->setBkColor(s_font, s_bkcolor.val));
}

/*
---------------------------------------
    创建文字绘制对象 (UCDOS 点阵)
---------------------------------------
*/
static bool_t
qst_crh_ucfont (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    iDATIN* asc;
    iDATIN* hzk;
    iDATIN* chr;
    ansi_t  name[MAX_PATHA];
    uint_t  size, xspc, yspc;

    CR_NOUSE(parm);

    /* 参数解析 <Size> <XSpace> <YSpace> [Suffix] */
    if (argc < 4)
        return (FALSE);

    /* 加载点阵字库文件 */
    size = str2intxA(argv[1]);
    if (argc == 4) {
        sprintf(name, QST_PATH_SOURCE "ucfont\\HZK%u", size);
    }
    else {
        sprintf(name, QST_PATH_SOURCE "ucfont\\HZK%u%c", size,
                      (argv[4])[0]);
    }
    hzk = create_disk_inA(name);
    if (hzk == NULL)
        return (FALSE);
    sprintf(name, QST_PATH_SOURCE "ucfont\\ASC%u", size);
    asc = create_disk_inA(name);
    sprintf(name, QST_PATH_SOURCE "ucfont\\HZK%uT", size);
    chr = create_disk_inA(name);

    /* 创建 UCDOS 点阵汉字绘制对象 */
    xspc = str2intxA(argv[2]);
    yspc = str2intxA(argv[3]);
    if (s_font != NULL)
        CR_VCALL(s_font)->release(s_font);
    s_font = create_ucdos_font(asc, hzk, chr, size, xspc, yspc);
    if (s_font == NULL) {
        CR_VCALL(hzk)->release(hzk);
        if (asc != NULL)
            CR_VCALL(asc)->release(asc);
        if (chr != NULL)
            CR_VCALL(chr)->release(chr);
        return (FALSE);
    }
    return (TRUE);
}

/*
---------------------------------------
    创建文字绘制对象 (自定义点阵)
---------------------------------------
*/
static bool_t
qst_crh_btfont (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    iDATIN* asc;
    iDATIN* hzk;
    ansi_t  name[MAX_PATHA];
    uint_t  size, xspc, yspc;

    CR_NOUSE(parm);

    /* 参数解析 <Size> <XSpace> <YSpace> <Type> */
    if (argc < 5)
        return (FALSE);

    /* 加载点阵字库文件 */
    size = str2intxA(argv[1]);
    sprintf(name, QST_PATH_SOURCE "btfont\\%s_%u.full", argv[4], size);
    hzk = create_disk_inA(name);
    if (hzk == NULL)
        return (FALSE);
    sprintf(name, QST_PATH_SOURCE "btfont\\%s_%u.half", argv[4], size);
    asc = create_disk_inA(name);
    if (asc == NULL) {
        CR_VCALL(hzk)->release(hzk);
        return (FALSE);
    }

    /* 创建自定义点阵文字绘制对象 */
    xspc = str2intxA(argv[2]);
    yspc = str2intxA(argv[3]);
    if (s_font != NULL)
        CR_VCALL(s_font)->release(s_font);
    s_font = create_bit_font(asc, hzk, size, xspc, yspc, argv[4]);
    if (s_font == NULL) {
        CR_VCALL(hzk)->release(hzk);
        CR_VCALL(asc)->release(asc);
        return (FALSE);
    }
    return (TRUE);
}

/* 文字对齐方式 */
#if     (FALSE)
    #define EGUI_ALN_L   1  /* 水平左对齐 */
    #define EGUI_ALN_R   2  /* 水平右对齐 */
    #define EGUI_ALN_C   3  /* 水平中对齐 */
    #define EGUI_ALN_T   4  /* 垂直左对齐 */
    #define EGUI_ALN_B   8  /* 垂直右对齐 */
    #define EGUI_ALN_M  12  /* 垂直中对齐 */
    #define EGUI_ALN_LT (EGUI_ALN_L | EGUI_ALN_T)
    #define EGUI_ALN_LB (EGUI_ALN_L | EGUI_ALN_B)
    #define EGUI_ALN_LM (EGUI_ALN_L | EGUI_ALN_M)
    #define EGUI_ALN_RT (EGUI_ALN_R | EGUI_ALN_T)
    #define EGUI_ALN_RB (EGUI_ALN_R | EGUI_ALN_B)
    #define EGUI_ALN_RM (EGUI_ALN_R | EGUI_ALN_M)
    #define EGUI_ALN_CT (EGUI_ALN_C | EGUI_ALN_T)
    #define EGUI_ALN_CB (EGUI_ALN_C | EGUI_ALN_B)
    #define EGUI_ALN_CM (EGUI_ALN_C | EGUI_ALN_M)
#endif

/*
---------------------------------------
    绘制文字 (实体/透明)
---------------------------------------
*/
static bool_t
qst_crh_text (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT   pos;
    ansi_t* str;
    ansi_t* txt;
    bool_t  rett;
    bool_t  tran;
    iGFX2*  gfx2;
    sIMAGE* draw;
    sint_t  sx, sy;
    uint_t  ww, hh, an;

    /* 参数解析 <X> <Y> <EscText> [Width Height Align] */
    if (argc < 4)
        return (FALSE);
    if (s_font == NULL)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);

    /* 必须使用转义字符串 */
    str = str_fmtA("\"%s\"", argv[3]);
    if (str == NULL)
        return (FALSE);
    txt = str_esc_dupU(str);
    mem_free(str);
    if (txt == NULL)
        return (FALSE);

    /* 区分实体和透明, 两个命令合并起来了 */
    if (str_cmpA(argv[0], "crh:texts") == 0)
        tran = FALSE;
    else
        tran = TRUE;
    rett = FALSE;
    sx = (sint_t)str2intxA(argv[1]);
    sy = (sint_t)str2intxA(argv[2]);

    /* GDI 文字绘制对象要特殊处理 */
    if (is_gdi_text_out())
    {
        sBLIT   blit;
        int32u  mode;
        sIMAGE* surf;

        /* 创建一个临时 GDI 表面然后复制输出的结果到目标画布 */
        gfx2 = s_create_gdi_bitmap(draw->position.ww, draw->position.hh,
                                   CR_ARGB8888, 0, NULL, 0);
        if (gfx2 != NULL)
        {
            /* 画布复制到临时 GDI 表面 */
            blit.dx = blit.dy = 0;
            blit.sx = blit.sy = 0;
            blit.sw = draw->position.ww;
            blit.sh = draw->position.hh;
            surf = CR_VCALL(gfx2)->lock(gfx2);
            blit_set32_c(surf, draw, &blit, NULL);
            CR_VCALL(gfx2)->unlock(gfx2);
            mode = tran ? TRANSPARENT : OPAQUE;

            /* 绑定目标表面并输出文字 */
            if (CR_VCALL(s_font)->bind(s_font, gfx2) &&
                CR_VCALL(s_font)->setMode(s_font, mode) &&
                CR_VCALL(s_font)->setColor(s_font, s_color.val) &&
                CR_VCALL(s_font)->setBkColor(s_font, s_bkcolor.val)) {
                CR_VCALL(s_font)->enter(s_font);
                if (argc < 7) {
                    rett = egui_draw_text2(s_font, txt, sx, sy);
                }
                else {
                    ww = str2intxA(argv[4]);
                    hh = str2intxA(argv[5]);
                    an = str2intxA(argv[6]);
                    rect_set_wh(&pos, sx, sy, ww, hh);
                    rett = egui_draw_text(s_font, txt, &pos, an);
                }
                CR_VCALL(s_font)->leave(s_font);

                /* 输出结果回拷到目标画布 */
                if (rett) {
                    surf = CR_VCALL(gfx2)->lock(gfx2);
                    blit_set32_c(draw, surf, &blit, NULL);
                    CR_VCALL(gfx2)->unlock(gfx2);
                }
            }
        }
    }
    else
    {
        sIMAGE  temp;

        /* 随便生成一个内存绘制表面然后替换其后台缓冲结构为画布 */
        gfx2 = create_mem_bitmap(1, 1, CR_ARGB8888, 0, NULL, 0);
        if (gfx2 != NULL)
        {
            /* 替换表面参数结构 */
            struct_cpy(&temp, &gfx2->__back__, sIMAGE);
            struct_cpy(&gfx2->__back__, draw, sIMAGE);

            /* 绑定目标表面并输出文字 */
            if (CR_VCALL(s_font)->bind(s_font, gfx2) &&
                CR_VCALL(s_font)->setMode(s_font, s_mode) &&
                CR_VCALL(s_font)->setColor(s_font, s_color.val) &&
                CR_VCALL(s_font)->setBkColor(s_font, s_bkcolor.val)) {
                if (argc < 7) {
                    if (tran)
                        rett = egui_draw_tran2(s_font, txt, sx, sy);
                    else
                        rett = egui_draw_text2(s_font, txt, sx, sy);
                }
                else {
                    ww = str2intxA(argv[4]);
                    hh = str2intxA(argv[5]);
                    an = str2intxA(argv[6]);
                    rect_set_wh(&pos, sx, sy, ww, hh);
                    if (tran)
                        rett = egui_draw_tran(s_font, txt, &pos, an);
                    else
                        rett = egui_draw_text(s_font, txt, &pos, an);
                }
            }

            /* 换回原来的表面参数 */
            struct_cpy(&gfx2->__back__, &temp, sIMAGE);
        }
    }

    /* 释放临时对象 */
    if (gfx2 != NULL)
        CR_VCALL(gfx2)->release(gfx2);
    mem_free(txt);
    return (rett);
}

/*
---------------------------------------
    延时一段时间 (ms)
---------------------------------------
*/
static bool_t
qst_crh_sleep (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(parm);

    /* 参数解析 <TimeMS> */
    if (argc < 2)
        return (FALSE);
    thread_sleep(str2intxA(argv[1]));
    return (TRUE);
}

/*
---------------------------------------
    创建文字绘制对象 (GDI 字体)
---------------------------------------
*/
static bool_t
qst_crh_winfont (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    leng_t      len;
    LOGFONTA    font;

    CR_NOUSE(parm);

    /* 参数解析 <Height> <FaceName> [Weight] [Width]
                [CharSet] [Quality] [Italic] [Underline]
                [StrikeOut] [Escapement] [Orientation] */
    if (argc < 3)
        return (FALSE);
    if (s_create_gdi_fontA == NULL ||
        s_create_gdi_bitmap == NULL)
        return (FALSE);

    /* 填充字体生成结构 */
    struct_zero(&font, LOGFONTA);
    font.lfCharSet = DEFAULT_CHARSET;
    font.lfQuality = ANTIALIASED_QUALITY;
    font.lfHeight = (LONG)str2intxA(argv[1]);
    if ((len = str_lenA(argv[2])) >= LF_FACESIZE)
        len = LF_FACESIZE - 1;
    chr_cpyA(font.lfFaceName, argv[2], len);
    if (argc > 3)
        font.lfWeight = (LONG)str2intxA(argv[3]);
    if (argc > 4)
        font.lfWidth = (LONG)str2intxA(argv[4]);
    if (argc > 5)
        font.lfCharSet = (BYTE)str2intxA(argv[5]);
    if (argc > 6)
        font.lfQuality = (BYTE)str2intxA(argv[6]);
    if (argc > 7)
        font.lfItalic = (BYTE)str2intxA(argv[7]);
    if (argc > 8)
        font.lfUnderline = (BYTE)str2intxA(argv[8]);
    if (argc > 9)
        font.lfStrikeOut = (BYTE)str2intxA(argv[9]);
    if (argc > 10)
        font.lfEscapement = (LONG)str2intxA(argv[10]);
    if (argc > 11)
        font.lfOrientation = (LONG)str2intxA(argv[11]);

    /* 生成文字输出对象 */
    if (s_font != NULL)
        CR_VCALL(s_font)->release(s_font);
    s_font = s_create_gdi_fontA(&font);
    if (s_font == NULL)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    命令单元导出表
=======================================
*/
CR_API const sQST_CMD   qst_v2d_cmdz[] =
{
    { "crh:init", qst_crh_init },
    { "crh:color", qst_crh_color },
    { "crh:mode", qst_crh_mode },
    { "crh:moveto", qst_crh_moveto },
    { "crh:lineto", qst_crh_lineto },
    { "crh:pixel", qst_crh_pixel },
    { "crh:line", qst_crh_line },
    { "crh:rect_xy", qst_crh_rect_xy },
    { "crh:rect_wh", qst_crh_rect_wh },
    { "crh:circle", qst_crh_circle },
    { "crh:ellps_xy", qst_crh_ellps_xy },
    { "crh:ellps_wh", qst_crh_ellps_wh },
    { "crh:fill_xy", qst_crh_fill_xy },
    { "crh:fill_wh", qst_crh_fill_wh },
    { "crh:bkcolor", qst_crh_bkcolor },
    { "crh:ucfont", qst_crh_ucfont },
    { "crh:btfont", qst_crh_btfont },
    { "crh:texts", qst_crh_text },
    { "crh:textt", qst_crh_text },
    { "crh:sleep", qst_crh_sleep },
    { "crh:winfont", qst_crh_winfont },
    { NULL, NULL },
};
