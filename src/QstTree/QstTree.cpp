//---------------------------------------------------------------------------

#include <vcl.h>
#include "uMain.h"
#include "QstTree.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("uMain.cpp", frmMain);
//---------------------------------------------------------------------------

/* 全局工作上下文 */
static sQstTree     s_wrk_ctx;

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/*
---------------------------------------
    合成包内文件路径
---------------------------------------
*/
static ansi_t*
qst_make_path (
  __CR_IN__ PVirtualNode        node,
  __CR_IN__ const ansi_t*       path,
  __CR_IN__ TVirtualStringTree* tree
    )
{
    ansi_t*         temp;
    ansi_t*         tots;
    sQTEE_file*     data;
    PVirtualNode    prev;

    data = (sQTEE_file*)tree->GetNodeData(node);
    temp = utf16_to_local(data->page, data->path);
    if (temp == NULL) {
        mem_free(path);
        return (NULL);
    }
    tots = str_fmtA("%s|%s", temp, path);
    mem_free(temp);
    mem_free(path);
    if (tots == NULL)
        return (NULL);
    prev = (PVirtualNode)data->root;
    if (prev == NULL)
        return (tots);
    return (qst_make_path(prev, tots, tree));
}

/*
---------------------------------------
    发送包内文件请求
---------------------------------------
*/
static void_t
qst_in_package (
  __CR_IN__ PVirtualNode        node,
  __CR_IN__ const sQTEE_file*   data,
  __CR_IN__ TVirtualStringTree* tree
    )
{
    ansi_t*         file;
    ansi_t*         path;
    ansi_t*         memo;
    ansi_t*         send;
    sQTEE_file*     prev_data;
    PVirtualNode    prev_node;

    /* 发送包内文件请求 */
    if (data->root == NULL)
        return;
    prev_node = (PVirtualNode)data->root;
    prev_data = (sQTEE_file*)tree->GetNodeData(prev_node);
    path = utf16_to_local(prev_data->page, prev_data->path);
    if (path == NULL)
        return;
    if (prev_data->root != NULL) {
        path = qst_make_path((PVirtualNode)prev_data->root, path, tree);
        if (path == NULL)
            return;
    }
    file = utf16_to_local(data->page, data->path);
    if (file == NULL) {
        mem_free(path);
        return;
    }
    if (data->memo == NULL ||
        data->memo[0] != CR_WC('$'))
        memo = NULL;
    else
        memo = utf16_to_local(data->page, data->memo);

    /* 合成命令行 (必须发送节点的指针) */
    if (memo == NULL) {
        send = str_fmtA("ldr:pack \"%s\" \"%s\" 0 0 %u 0x%08X",
                            path, file, data->page, node);
    }
    else {
        send = str_fmtA("ldr:pack \"%s\" \"%s\" 0 0 %u 0x%08X \"%s\"",
                            path, file, data->page, node, memo + 1);
        mem_free(memo);
    }
    mem_free(file);
    mem_free(path);

    /* 发送命令行 */
    if (send != NULL) {
        cmd_shl_send(s_wrk_ctx.netw, send);
        mem_free(send);
    }
}

/*
=======================================
    发送文件解析请求
=======================================
*/
CR_API void_t
qst_try_load (
  __CR_IN__ PVirtualNode        node,
  __CR_IO__ sQTEE_file*         data,
  __CR_IN__ TVirtualStringTree* tree
    )
{
    ansi_t* path;
    ansi_t* memo;
    ansi_t* send;
    fsize_t size;

    /* 清除当前内容加载 */
    cmd_shl_send(s_wrk_ctx.netw, "app:reset");

    /* 发送包内文件请求 */
    if (data->attr & QST_FILE_MEM) {
        qst_in_package(node, data, tree);
        return;
    }

    /* 设备文件不更新大小 */
    if (data->path[0] != CR_WC(':')) {
        size = file_sizeW(data->path);
        if (size == CR_F_ERROR)
            return;
        data->size = size;
    }

    /* 发送磁盘文件请求 */
    path = utf16_to_local(data->page, data->path);
    if (path == NULL)
        return;
    if (data->memo == NULL)
        memo = NULL;
    else
        memo = utf16_to_local(data->page, data->memo);

    /* 合成命令行 (必须发送节点的指针) */
    if (memo == NULL) {
        send = str_fmtA("ldr:file \"%s\" %I64u %I64u %u 0x%08X",
                        path, data->offs, data->offs + data->pack,
                                data->page, node);
    }
    else {
        send = str_fmtA("ldr:file \"%s\" %I64u %I64u %u 0x%08X \"%s\"",
                        path, data->offs, data->offs + data->pack,
                                data->page, node, memo);
        mem_free(memo);
    }
    mem_free(path);

    /* 发送命令行 */
    if (send != NULL) {
        cmd_shl_send(s_wrk_ctx.netw, send);
        mem_free(send);
    }
}

