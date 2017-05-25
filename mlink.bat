@echo off
cd inc
if exist crhack goto bat_exit
mklink /D /J crhack D:\GIT\crhack\inc
:bat_exit
