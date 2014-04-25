@echo off
del ..\..\tmp\*.obj /Q
make -fxSelectFile.mak
del ..\..\bin\*.tds /Q
pause
