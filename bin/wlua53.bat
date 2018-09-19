@echo off
set LUA_PATH=%~dp0lualib\?.lua
set LUA_CPATH=%~dp0lualib\?.dll
%~dp0wlua.exe %*
