/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-19  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 二维图形头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_GFX2_H__
#define __CR_GFX2_H__

#include "defs.h"

/*****************************************************************************/
/*                                 位图结构                                  */
/*****************************************************************************/

/* 2D点坐标结构 */
typedef struct
{
        sint_t  x, y;

} sPNT2;

/* 方框/画线结构 */
typedef struct
{
        sint_t  x1, y1;
        sint_t  x2, y2;
        uint_t  ww, hh;

} sRECT, sLINE;

/* 设置线段数据 (通过四点) */
CR_API void_t   line_set_xy (sLINE *line, sint_t x1, sint_t y1,
                             sint_t x2, sint_t y2);
/* 设置方框数据 (通过四点) */
CR_API void_t   rect_set_xy (sRECT *rect, sint_t x1, sint_t y1,
                             sint_t x2, sint_t y2);
/* 设置方框数据 (通过宽高) */
CR_API void_t   rect_set_wh (sRECT *rect, sint_t x1, sint_t y1,
                             uint_t ww, uint_t hh);
/* 方框对齐方式 */
#define RCT_ALIGN_LSB   0   /* 极小 */
#define RCT_ALIGN_HSB   1   /* 中间 */
#define RCT_ALIGN_MSB   2   /* 极大 */

/* 矫正方框的方向 */
CR_API void_t   rect_unique (sRECT *rect);

/* 方框水平对齐 */
CR_API void_t   rect_align_x (sRECT *dest, const sRECT *rect,
                              uint_t type);
/* 方框垂直对齐 */
CR_API void_t   rect_align_y (sRECT *dest, const sRECT *rect,
                              uint_t type);

/* 位图结构 */
typedef struct
{
        uchar*  data;       /* 图片的数据指针 */
        leng_t  size;       /* 图片的数据大小 */
        leng_t  bpl;        /* 每行数据字节数 */
        leng_t  bpc;        /* 图片的像素大小 */
        uint_t  fmt;        /* 图片的像素格式 */
        bool_t  gdi;        /* 图片的垂直颠倒 */
        uint_t  align;      /* 每行字节对齐数 */
        sRECT   clip_win;   /* 图片剪裁用方框 */
        sRECT   position;   /* 图片的位置信息 */
        cl32_t  keycolor;   /* 图片的透明颜色 */
        cl32_t  pal[256];   /* 调色板, 32位色 */

} sIMAGE;

/* CrHack 用的像素格式 */
#define CR_DXT1         1       /* D3DFMT_DXT1   */
#define CR_DXT2         2       /* D3DFMT_DXT2   */
#define CR_DXT3         3       /* D3DFMT_DXT3   */
#define CR_DXT4         4       /* D3DFMT_DXT4   */
#define CR_DXT5         5       /* D3DFMT_DXT5   */
#define CR_INDEX1       6       /* D3DFMT_P1 ??? */
#define CR_INDEX4       7       /* D3DFMT_P4 ??? */
#define CR_INDEX8       8       /* D3DFMT_P8     */
#define CR_INDEX2       9       /* ------------- */
#define CR_UNKNOWN      0       /* 未知/外部类型 */
#define CR_ARGB565      16      /* D3DFMT_R5G6B5 */
#define CR_ARGB888      24      /* D3DFMT_R8G8B8 */
#define CR_ARGB4444     12      /* D3DFMT_A4R4G4B4 */
#define CR_ARGBX555     15      /* D3DFMT_X1R5G5B5 */
#define CR_ARGB1555     17      /* D3DFMT_A1R5G5B5 */
#define CR_ARGB8888     32      /* D3DFMT_A8R8G8B8 */

/* CrHack 像素格式判断宏 */
#define isCrTypeDXTC(t)     (((t) >= CR_DXT1) && ((t) <= CR_DXT5))
#define isCrTypeCompr(t)    (((t) < CR_INDEX1) || ((t) > CR_ARGB8888))
#define isCrTypeIndex(t)    (((t) >= CR_INDEX1) && ((t) <= CR_INDEX2))
#define isCrTypePBits(t)    (((t) == CR_INDEX1) || ((t) == CR_INDEX2) || \
                             ((t) == CR_INDEX4))
/* 位图类型格式转换 */
CR_API int32u   image_crh_to_d3d (uint_t crh_fmt);
CR_API uint_t   image_d3d_to_crh (int32u d3d_fmt);

/* 新建位图 */
CR_API sIMAGE*  image_new (sint_t x, sint_t y, uint_t w, uint_t h,
                           uint_t fmt, bool_t gdi CR_DEFAULT(FALSE),
                           uint_t align CR_DEFAULT(4));
/* 填充位图结构 */
CR_API bool_t   image_set (sIMAGE *img, const void_t *pixel, leng_t size,
                           sint_t x, sint_t y, uint_t w, uint_t h,
                           uint_t fmt, bool_t gdi CR_DEFAULT(FALSE),
                           uint_t align CR_DEFAULT(4));
