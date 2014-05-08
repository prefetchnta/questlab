
#include "QstCommInt.h"

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/*
=======================================
    读入配置文件
=======================================
*/
CR_API void_t
qst_load_cfg (
  __CR_OT__ sQCOM_conf* cfgs
    )
{
    sINIu*  ini;
    ansi_t* str;

    /* 加载配置文件 */
    str = file_load_as_strA(QST_PATH_CONFIG WIN_ICONF);
    if (str == NULL)
        goto _load_defs;
    ini = ini_parseU(str);
    mem_free(str);
    if (ini == NULL)
        goto _load_defs;

    /* 读入配置参数 */
    cfgs->color = ini_key_intx32U("qcom::color", 0xFFC0C0C0UL, ini);
    cfgs->bkcolor = ini_key_intx32U("qcom::bkcolor", 0xFF000000UL, ini);
    cfgs->font_size = ini_key_intxU("qcom::font_size", 12, ini);
    cfgs->font_face = ini_key_stringU("qcom::font_face", ini);
    ini_closeU(ini);
    return;

_load_defs:
    cfgs->color = 0xFFC0C0C0UL;
    cfgs->bkcolor = 0xFF000000UL;
    cfgs->font_size = 12;
    cfgs->font_face = NULL;
}

/*
=======================================
    刷新浏览器设置
=======================================
*/
CR_API void_t
qst_set_viewer (
  __CR_IO__ sQstComm*   parm
    )
{
    QTextEdit*  edt;
    sQCOM_conf* cfg;

    cfg = &parm->cfgs;
    edt = (QTextEdit*)(parm->view);

    /* 设置全局字体 (默认 Fixedsys 字体) */
    QFont   font("Fixedsys", 12, QFont::Normal, false);

    if (cfg->font_face != NULL) {
        font.setFamily(cfg->font_face);
        font.setPointSize(cfg->font_size);
    }

    ansi_t  tmp[64];

    /* 设置默认前景和背景颜色
       使用 CSS 不会改变已有 HTML 文字的属性 */
    sprintf(tmp, "background-color: #%06X; color: #%06X;",
                  cfg->bkcolor, cfg->color);
    _ENTER_COM_SINGLE_
    edt->setFont(font);
    edt->setStyleSheet(tmp);
    _LEAVE_COM_SINGLE_
}

/*****************************************************************************/
/*                               公用命令单元                                */
/*****************************************************************************/

/*
---------------------------------------
    退出应用程序
---------------------------------------
*/
static bool_t
qst_com_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstComm*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstComm*)parm;
    ctx->quit = TRUE;
    ((QMainWindow*)(ctx->form))->close();
    return (FALSE);
}

/*
---------------------------------------
    显示窗口位置
---------------------------------------
*/
static bool_t
qst_com_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstComm*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstComm*)parm;
    misc_bring2top(ctx->hwnd, NULL);
    return (TRUE);
}

/*
---------------------------------------
    保存窗口位置
---------------------------------------
*/
static bool_t
qst_com_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    RECT        win;
    sQstComm*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 获取窗口位置 */
    ctx = (sQstComm*)parm;
    if (!GetWindowRect(ctx->hwnd, &win))
        return (FALSE);

    sint_t  x1 = win.left;
    sint_t  y1 = win.top;
    uint_t  ww = win.right - win.left;
    uint_t  hh = win.bottom - win.top;

    /* 保存窗口位置 */
    return (misc_desk_save(WIN_ICONF, x1, y1, ww, hh));
}

/*
---------------------------------------
    加载窗口位置
---------------------------------------
*/
static bool_t
qst_com_win_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sint_t  x1, y1;
    uint_t  ww, hh;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 保存窗口位置 */
    if (!misc_desk_load(WIN_ICONF, &x1, &y1, &ww, &hh))
        return (FALSE);

    sQstComm*   ctx;

    /* 设置窗口位置 */
    ctx = (sQstComm*)parm;
    misc_bring2top(ctx->hwnd, NULL);
    return (SetWindowPos(ctx->hwnd, HWND_TOP, (int)x1, (int)y1,
                 (int)ww, (int)hh, SWP_SHOWWINDOW));
}

/*
---------------------------------------
    加载配置文件
---------------------------------------
*/
static bool_t
qst_com_cfg_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstComm*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstComm*)parm;
    qst_load_cfg(&ctx->cfgs);
    qst_set_viewer(ctx);
    return (TRUE);
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit", qst_com_app_exit },
    { "cfg:load", qst_com_cfg_load },
    { "win:load", qst_com_win_load },
    { "win:save", qst_com_win_save },
    { "win:show", qst_com_win_show },

    /***** 私有命令映射 *****/
    { "qcom:app:exit", qst_com_app_exit },
    { "qcom:win:show", qst_com_win_show },
};

/*
=======================================
    工作线程
=======================================
*/
CR_API uint_t STDCALL
qst_com_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    sQstComm*   ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_com_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    ctx = (sQstComm*)param;

    /* 工作循环 */
    while (!ctx->quit)
    {
        ansi_t* string;

        /* 接收一条命令 */
        /* 即使是出错也要继续运行 */
        string = netw_cmd_recv(ctx->netw);
        if (string == NULL) {
            thread_sleep(1);
            continue;
        }

        /* 执行这条命令 */
        cmd_exec_main(obj, ctx, string);
        mem_free(string);
    }
    cmd_exec_free(obj);
    return (QST_OKAY);
}
