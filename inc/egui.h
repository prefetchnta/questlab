/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-03-21  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2013-04-09  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>>> CrHack eGUI 头文件 <<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_EGUI_H__
#define __CR_EGUI_H__ 0xF2A02CF7UL

#include "fmtz.h"

/*****************************************************************************/
/*                                 公用部分                                  */
/*****************************************************************************/

/* eGUI 对象类型 */
typedef void_t*     egui_t;

/* 建立 eGUI 对象 */
CR_API egui_t   egui_init (leng_t wdg CR_DEFAULT(0),
                           leng_t sty CR_DEFAULT(0),
                           leng_t res CR_DEFAULT(0));
/* 释放 eGUI 对象 */
CR_API void_t   egui_free (egui_t egui);

/* 设置 eGUI 附加参数 */
CR_API void_t   egui_set_param (egui_t egui, void_t *param);

/*****************************************************************************/
/*                                 资源部分                                  */
/*****************************************************************************/

/* 加载 eGUI 资源文件 */
CR_API bool_t   egui_res_load_s (egui_t egui, const ansi_t *text,
                                 uint_t type, engine_init_t port);
CR_API bool_t   egui_res_load_f (egui_t egui, const ansi_t *name,
                                 uint_t type, engine_init_t port);
CR_API bool_t   egui_res_set_root (egui_t egui, const ansi_t *root);

/* 查找 eGUI 资源对象 */
CR_API sFMTZ*       egui_res_get (egui_t egui, const ansi_t *name);
CR_API bool_t       egui_res_set (egui_t egui, const ansi_t *name,
                                  sFMTZ *resz);
CR_API void_t*      egui_res_get_dat (egui_t egui, const ansi_t *name,
                                      leng_t *size);
CR_API sIMAGE*      egui_res_get_img (egui_t egui, const ansi_t *name,
                                      int32u index, sRECT *rect);
CR_API sRECT*       egui_res_get_rct (egui_t egui, const ansi_t *name);
CR_API ansi_t*      egui_res_get_txt (egui_t egui, const ansi_t *name);
CR_API sFMT_PIC*    egui_res_get_anm (egui_t egui, const ansi_t *name);

/*****************************************************************************/
/*                                 风格部分                                  */
/*****************************************************************************/



/*****************************************************************************/
/*                                 控件部分                                  */
/*****************************************************************************/

/* 事件消息类型 */
#define EGUI_EVENT_INIT     0   /* on_init */
#define EGUI_EVENT_KILL     1   /* on_kill */
#define EGUI_EVENT_MOVE     2   /* on_move */
#define EGUI_EVENT_PAINT    3   /* on_paint */
#define EGUI_EVENT_WHEEL    4   /* on_wheel */
#define EGUI_EVENT_KEYUP    5   /* on_keyup */
#define EGUI_EVENT_KEYDN    6   /* on_keydn */
#define EGUI_EVENT_KEYCL    7   /* on_keycl */
#define EGUI_EVENT_KEYDB    8   /* on_keydb */

/* 事件 on_init 消息 */
typedef struct
{
        void_t* xml_node;   /* XML 节点W */

} sEGUI_MSG_INIT;

/* 事件 on_move 消息 */
typedef struct
{
        sint_t  x, y;   /* 指针的绝对坐标 */
        int32u  shift;  /* 附加按下的按钮 */

} sEGUI_MSG_MOVE;

/* 事件 on_keyXX 消息 */
typedef struct
{
        bool_t  type;   /* 是否有坐标信息 */
        sint_t  x, y;   /* 指针的绝对坐标 */
        int32u  shift;  /* 附加按下的按钮 */

} sEGUI_MSG_KEYS;

/* 事件消息结构 */
typedef struct
{
        uint_t  type;   /* 消息类型 */
        sRECT   rect;   /* 父控件的绝对区域 */

        /* 共用一个消息数据结构 */
        union
        {
            sEGUI_MSG_INIT  on_init;
            sEGUI_MSG_MOVE  on_move;
            sEGUI_MSG_KEYS  on_keys;

        } data;

} sEGUI_MSG;

