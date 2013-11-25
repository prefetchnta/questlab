//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "QstData.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
//---------------------------------------------------------------------------

/* 全局工作上下文 */
static sQstData     s_wrk_ctx;

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/*
---------------------------------------
    执行所有数值观察函数
---------------------------------------
*/
static void_t
qst_data_view_int (
  __CR_IN__ sQstData*       ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const ansi_t*   title
    )
{
    ansi_t*             rett;
    TfrmMain*           form;
    AnsiString          text;
    const sQDAT_UNIT*   list;

    _ENTER_DAT_SINGLE_
    form = (TfrmMain*)(ctx->form);
    form->txtValue->Clear();
    form->pgeMain->ActivePageIndex = 0;
    if (title != NULL)
        form->edtInput->Text = title;
    form->txtValue->Lines->BeginUpdate();
    for (list = ctx->viewer; list->name != NULL; list++) {
        rett = list->data_view(data, size);
        if (rett != NULL) {
            text = AnsiString(list->name);
            text += AnsiString(rett);
            form->txtValue->Lines->Append(text);
            mem_free(rett);
        }
    }
    form->txtValue->Lines->EndUpdate();
    _LEAVE_DAT_SINGLE_
}

/*
=======================================
    执行所有数值观察函数
=======================================
*/
CR_API void_t
qst_data_view (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const ansi_t*   title
    )
{
    /* GUI 使用的包装 */
    qst_data_view_int(&s_wrk_ctx, data, size, title);
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
qst_dat_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstData*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstData*)parm;
    frm = (TfrmMain*)(ctx->form);
    SendMessage(frm->Handle, WM_CLOSE, 0, 0);
    ctx->quit = TRUE;
    return (FALSE);
}

/*
---------------------------------------
    显示窗口位置
---------------------------------------
*/
static bool_t
qst_dat_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstData*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstData*)parm;
    frm = (TfrmMain*)(ctx->form);
    misc_bring2top(frm->Handle, Application->Handle);
    return (TRUE);
}

/*
---------------------------------------
    保存窗口位置
---------------------------------------
*/
static bool_t
qst_dat_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstData*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstData*)parm;
    frm = (TfrmMain*)(ctx->form);

    /* 获取窗口位置 */
    sint_t  x1 = frm->Left;
    sint_t  y1 = frm->Top;
    uint_t  ww = frm->Width;
    uint_t  hh = frm->Height;

    /* 保存窗口位置 */
    return (misc_desk_save(WIN_ICONF, x1, y1, ww, hh));
}

/*
---------------------------------------
    加载窗口位置
---------------------------------------
*/
static bool_t
qst_dat_win_load (
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

    TfrmMain*   frm;
    sQstData*   ctx;

    ctx = (sQstData*)parm;
    frm = (TfrmMain*)(ctx->form);
    misc_bring2top(frm->Handle, Application->Handle);

    /* 设置窗口位置 */
    frm->Left = x1;
    frm->Top  = y1;
    return (TRUE);
}

/*
---------------------------------------
    加载一个磁盘文件
---------------------------------------
*/
static bool_t
qst_dat_ldr_file (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    /* 参数解析 <文件路径> [头偏移] [尾偏移] [编码] [节点地址] [备注] */
    if (argc < 2)
        return (FALSE);

    TfrmMain*   frm;
    sQstData*   ctx;

    ctx = (sQstData*)parm;
    frm = (TfrmMain*)(ctx->form);

    /* 打开目标文件 */
    frm->txtValue->Clear();
    frm->edtInput->Text = "";
    if (ctx->datin != NULL)
        CR_VCALL(ctx->datin)->release(ctx->datin);
    ctx->datin = create_disk_inA(argv[1]);
    if (ctx->datin != NULL)
        frm->edtInput->Text = AnsiString(argv[1]);

    /* 无论成功失败都返回成功 */
    return (TRUE);
}

