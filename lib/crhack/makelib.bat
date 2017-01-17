@echo
rem ========================================
:qstlibs
if not exist QstLibs.lib goto tessocr
del coff\QstLibs.lib
del omf\QstLibs.lib
coff2omf QstLibs.lib omf\QstLibs.lib
move QstLibs.lib coff\QstLibs.lib
rem ========================================
:tessocr
if not exist TessOCR.lib goto xopencv
del coff\TessOCR.lib
del omf\TessOCR.lib
coff2omf TessOCR.lib omf\TessOCR.lib
move TessOCR.lib coff\TessOCR.lib
rem ========================================
:xopencv
if not exist xOpenCV.lib goto resloader
del coff\xOpenCV.lib
del omf\xOpenCV.lib
coff2omf xOpenCV.lib omf\xOpenCV.lib
move xOpenCV.lib coff\xOpenCV.lib
rem ========================================
:resloader
if not exist ResLoader.lib goto crh_core
del coff\ResLoader.lib
del omf\ResLoader.lib
coff2omf ResLoader.lib omf\ResLoader.lib
move ResLoader.lib coff\ResLoader.lib
rem ========================================
:crh_core
if not exist CrH_CORE.lib goto crh_fmtz
del coff\CrH_CORE.lib
del omf\CrH_CORE.lib
coff2omf CrH_CORE.lib omf\CrH_CORE.lib
move CrH_CORE.lib coff\CrH_CORE.lib
rem ========================================
:crh_fmtz
if not exist CrH_FMTZ.lib goto crh_gfx2
del coff\CrH_FMTZ.lib
del omf\CrH_FMTZ.lib
coff2omf CrH_FMTZ.lib omf\CrH_FMTZ.lib
move CrH_FMTZ.lib coff\CrH_FMTZ.lib
rem ========================================
:crh_gfx2
if not exist CrH_GFX2.lib goto crh_gfx3
del coff\CrH_GFX2.lib
del omf\CrH_GFX2.lib
coff2omf CrH_GFX2.lib omf\CrH_GFX2.lib
move CrH_GFX2.lib coff\CrH_GFX2.lib
rem ========================================
:crh_gfx3
if not exist CrH_GFX3.lib goto crh_math
del coff\CrH_GFX3.lib
del omf\CrH_GFX3.lib
coff2omf CrH_GFX3.lib omf\CrH_GFX3.lib
move CrH_GFX3.lib coff\CrH_GFX3.lib
rem ========================================
:crh_math
if not exist CrH_MATH.lib goto gfx2_gdi
del coff\CrH_MATH.lib
del omf\CrH_MATH.lib
coff2omf CrH_MATH.lib omf\CrH_MATH.lib
move CrH_MATH.lib coff\CrH_MATH.lib
rem ========================================
:gfx2_gdi
if not exist GFX2_GDI.lib goto hacklib
del coff\GFX2_GDI.lib
del omf\GFX2_GDI.lib
coff2omf GFX2_GDI.lib omf\GFX2_GDI.lib
move GFX2_GDI.lib coff\GFX2_GDI.lib
rem ========================================
:hacklib
if not exist HackLib.lib goto bat_exit
del coff\HackLib.lib
del omf\HackLib.lib
coff2omf HackLib.lib omf\HackLib.lib
move HackLib.lib coff\HackLib.lib
rem ========================================
:bat_exit
