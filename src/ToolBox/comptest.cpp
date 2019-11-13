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
 *             ##       CREATE: 2016-12-22
 *              #
 ================================================
        压缩算法测试小工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 性能测试对象 */
static xtime_t  s_profile;

/* 全局性能统计 */
static fp64_t   s_enc_speed;
static fp64_t   s_enc_ratio;
static fp64_t   s_dec_speed;
static int64u   s_file_okay;
static int64u   s_file_fail;

/* 压缩的上下文 */
typedef struct
{
    void_t*     e_param;
    enc_copy_t  encode4;
    enc_parm_t  encode5;
    void_t*     d_param;
    enc_copy_t  decode4;
    enc_parm_t  decode5;

} sCOMP;

/* 文字颜色属性 */
static int16u   s_clr_file;
static int16u   s_clr_info;
static int16u   s_clr_okay;
static int16u   s_clr_fail;

/* 附加的数组参数 */
static ansi_t*  s_param1 = NULL;
static ansi_t*  s_param2 = NULL;

/*
---------------------------------------
    文件处理回调
---------------------------------------
*/
static bool_t tester (void_t *param, sSEARCHa *info)
{
    sCOMP*  comp;
    fp32_t  time;
    leng_t  dsize;
    leng_t  ssize;
    void_t* ddata;
    void_t* sdata;
    fp32_t  ratio_e = 0;
    fp32_t  speed_e = 0;
    fp32_t  speed_d = 0;
    byte_t  dhash[20];
    byte_t  shash[20];

    /* 跳过空文件和大文件 */
    if (info->size == 0 || info->size > CR_M2B(64))
        return (TRUE);
    comp = (sCOMP*)param;

    /* 文件内容全部读起并分配目标内存 */
    sdata = file_load_as_binA(info->name, &ssize);
    if (sdata == NULL)
        return (TRUE);
    dsize = ssize * 2 + 1024;
    ddata = mem_malloc(dsize);
    if (ddata == NULL) {
        mem_free(sdata);
        return (TRUE);
    }

    /* 计算内容原始哈希 */
    hash_sha1_total(shash, sdata, ssize);

    /* 显示文件名和大小字节数 */
    cui_set_color(s_clr_file);
    printf("%s (%" CR_FSZ "u) ", info->name, info->size);

    /* 压缩文件数据并显示速度 */
    timer_set_base(s_profile);
    if (comp->encode4 != NULL)
        dsize = comp->encode4(ddata, dsize, sdata, ssize);
    else
        dsize = comp->encode5(ddata, dsize, sdata, ssize, comp->e_param);
    time = timer_get_delta(s_profile) * 1.024f;
    cui_set_color(s_clr_info);

    /* 解压文件数据并显示速度 */
    mem_zero(sdata, ssize);
    if (dsize != 0) {
        speed_e = (fp32_t)info->size / time;
        ratio_e = (fp32_t)dsize * 100.0f / info->size;
        printf("ENC %.2f KB/S, %.2f%%", speed_e, ratio_e);
        timer_set_base(s_profile);
        if (comp->decode4 != NULL)
            dsize = comp->decode4(sdata, ssize, ddata, dsize);
        else
            dsize = comp->decode5(sdata, ssize, ddata, dsize, comp->d_param);
        time = timer_get_delta(s_profile) * 1.024f;
        if (dsize != 0) {
            speed_d = (fp32_t)info->size / time;
            printf(" | DEC %.2f KB/S", speed_d);
        }
        else {
            printf(" | DEC ???? KB/S");
        }
    }

    /* 计算解压后的哈希并比较 */
    hash_sha1_total(dhash, sdata, dsize);
    if (mem_cmp(shash, dhash, sizeof(dhash)) != 0) {
        cui_set_color(s_clr_fail);
        printf(" [FAIL]\n");
        s_file_fail += 1;
    }
    else {
        cui_set_color(s_clr_okay);
        printf(" [OKAY]\n");
        s_enc_speed += speed_e;
        s_enc_ratio += ratio_e;
        s_dec_speed += speed_d;
        s_file_okay += 1;
    }
    mem_free(ddata);
    mem_free(sdata);
    return (TRUE);
}

