#!/bin/bash

source dependencies.sh
source deps_config.sh

download_dependency "VL" "$depsLocation" "git@github.com:skalexey/VL.git"
curentDir=${PWD}

cd $currentDir