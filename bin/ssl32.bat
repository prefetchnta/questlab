@echo off
set HOME=%~dp0crypto
set OPENSSL_ENGINES=%~dp0crypto
set OPENSSL_CONF=%~dp0crypto\openssl.cnf
%~dp0openssl.exe
