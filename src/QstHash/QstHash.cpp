//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "QstHash.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
//---------------------------------------------------------------------------

/* 全局工作上下文 */
static sQstHash     s_wrk_ctx;

/* 当前工作状态设置宏 */
#define QST_SET_STATE_FREE \
    ((TfrmMain*)(ctx.form))->Caption = WIN_TITLE; \
    ((TfrmMain*)(ctx.form))->Enabled = true;
#define QST_SET_STATE_BUSY \
    ((TfrmMain*)(ctx.form))->Caption = WIN_TITLE " - Hashing..."; \
    ((TfrmMain*)(ctx.form))->Enabled = false;

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/* 哈希上下文结构 */
typedef struct
{
        sARRAY      doit;   /* 哈希列表 */
        void_t**    ctxs;   /* 上下文列表 */
        sQstHash*   parm;   /* 应用上下文 */
        TfrmMain*   form;   /* 应用窗体 */
        bool_t      disk;   /* 是否磁盘文件 */

} sQHSH_CTX;

/*
---------------------------------------
    获取指定的哈希执行列表
---------------------------------------
*/
static void_t
qst_get_list (
  __CR_IO__ sQHSH_CTX*  ctx
    )
{
    AnsiString          name;
    const sQHSH_UNIT*   list;

    array_initT(&ctx->doit, sQHSH_UNIT*);
    for (int idx = 0; idx < ctx->form->lstHash->Count; idx++) {
        if (!ctx->form->lstHash->Checked[idx])
            continue;
        name = ctx->form->lstHash->Items->Strings[idx];
        for (list = ctx->parm->hasher; list->name != NULL; list++) {
            if (str_cmpA(list->name, name.c_str()) != 0)
                continue;
            if ((ctx->disk && !list->support_section) ||
                (array_push_growT(&ctx->doit, sQHSH_UNIT*, &list) == NULL))
                ctx->form->lstHash->Checked[idx] = false;
            break;
        }
    }
}

/*
---------------------------------------
    开始哈希计算
---------------------------------------
*/
static bool_t
qst_hash_init (
  __CR_IO__ sQHSH_CTX*  ctx
    )
{
    leng_t      idx, cnts;
    sQHSH_UNIT**    list;

    /* 初始化每个哈希单元的上下文 */
    qst_get_list(ctx);
    ctx->form->txtResult->Clear();
    cnts = array_get_sizeT(&ctx->doit, sQHSH_UNIT*);
    if (cnts == 0)
        goto _failure;
    ctx->ctxs = mem_talloc(cnts, void_t*);
    if (ctx->ctxs == NULL)
        goto _failure;
    list = array_get_dataT(&ctx->doit, sQHSH_UNIT*);
    for (idx = 0; idx < cnts; idx++)
        ctx->ctxs[idx] = list[idx]->hash_init();
    return (TRUE);

_failure:
    array_freeT(&ctx->doit, sQHSH_UNIT*);
    return (FALSE);
}

/*
---------------------------------------
    更新哈希计算
---------------------------------------
*/
static void_t
qst_hash_update (
  __CR_IN__ sQHSH_CTX*      ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    leng_t      idx, cnts;
    sQHSH_UNIT**    list;

    /* 只有磁盘文件才需要支持分块计算 */
    cnts = array_get_sizeT(&ctx->doit, sQHSH_UNIT*);
    list = array_get_dataT(&ctx->doit, sQHSH_UNIT*);
    for (idx = 0; idx < cnts; idx++) {
        if (ctx->ctxs[idx] != NULL)
            list[idx]->hash_update(ctx->ctxs[idx], data, size);
    }
}

/*
---------------------------------------
    结束哈希计算
---------------------------------------
*/
static void_t
qst_hash_finish (
  __CR_IO__ sQHSH_CTX*  ctx
    )
{
    ansi_t*     result;
    leng_t      idx, cnts;
    sQHSH_UNIT**    list;

    /* 结果加入文本框列表 */
    ctx->form->txtResult->Lines->BeginUpdate();
    cnts = array_get_sizeT(&ctx->doit, sQHSH_UNIT*);
    list = array_get_dataT(&ctx->doit, sQHSH_UNIT*);
    for (idx = 0; idx < cnts; idx++) {
        if (ctx->ctxs[idx] != NULL) {
            result = list[idx]->hash_finish(ctx->ctxs[idx]);
            if (result != NULL) {
                ctx->form->txtResult->Lines->Append(result);
                mem_free(result);
            }
        }
    }
    ctx->form->txtResult->Lines->EndUpdate();
    array_freeT(&ctx->doit, sQHSH_UNIT*);
    mem_free(ctx->ctxs);
}

/*
---------------------------------------
    内存文件哈希计算
---------------------------------------
*/
static void_t
qst_hash_memory (
  __CR_IO__ sQHSH_CTX*      ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    _ENTER_HSH_SINGLE_
    if (qst_hash_init(ctx)) {
        qst_hash_update(ctx, data, size);
        qst_hash_finish(ctx);
    }
    _LEAVE_HSH_SINGLE_
}

/* 分块计算大小 */
#define HSH_BLOCK   CR_M2B(4)

