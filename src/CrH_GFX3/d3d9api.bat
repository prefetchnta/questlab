@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fd3d9api.mak
pause
