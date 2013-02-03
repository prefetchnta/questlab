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
 *             ##       CREATE: 2012-08-17
 *              #
 ================================================
        QuestLAB 文件导入器
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 应用的名称 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "xImpScript"
    #define WIN_TITLE   "xImpScript"
    #define WIN_CLASS   "xImpScriptCLSS"
    #define WIN_ICONF   "xImpScript.ini"
    #define WIN_XCONF   "xImpScript.xml"
#endif

/* 回调使用的结构 */
struct  sTagOut
{
        FILE*   fp;     /* 输出文件 */
        uint_t  page;   /* 文件编码 */
        int64u  head;   /* 起始偏移 */
        int64u  tail;   /* 结尾偏移 */
        ansi_t* memo;   /* 文件备注 */
};

/*
---------------------------------------
    输出导入文件标签
---------------------------------------
*/
static bool_t
file_tag_out (
  __CR_IN__ void_t*         param,
  __CR_IN__ const sSEARCHa* finfo
    )
{
    ansi_t*     esc;
    ansi_t*     utf8;
    int64u      size;
    sTagOut*    ctx = (sTagOut*)param;

    if (ctx->head > finfo->size)
        return (FALSE);
    fprintf(ctx->fp, "<File size=\"%I64u\" ", finfo->size);
    if (finfo->attr & CR_FILE_ATTR_CMP)
        fprintf(ctx->fp, "comp=\"1\" ");
    if (finfo->attr & CR_FILE_ATTR_HID)
        fprintf(ctx->fp, "hide=\"1\" ");
    if (finfo->attr & CR_FILE_ATTR_ENC)
        fprintf(ctx->fp, "cryp=\"1\" ");
    if (ctx->head != 0)
        fprintf(ctx->fp, "offs=\"%I64u\" ", ctx->head);
    if (ctx->tail > finfo->size || ctx->tail <= ctx->head)
        size = finfo->size - ctx->head;
    else
        size = ctx->tail - ctx->head;
    if (size != finfo->size)
        fprintf(ctx->fp, "pack=\"%I64u\" ", size);
    if (ctx->memo != NULL) {
        esc = str_esc_makeU(ctx->memo);
        if (esc == NULL)
            return (FALSE);
        fprintf(ctx->fp, "memo=\"%s\" ", esc);
        mem_free(esc);
    }
    utf8 = local_to_utf8(ctx->page, finfo->name);
    if (utf8 == NULL)
        return (FALSE);
    esc = str_esc_makeU(utf8);
    mem_free(utf8);
    if (esc == NULL)
        return (FALSE);
    fprintf(ctx->fp, "path=\"%s\" />\n", esc);
    mem_free(esc);
    return (TRUE);
}

