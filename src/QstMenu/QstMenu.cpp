//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "uAbout.h"
#include "QstMenu.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
USEFORM("uAbout.cpp", frmAbout);
//---------------------------------------------------------------------------

/* 全局工作上下文 */
static sQstMenu     s_wrk_ctx;

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/*
=======================================
    发送单条远程命令
=======================================
*/
CR_API void_t
qst_send_cmdz (
  __CR_IN__ const ansi_t*   cmd
    )
{
    cmd_shl_send(s_wrk_ctx.netw, cmd);
}

/*
---------------------------------------
    加载一组子菜单项
---------------------------------------
*/
static leng_t
qst_load_item (
  __CR_IN__ sXMLw*          xml,
  __CR_IN__ leng_t          idx,
  __CR_IN__ const wide_t*   end,
  __CR_IN__ TMenuItem*      menu,
  __CR_IN__ sCURBEAD*       event
    )
{
    TMenuItem*  item;

    /* 逐个标签加载 */
    for (idx++; idx < xml->count; idx++)
    {
        ansi_t* name_a;
        ansi_t* text_a;
        ansi_t* scut_a;
        wide_t* name_w;
        wide_t* text_w;
        wide_t* scut_w;

        /* 跳过没用的节点 */
        if (str_cmpIW(xml->nodes[idx].name, end) == 0)
            break;
        if (str_cmpIW(xml->nodes[idx].name, CR_WS("MenuItem")) != 0) {
            idx = xml_skip_tagW(xml->nodes, idx, xml->count) - 1;
            continue;
        }

        /* 解析菜单项 */
        name_w = xml_attr_stringW(CR_WS("name"), &xml->nodes[idx]);
        if (name_w == NULL)
            continue;
        text_w = xml_attr_stringW(CR_WS("text"), &xml->nodes[idx]);
        if (text_w == NULL) {
            mem_free(name_w);
            continue;
        }
        scut_w = xml_attr_stringW(CR_WS("scut"), &xml->nodes[idx]);

        /* 转换到本地编码 */
        name_a = utf16_to_local(CR_LOCAL, name_w);
        mem_free(name_w);
        text_a = utf16_to_local(CR_LOCAL, text_w);
        mem_free(text_w);
        if (scut_w != NULL) {
            scut_a = utf16_to_local(CR_LOCAL, scut_w);
            mem_free(scut_w);
        }
        else {
            scut_a = NULL;
        }
        if (name_a == NULL || text_a == NULL) {
            TRY_FREE(name_a)
            TRY_FREE(text_a)
            TRY_FREE(scut_a)
            continue;
        }

        bool    radio;
        bool    chked;
        uint_t  group;

        if (xml_attr_intxW(CR_WS("radio"), FALSE, &xml->nodes[idx]))
            radio = true;
        else
            radio = false;
        if (xml_attr_intxW(CR_WS("checked"), FALSE, &xml->nodes[idx]))
            chked = true;
        else
            chked = false;
        group = xml_attr_intxW(CR_WS("group"), 0, &xml->nodes[idx]);

        sMenuEvent*     find;
        TNotifyEvent    call = NULL;

        /* 根据名称查找事件 */
        find = curbead_findT(event, sMenuEvent, name_a);
        if (find != NULL) call = find->call;

        /* 生成菜单项 */
        if (scut_a == NULL) {
            item = NewItem(text_a, 0, chked, true, call, 0, name_a);
        }
        else {
            item = NewItem(text_a, TextToShortCut(scut_a), chked,
                           true, call, 0, name_a);
            mem_free(scut_a);
        }
        mem_free(name_a);
        mem_free(text_a);
        item->AutoCheck  = radio;
        item->RadioItem  = radio;
        item->GroupIndex = (Byte)group;
        menu->Add(item);

        /* 有子菜单项 */
        if (!xml->nodes[idx].closed)
            idx = qst_load_item(xml, idx, CR_WS("/MenuItem"), item, event);
    }
    return (idx);
}

/*
=======================================
    加载菜单配置文件
=======================================
*/
CR_API bool_t
qst_load_menu (
  __CR_IN__ TMenuItem*  menu,
  __CR_IN__ sCURBEAD*   event
    )
{
    sXMLw*  xml;
    leng_t  idx;
    ansi_t* str;

    /* 加载文件 */
    str = file_load_as_strA(QST_PATH_CONFIG WIN_XCONF);
    if (str == NULL)
        return (FALSE);

    /* 解析 XML 文件 */
    xml = xml_from_textW(str);
    mem_free(str);
    if (xml == NULL)
        return (FALSE);

    /* 检查根标签 */
    if (xml->nodes[0].name[0] == CR_WC('?'))
        idx = 1;
    else
        idx = 0;
    if (xml->nodes[idx].closed ||
        str_cmpIW(xml->nodes[idx].name, CR_WS("QstMenu")) != 0) {
        xml_closeW(xml);
        return (FALSE);
    }

    /* 加载菜单项 (容错执行) */
    qst_load_item(xml, idx, CR_WS("/QstMenu"), menu, event);
    xml_closeW(xml);
    return (TRUE);
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
qst_mnu_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstMenu*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMenu*)parm;
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
qst_mnu_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstMenu*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMenu*)parm;
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
qst_mnu_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstMenu*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMenu*)parm;
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
=======================================
    保存窗口位置
=======================================
*/
CR_API void_t
qst_mnu_win_save2 (void_t)
{
    /* GUI 使用的包装 */
    qst_mnu_win_save(&s_wrk_ctx, 0, NULL);
}

/*
---------------------------------------
    加载窗口位置
---------------------------------------
*/
static bool_t
qst_mnu_win_load (
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
    sQstMenu*   ctx;

    ctx = (sQstMenu*)parm;
    frm = (TfrmMain*)(ctx->form);
    misc_bring2top(frm->Handle, Application->Handle);

    /* 设置窗口位置 */
    frm->Left   = x1;
    frm->Top    = y1;
    frm->Width  = ww;
    frm->Height = hh;
    return (TRUE);
}

/*
=======================================
    加载窗口位置
=======================================
*/
CR_API void_t
qst_mnu_win_load2 (void_t)
{
    /* GUI 使用的包装 */
    qst_mnu_win_load(&s_wrk_ctx, 0, NULL);
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit", qst_mnu_app_exit },
    { "win:load", qst_mnu_win_load },
    { "win:save", qst_mnu_win_save },
    { "win:show", qst_mnu_win_show },

    /***** 私有命令映射 *****/
    { "qmnu:app:exit", qst_mnu_app_exit },
    { "qmnu:win:show", qst_mnu_win_show },
};

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_mnu_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    sQstMenu*   ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_mnu_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    ctx = (sQstMenu*)param;

    /* 工作循环 */
    while (!ctx->quit)
    {
        sINIu*  iniu;
        ansi_t* text;
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

    thrd_t  thrd;

    /* 生成工作线程 */
    s_wrk_ctx.quit = FALSE;
    thrd = thread_new(0, qst_mnu_main, &s_wrk_ctx, TRUE);
    if (thrd == NULL)
        return (QST_ERROR);

    /* 标准 VCL 过程开始 */
    try
    {
        Application->Initialize();
        Application->Title = WIN_TITLE;
        Application->CreateForm(__classid(TfrmMain), &frmMain);
        Application->CreateForm(__classid(TfrmAbout), &frmAbout);
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
