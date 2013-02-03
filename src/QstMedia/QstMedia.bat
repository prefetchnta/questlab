@echo off
del ..\..\tmp\*.obj /Q
make -fQstMedia.mak
del ..\..\bin\*.tds /Q
pause
