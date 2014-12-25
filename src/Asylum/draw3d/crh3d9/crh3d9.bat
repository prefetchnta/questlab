@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fcrh3d9.mak
pause
