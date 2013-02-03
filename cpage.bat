@echo off
attrib .\tmp +h
cpage 0 65001 . *.*
attrib .\tmp -h
pause
