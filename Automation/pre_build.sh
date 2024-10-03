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

if ! command -v python3 &> /dev/null; then
    echo "Error: Python 3 is not installed."
    exit 2
else
    echo "Python 3 is available at: $(command -v python3)"
fi

echo "Printing files from: $exec_path"
python3 "$script_dir"/walk_over_folders.py "$exec_path"
echo "//// //// //// //// //// //// //// //// //// //// //// ////"
