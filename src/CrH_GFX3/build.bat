@echo off
rem =====================
call "%VS100COMNTOOLS%vsvars32.bat"
rem =====================
nmake -fd3d8api.mak
rem =====================
nmake -fd3d9api.mak
rem =====================