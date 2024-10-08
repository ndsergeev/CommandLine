@echo off

set "script_dir=%~dp0"

echo //// //// //// //// //// //// //// //// //// //// //// ////
echo //// //// //// //// Before Unreal Build //// //// //// ////
echo //// //// //// //// //// //// //// //// //// //// //// ////

if "%~1"=="" (
    echo "Error: No path provided."
    exit /b 1
)
set "exec_path=%~1"

if not exist "%exec_path%" (
    echo "Error: The path '%exec_path%' does not exist."
    exit /b 1
)

for %%I in (python python3) do (
    where %%I >nul 2>nul
    if not errorlevel 1 (
        set "python_cmd=%%I"
        goto :PythonFound
    )
)

:PythonFound
echo Python is available at: %python_cmd%
echo Printing files from: %exec_path%
%python_cmd% %script_dir%walk_over_folders.py %exec_path%
echo //// //// //// //// //// //// //// //// //// //// //// ////