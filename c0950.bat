@echo off
attrib .\tmp +h
cpage 950 65001 . *.*
attrib .\tmp -h
pause
