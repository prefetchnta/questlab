@echo off
copy /Y ..\..\..\FinalBIN\CRHACK\vc2010_win32_x86\CrH_CORE.lib
copy /Y ..\..\..\FinalBIN\CRHACK\vc2010_win32_x86\CrH_FMTZ.lib
copy /Y ..\..\..\FinalBIN\CRHACK\vc2010_win32_x86\CrH_GFX2.lib
copy /Y ..\..\..\FinalBIN\CRHACK\vc2010_win32_x86\CrH_GFX3.lib
copy /Y ..\..\..\FinalBIN\CRHACK\vc2010_win32_x86\CrH_MATH.lib
call makelib.bat
cd ..\..\bin
copy /Y ..\..\FinalBIN\CRHACK\vc2010_win32_x86\CrH_*.dll
copy /Y ..\..\FinalBIN\CRHACK\vc2010_win32_x86\GFX?_*.dll
copy /Y ..\..\FinalBIN\CRHACK\vc2010_win32_x86\z*.dll plugin
copy /Y ..\..\FinalBIN\CRHACK\vc2003_win32_x86\zSYN.dll plugin
pause
