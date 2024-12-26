@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fsfrw_srv.mak
pause
