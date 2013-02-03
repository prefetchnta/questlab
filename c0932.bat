@echo off
attrib .\tmp +h
cpage 932 65001 . *.*
attrib .\tmp -h
pause
