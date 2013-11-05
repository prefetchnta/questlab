
#include "../QstView2D/QstView2D.h"

/* 有用的函数类型 */
typedef iFONT*  (*create_font_t) (const LOGFONTA*);
typedef iGFX2*  (*create_surf_t) (uint_t, uint_t, uint_t,
                                int32u, const int32u*, uint_t);
typedef void_t  (*flldraw_t) (const sIMAGE*, const sFILL*,
                              cpix_t, const sRECT*);
/* 全局绘制参数 */
static iFONT*       s_font;     /* 文字绘制 */
static egui_t       s_resx;     /* 外部资源 */
static int32u       s_mode;     /* 绘制模式 */
static sint_t       s_posx;     /* 当前坐标 */
static sint_t       s_posy;     /* 当前坐标 */
static cpix_t       s_color;    /* 绘制颜色 */
static cpix_t       s_trans;    /* 透明颜色 */
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
    s_resx = NULL;
    s_mode = CR_BLT_SET;
    s_posx = s_posy = 0;
    s_pixdraw = pixel_set32z;
    s_flldraw = fill_set32_c;
    s_color.val = 0xFF000000;
    s_trans.val = 0x00000000;
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
    line_set_xy(&line, s_posx, s_posy, newx, newy);
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
    line_set_xy(&line, sx, sy, dx, dy);
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
    #define EGUI_ALN_T   4  /* 垂直顶对齐 */
    #define EGUI_ALN_B   8  /* 垂直底对齐 */
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
    绘制文字 (内部实现)
---------------------------------------
*/
static bool_t
qst_crh_text_int (
  __CR_IN__ sIMAGE*         draw,
  __CR_IN__ bool_t          tran,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ uint_t          align
    )
{
    sBLIT   blit;
    iGFX2*  gfx2;
    int32u  mode;
    sIMAGE  temp, *surf;
    bool_t  rett = FALSE;

    /* GDI 文字绘制对象要特殊处理 */
    if (is_gdi_text_out())
    {
        /* 创建一个临时 GDI 表面然后复制输出的结果到目标画布 */
        gfx2 = s_create_gdi_bitmap(draw->position.ww, draw->position.hh,
                                   CR_ARGB8888, 0, NULL, 0);
        if (gfx2 == NULL)
            return (FALSE);

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
            CR_VCALL(s_font)->setBkColor(s_font, s_bkcolor.val))
        {
            CR_VCALL(s_font)->enter(s_font);
            rett = egui_draw_text(s_font, text, rect, align);
            CR_VCALL(s_font)->leave(s_font);

            /* 输出结果回拷到目标画布 */
            if (rett)
            {
                /* 只复制颜色通道 (GDI 会清除 Alpha 通道) */
                surf = CR_VCALL(gfx2)->lock(gfx2);
                image_flp(surf, FALSE);
                for (leng_t idx = 0; idx < draw->size; idx += 4) {
                    draw->data[idx + 0] = surf->data[idx + 0];
                    draw->data[idx + 1] = surf->data[idx + 1];
                    draw->data[idx + 2] = surf->data[idx + 2];
                }
                CR_VCALL(gfx2)->unlock(gfx2);
            }
        }
    }
    else
    {
        /* 随便生成一个内存绘制表面然后替换其后台缓冲结构为画布 */
        gfx2 = create_mem_bitmap(1, 1, CR_ARGB8888, 0, NULL, 0);
        if (gfx2 == NULL)
            return (FALSE);

        /* 替换表面参数结构 */
        struct_cpy(&temp, &gfx2->__back__, sIMAGE);
        struct_cpy(&gfx2->__back__, draw, sIMAGE);

        /* 绑定目标表面并输出文字 */
        if (CR_VCALL(s_font)->bind(s_font, gfx2) &&
            CR_VCALL(s_font)->setMode(s_font, s_mode) &&
            CR_VCALL(s_font)->setColor(s_font, s_color.val) &&
            CR_VCALL(s_font)->setBkColor(s_font, s_bkcolor.val))
        {
            if (tran)
                rett = egui_draw_tran(s_font, text, rect, align);
            else
                rett = egui_draw_text(s_font, text, rect, align);
        }

        /* 换回原来的表面参数 */
        struct_cpy(&gfx2->__back__, &temp, sIMAGE);
    }

    /* 释放临时表面 */
    CR_VCALL(gfx2)->release(gfx2);
    return (rett);
}

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
    sRECT   rect;
    bool_t  rett;
    bool_t  tran;
    ansi_t* temp;
    ansi_t* text;
    sIMAGE* draw;
    uint_t  ww,hh;
    uint_t  align;

    /* 参数解析 <X> <Y> <EscText> [Width Height Align] */
    if (argc < 4)
        return (FALSE);
    if (s_font == NULL)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);

    /* 必须使用转义字符串 */
    temp = str_fmtA("\"%s\"", argv[3]);
    if (temp == NULL)
        return (FALSE);
    text = str_esc_dupU(temp);
    mem_free(temp);
    if (text == NULL)
        return (FALSE);

    /* 区分实体和透明, 两个命令合并起来了 */
    if (str_cmpA(argv[0], "crh:texts") == 0)
        tran = FALSE;
    else
        tran = TRUE;
    rect.x1 = (sint_t)str2intxA(argv[1]);
    rect.y1 = (sint_t)str2intxA(argv[2]);
    if (argc > 6) {
        ww = str2intxA(argv[4]);
        hh = str2intxA(argv[5]);
        align = str2intxA(argv[6]);
        rect_set_wh(&rect, rect.x1, rect.y1, ww, hh);
    }
    else {
        align = 0;
    }
    rett = qst_crh_text_int(draw, tran, &rect, text, align);
    mem_free(text);
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
---------------------------------------
    设置透明颜色
