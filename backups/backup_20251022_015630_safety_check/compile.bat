@echo off
setlocal

set MSYS2_PATH=C:\msys64
set PROJECT_PATH=%CD%

if not exist "%MSYS2_PATH%\usr\bin\bash.exe" (
    echo ❌ MSYS2 bash not found at "%MSYS2_PATH%\usr\bin\bash.exe"
    pause
    exit /b 1
)

echo ✅ Using MSYS2 environment at "%MSYS2_PATH%"

rem Run build.sh in the correct directory inside MSYS2
"%MSYS2_PATH%\usr\bin\bash.exe" --login -i -c "cd \"$(cygpath '%PROJECT_PATH%')\" && ./build.sh"

pause
endlocal
