@echo off
cd dev
if exist crhack goto crh_fmtz
mklink /D /J crhack D:\PRJ\CrHack\inc
:crh_fmtz
cd ..\src
if exist CrH_FMTZ goto crh_gfx2
mklink /D /J CrH_FMTZ D:\PRJ\CrHack\src\_fmtz_
:crh_gfx2
if exist CrH_GFX2 goto crh_gfx3
mklink /D /J CrH_GFX2 D:\PRJ\CrHack\src\_gfx2_
:crh_gfx3
if exist CrH_GFX3 goto bat_exit
mklink /D /J CrH_GFX3 D:\PRJ\CrHack\src\_gfx3_
:bat_exit
