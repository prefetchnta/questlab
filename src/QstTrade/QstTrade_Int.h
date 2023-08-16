
#ifndef __QL_QSTTRADE_INT_H__
#define __QL_QSTTRADE_INT_H__

#include "QstTrade.h"
#include "gfx2/gdiwin.h"

extern iFONT*   g_sml_font;
extern iFONT*   g_mdl_font;
extern iFONT*   g_lrg_font;
extern iGFX2*   g_bmp_temp;

extern sIMAGE*  g_win_candle;
extern sIMAGE*  g_win_voturn;
extern sIMAGE*  g_win_vaturn;
extern sIMAGE*  g_win_indctr[];

extern sQTRD_conf   g_sys_cfg;

extern sQTRD_candle g_can_chart;
extern sQTRD_amount g_mnt_chart;
extern sQTRD_indctr g_ind_chart[];

#endif  /* !__QL_QSTTRADE_INT_H__ */
