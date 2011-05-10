#!/bin/bash

set -e

system="$(uname -s)"
machine="$(uname -m)"

case "$system" in
    MINGW*)
        arch=win32 ;;
    Darwin)
        [[ "$(sysctl -n hw.optional.x86_64)" == 1 ]] && arch=x86_64-darwin || arch=universal-darwin ;;
    Linux)
        case "$machine" in
            x86_64)    arch=x86_64-linux ;;
            i386|i686) arch=i386-linux   ;;
        esac
        ;;
esac

if [[ -n "$arch" ]]
then
    echo "$arch"
else
    echo "Could not determine native TeX Live architecture from system type $system and machine type $machine" >&2
    exit 1
fi
