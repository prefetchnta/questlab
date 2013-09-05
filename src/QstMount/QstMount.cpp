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
 *             ##       CREATE: 2012-09-13
 *              #
 ================================================
        QuestLAB 资源服务器
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 应用的名称 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstMount"
    #define WIN_TITLE   "QstMount"
    #define WIN_CLASS   "QstMountCLSS"
    #define WIN_ICONF   "QstMount.ini"
    #define WIN_XCONF   "QstMount.xml"
#endif

/* 工作上下文结构 */
typedef struct
{
        /* 非实时参数 */
        HWND        hwnd;   /* 窗口句柄 */
        HANDLE      hcui;   /* 输出句柄 */
        bool_t      quit;   /* 是否退出 */
        leng_t      head;   /* 路径开头 */
        sARRAY      extz;   /* 插件列表 */
        sARRAY      paks;   /* 封包列表 */
        socket_t    netw;   /* 网络连接 */
        sCURBEAD    list;   /* 挂载列表 */
        sCURBEAD    resx;   /* 资源列表 */

        /* 远程资源管理 */
        share_t     smem;   /* 当前内存共享文件 */

} sQstMount;

/* 缺省的窗口大小 */
#define QMNT_DEF_WIDTH  668
#define QMNT_DEF_HEIGHT 434

/* 全局工作上下文 */
static sQstMount    s_wrk_ctx;

/* 挂载文件的类型 */
#define QST_MOUNT_DISK  0   /* 磁盘文件 */
#define QST_MOUNT_TEMP  1   /* 临时文件 */

/* 文件资源节点结构 */
typedef struct
{
        leng_t  size;   /* 共享文件大小 */
        ansi_t* name;   /* 文件标识名称 */
        ansi_t* file;   /* 资源的文件名 */
        share_t objs;   /* 内存共享文件 */

} sQstResNode;

/* 文件挂载节点结构 */
typedef struct
{
        uint_t      type;   /* 文件节点类型 */
        ansi_t*     name;   /* 挂载节点名 (释放) */
        ansi_t*     show;   /* 节点显示名 (释放) */
        ansi_t*     file;   /* 磁盘文件名 (释放) */
        void_t*     data;   /* 内存数据 (可能释放) */
        sARRAY      info;   /* 文件信息列表 (释放) */
        iPACKAGE*   pack;   /* 封包读取接口 (不释放) */
        sFMT_PRT*   fmtz;   /* 封包 FMTZ 对象 (释放) */

} sQstMntNode;

/* 显示颜色配置 */
static int16u   s_color_text;   /* 标题条的颜色 */
static int16u   s_color_head;   /* 总标题的颜色 */
static int16u   s_color_disk;   /* 磁盘文件颜色 */
static int16u   s_color_temp;   /* 临时文件颜色 */
static int16u   s_color_smem;   /* 内存文件颜色 */

/* 当前工作状态设置宏 */
#define QST_SET_STATE_FREE \
    SetConsoleTitleA(WIN_TITLE);
#define QST_SET_STATE_BUSY \
    SetConsoleTitleA(WIN_TITLE " - Loading...");

/*
---------------------------------------
    释放信息列表
---------------------------------------
*/
static void_t
finfo_free (
  __CR_IN__ void_t* obj
    )
{
    ansi_t**    unit;

    unit = (ansi_t**)obj;
    mem_free(*unit);
}

/*
---------------------------------------
    挂载节点释放回调
---------------------------------------
*/
static void_t
mount_free (
  __CR_IN__ void_t* obj
    )
{
    sQstMntNode*    unit;

    unit = (sQstMntNode*)obj;
    array_freeT(&unit->info, ansi_t*);
    fmtz_free((sFMTZ*)unit->fmtz);
    TRY_FREE(unit->data)
    mem_free(unit->show);
    mem_free(unit->name);
    if (unit->type == QST_MOUNT_TEMP)
        file_deleteA(unit->file);
    mem_free(unit->file);
}

/*
---------------------------------------
    挂载节点比较回调
---------------------------------------
*/
static bool_t
mount_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    sQstMntNode*    unit = (sQstMntNode*)obj;

    return (str_cmpIA(unit->name, (ansi_t*)key) == 0);
}

/*
---------------------------------------
    挂载节点索引回调
---------------------------------------
*/
static uint_t
mount_find (
  __CR_IN__ const void_t*   key
    )
{
    uint_t  hash = 5381, cha;
    leng_t  size = str_lenA((ansi_t*)key);

    /* 忽略大小写的哈希计算 */
    for (; size != 0; size--) {
        cha = *(byte_t*)key;
        key =  (byte_t*)key + 1;
        if (is_upperA((ansi_t)cha))
            cha += 'a' - 'A';
        hash = cha + (hash << 5) + hash;
    }
    return (hash);
}

