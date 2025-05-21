//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include "QstGraph.h"
#pragma hdrstop

#include "uMain.h"
#define MAX_PEN_WIDTH   20
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
    m_type = type;
    if (this->isChart())
    {
        int ww, hh;

        /* 设置曲线参数 */
        this->setBottom(0);
        this->setLeft(0, 0);
        ww = ChartMain->BottomAxis->Maximum;
        hh = ChartMain->LeftAxis->Maximum - ChartMain->LeftAxis->Minimum;
        ChartMain->Title->Text->Text = title;
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
        m_min = m_max = 0;
        ShowMain->Visible = false;
        ShowMain->Enabled = false;
        ChartMain->Visible = true;
        ChartMain->Enabled = true;
    }
    else
    {
        /* 其他图表类型 */
        ShowMain->Visible = true;
        ShowMain->Enabled = true;
        ChartMain->Visible = false;
        ChartMain->Enabled = false;
        this->Caption = this->Caption + " - ";
        this->Caption = this->Caption + title;
        m_crt = 0;
        m_pen = MAX_PEN_WIDTH;
        m_clr_bg = clRed;
        m_clr_fg = clBlue;
    }

    /* 设置窗口位置 */
    if (left >= 0 && top >= 0) {
        this->Position = poDesigned;
        this->Left = left;
        this->Top  = top;
        if (width  > 1) this->Width  = width;
        if (height > 1) this->Height = height;
    }
    this->FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
    int ww, hh;

    /* 角度图必须是正方形的 */
    if (this->isAngle()) {
        ww = ShowMain->Width;
        hh = ShowMain->Height;
        if (ww > hh)
            this->Height = this->Height + ww - hh;
        else
        if (ww < hh)
            this->Width = this->Width + hh - ww;
        if (ShowMain->Width >= ShowMain->Height)
            ww = ShowMain->Width;
        else
            ww = ShowMain->Height;
        ShowMain->Picture->Bitmap->Width  = ww;
        ShowMain->Picture->Bitmap->Height = ww;
        this->setAngle(m_crt);
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfrmMain::isAngle()
{
    return ((m_type == 0) ? true : false);
}
//---------------------------------------------------------------------------
bool __fastcall TfrmMain::isChart()
{
    return ((m_type >= 1 && m_type <= 11) ? true : false);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::clear()
{
    /* 清除所有数据 */
    if (this->isChart()) {
        m_line->Clear();
        this->setBottom(m_ww);
        m_xx = 0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setBottom(unsigned int max)
{
    /* 设置横坐标 */
    if (this->isChart()) {
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
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setBottom(double min, double max)
{
    /* 设置横坐标 */
    if (this->isChart()) {
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
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setLeft(double min, double max)
{
    /* 设置纵坐标 */
    if (this->isChart()) {
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
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setLineColor(unsigned int color, bool back)
{
    /* 设置绘制颜色 */
    if (back) {
        m_clr_bg = (TColor)argb32_to_gdi(&color);
        if (this->isChart()) {
            if (color == 0)
                ChartMain->BackColor = (TColor)clTeeColor;
            else
                ChartMain->BackColor = m_clr_bg;
        }
    }
    else {
        m_clr_fg = (TColor)argb32_to_gdi(&color);
        if (this->isChart())
            m_line->SeriesColor = m_clr_fg;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setLineWidth(unsigned int width)
{
    /* 设置绘制线宽 */
    if (width == 0)
        width = 1;
    else if (width > MAX_PEN_WIDTH)
        width = MAX_PEN_WIDTH;
    m_pen = width;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setValue(double value, bool move)
{
    if (this->isChart())
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
    else
    if (this->isAngle())
    {
        /* 绘制角度图 */
        this->setAngle(value);
        m_crt = value;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::setAngle(double value)
{
    int     temp;
    TRect   rect;
    ansi_t  txt[64];

    rect.left = rect.top = 0;
    rect.right = ShowMain->Picture->Bitmap->Width;
    rect.bottom = rect.right;
    ShowMain->Canvas->Brush->Color = clWhite;
    ShowMain->Canvas->FillRect(rect);
    ShowMain->Canvas->Brush->Color = clBlack;
    ShowMain->Canvas->FrameRect(rect);
    ShowMain->Canvas->Pen->Width = m_pen;
    ShowMain->Canvas->Pen->Color = m_clr_bg;
    rect.left   += m_pen;
    rect.top    += m_pen;
    rect.right  -= m_pen;
    rect.bottom -= m_pen;
    temp = (rect.left + rect.right) / 2;
    ShowMain->Canvas->Arc(rect.left, rect.top, rect.right, rect.bottom,
                          temp, rect.top, temp, rect.top);
    sprintf(txt, "%.2f", value);
    ShowMain->Canvas->Brush->Color = clWhite;
    ShowMain->Canvas->TextOut(10, 5, AnsiString(txt));
    value *= CR_DTOR;
    temp = ShowMain->Picture->Bitmap->Width / 2;
    ShowMain->Canvas->Pen->Color = m_clr_fg;
    ShowMain->Canvas->MoveTo(temp, temp);
    ShowMain->Canvas->LineTo((int)((temp - m_pen) * cos(value)) + temp,
                              temp - (int)((temp - m_pen) * sin(value)));
}
//---------------------------------------------------------------------------
