@echo off
setlocal

set "ROOT=%~dp0.."
set "VSDEVCMD=C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"
set "PWSH=C:\Users\elmet\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell\pwsh.exe"

if not exist "%VSDEVCMD%" (
    echo Visual Studio developer environment was not found:
    echo   %VSDEVCMD%
    exit /b 1
)

if not exist "%PWSH%" (
    set "PWSH=powershell.exe"
)

call "%VSDEVCMD%" -arch=x64 -host_arch=x64 >nul
if errorlevel 1 exit /b %errorlevel%

"%PWSH%" -NoProfile -ExecutionPolicy Bypass -File "%ROOT%\scripts\04_compile_sweep.ps1" %*
exit /b %errorlevel%