/*
---------------------------------------
    资源节点释放回调
---------------------------------------
*/
static void_t
resx_free (
  __CR_IN__ void_t* obj
    )
{
    sQstResNode*    unit;

    unit = (sQstResNode*)obj;
    if (unit->objs != NULL)
        share_file_close(unit->objs);
    TRY_FREE(unit->file)
    mem_free(unit->name);
}

/*
---------------------------------------
    资源节点比较回调
---------------------------------------
*/
static bool_t
resx_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    sQstResNode*    unit = (sQstResNode*)obj;

    return (str_cmpA(unit->name, (ansi_t*)key) == 0);
}

/*
---------------------------------------
    资源节点索引回调
---------------------------------------
*/
static uint_t
resx_find (
  __CR_IN__ const void_t*   key
    )
{
    return (hash_str_djb2(key, str_lenA((ansi_t*)key)));
}

/*
---------------------------------------
    FMTZ 插件释放回调
---------------------------------------
*/
static void_t
plugin_free (
  __CR_IN__ void_t* obj
    )
{
    sENGINE**   unit;

    unit = (sENGINE**)obj;
    engine_free(*unit);
}

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/*
---------------------------------------
    刷新资源显示
---------------------------------------
*/
static void_t
qst_refresh_resx (
  __CR_IN__ sCURBEAD*   tbl
    )
{
    if (tbl->__list__ == NULL)
        return;
    for (uint_t idx = 0; idx < tbl->__size__; idx++)
    {
        sLIST*      lst;
        sLST_UNIT*  node;

        lst = &tbl->__list__[idx];
        if (lst->__size__ == 0)
            continue;
        node = lst->__head__;
        do
        {
            sQstResNode*    info;

            info = list_get_dataT(node, sQstResNode);
            if (info->objs != NULL) {
                cui_set_color(s_color_head);
                printf("[%s] ", info->name);
                cui_set_color(s_color_text);
                printf("<%u Bytes> ", info->size);
                cui_set_color(s_color_smem);
                printf("%s\n", info->file);
            }
            node = node->next;
        } while (node != NULL);
    }
}

/*
---------------------------------------
    刷新挂载显示
---------------------------------------
*/
static void_t
qst_refresh_mount (
  __CR_IN__ sCURBEAD*   tbl
    )
{
    if (tbl->__list__ == NULL)
        return;
    for (uint_t idx = 0; idx < tbl->__size__; idx++)
    {
        sLIST*      lst;
        sLST_UNIT*  node;

        lst = &tbl->__list__[idx];
        if (lst->__size__ == 0)
            continue;
        node = lst->__head__;
        do
        {
            sQstMntNode*    info;

            cui_set_color(s_color_text);
            info = list_get_dataT(node, sQstMntNode);
            if (info->data != NULL) {
                if (info->type == QST_MOUNT_DISK)
                    printf("[Memory Disk File] ");
                else
                if (info->type == QST_MOUNT_TEMP)
                    printf("[Memory Temp File] ");
                cui_set_color(s_color_smem);
            }
            else
            if (info->type == QST_MOUNT_DISK) {
                printf("[Disk File] ");
                cui_set_color(s_color_disk);
            }
            else
            if (info->type == QST_MOUNT_TEMP) {
                printf("[Temp File] ");
                cui_set_color(s_color_temp);
            }
            printf("%s\n", info->show);
            node = node->next;
        } while (node != NULL);
    }
}

/*
---------------------------------------
    刷新列表显示
---------------------------------------
*/
static void_t
qst_refresh_list (
  __CR_IO__ sQstMount*  parm
    )
{
    system("cls");
    cui_set_color(s_color_head);
    printf("######################################\n");
    printf("##     QUESTLAB RESOURCE SERVER     ##\n");
    printf("######################################\n");
    qst_refresh_mount(&parm->list);
    cui_set_color(s_color_head);
    printf("--------------------------------------\n");
    qst_refresh_resx(&parm->resx);
}

/*
---------------------------------------
    发送文件信息
---------------------------------------
*/
static void_t
qst_send_finfo (
  __CR_IN__ sARRAY*     list,
  __CR_IN__ socket_t    netw
    )
{
    leng_t  cnts, idx;

    /* 清除前面的信息 */
    cmd_shl_send(netw, "txt:clear 0 2");

    /* 逐条发送文件信息 */
    cnts = array_get_sizeT(list, ansi_t*);
    for (idx = 0; idx < cnts; idx++)
    {
        ansi_t*     temp;
        ansi_t*     info;
        ansi_t**    unit;

        /* 转义转换后发给别人 */
        unit = array_get_unitT(list, ansi_t*, idx);
        temp = str_esc_makeU(*unit);
        if (temp == NULL)
            continue;
        info = str_fmtA("info::main=\"2> %s\"", temp);
        mem_free(temp);
        if (info != NULL) {
            cmd_ini_send(netw, info);
            mem_free(info);
        }
    }
}

