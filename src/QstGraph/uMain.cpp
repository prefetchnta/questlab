//---------------------------------------------------------------------------

#include <vcl.h>
#include "QstGraph.h"
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
        : TForm(Owner)
{
    /* 使用系统字体 */
    this->Font->Assign(Screen->MenuFont);
    frmMain->DoubleBuffered = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setup(unsigned int type, const AnsiString& title,
      int left, int top, unsigned int width, unsigned int height)
{
    int ww, hh;

    /* 设置曲线参数 */
    this->setBottom(0);
    this->setLeft(0, 0);
    ww = ChartMain->BottomAxis->Maximum;
    hh = ChartMain->LeftAxis->Maximum - ChartMain->LeftAxis->Minimum;
    ChartMain->Title->Text->Text = title;
    m_type = type;
    switch (type)
    {
        default:
        case 1:  m_line = (TChartSeries*)Series1;  break;
        case 2:  m_line = (TChartSeries*)Series2;  break;
        case 3:  m_line = (TChartSeries*)Series3;  break;
        case 4:  m_line = (TChartSeries*)Series4;  break;
        case 5:  m_line = (TChartSeries*)Series5;  break;
        case 6:  m_line = (TChartSeries*)Series6;  break;
        case 7:  m_line = (TChartSeries*)Series7;  break;
        case 8:  m_line = (TChartSeries*)Series8;  break;
        case 9:  m_line = (TChartSeries*)Series9;  break;
        case 10: m_line = (TChartSeries*)Series10; break;
        case 11: m_line = (TChartSeries*)Series11; break;
    }
    m_line->Active = true;
    m_line->ParentChart = ChartMain;
    for (int xx = 0; xx <= ww; xx++)
        m_line->AddXY(xx, random(hh) - hh / 2, "", m_line->SeriesColor);
    m_xx = 0;

    /* 设置窗口位置 */
    if (left >= 0 && top >= 0) {
        this->Position = poDesigned;
        this->Left = left;
        this->Top  = top;
        if (width  > 1) this->Width  = width;
        if (height > 1) this->Height = height;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::clear()
{
    /* 清除所有数据 */
    m_line->Clear();
    this->setBottom(m_ww);
    m_xx = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setBottom(unsigned int max)
{
    /* 设置横坐标 */
    try {
        if (ChartMain->BottomAxis->Maximum < 0) {
            if (max == 0)   /* 根据窗口的宽度决定横轴长度 */
                ChartMain->BottomAxis->Maximum = this->Width / 10;
            else
                ChartMain->BottomAxis->Maximum = max;
            ChartMain->BottomAxis->Minimum = 0;
        }
        else {
            ChartMain->BottomAxis->Minimum = 0;
            if (max == 0)   /* 根据窗口的宽度决定横轴长度 */
                ChartMain->BottomAxis->Maximum = this->Width / 10;
            else
                ChartMain->BottomAxis->Maximum = max;
        }
    }
    catch (...) {
        MessageBoxA(this->Handle, "Invalid Bottom Axis!",
                    "ERROR", MB_OK | MB_ICONWARNING);
        return;
    }
    m_ww = ChartMain->BottomAxis->Maximum;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setBottom(double min, double max)
{
    /* 设置横坐标 */
    if (min >= max) {
        this->setBottom(0);
    }
    else {
        try {
            if (max < ChartMain->BottomAxis->Minimum) {
                ChartMain->BottomAxis->Minimum = min;
                ChartMain->BottomAxis->Maximum = max;
            }
            else {
                ChartMain->BottomAxis->Maximum = max;
                ChartMain->BottomAxis->Minimum = min;
            }
        }
        catch (...) {
            MessageBoxA(this->Handle, "Invalid Bottom Axis!",
                        "ERROR", MB_OK | MB_ICONWARNING);
            return;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setLeft(double min, double max)
{
    /* 设置纵坐标 */
    if (min >= max) {
        m_auto_v = true;
    }
    else {
        m_auto_v = false;
        try {
            if (max < ChartMain->LeftAxis->Minimum) {
                ChartMain->LeftAxis->Minimum = min;
                ChartMain->LeftAxis->Maximum = max;
            }
            else {
                ChartMain->LeftAxis->Maximum = max;
                ChartMain->LeftAxis->Minimum = min;
            }
        }
        catch (...) {
            MessageBoxA(this->Handle, "Invalid Left Axis!",
                        "ERROR", MB_OK | MB_ICONWARNING);
            return;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setLineColor(int32u color)
{
    TColor  clrs;

    clrs = (TColor)argb32_to_gdi(&color);
    m_line->SeriesColor = clrs;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setValue(double value, bool move)
{
    int min = (int)(value - 10);
    int max = (int)(value + 10);

    /* 处理横坐标 */
    if (move) {
        if (m_xx > m_ww) {
            ChartMain->BottomAxis->Maximum++;
            ChartMain->BottomAxis->Minimum++;
        }
    }

    /* 添加数据点 */
    if (m_xx == 0) {
        m_min = value;
        m_max = value;
        if (m_auto_v) {
            try {
                if (max < ChartMain->LeftAxis->Minimum) {
                    ChartMain->LeftAxis->Minimum = min;
                    ChartMain->LeftAxis->Maximum = max;
                }
                else {
                    ChartMain->LeftAxis->Maximum = max;
                    ChartMain->LeftAxis->Minimum = min;
                }
            }
            catch (...) {
                MessageBoxA(this->Handle, "Invalid Left Axis!",
                            "ERROR", MB_OK | MB_ICONWARNING);
                return;
            }
        }
    }
    else {
        if (m_min > value) {
            m_min = value;
            if (m_auto_v)
                ChartMain->LeftAxis->Minimum = min;
        }
        else
        if (m_max < value) {
            m_max = value;
            if (m_auto_v)
                ChartMain->LeftAxis->Maximum = max;
        }
    }
    m_line->AddXY(m_xx++, value, "", m_line->SeriesColor);

    /* 处理横坐标 */
    if (!move) {
        if (m_xx > m_ww) {
            m_ww = m_xx;
            ChartMain->BottomAxis->Maximum = m_ww;
        }
    }
}
//---------------------------------------------------------------------------
