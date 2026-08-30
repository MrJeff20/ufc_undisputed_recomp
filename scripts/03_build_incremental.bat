@echo off
setlocal

set "ROOT=%~dp0.."
set "BUILD=%ROOT%\build\ufc-native"
set "VSDEVCMD=C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"
set "CLANGXX=C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang++.exe"
set "CMAKE=C:\Program Files\CMake\bin\cmake.exe"

if not exist "%VSDEVCMD%" (
    echo Visual Studio developer environment was not found:
    echo   %VSDEVCMD%
    exit /b 1
)

if not exist "%CLANGXX%" (
    echo Visual Studio LLVM compiler was not found:
    echo   %CLANGXX%
    exit /b 1
)

if not exist "%CMAKE%" (
    echo CMake was not found:
    echo   %CMAKE%
    exit /b 1
)

if not exist "%ROOT%\ppc_output\ppc_recomp_shared.h" (
    echo Generated PPC output was not found. Run XenonRecomp first.
    exit /b 1
)

call "%VSDEVCMD%" -arch=x64 -host_arch=x64 >nul
if errorlevel 1 exit /b %errorlevel%

"%CMAKE%" -S "%ROOT%" -B "%BUILD%" -G Ninja ^
    -DCMAKE_CXX_COMPILER="%CLANGXX%" ^
    -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit /b %errorlevel%

"%CMAKE%" --build "%BUILD%" --target ufc_native_incremental --config Release
exit /b %errorlevel%