/*
---------------------------------------
    输出文件列表文件
---------------------------------------
*/
static void_t
qst_out_flist (
  __CR_IO__ sQstMount*  parm,
  __CR_IN__ iPACKAGE*   pack,
  __CR_IN__ int32u      node,
  __CR_IN__ uint_t      page
    )
{
    FILE*   fp;
    int64u  idx;
    int64u  cnt;

    /* 输出标签头 */
    fp = fopen(QST_IMPORT_LST, "w");
    if (fp == NULL)
        return;
    fprintf(fp, "<!-- encoding:65001 -->\n");
    fprintf(fp, "<QstTree node=\"0x%08X\" page=\"%u\">\n", node, page);

    /* 输出子文件列表 */
    cnt = pack_file_num(pack);
    for (idx = 0; idx < cnt; idx++)
    {
        ansi_t*     esc;
        sPAK_FILE*  info;

        fprintf(fp, "<File ");
        pack_file_info(pack, &info, idx);
        fprintf(fp, "offs=\"%I64u\" ", info->offs);
        fprintf(fp, "pack=\"%I64u\" ", info->pack);
        fprintf(fp, "size=\"%I64u\" ", info->size);
        if (info->attr & PAK_FILE_CMP)
            fprintf(fp, "comp=\"1\" ");
        if (info->attr & PAK_FILE_ENC)
            fprintf(fp, "cryp=\"1\" ");
        if (info->memo != NULL) {
            esc = str_esc_makeU(info->memo);
            if (esc == NULL)
                goto _failure;
            fprintf(fp, "memo=\"%s\" ", esc);
            mem_free(esc);
        }
        esc = str_esc_makeU(info->name);
        if (esc == NULL)
            goto _failure;
        fprintf(fp, "path=\"%s\" />\n", esc);
        mem_free(esc);
    }

    /* 输出标签尾 */
    fprintf(fp, "</QstTree>\n");
    fclose(fp);

    /* 发加载列表命令 */
    cmd_shl_send(parm->netw, "dir:load");
    return;

_failure:
    fclose(fp);
    file_deleteA(QST_IMPORT_LST);
    return;
}

/*
---------------------------------------
    尝试加载目标数据
---------------------------------------
*/
static bool_t
qst_try_load (
  __CR_IO__ sQstMount*      parm,
  __CR_IO__ sLOADER*        ldrs,
  __CR_IO__ sQstMntNode*    node,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          type
    )
{
    leng_t  idx;
    leng_t  cnts;
    sFMTZ*  fmtz;
    sFMTZ*  tmpz;
    sARRAY  list;
    ansi_t* info;
    sLOADER copy;

    /* 初始化加载尝试 */
    fmtz = tmpz = NULL;
    array_initT(&list, ansi_t*);
    list.free = finfo_free;
    struct_cpy(&copy, ldrs, sLOADER);
    cnts = array_get_sizeT(&parm->extz, sENGINE*);

    sENGINE**   unit;
    sFMT_DAT*   datz;
    sFMT_PRT*   pakz;

    /* 尝试到有结果为止 */
_retry:
    /* 逐个插件试验加载 */
    for (unit = NULL, idx = 0; idx < cnts; idx++) {
        unit = array_get_unitT(&parm->extz, sENGINE*, idx);
        tmpz = engine_do(*unit, &copy);
        if (tmpz != NULL)
            break;
    }

    /* 无法识别的封包文件 */
    if (unit == NULL || tmpz == NULL)
        goto _func_out;

    /* 需要经过二次解码 */
    if (tmpz->type == CR_FMTZ_NOP ||
        tmpz->type == CR_FMTZ_DEC)
    {
        /* 换成内存解析 */
        if (fmtz != NULL)
            fmtz_free(fmtz);
        fmtz = tmpz;
        datz = (sFMT_DAT*)tmpz;
        set_ldrM(&copy, datz->data, datz->unsz, ldrs->aprm, 0, 0);
        copy.page = ldrs->page;

        /* 压入插件信息 */
        info = str_fmtA("Plug-in: %s", (*unit)->info);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入文件信息 */
        info = str_fmtA("+-Decoder: %s", datz->infor);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入大小信息 */
        info = str_fmtA("+---File size: %I64u Bytes / Real size: %u Bytes",
                                    datz->pksz, datz->unsz);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);
        goto _retry;
    }

    /* 得到可以加载的封包文件 */
    if (tmpz->type == CR_FMTZ_PRT)
    {
        pakz = (sFMT_PRT*)tmpz;
        if (str_cmpA(pakz->more, "iPACKAGE") != 0)
            goto _func_out;

        /* 填充返回的文件挂载结构 */
        node->type = type;
        node->pack = (iPACKAGE*)pakz->port;
        node->file = str_dupA(ldrs->name.ansi);
        if (node->file == NULL)
            goto _func_out;
        node->name = str_dupA(name);
        if (node->name == NULL) {
            mem_free(node->file);
            goto _func_out;
        }
        node->show = utf8_to_local(ldrs->page, name);
        if (node->show == NULL) {
            mem_free(node->name);
            mem_free(node->file);
            goto _func_out;
        }
        if (fmtz == NULL) {
            node->data = NULL;
        }
        else {
            datz = (sFMT_DAT*)fmtz;
            node->data = datz->data;
            mem_free(fmtz);
        }

        /* 压入插件信息 */
        info = str_fmtA("Plug-in: %s", (*unit)->info);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入封包信息 */
        info = str_fmtA("+-Package: %s", pakz->infor);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        int64u  count = pack_file_num(node->pack);

        /* 压入子文件数量 */
        info = str_fmtA("+---Sub file count: %I64u", count);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);
        node->fmtz = pakz;
        struct_cpy(&node->info, &list, sARRAY);
        return (TRUE);
    }

    /* 释放临时的数据 */