/*
---------------------------------------
    解析一个 XML 标签
---------------------------------------
*/
static bool_t
qst_do_node (
  __CR_OT__ sQTEE_file* data,
  __CR_IN__ bool_t      disk,
  __CR_IO__ sXNODEw*    node
    )
{
    /* 文件大小属性 (必有) */
    struct_zero(data, sQTEE_file);
    data->size = xml_attr_intx64W(CR_WS("size"), 0, node);
    if (!node->found) return (FALSE);

    /* 文件附加属性 (可选) */
    if (xml_attr_intxW(CR_WS("comp"), FALSE, node))
        data->attr |= QST_FILE_CMP;
    if (xml_attr_intxW(CR_WS("hide"), FALSE, node))
        data->attr |= QST_FILE_HID;
    if (xml_attr_intxW(CR_WS("cryp"), FALSE, node))
        data->attr |= QST_FILE_ENC;

    /* 包内文件必须有的属性 */
    if (!disk) {
        data->attr |= QST_FILE_MEM;
        data->offs = xml_attr_intx64W(CR_WS("offs"), 0, node);
        if (!node->found) return (FALSE);
        data->pack = xml_attr_intx64W(CR_WS("pack"), 0, node);
        if (!node->found) return (FALSE);

        /* 设置图标索引 */
        data->icon = QST_ICON_MEM;
    }
    else {
        data->offs = xml_attr_intx64W(CR_WS("offs"), 0, node);
        data->pack = xml_attr_intx64W(CR_WS("pack"), 0, node);

        /* 设置图标索引 */
        data->icon = xml_attr_intxW(CR_WS("icon"), QST_ICON_DSK, node);
        if (data->icon > QST_ICON_MAX || data->icon <= QST_ICON_MEM)
            data->icon = QST_ICON_DSK;
    }

    /* 压缩属性可以计算得出 */
    if (data->pack != 0 && data->pack != data->size)
        data->attr |= QST_FILE_CMP;

    /* 文件名称属性 (必有) */
    data->path = xml_attr_stringW(CR_WS("path"), node);
    if (data->path == NULL)
        return (FALSE);

    /* 文件备注属性 (可选) */
    data->memo = xml_attr_stringW(CR_WS("memo"), node);

    leng_t  len = str_lenW(data->path);

    /* 显示用的文件名生成 */
    for (; len != 0; len--) {
        if (is_slashW(data->path[len - 1]))
            break;
    }
    data->name = &data->path[len];
    if (data->name[0] == 0x0000)
        return (FALSE);

    wide_t* ext = str_dupW(data->name);

    /* 抽出文件扩展名并查找图标编号 */
    if (ext != NULL) {
        filext_extractW(ext, data->name);
        if (*ext == CR_WC('.'))
        {
            sQTEE_icon* icon;

            icon = curbead_findT(&s_wrk_ctx.icon, sQTEE_icon, str_lwrW(ext));
            if (icon != NULL) data->icon = icon->idx;
        }
        mem_free(ext);
    }
    return (TRUE);
}