/*
---------------------------------------
    加载压缩接口
---------------------------------------
*/
static sbin_t load_plugin (sCOMP *param)
{
    sINIu*  ini;
    uint_t  vals;
    sbin_t  sbin;
    ansi_t* text;

    /* 加载配置文件 */
    struct_zero(param, sCOMP);
    text = file_load_as_strA("comptest.ini");
    if (text == NULL)
        return (NULL);
    ini = ini_parseU(text);
    mem_free(text);
    if (ini == NULL)
        return (NULL);

    /* 根据配置加载插件 */
    text = ini_key_stringU("DLL", ini);
    if (text == NULL)
        goto _failure1;
    sbin = sbin_loadA(text);
    mem_free(text);
    if (sbin == NULL)
        goto _failure1;

    /* 获取压缩接口 */
    text = ini_key_stringU("ENC", ini);
    if (text == NULL)
        goto _failure2;
    s_param1 = ini_key_bufferU("ENC_PARM", ini);
    if (s_param1 != NULL && ini->found) {
        s_param1 = skip_spaceA(s_param1);
        if (*s_param1 == '\"') {
            s_param1 = ini_key_stringU("ENC_PARM", ini);
            if (s_param1 == NULL) {
                mem_free(text);
                goto _failure2;
            }
            param->e_param = (void_t*)s_param1;
        }
        else {
            vals = ini_key_intxU("ENC_PARM", 0, ini);
            param->e_param = (void_t*)vals;
        }
        param->encode5 = sbin_exportT(sbin, text, enc_parm_t);
        mem_free(text);
        if (param->encode5 == NULL)
            goto _failure2;
    }
    else {
        param->encode4 = sbin_exportT(sbin, text, enc_copy_t);
        mem_free(text);
        if (param->encode4 == NULL)
            goto _failure2;
    }

    /* 获取解压接口 */
    text = ini_key_stringU("DEC", ini);
    if (text == NULL)
        goto _failure2;
    s_param2 = ini_key_bufferU("DEC_PARM", ini);
    if (s_param2 != NULL && ini->found) {
        s_param2 = skip_spaceA(s_param2);
        if (*s_param2 == '\"') {
            s_param2 = ini_key_stringU("DEC_PARM", ini);
            if (s_param2 == NULL) {
                mem_free(text);
                goto _failure2;
            }
            param->d_param = (void_t*)s_param2;
        }
        else
        if (*s_param2 == '@') {
            s_param2 = s_param1;
            param->d_param = (void_t*)s_param2;
        }
        else {
            vals = ini_key_intxU("DEC_PARM", 0, ini);
            param->d_param = (void_t*)vals;
        }
        param->decode5 = sbin_exportT(sbin, text, enc_parm_t);
        mem_free(text);
        if (param->decode5 == NULL)
            goto _failure2;
    }
    else {
        param->decode4 = sbin_exportT(sbin, text, enc_copy_t);
        mem_free(text);
        if (param->decode4 == NULL)
            goto _failure2;
    }
    ini_closeU(ini);
    return (sbin);

_failure2:
    sbin_unload(sbin);
_failure1:
    ini_closeU(ini);
    return (NULL);
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    sCOMP param;
    sbin_t plugin;
    const ansi_t *root;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 [搜索根目录] */
    if (argc < 1) {
        printf("comptest [directory]\n");
        return (QST_ERROR);
    }

    /* 根据配置文件加载压缩接口 */
    plugin = load_plugin(&param);
    if (plugin == NULL) {
        printf("can't open compress plugin\n");
        return (QST_ERROR);
    }

    /* 颜色属性合成 */
    s_clr_file = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_GREEN |
                                  CR_CUI_TEXT_RED);
    s_clr_info = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_GREEN |
                                  CR_CUI_TEXT_BLUE  | CR_CUI_TEXT_RED);
    s_clr_okay = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_GREEN);
    s_clr_fail = cui_make_attr(0, CR_CUI_TEXT_LIGHT | CR_CUI_TEXT_RED);

    /* 生成性能测试对象 */
    s_profile = timer_new();
    s_enc_speed = 0;
    s_enc_ratio = 0;
    s_dec_speed = 0;
    s_file_okay = 0;
    s_file_fail = 0;

    /* 枚举所有文件 */
    if (argc == 1)
        root = "";
    else
        root = argv[1];
    file_searchA(root, TRUE, FALSE, FALSE, "*.*", tester, &param);
    s_clr_file = cui_make_attr(0, CR_CUI_TEXT_GREEN | CR_CUI_TEXT_BLUE |
                                  CR_CUI_TEXT_RED);
    cui_set_color(s_clr_file);

    /* 打印平均性能指标 */
    s_enc_speed /= (fp64_t)s_file_okay;
    s_enc_ratio /= (fp64_t)s_file_okay;
    s_dec_speed /= (fp64_t)s_file_okay;
    printf("==============================================================\n");
    printf("ENC %.2f KB/S, %.2f%%", s_enc_speed, s_enc_ratio);
    printf(" | DEC %.2f KB/S | ", s_dec_speed);
    printf("%" CR_I64 "u OKAY, %" CR_I64 "u FAIL\n", s_file_okay, s_file_fail);
    printf("==============================================================\n");

    /* 释放内存 */
    sbin_unload(plugin);
    timer_del(s_profile);
    return (QST_OKAY);
}
