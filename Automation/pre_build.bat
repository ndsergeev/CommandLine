@echo off

:: set "script_path=%~f0"
set "script_dir=%~dp0"

echo //// //// //// //// //// //// //// //// //// //// //// ////
echo //// //// //// //// Before Unreal Build //// //// //// ////
echo //// //// //// //// //// //// //// //// //// //// //// ////

set "exec_path=%~1"
if not exist "%exec_path%" (
    echo Error: The path '%exec_path%' does not exist.
    exit /b 1
)

where python3 >nul 2>&1
if not errorlevel 1 (
    for /f "delims=" %%i in ('where python3') do set "python_cmd=%%i"
) else (
    where python >nul 2>&1
    if not errorlevel 1 (
        for /f "delims=" %%i in ('where python') do set "python_cmd=%%i"
    ) else (
        echo Error: Python is not installed.
        exit /b 2
    )
)

echo Python is available at: %python_cmd%
echo Printing files from: %exec_path%
"%python_cmd%" "%script_dir%walk_over_folders.py" "%exec_path%"
echo //// //// //// //// //// //// //// //// //// //// //// ////