/* 事件回调类型 */
typedef bool_t  (*egui_event_t) (egui_t, void_t*, sEGUI_MSG*);

/* 事件回调结构 */
typedef struct
{
        /* 控件初始化事件 */
        bool_t  (*on_init) (egui_t egui, void_t *sender,
                            sEGUI_MSG *event);
        /* 控件释放事件 */
        bool_t  (*on_kill) (egui_t egui, void_t *sender,
                            sEGUI_MSG *event);
        /* 控件悬停事件 */
        bool_t  (*on_move) (egui_t egui, void_t *sender,
                            sEGUI_MSG *event);
        /* 控件绘制事件 */
        bool_t  (*on_paint) (egui_t egui, void_t *sender,
                             sEGUI_MSG *event);
        /* 鼠标滚轮事件 */
        bool_t  (*on_wheel) (egui_t egui, void_t *sender,
                             sEGUI_MSG *event);
        /* 按钮放开事件 */
        bool_t  (*on_keyup) (egui_t egui, void_t *sender,
                             sEGUI_MSG *event);
        /* 按钮按下事件 */
        bool_t  (*on_keydn) (egui_t egui, void_t *sender,
                             sEGUI_MSG *event);
        /* 按钮按击事件 */
        bool_t  (*on_keycl) (egui_t egui, void_t *sender,
                             sEGUI_MSG *event);
        /* 按钮双击事件 */
        bool_t  (*on_keydb) (egui_t egui, void_t *sender,
                             sEGUI_MSG *event);
} sEGUI_EVT;

/* 事件设置参数 */
typedef struct
{
        const wide_t*   evt_name;   /* 事件名称 */
        egui_event_t    evt_call;   /* 事件调用 */

} sEGUI_EVT_SET;

/* 控件生成参数 */
typedef struct
{
        const wide_t*   ctl_name;   /* 控件 XML 名称 */
        leng_t          ctl_size;   /* 控件字节大小 */
        sEGUI_EVT       evt_egui;   /* 控件系统事件 */

} sEGUI_WDG_SET;

/* eGUI widget */
typedef struct
{
        /* 公用属性 */
        sRECT   r_rect;     /* 控件位置区域 */
        wide_t* s_name;     /* 控件名称 (W) */
        wide_t* s_text;     /* 控件文本数据 */
        void_t* a_tree;     /* 控件关系节点 */
        ansi_t* s_style;    /* 控件风格属性 */
        uint_t  n_group;    /* 控件分组编号 */
        bool_t  b_visible;  /* 控件是否可见 */
        bool_t  b_enabled;  /* 控件是否启用 */
        bool_t  b_focused;  /* 控件是否焦点 */
        bool_t  b_hovered;  /* 控件是否悬停 */

        /* 事件属性 */
        sEGUI_EVT   m_user;     /* 用户事件表 */
        sEGUI_EVT   m_egui;     /* 系统事件表 */

} sEGUI_CTL;

/* 控件对象指针类型 */
typedef sEGUI_CTL*  widge_t;

/* 加载 eGUI 界面文件 */
CR_API bool_t   egui_wdg_load_s (egui_t egui, const ansi_t *text,
                           const sEGUI_WDG_SET *wdg_set, leng_t wdg_num,
                           const sEGUI_EVT_SET *evt_set, leng_t evt_num);
CR_API bool_t   egui_wdg_load_f (egui_t egui, const ansi_t *name,
                           const sEGUI_WDG_SET *wdg_set, leng_t wdg_num,
                           const sEGUI_EVT_SET *evt_set, leng_t evt_num);
/* 查找 eGUI 界面对象 */
CR_API widge_t  egui_wdg_get (egui_t egui, const ansi_t *name);

#define egui_wdg_getT(egui, name, type) \
                ((type*)egui_wdg_get(egui, name))

/* 查找界面对象的父对象 */
CR_API widge_t  egui_wdg_dad (widge_t wdge);

/* 查找界面对象的子对象 */
CR_API widge_t  egui_wdg_son (widge_t wdge, leng_t index);

