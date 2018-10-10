@echo off
set TCL_LIBRARY=%~dp0python\tcl\tcl8.6
set TK_LIBRARY=%~dp0python\tcl\tk8.6
set TIX_LIBRARY=%~dp0python\tcl\tix8.4.3
%~dp0tclsh86t.exe %*