---------------------------------------
*/
static bool_t
qst_crh_trans (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(parm);

    /* 参数解析 <Red> <Green> <Blue> [Alpha] */
    if (argc < 4)
        return (FALSE);
    s_trans.c32.rrr = (byte_t)str2intxA(argv[1]);
    s_trans.c32.ggg = (byte_t)str2intxA(argv[2]);
    s_trans.c32.bbb = (byte_t)str2intxA(argv[3]);
    if (argc > 4)
        s_trans.c32.lrp = (byte_t)str2intxA(argv[4]);
    else
        s_trans.c32.lrp = 255;
    return (TRUE);
}

/*
---------------------------------------
    加载外部资源
---------------------------------------
*/
static bool_t
qst_crh_loadres (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sIMAGE* draw;

    /* 参数解析 <ResFile> [RootDir] */
    if (argc < 2)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);

    /* 初始化资源描述对象 */
    if (s_resx != NULL)
        egui_res_free(s_resx);
    s_resx = egui_res_init(0);
    if (s_resx == NULL)
        return (FALSE);

    /* 指定文件根目录 */
    if (argc > 2) {
        if (!egui_res_set_root(s_resx, argv[2]))
            goto _failure;
    }

    /* 加载外部资源描述文件 */
    if (!egui_res_load_f(s_resx, argv[1], draw->fmt, NULL))
        goto _failure;
    return (TRUE);

_failure:
    egui_res_free(s_resx);
    s_resx = NULL;
    return (FALSE);
}

/*
---------------------------------------
    绘制图块 (内部实现)
---------------------------------------
*/
static void_t
qst_crh_blit_int (
  __CR_IN__ sIMAGE*         draw,
  __CR_IN__ sIMAGE*         srce,
  __CR_IN__ const sBLIT*    blit
    )
{
    switch (s_mode)
    {
        default:
        case CR_BLT_SET:
            if (s_trans.val == 0)
                blit_set32_c(draw, srce, blit, NULL);
            else
                blit_str32_c(draw, srce, blit, s_trans, NULL);
            break;

        case CR_BLT_AND:
            blit_and32_c(draw, srce, blit, NULL);
            break;

        case CR_BLT_ORR:
            blit_orr32_c(draw, srce, blit, NULL);
            break;

        case CR_BLT_NOT:
            if (s_trans.val == 0)
                blit_alp32_c(draw, srce, NULL, blit, NULL);
            else
                blit_dtr32_c(draw, srce, blit, s_trans, NULL);
            break;

        case CR_BLT_XOR:
            blit_xor32_c(draw, srce, blit, NULL);
            break;

        case CR_BLT_ADD:
            blit_add32_c(draw, srce, blit, NULL);
            break;

        case CR_BLT_SUB:
            blit_sub32_c(draw, srce, blit, NULL);
            break;

        case CR_BLT_ALP:
            if (s_trans.val == 0)
                blit_lrp32_c(draw, srce, blit, s_trans, FALSE, NULL);
            else
                blit_lrp32_c(draw, srce, blit, s_trans, TRUE, NULL);
            break;
    }
}

/*
---------------------------------------
    绘制图块 (支持对齐)
---------------------------------------
*/
static bool_t
qst_crh_blit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sBLIT   blit;
    sRECT   rect;
    sRECT   dest;
    sRECT   wins;
    int32u  idxs;
    sIMAGE* draw;
    sIMAGE* srce;
    uint_t  ww,hh;
    uint_t  align;

    /* 参数解析 <X> <Y> <SpriteName> [Index] [Width Height Align] */
    if (argc < 4)
        return (FALSE);
    if (s_resx == NULL)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);

    /* 查找目标图片对象 */
    if (argc > 4)
        idxs = str2intxA(argv[4]);
    else
        idxs = 0;
    srce = egui_res_get_img(s_resx, argv[3], idxs, &rect);
    if (srce == NULL)
        return (FALSE);

    /* 计算对齐输出图块 */
    dest.x1 = (sint_t)str2intxA(argv[1]);
    dest.y1 = (sint_t)str2intxA(argv[2]);
    rect_set_wh(&dest, dest.x1, dest.y1, rect.ww, rect.hh);
    if (argc > 7) {
        ww = str2intxA(argv[5]);
        hh = str2intxA(argv[6]);
        align = str2intxA(argv[7]);
        rect_set_wh(&wins, dest.x1, dest.y1, ww, hh);
        egui_rect_align(&dest, &wins, align);
    }
    blit.dx = dest.x1;
    blit.dy = dest.y1;
    blit.sx = rect.x1;
    blit.sy = rect.y1;
    blit.sw = dest.ww;
    blit.sh = dest.hh;
    qst_crh_blit_int(draw, srce, &blit);
    return (TRUE);
}

