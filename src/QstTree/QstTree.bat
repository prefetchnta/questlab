@echo off
del ..\..\tmp\*.obj /Q
make -fQstTree.mak
del ..\..\bin\*.tds /Q
pause
