@echo off
setlocal

set "ROOT=%~dp0.."
set "VSDEVCMD=C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"
set "CLANGXX=C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang++.exe"

if not exist "%VSDEVCMD%" (
    echo Visual Studio developer environment was not found.
    exit /b 1
)

if not exist "%CLANGXX%" (
    echo Visual Studio LLVM compiler was not found.
    exit /b 1
)

call "%VSDEVCMD%" -arch=x64 -host_arch=x64 >nul
"%CLANGXX%" -std=c++20 -fsyntax-only ^
    -I "%ROOT%\ppc_output" ^
    -I "%ROOT%\tools\XenonRecomp-ufc\thirdparty\simde" ^
    "%ROOT%\ppc_output\ppc_recomp.0.cpp"
