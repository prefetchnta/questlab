@echo off
attrib .\tmp +h
attrib .\bin\ext3rd +h
attrib .\bin\output +h
attrib .\bin\source\tessdata +h
cpage 1252 65001 . *.*
attrib .\tmp -h
attrib .\bin\ext3rd -h
attrib .\bin\output -h
attrib .\bin\source\tessdata -h
pause
