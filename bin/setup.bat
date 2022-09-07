@echo off
cd /D %~dp0
set "CD=%CD:\=\\%"
echo Windows Registry Editor Version 5.00> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - DOS]>> QuestLAB.reg
echo @="HaSHer - DOS(&1)">> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - DOS\command]>> QuestLAB.reg
echo @="cmd.exe /k cd \"%%1\"">> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - HSH]>> QuestLAB.reg
echo @="HaSHer - HSH(&2)">> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - HSH\command]>> QuestLAB.reg
echo @="%CD%\\HSH1.bat \"%%1\"">> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - MD5]>> QuestLAB.reg
echo @="HaSHer - MD5(&3)">> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - MD5\command]>> QuestLAB.reg
echo @="%CD%\\HaSHer.exe \"%%1\" -0">> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - SHA]>> QuestLAB.reg
echo @="HaSHer - SHA(&4)">> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - SHA\command]>> QuestLAB.reg
echo @="%CD%\\HaSHer.exe \"%%1\" -1">> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - SIG]>> QuestLAB.reg
echo @="HaSHer - SIG(&5)">> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - SIG\command]>> QuestLAB.reg
echo @="%CD%\\SIG1.bat \"%%1\"">> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - XOR]>> QuestLAB.reg
echo @="HaSHer - XOR(&6)">> QuestLAB.reg
echo.>> QuestLAB.reg
echo [HKEY_CLASSES_ROOT\Directory\shell\HaSHer - XOR\command]>> QuestLAB.reg
echo @="%CD%\\HaSHer.exe \"%%1\" -2">> QuestLAB.reg
echo.>> QuestLAB.reg
QuestLAB.reg
del /Q QuestLAB.reg
if exist "%SystemRoot%\Fonts\FSEX300.ttf" goto exit
copy /Y source\fonts\FSEX300.ttf "%SystemRoot%\Fonts"
FontReg.exe
:exit
