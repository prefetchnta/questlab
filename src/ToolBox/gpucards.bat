@echo off
call "%VS143COMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
nmake -fgpucards.mak
pause
