@echo
:qstlibs
if not exist QstLibs.lib goto crh_blit
del QstLibs_bcc.lib
del QstLibs_msc.lib
coff2omf QstLibs.lib QstLibs_bcc.lib
ren QstLibs.lib QstLibs_msc.lib
:crh_blit
if not exist CrH_BLIT.lib goto crh_comm
del CrH_BLIT_bcc.lib
del CrH_BLIT_msc.lib
coff2omf CrH_BLIT.lib CrH_BLIT_bcc.lib
ren CrH_BLIT.lib CrH_BLIT_msc.lib
:crh_comm
if not exist CrH_COMM.lib goto crh_core
del CrH_COMM_bcc.lib
del CrH_COMM_msc.lib
coff2omf CrH_COMM.lib CrH_COMM_bcc.lib
ren CrH_COMM.lib CrH_COMM_msc.lib
:crh_core
if not exist CrH_CORE.lib goto crh_extz
del CrH_CORE_bcc.lib
del CrH_CORE_msc.lib
coff2omf CrH_CORE.lib CrH_CORE_bcc.lib
ren CrH_CORE.lib CrH_CORE_msc.lib
:crh_extz
if not exist CrH_EXTZ.lib goto crh_fmtz
del CrH_EXTZ_bcc.lib
del CrH_EXTZ_msc.lib
coff2omf CrH_EXTZ.lib CrH_EXTZ_bcc.lib
ren CrH_EXTZ.lib CrH_EXTZ_msc.lib
:crh_fmtz
if not exist CrH_FMTZ.lib goto crh_gfx3
del CrH_FMTZ_bcc.lib
del CrH_FMTZ_msc.lib
coff2omf CrH_FMTZ.lib CrH_FMTZ_bcc.lib
ren CrH_FMTZ.lib CrH_FMTZ_msc.lib
:crh_gfx3
if not exist CrH_GFX3.lib goto crh_math
del CrH_GFX3_bcc.lib
del CrH_GFX3_msc.lib
coff2omf CrH_GFX3.lib CrH_GFX3_bcc.lib
ren CrH_GFX3.lib CrH_GFX3_msc.lib
:crh_math
if not exist CrH_MATH.lib goto crh_vsys
del CrH_MATH_bcc.lib
del CrH_MATH_msc.lib
coff2omf CrH_MATH.lib CrH_MATH_bcc.lib
ren CrH_MATH.lib CrH_MATH_msc.lib
:crh_vsys
if not exist CrH_VSYS.lib goto bat_exit
del CrH_VSYS_bcc.lib
del CrH_VSYS_msc.lib
coff2omf CrH_VSYS.lib CrH_VSYS_bcc.lib
ren CrH_VSYS.lib CrH_VSYS_msc.lib
:bat_exit
