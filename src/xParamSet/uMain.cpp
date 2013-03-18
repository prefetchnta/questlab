//---------------------------------------------------------------------------

#include <vcl.h>
#include "xParamSet.h"
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
inline TColor rgb2gdi (int32u color)
{
    return ((TColor)argb32_to_gdi(&color));
}
inline int32u gdi2rgb (TColor color)
{
    int32u  ctemp = (int32u)color;

    return ((int32u)argb32_to_gdi(&ctemp));
}
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
        : TForm(Owner)
{
    sINIu*  ini;
    ansi_t* str;
    ansi_t* name;
    sint_t  size;
    int32u  color;

    /* 使用系统字体 */
    this->Font->Assign(Screen->MenuFont);

_qst_view2d:
    /*********************/
    /***** QstView2D *****/
    /*********************/
    txtQV2D_bkcolor->Color = rgb2gdi(0x007F7F7FUL);
    isQV2D_is_blends->Checked = true;
    isQV2D_is_center->Checked = true;
    isQV2D_use_keycolor->Checked = true;
    isQV2D_use_keyalpha->Checked = true;
    txtQV2D_def_keycolor->Color = rgb2gdi(0x00000000UL);
    str = file_load_as_strA(QST_PATH_CONFIG "QstView2D.ini");
    if (str == NULL)
        goto _qst_media;
    ini = ini_parseU(str);
    mem_free(str);
    if (ini == NULL)
        goto _qst_media;
    color = ini_key_intx32U("qv2d::bkcolor", 0x007F7F7FUL, ini);
    txtQV2D_bkcolor->Color = rgb2gdi(color);
    if (ini_key_intxU("qv2d::is_blends", TRUE, ini))
        isQV2D_is_blends->Checked = true;
    else
        isQV2D_is_blends->Checked = false;
    if (ini_key_intxU("qv2d::is_center", TRUE, ini))
        isQV2D_is_center->Checked = true;
    else
        isQV2D_is_center->Checked = false;
    if (ini_key_intxU("qv2d::use_keycolor", TRUE, ini))
        isQV2D_use_keycolor->Checked = true;
    else
        isQV2D_use_keycolor->Checked = false;
    color = ini_key_intx32U("qv2d::def_keycolor", 0x00000000UL, ini);
    if (color & 0xFF000000UL)
        isQV2D_use_keyalpha->Checked = false;
    else
        isQV2D_use_keyalpha->Checked = true;
    txtQV2D_def_keycolor->Color = rgb2gdi(color);
    ini_closeU(ini);

_qst_media:
    /********************/
    /***** QstMedia *****/
    /********************/
    isQXMM_enabled->Checked = true;
    isQXMM_loop_play->Checked = false;
    str = file_load_as_strA(QST_PATH_CONFIG "QstMedia.ini");
    if (str == NULL)
        goto _qst_text;
    ini = ini_parseU(str);
    mem_free(str);
    if (ini == NULL)
        goto _qst_text;
    if (ini_key_intxU("qxmm::enabled", TRUE, ini))
        isQXMM_enabled->Checked = true;
    else
        isQXMM_enabled->Checked = false;
    if (ini_key_intxU("qxmm::loop_play", FALSE, ini))
        isQXMM_loop_play->Checked = true;
    else
        isQXMM_loop_play->Checked = false;
    ini_closeU(ini);

_qst_text:
    /*******************/
    /***** QstText *****/
    /*******************/
    btnQEDT_font->Caption = "Fixedsys, 12";
    edtQEDT_max_size_kb->Text = "20480";
    str = file_load_as_strA(QST_PATH_CONFIG "QstText.ini");
    if (str == NULL)
        goto _func_out;
    ini = ini_parseU(str);
    mem_free(str);
    if (ini == NULL)
        goto _func_out;
    size = ini_key_intxU("qedt::font_size", 12, ini);
    name = ini_key_stringU("qedt::font_face", ini);
    if (name != NULL) {
        str = name;
        name = utf8_to_local(CR_LOCAL, str);
        mem_free(str);
    }
    if (name == NULL) {
        str = str_fmtA("Fixedsys, %d", size);
    }
    else {
        str = str_fmtA("%s, %d", name, size);
        mem_free(name);
    }
    if (str != NULL) {
        btnQEDT_font->Caption = AnsiString(str);
        mem_free(str);
    }
    SetWindowLong(edtQEDT_max_size_kb->Handle, GWL_STYLE,
        GetWindowLong(edtQEDT_max_size_kb->Handle, GWL_STYLE) | ES_CENTER);
    edtQEDT_max_size_kb->Invalidate();
    size = ini_key_intxU("qedt::max_size_kb", 20480, ini);
    if (size < 0) size = 0;
    edtQEDT_max_size_kb->Text = IntToStr(size);
    ini_closeU(ini);

_func_out:
    return;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnOKClick(TObject *Sender)
{
    /* 确定按钮 */
    this->btnApplyClick(NULL);
    this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnApplyClick(TObject *Sender)
{
    FILE*       fp;
    ansi_t*     str;
    ansi_t*     name;
    sint_t      size;
    int32u      color;
    AnsiString  stemp;

    /* 应用按钮 */
_qst_view2d:
    /*********************/
    /***** QstView2D *****/
    /*********************/
    fp = fopen(QST_PATH_CONFIG "QstView2D.ini", "w");
    if (fp == NULL)
        goto _qst_media;
    color = gdi2rgb(txtQV2D_bkcolor->Color);
    fprintf(fp, "qv2d::bkcolor = 0x%08X\n", color);
    if (isQV2D_is_blends->Checked)
        fprintf(fp, "qv2d::is_blends = 1\n");
    else
        fprintf(fp, "qv2d::is_blends = 0\n");
    if (isQV2D_is_center->Checked)
        fprintf(fp, "qv2d::is_center = 1\n");
    else
        fprintf(fp, "qv2d::is_center = 0\n");
    if (isQV2D_use_keycolor->Checked)
        fprintf(fp, "qv2d::use_keycolor = 1\n");
    else
        fprintf(fp, "qv2d::use_keycolor = 0\n");
    color = gdi2rgb(txtQV2D_def_keycolor->Color);
    if (isQV2D_use_keyalpha->Checked)
        color &= 0x00FFFFFFUL;
    else
        color |= 0xFF000000UL;
    fprintf(fp, "qv2d::def_keycolor = 0x%08X\n", color);
    fclose(fp);

_qst_media:
    /********************/
    /***** QstMedia *****/
    /********************/
    fp = fopen(QST_PATH_CONFIG "QstMedia.ini", "w");
    if (fp == NULL)
        goto _qst_text;
    if (isQXMM_enabled->Checked)
        fprintf(fp, "qxmm::enabled = 1\n");
    else
        fprintf(fp, "qxmm::enabled = 0\n");
    if (isQXMM_loop_play->Checked)
        fprintf(fp, "qxmm::loop_play = 1\n");
    else
        fprintf(fp, "qxmm::loop_play = 0\n");
    fclose(fp);

_qst_text:
    /*******************/
    /***** QstText *****/
    /*******************/
    fp = fopen(QST_PATH_CONFIG "QstText.ini", "w");
    if (fp == NULL)
        goto _func_out;
    stemp = btnQEDT_font->Caption;
    name = stemp.c_str();
    str = str_chrA(name, CR_AC(','));
    if (str == NULL) {
        fprintf(fp, "qedt::font_size = 12\n");
    }
    else {
        *str = NIL;
        fprintf(fp, "qedt::font_size = %s\n", str + 2);
    }
    str = local_to_utf8(CR_LOCAL, name);
    if (str == NULL) {
        fprintf(fp, "qedt::font_face = \"Fixedsys\"\n");
    }
    else {
        fprintf(fp, "qedt::font_face = \"%s\"\n", str);
        mem_free(str);
    }
    size = StrToIntDef(edtQEDT_max_size_kb->Text, 20480);
    if (size < 0) size = 0;
    edtQEDT_max_size_kb->Text = IntToStr(size);
    fprintf(fp, "qedt::max_size_kb = %u\n", size);
    fclose(fp);

_func_out:
    /* 发送参数加载命令 */
    misc_call_exe("QstCmdz.exe script\\import\\param.qst", TRUE, TRUE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnCancelClick(TObject *Sender)
{
    /* 取消按钮 */
    this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::txtQV2D_bkcolorClick(TObject *Sender)
{
    /* 选择背景颜色 */
    dlgColor->Color = txtQV2D_bkcolor->Color;
    if (!dlgColor->Execute())
        return;
    txtQV2D_bkcolor->Color = dlgColor->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::txtQV2D_def_keycolorClick(TObject *Sender)
{
    /* 选择关键颜色 */
    dlgColor->Color = txtQV2D_def_keycolor->Color;
    if (!dlgColor->Execute())
        return;
    txtQV2D_def_keycolor->Color = dlgColor->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnQEDT_fontClick(TObject *Sender)
{
    ansi_t*     str;
    ansi_t*     name;
    sint_t      size;
    AnsiString  stemp;

    /* 选择全局字体 */
    stemp = btnQEDT_font->Caption;
    name = stemp.c_str();
    str = str_chrA(name, CR_AC(','));
    if (str == NULL) {
        size = 12;
    }
    else {
        *str = NIL;
        size = StrToIntDef(str + 2, 12);
    }
    dlgFont->Font->Name = AnsiString(name);
    dlgFont->Font->Size = size;
    if (!dlgFont->Execute())
        return;
    stemp = IntToStr(dlgFont->Font->Size);
    btnQEDT_font->Caption = dlgFont->Font->Name;
    btnQEDT_font->Caption = btnQEDT_font->Caption + ", ";
    btnQEDT_font->Caption = btnQEDT_font->Caption + stemp;
}
//---------------------------------------------------------------------------
