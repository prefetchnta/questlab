@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fQstView2D.mak
pause
