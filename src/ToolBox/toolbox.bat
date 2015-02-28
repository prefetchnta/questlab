@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
rc.exe /nologo /d "UNICODE" /d "_UNICODE" /fo.\toolbox.res .\toolbox.rc
pause
