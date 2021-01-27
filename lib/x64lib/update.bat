@echo off
copy /Y ..\..\..\FinalBIN\CRHACK\vc2019_win32_x64\CrH_CORE.lib
copy /Y ..\..\..\FinalBIN\CRHACK\vc2019_win32_x64\CrH_FMTZ.lib
copy /Y ..\..\..\FinalBIN\CRHACK\vc2019_win32_x64\CrH_GFX2.lib
copy /Y ..\..\..\FinalBIN\CRHACK\vc2019_win32_x64\CrH_GFX3.lib
copy /Y ..\..\..\FinalBIN\CRHACK\vc2019_win32_x64\CrH_MATH.lib
cd ..\..\bin\x64bin
copy /Y ..\..\..\FinalBIN\CRHACK\vc2019_win32_x64\CrH_*.dll
copy /Y ..\..\..\FinalBIN\CRHACK\vc2019_win32_x64\GFX?_*.dll
pause
