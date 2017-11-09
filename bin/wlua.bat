@echo off
set LUA_PATH=%cd%\lualib\?.lua
set LUA_CPATH=%cd%\lualib\?.dll
wlua %1 %2 %3 %4 %5 %6 %7
