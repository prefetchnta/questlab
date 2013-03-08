@echo off
del ..\..\tmp\*.obj /Q
make -fQstText.mak
del ..\..\bin\*.tds /Q
pause