/*
---------------------------------------
    加入一个文件到树形列表
---------------------------------------
*/
static bool_t
qst_add_node (
  __CR_IN__ leng_t              head,
  __CR_IN__ const sQTEE_file*   data,
  __CR_IN__ PVirtualNode        node,
  __CR_IN__ TVirtualStringTree* tree
    )
{
    uint_t      cnts;
    wide_t*     path;
    wide_t**    list;
    sQTEE_file  temp;
    sQTEE_file* unit;

    /* 分割文件路径 */
    path = str_dupW(data->path + head);
    if (path == NULL)
        return (FALSE);
    list = path_splitW(path, &cnts);
    if (list == NULL) {
        mem_free(path);
        return (FALSE);
    }

    /* 默认使用根节点 */
    if (node == NULL)
        node = tree->RootNode;
    if (cnts > 1)
    {
        uint_t  ii, jj, nums;

        /* 有文件路径 */
        struct_zero(&temp, sQTEE_file);
        temp.icon  = QST_ICON_DIR;
        temp.attr |= QST_FILE_DIR;
        for (ii = 0; ii < cnts - 1; ii++)
        {
            PVirtualNode    find;

            /* 查找已建目录 */
            jj = 0;
            nums = node->ChildCount;
            if (nums != 0) {
                find = node->FirstChild;
                for (; jj < nums; jj++) {
                    unit = (sQTEE_file*)tree->GetNodeData(find);
                    if (!(unit->attr & QST_FILE_DIR))
                        break;
                    if (str_cmpIW(unit->name, list[ii]) == 0)
                        break;
                    find = find->NextSibling;
                }
                if (jj < nums) {
                    node = find;
                    if (unit->attr & QST_FILE_DIR)
                        continue;
                }
            }

            /* 建立这个目录 */
            temp.name = str_dupW(list[ii]);
            if (temp.name == NULL) {
                mem_free(list);
                mem_free(path);
                return (FALSE);
            }

            try
            {
                if (jj >= nums)
                    node = tree->AddChild(node);
                else
                    node = tree->InsertNode(node, amInsertBefore);
            }
            catch (...)
            {
                mem_free(temp.name);
                mem_free(list);
                mem_free(path);
                return (FALSE);
            }
            struct_cpy(tree->GetNodeData(node), &temp, sQTEE_file);
        }
    }

    PVirtualNode    new_node;

    /* 文件加入列表 */
    mem_free(list);
    mem_free(path);
    try
    {
        new_node = tree->AddChild(node);
    }
    catch (...)
    {
        return (FALSE);
    }
    struct_cpy(tree->GetNodeData(new_node), data, sQTEE_file);
    return (TRUE);
}

