@echo
rem ========================================
:qstlibs
if not exist QstLibs.lib goto ocr3rd
del QstLibs_bcc.lib
del QstLibs_msc.lib
coff2omf QstLibs.lib QstLibs_bcc.lib
ren QstLibs.lib QstLibs_msc.lib
rem ========================================
:ocr3rd
if not exist ocr3rd.lib goto resloader
del ocr3rd_bcc.lib
del ocr3rd_msc.lib
coff2omf ocr3rd.lib ocr3rd_bcc.lib
ren ocr3rd.lib ocr3rd_msc.lib
rem ========================================
:resloader
if not exist ResLoader.lib goto crh_blit
del ResLoader_bcc.lib
del ResLoader_msc.lib
coff2omf ResLoader.lib ResLoader_bcc.lib
ren ResLoader.lib ResLoader_msc.lib
rem ========================================
:crh_blit
if not exist CrH_BLIT.lib goto crh_comm
del CrH_BLIT_bcc.lib
del CrH_BLIT_msc.lib
coff2omf CrH_BLIT.lib CrH_BLIT_bcc.lib
ren CrH_BLIT.lib CrH_BLIT_msc.lib
rem ========================================
:crh_comm
if not exist CrH_COMM.lib goto crh_core
del CrH_COMM_bcc.lib
del CrH_COMM_msc.lib
coff2omf CrH_COMM.lib CrH_COMM_bcc.lib
ren CrH_COMM.lib CrH_COMM_msc.lib
rem ========================================
:crh_core
if not exist CrH_CORE.lib goto crh_extz
del CrH_CORE_bcc.lib
del CrH_CORE_msc.lib
coff2omf CrH_CORE.lib CrH_CORE_bcc.lib
ren CrH_CORE.lib CrH_CORE_msc.lib
rem ========================================
:crh_extz
if not exist CrH_EXTZ.lib goto crh_fmtz
del CrH_EXTZ_bcc.lib
del CrH_EXTZ_msc.lib
coff2omf CrH_EXTZ.lib CrH_EXTZ_bcc.lib
ren CrH_EXTZ.lib CrH_EXTZ_msc.lib
rem ========================================
:crh_fmtz
if not exist CrH_FMTZ.lib goto crh_gfx3
del CrH_FMTZ_bcc.lib
del CrH_FMTZ_msc.lib
coff2omf CrH_FMTZ.lib CrH_FMTZ_bcc.lib
ren CrH_FMTZ.lib CrH_FMTZ_msc.lib
rem ========================================
:crh_gfx3
if not exist CrH_GFX3.lib goto crh_math
del CrH_GFX3_bcc.lib
del CrH_GFX3_msc.lib
coff2omf CrH_GFX3.lib CrH_GFX3_bcc.lib
ren CrH_GFX3.lib CrH_GFX3_msc.lib
rem ========================================
:crh_math
if not exist CrH_MATH.lib goto bat_exit
del CrH_MATH_bcc.lib
del CrH_MATH_msc.lib
coff2omf CrH_MATH.lib CrH_MATH_bcc.lib
ren CrH_MATH.lib CrH_MATH_msc.lib
rem ========================================
:bat_exit
