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
 *             ##       CREATE: 2012-07-14
 *              #
 ================================================
        QuestLAB 二维渲染器
 ================================================
 */

#ifndef __QL_QSTVIEW2D_H__
#define __QL_QSTVIEW2D_H__

#include "gfx2/gdiwin.h"
#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstView2D"
    #define WIN_TITLE   "QstView2D"
    #define WIN_CLASS   "QstView2DCLSS"
    #define WIN_ICONF   "QstView2D.ini"
    #define WIN_XCONF   "QstView2D.xml"
#endif

/* 配置参数结构 */
typedef struct
{
        cl32_t  bkcolor;        /* 背景填充颜色 */
        bool_t  is_blends;      /* 是否开启透明混合 */
        bool_t  is_center;      /* 是否居中显示图片 */
        bool_t  use_keycolor;   /* 是否开启透明关键色 */
        cl32_t  def_keycolor;   /* 自定义的透明关键色 */

} sQV2D_conf;

/* 工作上下文结构 */
typedef struct
{
        /* 图片渲染参数 */
        sFMTZ*      fmtz;   /* 渲染内容的对象 */
        int32u      index;  /* 当前图片的帧号 */
        uint_t      win_x;  /* 图片漫游 X 坐标 */
        uint_t      win_y;  /* 图片漫游 Y 坐标 */
        bool_t      trans;  /* 是否显示透明通道 */
        sIMAGE*     image;  /* 32位色的图片数据 */
        sIMAGE*     alpha;  /* 32位色的透明数据 */
        sIMAGE*     paint;  /* 32位色的绘图画布 (在下面增加不会影响绘图插件) */
        iPICTURE*   slide;  /* 多帧图片接口对象 */
        sFMT_PRT*   pictz;  /* 多帧图片 FMTZ 对象 */

        /* 应用程序参数 */
        HWND        hwnd;   /* 窗口句柄 */
        iGFX2*      draw;   /* 绘制对象 */
        lock_t      lock;   /* 同步的锁 */
        exec_t      objs;   /* 命令对象 */
        bool_t      quit;   /* 是否退出 */
        bool_t      send;   /* 是否发送 */
        sARRAY      extz;   /* 插件列表 */
        socket_t    netw;   /* 网络连接 */
        sQV2D_conf  cfgs;   /* 配置参数 */

        /* 两个鼠标光标 */
        HCURSOR     cur_free;   /* 正常光标 */
        HCURSOR     cur_busy;   /* 等待光标 */

        /* 滤镜相关参数 */
        sXMLu*      flt_scr;    /* 滤镜调用脚本 */
        xmlcaller_t flt_lst;    /* 滤镜接口列表 */

        /* 外部资源加载相关 */
        sbin_t              dll_router;     /* 资源路由插件句柄 */
        const sRES_LOADER*  res_loader;     /* 外部资源加载接口 */
        const sRES_ROUTER*  res_router;     /* 路由资源加载接口 */

} sQstView2D;

/* 鼠标状态设置宏 */
#define QST_SET_CURSOR(hwnd, cursor) \
    SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)(cursor))

/* 自定义窗口消息 */
#define WM_QV2D_SET_WINDOW_TITLE    (WM_USER + 100)
#define WM_QV2D_WIN_BRING_TO_TOP    (WM_USER + 101)

#endif  /* !__QL_QSTVIEW2D_H__ */