/* 获取界面对象儿子个数 */
CR_API leng_t   egui_wdg_next (widge_t wdge);

/* 设置界面对象文本属性 */
CR_API wide_t*  egui_wdg_set_text  (widge_t wdge, const wide_t *text);
CR_API ansi_t*  egui_wdg_set_style (widge_t wdge, const ansi_t *style);

/* 分发界面事件消息 */
CR_API void_t   egui_wdg_event (egui_t egui, sEGUI_MSG *event);
CR_API void_t   egui_wdg_event2 (egui_t egui, widge_t wdge, sEGUI_MSG *event);

/*****************************************************************************/
/*                                 绘制布局                                  */
/*****************************************************************************/

/* 元素对齐方式 */
#define EGUI_ALN_L  (1 << 0)    /* 水平左对齐 */
#define EGUI_ALN_R  (2 << 0)    /* 水平右对齐 */
#define EGUI_ALN_C  (3 << 0)    /* 水平中对齐 */
#define EGUI_ALN_T  (1 << 2)    /* 垂直左对齐 */
#define EGUI_ALN_B  (2 << 2)    /* 垂直右对齐 */
#define EGUI_ALN_M  (3 << 2)    /* 垂直中对齐 */
#define EGUI_ALN_LT (EGUI_ALN_L | EGUI_ALN_T)
#define EGUI_ALN_LB (EGUI_ALN_L | EGUI_ALN_B)
#define EGUI_ALN_LM (EGUI_ALN_L | EGUI_ALN_M)
#define EGUI_ALN_RT (EGUI_ALN_R | EGUI_ALN_T)
#define EGUI_ALN_RB (EGUI_ALN_R | EGUI_ALN_B)
#define EGUI_ALN_RM (EGUI_ALN_R | EGUI_ALN_M)
#define EGUI_ALN_CT (EGUI_ALN_C | EGUI_ALN_T)
#define EGUI_ALN_CB (EGUI_ALN_C | EGUI_ALN_B)
#define EGUI_ALN_CM (EGUI_ALN_C | EGUI_ALN_M)

/* 区域的对齐计算 */
CR_API void_t   egui_rect_align (sRECT *dest, const sRECT *rect,
                                 uint_t type);
/* 文字抽象输出 (实体) */
CR_API bool_t   egui_draw_text (iFONT *font, const void_t *text,
                    const sRECT *rect, uint_t type CR_DEFAULT(EGUI_ALN_CM),
                            uint_t cpage CR_DEFAULT(CR_LOCAL));
/* 文字抽象输出 (透明) */
CR_API bool_t   egui_draw_tran (iFONT *font, const void_t *text,
                    const sRECT *rect, uint_t type CR_DEFAULT(EGUI_ALN_CM),
                            uint_t cpage CR_DEFAULT(CR_LOCAL));
/* 文字抽象输出 (实体) */
CR_API bool_t   egui_draw_text2 (iFONT *font, const void_t *text,
                    sint_t x, sint_t y, uint_t cpage CR_DEFAULT(CR_LOCAL));

/* 文字抽象输出 (透明) */
CR_API bool_t   egui_draw_tran2 (iFONT *font, const void_t *text,
                    sint_t x, sint_t y, uint_t cpage CR_DEFAULT(CR_LOCAL));
/* 进度条的方向 */
#define EGUI_PRO_L2R    0   /* 从左到右 (默认) */
#define EGUI_PRO_R2L    1   /* 从右到左 */
#define EGUI_PRO_T2B    2   /* 从上到下 */
#define EGUI_PRO_B2T    3   /* 从下到上 */

/* 进度条区域计算 (整数) */
CR_API void_t   egui_rect_probar_i (sRECT *dest, const sRECT *rect,
                        uint_t type, dist_t vmin, dist_t vmax, dist_t vnow);

/* 进度条区域计算 (浮点) */
CR_API void_t   egui_rect_probar_f (sRECT *dest, const sRECT *rect,
                        uint_t type, real_t vmin, real_t vmax, real_t vnow);

#endif  /* !__CR_EGUI_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
