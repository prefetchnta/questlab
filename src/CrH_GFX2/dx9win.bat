@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fdx9win.mak
pause
