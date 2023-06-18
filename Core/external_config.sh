
function job()
{
	local THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
	source $THIS_DIR/os.sh

	if is_windows; then
		export DMBCore_deps="${HOME}/Projects"
	else
		export DMBCore_deps="${HOME}/Projects"
	fi

	[ ! -z DMBCore_deps ] && export build_deps=$DMBCore_deps
}

job $@