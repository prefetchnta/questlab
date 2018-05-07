@echo off
set LUA_PATH=%~dp0lualib\?.lua
set LUA_CPATH=%~dp0lualib\?.dll
%~dp0wlua.exe %1 %2 %3 %4 %5 %6 %7
