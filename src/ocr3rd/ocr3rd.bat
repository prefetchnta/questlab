@echo off
call "%VS90COMNTOOLS%vsvars32.bat"
nmake -focr3rd.mak
pause
