@echo off
rem One-shot helper: vcvars64 -> cmake configure -> cmake build -> ctest.
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
ctest --preset %PRESET% --output-on-failure
set "EC=%ERRORLEVEL%"
popd
endlocal & exit /b %EC%
