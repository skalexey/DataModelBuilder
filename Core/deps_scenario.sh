#!/bin/bash

deps_scenario()
{
	source dependencies.sh
	source deps_config.sh

	download_dependency "VL" "$depsLocation" "git@github.com:skalexey/VL.git"
	source "$depsLocation/VL/deps_scenario.sh"
}

deps_scenario $@
