#!/usr/bin/sh

dmbCorePath="../Core/"

curDir=$PWD
echo "=== Build DMBCore support script ==="

echo "Current dir: $curDir"

if [ ! -z "$2" ]; then
	cd $2
fi

echo "First argument: ${1,,}" 

buildArg="debug"
if [ "${1,,}" == "release" ]; then
	buildArg="release"
fi

echo "run DMBCore build.py"
cd ${dmbCorePath}
python build.py ${buildArg}

echo "=== Build DMBCore support script has been finished ==="

