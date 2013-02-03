@echo off
del ..\..\tmp\*.obj /Q
make -fQstMenu.mak
del ..\..\bin\*.tds /Q
pause
