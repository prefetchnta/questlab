@echo off
call "%VS120COMNTOOLS%vsvars32.bat"
nmake -fxOpenCV.mak
pause
