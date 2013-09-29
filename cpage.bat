@echo off
attrib .\tmp +h
attrib .\wrk +h
attrib .\bin\ext3rd +h
attrib .\bin\output +h
attrib .\bin\source\tessdata +h
cpage 0 65001 . *.*
attrib .\tmp -h
attrib .\wrk -h
attrib .\bin\ext3rd -h
attrib .\bin\output -h
attrib .\bin\source\tessdata -h
pause
