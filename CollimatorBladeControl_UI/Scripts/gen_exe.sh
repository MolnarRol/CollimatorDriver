#!/bin/bash
pyinstaller ../main.py --onefile --clean --distpath ../Release --name CollimatorGui  --noconsole --distpath ../Release
clear
echo "Build successful"


