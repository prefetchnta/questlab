@echo
rem ========================================
:qstlibs
if not exist QstLibs.lib goto ocr3rd
del coff\QstLibs.lib
del omf\QstLibs.lib
coff2omf QstLibs.lib omf\QstLibs.lib
move QstLibs.lib coff\QstLibs.lib
rem ========================================
:ocr3rd
if not exist ocr3rd.lib goto resloader
del coff\ocr3rd.lib
del omf\ocr3rd.lib
coff2omf ocr3rd.lib omf\ocr3rd.lib
move ocr3rd.lib coff\ocr3rd.lib
rem ========================================
:resloader
if not exist ResLoader.lib goto crh_blit
del coff\ResLoader.lib
del omf\ResLoader.lib
coff2omf ResLoader.lib omf\ResLoader.lib
move ResLoader.lib coff\ResLoader.lib
rem ========================================
:crh_blit
if not exist CrH_BLIT.lib goto crh_core
del coff\CrH_BLIT.lib
del omf\CrH_BLIT.lib
coff2omf CrH_BLIT.lib omf\CrH_BLIT.lib
move CrH_BLIT.lib coff\CrH_BLIT.lib
rem ========================================
:crh_core
if not exist CrH_CORE.lib goto crh_fmtz
del coff\CrH_CORE.lib
del omf\CrH_CORE.lib
coff2omf CrH_CORE.lib omf\CrH_CORE.lib
move CrH_CORE.lib coff\CrH_CORE.lib
rem ========================================
:crh_fmtz
if not exist CrH_FMTZ.lib goto crh_math
del coff\CrH_FMTZ.lib
del omf\CrH_FMTZ.lib
coff2omf CrH_FMTZ.lib omf\CrH_FMTZ.lib
move CrH_FMTZ.lib coff\CrH_FMTZ.lib
rem ========================================
:crh_math
if not exist CrH_MATH.lib goto bat_exit
del coff\CrH_MATH.lib
del omf\CrH_MATH.lib
coff2omf CrH_MATH.lib omf\CrH_MATH.lib
move CrH_MATH.lib coff\CrH_MATH.lib
rem ========================================
:bat_exit
