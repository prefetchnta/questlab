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
        if (data->icon > QST_ICON_MAX_INDEX || data->icon <= QST_ICON_MEM)
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
    {   QST_ICON_ASM        , CR_WS(".asm")                     },
    {   QST_ICON_BAT        , CR_WS(".bat")                     },
    {   QST_ICON_C          , CR_WS(".c")                       },
    {   QST_ICON_C          , CR_WS(".cod")                     },
    {   QST_ICON_CPP        , CR_WS(".cpp")                     },
    {   QST_ICON_CPP        , CR_WS(".cc")                      },
    {   QST_ICON_CPP        , CR_WS(".cxx")                     },
    {   QST_ICON_CPP        , CR_WS(".c++")                     },
    {   QST_ICON_CS         , CR_WS(".cs")                      },
    {   QST_ICON_RTF        , CR_WS(".rtf")                     },
    {   QST_ICON_H          , CR_WS(".h")                       },
    {   QST_ICON_H          , CR_WS(".hpp")                     },
    {   QST_ICON_H          , CR_WS(".hh")                      },
    {   QST_ICON_H          , CR_WS(".hxx")                     },
    {   QST_ICON_H          , CR_WS(".h++")                     },
    {   QST_ICON_H          , CR_WS(".tlh")                     },
    {   QST_ICON_INL        , CR_WS(".inc")                     },
    {   QST_ICON_INL        , CR_WS(".inl")                     },
    {   QST_ICON_INL        , CR_WS(".lst")                     },
    {   QST_ICON_INL        , CR_WS(".def")                     },
    {   QST_ICON_INL        , CR_WS(".bsc")                     },
    {   QST_ICON_INL        , CR_WS(".idl")                     },
    {   QST_ICON_INL        , CR_WS(".tli")                     },
    {   QST_ICON_INL        , CR_WS(".dfn")                     },
    {   QST_ICON_INL        , CR_WS(".template")                },
    {   QST_ICON_TXT        , CR_WS(".log")                     },
    {   QST_ICON_TXT        , CR_WS(".txt")                     },
    {   QST_ICON_TXT        , CR_WS(".dep")                     },
    {   QST_ICON_TXT        , CR_WS(".dof")                     },
    {   QST_ICON_TXT        , CR_WS(".tlog")                    },
    {   QST_ICON_TXT        , CR_WS(".lastbuildstate")          },
    {   QST_ICON_VB         , CR_WS(".vb")                      },
    {   QST_ICON_INI        , CR_WS(".ini")                     },
    {   QST_ICON_INI        , CR_WS(".inf")                     },
    {   QST_ICON_INI        , CR_WS(".cfg")                     },
    {   QST_ICON_INI        , CR_WS(".cnf")                     },
    {   QST_ICON_INI        , CR_WS(".conf")                    },
    {   QST_ICON_INI        , CR_WS(".hgeol")                   },
    {   QST_ICON_INI        , CR_WS(".hgignore")                },
    {   QST_ICON_INI        , CR_WS(".hgtags")                  },
    {   QST_ICON_INI        , CR_WS(".gitignore")               },
    {   QST_ICON_INI        , CR_WS(".gitmodules")              },
    {   QST_ICON_INI        , CR_WS(".gitattributes")           },
    {   QST_ICON_INI        , CR_WS(".editorconfig")            },
    {   QST_ICON_INI        , CR_WS(".properties")              },
    {   QST_ICON_INI        , CR_WS(".security")                },
    {   QST_ICON_DLL        , CR_WS(".dll")                     },
    {   QST_ICON_FON        , CR_WS(".fon")                     },
    {   QST_ICON_FON        , CR_WS(".fnt")                     },
    {   QST_ICON_TTF        , CR_WS(".ttf")                     },
    {   QST_ICON_ISO        , CR_WS(".iso")                     },
    {   QST_ICON_WRI        , CR_WS(".wri")                     },
    {   QST_ICON_COM        , CR_WS(".com")                     },
    {   QST_ICON_FNT        , CR_WS(".pfm")                     },
    {   QST_ICON_FNT        , CR_WS(".compositefont")           },
    {   QST_ICON_HLP        , CR_WS(".hlp")                     },
    {   QST_ICON_ILK        , CR_WS(".ilk")                     },
    {   QST_ICON_LIB        , CR_WS(".lib")                     },
    {   QST_ICON_MAK        , CR_WS(".mk")                      },
    {   QST_ICON_MAK        , CR_WS(".mak")                     },
    {   QST_ICON_NCB        , CR_WS(".ncb")                     },
    {   QST_ICON_NCB        , CR_WS(".sdf")                     },
    {   QST_ICON_OBJ        , CR_WS(".obj")                     },
    {   QST_ICON_PDB        , CR_WS(".pdb")                     },
    {   QST_ICON_SQL        , CR_WS(".sql")                     },
    {   QST_ICON_XDR        , CR_WS(".xdr")                     },
    {   QST_ICON_XML        , CR_WS(".xml")                     },
    {   QST_ICON_XSD        , CR_WS(".xsd")                     },
    {   QST_ICON_XSL        , CR_WS(".xsl")                     },
    {   QST_ICON_XSL        , CR_WS(".xslt")                    },
    {   QST_ICON_PCB        , CR_WS(".pcb")                     },
    {   QST_ICON_PCB        , CR_WS(".pcbdoc")                  },
    {   QST_ICON_SCH        , CR_WS(".sch")                     },
    {   QST_ICON_SCH        , CR_WS(".schdoc")                  },
    {   QST_ICON_SCH        , CR_WS(".schdot")                  },
    {   QST_ICON_PCBLIB     , CR_WS(".pcblib")                  },
    {   QST_ICON_SCHLIB     , CR_WS(".schlib")                  },
    {   QST_ICON_AUDIO      , CR_WS(".wav")                     },
    {   QST_ICON_AUDIO      , CR_WS(".mpa")                     },
    {   QST_ICON_AUDIO      , CR_WS(".mp2")                     },
    {   QST_ICON_AUDIO      , CR_WS(".m1a")                     },
    {   QST_ICON_AUDIO      , CR_WS(".m2a")                     },
    {   QST_ICON_AUDIO      , CR_WS(".mp3")                     },
    {   QST_ICON_AUDIO      , CR_WS(".ogg")                     },
    {   QST_ICON_AUDIO      , CR_WS(".m4a")                     },
    {   QST_ICON_AUDIO      , CR_WS(".aac")                     },
    {   QST_ICON_AUDIO      , CR_WS(".mka")                     },
    {   QST_ICON_AUDIO      , CR_WS(".mpc")                     },
    {   QST_ICON_AUDIO      , CR_WS(".mod")                     },
    {   QST_ICON_AUDIO      , CR_WS(".eac3")                    },
    {   QST_ICON_AUDIO      , CR_WS(".tak")                     },
    {   QST_ICON_AUDIO      , CR_WS(".aiff")                    },
    {   QST_ICON_AUDIO      , CR_WS(".aif")                     },
    {   QST_ICON_AUDIO      , CR_WS(".aifc")                    },
    {   QST_ICON_AUDIO      , CR_WS(".opus")                    },
    {   QST_ICON_AUDIO      , CR_WS(".m4b")                     },
    {   QST_ICON_AUDIO      , CR_WS(".aud")                     },
    {   QST_ICON_AUDIO      , CR_WS(".snd")                     },
    {   QST_ICON_AUDIO      , CR_WS(".au")                      },
    {   QST_ICON_AUDIO      , CR_WS(".it")                      },
    {   QST_ICON_AVI        , CR_WS(".avi")                     },
    {   QST_ICON_CD         , CR_WS(".cda")                     },
    {   QST_ICON_DVD        , CR_WS(".vob")                     },
    {   QST_ICON_DVD        , CR_WS(".ifo")                     },
    {   QST_ICON_DVD        , CR_WS(".evo")                     },
    {   QST_ICON_DVDA       , CR_WS(".ac3")                     },
    {   QST_ICON_DVDA       , CR_WS(".dts")                     },
    {   QST_ICON_DVDA       , CR_WS(".dtshd")                   },
    {   QST_ICON_FLV        , CR_WS(".flv")                     },
    {   QST_ICON_LOSSLESS   , CR_WS(".ape")                     },
    {   QST_ICON_LOSSLESS   , CR_WS(".flac")                    },
    {   QST_ICON_LOSSLESS   , CR_WS(".wv")                      },
    {   QST_ICON_LOSSLESS   , CR_WS(".dsf")                     },
    {   QST_ICON_LOSSLESS   , CR_WS(".tta")                     },
    {   QST_ICON_MKV        , CR_WS(".mkv")                     },
    {   QST_ICON_MP4        , CR_WS(".mp4")                     },
    {   QST_ICON_MP4        , CR_WS(".mp4v")                    },
    {   QST_ICON_MP4        , CR_WS(".m4v")                     },
    {   QST_ICON_MP4        , CR_WS(".m4p")                     },
    {   QST_ICON_MPG        , CR_WS(".mpg")                     },
    {   QST_ICON_MPG        , CR_WS(".mpeg")                    },
    {   QST_ICON_MPG        , CR_WS(".mpe")                     },
    {   QST_ICON_MPG        , CR_WS(".mpv")                     },
    {   QST_ICON_MPG        , CR_WS(".m1v")                     },
    {   QST_ICON_MPG        , CR_WS(".m2v")                     },
    {   QST_ICON_MPG        , CR_WS(".mpv2")                    },
    {   QST_ICON_MPG        , CR_WS(".mp2v")                    },
    {   QST_ICON_OGM        , CR_WS(".ogm")                     },
    {   QST_ICON_PLAYLIST   , CR_WS(".asx")                     },
    {   QST_ICON_PLAYLIST   , CR_WS(".m3u")                     },
    {   QST_ICON_PLAYLIST   , CR_WS(".m3u8")                    },
    {   QST_ICON_PLAYLIST   , CR_WS(".m4u")                     },
    {   QST_ICON_PLAYLIST   , CR_WS(".pls")                     },
    {   QST_ICON_PLAYLIST   , CR_WS(".wvx")                     },
    {   QST_ICON_PLAYLIST   , CR_WS(".wax")                     },
    {   QST_ICON_PLAYLIST   , CR_WS(".wmx")                     },
    {   QST_ICON_PLAYLIST   , CR_WS(".cue")                     },
    {   QST_ICON_PLAYLIST   , CR_WS(".mpls")                    },
    {   QST_ICON_PLAYLIST   , CR_WS(".mpl")                     },
    {   QST_ICON_PLAYLIST   , CR_WS(".dpl")                     },
    {   QST_ICON_PLAYLIST   , CR_WS(".xspf")                    },
    {   QST_ICON_QUICKTIME  , CR_WS(".amr")                     },
    {   QST_ICON_QUICKTIME  , CR_WS(".mov")                     },
    {   QST_ICON_QUICKTIME  , CR_WS(".moov")                    },
    {   QST_ICON_QUICKTIME  , CR_WS(".3gp")                     },
    {   QST_ICON_QUICKTIME  , CR_WS(".3gpp")                    },
    {   QST_ICON_QUICKTIME  , CR_WS(".3g2")                     },
    {   QST_ICON_QUICKTIME  , CR_WS(".3gp2")                    },
    {   QST_ICON_QUICKTIME  , CR_WS(".qt")                      },
    {   QST_ICON_QUICKTIME  , CR_WS(".qtm")                     },
    {   QST_ICON_REAL       , CR_WS(".ra")                      },
    {   QST_ICON_REAL       , CR_WS(".rm")                      },
    {   QST_ICON_REAL       , CR_WS(".ram")                     },
    {   QST_ICON_REAL       , CR_WS(".rmvb")                    },
    {   QST_ICON_SWF        , CR_WS(".swf")                     },
    {   QST_ICON_TS         , CR_WS(".ts")                      },
    {   QST_ICON_TS         , CR_WS(".tp")                      },
    {   QST_ICON_TS         , CR_WS(".trp")                     },
    {   QST_ICON_TS         , CR_WS(".m2ts")                    },
    {   QST_ICON_TS         , CR_WS(".m2t")                     },
    {   QST_ICON_TS         , CR_WS(".mts")                     },
    {   QST_ICON_VIDEO      , CR_WS(".tpr")                     },
    {   QST_ICON_VIDEO      , CR_WS(".ogv")                     },
    {   QST_ICON_VIDEO      , CR_WS(".nsv")                     },
    {   QST_ICON_VIDEO      , CR_WS(".dpg")                     },
    {   QST_ICON_VIDEO      , CR_WS(".dvr-ms")                  },
    {   QST_ICON_VIDEO      , CR_WS(".k3g")                     },
    {   QST_ICON_VIDEO      , CR_WS(".skm")                     },
    {   QST_ICON_VIDEO      , CR_WS(".nsr")                     },
    {   QST_ICON_VIDEO      , CR_WS(".amv")                     },
    {   QST_ICON_VIDEO      , CR_WS(".divx")                    },
    {   QST_ICON_VIDEO      , CR_WS(".wtv")                     },
    {   QST_ICON_VIDEO      , CR_WS(".f4v")                     },
    {   QST_ICON_VIDEO      , CR_WS(".mxf")                     },
    {   QST_ICON_WEBM       , CR_WS(".webm")                    },
    {   QST_ICON_WMA        , CR_WS(".wma")                     },
    {   QST_ICON_WMV        , CR_WS(".wmv")                     },
    {   QST_ICON_WMV        , CR_WS(".wmp")                     },
    {   QST_ICON_WMV        , CR_WS(".wm")                      },
    {   QST_ICON_WMV        , CR_WS(".asf")                     },
    {   QST_ICON_PY         , CR_WS(".py")                      },
    {   QST_ICON_PY         , CR_WS(".pyw")                     },
    {   QST_ICON_PYC        , CR_WS(".pyc")                     },
    {   QST_ICON_PYC        , CR_WS(".pyo")                     },
    {   QST_ICON_PYD        , CR_WS(".pyd")                     },
    {   QST_ICON_7Z         , CR_WS(".7z")                      },
    {   QST_ICON_7Z         , CR_WS(".lzma")                    },
    {   QST_ICON_7Z         , CR_WS(".xz")                      },
    {   QST_ICON_7Z         , CR_WS(".txz")                     },
    {   QST_ICON_ZIP        , CR_WS(".zip")                     },
    {   QST_ICON_BZ2        , CR_WS(".bz2")                     },
    {   QST_ICON_BZ2        , CR_WS(".bzip2")                   },
    {   QST_ICON_BZ2        , CR_WS(".tbz")                     },
    {   QST_ICON_BZ2        , CR_WS(".tbz2")                    },
    {   QST_ICON_RAR        , CR_WS(".rar")                     },
    {   QST_ICON_ARJ        , CR_WS(".arj")                     },
    {   QST_ICON_Z          , CR_WS(".z")                       },
    {   QST_ICON_Z          , CR_WS(".taz")                     },
    {   QST_ICON_CAB        , CR_WS(".cab")                     },
    {   QST_ICON_SPLIT      , CR_WS(".001")                     },
    {   QST_ICON_RPM        , CR_WS(".rpm")                     },
    {   QST_ICON_DEB        , CR_WS(".deb")                     },
    {   QST_ICON_CPIO       , CR_WS(".cpio")                    },
    {   QST_ICON_TAR        , CR_WS(".tar")                     },
    {   QST_ICON_GZ         , CR_WS(".gz")                      },
    {   QST_ICON_GZ         , CR_WS(".gzip")                    },
    {   QST_ICON_GZ         , CR_WS(".tgz")                     },
    {   QST_ICON_GZ         , CR_WS(".tpz")                     },
    {   QST_ICON_WIM        , CR_WS(".wim")                     },
    {   QST_ICON_WIM        , CR_WS(".swm")                     },
    {   QST_ICON_LZH        , CR_WS(".lha")                     },
    {   QST_ICON_LZH        , CR_WS(".lzh")                     },
    {   QST_ICON_DMG        , CR_WS(".dmg")                     },
    {   QST_ICON_HFS        , CR_WS(".hfs")                     },
    {   QST_ICON_HFS        , CR_WS(".squashfs")                },
    {   QST_ICON_XAR        , CR_WS(".xar")                     },
    {   QST_ICON_BMP        , CR_WS(".bmp")                     },
    {   QST_ICON_BMP        , CR_WS(".bmr")                     },
    {   QST_ICON_BMP        , CR_WS(".dib")                     },
    {   QST_ICON_BMP        , CR_WS(".emf")                     },
    {   QST_ICON_BMP        , CR_WS(".ico")                     },
    {   QST_ICON_BMP        , CR_WS(".pcx")                     },
    {   QST_ICON_BMP        , CR_WS(".pbm")                     },
    {   QST_ICON_BMP        , CR_WS(".pgm")                     },
    {   QST_ICON_BMP        , CR_WS(".ppm")                     },
    {   QST_ICON_BMP        , CR_WS(".pnm")                     },
    {   QST_ICON_BMP        , CR_WS(".pic")                     },
    {   QST_ICON_BMP        , CR_WS(".pict")                    },
    {   QST_ICON_BMP        , CR_WS(".pct")                     },
    {   QST_ICON_BMP        , CR_WS(".xbm")                     },
    {   QST_ICON_BMP        , CR_WS(".xpm")                     },
    {   QST_ICON_BMP        , CR_WS(".dcr")                     },
    {   QST_ICON_GIF        , CR_WS(".gif")                     },
    {   QST_ICON_GIF        , CR_WS(".png")                     },
    {   QST_ICON_GIF        , CR_WS(".tga")                     },
    {   QST_ICON_GIF        , CR_WS(".vda")                     },
    {   QST_ICON_GIF        , CR_WS(".icb")                     },
    {   QST_ICON_GIF        , CR_WS(".vst")                     },
    {   QST_ICON_GIF        , CR_WS(".targa")                   },
    {   QST_ICON_GIF        , CR_WS(".sun")                     },
    {   QST_ICON_GIF        , CR_WS(".ras")                     },
    {   QST_ICON_GIF        , CR_WS(".im1")                     },
    {   QST_ICON_GIF        , CR_WS(".im8")                     },
    {   QST_ICON_GIF        , CR_WS(".im24")                    },
    {   QST_ICON_GIF        , CR_WS(".im32")                    },
    {   QST_ICON_GIF        , CR_WS(".iff")                     },
    {   QST_ICON_GIF        , CR_WS(".lbm")                     },
    {   QST_ICON_GIF        , CR_WS(".ilbm")                    },
    {   QST_ICON_GIF        , CR_WS(".sgi")                     },
    {   QST_ICON_GIF        , CR_WS(".bw")                      },
    {   QST_ICON_GIF        , CR_WS(".rgb")                     },
    {   QST_ICON_GIF        , CR_WS(".rgba")                    },
    {   QST_ICON_GIF        , CR_WS(".cgr")                     },
    {   QST_ICON_GIF        , CR_WS(".argb")                    },
    {   QST_ICON_JPG        , CR_WS(".jpg")                     },
    {   QST_ICON_JPG        , CR_WS(".jpeg")                    },
    {   QST_ICON_JPG        , CR_WS(".jpe")                     },
    {   QST_ICON_JPG        , CR_WS(".jif")                     },
    {   QST_ICON_JPG        , CR_WS(".jfif")                    },
    {   QST_ICON_JPG        , CR_WS(".jp2")                     },
    {   QST_ICON_JPG        , CR_WS(".j2k")                     },
    {   QST_ICON_JPG        , CR_WS(".j2c")                     },
    {   QST_ICON_JPG        , CR_WS(".jng")                     },
    {   QST_ICON_JPG        , CR_WS(".jxr")                     },
    {   QST_ICON_JPG        , CR_WS(".wdp")                     },
    {   QST_ICON_JPG        , CR_WS(".hdp")                     },
    {   QST_ICON_TIF        , CR_WS(".dds")                     },
    {   QST_ICON_TIF        , CR_WS(".pcd")                     },
    {   QST_ICON_TIF        , CR_WS(".psd")                     },
    {   QST_ICON_TIF        , CR_WS(".tif")                     },
    {   QST_ICON_TIF        , CR_WS(".tiff")                    },
    {   QST_ICON_TIF        , CR_WS(".webp")                    },
    {   QST_ICON_PDF        , CR_WS(".pdf")                     },
    {   QST_ICON_MDB        , CR_WS(".mdb")                     },
    {   QST_ICON_ACCDB      , CR_WS(".accdb")                   },
    {   QST_ICON_DOC        , CR_WS(".doc")                     },
    {   QST_ICON_DOCX       , CR_WS(".docx")                    },
    {   QST_ICON_PPT        , CR_WS(".ppt")                     },
    {   QST_ICON_PPTX       , CR_WS(".pptx")                    },
    {   QST_ICON_XLS        , CR_WS(".xls")                     },
    {   QST_ICON_XLSX       , CR_WS(".xlsx")                    },
    {   QST_ICON_MARKDOWN   , CR_WS(".md")                      },
    {   QST_ICON_MARKDOWN   , CR_WS(".markdown")                },
    {   QST_ICON_SH         , CR_WS(".sh")                      },
    {   QST_ICON_CHM        , CR_WS(".chm")                     },
    {   QST_ICON_CER        , CR_WS(".cer")                     },
    {   QST_ICON_CER        , CR_WS(".crt")                     },
    {   QST_ICON_CER        , CR_WS(".der")                     },
    {   QST_ICON_CER        , CR_WS(".p7b")                     },
    {   QST_ICON_CER        , CR_WS(".p7r")                     },
    {   QST_ICON_CER        , CR_WS(".spc")                     },
    {   QST_ICON_CER        , CR_WS(".certs")                   },
    {   QST_ICON_PAL        , CR_WS(".pal")                     },
    {   QST_ICON_RC         , CR_WS(".rc")                      },
    {   QST_ICON_RC         , CR_WS(".rc2")                     },
    {   QST_ICON_RCT        , CR_WS(".rct")                     },
    {   QST_ICON_RCT        , CR_WS(".rll")                     },
    {   QST_ICON_RES        , CR_WS(".res")                     },
    {   QST_ICON_RESX       , CR_WS(".resx")                    },
    {   QST_ICON_SLN60      , CR_WS("._sln60")                  },
    {   QST_ICON_SLN70      , CR_WS("._sln70")                  },
    {   QST_ICON_SLN71      , CR_WS("._sln71")                  },
    {   QST_ICON_SLN80      , CR_WS("._sln80")                  },
    {   QST_ICON_SLN90      , CR_WS("._sln90")                  },
    {   QST_ICON_SLN100     , CR_WS("._sln100")                 },
    {   QST_ICON_A          , CR_WS(".a")                       },
    {   QST_ICON_A          , CR_WS(".dlb")                     },
    {   QST_ICON_ACA        , CR_WS(".aca")                     },
    {   QST_ICON_ACA        , CR_WS(".acf")                     },
    {   QST_ICON_ACA        , CR_WS(".acs")                     },
    {   QST_ICON_ASAX       , CR_WS(".asax")                    },
    {   QST_ICON_ASAX       , CR_WS(".asa")                     },
    {   QST_ICON_ASCX       , CR_WS(".ascx")                    },
    {   QST_ICON_ASHX       , CR_WS(".ashx")                    },
    {   QST_ICON_ASMX       , CR_WS(".asmx")                    },
    {   QST_ICON_ASMX       , CR_WS(".svc")                     },
    {   QST_ICON_ASP        , CR_WS(".asp")                     },
    {   QST_ICON_ASP        , CR_WS(".cdx")                     },
    {   QST_ICON_ASPX       , CR_WS(".aspx")                    },
    {   QST_ICON_ASPX       , CR_WS(".master")                  },
    {   QST_ICON_HEX        , CR_WS(".hex")                     },
    {   QST_ICON_HEX        , CR_WS(".s19")                     },
    {   QST_ICON_HEX        , CR_WS(".s28")                     },
    {   QST_ICON_HEX        , CR_WS(".s37")                     },
    {   QST_ICON_CMD        , CR_WS(".cmd")                     },
    {   QST_ICON_CSDPROJ    , CR_WS(".csdproj")                 },
    {   QST_ICON_CSHTML     , CR_WS(".cshtml")                  },
    {   QST_ICON_CSPROJ     , CR_WS(".csproj")                  },
    {   QST_ICON_CUR        , CR_WS(".cur")                     },
    {   QST_ICON_D88        , CR_WS(".d88")                     },
    {   QST_ICON_D88        , CR_WS(".hdm")                     },
    {   QST_ICON_DMP        , CR_WS(".dmp")                     },
    {   QST_ICON_DMP        , CR_WS(".hdmp")                    },
    {   QST_ICON_DMP        , CR_WS(".mdmp")                    },
    {   QST_ICON_DSP        , CR_WS(".dsp")                     },
    {   QST_ICON_DSP        , CR_WS(".vcp")                     },
    {   QST_ICON_DSP        , CR_WS(".vcproj")                  },
    {   QST_ICON_DSW        , CR_WS(".dsw")                     },
    {   QST_ICON_DSW        , CR_WS(".vcw")                     },
    {   QST_ICON_DSW        , CR_WS(".mdp")                     },
    {   QST_ICON_DTD        , CR_WS(".dtd")                     },
    {   QST_ICON_EXE        , CR_WS(".exe")                     },
    {   QST_ICON_EXP        , CR_WS(".exp")                     },
    {   QST_ICON_FDI        , CR_WS(".fdi")                     },
    {   QST_ICON_FDI        , CR_WS(".ddi")                     },
    {   QST_ICON_FILTERS    , CR_WS(".filters")                 },
    {   QST_ICON_FS         , CR_WS(".fs")                      },
    {   QST_ICON_FSI        , CR_WS(".fsi")                     },
    {   QST_ICON_FSPROJ     , CR_WS(".fsproj")                  },
    {   QST_ICON_FSX        , CR_WS(".fsx")                     },
    {   QST_ICON_FSX        , CR_WS(".fsscript")                },
    {   QST_ICON_HTM        , CR_WS(".htm")                     },
    {   QST_ICON_HTM        , CR_WS(".html")                    },
    {   QST_ICON_ICC        , CR_WS(".icc")                     },
    {   QST_ICON_ICC        , CR_WS(".icm")                     },
    {   QST_ICON_MANIFEST   , CR_WS(".manifest")                },
    {   QST_ICON_MHT        , CR_WS(".mht")                     },
    {   QST_ICON_MHT        , CR_WS(".mhtml")                   },
    {   QST_ICON_MID        , CR_WS(".mid")                     },
    {   QST_ICON_MID        , CR_WS(".midi")                    },
    {   QST_ICON_O          , CR_WS(".o")                       },
    {   QST_ICON_O          , CR_WS(".doj")                     },
    {   QST_ICON_O          , CR_WS(".ddp")                     },
    {   QST_ICON_PCH        , CR_WS(".pch")                     },
    {   QST_ICON_PIF        , CR_WS(".pif")                     },
    {   QST_ICON_PS1        , CR_WS(".ps1")                     },
    {   QST_ICON_REG        , CR_WS(".reg")                     },
    {   QST_ICON_REG        , CR_WS(".key")                     },
    {   QST_ICON_S          , CR_WS(".s")                       },
    {   QST_ICON_SLN        , CR_WS(".sln")                     },
    {   QST_ICON_SO         , CR_WS(".so")                      },
    {   QST_ICON_SO         , CR_WS(".axf")                     },
    {   QST_ICON_THEME      , CR_WS(".theme")                   },
    {   QST_ICON_THEME      , CR_WS(".msstyles")                },
    {   QST_ICON_TTC        , CR_WS(".ttc")                     },
    {   QST_ICON_USER       , CR_WS(".user")                    },
    {   QST_ICON_USER       , CR_WS(".suo")                     },
    {   QST_ICON_USER       , CR_WS(".vssettings")              },
    {   QST_ICON_JSE        , CR_WS(".js")                      },
    {   QST_ICON_JSE        , CR_WS(".jse")                     },
    {   QST_ICON_JSE        , CR_WS(".sct")                     },
    {   QST_ICON_VBDPROJ    , CR_WS(".vbdproj")                 },
    {   QST_ICON_VBHTML     , CR_WS(".vbhtml")                  },
    {   QST_ICON_VBPROJ     , CR_WS(".vbproj")                  },
    {   QST_ICON_VBS        , CR_WS(".vbs")                     },
    {   QST_ICON_VBS        , CR_WS(".vbe")                     },
    {   QST_ICON_VBS        , CR_WS(".vba")                     },
    {   QST_ICON_VCXPROJ    , CR_WS(".vcxproj")                 },
    {   QST_ICON_VDP        , CR_WS(".vdp")                     },
    {   QST_ICON_VDP        , CR_WS(".vdproj")                  },
    {   QST_ICON_VSPROPS    , CR_WS(".props")                   },
    {   QST_ICON_VSPROPS    , CR_WS(".vsprops")                 },
    {   QST_ICON_WSH        , CR_WS(".wsh")                     },
    {   QST_ICON_WSH        , CR_WS(".wsf")                     },
    {   QST_ICON_WSH        , CR_WS(".wsc")                     },
    {   QST_ICON_XAML       , CR_WS(".xaml")                    },
    {   QST_ICON_XAMLX      , CR_WS(".xamlx")                   },
    {   QST_ICON_XBAP       , CR_WS(".xbap")                    },
    {   QST_ICON_BPR        , CR_WS(".bpr")                     },
    {   QST_ICON_DCU        , CR_WS(".dcu")                     },
    {   QST_ICON_DFM        , CR_WS(".dfm")                     },
    {   QST_ICON_DPK        , CR_WS(".dpk")                     },
    {   QST_ICON_DPK        , CR_WS(".bpk")                     },
    {   QST_ICON_DPR        , CR_WS(".dpr")                     },
    {   QST_ICON_DPR        , CR_WS(".bpg")                     },
    {   QST_ICON_PAS        , CR_WS(".pas")                     },
    {   QST_ICON_1SC        , CR_WS(".1sc")                     },
    {   QST_ICON_1SC        , CR_WS(".enc")                     },
    {   QST_ICON_BT         , CR_WS(".bt")                      },
    {   QST_ICON_BT         , CR_WS(".msg")                     },
    {   QST_ICON_BDSPROJ    , CR_WS(".bdsproj")                 },
    {   QST_ICON_BDSGROUP   , CR_WS(".bdsgroup")                },
    {   QST_ICON_BDSGROUP   , CR_WS(".groupproj")               },
    {   QST_ICON_BPL        , CR_WS(".bpl")                     },
    {   QST_ICON_CBPROJ     , CR_WS(".cbproj")                  },
    {   QST_ICON_BPF        , CR_WS(".bpf")                     },
    {   QST_ICON_BPI        , CR_WS(".bpi")                     },
    {   QST_ICON_BPI        , CR_WS(".tlb")                     },
    {   QST_ICON_SYS        , CR_WS(".sys")                     },
    {   QST_ICON_SYS        , CR_WS(".ko")                      },
    {   QST_ICON_CLP        , CR_WS(".clp")                     },
    {   QST_ICON_DIFF       , CR_WS(".diff")                    },
    {   QST_ICON_DIFF       , CR_WS(".patch")                   },
    {   QST_ICON_HHP        , CR_WS(".hhp")                     },
    {   QST_ICON_ROM        , CR_WS(".rom")                     },
    {   QST_ICON_HTA        , CR_WS(".hta")                     },
    {   QST_ICON_HTA        , CR_WS(".htc")                     },
    {   QST_ICON_HTA        , CR_WS(".htaccess")                },
    {   QST_ICON_HTT        , CR_WS(".htt")                     },
    {   QST_ICON_HXA        , CR_WS(".hxa")                     },
    {   QST_ICON_HXA        , CR_WS(".hxw")                     },
    {   QST_ICON_HXC        , CR_WS(".hxc")                     },
    {   QST_ICON_HXD        , CR_WS(".hxd")                     },
    {   QST_ICON_HXD        , CR_WS(".hxf")                     },
    {   QST_ICON_HXD        , CR_WS(".hxh")                     },
    {   QST_ICON_HXD        , CR_WS(".hxi")                     },
    {   QST_ICON_HXD        , CR_WS(".hxq")                     },
    {   QST_ICON_HXD        , CR_WS(".hxr")                     },
    {   QST_ICON_HXE        , CR_WS(".hxe")                     },
    {   QST_ICON_HXK        , CR_WS(".hxk")                     },
    {   QST_ICON_HXS        , CR_WS(".hxs")                     },
    {   QST_ICON_HXT        , CR_WS(".hxt")                     },
    {   QST_ICON_HXV        , CR_WS(".hxv")                     },
    {   QST_ICON_REV        , CR_WS(".rev")                     },
    {   QST_ICON_386        , CR_WS(".386")                     },
    {   QST_ICON_ACCESSOR   , CR_WS(".accessor")                },
    {   QST_ICON_ACCESSOR   , CR_WS(".access")                  },
    {   QST_ICON_ACW        , CR_WS(".acw")                     },
    {   QST_ICON_ADDIN      , CR_WS(".addin")                   },
    {   QST_ICON_BOM        , CR_WS(".bom")                     },
    {   QST_ICON_CAT        , CR_WS(".cat")                     },
    {   QST_ICON_CRL        , CR_WS(".crl")                     },
    {   QST_ICON_CSV        , CR_WS(".csv")                     },
    {   QST_ICON_DB         , CR_WS(".db")                      },
    {   QST_ICON_DB         , CR_WS(".rdb")                     },
    {   QST_ICON_DB         , CR_WS(".db3")                     },
    {   QST_ICON_DB         , CR_WS(".dbf")                     },
    {   QST_ICON_DB         , CR_WS(".wdb")                     },
    {   QST_ICON_DB         , CR_WS(".sqlite")                  },
    {   QST_ICON_DDB        , CR_WS(".ddb")                     },
    {   QST_ICON_DRC        , CR_WS(".drc")                     },
    {   QST_ICON_DRV        , CR_WS(".drv")                     },
    {   QST_ICON_EML        , CR_WS(".eml")                     },
    {   QST_ICON_IDB        , CR_WS(".idb")                     },
    {   QST_ICON_ITS        , CR_WS(".its")                     },
    {   QST_ICON_ITS        , CR_WS(".itss")                    },
    {   QST_ICON_IVF        , CR_WS(".ivf")                     },
    {   QST_ICON_MSI        , CR_WS(".msi")                     },
    {   QST_ICON_MSI        , CR_WS(".msp")                     },
    {   QST_ICON_MTX        , CR_WS(".mtx")                     },
    {   QST_ICON_PCB3D      , CR_WS(".pcb3d")                   },
    {   QST_ICON_RDP        , CR_WS(".rdp")                     },
    {   QST_ICON_STL        , CR_WS(".stl")                     },
    {   QST_ICON_STL        , CR_WS(".lic")                     },
    {   QST_ICON_SVCLOG     , CR_WS(".svclog")                  },
    {   QST_ICON_SYNC       , CR_WS(".sync")                    },
    {   QST_ICON_TAB        , CR_WS(".tab")                     },
    {   QST_ICON_TAB        , CR_WS(".tbl")                     },
    {   QST_ICON_VSI        , CR_WS(".vsi")                     },
    {   QST_ICON_VSIX       , CR_WS(".vsix")                    },
    {   QST_ICON_MSU        , CR_WS(".msu")                     },
    {   QST_ICON_286        , CR_WS(".286")                     },
    {   QST_ICON_BIN        , CR_WS(".bin")                     },
    {   QST_ICON_BIN        , CR_WS(".tds")                     },
    {   QST_ICON_DAT        , CR_WS(".dat")                     },
    {   QST_ICON_ISU        , CR_WS(".isu")                     },
    {   QST_ICON_OCX        , CR_WS(".ocx")                     },
    {   QST_ICON_SCR        , CR_WS(".scr")                     },
    {   QST_ICON_VXD        , CR_WS(".vxd")                     },
    {   QST_ICON_AX         , CR_WS(".ax")                      },
    {   QST_ICON_AX         , CR_WS(".acm")                     },
    {   QST_ICON_IME        , CR_WS(".ime")                     },
    {   QST_ICON_MO         , CR_WS(".mo")                      },
    {   QST_ICON_PO         , CR_WS(".po")                      },
    {   QST_ICON_MUI        , CR_WS(".mui")                     },
    {   QST_ICON_NLS        , CR_WS(".nls")                     },
    {   QST_ICON_NLS        , CR_WS(".lng")                     },
    {   QST_ICON_MSC        , CR_WS(".msc")                     },
    {   QST_ICON_CPL        , CR_WS(".cpl")                     },
    {   QST_ICON_CONFIG     , CR_WS(".config")                  },
    {   QST_ICON_CONFIG     , CR_WS(".policy")                  },
    {   QST_ICON_DLANG      , CR_WS(".d")                       },
    {   QST_ICON_DLANG      , CR_WS(".di")                      },
    {   QST_ICON_JAR        , CR_WS(".jar")                     },
    {   QST_ICON_JMOD       , CR_WS(".jmod")                    },
    {   QST_ICON_JAVA       , CR_WS(".java")                    },
    {   QST_ICON_CLASS      , CR_WS(".class")                   },
    {   QST_ICON_JSON       , CR_WS(".json")                    },
    {   QST_ICON_PNDR       , CR_WS(".pndr")                    },
    {   QST_ICON_TCL        , CR_WS(".tcl")                     },
    {   QST_ICON_TCLM       , CR_WS(".tm")                      },
    {   QST_ICON_LUA        , CR_WS(".lua")                     },
    {   QST_ICON_LUA        , CR_WS(".wlua")                    },
    {   QST_ICON_PERL       , CR_WS(".pl")                      },
    {   QST_ICON_PERLM      , CR_WS(".pm")                      },
    {   QST_ICON_PERLM      , CR_WS(".pod")                     },
    {   QST_ICON_YAML       , CR_WS(".yml")                     },
    {   QST_ICON_YAML       , CR_WS(".yaml")                    },
    {   QST_ICON_CSS        , CR_WS(".css")                     },
    {   QST_ICON_CMAKE      , CR_WS(".cmake")                   },
    {   QST_ICON_CMAKE      , CR_WS(".ctest")                   },
    {   QST_ICON_ILAB       , CR_WS(".ilab")                    },
    {   QST_ICON_QST        , CR_WS(".qst")                     },
    {   QST_ICON_SVG        , CR_WS(".svg")                     },
    {   QST_ICON_GOLANG     , CR_WS(".go")                      },
    {   QST_ICON_RUST       , CR_WS(".rs")                      },
    {   QST_ICON_F2D        , CR_WS(".f2d")                     },
    {   QST_ICON_RLANG      , CR_WS(".r")                       },
    {   QST_ICON_RLANG      , CR_WS(".rsource")                 },
    {   QST_ICON_RDX        , CR_WS(".rdx")                     },
    {   QST_ICON_RDS        , CR_WS(".rds")                     },
    {   QST_ICON_SAVE       , CR_WS(".sav")                     },
    {   QST_ICON_SAVE       , CR_WS(".save")                    },
    {   QST_ICON_SAVE       , CR_WS(".out")                     },
    {   QST_ICON_SNIPPET    , CR_WS(".snippet")                 },
    {   QST_ICON_BASH       , CR_WS(".bash")                    },
    {   QST_ICON_PACK       , CR_WS(".pak")                     },
    {   QST_ICON_PACK       , CR_WS(".spak")                    },
    {   QST_ICON_PACK       , CR_WS(".spack")                   },
    {   QST_ICON_PACK       , CR_WS(".pack")                    },
    {   QST_ICON_PACK       , CR_WS(".na")                      },
    {   QST_ICON_PACK       , CR_WS(".xp3")                     },
    {   QST_ICON_PACK       , CR_WS(".nsa")                     },
    {   QST_ICON_PACK       , CR_WS(".sar")                     },
    {   QST_ICON_PACK       , CR_WS(".asar")                    },
    {   QST_ICON_PACK       , CR_WS(".pk")                      },
    {   QST_ICON_PACK       , CR_WS(".cpk")                     },
    {   QST_ICON_PACK       , CR_WS(".pac")                     },
    {   QST_ICON_PACK       , CR_WS(".pck")                     },
    {   QST_ICON_PACK       , CR_WS(".fpk")                     },
    {   QST_ICON_PACK       , CR_WS(".npk")                     },
    {   QST_ICON_PACK       , CR_WS(".arc")                     },
    {   QST_ICON_PACK       , CR_WS(".jbf")                     },
    {   QST_ICON_PACK       , CR_WS(".tsw")                     },
    {   QST_ICON_PACK       , CR_WS(".lmf")                     },
    {   QST_ICON_PACK       , CR_WS(".tpf")                     },
    {   QST_ICON_PACK       , CR_WS(".tpk")                     },
    {   QST_ICON_INDEX      , CR_WS(".idx")                     },
    {   QST_ICON_INDEX      , CR_WS(".dir")                     },
    {   QST_ICON_INDEX      , CR_WS(".index")                   },
    {   QST_ICON_INDEX      , CR_WS(".ni")                      },
    {   QST_ICON_TMX        , CR_WS(".tmx")                     },
    {   QST_ICON_VER        , CR_WS(".ver")                     },
    {   QST_ICON_VER        , CR_WS(".version")                 },
    {   QST_ICON_PRJ        , CR_WS(".prj")                     },
    {   QST_ICON_BXRC       , CR_WS(".bxrc")                    },
    {   QST_ICON_RUBY       , CR_WS(".rb")                      },
    {   QST_ICON_RUBY       , CR_WS(".rbw")                     },
    {   QST_ICON_RUBY       , CR_WS(".rake")                    },
    {   QST_ICON_RUBY       , CR_WS(".rjs")                     },
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
        if (curbead_findT(&s_wrk_ctx.icon, sQTEE_icon,
                s_ext_list[idx].ext) != NULL) {
            MessageBoxW(NULL, L"Repeat file EXT !!!",
                        (wchar_t*)s_ext_list[idx].ext,
                        MB_OK | MB_ICONINFORMATION);
        }
        if (curbead_insertT(&s_wrk_ctx.icon, sQTEE_icon,
                s_ext_list[idx].ext, &s_ext_list[idx]) == NULL) {
            return (QST_ERROR);
        }
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