/*
---------------------------------------
    加载一个文件列表文件
---------------------------------------
*/
static bool_t
qst_load_xml (
  __CR_IO__ sQstTree*       parm,
  __CR_IN__ const ansi_t*   name
    )
{
    sXMLw*  xml;
    leng_t  idx;
    ansi_t* str;
    wide_t* text;
    uint_t  page;

    /* 防止重入 */
    if (parm->busy)
        return (FALSE);
    parm->busy = TRUE;

    /* 加载文件 */
    str = file_load_as_strA(name);
    if (str == NULL) {
        parm->busy = FALSE;
        return (FALSE);
    }

    /* 转换到宽字符 */
    text = text_to_utf16(str, &page, TRUE);
    mem_free(str);
    if (text == NULL) {
        parm->busy = FALSE;
        return (FALSE);
    }
    if (page == CR_LOCAL)
        page = get_sys_codepage();

    /* 解析 XML 文件 */
    xml = xml_parseW(text);
    mem_free(text);
    if (xml == NULL) {
        parm->busy = FALSE;
        return (FALSE);
    }

    PVirtualNode        node;
    TVirtualStringTree* tree;

    /* 检查根标签 */
    if (xml->nodes[0].name[0] == CR_WC('?'))
        idx = 1;
    else
        idx = 0;
    if (xml->nodes[idx].closed ||
        str_cmpIW(xml->nodes[idx].name, CR_WS("QstTree")) != 0) {
        xml_closeW(xml);
        parm->busy = FALSE;
        return (FALSE);
    }

    /* 指定了特定编码 */
    page = xml_attr_intxW(CR_WS("page"), page, &xml->nodes[idx]);
    if (is_cr_unicode(page)) {
        xml_closeW(xml);
        parm->busy = FALSE;
        return (FALSE);
    }

    leng_t  begin = 0;
    bool_t  disk = FALSE;

    /* 获取节点地址 */
    tree = ((TfrmMain*)(parm->form))->lstTree;
    node = (PVirtualNode)xml_attr_intx32W(CR_WS("node"), 0, &xml->nodes[idx]);
    if (node != NULL)
    {
        /* 指定了节点地址 */
        try
        {
            tree->AbsoluteIndex(node);
        }
        catch (...)
        {
            xml_closeW(xml);
            parm->busy = FALSE;
            return (FALSE);
        }

        sQTEE_file* unit;

        /* 改变节点图标 */
        unit = (sQTEE_file*)tree->GetNodeData(node);
        if (unit->icon <= QST_ICON_MEM)
            unit->icon  = QST_ICON_PAK;
        unit->attr |= QST_FILE_PAK;
    }
    else
    {
        ansi_t* send;
        wide_t* root;

        /* 指定了磁盘根目录 */
        root = xml_attr_stringW(CR_WS("root"), &xml->nodes[idx]);
        if (root == NULL) {
            xml_closeW(xml);
            parm->busy = FALSE;
            return (FALSE);
        }
        begin = str_lenW(root);
        if (!is_slashW(root[begin - 1]))
            begin += 1;

        /* 发送根目录路径 */
        str = utf16_to_local(page, root);
        if (str == NULL) {
            mem_free(root);
            xml_closeW(xml);
            parm->busy = FALSE;
            return (FALSE);
        }
        send = str_fmtA("res:root \"%s\"", str);
        mem_free(str);
        if (send == NULL) {
            mem_free(root);
            xml_closeW(xml);
            parm->busy = FALSE;
            return (FALSE);
        }
        cmd_shl_send(parm->netw, send);
        mem_free(send);

        sQTEE_file  temp;

        /* 建立这个根目录 */
        struct_zero(&temp, sQTEE_file);
        temp.name  = root;
        temp.page  = page;
        temp.icon  = QST_ICON_DIR;
        temp.attr |= QST_FILE_DIR;
        try
        {
            node = tree->AddChild(tree->RootNode);
        }
        catch (...)
        {
            mem_free(root);
            xml_closeW(xml);
            parm->busy = FALSE;
            return (FALSE);
        }
        struct_cpy(tree->GetNodeData(node), &temp, sQTEE_file);
        disk = TRUE;
    }

    /* 逐个标签加载 */
    tree->BeginUpdate();
    for (idx++; idx < xml->count; idx++)
    {
        sQTEE_file  data;

        /* 跳过没用的节点 */
        if (str_cmpIW(xml->nodes[idx].name, CR_WS("/QstTree")) == 0)
            break;
        if (!xml->nodes[idx].closed ||
            str_cmpIW(xml->nodes[idx].name, CR_WS("File")) != 0) {
            idx = xml_skip_tagW(xml->nodes, idx, xml->count) - 1;
            continue;
        }

        /* 解析节点属性 */
        if (!qst_do_node(&data, disk, &xml->nodes[idx]))
            continue;
        data.page = page;
        data.head = begin;
        data.root = disk ? NULL : node;

        /* 加入线性列表 */
        if (disk) {
            if (array_push_growT(&parm->list, sQTEE_file, &data) == NULL) {
                TRY_FREE(data.path);
                TRY_FREE(data.memo);
                continue;
            }
        }

        /* 加入树形列表 */
        if (!qst_add_node(begin, &data, node, tree)) {
            if (disk)
                parm->list.__cnts__--;
            TRY_FREE(data.path);
            TRY_FREE(data.memo);
            continue;
        }
    }
    tree->EndUpdate();
    xml_closeW(xml);
    parm->busy = FALSE;
    misc_bring2top(frmMain->Handle, Application->Handle);
    return (TRUE);
}

