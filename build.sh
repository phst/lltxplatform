#!/bin/bash

set -e

rm -f built-archs.lst

build() {
    local arch="$1"
    shift
    echo "Building for architecture $arch..."
    export build_dir="$PWD/build/$arch"
    export stage_dir="$PWD/stage/$arch"
    if [[ ! -f "$build_dir/config.status" ]]
    then
        mkdir -v -p "$build_dir" "$stage_dir"
        (cd "$build_dir" && ../../configure "--prefix=$stage_dir" "$@") || return 1
    fi
    (cd "$build_dir" && make install) || return 1
    echo "$arch" >> built-archs.lst
    echo
}

cross_windows() {
    for host in i{6..3}86-mingw32{,msvc}
    do
	if which "$host-gcc" > /dev/null 2>&1
	then
	    build win32 "--build=$MACHTYPE" "--host=$host" "$@"
	    break
	fi
    done
}

cross_osx() {
    local arch="$1"
    local host="$2"
    shift 2
    build "$arch" "--build=$MACHTYPE" "--host=$host" "$@"
}

cross_linux() {
    local arch
    while IFS=';' read -r dir flag
    do
        arch=''
        if [[ "$dir" != . && -n "$flag" ]]
        then
            case "$dir" in
	        32) arch=i386-linux ;;
	        64) arch=x86_64-linux ;;
            esac
            local opt="${flag:1}"
            [[ -n "$arch" ]] &&
            gcc "-$opt" -lfontconfig -o /dev/null -x c - <<< 'int main(void) { }' 2> /dev/null &&
            build "$arch" "--build=$MACHTYPE" "--host=$arch" "CROSS_X86=${flag:1}" "$@"
        fi
    done < <(gcc -print-multi-lib)
}

native_arch="$(bash texlive-arch.sh)"

build "$native_arch" "$@"

case "$native_arch" in
    x86_64-darwin)
        cross_osx universal-darwin i386-apple-darwin9.0 --disable-dependency-tracking "$@"
	;;
    *-linux)
	cross_windows "$@"
	cross_linux "$@"
	;;
esac
