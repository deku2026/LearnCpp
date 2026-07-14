@echo off
setlocal EnableExtensions

rem Build CMake target learn_ffm (shared lib for Java FFM) using VS2026 vcvars64.
rem Usage (from repo root):
rem   scripts\build-ffm-native.cmd
rem   scripts\build-ffm-native.cmd windows-debug
rem   scripts\build-ffm-native.cmd windows-release

set "PRESET=%~1"
if "%PRESET%"=="" set "PRESET=windows-debug"

set "ROOT=%~dp0.."
pushd "%ROOT%" || exit /b 1

set "VCVARS=C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VCVARS%" (
  echo ERROR: vcvars64 not found: %VCVARS%
  echo Adjust path or run from a VS2026 x64 Developer Command Prompt and:
  echo   cmake --preset %PRESET%
  echo   cmake --build --preset %PRESET% --target learn_ffm
  popd
  exit /b 1
)

call "%VCVARS%" || (
  echo ERROR: vcvars64 failed
  popd
  exit /b 1
)

if not exist "build\%PRESET%\CMakeCache.txt" (
  echo Configuring preset %PRESET% ...
  cmake --preset "%PRESET%" || (
    popd
    exit /b 1
  )
)

echo Building target learn_ffm ^(preset %PRESET%^) ...
cmake --build --preset "%PRESET%" --target learn_ffm || (
  popd
  exit /b 1
)

set "OUT=build\%PRESET%\bin\learn_ffm.dll"
if exist "%OUT%" (
  echo OK: %CD%\%OUT%
) else (
  echo Build finished; check build\%PRESET%\bin for learn_ffm.*
)

popd
endlocal