/*
---------------------------------------
    加载一个内存文件
---------------------------------------
*/
static bool_t
qst_dat_ldr_smem (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    leng_t  size;
    void_t* data;

    /* 参数解析 <共享名> <文件大小> <文件名> [头偏移] [尾偏移] [编码] [备注] */
    if (argc < 4)
        return (FALSE);
    size = str2intx32A(argv[2]);

    /* 获取整个共享文件 */
    data = share_file_get(argv[1], size);
    if (data == NULL)
        return (FALSE);

    TfrmMain*   frm;
    sQstData*   ctx;

    ctx = (sQstData*)parm;
    frm = (TfrmMain*)(ctx->form);

    /* 打开目标文件 */
    frm->txtValue->Clear();
    frm->edtInput->Text = "";
    if (ctx->datin != NULL)
        CR_VCALL(ctx->datin)->release(ctx->datin);
    ctx->datin = create_buff_in(data, size, TRUE);
    if (ctx->datin != NULL)
        frm->edtInput->Text = AnsiString(argv[3]);
    else
        mem_free(data);

    /* 无论成功失败都返回成功 */
    return (TRUE);
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit", qst_dat_app_exit },
    { "win:load", qst_dat_win_load },
    { "win:save", qst_dat_win_save },
    { "win:show", qst_dat_win_show },

    /***** 公用加载命令 *****/
    { "ldr:file", qst_dat_ldr_file },
    { "ldr:smem", qst_dat_ldr_smem },

    /***** 私有命令映射 *****/
    { "qdat:app:exit", qst_dat_app_exit },
    { "qdat:win:show", qst_dat_win_show },
};

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_dat_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    sQstData*   ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_dat_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    ctx = (sQstData*)param;

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

/*
=======================================
    WinMain 程序入口
=======================================
*/
int WINAPI
WinMain (
  __CR_IN__ HINSTANCE   curt_app,
  __CR_IN__ HINSTANCE   prev_app,
  __CR_IN__ LPSTR       cmd_line,
  __CR_IN__ int         cmd_show
    )
{
    CR_NOUSE(curt_app);
    CR_NOUSE(prev_app);
    CR_NOUSE(cmd_line);
    CR_NOUSE(cmd_show);

    /* 只允许一个例程 */
    if (misc_is_running(EXE_XNAME))
        return (QST_ERROR);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_GUI))
        return (QST_ERROR);
    mem_zero(&s_wrk_ctx, sizeof(s_wrk_ctx));

    /* 初始化网络 */
    if (!socket_init())
        return (QST_ERROR);
    s_wrk_ctx.netw = netw_cli_open(EXE_XNAME);
    if (s_wrk_ctx.netw == NULL)
        return (QST_ERROR);

    /* 读取需要超时, 不然线程无法退出 */
    socket_set_timeout(s_wrk_ctx.netw, -1, QST_TCP_TOUT);

    sbin_t  sbin;

    /* 加载数值查看插件 (无需释放) */
    sbin = sbin_loadA("QstDataImp.dll");
    if (sbin == NULL)
        return (QST_ERROR);
    s_wrk_ctx.viewer = sbin_exportT(sbin, "viewer", sQDAT_UNIT*);
    if (s_wrk_ctx.viewer == NULL)
        return (QST_ERROR);

    thrd_t  thrd;

    /* 生成工作线程 */
    s_wrk_ctx.quit = FALSE;
    mtlock_init(&s_wrk_ctx.lock);
    thrd = thread_new(0, qst_dat_main, &s_wrk_ctx, TRUE);
    if (thrd == NULL)
        return (QST_ERROR);

    /* 标准 VCL 过程开始 */
    try
    {
        Application->Initialize();
        Application->Title = WIN_TITLE;
        Application->CreateForm(__classid(TfrmMain), &frmMain);
        s_wrk_ctx.form = (void_t*)frmMain;
        if (!thread_goon(thrd))
            return (QST_ERROR);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }

    /* 关闭线程直接退出 */
    if (!s_wrk_ctx.quit)
        s_wrk_ctx.quit = TRUE;
    thread_wait(thrd);
    thread_del(thrd);
    if (s_wrk_ctx.datin != NULL)
        CR_VCALL(s_wrk_ctx.datin)->release(s_wrk_ctx.datin);
    netw_cli_close(s_wrk_ctx.netw);
    return (QST_OKAY);
}
//---------------------------------------------------------------------------
