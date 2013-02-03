@echo off
del ..\..\tmp\*.obj /Q
make -fRunWebIE.mak
del ..\..\bin\*.tds /Q
pause
