@echo off
call "%VS142COMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
nmake -fjump_cui.mak
pause
