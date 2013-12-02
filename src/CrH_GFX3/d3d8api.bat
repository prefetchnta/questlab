@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fd3d8api.mak
pause
