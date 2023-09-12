@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fCompLib1.mak
nmake -fCompLib2.mak
pause