/*
---------------------------------------
    绘制图块 (支持对齐)
---------------------------------------
*/
static bool_t
qst_crh_blit2 (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sBLIT   blit;
    sRECT   rect;
    sRECT   dest;
    sRECT*  wins;
    int32u  idxs;
    uint_t  algn;
    sIMAGE* draw;
    sIMAGE* srce;

    /* 参数解析 <SpriteName> <RectName> <Align> [Index] */
    if (argc < 4)
        return (FALSE);
    if (s_resx == NULL)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);

    /* 查找目标矩形对象 */
    wins = egui_res_get_rct(s_resx, argv[2]);
    if (wins == NULL)
        return (FALSE);

    /* 查找目标图片对象 */
    if (argc > 4)
        idxs = str2intxA(argv[4]);
    else
        idxs = 0;
    srce = egui_res_get_img(s_resx, argv[1], idxs, &rect);
    if (srce == NULL)
        return (FALSE);

    /* 计算对齐输出图块 */
    algn = str2intxA(argv[3]);
    rect_set_wh(&dest, wins->x1, wins->y1, rect.ww, rect.hh);
    egui_rect_align(&dest, wins, algn);
    blit.dx = dest.x1;
    blit.dy = dest.y1;
    blit.sx = rect.x1;
    blit.sy = rect.y1;
    blit.sw = dest.ww;
    blit.sh = dest.hh;
    qst_crh_blit_int(draw, srce, &blit);
    return (TRUE);
}

/*
---------------------------------------
    绘制方框 (使用外部定义)
---------------------------------------
*/
static bool_t
qst_crh_rect_ex (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT*  rect;
    sIMAGE* draw;

    /* 参数解析 <RectName> */
    if (argc < 2)
        return (FALSE);
    if (s_resx == NULL)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);

    /* 查找目标矩形对象 */
    rect = egui_res_get_rct(s_resx, argv[1]);
    if (rect == NULL)
        return (FALSE);
    draw_rect(draw, rect, s_color, s_pixdraw);
    return (TRUE);
}

/*
---------------------------------------
    绘制填充 (使用外部定义)
---------------------------------------
*/
static bool_t
qst_crh_fill_ex (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sFILL   fill;
    sRECT*  rect;
    sIMAGE* draw;

    /* 参数解析 <RectName> */
    if (argc < 2)
        return (FALSE);
    if (s_resx == NULL)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);

    /* 查找目标矩形对象 */
    rect = egui_res_get_rct(s_resx, argv[1]);
    if (rect == NULL)
        return (FALSE);
    fill.dx = rect->x1;
    fill.dy = rect->y1;
    fill.dw = rect->ww;
    fill.dh = rect->hh;
    s_flldraw(draw, &fill, s_color, NULL);
    return (TRUE);
}

/*
---------------------------------------
    绘制文字 (使用外部定义)
---------------------------------------
*/
static bool_t
qst_crh_text_ex (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT*  rect;
    bool_t  free;
    bool_t  rett;
    bool_t  tran;
    uint_t  algn;
    ansi_t* temp;
    ansi_t* text;
    sIMAGE* draw;

    /* 参数解析 <EscText/TextName> <RectName> <Align> [IsTextName] */
    if (argc < 4)
        return (FALSE);
    if (s_font == NULL)
        return (FALSE);
    if (s_resx == NULL)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);

    /* 查找目标矩形对象 */
    rect = egui_res_get_rct(s_resx, argv[2]);
    if (rect == NULL)
        return (FALSE);

    /* 找不到或不使用文本名称的时候使用转义字符串 */
    if (argc > 4)
        text = egui_res_get_txt(s_resx, argv[1]);
    else
        text = NULL;
    if (text == NULL) {
        temp = str_fmtA("\"%s\"", argv[1]);
        if (temp == NULL)
            return (FALSE);
        text = str_esc_dupU(temp);
        mem_free(temp);
        if (text == NULL)
            return (FALSE);
        free = TRUE;
    }
    else {
        free = FALSE;
    }

    /* 区分实体和透明, 两个命令合并起来了 */
    if (str_cmpA(argv[0], "crh:texts_ex") == 0)
        tran = FALSE;
    else
        tran = TRUE;
    algn = str2intxA(argv[3]);
    rett = qst_crh_text_int(draw, tran, rect, text, algn);
    if (free)
        mem_free(text);
    return (rett);
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
    { "crh:trans", qst_crh_trans },
    { "crh:loadres", qst_crh_loadres },
    { "crh:blit", qst_crh_blit },
    { "crh:blit2", qst_crh_blit2 },
    { "crh:rect_ex", qst_crh_rect_ex },
    { "crh:fill_ex", qst_crh_fill_ex },
    { "crh:texts_ex", qst_crh_text_ex },
    { "crh:textt_ex", qst_crh_text_ex },
    { NULL, NULL },
};
