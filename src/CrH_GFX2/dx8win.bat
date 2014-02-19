@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fdx8win.mak
pause
