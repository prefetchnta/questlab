@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fe_falcom.mak
pause