/*
=======================================
    加载一个文件列表文件
=======================================
*/
CR_API bool_t
qst_load_xml2 (
  __CR_IN__ const ansi_t*   name
    )
{
    /* GUI 使用的包装 */
    return (qst_load_xml(&s_wrk_ctx, name));
}

/*
---------------------------------------
    释放所有文件节点
---------------------------------------
*/
static void_t
qst_free_lst (
  __CR_IO__ sQstTree*   parm
    )
{
    /* 防止重入 */
    if (parm->busy)
        return;
    parm->busy = TRUE;
    array_freeT(&parm->list, sQTEE_file);
    ((TfrmMain*)(parm->form))->lstTree->BeginUpdate();
    ((TfrmMain*)(parm->form))->lstTree->Clear();
    ((TfrmMain*)(parm->form))->lstTree->EndUpdate();
    parm->busy = FALSE;
}

/*
=======================================
    释放所有文件节点
=======================================
*/
CR_API void_t
qst_free_lst2 (void_t)
{
    /* GUI 使用的包装 */
    qst_free_lst(&s_wrk_ctx);
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
qst_tee_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstTree*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstTree*)parm;
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
qst_tee_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstTree*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstTree*)parm;
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
qst_tee_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    TfrmMain*   frm;
    sQstTree*   ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstTree*)parm;
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
qst_tee_win_load (
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
    sQstTree*   ctx;

    ctx = (sQstTree*)parm;
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
---------------------------------------
    加载文件列表
---------------------------------------
*/
static bool_t
qst_tee_dir_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    ansi_t* name;

    /* 参数解析 [文件导入表] */
    if (argc > 1)
        name = argv[1];
    else
        name = QST_IMPORT_LST;
    return (qst_load_xml((sQstTree*)parm, name));
}

/*
---------------------------------------
    释放当前列表
---------------------------------------
*/
static bool_t
qst_tee_dir_clear (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(argc);
    CR_NOUSE(argv);
    qst_free_lst((sQstTree*)parm);
    return (TRUE);
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit", qst_tee_app_exit },
    { "win:load", qst_tee_win_load },
    { "win:save", qst_tee_win_save },
    { "win:show", qst_tee_win_show },

    /***** 列表操作命令 *****/
    { "dir:load",  qst_tee_dir_load  },
    { "dir:clear", qst_tee_dir_clear },

    /***** 私有命令映射 *****/
    { "qtee:app:exit", qst_tee_app_exit },
    { "qtee:win:show", qst_tee_win_show },
};

/*
---------------------------------------
    工作线程
---------------------------------------
*/
static uint_t STDCALL
qst_tee_main (
  __CR_IN__ void_t* param
    )
{
    exec_t      obj;
    sQstTree*   ctx;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL) {
        qst_tee_app_exit(param, 0, NULL);
        return (QST_ERROR);
    }
    ctx = (sQstTree*)param;

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
---------------------------------------
    图标比较回调
---------------------------------------
*/
static bool_t
icon_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    const wide_t*       ext = (wide_t*)key;
    const sQTEE_icon*   unt = (sQTEE_icon*)obj;

    if (str_cmpW(ext, unt->ext) == 0)
        return (TRUE);
    return (FALSE);
}

/*
---------------------------------------
    图标索引回调
---------------------------------------
*/
static uint_t
icon_find (
  __CR_IN__ const void_t*   key
    )
{
    leng_t  len = str_sizeW((wide_t*)key);

    return (hash_crc16h_total(key, len));
}