/* 释放位图 */
CR_API void_t   image_del (const sIMAGE *img);

/* 克隆位图 */
CR_API sIMAGE*  image_dup (const sIMAGE *src);

/* 垂直翻转图片 (若需要) */
CR_API void_t   image_fuck_gdi (sIMAGE *img);

/* 位图垂直翻转 */
CR_API void_t   image_flp (const sIMAGE *img, bool_t solid);

/* 计算位图基本信息 */
CR_API bool_t   image_info (sIMAGE *img, uint_t w, uint_t h,
                            uint_t fmt, bool_t gdi, uint_t align);
/* 交换位图的字节顺序 */
CR_API void_t   image_swap (const sIMAGE *img);

/* 根据透明色设置 Alpha 通道 */
CR_API void_t   image_alptran32 (const sIMAGE *img);

/* 计算32位图片预乘 */
CR_API void_t   image_pre_mul32 (const sIMAGE *img);

/* 32位位图垂直翻转 + RB 互换 */
CR_API void_t   image_gdi_flp32 (const sIMAGE *img);

/* 拆分图片透明通道 */
CR_API sIMAGE*  image_get_alpha (const sIMAGE *img);

/* 合并图片透明通道 */
CR_API void_t   image_set_alpha (const sIMAGE *img, const sIMAGE *alp);

/* 分块/线性图片互换 */
CR_API bool_t   img_tile2line (sIMAGE *img, uint_t tile_w, uint_t tile_h);
CR_API bool_t   img_line2tile (sIMAGE *img, uint_t tile_w, uint_t tile_h);

/*****************************************************************************/
/*                                 位图绘制                                  */
/*****************************************************************************/

/* 填充结构 */
typedef struct
{
        sint_t  dx, dy;     /* 填充的坐标 */
        uint_t  dw, dh;     /* 填充的宽高 */
} sFILL;

/* BLT 结构 */
typedef struct
{
        sint_t  dx, dy;     /* BLT 的目标坐标 */
        sint_t  sx, sy;     /* BLT 的原始坐标 */
        uint_t  sw, sh;     /* BLT 的原始宽高 */
} sBLIT;

/* 缩放结构 */
typedef struct
{
        sint_t  dx, dy;     /* 缩放的目标坐标 */
        sint_t  sx, sy;     /* 缩放的原始坐标 */
        uint_t  dw, dh;     /* 缩放的目标宽高 */
        uint_t  sw, sh;     /* 缩放的原始宽高 */
} sZOOM;

/* BLT 计算结构 */
typedef struct
{
        uchar*  addr;       /* 起始位置的地址 */
        dist_t  rest;       /* 到下一行的距离 */
        leng_t  width;      /* 操作宽循环计数 */
        uint_t  height;     /* 操作高循环计数 */

} sBLTer;

/* 根据相关操作填写 BLT 计算结构 */
CR_API void_t   blt_use_fill (sBLTer *blt, const sIMAGE *dst,
                              leng_t bpc, const sFILL *fill);

CR_API void_t   blt_use_blit (sBLTer *dst_blt, sBLTer *src_blt,
                              leng_t dst_bpc, leng_t src_bpc,
                              const sIMAGE *dst, const sIMAGE *src,
                              const sBLIT *blit);

CR_API void_t   blt_use_zoom (sBLTer *dst_blt, sBLTer *src_blt,
                              leng_t dst_bpc, leng_t src_bpc,
                              const sIMAGE *dst, const sIMAGE *src,
                              const sZOOM *zoom);

CR_API bool_t   blt_use_fill_clip (sBLTer *blt, const sIMAGE *dst,
                                   leng_t bpc, const sFILL *fill,
                                   const sRECT *rect);

CR_API bool_t   blt_use_blit_clip (sBLTer *dst_blt, sBLTer *src_blt,
                                   leng_t dst_bpc, leng_t src_bpc,
                                   const sIMAGE *dst, const sIMAGE *src,
                                   const sBLIT *blit, const sRECT *rect);
/* 计算像素地址 */
#define pixel_addr1(img, x, y)  (byte_t*)(((img)->data)+(y)*(img)->bpl+(x)*1)
#define pixel_addr2(img, x, y)  (int16u*)(((img)->data)+(y)*(img)->bpl+(x)*2)
#define pixel_addr3(img, x, y)  (byte_t*)(((img)->data)+(y)*(img)->bpl+(x)*3)
#define pixel_addr4(img, x, y)  (int32u*)(((img)->data)+(y)*(img)->bpl+(x)*4)
#define pixel_addrN(img, x, y)  (void_t*)(((img)->data)+(y)*(img)->bpl+(x)* \
                                                            (img)->bpc)
