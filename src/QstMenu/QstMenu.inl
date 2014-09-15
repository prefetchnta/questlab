#if     defined(QST_MENU_HEADER)
//---------------------------------------------------------------------------
void __fastcall SetupMenu(void);
//---------------------------------------------------------------------------
void __fastcall subQstLibClick(TObject *Sender);
void __fastcall subQstRunClick(TObject *Sender);
void __fastcall subTreeCloseClick(TObject *Sender);
void __fastcall subG2dColorClick(TObject *Sender);
void __fastcall subG2dAlphaClick(TObject *Sender);
void __fastcall subG2dSaveNowClick(TObject *Sender);
void __fastcall subG2dSaveAllClick(TObject *Sender);
void __fastcall subG2dSaveShowClick(TObject *Sender);
void __fastcall subWinShowClick(TObject *Sender);
void __fastcall subWinLoadClick(TObject *Sender);
void __fastcall subWinSaveClick(TObject *Sender);
void __fastcall subExitClick(TObject *Sender);
//---------------------------------------------------------------------------
void __fastcall subOpenQstCmdzClick(TObject *Sender);
void __fastcall subOpenQstCommClick(TObject *Sender);
void __fastcall subOpenQstDataClick(TObject *Sender);
void __fastcall subOpenQstHashClick(TObject *Sender);
void __fastcall subOpenQstIndexClick(TObject *Sender);
void __fastcall subOpenQstInfozClick(TObject *Sender);
void __fastcall subOpenQstMediaClick(TObject *Sender);
void __fastcall subOpenQstMountClick(TObject *Sender);
void __fastcall subOpenQstServClick(TObject *Sender);
void __fastcall subOpenQstTextClick(TObject *Sender);
void __fastcall subOpenQstTreeClick(TObject *Sender);
void __fastcall subOpenQstView2DClick(TObject *Sender);
//---------------------------------------------------------------------------
void __fastcall subKillQstCmdzClick(TObject *Sender);
void __fastcall subKillQstCommClick(TObject *Sender);
void __fastcall subKillQstDataClick(TObject *Sender);
void __fastcall subKillQstHashClick(TObject *Sender);
void __fastcall subKillQstIndexClick(TObject *Sender);
void __fastcall subKillQstInfozClick(TObject *Sender);
void __fastcall subKillQstMediaClick(TObject *Sender);
void __fastcall subKillQstMountClick(TObject *Sender);
void __fastcall subKillQstServClick(TObject *Sender);
void __fastcall subKillQstTextClick(TObject *Sender);
void __fastcall subKillQstTreeClick(TObject *Sender);
void __fastcall subKillQstView2DClick(TObject *Sender);
//---------------------------------------------------------------------------
void __fastcall subToolsSciTEClick(TObject *Sender);
//---------------------------------------------------------------------------
void __fastcall subFilterNoneClick(TObject *Sender);
void __fastcall subFilterXXXXClick(TObject *Sender);
//---------------------------------------------------------------------------
void __fastcall subImgLabXXXXClick(TObject *Sender);
//---------------------------------------------------------------------------
void __fastcall subCPgameClick(TObject *Sender);
void __fastcall subCPsystemClick(TObject *Sender);
void __fastcall subCP1256Click(TObject *Sender);
void __fastcall subCP1257Click(TObject *Sender);
void __fastcall subCP1250Click(TObject *Sender);
void __fastcall subCP936Click(TObject *Sender);
void __fastcall subCP950Click(TObject *Sender);
void __fastcall subCP1251Click(TObject *Sender);
void __fastcall subCP1253Click(TObject *Sender);
void __fastcall subCP1255Click(TObject *Sender);
void __fastcall subCP932Click(TObject *Sender);
void __fastcall subCP949Click(TObject *Sender);
void __fastcall subCP874Click(TObject *Sender);
void __fastcall subCP1254Click(TObject *Sender);
void __fastcall subCP1258Click(TObject *Sender);
void __fastcall subCP1252Click(TObject *Sender);
//---------------------------------------------------------------------------
void __fastcall subParamClick(TObject *Sender);
//---------------------------------------------------------------------------
void __fastcall subAboutClick(TObject *Sender);
//---------------------------------------------------------------------------
#elif   defined(QST_MENU_SOURCE)
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subQstLibClick(TObject *Sender)
{
    /* 导入脚本仓库 */
    misc_call_exe("RunWebIE.exe " QST_PATH_SOURCE
                  "import\\index.html", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subQstRunClick(TObject *Sender)
{
    /* 打开导入脚本 */
    if (!dlgOpen->Execute())
        return;

    AnsiString  line;

    line = "QstCmdz.exe \"";
    line += dlgOpen->FileName;
    line += "\"";
    misc_call_exe(line.c_str(), FALSE, TRUE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subTreeCloseClick(TObject *Sender)
{
    /* 关闭导入文件 */
    misc_call_exe("QstCmdz.exe " QST_PATH_SCRIPT
                  "import\\reset.qst", TRUE, TRUE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subG2dColorClick(TObject *Sender)
{
    /* 显示图片色彩通道 */
    qst_send_cmdz("g2d:color");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subG2dAlphaClick(TObject *Sender)
{
    /* 显示图片透明通道 */
    qst_send_cmdz("g2d:alpha");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
typedef struct
{
        sQST_CTX    ctx;
        uint_t      typ;
        TfrmMain*   frm;

} sG2dSaveCtx;

/*
---------------------------------------
    保存当前图片帧线程
---------------------------------------
*/
static uint_t STDCALL
g2d_img_save (
  __CR_IN__ void_t* param
    )
{
    ansi_t*     file;
    ansi_t*     send;
    sG2dSaveCtx temp;

    struct_cpy(&temp, param, sG2dSaveCtx);
    misc_async_okay((sQST_CTX*)param);
    temp.frm->Enabled = false;
    misc_call_exe("xSelectFile.exe save", TRUE, FALSE);
    if (file_existA(QST_STOPS_NEXT))
        goto _func_out;
    file = file_load_as_strA(QST_SAVE_FILEX);
    if (file == NULL)
        goto _func_out;

    /* 不加引号可输入附加参数
       但是文件名里就不支持空格了 */
    if (temp.typ == 0)
        send = str_fmtA("g2d:save %s", file);
    else
    if (temp.typ == 1)
        send = str_fmtA("g2d:saveall %s", file);
    else
        send = str_fmtA("g2d:savenow %s", file);
    mem_free(file);
    if (send != NULL) {
        qst_send_cmdz(send);
        mem_free(send);
    }
_func_out:
    temp.frm->Enabled = true;
    return (TRUE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subG2dSaveNowClick(TObject *Sender)
{
    sG2dSaveCtx parm;

    /* 保存当前图片帧 */
    parm.frm = this;
    parm.typ = 0;
    misc_async_call(g2d_img_save, &parm.ctx);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subG2dSaveAllClick(TObject *Sender)
{
    sG2dSaveCtx parm;

    /* 保存所有图片帧 */
    parm.frm = this;
    parm.typ = 1;
    misc_async_call(g2d_img_save, &parm.ctx);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subG2dSaveShowClick(TObject *Sender)
{
    sG2dSaveCtx parm;

    /* 保存显示图片帧 */
    parm.frm = this;
    parm.typ = 2;
    misc_async_call(g2d_img_save, &parm.ctx);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subWinShowClick(TObject *Sender)
{
    /* 显示所有窗口 */
    qst_send_cmdz("win:show");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subWinLoadClick(TObject *Sender)
{
    /* 加载窗口位置 */
    qst_mnu_win_load2();
    qst_send_cmdz("win:load");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subWinSaveClick(TObject *Sender)
{
    /* 保存窗口位置 */
    qst_mnu_win_save2();
    qst_send_cmdz("win:save");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subExitClick(TObject *Sender)
{
    /* 退出整个系统 */
    this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstCmdzClick(TObject *Sender)
{
    /* 打开 QstCmdz 窗口 */
    qst_send_cmdz("qcmd:win:show");
    misc_call_exe("QstCmdz.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstCommClick(TObject *Sender)
{
    /* 打开 QstComm 窗口 */
    qst_send_cmdz("qcom:win:show");
    misc_call_exe("QstComm.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstDataClick(TObject *Sender)
{
    /* 打开 QstData 窗口 */
    qst_send_cmdz("qdat:win:show");
    misc_call_exe("QstData.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstHashClick(TObject *Sender)
{
    /* 打开 QstHash 窗口 */
    qst_send_cmdz("qhsh:win:show");
    misc_call_exe("QstHash.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstIndexClick(TObject *Sender)
{
    /* 打开 QstIndex 窗口 */
    qst_send_cmdz("qidx:win:show");
    misc_call_exe("QstIndex.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstInfozClick(TObject *Sender)
{
    /* 打开 QstInfoz 窗口 */
    qst_send_cmdz("qinf:win:show");
    misc_call_exe("QstInfoz.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstMediaClick(TObject *Sender)
{
    /* 打开 QstMedia 窗口 */
    qst_send_cmdz("qxmm:win:show");
    misc_call_exe("QstMedia.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstMountClick(TObject *Sender)
{
    /* 打开 QstMount 窗口 */
    qst_send_cmdz("qmnt:win:show");
    misc_call_exe("QstMount.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstServClick(TObject *Sender)
{
    /* 打开 QstServ 窗口 */
    qst_send_cmdz("qsrv:win:show");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstTextClick(TObject *Sender)
{
    /* 打开 QstText 窗口 */
    qst_send_cmdz("qedt:win:show");
    misc_call_exe("QstText.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstTreeClick(TObject *Sender)
{
    /* 打开 QstTree 窗口 */
    qst_send_cmdz("qtee:win:show");
    misc_call_exe("QstTree.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subOpenQstView2DClick(TObject *Sender)
{
    /* 打开 QstView2D 窗口 */
    qst_send_cmdz("qv2d:win:show");
    misc_call_exe("QstView2D.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstCmdzClick(TObject *Sender)
{
    /* 关闭 QstCmdz 窗口 */
    qst_send_cmdz("qcmd:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstCommClick(TObject *Sender)
{
    /* 关闭 QstComm 窗口 */
    qst_send_cmdz("qcom:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstDataClick(TObject *Sender)
{
    /* 关闭 QstData 窗口 */
    qst_send_cmdz("qdat:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstHashClick(TObject *Sender)
{
    /* 关闭 QstHash 窗口 */
    qst_send_cmdz("qhsh:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstIndexClick(TObject *Sender)
{
    /* 关闭 QstIndex 窗口 */
    qst_send_cmdz("qidx:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstInfozClick(TObject *Sender)
{
    /* 关闭 QstInfoz 窗口 */
    qst_send_cmdz("qinf:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstMediaClick(TObject *Sender)
{
    /* 关闭 QstMedia 窗口 */
    qst_send_cmdz("qxmm:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstMountClick(TObject *Sender)
{
    /* 关闭 QstMount 窗口 */
    qst_send_cmdz("qmnt:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstServClick(TObject *Sender)
{
    /* 关闭 QstServ 窗口 */
    qst_send_cmdz("qsrv:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstTextClick(TObject *Sender)
{
    /* 关闭 QstText 窗口 */
    qst_send_cmdz("qedt:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstTreeClick(TObject *Sender)
{
    /* 关闭 QstTree 窗口 */
    qst_send_cmdz("qtee:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subKillQstView2DClick(TObject *Sender)
{
    /* 关闭 QstView2D 窗口 */
    qst_send_cmdz("qv2d:app:exit");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subToolsSciTEClick(TObject *Sender)
{
    /* 外部工具 SciTE 开关 */
    m_use_scite = !m_use_scite;
    ((TMenuItem*)Sender)->Checked = m_use_scite;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subFilterNoneClick(TObject *Sender)
{
    /* 取消图片滤镜功能 */
    qst_send_cmdz("qv2d:flt:load");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subFilterXXXXClick(TObject *Sender)
{
    int         size;
    AnsiString  line;

    /* 选择图片滤镜功能 */
    line = "qv2d:flt:load \"";
    line += ((TMenuItem*)Sender)->Caption;
    line += "\"";
    size = line.Length();
    for (int idx = 1; idx <= size; idx++) {
        if (line[idx] == '&') {
            line.Delete(idx, 1);
            idx  -= 1;
            size -= 1;
        }
    }
    qst_send_cmdz(line.c_str());
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subImgLabXXXXClick(TObject *Sender)
{
    int         size;
    AnsiString  line;

    /* 选择图像实验功能 */
    line = "xIMGLAB.exe ";
    line += ((TMenuItem*)Sender)->Caption;
    line += " :computer/camera0";
    size = line.Length();
    for (int idx = 1; idx <= size; idx++) {
        if (line[idx] == '&') {
            line.Delete(idx, 1);
            idx  -= 1;
            size -= 1;
        }
    }
    misc_call_exe(line.c_str(), FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCPgameClick(TObject *Sender)
{
    /* Game Code Page */
    qst_send_cmdz("edt:cpage 0");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCPsystemClick(TObject *Sender)
{
    AnsiString  line;

    /* System Code Page */
    line = "edt:cpage ";
    line += IntToStr(get_sys_codepage());
    qst_send_cmdz(line.c_str());
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP1256Click(TObject *Sender)
{
    /* Arabic [1256] */
    qst_send_cmdz("edt:cpage 1256");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP1257Click(TObject *Sender)
{
    /* Baltic [1257] */
    qst_send_cmdz("edt:cpage 1257");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP1250Click(TObject *Sender)
{
    /* Central European [1250] */
    qst_send_cmdz("edt:cpage 1250");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP936Click(TObject *Sender)
{
    /* Chinese (Simplified) [936] */
    qst_send_cmdz("edt:cpage 936");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP950Click(TObject *Sender)
{
    /* Chinese (Traditional) [950] */
    qst_send_cmdz("edt:cpage 950");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP1251Click(TObject *Sender)
{
    /* Cyrillic [1251] */
    qst_send_cmdz("edt:cpage 1251");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP1253Click(TObject *Sender)
{
    /* Greek [1253] */
    qst_send_cmdz("edt:cpage 1253");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP1255Click(TObject *Sender)
{
    /* Hebrew [1255] */
    qst_send_cmdz("edt:cpage 1255");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP932Click(TObject *Sender)
{
    /* Japanese [932] */
    qst_send_cmdz("edt:cpage 932");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP949Click(TObject *Sender)
{
    /* Korean [949] */
    qst_send_cmdz("edt:cpage 949");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP874Click(TObject *Sender)
{
    /* Thai [874] */
    qst_send_cmdz("edt:cpage 874");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP1254Click(TObject *Sender)
{
    /* Turkish [1254] */
    qst_send_cmdz("edt:cpage 1254");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP1258Click(TObject *Sender)
{
    /* Vietnamese [1258] */
    qst_send_cmdz("edt:cpage 1258");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subCP1252Click(TObject *Sender)
{
    /* Western European [1252] */
    qst_send_cmdz("edt:cpage 1252");
    ((TMenuItem*)Sender)->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subParamClick(TObject *Sender)
{
    /* 参数菜单项 */
    misc_call_exe("xParamSet.exe", FALSE, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::subAboutClick(TObject *Sender)
{
    /* 关于菜单项 */
    frmAbout->ShowModal();
}
//---------------------------------------------------------------------------
/*
---------------------------------------
    添加图片滤镜菜单
---------------------------------------
*/
static bool_t
add_filter_menu (
  __CR_IN__ void_t*     param,
  __CR_IN__ sSEARCHa*   finfo
    )
{
    TMenuItem*  item;
    TMenuItem*  parent;

    /* 过滤文件大小 */
    if (finfo->size == 0)
        return (TRUE);

    ansi_t          name[128];
    static uint_t   name_idx = 0;

    /* 用文件路径做菜单文字 */
    parent = (TMenuItem*)param;
    sprintf(name, "subFilter%04u", name_idx++);
    item = NewItem(str_uprA(&finfo->name[2]), 0, false, true,
                   frmMain->subFilterXXXXClick, 0, name);
    item->RadioItem = true;
    item->GroupIndex = 1;
    parent->Add(item);
    return (TRUE);
}
//---------------------------------------------------------------------------
/*
---------------------------------------
    添加图像实验菜单
---------------------------------------
*/
static bool_t
add_imglab_menu (
  __CR_IN__ void_t*     param,
  __CR_IN__ sSEARCHa*   finfo
    )
{
    TMenuItem*  item;
    TMenuItem*  parent;

    /* 过滤文件大小 */
    if (finfo->size == 0)
        return (TRUE);

    ansi_t          name[128];
    static uint_t   name_idx = 0;

    /* 用文件路径做菜单文字 */
    parent = (TMenuItem*)param;
    sprintf(name, "subImgLab%04u", name_idx++);
    item = NewItem(&finfo->name[2], 0, false, true,
                   frmMain->subImgLabXXXXClick, 0, name);
    parent->Add(item);
    return (TRUE);
}
//---------------------------------------------------------------------------
/*
---------------------------------------
    成员比较回调
---------------------------------------
*/
static bool_t
unit_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    sMenuEvent* unit = (sMenuEvent*)obj;

    return (str_cmpA(unit->name, (ansi_t*)key) == 0);
}

/*
---------------------------------------
    成员索引回调
---------------------------------------
*/
static uint_t
unit_find (
  __CR_IN__ const void_t*   key
    )
{
    return (hash_str_djb2(key, str_lenA((ansi_t*)key)));
}

/* 事件注册简化宏 */
#define QST_MENU_EVENT(_name_) \
    tmp.name = #_name_; \
    tmp.call = this->##_name_##Click; \
    curbead_insertT(&tbl, sMenuEvent, tmp.name, &tmp)
//---------------------------------------------------------------------------
void __fastcall TfrmMain::SetupMenu(void)
{
    int         idx;
    sCURBEAD    tbl;
    sMenuEvent  tmp;

    /* 加载所有事件到哈希表 */
    if (!curbead_initT(&tbl, sMenuEvent, 0))
        return;
    tbl.find = unit_find;
    tbl.comp = unit_comp;

    /* 逐个注册事件 */
    QST_MENU_EVENT(subQstLib);
    QST_MENU_EVENT(subQstRun);
    QST_MENU_EVENT(subTreeClose);
    QST_MENU_EVENT(subG2dColor);
    QST_MENU_EVENT(subG2dAlpha);
    QST_MENU_EVENT(subG2dSaveNow);
    QST_MENU_EVENT(subG2dSaveAll);
    QST_MENU_EVENT(subG2dSaveShow);
    QST_MENU_EVENT(subWinShow);
    QST_MENU_EVENT(subWinLoad);
    QST_MENU_EVENT(subWinSave);
    QST_MENU_EVENT(subExit);
    QST_MENU_EVENT(subOpenQstCmdz);
    QST_MENU_EVENT(subOpenQstComm);
    QST_MENU_EVENT(subOpenQstData);
    QST_MENU_EVENT(subOpenQstHash);
    QST_MENU_EVENT(subOpenQstIndex);
    QST_MENU_EVENT(subOpenQstInfoz);
    QST_MENU_EVENT(subOpenQstMedia);
    QST_MENU_EVENT(subOpenQstMount);
    QST_MENU_EVENT(subOpenQstServ);
    QST_MENU_EVENT(subOpenQstText);
    QST_MENU_EVENT(subOpenQstTree);
    QST_MENU_EVENT(subOpenQstView2D);
    QST_MENU_EVENT(subKillQstCmdz);
    QST_MENU_EVENT(subKillQstComm);
    QST_MENU_EVENT(subKillQstData);
    QST_MENU_EVENT(subKillQstHash);
    QST_MENU_EVENT(subKillQstIndex);
    QST_MENU_EVENT(subKillQstInfoz);
    QST_MENU_EVENT(subKillQstMedia);
    QST_MENU_EVENT(subKillQstMount);
    QST_MENU_EVENT(subKillQstServ);
    QST_MENU_EVENT(subKillQstText);
    QST_MENU_EVENT(subKillQstTree);
    QST_MENU_EVENT(subKillQstView2D);
    QST_MENU_EVENT(subToolsSciTE);
    QST_MENU_EVENT(subFilterNone);
    QST_MENU_EVENT(subCPgame);
    QST_MENU_EVENT(subCPsystem);
    QST_MENU_EVENT(subCP1256);
    QST_MENU_EVENT(subCP1257);
    QST_MENU_EVENT(subCP1250);
    QST_MENU_EVENT(subCP936);
    QST_MENU_EVENT(subCP950);
    QST_MENU_EVENT(subCP1251);
    QST_MENU_EVENT(subCP1253);
    QST_MENU_EVENT(subCP1255);
    QST_MENU_EVENT(subCP932);
    QST_MENU_EVENT(subCP949);
    QST_MENU_EVENT(subCP874);
    QST_MENU_EVENT(subCP1254);
    QST_MENU_EVENT(subCP1258);
    QST_MENU_EVENT(subCP1252);
    QST_MENU_EVENT(subParam);
    QST_MENU_EVENT(subAbout);

    TMenuItem*  temp_menu;
    TMenuItem*  root_menu;

    /* 加载菜单配置文件 */
    root_menu = mnuMain->Items;
    qst_load_menu(root_menu, &tbl);
    curbead_freeT(&tbl, sMenuEvent);

    /* 根据滤镜脚本文件添加菜单 */
    for (idx = 0; idx < root_menu->Count; idx++) {
        temp_menu = root_menu->Items[idx];
        if (temp_menu->Name == "subFilter2D")
            break;
    }
    if (idx < root_menu->Count) {
        file_searchA(QST_PATH_SCRIPT, TRUE, TRUE, FALSE,
            "filter\\*.f2d", add_filter_menu, temp_menu);
    }

    /* 根据图像实验插件添加菜单 */
    for (idx = 0; idx < root_menu->Count; idx++) {
        temp_menu = root_menu->Items[idx];
        if (temp_menu->Name == "subImgLab2D")
            break;
    }
    if (idx < root_menu->Count) {
        file_searchA(QST_PATH_PLUGIN, FALSE, TRUE, FALSE,
                "*.ilab", add_imglab_menu, temp_menu);
    }
}
//---------------------------------------------------------------------------
#endif
