#!/bin/bash

set -e

rm -f built-archs.lst

if [[ "$1" == -c ]]
then
    configure=1
    shift
else
    configure=0
fi

build() {
    local arch="$1"
    shift
    export build_dir="$PWD/build/$arch"
    export stage_dir="$PWD/stage/$arch"
    if [[ "$configure" == 1 || ! -d "$build_dir" ]]
    then
        mkdir -v -p "$build_dir" "$stage_dir"
        (
            set -e
            cd "$build_dir"
            ../../configure --prefix="$stage_dir" "$@"
        )
    fi
    (
        set -e
        cd "$build_dir"
        make install
    )
    echo "$arch" >> built-archs.lst
}

native_arch="$(bash texlive-arch.sh)"

build "$native_arch" "$@"

case "$native_arch" in
    x86_64-darwin)
        build universal-darwin --disable-dependency-tracking --host=i386-apple-darwin9.0 "$@" ;;
    x86_64-linux)
	prefix=i486-mingw32
	which "$prefix-gcc" > /dev/null && build win32 "--host=$prefix" "$@"
	build i386-linux --host=i386-linux 'CC=gcc -m32' "$@" ;;
    i386-linux)
        prefix=i586-mingw32msvc
        which "$prefix-gcc" > /dev/null && build win32 "--host=$prefix" "$@"
        build x86_64-linux --host=x86_64-linux 'CC=gcc -m64' "$@" ;;
esac