_func_out:
    if (fmtz != NULL)
        fmtz_free(fmtz);
    if (tmpz != NULL)
        fmtz_free(tmpz);
    array_freeT(&list, ansi_t*);
    return (FALSE);
}

/*
---------------------------------------
    加载封包文件
---------------------------------------
*/
static void_t
qst_load_package (
  __CR_IO__ sQstMount*      parm,
  __CR_IO__ sLOADER*        ldrs,
  __CR_IN__ int32u          node,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          type
    )
{
    sQstMntNode     temp;
    sQstMntNode*    find;

    /* 检查封包是否已加载 */
    if (type == QST_MOUNT_DISK) {
        find = curbead_findT(&parm->list, sQstMntNode, name);
        if (find != NULL) {
            qst_send_finfo(&find->info, parm->netw);
            return;
        }
    }

    /* 尝试加载文件 */
    if (!qst_try_load(parm, ldrs, &temp, name, type)) {
        if (type == QST_MOUNT_TEMP)
            file_deleteA(ldrs->name.ansi);
        return;
    }

    /* 封包对象加入已加载列表 */
    if (curbead_insertT(&parm->list, sQstMntNode, name, &temp) == NULL) {
        mount_free(&temp);
        return;
    }
    array_push_growT(&parm->paks, iPACKAGE*, &temp.pack);
    qst_send_finfo(&temp.info, parm->netw);
    qst_refresh_list(parm);

    /* 输出文件列表文件 */
    if (node != 0x00000000UL)
        qst_out_flist(parm, temp.pack, node, ldrs->page);
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
qst_mnt_app_exit (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    CR_NOUSE(argc);
    CR_NOUSE(argv);
    ((sQstMount*)parm)->quit = TRUE;
    return (FALSE);
}

/*
---------------------------------------
    显示窗口位置
---------------------------------------
*/
static bool_t
qst_mnt_win_show (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstMount*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMount*)parm;
    misc_bring2top(ctx->hwnd, NULL);
    return (TRUE);
}

