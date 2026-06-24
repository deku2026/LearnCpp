@echo off
rem One-shot helper: vcvars64 -> cmake configure -> cmake build -> run learn_cpp.
rem Useful when you don't want an interactive shell.

setlocal
if "%VSINSTALL_DIR%"=="" set "VSINSTALL_DIR=C:\Program Files\Microsoft Visual Studio\18\Community"
set "VCVARS=%VSINSTALL_DIR%\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VCVARS%" (
    echo [configure-and-build] vcvars64.bat not found at %VCVARS%
    exit /b 1
)

set "PRESET=%1"
if "%PRESET%"=="" set "PRESET=windows-debug"

call "%VCVARS%" >nul || exit /b 1
pushd "%~dp0.."
cmake --preset %PRESET% || (popd & exit /b 1)
cmake --build --preset %PRESET% || (popd & exit /b 1)
rem Smoke-run: list registered topics. Pass a topic id as the second arg to run that topic instead.
if "%2"=="" (
    "build\%PRESET%\bin\learn_cpp.exe"
) else (
    "build\%PRESET%\bin\learn_cpp.exe" %2 %3 %4 %5 %6 %7 %8 %9
)
set "EC=%ERRORLEVEL%"
popd
endlocal & exit /b %EC%
