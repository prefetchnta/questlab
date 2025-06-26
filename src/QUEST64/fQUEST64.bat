@echo off
call "%VS143COMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
nmake -ffQUEST64.mak
pause
