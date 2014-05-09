
#include "QstCommInt.h"

/* 接收线程的声明 */
CR_API uint_t STDCALL qst_rs232_main (void_t *param);
CR_API uint_t STDCALL qst_tcpv4_main (void_t *param);
CR_API uint_t STDCALL qst_udpv4_main (void_t *param);

/* 发送函数的声明 */
CR_API void_t   qst_rs232_send (void_t *obj, const void_t *data, uint_t size);
CR_API void_t   qst_tcpv4_send (void_t *obj, const void_t *data, uint_t size);
CR_API void_t   qst_udpv4_send (void_t *obj, const void_t *data, uint_t size);

/* 数据变换的声明 */
CR_API void_t*  qst_dos_tran (const ansi_t *string, uint_t *ot_size);
CR_API void_t*  qst_unx_tran (const ansi_t *string, uint_t *ot_size);
CR_API void_t*  qst_mac_tran (const ansi_t *string, uint_t *ot_size);

/* 数据渲染的声明 */
CR_API void_t   qst_hex_show (void_t *parm, const void_t *data, uint_t size);

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

/*
---------------------------------------
    关闭通讯接口
---------------------------------------
*/
static bool_t
qst_com_close (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstComm*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstComm*)parm;
    if (ctx->comm.thrd == NULL)
        return (FALSE);
    ctx->comm.quit = TRUE;
    thread_wait(ctx->comm.thrd);
    thread_del(ctx->comm.thrd);
    ctx->comm.thrd = NULL;
    ctx->comm.quit = FALSE;
    SetWindowTextA(ctx->hwnd, WIN_TITLE);
    return (TRUE);
}

/*
---------------------------------------
    打开 RS232 通讯接口
---------------------------------------
*/
static bool_t
qst_com_rs232 (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    int32u  baud;
    uint_t  port, bits;
    uint_t  stop, parity;
    /* --------------- */
    const ansi_t*   sstop;
    const ansi_t*   sparity;

    /* 参数解析 <串口号> [波特率] [数据位] [校验位] [停止位] */
    if (argc < 2)
        return (FALSE);
    bits = 8;
    baud = 115200UL;
    stop = CR_SIO_STOP10;
    parity = CR_SIO_NOP;
    sstop = "1";
    sparity = "no";
    port = str2intxA(argv[1]);
    if (argc > 2) {
        baud = str2intx32A(argv[2]);
        if (argc > 3) {
            bits = str2intxA(argv[3]);
            if (argc > 4) {
                sparity = argv[4];
                if (str_cmpA(argv[4], "no") == 0)
                    parity = CR_SIO_NOP;
                else
                if (str_cmpA(argv[4], "odd") == 0)
                    parity = CR_SIO_ODD;
                else
                if (str_cmpA(argv[4], "even") == 0)
                    parity = CR_SIO_EVEN;
                else
                if (str_cmpA(argv[4], "mark") == 0)
                    parity = CR_SIO_MARK;
                else
                if (str_cmpA(argv[4], "space") == 0)
                    parity = CR_SIO_SPCE;
                else
                    sparity = "no";
                if (argc > 5) {
                    sstop = argv[5];
                    if (str_cmpA(argv[5], "1") == 0)
                        stop = CR_SIO_STOP10;
                    else
                    if (str_cmpA(argv[5], "1.5") == 0)
                        stop = CR_SIO_STOP15;
                    else
                    if (str_cmpA(argv[5], "2") == 0)
                        stop = CR_SIO_STOP20;
                    else
                        sstop = "1";
                }
            }
        }
    }

    ansi_t      title[128];
    sQstComm*   ctx = (sQstComm*)parm;

    /* 关闭当前接口并打开串口 */
    if (!sio_open(port))
        return (FALSE);
    sio_setup(port, baud, bits, parity, stop);
    sio_set_buffer(port, 1024, 0);
    sio_set_rd_timeout(port, 0xFFFFFFFF, 0, 0);
    sio_set_wr_timeout(port, 0, 0);
    sio_clear_error(port);
    sio_flush(port, CR_SIO_FLU_RT);

    /* 设置工作参数 */
    qst_com_close(parm, argc, argv);
    ctx->comm.obj.port = port;
    ctx->comm.send = qst_rs232_send;

    /* 启动接收线程 */
    ctx->comm.thrd = thread_new(0, qst_rs232_main, parm, FALSE);
    if (ctx->comm.thrd == NULL) {
        sio_close(port);
        return (FALSE);
    }
    sprintf(title, WIN_TITLE " - COM%u, %u, %u, %s, %s",
                port, baud, bits, sparity, sstop);
    SetWindowTextA(ctx->hwnd, title);
    return (TRUE);
}

