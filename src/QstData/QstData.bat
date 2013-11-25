@echo off
del ..\..\tmp\*.obj /Q
make -fQstData.mak
del ..\..\bin\*.tds /Q
pause