/*
---------------------------------------
    保存窗口位置
---------------------------------------
*/
static bool_t
qst_mnt_win_save (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    RECT        win;
    sQstMount*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 获取窗口位置 */
    ctx = (sQstMount*)parm;
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
qst_mnt_win_load (
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

    sQstMount*  ctx;

    /* 设置窗口位置 */
    ctx = (sQstMount*)parm;
    misc_bring2top(ctx->hwnd, NULL);
    return (misc_cui_setwin(ctx->hwnd, ctx->hcui, x1, y1, ww, hh));
}

/*
---------------------------------------
    释放所有 FMTZ 插件
---------------------------------------
*/
static bool_t
qst_mnt_ext_free (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstMount*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    ctx = (sQstMount*)parm;
    array_freeT(&ctx->extz, sENGINE*);
    ctx->extz.free = plugin_free;
    return (TRUE);
}

/*
---------------------------------------
    加载一个 FMTZ 插件
---------------------------------------
*/
static bool_t
qst_mnt_ext_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sbin_t      dll;
    sQstMount*  ctx;

    /* 参数解析 <插件路径> */
    if (argc < 2)
        return (FALSE);

    sENGINE*        port;
    engine_init_t   func;

    /* 获取引擎接口 */
    dll = sbin_loadA(argv[1]);
    if (dll == NULL)
        return (FALSE);
    func = sbin_exportT(dll, "engine_get", engine_init_t);
    if (func == NULL)
        goto _failure;
    if ((port = func()) == NULL)
        goto _failure;
    port->sbin = dll;
    port->mask = CR_FMTZ_MASK_DAT | CR_FMTZ_MASK_PAK;

    /* 压入插件列表 */
    ctx = (sQstMount*)parm;
    if (array_push_growT(&ctx->extz, sENGINE*, &port) == NULL) {
        engine_free(port);
        return (FALSE);
    }
    ctx->extz.free = plugin_free;
    return (TRUE);

_failure:
    sbin_unload(dll);
    return (FALSE);
}

/*
---------------------------------------
    加载一个磁盘文件
---------------------------------------
*/
static bool_t
qst_mnt_ldr_file (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t  page;
    int32u  node;
    ansi_t* name;
    fdist_t head;
    fdist_t tail;

    /* 参数解析 <文件路径> [头偏移] [尾偏移] [编码] [节点地址] [备注] */
    if (argc < 2)
        return (FALSE);
    node = 0UL;
    page = CR_LOCAL;
    head = tail = 0;
    if (argc > 2) {
        head = str2intx64A(argv[2]);
        if (argc > 3) {
            tail = str2intx64A(argv[3]);
            if (argc > 4) {
                page = str2intxA(argv[4]);
                if (argc > 5)
                    node = str2intx32A(argv[5]);
            }
        }
    }

    sLOADER     ldr;
    sQstMount*  ctx = (sQstMount*)parm;

    /* 根据路径名生成挂载名 */
    name = local_to_utf8(page, argv[1] + ctx->head);
    if (name == NULL)
        return (FALSE);
    path_uniqueA(name);

    /* 附加参数 aprm 不设为空 */
    if (argc > 6)
        set_ldrA(&ldr, argv[1], argv[6], head, tail);
    else
        set_ldrA(&ldr, argv[1], "", head, tail);
    ldr.page = page;

    /* 尝试加载文件 */
    QST_SET_STATE_BUSY
    qst_load_package(ctx, &ldr, node, name, QST_MOUNT_DISK);
    QST_SET_STATE_FREE

    /* 无论成功失败都返回成功 */
    mem_free(name);
    return (TRUE);
}

