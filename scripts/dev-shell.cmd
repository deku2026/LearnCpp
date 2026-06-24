@echo off
rem -------------------------------------------------------------------------
rem  LearnCpp Windows dev shell
rem  Opens a persistent prompt with the VS 2026 x64 build tools (clang-cl,
rem  clang-tidy, ninja, rc.exe, MSVC headers) on PATH and INCLUDE/LIB set.
rem
rem  Usage: double-click, or run from any shell:
rem      scripts\dev-shell.cmd
rem
rem  Override the VS install directory by setting VSINSTALL_DIR before running.
rem -------------------------------------------------------------------------

if "%VSINSTALL_DIR%"=="" set "VSINSTALL_DIR=C:\Program Files\Microsoft Visual Studio\18\Community"

set "VCVARS=%VSINSTALL_DIR%\VC\Auxiliary\Build\vcvars64.bat"

if not exist "%VCVARS%" (
    echo [dev-shell] vcvars64.bat not found at:
    echo               %VCVARS%
    echo            Set VSINSTALL_DIR to point at your VS install, e.g.:
    echo               set "VSINSTALL_DIR=C:\Program Files\Microsoft Visual Studio\2022\Community"
    exit /b 1
)

echo [dev-shell] Launching x64 dev prompt rooted at %~dp0..
%comspec% /k "%VCVARS%" ^&^& cd /d "%~dp0.."
