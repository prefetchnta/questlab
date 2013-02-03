@echo off
del ..\..\tmp\*.obj /Q
make -fxSelectDir.mak
del ..\..\bin\*.tds /Q
pause
