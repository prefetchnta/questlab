
#include "../QstView2D/QstView2D.h"

/* 填充类型 */
typedef void_t  (*flldraw_t) (const sIMAGE*, const sFILL*,
                              cpix_t, const sRECT*);
/* 全局绘制参数 */
static iFONT*       s_font;     /* 文字绘制 */
static iGFX2*       s_gfx2;     /* 绘制对象 */
static int32u       s_mode;     /* 绘制模式 */
static sint_t       s_posx;     /* 当前坐标 */
static sint_t       s_posy;     /* 当前坐标 */
static cpix_t       s_color;    /* 绘制颜色 */
static cpix_t       s_bkcolor;  /* 背景颜色 */
static pixdraw_t    s_pixdraw;  /* 绘制模式 */
static flldraw_t    s_flldraw;  /* 填充模式 */

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

    s_font = NULL;
    s_gfx2 = NULL;
    s_mode = CR_BLT_SET;
    s_posx = s_posy = 0;
    s_pixdraw = pixel_set32z;
    s_flldraw = fill_set32_c;
    s_color.val = 0xFF000000;
    s_bkcolor.val = 0xFFFFFFFF;
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
qst_crh_ellipse_xy (
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
qst_crh_ellipse_wh (
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
    sIMAGE* draw;
    ansi_t  name[MAX_PATHA];
    uint_t  size, xspc, yspc;

    /* 参数解析 <Size> <XSpace> <YSpace> [Suffix] */
    if (argc < 4)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
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

    /* 这里只能直接在内部数据成员上做手脚了
       随便生成一个屏幕绘制对象然后替换其后台缓冲结构 */
    if (s_gfx2 == NULL) {
        s_gfx2 = create_mem_bitmap(1, 1, CR_ARGB8888, 0, NULL, 0);
        if (s_gfx2 == NULL)
            goto _failure;
        mem_free(s_gfx2->__back__.data);
        struct_cpy(&s_gfx2->__back__, draw, sIMAGE);
    }
    if (!CR_VCALL(s_font)->bind(s_font, s_gfx2))
        goto _failure;
    if (!CR_VCALL(s_font)->setMode(s_font, s_mode))
        goto _failure;
    if (!CR_VCALL(s_font)->setColor(s_font, s_color.val))
        goto _failure;
    if (!CR_VCALL(s_font)->setBkColor(s_font, s_bkcolor.val))
        goto _failure;
    return (TRUE);

_failure:
    CR_VCALL(s_font)->release(s_font);
    s_font = NULL;
    return (FALSE);
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
    sIMAGE* draw;
    ansi_t  name[MAX_PATHA];
    uint_t  size, xspc, yspc;

    /* 参数解析 <Size> <XSpace> <YSpace> <Type> */
    if (argc < 5)
        return (FALSE);
    draw = ((sQstView2D*)parm)->paint;
    if (draw == NULL)
        return (FALSE);

    /* 加载点阵字库文件 */
    size = str2intxA(argv[1]);
    sprintf(name, QST_PATH_SOURCE "btfont\\FULL%u", size);
    hzk = create_disk_inA(name);
    if (hzk == NULL)
        return (FALSE);
    sprintf(name, QST_PATH_SOURCE "btfont\\HALF%u", size);
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

    /* 这里只能直接在内部数据成员上做手脚了
       随便生成一个屏幕绘制对象然后替换其后台缓冲结构 */
    if (s_gfx2 == NULL) {
        s_gfx2 = create_mem_bitmap(1, 1, CR_ARGB8888, 0, NULL, 0);
        if (s_gfx2 == NULL)
            goto _failure;
        mem_free(s_gfx2->__back__.data);
        struct_cpy(&s_gfx2->__back__, draw, sIMAGE);
    }
    if (!CR_VCALL(s_font)->bind(s_font, s_gfx2))
        goto _failure;
    if (!CR_VCALL(s_font)->setMode(s_font, s_mode))
        goto _failure;
    if (!CR_VCALL(s_font)->setColor(s_font, s_color.val))
        goto _failure;
    if (!CR_VCALL(s_font)->setBkColor(s_font, s_bkcolor.val))
        goto _failure;
    return (TRUE);

_failure:
    CR_VCALL(s_font)->release(s_font);
    s_font = NULL;
    return (FALSE);
}

/*
---------------------------------------
    绘制文字 (实体)
---------------------------------------
*/
static bool_t
qst_crh_texts (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT   pos;
    ansi_t* str;
    ansi_t* txt;

    CR_NOUSE(parm);

    /* 参数解析 <X> <Y> <EscText> */
    if (argc < 4)
        return (FALSE);
    if (s_font == NULL)
        return (FALSE);

    /* 必须使用转义字符串 */
    str = str_fmtA("\"%s\"", argv[3]);
    if (str == NULL)
        return (FALSE);
    txt = str_esc_dupU(str);
    mem_free(str);
    if (txt == NULL)
        return (FALSE);
    pos.x1 = (sint_t)str2intxA(argv[1]);
    pos.y1 = (sint_t)str2intxA(argv[2]);
    if (!CR_VCALL(s_font)->calc_rect(s_font, txt, &pos, CR_LOCAL)) {
        mem_free(txt);
        return (FALSE);
    }
    if (!CR_VCALL(s_font)->draw_text(s_font, txt, &pos, CR_LOCAL)) {
        mem_free(txt);
        return (FALSE);
    }
    mem_free(txt);
    return (TRUE);
}

/*
---------------------------------------
    绘制文字 (透明)
---------------------------------------
*/
static bool_t
qst_crh_textt (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sRECT   pos;
    ansi_t* str;
    ansi_t* txt;

    CR_NOUSE(parm);

    /* 参数解析 <X> <Y> <EscText> */
    if (argc < 4)
        return (FALSE);
    if (s_font == NULL)
        return (FALSE);

    /* 必须使用转义字符串 */
    str = str_fmtA("\"%s\"", argv[3]);
    if (str == NULL)
        return (FALSE);
    txt = str_esc_dupU(str);
    mem_free(str);
    if (txt == NULL)
        return (FALSE);
    pos.x1 = (sint_t)str2intxA(argv[1]);
    pos.y1 = (sint_t)str2intxA(argv[2]);
    if (!CR_VCALL(s_font)->calc_rect(s_font, txt, &pos, CR_LOCAL)) {
        mem_free(txt);
        return (FALSE);
    }
    if (!CR_VCALL(s_font)->draw_tran(s_font, txt, &pos, CR_LOCAL)) {
        mem_free(txt);
        return (FALSE);
    }
    mem_free(txt);
    return (TRUE);
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
    { "crh:ellipse_xy", qst_crh_ellipse_xy },
    { "crh:ellipse_wh", qst_crh_ellipse_wh },
    { "crh:fill_xy", qst_crh_fill_xy },
    { "crh:fill_wh", qst_crh_fill_wh },
    { "crh:bkcolor", qst_crh_bkcolor },
    { "crh:ucfont", qst_crh_ucfont },
    { "crh:btfont", qst_crh_btfont },
    { "crh:texts", qst_crh_texts },
    { "crh:textt", qst_crh_textt },
    { "crh:sleep", qst_crh_sleep },
    { NULL, NULL },
};
