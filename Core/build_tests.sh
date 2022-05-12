#!/usr/bin/bash

onlyRun=false

for var in "$@"
do
	if [ "$var" = "run" ] ; then
		onlyRun=true
	fi
done
if ! $onlyRun ; then
	echo "--- Build the project"
	python clean.py
	python build.py "$@"
fi
./Build/Debug/DMBTest.exe

