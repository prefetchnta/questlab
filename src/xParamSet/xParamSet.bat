@echo off
del ..\..\tmp\*.obj /Q
make -fxParamSet.mak
del ..\..\bin\*.tds /Q
pause
