@echo off
call "%VS90COMNTOOLS%vsvars32.bat"
nmake -fTessOCR.mak
pause
