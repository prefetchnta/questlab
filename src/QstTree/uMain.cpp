//---------------------------------------------------------------------------

#include <vcl.h>
#include "QstTree.h"
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ThemeMgr"
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
CR_API void_t   qst_free_lst2 (void_t);
CR_API bool_t   qst_load_xml2 (const ansi_t *name);
CR_API void_t   qst_try_load (PVirtualNode node, const sQTEE_file *data,
                              TVirtualStringTree *tree);
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
        : TForm(Owner)
{
    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 使用系统字体 */
    this->Font->Assign(Screen->MenuFont);

    /* 初始化节点大小 */
    lstTree->NodeDataSize = sizeof(sQTEE_file);

    /* 加载窗口位置配置 */
    misc_desk_init(WIN_ICONF, &x1, &y1, &ww, &hh,
                   this->Width, this->Height);
    this->Left   = x1;
    this->Top    = y1;
    this->Width  = ww;
    this->Height = hh;
    this->FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
    /* 区分上下区域 */
    lstTree->Height = (this->Height - sptTnB->Height) * 800 / 1000;
    sptTnB->MinSize = lstTree->Height * 200 / 1000;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::lstTreeFreeNode(TBaseVirtualTree *Sender,
      PVirtualNode Node)
{
    sQTEE_file* data;

    /* 释放节点数据 */
    data = (sQTEE_file*)lstTree->GetNodeData(Node);
    TRY_FREE(data->path)
    TRY_FREE(data->memo)
    if (data->attr & QST_FILE_DIR)
        mem_free(data->name);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::lstTreeGetText(TBaseVirtualTree *Sender,
      PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
      WideString &CellText)
{
    sQTEE_file* data;

    /* 设置显示文本 */
    data = (sQTEE_file*)lstTree->GetNodeData(Node);
    CellText = WideString((wchar_t*)data->name);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::lstTreeGetImageIndex(TBaseVirtualTree *Sender,
      PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
      bool &Ghosted, int &ImageIndex)
{
    sQTEE_file* data;

    /* 设置图标属性 */
    data = (sQTEE_file*)lstTree->GetNodeData(Node);
    ImageIndex = (int)(data->icon);

    /* 是否是隐藏文件 */
    if (data->attr & QST_FILE_HID)
        Ghosted = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::lstTreePaintText(TBaseVirtualTree *Sender,
      const TCanvas *TargetCanvas, PVirtualNode Node, TColumnIndex Column,
      TVSTTextType TextType)
{
    sQTEE_file* data;

    /* 设置文字颜色 */
    if (Node == lstTree->FocusedNode) {
        TargetCanvas->Font->Color = clHighlightText;
        return;
    }
    data = (sQTEE_file*)lstTree->GetNodeData(Node);
    if (data->attr & QST_FILE_ENC) {
        if (data->attr & QST_FILE_CMP)
            TargetCanvas->Font->Color = clRed;
        else
            TargetCanvas->Font->Color = clGreen;
    }
    else
    if (data->attr & QST_FILE_CMP) {
        TargetCanvas->Font->Color = clBlue;
    }
    else {
        TargetCanvas->Font->Color = clBlack;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    /* 直接导入列表文件 */
    if (Shift.Contains(ssCtrl)) {
        if (Key == 'C') {
            qst_free_lst2();
        }
        else
        if (Key == 'O') {
            if (!dlgOpen->Execute())
                return;
            qst_free_lst2();
            qst_load_xml2(dlgOpen->FileName.c_str());
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::lstTreeChange(TBaseVirtualTree *Sender,
      PVirtualNode Node)
{
    sQTEE_file* data;
    static PVirtualNode pre = NULL;

    /* 防止多次触发 */
    if (pre == Node)
        return;
    pre = Node;

    /* 过滤发送 */
    txtInfo->Clear();
    if (Node == NULL)
        return;
    data = (sQTEE_file*)lstTree->GetNodeData(Node);
    if (data->attr & QST_FILE_DIR)
        return;

    ansi_t*     text;
    AnsiString  string;

    /* 压入文件存放大小 */
    string  = "File size: ";
    string += IntToStr((__int64)data->pack);
    string += " Bytes";
    txtInfo->Lines->Append(string);

    /* 压入文件真实大小 */
    string  = "Real size: ";
    string += IntToStr((__int64)data->size);
    string += " Bytes";
    txtInfo->Lines->Append(string);

    /* 压入文件偏移位置 */
    string  = "File offset: ";
    string += IntToStr((__int64)data->offs);
    txtInfo->Lines->Append(string);

    /* 压入文件编码信息 */
    string  = "Name code page: ";
    string += IntToStr((int)data->page);
    txtInfo->Lines->Append(string);

    /* 压入文件备注信息 */
    if (data->memo != NULL) {
        text = utf16_to_local(data->page, data->memo);
        if (text != NULL) {
            string  = "Memo: ";
            string += AnsiString(text);
            txtInfo->Lines->Append(string);
            mem_free(text);
        }
    }

    /* 压入文件的全路径 */
    text = utf16_to_local(data->page, data->path);
    if (text != NULL) {
        string  = "Path: ";
        string += AnsiString(text);
        txtInfo->Lines->Append(string);
        mem_free(text);
    }

    /* 发送请求 */
    qst_try_load(Node, data, lstTree);
}
//---------------------------------------------------------------------------