/*
---------------------------------------
    加载一个包内文件
---------------------------------------
*/
static bool_t
qst_mnt_ldr_pack (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    uint_t  page;
    int32u  node;
    ansi_t* name;
    fdist_t head;
    fdist_t tail;

    /* 参数解析 <挂载名> <文件名> [头偏移] [尾偏移] [编码] [节点地址] [备注] */
    if (argc < 3)
        return (FALSE);
    node = 0UL;
    page = CR_LOCAL;
    head = tail = 0;
    if (argc > 3) {
        head = str2intx64A(argv[3]);
        if (argc > 4) {
            tail = str2intx64A(argv[4]);
            if (argc > 5) {
                page = str2intxA(argv[5]);
                if (argc > 6)
                    node = str2intx32A(argv[6]);
            }
        }
    }

    int64u          idx;
    sQstMount*      ctx;
    sQstMntNode*    mnt;

    /* 查找挂载节点 */
    ctx = (sQstMount*)parm; /* 需要去掉根目录路径 */
    name = local_to_utf8(page, argv[1] + ctx->head);
    if (name == NULL)
        return (FALSE);
    path_uniqueA(name);
    mnt = curbead_findT(&ctx->list, sQstMntNode, name);
    mem_free(name);
    if (mnt == NULL)
        return (FALSE);

    /* 查找包内文件 */
    if (!pack_find_fileA(mnt->pack, &idx, argv[2], page))
        return (FALSE);

    leng_t      size;
    ansi_t*     send;
    sBUFFER     buff;
    sPAK_FILE*  info;

    /* 合成挂载节点名称 */
    pack_file_info(mnt->pack, &info, idx);
    send = str_fmtA("%s|%s", argv[1] + ctx->head, argv[2]);
    if (send == NULL)
        return (FALSE);
    name = local_to_utf8(page, send);
    mem_free(send);
    if (name == NULL)
        return (FALSE);
    path_uniqueA(name);

    /* 文件加载到内存 */
    if (!pack_file_data(mnt->pack, &buff, idx, TRUE))
        goto _failure1;
    size = buffer_size(&buff);

    /* 调试用的文件导出功能 */
    if (file_existA(QST_PATH_OUTPUT "hack.dat"))
        file_saveA(QST_PATH_OUTPUT "hack.dat", buff.data, size);

    ansi_t  strn[50];

    /* 释放上次的内存共享文件 */
    mem_zero(strn, sizeof(strn));
    if (ctx->smem != NULL) {
        share_file_close(ctx->smem);
        ctx->smem = NULL;
    }

    /* 建立内存共享文件, 并通知别人 */
    ctx->smem = share_file_open(name, strn, size);
    if (ctx->smem != NULL) {
        if (share_file_fill(ctx->smem, buff.data, size)) {
            if (argc > 7) {
                send = str_fmtA("ldr:smem %s %u \"%s\" %I64u %I64u %u \"%s\"",
                            strn, size, argv[2], head, tail, page, argv[7]);
            } else {
                send = str_fmtA("ldr:smem %s %u \"%s\" %I64u %I64u %u",
                            strn, size, argv[2], head, tail, page);
            }
            if (send != NULL) {
                cmd_shl_send(ctx->netw, send);
                mem_free(send);
            }
        }
    }

    /* 检查封包是否已加载 */
    mnt = curbead_findT(&ctx->list, sQstMntNode, name);
    if (mnt != NULL) {
        qst_send_finfo(&mnt->info, ctx->netw);
        buffer_free(&buff);
        mem_free(name);
        return (TRUE);
    }

    ansi_t* ftmp;
    ansi_t* fext;

    /* 将文件写入临时目录做读包尝试 (解决无限包中包嵌套的问题) */
    if (strn[0] == 0x00)
        goto _failure2;
    fext = str_allocA(str_sizeA(argv[2]));
    if (fext == NULL)
        goto _failure2;
    filext_extractA(fext, argv[2]);
    ftmp = str_fmtA(QST_PATH_OUTPUT "%s%s", strn, fext);
    mem_free(fext);
    if (ftmp == NULL)
        goto _failure2;
    if (!file_saveA(ftmp, buff.data, size)) {
        mem_free(ftmp);
        goto _failure2;
    }
    buffer_free(&buff);

    sLOADER ldr;

    /* 附加参数 aprm 不设为空 */
    if (argc > 7)
        set_ldrA(&ldr, ftmp, argv[7], head, tail);
    else
        set_ldrA(&ldr, ftmp, "", head, tail);
    ldr.page = page;

    /* 尝试加载文件 */
    QST_SET_STATE_BUSY
    qst_load_package(ctx, &ldr, node, name, QST_MOUNT_TEMP);
    QST_SET_STATE_FREE

    /* 无论成功失败都返回成功 */
    mem_free(name);
    mem_free(ftmp);
    return (TRUE);

_failure2:
    buffer_free(&buff);
_failure1:
    mem_free(name);
    return (FALSE);
}

/*
---------------------------------------
    释放所有已加载内容
---------------------------------------
*/
static bool_t
qst_mnt_ldr_free (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstMount*  ctx;

    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 重新生成出错直接退出 */
    ctx = (sQstMount*)parm;
    array_freeT(&ctx->paks, iPACKAGE*);
    curbead_freeT(&ctx->list, sQstMntNode);
    if (!curbead_initT(&ctx->list, sQstMntNode, 0)) {
        ctx->quit = TRUE;
        return (FALSE);
    }
    ctx->list.find = mount_find;
    ctx->list.comp = mount_comp;
    ctx->list.free = mount_free;
    curbead_freeT(&ctx->resx, sQstResNode);
    if (!curbead_initT(&ctx->resx, sQstResNode, 0)) {
        ctx->quit = TRUE;
        return (FALSE);
    }
    ctx->resx.find = resx_find;
    ctx->resx.comp = resx_comp;
    ctx->resx.free = resx_free;
    qst_refresh_list(ctx);
    ctx->head = 0;
    return (TRUE);
}

/*
---------------------------------------
    设置资源根目录长度
---------------------------------------
*/
static bool_t
qst_mnt_res_root (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    ansi_t*     str;
    sQstMount*  ctx;

    /* 参数解析 <根目录路径> */
    ctx = (sQstMount*)parm;
    if (argc < 2)
        return (FALSE);
    str = argv[1];
    ctx->head = str_lenA(str);
    if (!is_slashA(str[ctx->head - 1]))
        ctx->head += 1;
    return (TRUE);
}

