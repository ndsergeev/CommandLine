#!/bin/bash

script_path=$(realpath "$0")
script_dir=$(dirname "$script_path")

echo "//// //// //// //// //// //// //// //// //// //// //// ////"
echo "//// //// //// //// Before Unreal Build //// //// //// ////"
echo "//// //// //// //// //// //// //// //// //// //// //// ////"
exec_path=$1
if [ ! -e "$exec_path" ]; then
    echo "Error: The path '$exec_path' does not exist."
    exit 1
fi

if command -v python3 &> /dev/null; then
    python_cmd=$(command -v python3)
elif command -v python &> /dev/null; then
    python_cmd=$(command -v python)
else
    echo "Error: Python is not installed."
    exit 1
fi

echo "Python is available at: $python_cmd"
echo "Printing files from: $exec_path"
"$python_cmd" "$script_dir"/walk_over_folders.py "$exec_path"
echo "//// //// //// //// //// //// //// //// //// //// //// ////"
exit 0
