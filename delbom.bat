@echo off
attrib .\tmp +h
cpage 0 0 . *.*
attrib .\tmp -h
pause
