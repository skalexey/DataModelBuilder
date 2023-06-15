#!/bin/bash

function update_cmake_modules()
{
	local THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
	cd "$THIS_DIR"

	source log.sh
	local log_prefix="[update_cmake_modules]: "

	[ $? -ne 0 ] && log_error "Error while delivering update_scripts.sh" && return 1
	source automation_config.sh
	[ $? -ne 0 ] && echo "Get automation_config.sh manually for the first time. It contains paths to dependent directories" && return 3
	source $automation_dir/templates/templates_config.sh
	[ $? -ne 0 ] && echo "Get automation_config.sh manually for the first time. It contains paths to dependent directories" && return 3
	local modules_dir=$templates_dir/CMake/Modules/
	local list=$(ls $modules_dir)
	if [ ! -d cmake_modules ]; then
		mkdir cmake_modules
	fi

	[ $? -ne 0 ] && log_error "Error while creating cmake_modules directory" && return 1
	local modules_folder=$THIS_DIR/cmake_modules
	log_info "Copying cmake modules to '$modules_folder'..."
	for file in $list; do
		log_info "Copying module '$file'"
		cp "$modules_dir/$file" $modules_folder/
	done

	[ $? -ne 0 ] && log_error "Error while delivering cmake modules" && return 1

	log_success "Done"
}

update_cmake_modules $@