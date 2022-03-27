#!/usr/bin/sh

rm -rf Src

includePath="../include/"
srcPath="../src/"
jsonConverterPath="../dependencies/VL/JSONConverter"
jsonConverterInclude="$jsonConverterPath/include/"
jsonConverterSrc="$jsonConverterPath/src/"

mkdir Src
cd Src
	cp -r "../$includePath" DMBCore
	cp -r "../$srcPath" DMBCore/src
	mkdir Extensions
	mv DMBCore/VLBackwardTraversable.h Extensions
	mkdir Extensions/src
	cd Extensions
		mv ../DMBCore/src/VLBackwardTraversable.cpp src
		cp -r ../../$jsonConverterInclude .
		mv include JSONConverter
		cp -r ../../$jsonConverterSrc JSONConverter
	cd ..
	mkdir Utility
	mv DMBCore/VLUtility.h Utility
	mkdir Utility/src
	mv DMBCore/src/VLUtility.cpp Utility/src
cd ..

echo "Done"