/* 扩展名查找表 */
static const sQTEE_icon s_ext_list[] =
{
    {   QST_ICON_ASM        , CR_WS(".asm")         },
    {   QST_ICON_ASM        , CR_WS(".s")           },
    {   QST_ICON_BAT        , CR_WS(".bat")         },
    {   QST_ICON_BAT        , CR_WS(".cmd")         },
    {   QST_ICON_C          , CR_WS(".c")           },
    {   QST_ICON_CPP        , CR_WS(".cpp")         },
    {   QST_ICON_CPP        , CR_WS(".cc")          },
    {   QST_ICON_CPP        , CR_WS(".cxx")         },
    {   QST_ICON_CS         , CR_WS(".cs")          },
    {   QST_ICON_DOC        , CR_WS(".doc")         },
    {   QST_ICON_DOC        , CR_WS(".rtf")         },
    {   QST_ICON_H          , CR_WS(".h")           },
    {   QST_ICON_H          , CR_WS(".hpp")         },
    {   QST_ICON_H          , CR_WS(".hh")          },
    {   QST_ICON_H          , CR_WS(".hxx")         },
    {   QST_ICON_INL        , CR_WS(".inc")         },
    {   QST_ICON_INL        , CR_WS(".inl")         },
    {   QST_ICON_INL        , CR_WS(".lst")         },
    {   QST_ICON_TXT        , CR_WS(".log")         },
    {   QST_ICON_TXT        , CR_WS(".txt")         },
    {   QST_ICON_VB         , CR_WS(".vb")          },
    {   QST_ICON_INI        , CR_WS(".ini")         },
    {   QST_ICON_INI        , CR_WS(".inf")         },
    {   QST_ICON_INI        , CR_WS(".cfg")         },
    {   QST_ICON_DLL        , CR_WS(".dll")         },
    {   QST_ICON_FON        , CR_WS(".fon")         },
    {   QST_ICON_TTF        , CR_WS(".ttc")         },
    {   QST_ICON_TTF        , CR_WS(".ttf")         },
    {   QST_ICON_ISO        , CR_WS(".iso")         },
    {   QST_ICON_WRI        , CR_WS(".wri")         },
    {   QST_ICON_EXE        , CR_WS(".com")         },
    {   QST_ICON_EXE        , CR_WS(".exe")         },
    {   QST_ICON_FNT        , CR_WS(".fnt")         },
    {   QST_ICON_HLP        , CR_WS(".hlp")         },
    {   QST_ICON_ILK        , CR_WS(".ilk")         },
    {   QST_ICON_LIB        , CR_WS(".lib")         },
    {   QST_ICON_MAK        , CR_WS(".mak")         },
    {   QST_ICON_NCB        , CR_WS(".ncb")         },
    {   QST_ICON_OBJ        , CR_WS(".obj")         },
    {   QST_ICON_PDB        , CR_WS(".pdb")         },
    {   QST_ICON_SQL        , CR_WS(".sql")         },
    {   QST_ICON_XDR        , CR_WS(".xdr")         },
    {   QST_ICON_XML        , CR_WS(".xml")         },
    {   QST_ICON_XSD        , CR_WS(".xsd")         },
    {   QST_ICON_XSL        , CR_WS(".xsl")         },
    {   QST_ICON_XSL        , CR_WS(".xslt")        },
    {   QST_ICON_PCB        , CR_WS(".pcb")         },
    {   QST_ICON_PCB        , CR_WS(".pcbdoc")      },
    {   QST_ICON_SCH        , CR_WS(".sch")         },
    {   QST_ICON_SCH        , CR_WS(".schdoc")      },
    {   QST_ICON_PCBLIB     , CR_WS(".pcblib")      },
    {   QST_ICON_SCHLIB     , CR_WS(".schlib")      },
};

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

    uint_t  idx, len = cntsof(s_ext_list);

    /* 创建扩展名图标表 */
    if (!curbead_initT(&s_wrk_ctx.icon, sQTEE_icon, len))
        return (QST_ERROR);
    s_wrk_ctx.icon.find = icon_find;
    s_wrk_ctx.icon.comp = icon_comp;
    for (idx = 0; idx < len; idx++) {
        if (curbead_insertT(&s_wrk_ctx.icon, sQTEE_icon,
                s_ext_list[idx].ext, &s_ext_list[idx]) == NULL)
            return (QST_ERROR);
    }

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
    s_wrk_ctx.busy = FALSE;
    s_wrk_ctx.quit = FALSE;
    thrd = thread_new(0, qst_tee_main, &s_wrk_ctx, TRUE);
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
    curbead_freeT(&s_wrk_ctx.icon, sQTEE_icon);
    return (QST_OKAY);
}
//---------------------------------------------------------------------------