/* 二维剪裁函数 */
CR_API bool_t   clip_pixel (sint_t x, sint_t y, const sRECT *win);
CR_API bool_t   clip_line (sLINE *dst, const sLINE *src, const sRECT *win);
CR_API bool_t   clip_fill (sFILL *dst, const sFILL *src, const sRECT *win);
CR_API bool_t   clip_blit (sBLIT *dst, const sBLIT *src, const sRECT *win);
CR_API bool_t   clip_rect (sRECT *dst, const sRECT *src, const sRECT *win);

/* 点阵图形处理 */
CR_API void_t*  font1_h2l (void_t *mask, const void_t *data, uint_t width);
CR_API void_t*  font1_l2h (void_t *mask, const void_t *data, uint_t width);
CR_API void_t*  font2_h2l (void_t *mask, const void_t *data, uint_t width);
CR_API void_t*  font2_l2h (void_t *mask, const void_t *data, uint_t width);
CR_API void_t*  font4_h2l (void_t *mask, const void_t *data, uint_t width);
CR_API void_t*  font4_l2h (void_t *mask, const void_t *data, uint_t width);
CR_API void_t*  fontX_h2l (void_t *mask, const void_t *data, uint_t width,
                           byte_t hpart, byte_t lpart);
CR_API void_t*  fontX_l2h (void_t *mask, const void_t *data, uint_t width,
                           byte_t hpart, byte_t lpart);

/*****************************************************************************/
/*                                 绘制接口                                  */
/*****************************************************************************/

struct  _iGFX2;
#define iGFX2   struct _iGFX2

/* 屏幕绘制接口表 */
typedef struct
{
        /* 通用组 */
        void_t  (*release) (iGFX2 *that);
        void_t* (*getMore) (iGFX2 *that, port_t iid);

        /* 改变屏幕大小 */
        bool_t  (*reset) (iGFX2 *that);

        /* 锁住后台缓冲 */
        sIMAGE* (*lock) (iGFX2 *that);

        /* 解锁后台缓冲 */
        void_t  (*unlock) (iGFX2 *that);

        /* 显示后台缓冲 */
        bool_t  (*flip) (iGFX2 *that, bool_t sync);

        /* 清除后台缓冲 */
        bool_t  (*clear) (iGFX2 *that, cl32_t color, int32u param);

        /* 设置后台调色板 */
        bool_t  (*setPal) (iGFX2 *that, uint_t start, uint_t count);

} iGFX2_vtbl;

#undef  iGFX2

/* 屏幕绘制接口类 */
typedef struct  _iGFX2
{
        /* 虚函数表 */
        const iGFX2_vtbl*   __vptr__;

        /* 数据成员 */
        sIMAGE  __back__;   /* 显示屏的后台缓冲 */
} iGFX2;

/* 生成图形绘制接口的函数类型 */
typedef iGFX2*  (*create_gfx2_t) (void_t*, uint_t, uint_t, uint_t,
                                  bool_t, const int32u*, uint_t);

/*****************************************************************************/
/*                                 文字接口                                  */
/*****************************************************************************/

struct  _iFONT;
#define iFONT   struct _iFONT

/* 文字绘制接口表 */
typedef struct
{
        /* 通用组 */
        void_t  (*release) (iFONT *that);
        void_t* (*getMore) (iFONT *that, port_t iid);

        /* 开始批绘制 */
        void_t  (*enter) (iFONT *that);

        /* 结束批绘制 */
        void_t  (*leave) (iFONT *that);

        /* 绑定绘制接口 */
        bool_t  (*bind) (iFONT *that, iGFX2 *gfx2);

        /* 设置绘制模式 (平台相关) */
        bool_t  (*setMode) (iFONT *that, int32u mode);

        /* 设置前景色 (标准32位色) */
        bool_t  (*setColor) (iFONT *that, cl32_t color);

        /* 设置背景色 (标准32位色) */
        bool_t  (*setBkColor) (iFONT *that, cl32_t color);

        /* 绘制文字 (透明) */
        bool_t  (*draw_tran) (iFONT *that, const void_t *text,
                              const sRECT *rect, uint_t cpage);
        /* 绘制文字 (实体) */
        bool_t  (*draw_text) (iFONT *that, const void_t *text,
                              const sRECT *rect, uint_t cpage);
        /* 测量文字输出范围 */
        bool_t  (*calc_rect) (iFONT *that, const void_t *text,
                              sRECT *rect, uint_t cpage);
} iFONT_vtbl;

#undef  iFONT

/* 文字绘制接口类 */
typedef struct  _iFONT
{
        /* 虚函数表 */
        const iFONT_vtbl*   __vptr__;

        /* 数据成员 */
        int32u  __draw_mode__;              /* 模式 (平台相关) */
        int32u  __color__, __bkcolor__;     /* 颜色 (平台相关) */
} iFONT;

#endif  /* !__CR_GFX2_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
