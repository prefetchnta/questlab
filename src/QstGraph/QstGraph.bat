@echo off
del ..\..\tmp\*.obj /Q
make -fQstGraph.mak
del ..\..\bin\*.tds /Q
pause
