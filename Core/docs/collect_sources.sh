#!/usr/bin/sh

rm -rf Src

includePath="../include/"
srcPath="../src/"

mkdir Src
cd Src
	cp -r "../$includePath" DMBCore
	cp -r "../$srcPath" DMBCore/src
	mkdir Extensions
	mv DMBCore/VLBackwardTraversable.h Extensions
	mkdir Extensions/src
	mv DMBCore/src/VLBackwardTraversable.cpp Extensions/src
	
	mkdir Utility
	mv DMBCore/VLUtility.h Utility
	mkdir Utility/src
	mv DMBCore/src/VLUtility.cpp Utility/src
cd ..

echo "Done"
