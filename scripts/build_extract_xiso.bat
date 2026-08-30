@echo off
setlocal

set "VSDEVCMD=C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"

if not exist "%VSDEVCMD%" (
  echo VsDevCmd not found: %VSDEVCMD%
  exit /b 1
)

call "%VSDEVCMD%" -arch=x64
if errorlevel 1 exit /b %errorlevel%

cmake -S tools\extract-xiso -B tools\extract-xiso\build-nmake -G "NMake Makefiles"
if errorlevel 1 exit /b %errorlevel%

cmake --build tools\extract-xiso\build-nmake --config Release
if errorlevel 1 exit /b %errorlevel%

echo extract-xiso build complete.
