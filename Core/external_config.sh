
THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source $THIS_DIR/os.sh

if is_windows; then
	export dmbcore_deps="${HOME}/Projects"
else
	export dmbcore_deps="${HOME}/Projects"
fi