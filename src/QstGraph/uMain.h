//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Chart.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ArrowCha.hpp>
#include <BubbleCh.hpp>
#include <GanttCh.hpp>
#include <Series.hpp>
#include <TeeShape.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:    // IDE-managed Components
        TChart *ChartMain;
        TLineSeries *Series1;
        TBarSeries *Series2;
        THorizBarSeries *Series3;
        TAreaSeries *Series4;
        TPointSeries *Series5;
        TPieSeries *Series6;
        TFastLineSeries *Series7;
        TChartShape *Series8;
        TGanttSeries *Series9;
        TArrowSeries *Series10;
        TBubbleSeries *Series11;
private:        // User declarations
        bool m_auto_v;
        int m_xx, m_ww;
        double m_min, m_max;
        unsigned int m_type;
        TChartSeries *m_line;
public:         // User declarations
        __fastcall TfrmMain(TComponent* Owner);
        void __fastcall setup(unsigned int type, const AnsiString& title);
        void __fastcall clear();
        void __fastcall setBottom(unsigned int max);
        void __fastcall setBottom(double min, double max);
        void __fastcall setLeft(double min, double max);
        void __fastcall setLineColor(unsigned int color);
        void __fastcall setValue(double value, bool move);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
