@echo off
setlocal

REM Safe MiniGit Wrapper - Prevents file deletion and corruption
REM Usage: safe_minigit.bat <command> [options]

set PROJECT_DIR=%~dp0
set MINIGIT_EXE=%PROJECT_DIR%mini_git.exe
set BACKUP_SCRIPT=%PROJECT_DIR%backup_system.py
set SAFETY_LOG=%PROJECT_DIR%safety_log.json

echo ========================================
echo    Safe MiniGit Wrapper
echo ========================================

REM Check if MiniGit executable exists
if not exist "%MINIGIT_EXE%" (
    echo ❌ MiniGit executable not found: %MINIGIT_EXE%
    pause
    exit /b 1
)

REM Check if Python is available for backup system
python --version >nul 2>&1
if errorlevel 1 (
    echo ⚠️  Python not found - backup system disabled
    set BACKUP_ENABLED=false
) else (
    set BACKUP_ENABLED=true
)

REM Get the command
set COMMAND=%1
if "%COMMAND%"=="" (
    echo Usage: safe_minigit.bat ^<command^> [options]
    echo.
    echo Commands:
    echo   init     - Initialize repository (safe)
    echo   add      - Add files (safe)
    echo   commit   - Commit changes (safe)
    echo   log      - View history (safe)
    echo   status   - Check status (safe)
    echo   branch   - Branch operations (safe)
    echo   checkout - Switch branches/commits (RISKY - creates backup)
    echo   merge    - Merge branches (RISKY - creates backup)
    echo   help     - Show help
    echo.
    echo Safety features:
    echo   - Automatic backups before risky operations
    echo   - File integrity checks
    echo   - Recovery options
    pause
    exit /b 0
)

REM Determine if this is a risky operation
set RISKY_OPERATIONS=checkout merge reset
set IS_RISKY=false
for %%i in (%RISKY_OPERATIONS%) do (
    if "%%i"=="%COMMAND%" set IS_RISKY=true
)

REM Create backup before risky operations
if "%IS_RISKY%"=="true" (
    echo ⚠️  RISKY OPERATION DETECTED: %COMMAND%
    echo.
    if "%BACKUP_ENABLED%"=="true" (
        echo 🔄 Creating safety backup...
        python "%BACKUP_SCRIPT%" auto_backup_before_operation %COMMAND%
        if errorlevel 1 (
            echo ❌ Backup failed - operation cancelled for safety
            pause
            exit /b 1
        )
        echo ✅ Backup created successfully
    ) else (
        echo ⚠️  No backup system available - proceeding with caution
        echo Press any key to continue or Ctrl+C to cancel...
        pause >nul
    )
    echo.
)

REM Run the actual MiniGit command
echo 🚀 Executing: mini_git.exe %*
"%MINIGIT_EXE%" %*

REM Check result and create post-operation backup if successful
if errorlevel 0 (
    echo ✅ Operation completed successfully
    if "%IS_RISKY%"=="true" (
        if "%BACKUP_ENABLED%"=="true" (
            echo 🔄 Creating post-operation backup...
            python "%BACKUP_SCRIPT%" auto_backup_after_operation %COMMAND%
        )
    )
) else (
    echo ❌ Operation failed with error code %errorlevel%
    if "%IS_RISKY%"=="true" (
        echo 💡 You can restore from backup if needed
        if "%BACKUP_ENABLED%"=="true" (
            echo    Run: python "%BACKUP_SCRIPT%" to see available backups
        )
    )
)

echo.
echo ========================================
pause
