@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fkernel33.mak
pause
