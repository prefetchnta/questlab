@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fQstCmdz.mak
pause
