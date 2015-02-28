@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fobj2bobj.mak
pause
