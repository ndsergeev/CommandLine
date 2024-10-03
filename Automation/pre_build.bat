@echo off

set "script_path=%~f0"
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
if errorlevel 1 (
    echo Error: Python 3 is not installed.
    exit /b 2
) else (
    for /f "delims=" %%i in ('where python3') do set "python_path=%%i"
    echo Python 3 is available at: %python_path%
)

echo Printing files from: %exec_path%
python3 "%script_dir%walk_over_folders.py" "%exec_path%"
echo //// //// //// //// //// //// //// //// //// //// //// ////
