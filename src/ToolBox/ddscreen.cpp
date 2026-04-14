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
 *             ##       CREATE: 2026-04-02
 *              #
 ================================================
        截屏解码小工具
 ================================================
 */

#include "LabAI.h"
#include "../QstLibs/QstLibs.h"
#pragma comment (lib, "LabAI.lib")

/* 32位色 BMP 文件头 */
static byte_t s_bmp[54] =
{
/*000*/ 0x42, 0x4D,
/*002*/ 0x00, 0x00, 0x00, 0x00, /* 文件大小 */
/*006*/ 0x00, 0x00, 0x00, 0x00,
/*010*/ 0x36, 0x00, 0x00, 0x00,
/*014*/ 0x28, 0x00, 0x00, 0x00,
/*018*/ 0x00, 0x00, 0x00, 0x00, /* 图片宽度 */
/*022*/ 0x00, 0x00, 0x00, 0x00, /* 图片高度 */
/*026*/ 0x01, 0x00,
/*028*/ 0x20, 0x00,
/*030*/ 0x00, 0x00, 0x00, 0x00,
/*034*/ 0x00, 0x00, 0x00, 0x00, /* 数据大小 */
/*038*/ 0x00, 0x00, 0x00, 0x00,
/*042*/ 0x00, 0x00, 0x00, 0x00,
/*046*/ 0x00, 0x00, 0x00, 0x00,
/*050*/ 0x00, 0x00, 0x00, 0x00,
/*054*/
};

/*
---------------------------------------
    保存32位色 BMP 图片
---------------------------------------
*/
static void_t
save_bmp32 (
  __CR_IN__ const sIMAGE*   img
    )
{
    FILE*   fp;

    fp = fopen("__screen__.bmp", "wb");
    if (fp != NULL) {
        *(int32u*)(&s_bmp[ 2]) = (int32u)(img->size + sizeof(s_bmp));
        *(int32u*)(&s_bmp[18]) = (int32u)(img->position.ww);
        *(int32u*)(&s_bmp[22]) = (int32u)(0-img->position.hh);
        *(int32u*)(&s_bmp[34]) = (int32u)(img->size);
        fwrite(s_bmp, sizeof(s_bmp), 1, fp);
        fwrite(img->data, img->size, 1, fp);
        fclose(fp);
    }
}

/*
---------------------------------------
    显示工具用法
---------------------------------------
*/
static void_t usage (void_t)
{
    printf("spackage <ddtype> [ddparam]...\n");
}

/*
---------------------------------------
    复制文本到剪贴板
---------------------------------------
*/
static void_t
text2clipboard (
  __CR_IN__ const wide_t*   text
    )
{
    if (!OpenClipboard(NULL))
        return;
    EmptyClipboard();

    leng_t  len = str_sizeW(text);
    HGLOBAL hmem = GlobalAlloc(GMEM_MOVEABLE, len);

    if (hmem != NULL) {
        memcpy(GlobalLock(hmem), text, len);
        GlobalUnlock(hmem);
        SetClipboardData(CF_UNICODETEXT, hmem);
    }
    CloseClipboard();
}

/*
---------------------------------------
    ZXing 图形码识别
---------------------------------------
*/
static void_t
zxing_decode (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   word,
  __CR_IN__ sint_t          cpage
    )
{
    size_t      cnts;
    ansi_t*     memo;
    ximage_t    matz;
    str_lstA_t  text;

    matz = imglab_crh2mat_set(img);
    if (matz == NULL)
        return;
    memo = NULL;
    text = NULL;
    cnts = imglab_zxi_grpcode_doit(matz, &text, NULL);
    if (cnts != 0 && text != NULL) {
        for (size_t idx = 0; idx < cnts; idx++)
        {
            ansi_t* str = text[idx * 2 + 1];

            if (str != NULL)
            {
                ansi_t* tmp;

                if (word != NULL) {
                    if (str_strIA(str, word) == NULL)
                        continue;
                }
                if (memo == NULL) {
                    memo = str_fmtA("%s\n", str);
                }
                else {
                    tmp = str_fmtA("%s\n%s\n", memo, str);
                    mem_free(memo);
                    memo = tmp;
                }
            }
        }
        if (memo != NULL)
        {
            wide_t* ucs2;

            printf("%s", memo);
            if (cpage <= 0)
                cpage = get_sys_codepage();
            ucs2 = local_to_utf16(cpage, memo);
            if (ucs2 != NULL) {
                text2clipboard(ucs2);
                mem_free(ucs2);
            }
        }
        strlst_freeA(text, cnts * 2);
    }
    imglab_mat_del(matz);
    TRY_FREE(memo);
}

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    uint_t  ddtype;
    sIMAGE* screen;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 <解码类型> [解码参数]... */
    if (argc < 2) {
        usage();
        return (QST_ERROR);
    }

    /* 保存当前截屏 */
    screen = desktop_capture();
    if (screen == NULL) {
        printf("desktop_capture() failure!\n");
        return (QST_ERROR);
    }
    save_bmp32(screen);

    /* 根据解码类型执行对应的操作 */
    ddtype = str2intxA(argv[1]);
    switch (ddtype)
    {
        default:
            usage();
            return (QST_ERROR);

        case 0:     /* ZXing 图形码识别 */
        {
            sint_t          page = -1;
            const ansi_t*   word = NULL;

            /* 参数解析 [字符串编码] [过滤单词] */
            if (argc > 2) {
                page = str2intxA(argv[2]);
                if (argc > 3)
                    word = argv[3];
            }
            zxing_decode(screen, word, page);
            break;
        }
    }
    image_del(screen);
    return (QST_OKAY);
}
