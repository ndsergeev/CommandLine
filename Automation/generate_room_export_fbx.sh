#!/bin/bash

# UE Editor executable
UE_EDITOR="/Applications/UE_5.4/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor"

# Unreal Engine project path
PROJECT_PATH="/Users/nsergeev/Developer/HART/Test/CommandLine/CommandLine.uproject"

# Path to export FBX
EXPORT_FILEPATH="/Users/nsergeev/Developer/HART/Test/CommandLine/Exports"

# Python script to run
PYTHON_SCRIPT="/Users/nsergeev/Developer/HART/Test/CommandLine/Automation/generate_room_export_fbx.py"

# Level to Open by Py Script
LEVEL_NAME="MyLevel"

echo "UEEditor: $UE_EDITOR"
echo "Project Path: $PROJECT_PATH"
echo "Python Script: $PYTHON_SCRIPT"
echo "Level: $LEVEL_NAME"
echo "Export Path: $EXPORT_FILEPATH"

chmod 755 $PYTHON_SCRIPT

# "$UE_EDITOR" "$PROJECT_PATH" -ExecutePythonScript="$PYTHON_SCRIPT"
# "$UE_EDITOR" "$PROJECT_PATH" -run=pythonscript -script="$PYTHON_SCRIPT"
"$UE_EDITOR" "$PROJECT_PATH" -ExecutePythonScript="$PYTHON_SCRIPT --level_name $LEVEL_NAME --export_filepath $EXPORT_FILEPATH"
