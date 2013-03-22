#if     defined(QST_MENU_HEADER)
//---------------------------------------------------------------------------
void __fastcall SetupMenu(void);
//---------------------------------------------------------------------------
void __fastcall subQstLibClick(TObject *Sender);
void __fastcall subQstRunClick(TObject *Sender);
void __fastcall subTreeCloseClick(TObject *Sender);
void __fastcall subG2dColorClick(TObject *Sender);
void __fastcall subG2dAlphaClick(TObject *Sender);
void __fastcall subWinShowClick(TObject *Sender);
void __fastcall subWinLoadClick(TObject *Sender);
void __fastcall subWinSaveClick(TObject *Sender);
void __fastcall subExitClick(TObject *Sender);
//---------------------------------------------------------------------------
void __fastcall subOpenQstCmdzClick(TObject *Sender);
void __fastcall subOpenQstIndexClick(TObject *Sender);
void __fastcall subOpenQstInfozClick(TObject *Sender);
void __fastcall subOpenQstMediaClick(TObject *Sender);
void __fastcall subOpenQstMountClick(TObject *Sender);
void __fastcall subOpenQstServClick(TObject *Sender);
void __fastcall subOpenQstTextClick(TObject *Sender);
void __fastcall subOpenQstTreeClick(TObject *Sender);
void __fastcall subOpenQstView2DClick(TObject *Sender);
void __fastcall subKillQstCmdzClick(TObject *Sender);
void __fastcall subKillQstIndexClick(TObject *Sender);
void __fastcall subKillQstInfozClick(TObject *Sender);
void __fastcall subKillQstMediaClick(TObject *Sender);
void __fastcall subKillQstMountClick(TObject *Sender);
void __fastcall subKillQstServClick(TObject *Sender);
void __fastcall subKillQstTextClick(TObject *Sender);
void __fastcall subKillQstTreeClick(TObject *Sender);
void __fastcall subKillQstView2DClick(TObject *Sender);
//---------------------------------------------------------------------------
void __fastcall subFilterNoneClick(TObject *Sender);
void __fastcall subFilterXXXXClick(TObject *Sender);
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
    misc_call_exe("RunWebIE.exe source\\import\\index.html", FALSE, FALSE);
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
    misc_call_exe("QstCmdz.exe script\\import\\reset.qst", TRUE, TRUE);
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
    QST_MENU_EVENT(subWinShow);
    QST_MENU_EVENT(subWinLoad);
    QST_MENU_EVENT(subWinSave);
    QST_MENU_EVENT(subExit);
    QST_MENU_EVENT(subOpenQstCmdz);
    QST_MENU_EVENT(subOpenQstIndex);
    QST_MENU_EVENT(subOpenQstInfoz);
    QST_MENU_EVENT(subOpenQstMedia);
    QST_MENU_EVENT(subOpenQstMount);
    QST_MENU_EVENT(subOpenQstServ);
    QST_MENU_EVENT(subOpenQstText);
    QST_MENU_EVENT(subOpenQstTree);
    QST_MENU_EVENT(subOpenQstView2D);
    QST_MENU_EVENT(subKillQstCmdz);
    QST_MENU_EVENT(subKillQstIndex);
    QST_MENU_EVENT(subKillQstInfoz);
    QST_MENU_EVENT(subKillQstMedia);
    QST_MENU_EVENT(subKillQstMount);
    QST_MENU_EVENT(subKillQstServ);
    QST_MENU_EVENT(subKillQstText);
    QST_MENU_EVENT(subKillQstTree);
    QST_MENU_EVENT(subKillQstView2D);
    QST_MENU_EVENT(subFilterNone);
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
            "filter\\*.xmlcall", add_filter_menu, temp_menu);
    }
}
//---------------------------------------------------------------------------
#endif
