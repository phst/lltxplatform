#!/bin/bash

set -e

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
    if [[ "$configure" == 1 ]]
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
}

native_arch="$(bash texlive-arch.sh)"

build "$native_arch" "$@"

if [[ "$native_arch" == x86_64-darwin ]]
then
    build universal-darwin --disable-dependency-tracking --build=i386-apple-darwin9.0 "$@"
fi
