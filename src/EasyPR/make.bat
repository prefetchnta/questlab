@echo off
call "%VS120COMNTOOLS%vsvars32.bat"
devenv /build Release EasyPR.sln
move *.exe ..\..\bin
move _build\*.lib ..\..\lib\easypr
rd /s /q _build
pause