/*
---------------------------------------
    加载临时资源到内存
---------------------------------------
*/
static bool_t
qst_mnt_res_load (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    int64u          idx;
    iPACKAGE*       prt;
    sQstMount*      ctx;
    sQstResNode     tmp;
    sQstMntNode*    mnt;
    sQstResNode*    res;

    /* 参数解析 <标识名> <挂载名> <文件名> [编码] */
    if (argc < 4)
        return (FALSE);

    uint_t      page;
    leng_t      size;
    ansi_t*     send;
    sBUFFER     buff;
    sPAK_FILE*  info;

    page = CR_LOCAL;
    if (argc > 4)
        page = str2intxA(argv[4]);
    ctx = (sQstMount*)parm;

    /* 可以使用全局挂载名 */
    if (str_cmpA(argv[2], QST_STR_GLOBALS) == 0)
    {
        leng_t  ii, nn;

        /* 逐个封包查找文件 */
        send = local_to_utf8(page, argv[3]);
        if (send == NULL)
            goto _failure1;
        idx = 0;
        prt = NULL;
        nn = array_get_sizeT(&ctx->paks, iPACKAGE*);
        for (ii = 0; ii < nn; ii++) {
            prt = (array_get_unitT(&ctx->paks, iPACKAGE*, ii))[0];
            if (pack_find_fileU(prt, &idx, send))
                break;
        }
        mem_free(send);
        if (ii >= nn)
            goto _failure1;
    }
    else
    {
        /* 直接使用挂载名称 */
        send = local_to_utf8(page, argv[2]);
        if (send == NULL)
            goto _failure1;
        path_uniqueA(send);
        mnt = curbead_findT(&ctx->list, sQstMntNode, send);
        mem_free(send);
        if (mnt == NULL)
            goto _failure1;

        /* 查找包内文件 */
        if (!pack_find_fileA(mnt->pack, &idx, argv[3], page))
            goto _failure1;
        prt = mnt->pack;
    }

    /* 文件加载到内存 */
    pack_file_info(prt, &info, idx);
    if (!pack_file_data(prt, &buff, idx, TRUE))
        goto _failure1;
    size = buffer_size(&buff);

    /* 根据标识名查找建立资源槽位 */
    res = curbead_findT(&ctx->resx, sQstResNode, argv[1]);
    if (res != NULL)
    {
        /* 释放上次建立的内存共享文件 */
        if (res->objs != NULL) {
            share_file_close(res->objs);
            res->objs = NULL;
        }
        SAFE_FREE(res->file)
    }
    else
    {
        /* 新建一个资源槽位 */
        struct_zero(&tmp, sQstResNode);
        tmp.name = str_dupA(argv[1]);
        if (tmp.name == NULL)
            goto _failure2;
        res = curbead_insertT(&ctx->resx, sQstResNode, argv[1], &tmp);
        if (res == NULL) {
            mem_free(tmp.name);
            goto _failure2;
        }
    }

    /* 使用标识名称建立内存共享文件 */
    res->objs = share_file_open(argv[1], NULL, size);
    if (res->objs == NULL)
        goto _failure2;
    if (!share_file_fill(res->objs, buff.data, size)) {
        share_file_close(res->objs);
        res->objs = NULL;
        goto _failure2;
    }
    res->size = size;
    res->file = str_dupA(argv[3]);
    buffer_free(&buff);

    /* 发出已经建立成功的通知 */
    send = str_fmtA("res:okay %s %u", argv[1], size);
    if (send != NULL) {
        cmd_shl_send(ctx->netw, send);
        mem_free(send);
    }
    qst_refresh_list(ctx);
    return (TRUE);

_failure2:
    buffer_free(&buff);
_failure1:
    send = str_fmtA("res:fail %s", argv[1]);
    if (send != NULL) {
        cmd_shl_send(ctx->netw, send);
        mem_free(send);
    }
    return (FALSE);
}

/*
---------------------------------------
    释放已加载的临时资源
---------------------------------------
*/
static bool_t
qst_mnt_res_free (
  __CR_IN__ void_t*     parm,
  __CR_IN__ uint_t      argc,
  __CR_IN__ ansi_t**    argv
    )
{
    sQstMount*  ctx;

    /* 参数解析 <标识名> */
    if (argc < 2)
        return (FALSE);
    ctx = (sQstMount*)parm;
    curbead_deleteT(&ctx->resx, sQstResNode, argv[1]);
    qst_refresh_list(ctx);
    return (TRUE);
}

/*****************************************************************************/
/*                               命令行功能表                                */
/*****************************************************************************/

