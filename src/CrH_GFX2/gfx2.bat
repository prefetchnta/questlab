@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fgfx2.mak
pause
