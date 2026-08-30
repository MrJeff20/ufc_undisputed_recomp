@echo off
setlocal

set "ROOT=%~dp0.."
set "SRC=%ROOT%\tools\XenonRecomp-ufc"
set "BUILD=%SRC%\build-vs"
set "VSDEV=C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"

call "%VSDEV%" -arch=x64
if errorlevel 1 exit /b %errorlevel%

cmake -S "%SRC%" -B "%BUILD%" -G Ninja
if errorlevel 1 exit /b %errorlevel%

cmake --build "%BUILD%" --config Release
exit /b %errorlevel%
