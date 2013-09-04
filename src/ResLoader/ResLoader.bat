@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fResLoader.mak
pause
