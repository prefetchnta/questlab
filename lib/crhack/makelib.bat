@echo
:qstlibs
if not exist QstLibs.lib goto crh_core
del QstLibs_bcc.lib
del QstLibs_msc.lib
coff2omf QstLibs.lib QstLibs_bcc.lib
ren QstLibs.lib QstLibs_msc.lib
:crh_core
if not exist CrH_CORE.lib goto crh_blit
del CrH_CORE_bcc.lib
del CrH_CORE_msc.lib
coff2omf CrH_CORE.lib CrH_CORE_bcc.lib
ren CrH_CORE.lib CrH_CORE_msc.lib
:crh_blit
if not exist CrH_BLIT.lib goto crh_extz
del CrH_BLIT_bcc.lib
del CrH_BLIT_msc.lib
coff2omf CrH_BLIT.lib CrH_BLIT_bcc.lib
ren CrH_BLIT.lib CrH_BLIT_msc.lib
:crh_extz
if not exist CrH_EXTZ.lib goto bat_exit
del CrH_EXTZ_bcc.lib
del CrH_EXTZ_msc.lib
coff2omf CrH_EXTZ.lib CrH_EXTZ_bcc.lib
ren CrH_EXTZ.lib CrH_EXTZ_msc.lib
:bat_exit
