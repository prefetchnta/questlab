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
 *             ##       CREATE: 2025-02-25
 *              #
 ================================================
        SVN 文件名大小写批量更改工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

static leng_t   s_file_numb = 0;
static leng_t   s_root_leng = 0;
static ansi_t   s_file_case = 0;
static bool_t   s_dest_slsh = FALSE;
static bool_t   s_make_dirs = FALSE;

/*
---------------------------------------
    显示工具用法
---------------------------------------
*/
static void_t usage (void_t)
{
    printf("svnfcase <src dir> <dst dir> <u/l> [dir=1]\n");
}

/*
---------------------------------------
    文件处理回调
---------------------------------------
*/
static bool_t fmover (void_t *param, sSEARCHa *info)
{
    ansi_t* cmd;
    ansi_t* dst;

    /* 替换根目录 */
    if (s_dest_slsh)
        dst = str_fmtA("%s%s", param, &info->name[s_root_leng]);
    else
        dst = str_fmtA("%s\\%s", param, &info->name[s_root_leng]);
    if (dst != NULL)
    {
        /* 切换大小写 */
        if (s_file_case == 'u' || s_file_case == 'U')
            str_uprA(dst);
        else
            str_lwrA(dst);
        if (s_make_dirs)
        {
            /* 只建立目录 */
            if (info->attr & CR_FILE_ATTR_DIR) {
                str_catA(dst, "\\");
                path_buildA(dst);
                s_file_numb += 1;
                printf("%s\n", dst);
            }
        }
        else
        {
            /* 只移动文件 */
            cmd = str_fmtA("svn -q mv \"%s\" \"%s\"", info->name, dst);
            if (cmd != NULL) {
                shell_cmd(cmd, NULL);
                mem_free(cmd);
                s_file_numb += 1;
                printf("%s\n", dst);
            }
        }
        mem_free(dst);
    }
    return (TRUE);
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 <源目录> <目标目录> <U/L> [DIR=1] */
    if (argc < 4) {
        usage();
        return (QST_ERROR);
    }

    /* 大小写参数 */
    s_file_case = (argv[3])[0];
    if (s_file_case != 'u' && s_file_case != 'l' &&
        s_file_case != 'U' && s_file_case != 'L') {
        usage();
        return (QST_ERROR);
    }

    ansi_t* src_root = str_dupA(argv[1]);
    ansi_t* dst_root = str_dupA(argv[2]);

    if (src_root == NULL || dst_root == NULL) {
        printf("str_dupA() failure!\n");
        return (QST_ERROR);
    }
    path_uniqueA(dst_root);
    s_root_leng = str_lenA(dst_root);
    s_dest_slsh = is_slashA(dst_root[s_root_leng - 1]);
    path_uniqueA(src_root);
    s_root_leng = str_lenA(src_root);
    if (!is_slashA(src_root[s_root_leng - 1]))
        s_root_leng++;
    s_file_numb = 0;
    if (argc == 4 || (argv[4])[0] == '1') {
        s_make_dirs = TRUE;     /* 只建立目录 */
        file_searchA(src_root, TRUE, FALSE, TRUE, "*.*", fmover, dst_root);
    }
    else {
        s_make_dirs = FALSE;    /* 只移动文件 */
        file_searchA(src_root, TRUE, FALSE, FALSE, "*.*", fmover, dst_root);
    }
    printf("%u\n", s_file_numb);
    mem_free(src_root);
    mem_free(dst_root);
    return (QST_OKAY);
}
