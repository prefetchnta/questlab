@echo off
del ..\..\tmp\*.obj /Q
make -fQstInfoz.mak
del ..\..\bin\*.tds /Q
pause