/*
---------------------------------------
    打开 TCPv4 通讯接口
---------------------------------------
*/
static bool_t
qst_com_tcpv4 (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t      port;
    socket_t    netw;

    /* 参数解析 <目标地址> <端口号> */
    if (argc < 3)
        return (FALSE);
    port = str2intxA(argv[2]);
    if (port > 65535)
        return (FALSE);

    ansi_t*     title;
    sQstComm*   ctx = (sQstComm*)parm;

    /* 关闭当前接口并打开 TCPv4 连接 */
    netw = client_tcp_open(argv[1], (int16u)port, 5000);
    if (netw == NULL)
        return (FALSE);
    socket_set_timeout(netw, -1, 50);

    /* 设置工作参数 */
    qst_com_close(parm, argc, argv);
    ctx->comm.obj.netw = netw;
    ctx->comm.send = qst_tcpv4_send;

    /* 启动接收线程 */
    ctx->comm.thrd = thread_new(0, qst_tcpv4_main, parm, FALSE);
    if (ctx->comm.thrd == NULL) {
        socket_close(netw);
        return (FALSE);
    }
    title = str_fmtA(WIN_TITLE " - TCPv4 [%s], %u", argv[1], port);
    if (title != NULL) {
        SetWindowTextA(ctx->hwnd, title);
        mem_free(title);
    }
    return (TRUE);
}

/*
---------------------------------------
    打开 UDPv4 通讯接口
---------------------------------------
*/
static bool_t
qst_com_udpv4 (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t      port;
    socket_t    netw;

    /* 参数解析 <目标地址> <端口号> */
    if (argc < 3)
        return (FALSE);
    port = str2intxA(argv[2]);
    if (port > 65535)
        return (FALSE);

    ansi_t*     title;
    sQstComm*   ctx = (sQstComm*)parm;

    /* 关闭当前接口并打开 UDPv4 连接 */
    netw = client_udp_open(argv[1], (int16u)port);
    if (netw == NULL)
        return (FALSE);
    socket_set_timeout(netw, -1, 50);

    /* 设置工作参数 */
    qst_com_close(parm, argc, argv);
    ctx->comm.obj.netw = netw;
    ctx->comm.send = qst_udpv4_send;

    /* 启动接收线程 */
    ctx->comm.thrd = thread_new(0, qst_udpv4_main, parm, FALSE);
    if (ctx->comm.thrd == NULL) {
        socket_close(netw);
        return (FALSE);
    }
    title = str_fmtA(WIN_TITLE " - UDPv4 [%s], %u", argv[1], port);
    if (title != NULL) {
        SetWindowTextA(ctx->hwnd, title);
        mem_free(title);
    }
    return (TRUE);
}

/*
---------------------------------------
    设置通讯发送模式
---------------------------------------
*/
static bool_t
qst_com_stype (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    /* 参数解析 <发送模式> */
    if (argc < 2)
        return (FALSE);

    sQstComm*   ctx = (sQstComm*)parm;

    if (str_cmpA(argv[1], "text") == 0)
        ctx->comm.tran = NULL;
    else
    if (str_cmpA(argv[1], "dos") == 0)
        ctx->comm.tran = qst_dos_tran;
    else
    if (str_cmpA(argv[1], "unix") == 0)
        ctx->comm.tran = qst_unx_tran;
    else
    if (str_cmpA(argv[1], "mac") == 0)
        ctx->comm.tran = qst_mac_tran;
    return (TRUE);
}

/*
---------------------------------------
    设置通讯接收模式
---------------------------------------
*/
static bool_t
qst_com_rtype (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    /* 参数解析 <接收模式> */
    if (argc < 2)
        return (FALSE);

    sQstComm*   ctx = (sQstComm*)parm;

    _ENTER_COM_SINGLE_
    if (str_cmpA(argv[1], "text") == 0)
        ctx->comm.render = NULL;
    else
    if (str_cmpA(argv[1], "hex") == 0)
        ctx->comm.render = qst_hex_show;
    _LEAVE_COM_SINGLE_
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

    /***** 通讯控制命令 *****/
    { "com:close", qst_com_close },
    { "com:rs232", qst_com_rs232 },
    { "com:tcpv4", qst_com_tcpv4 },
    { "com:udpv4", qst_com_udpv4 },
    { "com:stype", qst_com_stype },
    { "com:rtype", qst_com_rtype },

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

        /* 非命令直接交由发送函数处理 */
        if (!cmd_type_okay(string) && ctx->comm.thrd != NULL) {
            if (ctx->comm.tran == NULL)
            {
                /* 直接发送 */
                ctx->comm.send(ctx->comm.obj.parm, string, str_lenA(string));
            }
            else
            {
                uint_t  size;
                void_t* send;

                /* 变换后发送 */
                send = ctx->comm.tran(string, &size);
                if (send != NULL) {
                    ctx->comm.send(ctx->comm.obj.parm, send, size);
                    mem_free(send);
                }
            }
            mem_free(string);
            continue;
        }

        /* 执行这条命令 */
        cmd_exec_main(obj, ctx, string);
        mem_free(string);
    }

    /* 等待通讯线程结束 */
    if (ctx->comm.thrd != NULL) {
        ctx->comm.quit = TRUE;
        thread_wait(ctx->comm.thrd);
        thread_del(ctx->comm.thrd);
    }
    cmd_exec_free(obj);
    return (QST_OKAY);
}
