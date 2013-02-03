@echo off
call "%VS71COMNTOOLS%vsvars32.bat"
nmake -fe_syn.mak
pause
