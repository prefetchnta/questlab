@echo off
rem =====================
call "%VS100COMNTOOLS%vsvars32.bat"
rem =====================
nmake -fblit.mak
rem =====================