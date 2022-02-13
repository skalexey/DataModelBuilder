#!/usr/bin/sh

qvlPath="../QVL/"
buildFolder="Build-cmake"

curDir=$PWD

echo "Current dir: $curDir"

if [ ! -z "$2" ]; then
	cd $2
fi

buildConfig="Debug"
if [ "${1,,}" == "release" ]; then
	buildConfig="Release"
fi

qvlBuildPath="$qvlPath$buildFolder"
rm -rf "Build-cmake/$buildConfig/QVL"
echo "Copy QVL artifacts from $qvlBuildPath/$buildConfig"
cp -r "$qvlBuildPath/$buildConfig" "Build-cmake/$buildConfig"
mv "Build-cmake/$buildConfig/$buildConfig" "Build-cmake/$buildConfig/QVL"
echo "Copy $qvlBuildPath/qvl.qmltypes"
cp "$qvlBuildPath/qvl.qmltypes" "Build-cmake/$buildConfig/QVL/"
echo "Copy $qvlBuildPath/qmldir"
cp "$qvlBuildPath/qmldir" "Build-cmake/$buildConfig/QVL/"

if [ ! -d "imports" ]; then
	echo "Create imports directory"
	mkdir imports
else
	echo "Imports directory exists"
fi

cd imports

if [ -d "QVL" ]; then
	echo "Remove imports/QVL directory"
	rm -rf QVL
fi

cd ..

echo "Copy QVL Release artifacts to imports/QVL"
cp -r "$qvlBuildPath/Release" "imports"
mv "imports/Release" "imports/QVL"
echo "Copy $qvlBuildPath/qmldir into imports/QVL"
cp "$qvlBuildPath/qmldir" "imports/QVL/"
echo "Copy $qvlBuildPath/qvl.qmltypes into imports/QVL"
cp "$qvlBuildPath/qvl.qmltypes" "imports/QVL/"

