@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
nmake -fcsv2days.mak
pause
