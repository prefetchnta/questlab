@echo off
del ..\..\tmp\*.obj /Q
make -fQstHash.mak
del ..\..\bin\*.tds /Q
pause