/*
---------------------------------------
    磁盘文件哈希计算
---------------------------------------
*/
static void_t
qst_hash_disk (
  __CR_IO__ sQHSH_CTX*      ctx,
  __CR_IN__ const ansi_t*   name
    )
{
    fraw_t  file;
    void_t* data;
    leng_t  blks;
    leng_t  rsts;
    fsize_t size;

    _ENTER_HSH_SINGLE_
    if (!qst_hash_init(ctx))
        goto _func_out;

    /* 打开目标文件 */
    size = file_sizeA(name);
    if ((fdist_t)size <= 0)
        goto _func_out;
    data = mem_malloc(HSH_BLOCK);
    if (data == NULL)
        goto _func_out;
    file = file_raw_openA(name, CR_FO_RO | CR_FO_SEQ);
    if (file == NULL)
        goto _failure1;

    /* 分块读取计算 */
    blks = (leng_t)(size / HSH_BLOCK);
    rsts = (leng_t)(size % HSH_BLOCK);
    for (; blks != 0; blks--) {
        if (file_raw_read(data, HSH_BLOCK, file) != HSH_BLOCK)
            goto _failure2;
        qst_hash_update(ctx, data, HSH_BLOCK);
    }
    if (rsts != 0) {
        if (file_raw_read(data, rsts, file) != rsts)
            goto _failure2;
        qst_hash_update(ctx, data, rsts);
    }

    /* 结束释放资源 */
    qst_hash_finish(ctx);
_failure2:
    file_raw_close(file);
_failure1:
    mem_free(data);
_func_out:
    _LEAVE_HSH_SINGLE_
}

/*
=======================================
    一次性完成哈希计算
=======================================
*/
CR_API void_t
qst_hash_total (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sQHSH_CTX   ctx;

    ctx.parm = &s_wrk_ctx;
    ctx.form = (TfrmMain*)(ctx.parm->form);
    ctx.disk = FALSE;

    QST_SET_STATE_BUSY
    qst_hash_memory(&ctx, data, size);
    QST_SET_STATE_FREE
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
qst_hsh_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstHash*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstHash*)parm;
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
qst_hsh_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstHash*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstHash*)parm;
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
qst_hsh_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstHash*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstHash*)parm;
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
qst_hsh_win_load (
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
    sQstHash*   ctx;

    ctx = (sQstHash*)parm;
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
qst_hsh_ldr_file (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    /* 参数解析 <文件路径> [头偏移] [尾偏移] [编码] [节点地址] [备注] */
    if (argc < 2)
        return (FALSE);

    sQHSH_CTX   ctx;

    ctx.parm = (sQstHash*)parm;
    ctx.form = (TfrmMain*)(ctx.parm->form);
    ctx.disk = TRUE;

    /* 执行哈希计算 */
    QST_SET_STATE_BUSY
    qst_hash_disk(&ctx, argv[1]);
    QST_SET_STATE_FREE

    /* 无论成功失败都返回成功 */
    return (TRUE);
}

/*
---------------------------------------
    加载一个内存文件
---------------------------------------
*/
static bool_t
qst_hsh_ldr_smem (
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

    sQHSH_CTX   ctx;

    ctx.parm = (sQstHash*)parm;
    ctx.form = (TfrmMain*)(ctx.parm->form);
    ctx.disk = FALSE;

    /* 获取整个共享文件 */
    data = share_file_get(argv[1], size);
    if (data == NULL)
        return (FALSE);

    /* 执行哈希计算 */
    QST_SET_STATE_BUSY
    qst_hash_memory(&ctx, data, size);
    QST_SET_STATE_FREE

    /* 用完后需要释放掉 */
    mem_free(data);
    return (TRUE);
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit", qst_hsh_app_exit },
    { "win:load", qst_hsh_win_load },
    { "win:save", qst_hsh_win_save },
    { "win:show", qst_hsh_win_show },

    /***** 公用加载命令 *****/
    { "ldr:file", qst_hsh_ldr_file },
    { "ldr:smem", qst_hsh_ldr_smem },

    /***** 私有命令映射 *****/
    { "qhsh:app:exit", qst_hsh_app_exit },
    { "qhsh:win:show", qst_hsh_win_show },
};

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_hsh_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    sQstHash*   ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_hsh_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    ctx = (sQstHash*)param;

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

    /* 加载哈希实现插件 (无需释放) */
    sbin = sbin_loadA("QstHashImp.dll");
    if (sbin == NULL)
        return (QST_ERROR);
    s_wrk_ctx.hasher = sbin_exportT(sbin, "hasher", sQHSH_UNIT*);
    if (s_wrk_ctx.hasher == NULL)
        return (QST_ERROR);

    thrd_t  thrd;

    /* 生成工作线程 */
    s_wrk_ctx.quit = FALSE;
    mtlock_init(&s_wrk_ctx.lock);
    thrd = thread_new(0, qst_hsh_main, &s_wrk_ctx, TRUE);
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
    netw_cli_close(s_wrk_ctx.netw);
    return (QST_OKAY);
}
//---------------------------------------------------------------------------
