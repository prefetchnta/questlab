@echo off
attrib .\tmp +h
cpage 1252 65001 . *.*
attrib .\tmp -h
pause
