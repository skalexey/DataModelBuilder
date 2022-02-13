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

buildConfigFolder="$buildFolder/$buildConfig"

rm -rf "$buildConfigFolder/QVL"
echo "Copy QVL artifacts from $qvlBuildPath/$buildConfig"
cp -r "$qvlBuildPath/$buildConfig" "$buildConfigFolder"
mv "Build-cmake/$buildConfig/$buildConfig" "$buildConfigFolder/QVL"
echo "Copy $qvlBuildPath/qvl.qmltypes"
cp "$qvlBuildPath/qvl.qmltypes" "$buildConfigFolder/QVL/"
echo "Copy $qvlBuildPath/qmldir"
cp "$qvlBuildPath/qmldir" "$buildConfigFolder/QVL/"

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

