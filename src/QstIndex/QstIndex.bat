@echo off
del ..\..\tmp\*.obj /Q
make -fQstIndex.mak
del ..\..\bin\*.tds /Q
pause