static const sQST_CMD   s_cmdz[] =
{
    /***** 公用系统命令 *****/
    { "app:exit", qst_mnt_app_exit },
    { "win:load", qst_mnt_win_load },
    { "win:save", qst_mnt_win_save },
    { "win:show", qst_mnt_win_show },

    /***** 公用插件命令 *****/
    { "ext:free", qst_mnt_ext_free },
    { "ext:load", qst_mnt_ext_load },

    /***** 公用加载命令 *****/
    { "ldr:file", qst_mnt_ldr_file },
    { "ldr:pack", qst_mnt_ldr_pack },
    { "ldr:free", qst_mnt_ldr_free },

    /***** 公用资源命令 *****/
    { "res:root", qst_mnt_res_root },
    { "res:load", qst_mnt_res_load },
    { "res:free", qst_mnt_res_free },

    /***** 封包插件命令 *****/
    { "pak:ext:free", qst_mnt_ext_free },
    { "pak:ext:load", qst_mnt_ext_load },

    /***** 私有命令映射 *****/
    { "qmnt:app:exit", qst_mnt_app_exit },
    { "qmnt:win:show", qst_mnt_win_show },
};

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 只允许一个例程 */
    if (misc_is_running(EXE_XNAME))
        return (QST_ERROR);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);
    SetConsoleTitleA(WIN_TITLE);
    mem_zero(&s_wrk_ctx, sizeof(s_wrk_ctx));

    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 初始化窗口 */
    s_wrk_ctx.hcui = GetStdHandle(STD_OUTPUT_HANDLE);
    if (s_wrk_ctx.hcui == INVALID_HANDLE_VALUE)
        return (QST_ERROR);
    s_wrk_ctx.hwnd = GetConsoleWindow();
    if (s_wrk_ctx.hwnd == NULL)
        return (QST_ERROR);
    misc_desk_init(WIN_ICONF, &x1, &y1, &ww, &hh,
                   QMNT_DEF_WIDTH, QMNT_DEF_HEIGHT);
    misc_cui_setwin(s_wrk_ctx.hwnd, s_wrk_ctx.hcui, x1, y1, ww, hh);

    exec_t  obj;

    /* 加载命令表 */
    obj = cmd_exec_init(s_cmdz, cntsof(s_cmdz));
    if (obj == NULL)
        return (QST_ERROR);

    /* 初始化挂载表 */
    if (!curbead_initT(&s_wrk_ctx.list, sQstMntNode, 0))
        return (QST_ERROR);
    s_wrk_ctx.list.find = mount_find;
    s_wrk_ctx.list.comp = mount_comp;
    s_wrk_ctx.list.free = mount_free;

    /* 初始化资源表 */
    if (!curbead_initT(&s_wrk_ctx.resx, sQstResNode, 0))
        return (QST_ERROR);
    s_wrk_ctx.resx.find = resx_find;
    s_wrk_ctx.resx.comp = resx_comp;
    s_wrk_ctx.resx.free = resx_free;

    /* 初始化网络 */
    if (!socket_init())
        return (QST_ERROR);
    s_wrk_ctx.netw = netw_cli_open(EXE_XNAME);
    if (s_wrk_ctx.netw == NULL)
        return (QST_ERROR);

    /* 读取需要超时, 不然线程无法退出 */
    socket_set_timeout(s_wrk_ctx.netw, -1, QST_TCP_TOUT);

    /* 建立颜色值 */
    s_color_temp = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_RED);
    s_color_disk = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_GREEN);
    s_color_smem = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_BLUE);
    s_color_head = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_GREEN |
                        CR_CUI_TEXT_RED | CR_CUI_TEXT_BLUE);
    s_color_text = cui_make_attr(0, CR_CUI_TEXT_LIGHT |
                        CR_CUI_TEXT_RED | CR_CUI_TEXT_GREEN);
    /* 工作循环 */
    s_wrk_ctx.quit = FALSE;
    qst_refresh_list(&s_wrk_ctx);
    while (!s_wrk_ctx.quit)
    {
        ansi_t* string;

        /* 接收一条命令 */
        /* 即使是出错也要继续运行 */
        string = netw_cmd_recv(s_wrk_ctx.netw);
        if (string == NULL) {
            thread_sleep(1);
            continue;
        }

        /* 执行这条命令 */
        cmd_exec_main(obj, &s_wrk_ctx, string);
        mem_free(string);
    }
    /* 必须正常退出, 否则临时文件无法删除 */
    if (s_wrk_ctx.smem != NULL)
        share_file_close(s_wrk_ctx.smem);
    curbead_freeT(&s_wrk_ctx.resx, sQstResNode);
    curbead_freeT(&s_wrk_ctx.list, sQstMntNode);
    cmd_exec_free(obj);
    netw_cli_close(s_wrk_ctx.netw);
    return (QST_OKAY);
}
