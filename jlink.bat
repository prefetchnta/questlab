@echo off
if exist inc goto crh_fmtz
junction inc D:\PRJ\CrHack\inc
:crh_fmtz
cd src
if exist CrH_FMTZ goto crh_gfx2
junction CrH_FMTZ D:\PRJ\CrHack\src\_fmtz_
:crh_gfx2
if exist CrH_GFX2 goto crh_gfx3
junction CrH_GFX2 D:\PRJ\CrHack\src\_gfx2_
:crh_gfx3
if exist CrH_GFX3 goto bat_exit
junction CrH_GFX3 D:\PRJ\CrHack\src\_gfx3_
:bat_exit