/*
---------------------------------------
    解析执行导入脚本
---------------------------------------
*/
static bool_t
do_import_scr (
  __CR_IN__ sTagOut*        parm,
  __CR_IN__ const ansi_t*   file,
  __CR_IN__ const ansi_t*   root
    )
{
    sXMLu*  xml;
    leng_t  idx;
    ansi_t* str;

    /* 加载文件 */
    str = file_load_as_strA(file);
    if (str == NULL)
        return (FALSE);

    /* 解析 XML 文件 */
    xml = xml_parseU(str);
    mem_free(str);
    if (xml == NULL)
        return (FALSE);

    /* 检查根标签 */
    if (xml->nodes[0].name[0] == CR_AC('?'))
        idx = 1;
    else
        idx = 0;
    if (xml->nodes[idx].closed ||
        str_cmpIA(xml->nodes[idx].name, CR_AS("ImpScript")) != 0)
        goto _failure;

    /* 逐个标签执行 */
    for (idx++; idx < xml->count; idx++)
    {
        bool_t      subx, hide, rett;
        sXNODEu*    node = &xml->nodes[idx];

        /* 跳过没用的节点 */
        if (str_cmpIA(node->name, CR_AS("/ImpScript")) == 0)
            break;
        if (!node->closed ||
            str_cmpIA(node->name, CR_AS("Import")) != 0) {
            idx = xml_skip_tagU(xml->nodes, idx, xml->count) - 1;
            continue;
        }

        ansi_t*         utf8;
        const ansi_t*   filter;

        /* 执行一个节点 */
        utf8 = xml_attr_stringU(CR_AS("name"), node);
        if (utf8 != NULL) {
            filter = utf8_to_local(parm->page, utf8);
            mem_free(utf8);
            if (filter == NULL)
                goto _failure;
        }
        else {
            filter = NULL;
        }
        subx = xml_attr_intxU(CR_AS("sub"),  FALSE, node);
        hide = xml_attr_intxU(CR_AS("hide"), FALSE, node);
        parm->head = xml_attr_intx64U(CR_AS("head"), 0, node);
        parm->tail = xml_attr_intx64U(CR_AS("tail"), 0, node);
        parm->memo = xml_attr_stringU(CR_AS("memo"), node);
        if (filter == NULL) {
            filter = "*.*";
            rett = file_searchA(root, subx, hide, FALSE,
                        &filter, 1, file_tag_out, parm);
        }
        else {
            rett = file_searchA(root, subx, hide, FALSE,
                        &filter, 1, file_tag_out, parm);
            mem_free(filter);
        }
        TRY_FREE(parm->memo)
        if (!rett)
            goto _failure;
    }
    xml_closeU(xml);
    return (TRUE);

_failure:
    xml_closeU(xml);
    return (FALSE);
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    /* 只允许一个例程 */
    if (misc_is_running(EXE_XNAME))
        return (QST_ERROR);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);
    SetConsoleTitleA(WIN_TITLE);

    ansi_t* root;
    ansi_t* utf8;
    ansi_t* file = NULL;
    uint_t  page = CR_LOCAL;

    /* 参数解析 [列表编码] [导入脚本] */
    if (argc > 1) {
        page = str2intxA(argv[1]);
        if (argc > 2)
            file = argv[2];
    }

    /* 获取根目录字符串 */
    root = file_load_as_strA(QST_SELECT_DIR);
    if (root == NULL)
        return (QST_ERROR);
    utf8 = local_to_utf8(page, root);
    if (utf8 == NULL)
        return (QST_ERROR);

    FILE*   fp;
    ansi_t* esc;

    /* 需要转义输出 */
    esc = str_esc_makeU(utf8);
    mem_free(utf8);
    if (esc == NULL)
        return (QST_ERROR);

    /* 输出 XML 头标签 */
    fp = fopen(QST_IMPORT_LST, "w");
    if (fp == NULL)
        return (QST_ERROR);
    if (page == CR_LOCAL)
        page = get_sys_codepage();
    fprintf(fp, "<!-- encoding:65001 -->\n");
    fprintf(fp, "<QstTree root=\"%s\" page=\"%u\">\n", esc, page);
    mem_free(esc);

    sTagOut param;

    /* 输出文件标签 */
    param.fp = fp;
    param.page = page;
    if (file == NULL)
    {
        const ansi_t*   filter = "*.*";

        /* 导入所有文件 */
        param.head = 0;
        param.tail = 0;
        param.memo = NULL;
        if (!file_searchA(root, FALSE, FALSE, FALSE,
                &filter, 1, file_tag_out, &param)) {
            fclose(fp);
            file_deleteA(QST_IMPORT_LST);
            return (QST_ERROR);
        }
    }
    else
    {
        /* 执行指定的导入脚本 */
        if (!do_import_scr(&param, file, root)) {
            fclose(fp);
            file_deleteA(QST_IMPORT_LST);
            return (QST_ERROR);
        }
    }

    /* 输出 XML 尾标签 */
    fprintf(fp, "</QstTree>\n");
    fclose(fp);
    return (QST_OKAY);
}
