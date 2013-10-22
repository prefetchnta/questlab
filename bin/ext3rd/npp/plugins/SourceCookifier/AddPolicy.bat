@echo off

set polname=SourceCookifier
set target=%~dp0..\*

for /F "tokens=2* delims=	 " %%A in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\.NETFramework" /v InstallRoot') do set runtimes=%%B
set caspol="%runtimes%v2.0.50727\caspol.exe"
if not exist %caspol% (
  for /F "tokens=2* delims=	 " %%A in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\.NETFramework" /v InstallRoot') do set runtimes=%%B
  set caspol="%runtimes%v2.0.50727\caspol.exe"
)
if not exist %caspol% (
	echo *** CasPol for .NET Framework 2.0 NOT found !
	goto finit
) else (
	echo ~~} CasPol for .NET Framework 2.0 found
	echo.
	%caspol% -pp off -machine -addgroup All_Code -url file:%target% FullTrust -name %polname%
)
:finit
echo.
pause